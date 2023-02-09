# Overview 

-   CMSIS-Stream is a scheduler of DSP/ML software components using a
    standard callaing interface, and arranged in for stream-based
    processing. CMSIS-Stream is open-source, and portable to Cortex-M,
    Cortex-R, Cortex-A and Laptop computers.

    -   Example of scheduled software components : image and voice
        codec, data conditioning, motion classifiers, data mixers, etc
        \...

-   **From the developer point of view**, it creates opaque memory
    interfaces to the input/output streams of a graph, and arranges data
    are exchanged in the desired formats of each component. CMSIS-Stream
    manages the memory mapping with speed constraints, provided by the
    developer, at instance creation. This lets software run with maximum
    performance in rising situations of memory bounded problems.
    CMSIS-Stream will accept code in binary format and software
    protection with keys. It is ready to integrate memory protection
    between software components.

-   **From the system integrator view**, it eases the tuning and the
    replacement of one component by another one, and is made to ease
    processing split with multiprocessors. The stream is described with
    a graph (a text file) designed with a graphical tool. The
    development of DSP/ML processing will be possible without need to
    write code, and allow graph changes and tuning without
    recompilation.

# Contents {#contents .TOC-Heading}

[Overview [1](#overview)](#overview)

[Compute Streaming Interface
[2](#compute-streaming-interface)](#compute-streaming-interface)

[Entry-point Functions
[2](#entry-point-functions)](#entry-point-functions)

[Two entry points [3](#two-entry-points)](#two-entry-points)

[Calling sequence [3](#calling-sequence)](#calling-sequence)

[1) Platform and SWC manifests
[4](#platform-and-swc-manifests)](#platform-and-swc-manifests)

[1.1 Platform manifests [4](#platform-manifests)](#platform-manifests)

[1.2 Plaform IO manifest
[4](#plaform-io-manifest)](#plaform-io-manifest)

[1.3 SWC manifests [5](#swc-manifests)](#swc-manifests)

[2) The graph boundaries
[6](#the-graph-boundaries)](#the-graph-boundaries)

[3) Linked list of SWC [7](#linked-list-of-swc)](#linked-list-of-swc)

[3.1 SWC interface [7](#swc-interface)](#swc-interface)

[3.1.1 SWC parameter "MEMREQ"
[7](#swc-parameter-memreq)](#swc-parameter-memreq)

[3.1.2 SWC parameter "RESET"
[7](#swc-parameter-reset)](#swc-parameter-reset)

[3.1.3 SWC parameter "SET_PARAMETER"
[7](#swc-parameter-set_parameter)](#swc-parameter-set_parameter)

[3.1.4 SWC parameter "READ_PARAMETER"
[7](#swc-parameter-read_parameter)](#swc-parameter-read_parameter)

[3.1.5 SWC parameter "RUN" [7](#swc-parameter-run)](#swc-parameter-run)

[3.1.6 SWC parameter "STOP"
[7](#swc-parameter-stop)](#swc-parameter-stop)

[3.2 ARC descriptors [7](#arc-descriptors)](#arc-descriptors)

[3.3 SWC parameters [7](#swc-parameters)](#swc-parameters)

[3.4 SWC test patterns [7](#swc-test-patterns)](#swc-test-patterns)

[A Data types [8](#a-data-types)](#a-data-types)

[A.1Raw data types [8](#a.1raw-data-types)](#a.1raw-data-types)

[A.2 Array of Raw data types
[8](#a.2-array-of-raw-data-types)](#a.2-array-of-raw-data-types)

[A.3 Stream digital "data formats\" [9](#_Toc126861480)](#_Toc126861480)

[A.4 Memory types [9](#a.4-memory-types)](#a.4-memory-types)

[B SWC delivery [10](#_Toc126861482)](#_Toc126861482)

**Compute Streaming Interface**

**Entry-point Functions**

**1) Platform and SWC manifests**

> **1.1 Platform manifests**
>
> **1.2 Plaform IO manifest**
>
> **1.3 SWC manifests**

**2) The graph boundaries**

**3) Linked list of SWC**

> **3.1 SWC interface**
>
> **3.1.1 SWC parameter "MEMREQ"**
>
> **3.1.2 SWC parameter "RESET"**
>
> **3.1.3 SWC parameter "SET_PARAMETER"**
>
> **3.1.4 SWC parameter "READ_PARAMETER"**
>
> **3.1.5 SWC parameter "RUN"**
>
> **3.1.6 SWC parameter "STOP"**
>
> **3.2 ARC descriptors**
>
> **3.3 SWC parameters**
>
> **3.4 SWC test patterns**

**A Data types**

> **A.1 Raw data types**
>
> **A.2 Array of Raw data types**
>
> **A.3 Stream digital "data formats\"**
>
> **A.4 Memory types**

**B SWC deliveries**

# Compute Streaming Interface 

CMSIS-Stream is scheduling the software components of a graph. The nodes
of the graph are **software components** ("SWC ") independent of the
platform capabilities

The graph description is a text file (example
[*here*](https://github.com/ARM-software/EndpointAI/blob/master/Kernels/Research/CMSIS-Stream/graph.txt))
and is the result from the translation made in a GUI tool, using :

-   a **manifest of the platform** (details on processors, memory,
    peripherals)

-   a **manifest of each SWC** : description of the data formats of the
    interfaces

CMSIS-Stream is translating the **graph description text** file to a
binary data structure, with the help of the data in the manifests. This
result is placed in shared memory area to all processes and processors.

This shared **binary** **graph structure** consists in :

-   the linked list of arcs and nodes (the SWC) of the graph

-   the arcs descriptors (read and write indexes to circular buffers)

-   the memory of the CMSIS-Stream instances scheduled the graph

-   the structure describing the operations at the boundary of the graph
    (the graph "IOs")

-   registers used to synchronize the different CMSIS-Stream instances,
    if any

## Entry-point Functions 

CMSIS-Stream has two entry-points, one for controls and services, and a
second one used as callback for notifications of data transfers :

void arm_stream (uint32_t command, PTR_INT ptr1, PTR_INT ptr2, PTR_INT
ptr3);

void arm_stream_io (uint32_t fw_io_idx, void \*data, uint32_t length);

### Two entry points 

The first control API has four parameters, three data parameters and a
command with values :

-   **STREAM_MEMREQ** : the application asks for the amount of memory
    needed to schedule the graph, the function returns the amount of
    memory for each memory bank (see "1.1.2 processor characteristics").
    The other parameters are :

    -   A function pointer to the **firmware of the platform**, in
        charge of the low-level abstraction of the hardware controls.
        Example of commands for "fw_entry_point (int,\*,\*,\*)":

        -   Return the details of current processor: its index,
            architecture and FPU options

        -   Control a semaphore

        -   Call one on the three functions used to control the device
            drivers : "set", "start", "stop" (see "2) The graph
            boundaries")

    ```{=html}
    <!-- -->
    ```
    -   Read the time information computed from a SYSTICK global counter
        increment.

    -   A pointer to the list of SWC entry points, and a pointer to
        their respective manifests (see "1.3 SWC manifests")

    -   A pointer to the "graph description text" to be compiled to
        "binary graph structure"

```{=html}
<!-- -->
```
-   **STREAM_RESET** : pointers memory banks are provided to
    "arm_stream()" which can initialize its instances and the SWC
    instances of the graph.

-   **STREAM_RUN** : the graph of components is scheduled (the
    linked-list of the "binary graph structure" is parsed, see "3)
    Linked list of SWC")

-   STREAM_END : the application releases memory

The second control API has three parameters : the index of the device
driver calling this function, the base address of the buffer, the size
of the buffer. The "index" is given in the platform IO manifest (see
"1.2.4 The ID of the hardware port"). Data format and interleaving is
described at "A.3.1 Data format fields common to all streams".

### Calling sequence

The main CMSIS-Stream instance is called by the application to compute
the amount of memory needed to execute the graph : memory of the arcs,
the SWC instances of the graph, the buffers used for IOs (command
"STREAM_MEMREQ" above).

In a second step, the application provides memory pointers to the memory
banks requested, and calls the CMSIS-Stream instance to activate of the
IOs at the boundary of the graph, for the memory initialization of each
CMSIS-Stream instances and all SWC (command "STREAM_RESET" above)

Finally the application lets the graph being scheduled by CMSIS-Stream
(command "STREAM_RUN" above).

The description of the scheduling of the graph consists in :

-   the content of the **manifests** of the platform and the manifests
    of the SWC (below paragraph "1) Platform and SWC manifests").

-   the way the **IOs** are sharing data with the ring buffers at the
    boundary of the graph (below paragraph "2) The graph boundaries").

-   the description of the **linked-list** and the connexions between
    arcs and nodes (below paragraph "3) Linked list of SWC").

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

-   **1.1.1 the list of services** delivered through CMSIS-Stream and
    its APIs, in two categories : the DSP/ML services and the
    Application services. This allows SWC delivered in binary to use
    libraries without incorporating them in the code. One example of
    use-case is to let a code written for the architecture ARMv7-M to
    run on ARM-v8.2-M with acceleration.

    -   DSP/ML services : abstraction to a short-list of kernels from
        CMSIS-DSP (filtering, spectral analysis, linear algebra)

    -   Multimedia audio and image compression: abstraction to audio
        codecs

    -   Application services: interface to IO (LED, Digit, GPIO),
        storage and timer services

-   **1.1.2 processor characteristics** : number of processors and
    pointer to description of each, number of different archictures and
    a pointer to the table of description, the memory bank used for the
    shared memory access

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

-   **1.1.3 The number of I/O** interfaces and the way to use them
    (paragraph below)

### 1.2 Plaform IO manifest

The platform_io_manifest is a structure of bit-field (28 Bytes min.)
holding :

-   **Four characters** used for debug and trace purpose indentifying
    the IO interface

-   **1.2.1 The "domain"** of the IO interface. This information is used
    differentiate the standard tuning of the interface (see A.4
    Domain-specific stream data types) :

    -   **Audio recording** stream, and the control of the number of
        channels, sampling rate, frame size, acoustic sensitivity, gain
        and automatic gain control, high-pass filtering, routing

    -   **Audio playing** stream, and the control of the number of
        channels, sampling rate, frame size, voltage sensitivity, gain
        and automatic gain, three bands frequency and gain control,
        routing

    -   **Motion recording** stream to control the number of channels
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

-   **1.2.2 The device driver capabilities**:

    -   **data flow initiatiator** either made by the hardware (device
        driver is master, for example with the frames from an audio
        Codec) or initiated by the graph (device driver is follower, for
        example when polling the temperature from application requests).

    -   **buffer allocation** : tells if the buffer used for the
        communication with the application is reserved by the device
        driver or needs to be allocated during the graph initialization
        steps, and in which platform memory area (one of the memory
        banks given in "1.1 Platform manifests").

-   **1.2.3 The default digital stream setting** (8 Bytes) : see "A.3
    Stream digital data types" for the description of the bit-fields
    giving the frame-size, data interleaving configuration, the raw data
    format in the frame, the number of channels, the sampling rate and
    the optional time-stamping format.

-   **1.2.4 The ID of the hardware port** used as parameter of the
    callback to notify the graph of new data (receive), or completion of
    the previous transfter (transmit).

-   **1.2.5 The default sensor configuration** (8 Bytes) related to the
    domain (see above). An a table of configuration control structured
    by lines (number of fields, data). To initialize a data stream at
    the boundary of the graph, the graph description text file gives the
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

There are three formats of Software Component manifests: normal with
maximum 4 streams connected to the component (minimum 76 Bytes), compact
format limited to 2 streams (40 Bytes), fat-binary delivery of the SWC
(minimum 80 Bytes).

-   **Format** selection, 2 bits, for normal, compact, binary format

-   **Version** of the CMSIS-Stream used for the design of this
    component, 4 bits.

-   **Number of stream** (input and output) managed by this component

-   Bit-field of dependencies to CMSIS-Stream for DSP/ML and Application
    services (see 1.1 Platform manifests)

-   The developer identification, 21 bits

-   The processor architectures and architecture options the component
    is compatible with

-   **The SWC identification code and revision**

-   A short name for debug, 4 characters

-   A flag telling if the SWC data instance is relocatable and can be
    moved between calls (the code allays making reference to the base
    address of the instance, and if the code and its the inlined
    assembly is made ready for "position independent execution".

-   **The stream format of each input and output stream** of the
    components (see "A.3 Stream digital "data formats\""), giving the
    default values. The stream format field "frame size" is used to
    define the minimum amount of data (in Bytes) needed on input streams
    before trying to call the SWC. Or the minimum amount of free space
    to reserve on the output buffer.

-   The allowed stream deviations from the above default value,
    concerning the maximum number of channels and the sampling rate.

The short format of SWC manifest stops here, and manifests data
continues for normal and binary formats:

-   The number of **parameter presets**, on 6 bits.

-   The optional need to reset the component when loading new
    parameters.

-   The size of the parameter presets, on 18 bits (\<256kBytes)

-   A pointer to the beginning of the parameter presets

-   A pointer to the test pattern, expected results, the configuration
    and replay options, *TBD*

-   A key to unlock the SWC or the full feature list, *TBD*

The binary manifest format are using offsets instead of pointers.

## 2) The graph boundaries

The below IO protocol facilitates code portability when switching
between hardware platforms. CMSIS-Stream provides an interface to the
firmware. It hides to the graph scheduler the way the samples are
generated or consumed (DMA, USB, files of the application, ..).

The board designer validates the operations of its sensors and
transducers in several configurations, which are saved in the "platform
IO manifest" (see "1.2.5 The default sensor configuration") and used
during the graph creation. If the IO stream do not deliver the data in
the format desired by the SWC consumer, CMSIS-Stream GUI tool will
insert a node for the format conversion. This automatic conversion
scheme applies between SWC in the middle of the graph too.

Sequence of functions called between the application and the device
driver :

-   **1) \"init\"** The GUI has a copy of the IO manifest and decides
    how the buffer used for exchanges will be defined, the domain
    settings. The type of data move to operate once receiving the
    information from the device driver (data to copy or the application
    will access the data \"in place\") was decided during the software
    architecture definition.

-   **2) \"set\"** The scheduler calls \"io_set()\" through the platform
    firmware interface, see "Entry-point Functions" with STREAM_MEMREQ
    parameters, for this desired configuration with calling parameters:

    -   the domain settings

    -   a proposed buffer address and size (if needed, see previous
        \"buffer allocation\").

> \"RX\" means a flow of data from external sensor to the application,
>
> \"TX\" means a flow of data generated by the application to the
> external transducer.

-   **3) \"start / continue\"** The scheduler initiates the data
    transfers with \"io_start()\" with two parameters : a pointer to the
    base address of buffer, the size in bytes to tell (\"io_start()\"
    returns a boolean telling if the data transfer is possible):

    -   **RX case** : \"this memory address and buffer size, exchanged
        during io_set() , is proposed for receiving new data\".

    -   **TX case** : \"this base address and buffer size, exchanged
        during io_set() (or the previous ack(), see below) is the first
        data to be transmitted\".

```{=html}
<!-- -->
```
-   **4) \"ack\"** Once new data are ready (or have just been
    transferred out), the device driver calls the callback (managed by
    the application, and encapsulating "arm_stream_io(int,\*,int)")with
    three parameters : ID (the device driver index), a pointer to a base
    address of a buffer, the size in bytes to tell,:

    -   **RX case** : \"I prepared for you a buffer of data, copy the
        data or use it directly from this place, and for this amount of
        bytes\". The address can change from last callback in case the
        device driver is using a ping-pong buffer protocol (or more
        buffers).

    -   **TX case** : \"I have completed the transfer of this buffer
        with this amount of bytes you told me to move out, and you can
        refill this new (or the same) buffer for the next transfer\".

-   **5) \"stop\"** The application ends up the use-case with a call to
    io_stop(). The device driver acknowledges the end of on-going
    transfers with a call to the callback, with no parameter.

## 3) Linked list of SWC 

After translation, binary graph structure holds different buffers, in a
shared memory space, incluing the linked-list representation of the
graph, as a table with the following format:

-   Header 32bits word

    -   12 bits to address the entry point of the component

    -   7 bits to quickly help the scheduler know if the component is
        executatble on the current processor (4 bits for the processor
        ID, 3 bits for the architecture ID)

    -   2 bits to set debug options (counter of cache-miss of this SWC,
        patch the input for self-test, ..)

    -   1 bit (debug) to force the scratch memory area of the component
        to be cleared before execution

    -   1 bit to activate a verbose mode of execution trace

    -   3 bits of SWC execution status :

        -   swc_status_boot : during memory reset and

        -   swc_status_reset : SWC reported its memory allocation
            requests

        -   swc_status_idle : SWC instance was created and initialized,
            not running

        -   swc_status_active : SWC is running

        -   swc_status_need_rerun: SWC returned but was partly executed
            to let higher priority tasks run for situation where there
            is no RTOS

        -   swc_status_stopped: disabled

    -   1 bit to pack the SWC description for 2 arcs

    -   1 bit to tell there a boot parameter index (32bits) to used at
        reset time

    -   1 bit to tell the SWC needs or not a MEMREQ initialization if
        there is not need for declaration of an instance (no instance,
        no static memory used for this component)

-   Optional instance index to a memory space of the binary graph
    structure.

-   Optional parameter set of data to a memory space of the binary graph
    structure.

-   The first two 16 bits arc descriptors with the format :

    -   12 bits to index the arc descriptor in the binary graph
        structure (see "3.2 ARC descriptors")

    -   1 bit "ready flag" used accelerate the scheduler decision : for
        input arcs of the SWC the flag is set when at least a minimum of
        data is available in the buffer. For output arcs of the SWC the
        flag is set when at least a minimum of free area is available in
        the output buffer.

    -   1 bit "high QoS flag" indicating the SWC can be executed even if
        the other streams do not have yet their "ready flag" set. The
        arcs descriptors with flow errors give indications on the
        operation to do to interpolate the data.

### 3.1 SWC interface

Example of SWC API
[here](https://github.com/ARM-software/EndpointAI/blob/master/Kernels/Research/CMSIS-Stream/SWC_sample.c).

### 3.1.1 SWC parameter "MEMREQ"

### 3.1.2 SWC parameter "RESET"

### 3.1.3 SWC parameter "SET_PARAMETER"

### 3.1.4 SWC parameter "READ_PARAMETER"

### 3.1.5 SWC parameter "RUN"

### 3.1.6 SWC parameter "STOP"

### 3.2 ARC descriptors

### 3.3 SWC parameters

### 3.4 SWC test patterns

## A Data types

### 

### A.1Raw data types

Sample of raw data type
[here](https://github.com/ARM-software/EndpointAI/blob/master/Kernels/Research/CMSIS-Stream/Stream_type.h).

### A.2 Array of Raw data types

### A.3 Stream digital "data formats\"

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

## B SWC delivery

A SWC delivery consists in :

-   a description of the parameter, and some presets

-   the input test pattern and expected results

-   a documentation for the GUI

-   the code of the SWC (source or binary) using one entry point with
    the template func(int,\*,\*,\*);
