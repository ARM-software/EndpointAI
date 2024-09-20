[TOC]
--------------------------------------
# Node manifest template
A node manifest file gives the name of the software component, its author, the targeted architecture, the description of input and output streams connected to it. 

The graph interpreter allocates a predefined amount of memory and this file explains the way to compute the memory allocation. When the first non-blank character is a semi-colon ";" the entire line is skipped as a comment 

## Declaration of "options"

The manifest give the list of compatible options possible. Some options are described as a **list**, or as a **range** of values. </br></br>
The syntax is : an index and the list of numbers within brackets "{" and "}". The index gives the default value to consider in the list. Index "1" corresponds to the first element of the list.
Index value "0" means "any value". The list can be empty in that case.

Example of an option list between five values, the index is 2 meaning the default value is the second in the list (value = 6).

```
{ 2   5 6 7 8 9 }
```

When the index is negative the list is decoded as a "range". A Range is a set of three numbers : 

- the first option
- the step to the next possible option
- the last (included) option

The absolute index value selects the default value in this range.

Example of is an option list of values (1, 1.2, 1.4, 1.6, 1.8, .. , 4.2), the index is -3 meaning the default value is the third in the list (value = 1.4).

```
 { -3   1    0.2   4.2 } 
```

## Example of node manifest

```
; ------------------------------------------------------------------------------------------------------------
; SOFTWARE COMPONENT MANIFEST - "arm_stream_filter"
; ------------------------------------------------------------------------------------------------------------
;
node_developer_name   ARM                    ; developer name
node_name             arm_stream_filter      ; node name

node_using_arc_format  1           ; to let filter manage q15 and fp32
node_mask_library     64           ; dependency with DSP services

;----------------------------------------------------------------------------------------
;   MEMORY ALLOCATIONS

node_mem                0		    ; first memory bank (node instance)
node_mem_alloc         76			; amount of bytes

node_mem                1			; second memory bank (node fast working area)
node_mem_alloc         52           ;
node_mem_type           1           ; working memory
node_mem_speed          2           ; critical fast 
;--------------------------------------------------------------------------------------------
;    ARCS CONFIGURATION
node_arc            0
node_arc_nb_channels      {1 1 2}   ; arc intleaved,  options for the number of channels
node_arc_raw_format     {1 17 27}   ; options for the raw arithmetics STREAM_S16, STREAM_FP32

node_arc            1
node_arc_nb_channels    {1 1 2}     ; options for the number of channels
node_arc_raw_format     {1 17 27}   ; options for the raw arithmetics STREAM_S16, STREAM_FP32

end
```

## Manifest header
The manifest starts with the identification of the node.

### node_developer_name "name"
Name of the developer/company having the legal owner of this node.
Example: 
```
    node_developer_name CompanyA & Sons Ltd  
```

### node_name "name"
Name of the node when using a graphical design environment.
Example: 
```
    node_name arm_stream_filter 
```
### node_logo "file name"
Name of the graphical logo file (file path of the manifest) of the node when using a graphical design environment.
Example: 
```
    node_logo arm_stream_filter.gif 
```
### node_nb_arcs "in out"
Number of input and output arcs of the node used for data streaming. 
Example 

```
    node_nb_arcs 1 1 ; nb arc input, output, default values "1 1"
```

### node_arc_parameter "n"
When the amount of parameters exceeds 256kB (deep-learning models, video file, etc..) the parameters must be read from extra arcs.
Example 
```
    node_arc_parameter 2 ; 2 additional arcs for large parameters
```

### node_variable_rxtx_data_rate "0/1"
The nodes are called with a data structure for each arc giving :
 - a pointers on input and out arcs 
 - the amount of data (input arcs) and free space (output arcs)

The nodes returns after updating the data structures, but to lower the computation overhead in the node it can tell here if the data flow can vary between input and output arcs. If the data rate do not change it tells here the scheduler to update the data structure automatically.

Examples of variable data rate (node_variable_rxtx_data_rate 1): MP3 encoder with variable rate encoding option, audio rate converter at using 160/147 ratio.
Examples of identical data rate (node_variable_rxtx_data_rate 0) : an audio filter, an amplifier receiving and delivering the same data formats and frame sizes.

Parameter value "1" tells the data flow is variable, the scheduler proposes input buffers as large as possible and output buffers with as much free space as possible. It is the responsibility of the node to read and write to good amount of data to avoid underflow on input and overflow on output arcs.

**The default is "0"** : ALL input and output arcs have the same raw data rate (same number of bytes consumed and produced per call), the scheduler arranges to find the minimum common denominator between all arcs. The nodes do not have to update buffer consumption and production which known before the call.
Example : 

````
 node_variable_rxtx_data_rate   1  ; variable stream rates
````


### node_using_arc_format "0/1"
During the RESET phase of the node, the scheduler will push (with "1" or not with "0") the arc data formats of all the arcs. The arc format is described in the chapter "IO control and stream data formats" of the file "Graph Template".
Example
` node_using_arc_format 1  the node is using arc stream format `

### node_mask_library    "n" 
The graph interpreter offers a short list of optimized DSP/ML and Math functions optimized for the platform using dedicated vector insttructions and coprocessors. 
Some platform may not incorporate all the libraries, the "node_mask_library" is a bit-field associate to one of the librarry service. 
This list of service is specially useful when the node is delivered in binary format.

bit 4 for the STDLIB library (string.h, malloc)
bit 5 for MATH (trigonometry, random generator, time processing)
bit 6 for DSP_ML (filtering, FFT, 2D convolution-8bits)
bit 7 for audio codecs
bit 8 for image and video codec

Example :

```
node_mask_library  64  ; the node has a dependency to DSP/ML computing services
```



### node_architecture  "name" (TBD)
Create a dependency of the execution of the node to a specific processor architecture. For example when the code is incorportating in-line assembly to a specific architecture.

Example:
` node_architecture    armv6m  ; a node made for Cortex-M0 ` 


### node_fpu_used "option" (TBD)
The command creates a dependency on the FPU capabilities
Example :

```
   node_fpu_used   0  ; fpu option used (default 0: none, no FPU assembly or intrinsic) 
```

### node_use_unlock_key  "0/1"  (TBD)
Tells if a key-exchange protocol is initiated at reset time to de-obfuscate node's firmware and enable specific features. The protocol tells where are the public key used to validate the digitally signed package. 
Example :

```
   node_use_unlock_key  1  
```

### node_version    "n"

For information, the version of the node
Example :

```
 node_version    101            ; version of the computing node
```

### node_complexity_index    "n"

For information and debug to set a watchdog timer: the parameters give the maximum amount of cycles for the initialization of the node and the processing of a frame.
Example :

```
 node_complexity_index 1e5 1e6 ; maximum number of cycles at initialization time and execution of a frame
```

node_not_reentrant

### node_not_reentrant    "n"

Information of reentrancy :  the function cannot be called again before it completes its previous execution. Default is "0", nodes are reentrant.
Example :

```
 node_not_reentrant 1 ; one single instance of the node can be scheduled in the graph
```

### node_stream_version  "n"            
Version of the stream scheduler it is compatible with.
Example :

```
  node_stream_version    101
```

 

### node_logo  "file name" 
File name of the node logo picture (JPG/GIF format) to use in the GUI.

--------------------------------------

## Node memory allocation
A node can ask for **up to 6 memory banks** with tunable fields : 
- type (static, working/scratch, static with periodic backup)
- speed (normal, fast, critical fast)
- relocatable (the location can change after the node was notified)
- program / data 
- size in bytes

The size can be a simple number of bytes or a computed number coupled to a function of stream format parameters (number of channels, sampling rate, frame size) and a flexible parameter defined in the graph, here.
The total memory allocation size in bytes = 
```
   A                               fixed memory allocation in Bytes (default 0) 
   + B x nb_channels of arc(i)     number of channels in arc index i (default 0) 
   + C x sampling_rate of arc(j)   sampling rate of arc index j (default 0) 
   + D x frame_size of arc(k)      frame size used for the arc index k (default 0) 
   + parameter from the graph      optional field "node_malloc_add" 
```

**The first memory block** **is the** **node instance**, followed by other blocks. This block has the index #0.
### node_mem "index"           
The command is used to start a memory block declaration with the index in the parameter.
Example :

```
` node_mem 0    ; starts the declaration section of memory block #0 `  
```

### node_mem_alloc  "A"
The parameter gives the "A" value of fixed memory allocation in Bytes.
Example :
```
node_mem_alloc          32          ; size = 32Bytes data memory, default Static, default Fast memory block
```

### node_mem_nbchan "B" "i" 
Declaration of extra memory in proportion to the number of channel of a specified arc index.
Example :

```
node_mem_nbchan 44 3  ; add this amount of bytes : 44 x nb of channels of arc 3
```

### node_mem_sampling_rate "C" "j'"
Declaration of extra memory in proportion with the sampling rate of a given arc index.
Example :
```
node_mem_sampling_rate 44.0 3  ; add this amount of bytes : 44.0 x sampling_rate in Hertz of arc 3
```

### node_mem_frame_size "D" "k"   
Declaration of extra memory in proportion with the frame size  of the stream flowing through a specified arc index.
Example :
```
node_mem_frame_size 44 3  ; add this amount of bytes : 44 x frame size of arc 3
```

### node_mem_alignment "n"
Declaration of the memory Byte alignment
Example :
```
node_mem_alignement     4           ; 4 bytes to (default) ` 
```

### node_mem_type "n"
Definition of the dynamic characteristics of the memory block :

0 STATIC : memory content is preserved (default )

1 WORKING : scratch memory content is not preserved between two calls 

2 PERIODIC_BACKUP static parameters to reload during a warm reboot 

3 PSEUDO_WORKING static only during the uncompleted execution state of the NODE

Example :

```
node_mem_type 3   ; memory block put in a backup memory area when possible
```

### node_mem_speed "n"
Declaration of the memory desired for the memory block.

0 for 'best effort' or 'no constraint' on speed access

1 for 'fast' memory selection when possible

2 for 'critical fast' section, to be in I/DTCM when available

Example :

```
node_mem_speed 0   ; relax speed constraint for this block
```

### node_mem_relocatable "0/1"
Declares if the pointer to this memory block is relocatable (parameter '1'), or assigned a fixed address at reset (default, parameter '0').
When the memory block is relocatable a command 'STREAM_UPDATE_RELOCATABLE' is sent to the node with address changes.
Example :

```
node_mem_relocatable    1   ; the address of the block can change
```

### node_mem_data0prog1 "0/1"
This command tells if the memory will be used for data or program accesses. Default is '0' for data access.
Example :
`   node_mem_data0prog1  1 ; program memory block ` 

--------------------------------------

## Configuration of the arcs attached to the node
The arc configuration gives the list of compatible options possible for the node processing. Some options are described as a list, or as a range of values. 
The syntax is : an index and the list of numbers within brackets "{" and "}". The index gives the default value to consider in the list. Index "1" corresponds to the first element of the list.
Index value "0" means "any value". The list can be empty in that case.

Example of an option list between five values, the index is 2 meaning the default value is the second in the list (value = 6).
`   { 2  5 6 7 8 9 } ` 
When the index is negative the list is decoded as a "range". A Range is a set of three numbers : 

- the first option
- the step to the next possible option
- the last (included) option

The absolute index value selects the default value in this range.

Example of is an option list of values (1, 1.2, 1.4, 1.6, 1.8, .. , 4.2), the index is -3 meaning the default value is the third in the list (value = 1.4).
```
    { -3  1 0.2 4.2 } 
```

### node_arc "n"
The command starts the declaration of a new arc, followed by its index used when connecting two nodes.
Example :

```
   node_arc 2    ; start the declaration of a new arc with index 2
```

Implementation comment : all the nodes have at least one arc on the transmit side used to manage the node's locking field.

### node_arc_rx0tx1 "0/1"
Declares the direction of the arc from the node point of view : "0" means a stream is received  through this arc, "1" means the arc is used to push a stream of procesed data.
```
    node_arc_rx0tx1             0               ; followed by 0:input 1:output, default = 0 and 1
```

### node_arc_interleaving  "0/1"
Arc data stream interleaving scheme: "0" for no interleaving (independent data frames per channel), "1" for data interleaving at raw-samples levels.
Example :
```
    node_arc_interleaving 0     data is deinterleaved on this arc
```

### node_arc_nb_channels  "n"
Number of the channels possible for this arc (default is 1).
Example :
```
    node_arc_nb_channels {1 1 2}  ; options for the number of channels is mono or stereo
```

### node_arc_raw_format "f"
Raw samples data format for read/write and arithmetic's operations. The stream in the "2D domain" are defining other sub-format 
Example :
```
    node_arc_raw_format {1 17 27} raw format options: STREAM_S16, STREAM_FP32, default values S16
```

### node_arc_frame_length "n"
Frame size options in Bytes. 
node_arc_frame_length       {1 1 2 16}      ; options of possible frame_size in number of sample (can mono or multi-channel)
Example :
```
    node_arc_frame_length 2    ; start the declaration of a new arc with index 2 
```

### node_arc_frame_duration "t"
Duration of the frame in milliseconds. The translation to frame length in Bytes is made during the compilation of the graph from the sampling-rate and the number of channels. 
A value "0" means "any duration" which is the default.
Example :
```
    node_arc_frame_duration {1 10 22.5}  frame of 10ms (default) or 22.5ms
```

### node_arc_sampling_rate "fs"
Declaration of the allowed options for the node_arc_sampling_rate in Hertz.
Example :
```
    node_arc_sampling_rate {1 16000 44100} ; sampling rate options, 16kHz is the default value if not specified 
```

### node_arc_sampling_period_s  "T"
Duration of the frame in seconds. The translation to frame length in Bytes is made during the compilation of the graph from the sampling-rate and the number of channels. 
A value "0" means "any duration" which is the default.
Example :
```
    node_arc_sampling_period_s {-2 0.1 0.1 1}  frame sampling going from 100ms to 1000ms, with default 200ms
```

### node_arc_sampling_period_day "D"
Duration of the frame in days. The translation to frame length in Bytes is made during the compilation of the graph from the sampling-rate and the number of channels. 
A value "0" means "any duration" which is the default.
Example :
```
    node_arc_sampling_period_day {-2 1 1 30}  frame sampling going from 1 day to 1 month with steps of 1 day.
```

### node_arc_sampling_accuracy  "p"
When a node does not need the input data to be rate-accurate, this command allows some rate flexibility without the need for the insertion of a synchronous rate converter.
The command parameter is in percent.
Example :
```
    node_arc_sampling_accuracy  0.1  ; sampling rate accuracy is 0.1%
```


### node_arc_inPlaceProcessing  "in out"
Memory optimization with arc buffer overlay. This command tells the "in" arc index is overlaid with the "out" arc index. The default configuration is to allocate different memory for input and output arcs. 
The arc descriptors are different but the base address of the buffers are identical.
Example :
```
    node_arc_inPlaceProcessing  1 2   ; in-place processing can be made between arc 1 and 2
```

