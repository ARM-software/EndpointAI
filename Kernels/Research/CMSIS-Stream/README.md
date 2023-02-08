# Overview 

-   CMSIS-Stream is a scheduler of DSP/ML software components using an
    interface standard. CMSIS-Stream will be open-source in Apache
    license, and portable to Cortex-M, Cortex-R, Cortex-A and Laptop
    computers.

    -   Example of scheduled software components : image and voice
        codec, data conditioning, motion classifiers, data mixers, etc
        \...

-   **From the developer point of view**, it creates opaque memory
    interfaces to the input/output streams, and arranges data are
    exchanged in the desired formats of each component. CMSIS-Stream
    manages the memory mapping with speed constraints provided by the
    developer at instance creation. This lets software run with maximum
    performance in rising situations of memory bounded problems.
    CMSIS-Stream will accept code in binary format and software
    protection with keys. In future releases CMSIS-Stream will provide
    memory protection between software components.

-   **From the system integrator view**, it eases the tuning and the
    replacement of one component by another one, and is made to ease
    processing split with multiprocessors. The stream is described with
    a graph (a text file) which can be designed with a graphical tool.
    The development of DSP/ML processing will be possible without need
    to write code, and allow graph adaptation and tuning without
    recompilation.

# Compute Streaming Interface 

CMSIS-Stream is scheduling the software components of a graph. The nodes
of the graph are **software components** ("SWC ") designed to be
independent of the platform capabilities

The graph description is a text file (example *here*) and results from
the translation made in a GUI tool, using :

-   a **manifest of the platform** (details on processors, memory,
    peripherals)

-   a **manifest of each SWC** : description of the data formats of the
    interfaces

CMSIS-Stream is translating the graph description text file to a binary
data structure, with the help of the data in the manifests. This result
is placed in shared memory area to all processes and processors.

This shared binary **graph structure** consists in :

-   the linked list of arcs and nodes (the SWC) of the graph

-   the arcs descriptors (read and write indexes to circular buffers)

-   the memory of the CMSIS-Stream instances scheduling the graph

-   the structure describing the operations at the boundary of the graph
    (the graph "IOs")

-   registers used to synchronize the different CMSIS-Stream instances,
    if any

The main CMSIS-Stream instance is called by the application to compute
the amount of memory needed to execute the graph : memory of the arcs
and each SWC instances of the graph.

In a second step, the application provides memory pointers to the memory
banks requested, and calls the CMSIS-Stream instance for the activation
of the IOs at the boundary of the graph, the memory initialization of
each CMSIS-Stream instances and all SWC. Finally the application lets
the graph being scheduled by CMSIS-Stream.

The description of the scheduling of the graph consists in explainations
of

-   the content of the manifests of the platform and the manifests of
    the SWC

-   the way the IOs are sharing data with the ring buffers at the
    boundary of the graph

-   the description of the linked-list and the connexions between arcs
    and nodes

## 1) Platform and SWC manifests

The flash memory of the device holds the data used to generate the
binary graph and execute it :

-   the table of entry points to SWC, and the code. This data is
    different for each processor architecture of the device.

-   the platform manifest : a combination of the description of the
    processor and a "platform_io_manifest" for the details of the
    sensors addressed by the device.

### 1.1 Platform manifests

The platform manisfest is structure of bit-fields (20 Bytes min.)
holding:

-   the list of services delivered through CMSIS-Stream and its APIs, in
    two categories : the DSP/ML services and the Application services.
    This allows SWC delivered in binary to use libraries without
    incorporating them in the code. One example of use-case is to let a
    code written for the architecture ARMv7-M to run on ARM-v8.2-M with
    acceleration.

    -   DSP/ML services : abstraction to a short-list of kernels from
        CMSIS-DSP (filtering, spectral analysis, linear algebra)

    -   Multimedia audio and image compression: abstraction to audio
        codecs

    -   Application services: interface to IO (LED, Digit, GPIO),
        storage and timer services

-   processor characteristics : number of processors and pointer to
    description of each, number of different archictures and a pointer
    to the table of description, the memory bank used for the shared
    memory access

    -   CMSIS-Stream uses several memory banks for the memory allocation
        of SWC :

        -   "internal memory"

        -   "external memory" with the slowest speed constraint

        -   "IO data memory" reserved for exchanges with peripherals

        -   "program memory",

        -   "fast data memory" for DTCM of Cortex-M or LLRAM of Cortex-R

        -   "graph memory" only addressed by CMSIS-Stream

        -   "backup memory" used to save critical data fast restore on
            warm boot

    -   The processor and architecture tables give:

        -   The processor architecture (ARMv6-M, ARMv7-M.. ARMv9-A)

        -   The architecture options (FPU options, Vector processing
            options)

        -   The indexes used to differentiate each processor

        -   The memory banks (above) able to be addressed

-   The number of I/O interfaces (see below)

### 1.2 Plaform IO manifest

The platform_io_manifest is a structure of bit-field (28 Bytes min.)
holding :

-   Four characters used for debug and trace purpose indentifying the IO
    interface

-   The "domain" of the IO interface. This information is used
    differentiate the standard tuning of the interface (see A.4
    Domain-specific stream data types) :

    -   Audio recording stream, and the control of the number of
        channels, sampling rate, frame size, acoustic sensitivity, gain
        and automatic gain control, high-pass filtering, routing

    -   Audio playing stream, and the control of the number of channels,
        sampling rate, frame size, voltage sensitivity, gain and
        automatic gain, three bands frequency and gain control, routing

    -   Motion recording stream to control the number of channels
        (accelerometer, gyroscope, magnetometer, metadata), sampling
        rate, frame size, sensitivity, averager, vector magnitude
        computation, metadata (free fall, shock, tap, transient
        detection..)

    -   Electrical domain, *TBD*, focus on motor control.

    -   Position domain, *TBD*, linear, angular, rotary position sensor

    -   Environmental domain, *TBD*, temperature, light, humidity, air
        quality

    -   Chemical and agricultural sensors, *TBD*

    -   Image sensor, *TBD*, X-Y control of the point of focus, zoom
        control

    -   Image codec stream, *TBD*, codec ID, initial padding and delay,
        vbr control, data rate

    -   Audio codec and MIDI stream, *TBD*, packing type, bit rate, dual
        mono mode, LFE audio, BFI flags, fast forward, final padding,
        VBR control

-   The default digital stream setting (8 Bytes) : see "A.3 Stream
    digital data types" for the description of the bit-fields giving the
    frame-size, data interleaving configuration, the raw data format in
    the frame, the number of channels, the sampling rate and the
    optional time-stamping format.

-   The default sensor configuration (8 Bytes) related to the domain
    (see above). An a table of configuration control structured by lines
    (number of fields, data). To initialize a data stream to the
    boundary of the graph, the graph description text file gives the
    bit-fields of the desired configuration, or let all the bits at "0",
    for the default configuration of the platform firmware. In the
    example below a microphone path can be configure with the number of
    channels (3 bits on the LSB, 8 configurations possible), the
    manifest proposes 3 options with 1, 2 and 8 channels. The two bits
    (4 options) allocated to the frame-size options have only 2 options:
    10ms and 16ms frame lengths :

> /\* tuning of AUDIO_REC_STREAM_SETTING \*/
>
> const float microphone_settings \[\] = {
>
> /\* nb of bits, format = nb of options, list of options \*/
>
> /\* 3 nchan \*/ 3, 1, 2, 8,
>
> /\* 4 FS \*/ 2, 16000.0f, 48000.0f,
>
> /\* 2 framesize \[s\] \*/ 2, 0.01f, 0.016f,
>
> /\* 1 option = \"fixed in HW and this is the setting\" \*/
>
> /\* 0 option = \"fixed in HW, and it depends\" \*/
>
> /\* 4 PGA gain + digital gain option \[dB\]\*/ 0,
>
> /\* 2 hpf set at 20Hz \*/ 1, 20.0f,
>
> /\* 1 agc option \*/ 0,
>
> /\* 2 router to MIC, LINE, BT, LOOP \*/ 0,
>
> /\*14 bits remains \*/
>
> };

### 1.3 SWC manifests

Three formats of Software Component manifests are possible : normal
format with maximum 4 streams connected to the component, compact format
limited to 2 streams, fat-binary delivery of the SWC.

-   Format selection, 2 bits, for normal, compact, binary format

-   Version of the CMSIS-Stream used for the design of this component, 4
    bits.

-   Number of input streams and output streams managed by this component

-   Bit-field of dependencies to CMSIS-Stream for DSP/ML and Application
    services (see 1.1 Platform manifests)

-   The developer identification, 21 bits

-   The processor architectures and architecture options the component
    is compatible with

-   The SWC identification code and subversion

-   A short name for debug, 4 characters

-   A flag telling if the SWC data instance is relocatable and can be
    moved between calls (the code allays making reference to the base
    address of the instance, and if the code and its the inlined
    assembly is made ready for "position independent execution".

-   The stream format of each input and output ports of the components
    (see "A.3 Stream digital data types -- "data formats\""), giving the
    default values.

-   The allowed stream deviations from the above default value,
    concerning the maximum number of channels and the sampling rate.

The short format of SWC manifest stops here

-   The number of presets : configuration of parameters, on 6 bits.

-   The optional need to reset the component when loading new
    parameters.

-   The size of the parameter preset array, on 18 bits (\<256kBytes)

-   A pointer to the beginning of the parameter presets

-   A pointer to the test pattern, expected results, the configuration
    and replay options, *TBD*

-   A key to unlock the SWC or the full feature list, *TBD*

The binary manifest format have extra fields (offsets).

## 2) IOs at the graph boundary

A SWC manifest is ..

### Protocol

We want to ease code portability when switching between hardware
platforms, with a combination of predefined subroutines and features
manifests.

Our focus is to create abstraction means with : the processor(s), the
memory map (TCM, external RAM,..), the embedded peripherals (DMA,
GPIO,..), the mixed-signal components (audio Codec, accelerometers,
camera,..), PCB routing.

Practically, we want to reuse as much of the code used during the
platform validation, and couple this code with a manifest, a binary
description of the features of the interface, of the possible
configurations (analog setting, data format, frame format).

configurations (analog setting, data format, frame format).

+-----------------------------------------------------------------------+
| information provided by the device driver to the application (\"IO    |
| manifest\")                                                           |
+=======================================================================+
| domain of operation : audio play (audio samples to                    |
| loudspeaker/line-out), audio record (audio sample from                |
| microphone/line-in), motion (1 to 9-Axis IMU), electrical (voltage,   |
| current, power sensors), position (linear, circular position          |
| sensors), chemical (air, gas, radiation, agricultural sensor),        |
| control (data flow of the trace and remote control UART), image       |
| (picture sensor), voice compressed (ITU/3GPP compressed stream),      |
| audio compressed  (MPEG and Bluetooth compressed stream), MIDI, image |
| compressed (JPG/PNG encoded stream), video compressed (TBD).          |
|                                                                       |
| master / follower stream : tells if the data flow is initiated by the |
| hardware (device driver is master, for example with the frames from   |
| an audio Codec) or initiated by the application (device driver is     |
| follower, for example when polling the temperature from application   |
| requests).                                                            |
|                                                                       |
| buffer allocation : tells if the buffer used for the communication    |
| with the application is reserved by the device driver, and in which   |
| platform memory area (for hardware design constraint).                |
|                                                                       |
| stream format : raw data format, frame size, number of channels,      |
| insertion of time-stamps in the frames, type of interleaving.         |
|                                                                       |
| domain settings : different for each domain. For audio recording :    |
| allowed number of channels, allowed sampling rates, filtering         |
| options. For motion sensing : sensitivity options, sampling rates,    |
| averaging options.                                                    |
|                                                                       |
| default domain settings : default setting applied at reset.           |
|                                                                       |
| validated settings : documentation of list of validated settings      |
|                                                                       |
| functions entry points : for io_start / io_stop / io_set              |
| configuration                                                         |
|                                                                       |
| index : the \"ID\", index of the IO port                              |
|                                                                       |
| callback : entry point to the application for the notification of     |
| data transfers completion.                                            |
+-----------------------------------------------------------------------+

+-----------------------------------------------------------------------+
| sequence of functions called between the application and the device   |
| driver                                                                |
+=======================================================================+
| 1\) \"init\" The application reads the IO manifest and decides the    |
| location of the buffer, the domain settings. The type of data move to |
| operate once receiving the information from the device driver (data   |
| to copy or the application will access the data \"in place\") was     |
| decided during the software architecture definition.                  |
|                                                                       |
| 2\) \"set\" The application calls \"io_set()\" for this desired       |
| configuration with calling parameters: the domain settings, a         |
| proposed buffer address and size (if needed, see previous \"buffer    |
| allocation\"). \"RX\" means a flow of data from external sensor to    |
| the application, \"TX\" means a flow of data generated by the         |
| application to the external transducer.                               |
|                                                                       |
| 3\) \"start / continue\" The application initiates the data transfers |
| with \"io_start()\" with two parameters : a pointer to the base       |
| address of buffer, the size in bytes to tell :                        |
|                                                                       |
| RX case : \"this memory address and buffer size, exchanged during     |
| io_set() , is proposed for receiving new data\".                      |
|                                                                       |
| TX case : \"this base address and buffer size, exchanged during       |
| io_set() (or the previous ack(), see below) is the first data to be   |
| transmitted\".                                                        |
|                                                                       |
|    \"io_start()\" returns a boolean telling if the data transfer is   |
| possible.                                                             |
|                                                                       |
| 4\) \"ack\" Once new data are ready (or have just been transferred    |
| out), the device driver calls the callback with three parameters : ID |
| (the device driver index), a pointer to a base address of a buffer,   |
| the size in bytes to tell,:                                           |
|                                                                       |
| RX case : \"I prepared for you a buffer of data, copy the data or use |
| it directly from this place, and for this amount of bytes\". The      |
| address can change from last callback in case the device driver is    |
| using a ping-pong buffer protocol (or more buffers).                  |
|                                                                       |
| TX case : \"I have completed the transfer of this buffer with this    |
| amount of bytes you told me to move out, and you can refill this new  |
| (or the same) buffer for the next transfer\".                         |
|                                                                       |
| 5\) \"stop\" The application ends up the use-case with a call to      |
| io_stop(). The device driver acknowledges the end of on-going         |
| transfers with a call to the callback, with no parameter.             |
+-----------------------------------------------------------------------+

Interfaces

+-----------------------------------------------------------------------+
| The manifest (in Flash)                                               |
+=======================================================================+
| struct platform_io_manifest           \                               |
| {\                                                                    |
|     /\* name of the IO interface \*/\                                 |
|     const char io_name\[16\];   \                                     |
|     \                                                                 |
|     /\* start/move, stop, set stream \*/\                             |
|     const swc_function_ptr io_controls\[3\];                          |
|                                                                       |
|     /\* bit-fields: physical domains, memory plane, master/follower   |
| \*/\                                                                  |
|     const uint32_t io_domain;                                         |
|                                                                       |
|     /\* bit-fields: digital formats and mixed-signal settings \*/\    |
|     const uint32_t stream_settings_default\[4\];                      |
|                                                                       |
|     /\* unit and RMS level corresponding to integer full-scale or     |
| 1.0f \*/\                                                             |
|     const uint32_t \*unit;\                                           |
|     const float \*sensitivity;                                        |
|                                                                       |
|     /\* list of options \*/\                                          |
|     const float \*stream_setting_extension;\                          |
| };                                                                    |
+-----------------------------------------------------------------------+

### IOs at the graph boundary

## 3) Linked list of SWC 

A SWC delivery consists in :

-   a description of the parameter, and some presets

-   the input test pattern and expected results

-   a documentation for the GUI

-   the code of the SWC (source or binary)

3bits status, only bit-field in write access, at the end of \_RUN

1bits verbose level, 0=quiet, 1=full details

1bits clear working areas at SWC transitions (debug)

2bits 2 bit-field to enable debug options : counter of PMU cache-miss,
patch in/out

3bits bit-field : this swc usable on architectures \[0..6\] 0=any (=same
as main_proc) 7=script

4bits this swc to proc-1 (among 15), 0=any from this architecture

12bits SWIDX swc index of swc_entry_points\[\]\[\] and
swc_manifest_all\[\]

### SWC interface

int32_t arm_beamformer_f32 (int32_t command, void \*instance, void
\*data, void \*parameters)

{

int32_t swc_returned_status = 0;

switch (command)

{

case STREAM_MEMREQ:

/\* func (STREAM_MEMREQ,

\*pointer to memory_request =\> the first pointer is the instance

\*in/out stacked STREAM_DOMAIN_FORMAT_SIZE_W32

(buffer sizes) ending with 0x00,

parameters: input : the swc_execution_option code

output : returned KEY256

)

The first request is the instance which holds the other pointers

\*/

{ uint32_t \*memreq = (uint32_t \*)instance; /\* table of memory
requests \*/

uint32_t \*format_of_io = (uint32_t \*)data; /\* not used here \*/

uint32_t \*u32_param = (uint32_t \*)parameters, execution_option;

/\* 1st memory request = SWC instance , 2nd..= data pointers \*/

\*memreq++ = sizeof(beamformer_f32_instance);

\*memreq++ =
PACKSWCMEM(MEM_TYPE_STATIC,MEM_REQ_DATA,MEM_SPEED_REQ_CRITICAL_FAST,

MEM_REQ_4BYTES_ALIGNMENT,0,sizeof(beamformer_f32_fastdata_static));

\*memreq++ =
PACKSWCMEM(MEM_TYPE_WORKING,MEM_REQ_DATA,MEM_SPEED_REQ_CRITICAL_FAST,

MEM_REQ_4BYTES_ALIGNMENT,0,sizeof(beamformer_f32_fastdata_working));

\*memreq++ = 0; /\* end \*/

execution_option = \*u32_param; /\* partial / full execution code \*/

\*u32_param++ = 2902803550; /\* returned key \*/

\*u32_param++ = 1069229832;

\*u32_param++ = 2043483706;

\*u32_param++ = 1714015047;

break;

}

case STREAM_RESET:

/\* func(STREAM_RESET,

\*instance, pointer a size defined in the 1st parameter memreq

\*memory_results, pointers defined in the 2nd.. parameters of memreq
(above)

\*Stream) address of Stream

memory pointers are in the same order as in MEMREQ,

the SWC can access the system time and other services using the \@Stream
APIs

\*/

{

uint32_t \*memresults = (uint32_t \*)data;

beamformer_f32_instance \*pinstance = (beamformer_f32_instance
\*)instance;

pinstance-\>st = (beamformer_f32_fastdata_static \*) \*memresults++;

pinstance-\>w = (beamformer_f32_fastdata_working \*) \*memresults++;

pinstance-\>st-\>STREAM = parameters;

arm_beamformer_f32_reset(instance);

break;

}

case STREAM_SET_PARAMETER:

/\* func(STREAM_SET_PARAMETER,

\*instance,

\*parameter(s) (one or all)

Index of parameter :

index/tag of a single parameter to set

255 means \"all the parameters are changed\"

256+N means preset N

\*/

{

uint8_t \*new_parameters = (uint8_t \*)data;

break;

}

case STREAM_READ_PARAMETER: /\* func(STREAM_SET_PARAMETER,

\*instance,

\*parameter(s) (one or all)

Index of parameter :

index/tag of a single parameter to set

255 means \"all the parameters are read\" \*/

{

uint8_t \*new_parameters = (uint8_t \*)data;

break;

}

case STREAM_RUN:

/\* When data format is FMT_INTERLEAVED

Left and Right samples are in this order : LRLRLRLRLRLR ..

func(STREAM_RUN, instance, ptr to stereo data, nBytes buffer size)

When data format is FMT_DEINTERLEAVED_1PTR

Left and Right samples are in this order : LLLL..LLLLRRRR\...RRRR

Size of the first buffer : \<\--size\--\>

func(STREAM_RUN, instance, ptr to the Left buffer, Size in Bytes)

When data format is FMT_DEINTERLEAVED_NPTR

Left and Right samples are independent : LLLLL..LLLLL .. RRRRR\...RRRRR

Size of the first buffer : \<\--size L\--\> \<\--size R\-\--\>

Pointer to a structure : \[ {\*ptr_L, size L}, {\*ptr_R, size R} \]

func(STREAM_RUN, instance, XDAIS PTR_INT, 0)

\*/

{

uint32_t buffer1_size, buffer2_size, bufferout_free, increment,
\*pt_pt=0;

int32_t nb_input_samples, input_samples_consumed,
output_samples_produced;

uint8_t \*inBufs1stChannel=0, \*inBufs2ndChannel=0, \*outBufs;

/\* input pointers { (\*,n),(\*,n),..} updated at the end \*/

pt_pt = (uint32_t \*)data;

inBufs1stChannel = (uint8_t \*)(\*pt_pt++); /\* FMT_DEINTERLEAVED_NPTR
\*/

buffer1_size = (uint32_t )(\*pt_pt++);

inBufs2ndChannel = (uint8_t \*)(\*pt_pt++);

buffer2_size = (uint32_t )(\*pt_pt++);

outBufs = (uint8_t \*)(\*pt_pt++);

bufferout_free = (uint32_t) (\*pt_pt++);

nb_input_samples = buffer1_size / sizeof(int16_t);

arm_beamformer_f32_run(instance, (int16_t \*)inBufs1stChannel,

(int16_t \*)inBufs2ndChannel, nb_input_samples,

(int16_t \*)outBufs,

&input_samples_consumed,

&output_samples_produced,

&swc_returned_status);

pt_pt = (uint32_t \*)data;

increment = (input_samples_consumed \* sizeof(int16_t)\*MONO);

\*pt_pt++ = (uint32_t)(inBufs1stChannel += increment);

\*pt_pt++ = (buffer1_size -= increment);

\*pt_pt++ = (uint32_t)(inBufs2ndChannel += increment);

\*pt_pt++ = (buffer2_size -= increment);

\*pt_pt++ = (uint32_t)(outBufs += increment);

\*pt_pt++ = (bufferout_free -= increment);

break;

}

case STREAM_END: /\* func(STREAM_END, instance, 0) \*/

{ arm_beamformer_f32_memory_free(instance);

break;

}

}

return swc_returned_status;

}

### SWC parameters

### SWC test patterns

## A Data types

### 

### A.1Raw data types

enum stream_raw_data_type {

STREAM_DATA_ARRAY = 0, /\* see stream_array: \[0NNNTT00\] 0, type, nb
\*/

/\* one bit per data \*/

STREAM_S1, /\* S, one signed bit \*/

STREAM_U1, /\* one bit unsigned \*/

/\* two bits per data \*/

STREAM_S2, /\* SX \*/

STREAM_U2, /\* XX \*/

STREAM_Q1, /\* Sx \~stream_s2 with saturation management\*/

/\* four bits per data \*/

STREAM_S4, /\* Sxxx \*/

STREAM_U4, /\* xxxx \*/

STREAM_Q3, /\* Sxxx \*/

STREAM_FP4_E2M1, /\* Seem micro-float \[8 .. 64\] \*/

STREAM_FP4_E3M0, /\* Seee \[8 .. 512\] \*/

/\* eight bits per data \*/

STREAM_S8, /\* Sxxxxxxx \*/

STREAM_U8, /\* xxxxxxxx \*/

STREAM_Q7, /\* Sxxxxxxx \*/

STREAM_CHAR, /\* xxxxxxxx \*/

STREAM_FP8_E4M3, /\* Seeeemmm tiny-float \[0.02 .. 448\] \*/

STREAM_FP8_E5M2, /\* Seeeeemm \[0.0001 .. 57344\] for NN training \*/

/\* 2 bytes per data \*/

STREAM_S16, /\* Sxxxxxxx.xxxxxxxx \*/

STREAM_U16, /\* xxxxxxxx.xxxxxxxx \*/

STREAM_Q15, /\*19 Sxxxxxxx.xxxxxxxx \*/

STREAM_FP16, /\* Seeeeemm.mmmmmmmm half-precision float \*/

STREAM_BF16, /\* Seeeeeee.mmmmmmmm bfloat \*/

/\* 3 bytes per data \*/

STREAM_Q23, /\* Sxxxxxxx.xxxxxxxx.xxxxxxxx in a 24bits container ! \*/

/\* 4 bytes per data \*/

STREAM_Q23_32, /\* SSSSSSSS.Sxxxxxxx.xxxxxxxx.xxxxxxxx \*/

STREAM_S32, /\* one long word \*/

STREAM_U32, /\* xxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx \*/

STREAM_Q31, /\* Sxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx \*/

STREAM_FP32, /\*27 Seeeeeee.mmmmmmmm.mmmmmmmm.mmmmmmmm float \*/

STREAM_CQ15, /\* Sxxxxxxx.xxxxxxxx Sxxxxxxx.xxxxxxxx (I Q) \*/

STREAM_CFP16, /\* Seeeeemm.mmmmmmmm Seeeeemm.mmmmmmmm (I Q) \*/

/\* 8 bytes per data \*/

STREAM_S64, /\* long long \*/

STREAM_U64, /\* unsigned 64 bits \*/

STREAM_Q63, /\* Sxxxxxxx.xxxxxx \...\.... xxxxx.xxxxxxxx \*/

STREAM_CQ31, /\* Sxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx Sxxxxxxx.xxxxxxxx..
\*/

STREAM_FP64, /\* Seeeeeee.eeemmmmm.mmmmmmm \... double \*/

STREAM_CFP32, /\* Seeeeeee.mmmmmmmm.mmmmmmmm.mmmmmmmm Seeeeeee.mmm.. (I
Q) \*/

/\* 16 bytes per data \*/

STREAM_FP128, /\* Seeeeeee.eeeeeeee.mmmmmmm \... quadruple precision \*/

STREAM_CFP64, /\* fp64 fp64 (I Q) \*/

/\* 32 bytes per data \*/

STREAM_FP256, /\* Seeeeeee.eeeeeeee.eeeeemm \... octuple precision \*/

/\* 2D formats \*/

STREAM_YUV420P, /\*39 Luminance, Blue projection, Red projection, 6
bytes per 4 pixels, reordered \*/

STREAM_YUV422P, /\* 8 bytes per 4 pixels, or 16bpp, Y0 Cb Y1 Cr (1 Cr &
Cb sample per 2x1 Y samples) \*/

STREAM_YUV444P, /\* 12 bytes per 4 pixels, or 24bpp, (1 Cr & Cb sample
per 1x1 Y samples) \*/

STREAM_CYM24, /\* cyan yellow magenta \*/

STREAM_CYMK32, /\* cyan yellow magenta black \*/

STREAM_RGB8, /\* RGB 3:3:2, 8bpp, (msb)2B 3G 3R(lsb) \*/

STREAM_RGB16, /\* RGB 5:6:5, 16bpp, (msb)5R 6G 5B(lsb) \*/

STREAM_RGBA16, /\* RGBA 4:4:4:4 32bpp (msb)4R \*/

STREAM_RGB24, /\* BBGGRR 24bpp (msb)8B \*/

STREAM_RGBA32, /\* BBGGRRAA 32bpp (msb)8B \*/

STREAM_RGBA8888, /\* AABBRRGG OpenGL/PNG format R=lsb A=MSB (\"ABGR32\"
little endian) \*/

STREAM_BW1B, /\* Y, 1bpp, 0 is black, 1 is white, ordered from the msb
to the lsb. \*/

STREAM_BW1BINV, /\* Y, 1bpp, 0 is white, 1 is black \*/

STREAM_GREY8B, /\* Grey 8b \*/

STREAM_GREY8BINV, /\*53 Grey 8b inverted 0 is white \*/

LAST_RAW_TYPE = 64, /\* coded on 6bits RAW_FMT0_LSB \*/

};

### A.2 Array of Raw data types

typedef uint32_t stream_array; /\* on 32b container, used for array of
parameters \*/

#define \_\_UNUSED_ARRAY_MSB 31

#define \_\_UNUSED_ARRAY_LSB 30

#define NBELEM_ARRAY_MSB 29

#define NBELEM_ARRAY_LSB 16 /\* 14 bits linear \*/

#define DATA_TYPE_ARRAY_MSB 15

#define DATA_TYPE_ARRAY_LSB 8 /\* 8 bits stream_raw_data_type \*/

#define ZERO_ARRAY_MSB 7

#define ZERO_ARRAY_LSB 0 /\* 8 STREAM_DATA_ARRAY = 00 \*/

### A.3 Stream digital data types -- "data formats\"

The description of a full data format is made on 16 Bytes (4 words of
32bits): 8 Bytes for the digital format (frame-size, data interleaving,
the raw data format in the frame, the number of channels, the sampling
rate..) and 8 Bytes for domain-specific data format and mixed-signal
sensor/transducer peripheral control.

#### A.3.1 Data format fields common to all streams

The first word of the common data format has the fields:

-   Frame size in Bytes, on 24bits, allowing data frames of 16MB. The
    value 0 means "any size". SWC manifests are using the data format to
    for each of their input and output ports. In this case, the field
    "frame size" means the minimum of input data needed by the SWC
    before any processing is feasible. And for the output ports: the
    minimum of free area to have in the ouput buffer to avoid overflow
    due to the data being produced by the SWC.

-   Interleaving scheme, on 2 bits, three options being used:

    -   "FMT_INTERLEAVED" : data is interleaved within the frame, in
        sequence. For example left and right audio samples are found
        like "LRLRLRLR.." or gyroscope data is provided like
        "XYZXYZXYZ.."

    -   "FMT_DEINTERLEAVED_1PTR" : interleaved format of frame-size
        ("LLLLLRRRRR LLLLLRRRRR..")

    -   "FMT_DEINTERLEAVED_NPTR" : the data addressed with pairs of
        pointers and frame-size, the buffer do not need to be
        contiguous.

-   Raw data format, on 6 bits, see "A.1Raw data types"

The second word of the common data format has the fields:

-   Number of channels minus one, on 5 bits, allowing up to 32 channels.

-   Sampling rate, on 21 bits, the value 0 means "asynchronous or slave
    IO port". The data format is F=M x 2\*\*E. With 19 bits for the
    unsigned mantissa "M", 2 bits for the exponents (values of "E" =
    0,-8,-16,-24), allowing to set the rate from 1 week period up to
    524kHz, with all the common audio and voice sampling rate with full
    accuracy.

-   Time-stamp format, on 2bits:

    -   No time-stamp on the data frames

    -   Absolute value of the time (64bits) on each frame, 2 MSB to
        control the format, 40 bits counting the SYSTICK periods of 1ms,
        16 bits computed from SYST_RVR to have the fraction of 1ms.

    -   Relative time from last frame (32 bits), 30bits of data and 2
        MSB to control the format (0: seconds spent from reset, 1:
        seconds spent from January 1^st^ 2023, 2: seconds interval in
        U14.16 format, 3: seconds interval in U24.4 format).

-   Four bits unused

#### A.4 Domain-specific stream data format

The third word of the domain-specific data format has the fields :

-   Domain code on 6 bits

-   Data mapping of the channels on 24 bits. Example of 7.1 format audio
    format (8 channels) FrontLeft, FrontRight, FrontCenter,
    LowFrequency, BackLeft, BackRight, SideLeft, SideRight. The code
    0101b of a 2-channel stream, means data for FrontLeft and
    FrontCenter.

-   Direction of the flow, 1 bit, "0" means data flow sent to the graph
    and "1" means generated by the graph processing

-   Hashing of the stream, *TBD*

### A.4 Memory types

enum prog_data_type

{ MEM_REQ_DATA = 0,

MEM_REQ_PROG = 1,

};

enum mem_speed_type /\* memory requirements associated to enum
memory_banks \*/

{ MEM_SPEED_REQ_ANY = 0, /\* best effort \*/

MEM_SPEED_REQ_NORMAL = 1, /\* can be external memory \*/

MEM_SPEED_REQ_FAST = 2, /\* will be internal SRAM when possible \*/

MEM_SPEED_REQ_CRITICAL_FAST = 3, /\* will be TCM when possible \*/

};

enum buffer_alignment_type

{ MEM_REQ_4BYTES_ALIGNMENT = 0, /\* 0 placed here as default value \*/

MEM_REQ_8BYTES_ALIGNMENT = 1,

MEM_REQ_16BYTES_ALIGNMENT = 2,

MEM_REQ_32BYTES_ALIGNMENT = 3,

MEM_REQ_64BYTES_ALIGNMENT = 4,

MEM_REQ_128BYTES_ALIGNMENT = 5,

MEM_REQ_NOALIGNMENT_REQ = 6, /\* address binary mask : \*/

MEM_REQ_2BYTES_ALIGNMENT = 7, /\* mask = \~((1 \<\<
(7&(mem_req_2bytes_alignment+2)) -1) \*/

};

enum { SWC_CONTROLS = 4, };

enum { SWC_CONTROLS_NAME = 8, };

enum buffer_relocation_type

{ NOT_RELOCATABLE = 0,

RELOCATABLE= 1,

};
