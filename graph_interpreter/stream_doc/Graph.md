[TOC]

-----------------------------------------

# Graph design template

A graph text has several sections :
- **Control of the scheduler** : debug option, location of the graph in memory
- **File paths** : to easily incorporate sections of data “included” with files
- **Formats** : most of the arcs are using the same frame length and sampling rate, to avoid repeating the same information the formats are grouped in a table and referenced by indexes
- The **IOs or boundaries of the graph** : the IOs are a kind of arcs producing or consuming a stream of data
- The **scripts** are byte-code interpreted programs used for simple operations like setting parameters, sharing debug information, calling “callbacks” predefined in the application.
- The list of **nodes** ("linked-list" of nodes), without their connexions with other nodes. This section defines also the boot parameters, the memory mapping
- The list of **arcs**, their relations with two nodes and the minimal type of debug activity on data moves



## Example of graph

The graph in text format :

```
;--------------------------------------------------------------------------
;   Stream-based processing using a graph interpreter :                    
;   
;       - The ADC detection is used to toggle a GPIO
; 
;   +----------+     +--------+      +--------+     +--------+
;   | ADC      +-----> filter +------> detect +-----> GPIO   | 
;   +----------+     +--------+      +--------+     +--------+
;                
;----------------------------------------------------------------------
format_index            0
format_frame_length     8
format_index            1
format_frame_length     16
;----------------------------------------------------------------------
stream_io               0                       ; IO0
stream_io_hwid          1                       ; io_platform_data_in_1.txt
stream_io               1                       ; IO1
stream_io_hwid          9                       ; io_platform_data_out_0.txt
;----------------------------------------------------------------------
node arm_stream_filter  0 					    ; first node 
    node_preset         1                       ; Q15 filter
    node_map_hwblock    1  5                    ; TCM = VID5
    node_parameters     0                       ; TAG = "all parameters"
        1  u8;  2                               ; Two biquads
        1  u8;  1                               ; postShift
        5 s16; 681   422   681 23853 -15161     ;  elliptic band-pass 1450..1900/16kHz
        5 s16; 681 -1342   681 26261 -15331     ; 
    end
;----------------------------------------------------------------------
node sigp_stream_detector 0     				; second node
    node_preset         3               		; detector preset 
;----------------------------------------------------------------------
;  arc connexions between IOs and node and between nodes
arc_input   0 1 0 arm_stream_filter     0 0 0  ; io0 set0copy1 fmt0     ; DETECT => OUTPUT 
arc_output  1 1 1 sigp_stream_detector  0 1 1  ; io1 set0copy1 fmt1     ; INPUT => IIR

arc arm_stream_filter 0 1 0 sigp_stream_detector 0 0 1                  ; IIR => DETECT
    arc_jitter_ctrl  1.5  ; factor to apply to the minimum size between the producer and the consumer
end
```



Platform manifest used for the mapping of IOs in the graph above 

```
; ------------------------------------------------------------------------------------------------------------
; TOP MANIFEST :
;   paths to the files
;   processors manifests (memory and architecture)
;   IO manifests to use for stream processing
;   list of the nodes installed in the platform and their affinities with processors
; ------------------------------------------------------------------------------------------------------------
; list of paths for the included files
    3                                               three file paths
    ../../stream_platform/                           "" path index 0 
    ../../stream_platform/computer/manifest/         "" path index 1
    ../../stream_nodes/                              "" path index 2
; ------------------------------------------------------------------------------------------------------------
; PLATFORM DIGITAL, MIXED-SIGNAL AND IO MANIFESTS

    1   procmap_manifest_computer.txt       path index + file name

;   path:       path ID 
;   Manifest    manifests file 
;   IO IDX      index of the IO used in the graph 
;   ProcCtrl    processor affinity bit-field 
;   ClockDomain provision for insertion of ASRC 

    10  : number of IO streams available
    
    ;Path      Manifest         IO_AL_idx ProcCtrl clock-domain     Comments               
    1   io_platform_data_in_0.txt       0     1        0            application processor  
    1   io_platform_data_in_1.txt       1     1        0            application processor  
    1   io_platform_analog_sensor_0.txt 2     1        0            ADC                    
    1   io_platform_motion_in_0.txt     3     1        0            accelero=gyro          
    1   io_platform_audio_in_0.txt      4     1        0            microphone             
    1   io_platform_2d_in_0.txt         5     1        0            camera                 
    1   io_platform_line_out_0.txt      6     1        0            audio out stereo       
    1   io_platform_gpio_out_0.txt      7     1        0            GPIO/LED               
    1   io_platform_gpio_out_1.txt      8     1        0            GPIO/PWM               
    1   io_platform_data_out_0.txt      9     1        0            application processor    
; ------------------------------------------------------------------------------------------------------------
; SOFTWARE COMPONENTS MANIFESTS 
    2                               node_manifest_none.txt               /*  0 ID0 is reserved for by-passes */
    2                       arm/script/node_manifest_script.txt          /*  1 arm_stream_script          */
    2                       arm/script/node_manifest_graph_control.txt   /*  2 arm_stream_graph_control   */
    2                       arm/router/node_manifest_router.txt          /*  3 arm_stream_router          */
    2    signal-processingFR/converter/node_manifest_converter.txt       /*  4 sigp_stream_converter      */
    2                    arm/amplifier/node_manifest_amplifier.txt       /*  5 arm_stream_amplifier       */
    2                        arm/mixer/node_manifest_mixer.txt           /*  6 arm_stream_mixer           */
    2                       arm/filter/node_manifest_filter.txt          /*  7 arm_stream_filter          */
    2     signal-processingFR/detector/node_manifest_detector.txt        /*  8 sigp_stream_detector       */
    2                     arm/rescaler/node_manifest_rescaler.txt        /*  9 arm_stream_rescaler        */
    2   signal-processingFR/compressor/node_manifest_compressor.txt      /* 10 sigp_stream_compressor     */
    2 signal-processingFR/decompressor/node_manifest_decompressor.txt    /* 11 sigp_stream_decompressor   */
    2                    arm/modulator/node_manifest_modulator.txt       /* 12 arm_stream_modulator       */
    2                  arm/demodulator/node_manifest_demodulator.txt     /* 13 arm_stream_demodulator     */
    2    signal-processingFR/resampler/node_manifest_resampler.txt       /* 14 sigp_stream_resampler      */
    2                          arm/qos/node_manifest_qos.txt             /* 15 arm_stream_qos             */
    2                        arm/split/node_manifest_split.txt           /* 16 arm_stream_split           */
    2   signal-processingFR/detector2D/node_manifest_detector2D.txt      /* 17 sigp_stream_detector2D     */
    2                     arm/filter2D/node_manifest_filter2D.txt        /* 18 arm_stream_filter2D        */
    2                     arm/analysis/node_manifest_analysis.txt        /* 19 arm_stream_analysis        */
    2                  bitbank/JPEGENC/node_manifest_bitbank_JPEGENC.txt /* 20 JPG encoder                */
    2                 elm-lang/TJpgDec/node_manifest_TjpgDec.txt         /* 21 JPG decoder                */
    2             arm/format_converter/node_manifest_format_converter.txt /* 22 arm_stream_format_converter*/
; ------------------------------------------------------------------------------------------------------------
```
The compiled result which will be the input file of the interpreter:
```
//--------------------------------------
//  DATE Thu Sep 19 19:49:37 2024
//  AUTOMATICALLY GENERATED CODES
//  DO NOT MODIFY !
//--------------------------------------
0x0000003C, // ------- Graph size = Flash=36[W]+RAM24[W]  +Buffers=48[B] 12[W] 
0x00000000, // 000 000 [0] Destination in RAM 0, and RAM split 0 
0x00000042, // 004 001 [1] Number of IOs 2, Formats 2, Scripts 0 
0x00000015, // 008 002 LinkedList size = 21, ongoing IO bytes, Arc debug table size 0 
0x00000003, // 00C 003 [3] Nb arcs 3  SchedCtrl 0 ScriptCtrl 0   
0x00000001, // 010 004 [4] Processors allowed 
0x00000000, // 014 005 [5] memory consumed 0,1,2,3 
0x00000000, // 018 006 [6] memory consumed 4,5,6,7 ...  
0x00083000, // 01C 007 IO(graph0) 1 arc 0 set0copy1=1 rx0tx1=0 servant1 1 shared 0 domain 0 
0x00000000, // 020 008 IO(settings 0, fmtProd 0 (L=8) fmtCons 0 (L=8) 
0x00000000, // 024 009  
0x00000000, // 028 00A  
0x00483801, // 02C 00B IO(graph1) 9 arc 1 set0copy1=1 rx0tx1=1 servant1 1 shared 0 domain 0 
0x00000000, // 030 00C IO(settings 0, fmtProd 1 (L=16) fmtCons 1 (L=16) 
0x00000000, // 034 00D  
0x00000000, // 038 00E  
0x00C04807, // 03C 00F -----  arm_stream_filter(0) idx:7 Nrx 1 Ntx 1 ArcFmt 1 lockArc 1 
0x08020000, // 040 010 ARC 0 Rx0Tx1 0 L=8 dbgpage0 -- ARC 2 Rx0Tx1 1 L=16 dbgpage0     
0x30000048, // 044 011 Nb Memreq-1 1  XDM11_same_rate 1 
0x0000004C, // 048 012 Reserved static memory bank(0) = bank 0 stat0work1ret2 = 0 size 76  
0x30800000, // 04C 013  
0x00000034, // 050 014 Scratch memory bank(1) = bank 2 stat0work1ret2 = 1  size 52  
0x01000007, // 054 015 ParamLen 6+1 Preset 1 Tag0ALL 0 
0x02A90102, // 058 016 (0) 
0x02A901A6, // 05C 017 (1) 
0xC4C75D2D, // 060 018 (2) 
0xFAC202A9, // 064 019 (3) 
0x669502A9, // 068 01A (4) 
0x0000C41D, // 06C 01B (5) 
0x00C04808, // 070 01C -----  sigp_stream_detector(0) idx:8 Nrx 1 Ntx 1 ArcFmt 1 lockArc 1 
0x08010002, // 074 01D ARC 2 Rx0Tx1 0 L=8 dbgpage0 -- ARC 1 Rx0Tx1 1 L=16 dbgpage0     
0x30000058, // 078 01E Nb Memreq-1 1  XDM11_same_rate 1 
0x00000034, // 07C 01F Reserved static memory bank(0) = bank 0 stat0work1ret2 = 0 size 52  
0x30000064, // 080 020  
0x00000020, // 084 021 Reserved static memory bank(1) = bank 0 stat0work1ret2 = 2 size 32  
0x03000001, // 088 022 ParamLen 0+1 Preset 3 Tag0ALL 0 
0x000003FF, // 08C 023 ^^^^^^^^^ LINKED-LIST END ^^^^^^^^^ vvvvvvvvvvv RAM vvvvvvvvvvv 
0xFFFF7F7F, // 090 024 LinkedList size = 21, ongoing IO bytes, Arc debug table size 0 
0x00000008, // 094 025 Format  0 frameSize 8  
0x00004400, // 098 026           nchan 1 raw 17 
0x00000000, // 09C 027           domain-dependent 
0x00000000, // 0A0 028           domain-dependent 
0x00000010, // 0A4 029 Format  1 frameSize 16  
0x00004400, // 0A8 02A           nchan 1 raw 17 
0x00000000, // 0AC 02B           domain-dependent 
0x00000000, // 0B0 02C           domain-dependent 
0x0000003C, // 0B4 02D IO-ARC descriptor(0) Base 3Ch (Fh words) fmtProd_0 frameL 8.0 
0x00000008, // 0B8 02E     Size 8h[B] fmtCons_0 FrameL 8.0 jitterScaling 1.0 
0x00000000, // 0BC 02F  
0x00000000, // 0C0 030  
0x00000000, // 0C4 031     fmtCons 0 fmtProd 0 dbgreg 0 dbgcmd 0 
0x0000003E, // 0C8 032 IO-ARC descriptor(1) Base 3Eh (Fh words) fmtProd_1 frameL 16.0 
0x00000010, // 0CC 033     Size 10h[B] fmtCons_1 FrameL 16.0 jitterScaling 1.0 
0x00000000, // 0D0 034  
0x00000000, // 0D4 035  
0x00000101, // 0D8 036     fmtCons 1 fmtProd 1 dbgreg 0 dbgcmd 0 
0x00000042, // 0DC 037 ARC descriptor(2) Base 42h (10h words) fmtProd_0 frameL 8.0 
0x00000018, // 0E0 038     Size 18h[B] fmtCons_1 FrameL 16.0 jitterScaling 1.5 
0x00000000, // 0E4 039  
0x00000000, // 0E8 03A  
0x00000100, // 0EC 03B     fmtCons 1 fmtProd 0 dbgreg 0 dbgcmd 0 
```


## Control of the scheduler
The first words of the binary graph give the portion of the graph to move to RAM.
To have addresses portability of addresses between processors, the graph interpreter is managing a list of "memory-offsets".
Every physical address is computed from a 28 bit-field structure made of : 6 bits used to select maximum 64 memory-offsets (or memory bank). And a 22bits field used as an index in this memory bank.
The function "platform_init_stream_instance()" initializes the interpreter memory-offset table.

### graph_location  "x"
Parameter x :
- 0: (default) the destination of the binary graph is fully in RAM address corresponding to the first memory-offset
- 1: the graph stays in Flash and the static portion is copied in RAM is starting from the end of nodes linked-list
- 2: the graph is provided by the application, already in RAM, there is nothing to do

### graph_location_offset "x"
The parameter (default 0) gives the binary graph destination offset from the memory-offset #0 

### debug_script_fields "x"
The parameter is a bit-field of flags controlling the scheduler loop :
- bit 0 (lsb 1) set means "call the debug/trace script before each node is called"
- bit 1 (2) set means "call the debug script after each node is called"
- bit 2 (4) set means "call the debug script at the end of the loop"
- bit 3 (8) set means "call the debug script when starting the graph scheduling"
- bit 4 (16) set means "call the debug script when returning from the graph scheduling"
- no bit is set (default) the debug script is not called 

### scheduler_return "x"
- 1: return to application caller subroutines after each node execution calls
- 2: return to caller once all node of the graph are parsed
- 3: return to caller when all nodes are starving (default 3)

### allowed_processors "x"
bit-field of the processors allowed to execute this graph, (default = 1 main processor)

### set_file_path "index"  "path"
Example `set_file_path 2 ./   ; file path index 2 is the current folder`
Index and its file path, used when including files (sub graphs, parameter files and scripts)

### graph_memory_bank "x"
Command used in the context of memory mapping tuning.
Example `graph_memory_bank 1   ; selection of memory bank 1`
"x" : index of the memory bank indexes where to map the graph (default 0).

-----------------------------------------

## IO control and stream data formats
There are three data declared in the graph scheduler instance (*arm_stream_instance_t*):
A - a pointer to a RAM area giving 

   - on-going transfer flag

B - a pointer to the list of IOs bit-fields controlling the setting of the IO, the content of which depends on the *Domain*:
   - the index of the arc creating the interface between the IO and a node of the graph ("arcID")
   - the Rx/Tx direction of the stream, from the point of view of the graph
   - the dynamic behavior : data polling initiated by the scheduler or transfer initiated outside of the graph
   - flag telling if the data are copied in the arc's buffer or if the arc's descriptor is modified to point directly to the data
   - flag telling if the buffer used by the IO interface must be reserved by the graph compiler 
   - physical Domain of the data (see command "format_domain")
   - index to the Abstraction Layer in charge of operating the transfers
   - for audio  (mixed-signal setting, gains, sampling-rate, ..)

C - a pointer to the "Formats" which are structures of four words giving :
 - word 0 : frame size 4MB (Byte accurate) 
 - word 1 : number of channels (1..32), interleaving scheme, time-stamp, raw format, domain, sub-type, frame size extension (up to 64GB +/-16kB)
 - word 2 : sampling rate in [Hz], truncated IEEE FP32 on 24bits : S_E8_M15 
 - word 3 : specific to each domain (audio and motion channel mapping, image format and border)


### format "n"            
This command starts the declaration of a new format.
example `format_raw_data 2 ; all further details are for format index 2`
index used to start the declaration of a new format

### format_raw_data "n"
The parameter is the raw data code of the table below.
Example `format_raw_data 17 ; raw data is "signed integers of 16bits" `
The default index is 17 : STREAM_16 (see Annexe "Data Types").

### format_frame_length "n"          
Frame length in number of bytes of the current format declaration (default :1)
Example `format_frame_length 160 `

### format_nbchan "n"
Number of channels in the stream (default 1)
Example `format_nbchan 2 ; stereo format ` 

### format_interleaving "n" 
Example `format_interleaving 0 `
0 means interleaved raw data, 1 means deinterleaved data by packets of "frame size"

### format_time_stamp "n"
Example `format_time_stamp 40 ; time-stamp format TIME16D `
time-stamp format :
- 0: no time stamp
- 1: simple counter
- 39: STREAM_TIME16 format q14.2 in seconds, maximum range 1 hour + 8mn +/- 0.0625
- 40: STREAM_TIME16D format q15 seconds, for time differences +/- 15us 
- 41: STREAM_TIME32 format q28.4 seconds, maximum 8.5 years +/- 0.0625s
- 42: STREAM_TIME32D format q17.15 seconds, maximum 36hours +/- 30us for time differences
- 43: STREAM_TIME64 format q32.28 seconds, maximum 140 years +/- 4ns
- 44: STREAM_TIME64MS format u42 in milliseconds, amximum 140 years
- 45: STREAM_TIME64ISO ISO8601 with signed offset, example 2024-05-04T21:12:02+07:00

### format_domain "n"
Usage context of this command is for the section "B" of above chapter "IO control and stream data formats".
Example `format_domain 2   ; this format uses specific details of audio out domain `
 | DOMAIN | CODE | COMMENTS |
 | --------- | -- | ---------- |
 | GENERAL | 0 | (a)synchronous sensor, electrical, chemical, color, remote data, compressed streams, JSON, SensorThings, application processor | 
 | AUDIO_IN | 1 | microphone, line-in, I2S, PDM RX | 
 | AUDIO_OUT | 2 | line-out, earphone / speaker, PDM TX, I2S, | 
 | MOTION | 5 | accelerometer, combined or not with pressure and gyroscope audio_in microphone, line-in, I2S, PDM RX | 
 | 2D_IN | 6 | camera sensor audio_out line-out, earphone / speaker, PDM TX, I2S, | 
 | 2D_OUT | 7 | display, led matrix, gpio_in generic digital IO  | 


### Information specific of domains 
Word 3 of "Formats" holds specific information of each domain.

#### Audio
Audio channel mapping is encoded on 20 bits. For example a stereo channel holding "Back Left" and "Back Right" will be encoded as 0x0030.
 | Channel name    | Name | Bit | 
 | ----            | -- | -- | 
 |  Front Left               | FL    | 0    |
 |  Front Right              | FR    |    1    |
 |  Front Center             | FC    |    2    |
 |  Low Frequency            | LFE   |    3    |
 |  Back Left                | BL    |    4    |
 |  Back Right               | BR    |    5    |
 |  Front Left of Center     | FLC   |    6    |
 |  Front Right of Center    | FRC   |    7    |
 |  Back Center              | BC    |    8    |
 |  Side Left                | SL    |    9    |
 |  Side Right               | SR    |    10   |
 |  Top Center               | TC    |    11   |
 |  Front Left Height        | TFL   |    12   |
 |  Front Center Height      | TFC   |    13   |
 |  Front Right Height       | TFR   |    14   |
 |  Rear Left Height         | TBL   |    15   |
 |  Rear Center Height       | TBC   |    16   |
 |  Rear Right Height        | TBR   |    17   |
 |  Channel 19        | C19   |    18   |
 |  Channel 20        | C20   |    19   |

#### Motion
Motion sensor channel mapping (w/wo the temperature)
 | Motion sensor data    | Code |  
 | ----            | -- | 
 |  only acceleromete   | 1  |
 |  only gyroscope    | 2  |
 |  only magnetometer   | 3  |
 |  A + G    | 4  |
 |  A + M    | 5  |
 |  G + M    | 6  |
 |  A + G + M    | 7  |

#### 2D
Format of the images in pixels: height, width, border.

-----------------------------------------

## Interfaces of the graph

### stream_io "n" 
This command starts a section for the declaration of IO "n". The parameter is the interface index used in the graph. This declaration starts the definition of a new IO 
Example 
```
    stream_io 2 
```

### stream_io_hwid "ID"
The stream_io is using the ID of the physical interface given in platform manifests (default #0)
Example 
```
    stream_io_hwid 2 
```

### stream_io_format "n" 
Parameter: index to the table of formats (default #0)
Example 
```
    stream_io_format 0 
```

### stream_io_setting "W32 W32 W32"
"IO settings" is a specific bit-field structure, specific to the IO domain, placed at the beginning of the binary graph, and used during the initialization sequence of the graph.
Up to three control words in hexadecimal can be used.
Example 
```
    stream_io_setting 7812440 
```

### stream_io_setting_callback "cb" "X" 
The function "platform_init_stream_instance()" initializes the interpreter pointers to the callbacks proposed by the platform. 
Example 
```
    stream_io_setting_callback 6 7812440 ; Use callback 6 for the setting of the  
                                         ; current stream_io using parameter 7812440 
```
-----------------------------------------

## Memory mapping (TBD)
Split the memory mapping to ease memory overlays between nodes and arcs by defining new memory-offset index ("ID").
Format : ID, new ID to use in the node/arc declarations, byte offset within the original ID, length of the new memory offset.
```
;               original_id  new_id    start   length 
; memory_mapping      2        100      1024    32700 
```

### Memory fill
Filling pattern placed after the arc descriptors 
@@@@ TODO insert a picture of the graph memory map 
```
    mem_fill_pattern 5 3355AAFF   memory fill 5 word32 value 0x3355AAFF (total 20 Bytes)
```

-----------------------------------------

## Subgraphs (TBD)
A subgraph is equalivalent to program subroutines for graphs. A subgraph can be reused in several places in the graph or in other subgraph.
The graph compiler creates references by name mangling from the call hierarchy. A subgraph receives indexes of IO streams and memory bank indexes for tuning the memory map.
The caller gives its indexes of the arcs to use in the subgraph, and the memory mapping offset indexes. 
Example :
```
    subgraph 
       sub1                        ; subgraph name, used for name mangling 
       3 sub_graph_0.txt           ; path and file name 
       5 i16: 0 1 2 3 4            ; 5 streaming interfaces data_in_0, data_out_0 ..  
       3 i16: 0 0 0                ; 3 partitions for fast/slow/working (identical here) 
```
-----------------------------------------

## Nodes declarations
Nodes are declared with their name and respective instance index in the graph (or subgraph). 
The system integrator can set a "preset" (pre-tuned list of parameters described on node's documentation) and node-specific parameters to load at boot-time.
The address offset of the nodes is provided as a result of the graph compilation step.
Declaration syntax example : 
```
    node arm_stream_filter  0  ; first instance of the nore "arm_stream_filter" 
```

### node_preset "n"
The system intgrator can select 16 "presets" when using a node, each corresponding to a configuration of the node (see its documentation). 
The Preset value is with RESET and SET_PARAMETER commands, the default value is 0.
Example : 
```
    node_preset              1      ; parameter preset used at boot time 
```

### node_malloc_add "n" "i"
A node memory allocation is described in its manifest. 
A node can ask for up to 6 memory banks with tunable fields : 
- type (static, working, static with periodic backup)
- speed (normal, fast, critical fast)
- relocatable (the location can change after the node was notified)
- program / data 
- size in bytes

The size can be a simple number of bytes or a computed number coupled to a function of stream format parameters (number of channels, sampling rate, frame size) and a flexible parameter defined in the graph, here.
The total memory allocation size in bytes = 
```
     A                             fixed memory allocation in Bytes (default 0)
   + B x nb_channels of arc(i)     number of channels in arc index i (default 0)
   + C x sampling_rate of arc(j)   sampling rate of arc index j (default 0)
   + D x frame_size of arc(k)      frame size used for the arc index k (default 0)
   + parameter from the graph      optional field "node_malloc_add"
```

For example an extra scratch area allocation can added as a function of the number of pixels in the images to process (default 0).
Example : 
```
    node_malloc_add 12 0  ; add 12 bytes to segment 0
```

### node_map_hwblock "m" "o"
This command is used to tune the memory mapping and bypass the speed requirement of the node manifest. It tells to force the memory segment index given in the first parameter to be mapped to the memory offset index of the second parameter.
Example : 
```
    node_map_hwblock 0 2 ; memory segment 0 is mapped to bank offset 2 
```

### node_map_copy / node_map_swap "m" "o"
This command is used to optimize the memory mapping of small and fast memory segment by copying, or swapping, a memory segment content from and other memory offset (usually a slower one).
Usage : 
```
    node_map_copy 1 0; forced copy of the indexed node memory segment 1 to hardware memory offset 0  
    node_map_swap 1 0; forced swap of the indexed node memory segment 1 to hardware memory offset 0 
```
In the above both cases the memory segment 1 is copied from offset memory segment 1 (a dummy arc descriptor is created to access this temporary area). 
In the second swap case the scheduler reads node memory segment and updates the slow memory.

### node_trace_id "io"
Selection of the graph IO interface used for sending the debug and trace informations.
Example : 
```  
    node_trace_id  0      ; IO port 0 is used to send the trace 
```

### node_map_proc
### node_map_arch
### node_map_rtos
The graph can be executed in a multiprocessor and multi tasks platform. Those commands allow the graph interpreter scheduler to skip the nodes not associated to the current processor / architecture and task.
The platform can define 7 architectures and 7 processors. When the parameter is not defined (or with value 0) the scheduler interprets it as "any processor" or "any architecture" can execute this node.
Several OS threads can interpret the graph at the same time. A parameter "0" means any thread can execute this node, and by convention the value "1" is allocated to low-latency tasks, "3" to background tasks. 
Examples :
```
    node_map_proc 2 ; run this node on processor 2 defined in the architecture manifest 
    node_map_arch 1 ; run this node on architecture 1 defined in the architecture manifest 
    node_map_rtos 1 ; run this node on thread index 1 (low-latency) 
```

### node_map_verbose "level"
The default verbose level on the debug trace of this node is level "0", meaning "quiet". Other values from 1 to 3 tell to trace more information.
Example:
```
    node_map_verbose 3 ; highest level of verbose debug trace
```

### node_memory_isolation  "0/1"
Activate (parameter "1") the processor memory protection unit (on code, private memory allocated segments, and stack) during the execution of this node. 
Example : 

```
   node_memory_isolation 1 ; activation of the memory protection unit (MPU), default 0 
```

### node_memory_clear "m"

Debug and security feature: Clear the memory bank "m" before and after the execution of the node.  
Example : 
```
   node_memory_clear 2 ; clear the memory bank 2 as seen in the manifest before and after execution 
```

### node_script "index"
The indexed script is executed before and after the node execution. The conditional is set on the first call and cleared on the second call.
Example :

```
  node_script 12 ; call script #12 associated to this node
```

### node_parameters "tag"
This command declares the parameters to share with the node during the RESET sequence. If the "tag" parameter is null is tells the following parameters is a full set. Otherwise it is an index of a subset defined in the node documentation.
The following declaration is a list of data terminated with the "end".
Example of a packed structure of 22 bytes of parameters:

```
     node_parameters     0                   TAG = "all parameters" 
         1  u8;  2                           Two biquads 
         1  u8;  1                           postShift 
         5 s16; 681   422   681 23853 -15161  elliptic band-pass 1450..1900/16kHz 
         5 s16; 681 -1342   681 26261 -15331 
     end 
```

-----------------------------------------

## Scripts 
Scripts are small interpreted byte-codes designed for control and calls to the graph scheduler for node control and parameter settings. Scripts are declared as standard nodes with extra parameters memory size and allowing it to be reused for several scripts. The nodes have arc connected as transmit streams. The arc descriptor points to a buffer used as working area (registers of the virtual machine, stack and heap memory). 

The virtual engine has 20 instructions. There are 12 registers, 2 indexes to the stack and the stack are all 64 bits wide. All the instructions can be executed conditionally. There two main families of instructions :

**Load** registers with arithmetic's operation have a similar syntax as test.

```
r6 = 3	       			; r6 = 3  (the default litterals type is int32)
r6 = add r5 3	        ; r6 = ( r5 + 3 )
r6 = sub r5 r4	        ; r6 = ( r5 - r4 )
```

**Tests**, the syntax is : {test type) {register to compare}  {optional arithmetic operator} {register 2} {register 3 or a constants}. Examples

```
testlt r6 3	       	    ; test r6 < 3
testlt r6 add r5 3	    ; test r6 < ( r5 + 3 )
testlt r6 sub r5 r4	    ; test r6 < ( r5 - r4 )
if_yes r6 = #float 3	; conditional load of r6 with 3.0
```

The last family are a controls (circular addressing, jumps, calls, loops, bit-field extraction, scatter/gather load). Examples

```
Example of instructions		Comments
-----------------------     --------
if_no set r2 type #float 	conditional set of the type of r2 to float
set r4 base r5 				set the base address of circular buffer
set r4 size r5 				set the size of the circular buffer
st r2 [r4] r3 				scatter save with indexes r2[r4] = r3
ld r2 r3 [r4] 				gather addressing r2 = r3[r4]
move r2 |len pos| r3 	    store r3 to a bit-field of r2
move r2 r3 |len pos| 		save in r2 a bit-field from r3
swap r2 r3 					swap two registers
delete 4 					remove the last 4 registers from the stack
save r3 r0 r11 				push 3 registers on the stack
restore r3 r0 r11 			restore 3 registers from the stack
jump L_label r1				jump to label and save r1 on the stack
banz L_Label r0 			decrement r0 and branch if not zero
call L_Label r1 r2 r3		call a subroutine and save two registers
callsys 17 r1 r2 r3 	    call a system suboutine and save 3 parameters
return                    	return from subroutine
```

The graph declares the script like standard nodes :

```
node arm_stream_script 1  ; script (instance) index           
    script_stack      12  ; size of the stack in word64      
    script_mem_shared  1  ; default is private memory (0) or shared (1)  
    script_mem_map     0  ; mapping of the working memory to VID #0 (default)      
 
    script_code       
	...
	return				  ; return to the graph scheduler
    end                                                      
    node_parameters <ID2>                                    
       include 1 binary_code.txt ; path ID and file name     
    end                                                     
```

-----------------------------------------

## ARC of the graph
The syntax is different for arcs connected to the boundary of the graph, and arcs placed between two nodes.
Depending on real-time behaviors (CPU load and jitter, task priorities, speed of data streams) the data can be processed in-place (large input images for examples) or it can be mandatory to copy the data in temporary FIFO before being processed in the graph.
The parameter "set0copy1" is set to 0 (default value) for a processing made "in-place" : the base address the IO FIFO descriptor is modified in the IO transfer acknowledgment subroutine arm_graph_interpreter_io_ack() to point directly to the IO data.
When the parameter is 1 the data is copied in the FIFO, and the graph compiler will allocate an amount of memory corresponding to a "format_frame_length".

Example :
```
; Syntax :
; arc_input   { io / set0copy1 / fmtProd } + { node / inst / arc / fmtCons }
; arc_output  { io / set0copy1 / fmtCons } + { node / inst / arc / fmtProd }
; arc  { node1 / inst / arc / fmtProd } + { node2 / inst / arc / fmtCons }

 arc_input 4 1 0    xxfilter 6 0 8                                    
                                                                      
 output arc from node xxdetector instance 5 output #1 using format #2 
            to graph IO 7 using set0copy1=0 and format #9             
 arc_output 5 1 2   xxdetector 7 0 9                                   
                                                                      
 arc between nodeAAA instance 1 output #2 using format #0             
     and nodeBBB instance 3 output #4 using format #1                 
 arc nodeAAA 1 2 0   nodeBBB 3 4 1                                    
```

### arc flow control 
Example
``` 
arc_flow_error 1  ; #1 do something depending on the IO domain when a flow error occurs, default #0 (no extra processing, just skip data) 
```

### arc debug
Each arc descriptor can be configured to have an operation (in a list of 32) implemented with result returned in a dedicated memory section of the graph.
| CODE | DEBUG OPERATION |
| --------- | :---------- |
| 0   | no operation                                                                        |
| 1   | increment DEBUG_REG_ARCW1 with the number of RAW samples                            |
| 2   | set a 0 in to *DEBUG_REG_ARCW1, 5 MSB gives the bit to clear                        |
| 3   | set a 1 in to *DEBUG_REG_ARCW1, 5 MSB gives the bit to set                          |
| 4   | increment *DEBUG_REG_ARCW1                                                          |
| 5   |                                                                                     |
| 6   | call-back in the application side, data rate estimate in DEBUG_REG_ARCW1            |
| 7   | second call-back : wake-up processor from DEBUG_REG_ARCW1=[ProcID, command]         |
| 8   |                                                                                     |
| 9   | time_stamp_last_access                                                              |
| 10  | peak with forgeting factor 1/256 in DEBUG_REG_ARCW1                                 |
| 11  | mean with forgeting factor 1/256 in DEBUG_REG_ARCW1                                 |
| 12  | min with forgeting factor 1/256 in DEBUG_REG_ARCW1                                  |
| 13  | absmin with forgeting factor 1/256 in DEBUG_REG_ARCW1                               |
| 14  | when data is changing the new data is push to another arc DEBUG_REG_ARCW1=[ArcID]   |
| 15  | automatic rewind read/write                                                         |

Example :
```
    arc_debug_cmd  1  debug action "ARC_INCREMENT_REG"         
    arc_debug_reg  3  index of the 64bits result, default = #0  
    arc_debug_page 0  page of 32 words / page, default = #0    
```

### arc_flush
```
    arc_flush  0 ; forced flush of data in MProcessing and shared tasks
```

### arc_map_hwblock
```
     arc_map_hwblock   0  map the buffer to a memory offset, default = #0 (VID0)
```

### arc_jitter_ctrl
Command used during the compilation step for the FIFO buffer memory allocation with some margin.
```
    arc_jitter_ctrl  1.5  ; factor to apply to the minimum size between the producer and the consumer, default = 1.0 (no jitter) 
```

### arc_parameters
Arcs are used to node parameters when the inlined way (with the node declaration) is limited to 256kBytes. The node manifest declares the number of arcs used for large amount of parameters (NN model, video file, etc ..).
```
    arc_parameters  0       ; (parameter arcs) buffer preloading, or arc descriptor set with script 
       7  i8; 2 3 4 5 6 7 8 ; parameters                                                            
    include 1 filter_parameters.txt ; path + text file-name using parameter syntax                    
    end                                                                                              
```
-----------------------------------------

