[TOC]
--------------------------------------
# Introduction 

The Graph-Interpreter is scheduling a linked-list of computing nodes using arcs. Nodes descriptors tell which processor can execute the code, which arc it is connected to. The arcs descriptors tell the base address of the buffers, read/write indexes, debug/trace information to log and a flag to tell the consumer node to wrap data to the base addresses. The buffers base address are portable using 6-bits "offset" and 22-bits "index". The offset is translated by each graph interpreter instance of each processor in a physical address given in the Platform-Manifest. 

The graph is placed in a shared memory for all processors, there is no message passing scheme, the Graph-Interpreter scheduler’s instances and doing the same estimations in parallel, deciding which node needs to be executed in priority. 

The application is connected to the boundary of the graph, also looking to the same graph structure, the data moves are identical with hardware peripherals and the data flow is an "IO" of the graph. When data is exchanged with the graph (from the application or a DMA device driver) and API 

## **Binary format of the graph** 

| Graph section name                                           | Description                                                  |
| ------------------------------------------------------------ | :----------------------------------------------------------- |
| ------ next sections can either be in RAM or Flash           |                                                              |
| Header (7 words)                                             | The header tells where the graph will be in RAM the size of the following sections, the processors allowed to use it, the percentage of memory consumed in the memory banks |
| IO description and links to the device-driver abstraction (4 words/IO) | Each IO descriptor tell if data will be copied in the arc buffers or if the arc descriptor will be set to point directly to the data. |
| Scripts byte code and parameters                             | Scripts are made to update parameters, interface with the application's callbacks, implement simple state-machines, interface with the IOs of the graph |
| List of Nodes instance and their parameters to use at reset time | This section is the translation of the node manifests with additional information from the graph : memory mapping of the node data banks and parameters (preset and specific paremeters) |
| ------ graph sections in RAM area starts here                |                                                              |
| List of flags telling if data requests are on-going on the IOs (1 byte/IO) | The flags "on-going" are set by the scheduler and reset upon data transfer completion |
| List of debug/trace registers used by arcs (2 words/debug register) | Basic programmable data stream analysis (time of last access, average values, estimated data rate) |
| List of Formats, max 256, 4 words/format                     | Frame length, number of channels, interleaving scheme, specific data of the domain |
| List of arc descriptors (5 words/arc)                        | Base address in the portable format (6bits offset 22bits index in words), read/write indexes with Byte accuracy. The descriptor has an "extension" factor to scale all parameter up to 64GB addressing space. |

## **Reset sequence**

Each Node is define by an index on 10 bits and a "synchronization Byte" with 3-bits defining the architecture it made for (from 1 to 7, "0" means any architecture), 3-Bits defining the processor index within this architecture (from 1 to 7, "0" means any processor), and 2-bits for thread instance ("0" means any thread, "1, 2, 3" respectively for low-latency, normal latency, and background tasks. At reset time processor 1, of architecture 1 is allowed to copy the graph from Flash to RAM and unlock the others.

Then each processor parses the graph looking nodes associated to him, resets it and updates the parameters from graph data. When all the nodes have been set the application is notified and the graph switches to "run" mode. Each graph scheduler instance takes care input and output streams are not blocked : each IOs is associated to a processor. Most of the time a single processor is in charge of all.

The multiprocessor synchronization mechanisms are abstracted outside of the graph interpreter (in the platform abstraction layer), a software-based lock is proposed by default.



# TOP MANIFEST



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
    ../../stream_platform/                           "" path index 0 is local
    ../../stream_platform/computer/manifest/         "" path index 1
    ../../stream_nodes/                              "" path index 2

; ------------------------------------------------------------------------------------------------------------
; PLATFORM DIGITAL, MIXED-SIGNAL AND IO MANIFESTS - max 32 IOs => iomask

    1   procmap_manifest_computer.txt       path index + file name

;   path:       path ID 
;   Manifest    manifests file 
;   FW IO IDX   index used in the graph 
;   ProcCtrl    processor affinity bit-field 
;   ClockDomain provision for ASRC (clock-domain)
;       some IO can be alternatively clocked from the system clock (0) or other ones. The system integrator decides
;       with this field to manage the flow errors with buffer interpolation (0) or ASRC (other clock domain index)
;       The clock domain index is just helping to group and synchronize the data flow per domain.

    10  number of IO streams available      aligned with struct platform_io_control platform_io[] and platform_computer.h 

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

    ;  p_stream_node node_entry_point_table[NB_NODE_ENTRY_POINTS] =
    2                               node_manifest_none.txt            /*  0 ID0 is reserved for by-passes */
    2                        arm/script/node_manifest_script.txt         /*  1 arm_stream_script          */
    2            arm/format_converter/node_manifest_format_converter.txt /*  2 arm_stream_format_converter*/
    2                        arm/router/node_manifest_router.txt         /*  3 arm_stream_router          */
    2     signal-processingFR/converter/node_manifest_converter.txt      /*  4 sigp_stream_converter      */
    2                     arm/amplifier/node_manifest_amplifier.txt      /*  5 arm_stream_amplifier       */
    2                         arm/mixer/node_manifest_mixer.txt          /*  6 arm_stream_mixer           */
    2                        arm/filter/node_manifest_filter.txt         /*  7 arm_stream_filter          */
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
    2                         elm-lang/TJpgDec/node_manifest_TjpgDec.txt /* 21 JPG decoder                */

; ------------------------------------------------------------------------------------------------------------



```



# Digital Manifest



```
; ------------------------------------------------------------------------------------------------------------
; Processor and memory configuration + default memory mapping 
; ------------------------------------------------------------------------------------------------------------

    1 1 9 4 number of architectures, processors, memory banks, offsets

;   memory banks:
;   - ID        base offset ID reference above
;   - VID       virtual ID used in the graph for manual mapping, 
;   - S         0=any/1=normal/2=fast/3=critical-Fast, 
;   - W         static0/working1/retention2
;   - P         shared0/private1,
;   - H         DMAmemHW1
;   - D         Data0/Prog1/Both2 
;   - Size      minimum sizes guaranteed per VID starting from @[ID]+offset below
;   - Offset    maximum offset from the base offset ID,

;   the memory is further split in the graph "top_memory_mapping" to ease mapping and overlays

;   ID VID  S W P H D     Size offset from offsetID 
    0   0   1 0 0 0 0    95526 10       VID0=DEFAULT flat memory bank, can overlap with others
    0   1   0 0 0 0 0    65526 10       SRAM0 static, hand tuned memory banks
    0   2   0 0 0 0 0    30000 65536    SRAM1 static
    0   3   0 1 0 0 0    15000 95536    SRAM1 working at application level
    0   4   0 1 0 0 0   256000 262144   DDR working at application level
    2   5   3 1 1 0 0     1024 262144   DTCM Private memory of processor 1 
    1  10   0 2 0 0 0     1024 524288   Retention memory
    3  20   0 0 0 0 0   200000 10       Data in Flash
    2   8   3 1 1 0 1    16384 0        ITCM Private memory of processor 1 
  

;   memory offsets ID used by all processors and physical address seen from the main processor
;   0       h20000000   image of "platform_specific_long_offset(intPtr_t long_offset[])" 
;   1       h28000000       in stream_al/platform_XXXXX.c
;   2       h2C000000   TCM Private memory of processor 1 
;   3       h08000000   Internal Flash   

;----------------------------------------------------------------------------------------
; all architectures
    ;   all processors (processor IDs >0)
        ;------------------------------------------------------------
        1 1 15      processor ID, boolean "I am the main processor" allowed to boot the graphs
        ;           Bit-field computation firmware extensions, 


```

