---------------------------
[TOC]
--------------------------------------
# IO MANIFEST TEMPLATE

The graph interface manifests ("IO manifests") is text file of commands used by the graph compiler to understand the type of data flowing through the IOs of the graph. A manifest gives a precise list of information about the data format (frame length, data type, sampling rate, ...). This document is detailing the syntax used in a manifest. 

A manifest starts with a *Header* of general information, followed by the list of command used for the description of the stream. The IO manifest reader is assuming default values and associated command are useless and inserted by the programmer for information and readability.

Because of the variety of stream data types and setting options, the graph interpreter introduces the concept of physical "*domains*" (audio, motion, 2D, ...). The document gives starts with the list of general information for the stream digital format, followed by the specification of domain-related information.

## IO manifest header 
The "IO manifest" starts with the name which will be used in a design tool, followed by the "domain" (list below)
Example :

```
    io_name io_platform_sensor_in_0 
    io_domain   analog_in           
```

## IO Domains
| Domain name        | Code | Comments |
| ----            |   -- | -- |
| GENERAL            |   0 | Any stream, for example (a)synchronous sensor data + electrical, chemical, pixels, .. data from the application processors, compressed streams, JSON, etc .. |
| AUDIO_IN           |   1 | microphone, line-in, I2S, PDM RX                                                                                        |
| AUDIO_OUT          |   2 | line-out, earphone / speaker, PDM TX, I2S,                                                                              |
| GPIO_IN            |   3 | generic digital IO                                                                                   |
| GPIO_OUT           |   4 | generic digital IO , control of relay,                                                                                  |
| MOTION             |   5 | accelerometer, combined or not with pressure and gyroscope                                                              |
| 2D_IN              |   6 | camera sensor                                                                                                           |
| 2D_OUT             |   7 | display, led matrix,                                                                                                    |
| ANALOG_IN          |   8 | analog sensor with aging/sensitivity/THR control, example : light, pressure, proximity, humidity, color, voltage        |
| ANALOG_OUT         |   9 | D/A, position piezzo, PWM converter                                                                                     |
| RTC                |  10 | ticks sent from a programmable timer                                                                                    |
| USER_INTERFACE_IN  |  11 | button, slider, rotary button                                                                                           |
| USER_INTERFACE_OUT |  12 | LED, digits, display,                                                                                                   |
| PLATFORM_3         |  13 | platform-specific #3            |
| PLATFORM_2         |  14 | platform-specific #2                                                                            |
| PLATFORM_1         |  15 | platform-specific #1                                                                         |

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

## Common information of all digital stream

### io_commander0_servant1 "0/1"
The IO is "commander" when it start the data exchanges with the graph without the control from the scheduler. It is "servant" when the scheduler needs to pull or push the by calling the IO functions.
commander=0 servant=1 (default is servant)
IO stream are managed from the graph scheduler with the help of one subroutine per IO using the template : typedef void (*p_io_function_ctrl) (uint32_t command, uint8_t *data, uint32_t length); 
The "command" parameter can be : STREAM_SET_PARAMETER, STREAM_DATA_START, STREAM_STOP, STREAM_SET_BUFFER.

When the IO is "Commander" it calls arm_graph_interpreter_io_ack() when data is read
When the IO is "Servant" the scheduler call p_io_function_ctrl(STREAM_RUN, ..) to ask for data move. Once the move is done the IO driver calls arm_graph_interpreter_io_ack()

Example :

```
io_commander0_servant1  1  
```

**Graph interpreter implementation details**

IO stream are managed from the graph scheduler with the help of one subroutine per IO using the template : typedef void (*p_io_function_ctrl) (uint32_t command, uint8_t *data, uint32_t length); 
The "command" parameter can be : STREAM_SET_PARAMETER, STREAM_DATA_START, STREAM_STOP, STREAM_SET_BUFFER.

When the IO is "Commander" it calls arm_graph_interpreter_io_ack() when data is read
When the IO is "Servant" the scheduler call p_io_function_ctrl(STREAM_RUN, ..) to ask for data move. Once the move is done the IO driver calls arm_graph_interpreter_io_ack()


### io_buffer_allocation "x"

```
io_buffer_allocation    2.0     ;  default is 0, which means the buffer is declared outside of the graph
;  The floating-point number is a multiplication factor of the frame size (here 2 frames), 
;   the buffer size is computed with rounding (n = floor(X+0.5))
;  
;  When more than one byte are exchanged, the IO driver needs a temporary buffer. This buffer
;   can be allocated "outside(0)" by the IO driver, or ">1" during the graph memory mapping preparation
;   The memora mapping of this allocation is decided in the graph and can be in general-purpose or 
;   any RAM "0" or specific memory bank for speed reason or reserved for DMA processing, etc ..
```

### io_set0_copy1 "0/1"

Declares if the IO stream is used with **set**ting a pointer to the data (value 0) or with a memory **copy** (value 1). 

```
io_set0_copy1      1            
```

### io_direction_rx0tx1 "0/1"

Declaration of the direction of the stream from the graph scheduler point of view.

```
io_direction_rx0tx1   1         ; direction of the stream  0:input 1:output 
```

### io_raw_format "n"

Declaration of the size and type of the raw data.

```
io_raw_format      S16          ; options for the raw arithmetics computation format here  STREAM_S16 
```

### io_interleaving "0/1"
```
io_interleaving    1            ; multichannel interleaved (0), deinterleaved by frame-size (1) 
```

### io_nb_channels "n"
```
io_nb_channels     1            ; options for the number of channels 
```

### io_frame_length "o"
```
io_frame_length   {1 1 2 16 }   ; options of possible frame_size in number of sample (can mono or multi-channel). 
```

### io_frame_duration "o"
```
io_frame_duration {1 10 22.5}   ; options of possible frame_size in [milliseconds].     The default frame length is 1 sample
```

### io_subtype_units "x"

See file "Table.md" for the list of available Units from RFC8798 and RFC8428.

```
io_subtype_units  VRMS          ; depending on the domain. Here Units_Vrms of the "general" domain (0 = any or underfined)
```

### io_subtype_multiple "x"

Multiple units for interleaved streams

```
io_subtype_multiple {DPS GAUSS}     
; example of two multi domain stream : motion sensors for example can have 4 data 
;   units for accelerometer, gyroscope, magnetometer, temperature
```


### io_position "x"
```
io_position meter 1.1 -2.2 0.01 ; unit and relative XYZ position with the platform reference point
```

### io_euler_angles "x"
```
io_euler_angles  10 20 90       ; Euler angles with respect to the platform reference orientation, in degrees
```

### io_sampling_rate "x"
```
io_sampling_rate       {1 16000 44100 48000} ; sampling rate options (enumeration in Hz)
```

### io_sampling_period_s "s"
```
io_sampling_period_s   {1 1 60 120 }         ; sampling period options (enumeration in [second])   
```

### io_sampling_period_day "s"
```
io_sampling_period_day {1 0.25 1 7 }         ; sampling period options (enumeration in [day])   
```

### io_sampling_rate_accuracy "p"
```
io_sampling_rate_accuracy       0.1          ; in percentage
```

### io_time_stamp_type "n"

```
io_time_stamp_type {1 1}   ; 0 no time-stamp, 1 absolute time, 2 relative time from last frame, 3 frame counter
```

### io_time_stamp_format "n"

See file "Table.md" for the definition of Time types.

```
io_time_stamp_format {1 1}   ; 0/1/2/3 corresponding to  16/32/64/64 bits time formats (default : STREAM_TIME32)
```


--------------------------

## Comments section 
Information :
- jumpers to set on the board
- manufacturer references and internet URLs
- any other system integration warning and recommendations

​       