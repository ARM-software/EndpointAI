# Overview 

-   CMSIS-Stream is a scheduler of DSP/ML software components using a
    standard calling interface and arranged in for stream-based
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

# Contents 

-Overview . 1
-Compute Streaming Interface . 2
-Entry-point Functions 2
-Two entry points . 2
-Calling sequence . 3
-1) Platform and SWC manifests  3
-1.1 Platform manifests  3
-1.2 Platform IO manifest . 4
-1.3 SWC manifests  5
-2) The graph boundaries . 6
-3) Linked list of SWC 6
-3.1 SWC interface  7
-3.1.1 SWC parameter “MEMREQ”  7
-3.1.2 SWC parameter “RESET”  8
-3.1.3 SWC parameter “SET_PARAMETER”  8
-3.1.4 SWC parameter “READ_PARAMETER” . 8
-3.1.5 SWC parameter “RUN”  8
-3.1.6 SWC parameter “END” . 9
-3.2 ARCs descriptors . 9
-A Data types . 10
-A.1 Raw data types 10
-A.2 Array of Raw data types  10
-A.3 Stream digital “data formats" . 10
-A.4 Memory types . 11
-B Scripts  11
-C SWC delivery . 12

# Compute Streaming Interface 

CMSIS-Stream is scheduling the software components of a graph. The nodes
of the graph are **software components** ("SWC ") independent of the
platform capabilities.

The **graph description is a text** file (example
[*here*](https://github.com/ARM-software/EndpointAI/blob/master/Kernels/Research/CMSIS-Stream/graph.txt))
and is the result from the translation made in a GUI tool, using :

-   a **manifest of the platform** (details on processors, memory,
    peripherals)

-   a **manifest of each SWC** : description of the data formats of the
    interfaces

CMSIS-Stream is translating the graph description text file to a
**binary graph structure**, with the help of the data in the manifests.
This result is placed in shared memory area to all processes and
processors.

This shared **binary** **graph structure** consists in :

-   the **linked lis**t of arcs and nodes (the SWC) of the graph

-   the **arcs descriptors** (read and write indexes to circular
    buffers)

-   the memory of the **CMSIS-Stream instances** scheduled the graph.

-   the structure describing the operations at the boundary of the graph
    (the graph "IOs")

-   registers used to synchronize the different CMSIS-Stream instances,
    if any

## Two entry-points 

CMSIS-Stream has two entry-points, one for controling and asking for
services, and a second one used as callback for notifications of data
transfers :

void arm_stream (uint32_t command, PTR_INT ptr1, PTR_INT ptr2, PTR_INT
ptr3);

void arm_stream_io (uint32_t fw_io_idx, void \*data, uint32_t length);

### Calling sequence

The main CMSIS-Stream instance (the one located in the main process or
processor) is called by the application to compute the amount of memory
needed to execute the graph : the buffers of the arcs, the SWC instances
of the graph, the buffers used for IOs (command "STREAM_MEMREQ" below).

In a second step, the application provides the memory pointers to the
requested memory banks. The CMSIS-Stream instances are now allowed to
activate of the IOs at the boundary of the graph, do the memory
initialization of all SWC (command "STREAM_RESET" below).

Finally, the application lets the graph being scheduled by CMSIS-Stream
(command "STREAM_RUN" above).

The first control API has four parameters, three data parameters and a
command with values :

**STREAM_MEMREQ** : the application asks for the amount of memory needed
to schedule the graph; the function returns the needed amount of memory
for each memory bank (see "1.1.2 processor characteristics"). The
parameters are :

-   A function pointer to the firmware of the platform, in charge of the
    low-level abstraction of the hardware controls:

    -   Returns the details of current processor: its index in the
        manifest table, its architecture and the FPU options

    -   Call one on the three functions used to control the device
        drivers : "set", "start", "stop" (see "2) The graph boundaries")

    -   Read the time information, for example computed from a SYSTICK
        global counter.

-   A pointer to the list of SWC entry points, and a pointer to their
    respective manifests (see "1.3 SWC manifests")

-   A pointer to the "graph description text" to be compiled to "binary
    graph structure"

a)  **STREAM_RESET** : pointers memory banks are provided to
    "arm_stream()" which can initialize its instances and the SWC
    instances of the graph. In a similar way described for SWC (see
    "3.1.2 SWC parameter RESET") the application is providing to the
    CMSIS-Stream a callback mechanism. Each CMSIS-Stream instance is
    stored in the shared memory of "binary graph structure" with the
    format:

-   7 offsets to the physical memory banks (see "1.1.2 processor
    characteristics"). This information lets the arc's buffer being
    address with indexes instead of physical pointers, it allows sharing
    the same arc's descriptors among processors having different memory
    address decoding (including arch64 processors).

-   Debug informations, the execution state of the instance, the current
    SWC under processing.

-   A 32bits-field of the graph IO ports to have look to. Most
    CMSIS-Stream instances will not be given access to the peripherals.
    Those indexes are used to address the platform IO manifest and
    checking the associated graph's ring buffers are not having flow
    problems.

-   A function pointer to the firmware of the platform (see above).

-   The list of SWC entry points

b)  **STREAM_RUN** : the graph of components is scheduled (the
    linked-list of the "binary graph structure" is parsed, see "3)
    Linked list of SWC")

c)  STREAM_END : command forwarded to each SWC to release memory
    allocated with stdlib.

The second control API (arm_stream_io) has three parameters : the index
of the device driver calling this function, the base address of the
buffer, the size of the buffer. The "index" is given in the platform IO
manifest (see "1.2.4 The ID of the hardware port"). Data format and
interleaving is described at "A.3.1 Data format fields common to all
streams".

The description of the scheduling of the graph consists in :

-   the content of the **manifests** of the platform and the manifests
    of the SWC

    -   paragraph below "1) Platform and SWC manifests"

-   the way the **IOs** are sharing data with the ring buffers at the
    boundary of the graph

    -   paragraph below "2) The graph boundaries"

-   the description of the **linked-list** and the connexions between
    arcs and nodes

    -   paragraph below "3) Linked list of SWC"

## 1) Platform and SWC manifests

The flash memory of the device holds the data used to generate the
binary graph and execute it :

-   the table of entry points to SWC, and the code. This data is
    different for each processor architecture of the device.

-   the **platform manifest** : a combination of the **description of
    the processor** and a "**platform_io_manifest**" for the details of
    the sensors used by the platform.

### 1.1 Platform manifests

The platform manifest is structure of bit-fields (20 Bytes min.)
holding:

-   **1.1.1 the list of services** delivered through CMSIS-Stream and
    its APIs, in two categories : the DSP/ML services and the HAL
    services. This allows SWC delivered in binary to use libraries
    without incorporating them in the code. One example of use-case is
    to let a code written for the architecture ARMv7-M to run on
    ARM-v8.2-M with acceleration.

    -   DSP/ML services : abstraction to a short-list of kernels from
        CMSIS-DSP (filtering, spectral analysis, linear algebra)

    -   Basic multimedia audio and image compression: abstraction to
        audio codecs

    -   HAL services: interface to IO (LED, Digit, GPIO), storage and
        timer services

-   **1.1.2 processor characteristics** : number of processors and
    pointer to description of each, number of different architectures
    and a pointer to the table of description, the memory bank used for
    the shared memory access

    -   The processor and architecture tables give:

        -   The processor architecture (ARMv6-M, ARMv7-M.. ARMv9-A)

        -   The architecture options (FPU options, Vector processing
            options)

        -   The indexes used to differentiate each processor.

        -   The memory banks (above) able to be addressed.

    -   CMSIS-Stream uses several virtual memory banks for the memory
        allocation of SWC. The responsibility of the application is to
        map those names to physical memory :

        -   "Internal memory"

        -   "External memory" with the slowest speed constraint

        -   "IO data memory" reserved for exchanges with peripherals

        -   "Program memory",

        -   "fast data memory" for DTCM of Cortex-M or LLRAM of Cortex-R

        -   "Graph memory" only addressed by CMSIS-Stream

        -   "Backup memory" used to save critical data fast restore on
            warm boot.

-   **1.1.3 The number of I/O** interfaces and the way to use them
    (paragraph 1.2)

### 1.2 Platform IO manifest

The platform_io_manifest is a structure of bit-field (28 Bytes min.)
holding :

-   **Four characters** used for debug and trace purpose identifying the
    IO interface.

-   **1.2.1 The "domain"** of the IO interface. This information is used
    differentiate the different tuning parameters of the sensors /
    transducers (see A.3.2 Domain-specific stream data types) :

    -   **Audio recording** stream, and the control of the number of
        channels, sampling rate, frame size, acoustic sensitivity, gain
        and automatic gain control, high-pass filtering, routing

    -   **Audio playing** stream, and the control of the number of
        channels, sampling rate, frame size, voltage sensitivity, gain
        and automatic gain, three bands frequency and gain control,
        routing

    -   **Motion recording** stream to control the number of channels
        (accelerometer, gyroscope, magnetometer, metadata), sampling
        rate, frame size, sensitivity, average, vector magnitude
        computation, metadata (free fall, shock, tap, transient
        detection..)

    -   **Generic data moves** used streams generated by the application

    -   Electrical domain, *TBD*, synchronous I/V sensing for motor
        control.

    -   Position domain, *TBD*, linear, angular, rotary position sensor

    -   Environmental domain, *TBD*, temperature, light, humidity, air
        quality

    -   Chemical and agricultural sensors, *TBD*

    -   Image sensor, *TBD*, X-Y control of the point of focus, zoom
        control.

    -   Image codec stream, *TBD*, codec ID, initial padding and delay,
        vbr control, data rate

    -   Audio codec and MIDI stream, *TBD*, packing type, bit rate, dual
        mono mode, LFE audio, BFI flags, fast forward, final padding,
        VBR control

-   **1.2.2 The device driver capabilities**:

    -   **data flow initialization** either made by the hardware (device
        driver is master, for example with the frames from an audio
        Codec) or initiated by the graph (device driver is follower, for
        example when polling the temperature from application requests).

    -   **buffer allocation**: tells if the buffer used for the
        communication with CMSIS-Stream is reserved by the device driver
        or needs to be allocated during the graph initialization steps,
        and in which platform memory area (one of the memory banks given
        in "1.1 Platform manifests").

-   **1.2.3 The default digital stream setting** (8 Bytes) : see "A.3
    Stream digital data types" for the description of the bit-fields
    giving the frame-size, data interleaving configuration, the raw data
    format in the frame, the number of channels, the sampling rate and
    the optional time-stamping format.

-   **1.2.4 The ID of the hardware port** used as parameter of the
    callback to notify the graph of new data (receive), or completion of
    the previous transfer (transmit).

-   **1.2.5 The default sensor configuration** (8 Bytes) related to the
    domain (see above). This is a table of configuration controls
    structured by lines (number of fields, data). To initialize a data
    stream at the boundary of the graph, the graph description text file
    gives the bit-fields of the desired configuration, for the default
    configuration of the platform firmware. In the example below a
    microphone path can be configure with the number of channels (3 bits
    on the LSB, 8 configurations possible), the manifest proposes 3
    options with 1, 2 and 8 channels. The two bits (4 options) allocated
    to the frame-size options have only 2 options: 10ms and 16ms frame
    lengths :

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
> /\*14 bits remain \*/
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

-   Bit-field of dependencies to CMSIS-Stream for DSP/ML and HAL
    services (see 1.1 Platform manifests)

-   The developer identification, 21 bits

-   The processor architectures and architecture options the component
    is compatible with

-   **The SWC identification code and revision**

-   A short name for debug, 4 characters

-   A flag telling if the SWC data instance is relocatable and/or can be
    moved between calls (the SWC is always reading instance data with a
    reference to the base address of the instance), and if the code and
    it's the inline assembly is made ready for "position independent
    execution".

-   **The stream format of each input and output stream** of the
    components (see "A.3 Stream digital "data formats\""), giving the
    default values. The stream format field "frame size" is used to
    define the minimum amount of data (in Bytes) needed on input streams
    before trying to call the SWC. Or the minimum amount of free space
    to reserve on the output buffer.

-   The allowed stream deviations from the above default value,
    concerning the maximum number of channels and the sampling rate.

The short format of SWC manifest stops here, and continues for normal
and binary formats:

-   The number of **parameter presets**, on 6 bits (64 parameter presets
    possible)

-   The optional need to reset the component after loading new
    parameters.

-   The size of the parameter presets, on 18 bits (\<256kBytes)

-   A pointer to the beginning of the array of parameter presets

-   A pointer to the test pattern, expected results, the configuration
    and replay options, *TBD*

-   A key to unlock the SWC or the full feature list, *TBD*

The binary manifest format are using offsets instead of pointers.

## 2) The graph boundaries

The below IO protocol facilitates code portability when switching
between hardware platforms. The firmware of the device proposes 3
functions ("set", "start", "stop") for all interfaces. The protocol
described below hides to the graph scheduler the way the samples are
generated or consumed (DMA, USB, files of the application, ..).

The board designer validates the operations of its sensors and
transducers in several configurations. Those configurations are saved in
the "platform IO manifest" (see "1.2.5 The default sensor
configuration"). If the IO stream do not deliver the data in the format
desired by the consumer SWC then CMSIS-Stream GUI tool will insert a
node for the format conversion. This automatic conversion scheme applies
between SWC in the middle of the graph too.

Sequence of functions called between the CMSIS-Stream and the device
driver :

-   **1) \"init\"** The GUI holds a copy of the IO manifest and decides
    how the buffer is created and used for exchanges. The GUI sets the
    type of data move to operate once receiving the information from the
    device driver (data to copy to the ring buffers or let the SWC have
    the processing being made \"in place\").

-   **2) \"set\"** The scheduler calls the firmware with a "set\"
    command with the desired sensor configuration with parameters:

    -   the domain settings

    -   a proposed buffer address and size (if needed).

> \"RX\" means a flow of data from external sensor to the CMSIS-Stream,
>
> \"TX\" means a flow of data generated by the CMSIS-Stream to the
> external transducer.

-   **3) \"start / continue\"** The scheduler initiates the data
    transfers with a call to the firmware using "start" command with two
    parameters (the function returns a Boolean telling if the data
    transfer is possible):

    -   a pointer to the base address of buffer, the size in bytes to
        tell :

        -   **RX case** : \"this memory address and buffer size, is
            proposed for receiving new data\"

        -   **TX case** : \"this base address and buffer size is the
            first data to be transmitted\"

-   **4) \"ack\"** When new data is ready (or have just been transferred
    out), the device driver calls the application callback (which is a
    wrapper to "**arm_stream_io**(int,\*,int)") with three parameters :

    -   ID (the device driver index), a pointer to a base address of a
        buffer, the size in bytes to tell,:

        -   **RX case** : \"I prepared for you a buffer of data, copy
            the data or use it directly from this place, and for this
            amount of bytes\". The address can change from last callback
            for example in case the device driver is using a ping-pong
            buffer protocol (or more buffers).

        -   **TX case** : \"I have completed the transfer of this buffer
            with this amount of bytes you told me to move out, and you
            can refill this new (or the same) buffer for the next
            transfer\".

-   **5) \"stop\"** CMSIS-Stream stops the use-case with a call to
    firmware and "stop" command. The device driver acknowledges the end
    of on-going transfers with a call to the callback, with no
    parameter.

## 3) Linked list of SWC 

After translation from the text format, the binary graph structure holds
different data, in a shared memory space, with the linked-list (a table)
representation of the graph. This table will be continuously parsed by
the schedulers to find which SWC needs to be executed, and which with
higher priority from others.

The linked-list is a succession of data records : a 32bits header (H),
an index to the location of the SWC instance (I), and to the boot
parameters to use (B). And finally two pairs of arcs index (A). The
header tells if a compact representation is used (SWC with no instance,
no parameters, and only two arcs). The size varies 8 to 20bytes:

> ADDR LIST
>
> \-\-\-\-\-\-\-\--
>
> 0000 HHHH standard format
>
> 0004 IIII
>
> 0008 BBBB
>
> 000C A1A0
>
> 0010 A3A3
>
> 0000 HHHH compact format
>
> 0004 A1A0

This linked-list table gives the index of the SWC for the table of
entry-points and table of manifests, the index of the arcs connected to
this SWC. The linked-list is in RAM and the first word has 3 bits used
to pack the content to the minimum size possible. The data format is :

-   Header 32bits word

    -   12 bits to **index** the entry point of the component.

    -   7 bits to indicate if the component is executable on the current
        processor (4 bits for the processor ID, 3 bits for the
        architecture ID with the value 0 meaning "any" and 7 meaning
        this is a script file (see "Annexe B Scripts").

    -   2 bits to set debug options (counter of cache-miss of this SWC,
        patch the input for self-test, ..)

    -   1 bit (debug) to force the scratch memory area of the component
        to be cleared before execution

    -   1 bit to activate a verbose mode of execution trace

    -   3 bits of SWC execution **status** :

        -   swc_status_boot : during memory reset and

        -   swc_status_reset : SWC reported its memory allocation
            requests.

        -   swc_status_idle : SWC instance was created and initialized,
            not running.

        -   swc_status_active : SWC is running.

        -   swc_status_need_rerun: SWC returned but was partly executed
            to let higher priority tasks run for situation where there
            is no RTOS

        -   swc_status_stopped: disabled.

    -   1 bit to pack the SWC description for 2 arcs.

    -   1 bit to tell there a boot parameter array index (32bits) to use
        at reset time

    -   1 bit to tell the SWC needs or not a MEMREQ initialization if
        there is no need for declaration of an instance (no instance, no
        static memory used for this component)

-   Optional **instance** index to a memory space of the binary graph
    structure.

-   Optional index to a set of parameters.

-   The first two 16 bits **arc descriptors** with the format :

    -   12 bits to index the arc descriptor in the binary graph
        structure (see "3.2 ARC descriptors")

    -   1 bit "**ready flag**" used accelerate the scheduler decision :
        for input arcs of the SWC the flag is set when at least a
        minimum of data is available in the buffer. For output arcs of
        the SWC the flag is set when at least a minimum of free area is
        available in the output buffer.

    -   1 bit "**high QoS flag**" indicating the SWC can be executed
        even if the other streams do not have yet their "ready flag"
        set. The arcs descriptors with flow errors give indications on
        the operation to do to interpolate the data.

The main loop of the scheduler consists in checking the list of ring
buffers at the boundary of the graph, and activating the firmware for
data exchanges. Then, parsing the linked-list, finding the SWC having
its "ready flags" set and call it, until the end of the table, loop
until the situation is stable and return to the caller.

### 3.1 SWC interface

The SWC have a single entry point in the format "func (int, \*,\*,\*)".
The first parameter is the execution command (memory request, reset, set
parameters, run, stop). The SWC can call CMSIS-Stream through a
dedicated function pointer provided at reset time. An example of SWC API
[here](https://github.com/ARM-software/EndpointAI/blob/master/Kernels/Research/CMSIS-Stream/SWC_sample.c).

### 3.1.1 SWC parameter "MEMREQ"

The first operation asked by the scheduler is to ask the SWC for memory
allocation with respect to parameters associated to the input and output
stream format (the SWC may ask for working memory buffer size in
relation with the frame size of the streams).

A SWC delivered in source-code, or as library object using the same
compilation tool chain as the application, can use memory allocation
function from the standard C library (malloc(), realloc(), calloc()),
and will have to manage the "free()" deallocation upon reception of the
command "STREAM_END".

The format is : func (STREAM_MEMREQ, \*ptr1, \*ptr2, \*ptr3):

-   The first pointer is a memory space of 7 words of 32bits. The SWC
    will fill this area with up to 6 memory allocation requests
    terminated with "0". Each word is a bit-field (description in "A.4
    Memory types") giving the size of the memory buffer, the byte
    alignment and the recommended speed. The memory can be declared
    "static" or "working"(or "scratch memory area"), depending if the
    content needs to be preserved between two calls. The first memory
    request is the "instance", which holds pointers to static and
    working memory buffers. The pointer to this memory area is reused in
    all the other SWC commands.

-   The second parameter is pointing to a table of the stream formats
    used (see "A.3 Stream digital "data formats\""). This information
    (buffer size, sampling rate, interleaving scheme) can be used by the
    SWC to adjust the request to the minimum amount of memory.

-   The last parameter is *TBD* and reserved for a SWC activation
    protocol with key exchanges

### 3.1.2 SWC parameter "RESET"

The second operation of the scheduler is to provide the SWC with the
memory being allocated.

The format is : func (STREAM_RESET, \*ptr1, \*ptr2, \*ptr3):

-   The first parameter points to the SWC instance, with memory
    allocation corresponding to the first word of the STREAM_MEMREQ. The
    following data is a vector of pointers corresponding the memory
    allocation requested in the same following order provided by the
    STREAM_MEMREQ.

-   The second parameter is a pointer to the entry point of
    CMSIS-Stream, and giving access to optional services in computing,
    signal compression. There is a protocol *TBD* to activate this link
    : the SWC will use a single subroutine as calling address and will
    register the return address (seen by CMSIS-Stream) with a dummy call
    during this initialization sequence.

-   The last parameter is unused.

### 3.1.3 SWC parameter "SET_PARAMETER"

CMSIS-Stream is setting the SWC parameter at reset time with the default
reset parameter vector provided in its manifest. This API allows to
change a single parameter or the full set.

The format is : func (STREAM_SET_PARAMETERS, \*ptr1, \*ptr2, \*ptr3):

-   The first parameter "ptr1" points to the SWC instance.

-   The second parameter points to the parameters to be updated

-   The last parameter will be casted to integer and the LSB 9 bits tell
    the index or the tag used by the SWC documentation to change one
    parameter. The value 256 tells the full parameter list will be set.

The scheduler has no way to decide to change a parameter during the
execution of the graph. The **Scripts** are used for this purpose.

### 3.1.4 SWC parameter "READ_PARAMETER"

The scheduler is reading the SWC parameter at reset time with the
default reset parameter vector provided in its manifest. This API allows
to change a single parameter or the full set.

The format is : func (STREAM_READ_PARAMETERS, \*ptr1, \*ptr2, \*ptr3):

-   The first parameter "ptr1" points to the SWC instance.

-   The second parameter points to the parameters to be updated

-   The last parameter will be casted to integer and the LSB 9 bits tell
    the index or the tag used by the SWC documentation to change one
    parameter. The value 256 tells the full parameter list will be set.

### 3.1.5 SWC parameter "RUN"

The scheduler launches the execution of the SWC when the conditions,
found in the linked-list fields (processor architecture, arc's
ready-flags) are set. The stream of data from the arcs are exchanged in
the format detailed in the SWC manifest (see "A.3.1 Data format fields
common to all streams").\
The calling format is : func (STREAM_RUN, \*ptr1, \*ptr2, \*ptr3).

As previously "ptr1" is the instance pointer of the component. "ptr2" is
a pointer to a structure.

The \"\*ptr2\" field points to a structure : \[{data pointer stream1}
{data pointer stream2} .. \]

The \"\*ptr3\" field points to a similar structure : \[{parameter of
stream1} {parameter of stream2} .. \]

Simple components have two streams : one as input, the other as output
other can have up to 4 streams (several input/output combinations).

A data buffer is combination of a pointer and size. For input streams
the size is the amount of data in the buffer. For output streams this is
the amount of free space in the output buffer starting from the pointer
address,

The SWC is updating the base address pointers and data sizes before
returning to caller.

When a stream data format is **FMT_INTERLEAVED**, (for example Left and
Right audio samples are in this order : LRLRLRLRLRLR .. ) then {data
stream} is a pointer to the base address, {parameter stream} is the
number bytes in the buffer.

When a stream data format is **FMT_DEINTERLEAVED_1PTR**, (for example
Left and Right audio samples are in this order : LLLL..LLLLRRRR\...RRRR
) the size of the first buffer (the \"frame\") then {data stream} is a
pointer to the base address, {parameter stream} is the number bytes for
a single frame (the size of the Left sample portion). The SWC will
address the second and following channels by incrementing the base
pointer address with the size of the frame.

When a stream data format is **FMT_DEINTERLEAVED_NPTR**. The buffers
have independent positions (for example color planes of images). The
{data stream} is a combination of the pointer to the base address, and
the size of the corresponding buffer. {parameter stream} is unused. For
example with stereo audio : {data stream} points to a structure : \[
{\*ptr_L, size L}, {\*ptr_R, size R} \], this is the format used in
**EEMBC-audiomark**.

### 3.1.6 SWC parameter "END"

This command is used to free the allocated memory. The format is : func
(STREAM_END, instance pointer, 0, 0).

### 3.2 ARCs descriptors

Two types of arcs are used in the graph : simple linear buffers and ring
buffers. Simple arcs are described with two words of 32bits, Ring
descriptors are using 6 words (the first 2 words are identical to the
ones of simple arcs) with the purpose of managing complex situation with
peripherals, multiprocessing, drift compensations, data monitoring. Ring
buffers are used at the boundary of the graph, their content is
realigned to the base address to avoid the SWC to manage folding
addresses. The data format is:

-   Word 0 : 27 bits for the buffer base address and 5bits for the data
    format

    -   The base address is computed in 3 sub-fields of a linear 22bits
        address shifted with an exponent of E = 2bits and 3bits
        selecting one of the 7 memory banks (see "1.1.2 processor
        characteristics"). Each CMSIS-Stream instance (see "Two entry
        points b) STREAM_RESET") has its table of offset to physical
        memory banks. The base address value is
        physical_address\[offset\] + (linear address \<\< (E\<\<2)).

    -   The 5bits data format field is a compact way to give the data
        format details (see "A.3.1 Data format fields common to all
        streams") which is 128bits long. This data is an index to the
        table of data formats used in the graph. This table is part of
        the shared binary graph structure.

-   Word 1 : 24bits buffer size, 2bits to select the arc type, 8 bits
    for debug

    -   The buffer length is computed with a 22bits linear address and a
        2 bits shifter to extend the length.

    -   The 2 bits selector give the indication of linear or ring
        descriptor format

    -   For debug, 4bits are ging the debug task to proceed, and 4 bits
        to select the debug register of the result. The debug register
        array is in the shared binary graph structure. The debug tasks
        are *TBD* and could be : estimation of the data rate, the
        time-stamp of the last acces, the peak / min / absmax data
        values with different forgetting factors.

-   Word 2: read index on 24bits, flow management on 4bits, data
    alignment decision bit

    -   The read index is a plain 24bit index in Bytes, and allows to
        manage 16MB buffers

    -   There are 2bits for underflow and 2bits for overflow management
        during read/write access. The decision thresholds ("crumb_in",
        "crumb_out") are given in the Word-3. The underflow options are
        : repeat last frames, generate zeroes (default), extrapolate
        last frame. The overflow options are : skip last frame,
        interpolate last frame.

    -   To avoid data to fold when the write pointer is too close to the
        top of the buffer a data realignement will be proceeded based on
        a selection of '0' the crumb-in threshold, '1' the crumb-out
        threshold.

-   Word 3: write index on 24bits, 4 bits of crumb_in, crumb_out

    -   The write index is a plain 24bit index in Bytes. Buffer full
        condition corresponds to (W-R) = (Size-1).

    -   Crumb_in (2bits) is used to set the "ready flag" (see "3) Linked
        list of SWC") of the consumer SWC when the amount of data in the
        buffer is either larger the buffer size /2 /4 /8 /16.

    -   Crumb_out (2bits) is used to set the "ready flag" (see "3)
        Linked list of SWC") of the producer SWC when the amount of free
        area in the buffer is either larger the buffer size /2 /4 /8
        /16.

-   Word 4: offset (19 bits) to the linked-list header of the SWC
    consumer of this arc.

-   Word 5: Debug address field (27bits, format of the base address)

## A Data types

### 

### A.1 Raw data types

Sample of raw data type
[here](https://github.com/ARM-software/EndpointAI/blob/master/Kernels/Research/CMSIS-Stream/Stream_type.h).

### A.2 Array of Raw data types

When the raw data type is null, the next byte is the raw data type and
the next 2 bytes are the number of raw data as array.

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
    minimum of free area to have in the output buffer to avoid overflow
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

-   Raw data format, on 6 bits, see "A.1 Raw data types".

The second word of the common data format has the fields:

-   Number of channels minus one, on 5 bits, allowing up to 32 channels.

-   Sampling rate, on 21 bits, the value 0 means "asynchronous or slave
    IO port". The data format is F=M x 2\*\*E. With 19 bits for the
    unsigned mantissa "M", 2 bits for the exponents (values of "E" =
    0,-8,-16,-24), allowing to set the rate from 1 week period up to
    524kHz, with all the common audio and voice sampling rate with full
    accuracy.

-   Time-stamp format, on 2bits:

    -   No timestamp on the data frames

    -   Absolute value of the time (64bits) on each frame, 2 MSB to
        control the format, 40 bits counting the SYSTICK periods of 1ms,
        16 bits computed from SYST_RVR to have the fraction of 1ms.

    -   Relative time from last frame (32 bits), 30bits of data and 2
        MSB to control the format (0: seconds spent from reset, 1:
        seconds spent from January 1^st^ 2023, 2: seconds interval in
        U14.16 format, 3: seconds interval in U24.4 format).

-   Four bits unused

#### A.3.2 Domain-specific stream data format

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

Memory requests bit-fields :

-   Size 20bits : 18bits linear and 2bits shifter. The size is
    (linear-18b) \<\< (shift x 4). Allowing a size range up to 1GB.

-   Buffer byte alignment 3bits

-   Speed memory requirement 2bits

-   Data/Program 1bit

-   Working/static 2bits

enum mem_speed_type memory requirements associated to enum memory_banks

-   MEM_SPEED_REQ_ANY = 0, best effort

-   MEM_SPEED_REQ_NORMAL = 1, can be external memory

-   MEM_SPEED_REQ_FAST = 2, will be internal SRAM when possible

-   MEM_SPEED_REQ_CRITICAL_FAST = 3, will be TCM when possibl

enum mem_mapping_type

-   MEM_TYPE_STATIC = 0, (LSB) memory content is preserved (default )

-   MEM_TYPE_WORKING = 1, scratch memory content is not preserved
    between two calls

-   MEM_TYPE_PSEUDO_WORKING = 2, static only during the uncompleted
    execution state of the SWC, see "NODE_RUN"

-   MEM_TYPE_PERIODIC_BACKUP = 3, static parameters to reload for warm
    boot after a crash, holding for example

    -   long-term estimators. This memory area is cleared at cold
        NODE_RESET and

    -   refreshed for warm NODE_RESET. The SWC should not reset it
        (there is

    -   no \"warm boot reset\" entry point.

    -   the period of backup is in the range of 15 seconds

};

enum buffer_alignment_type

-   MEM_REQ_4BYTES_ALIGNMENT = 0, 0 placed here as default value

-   MEM_REQ_8BYTES_ALIGNMENT = 1,

-   MEM_REQ_16BYTES_ALIGNMENT = 2,

-   MEM_REQ_32BYTES_ALIGNMENT = 3,

-   MEM_REQ_64BYTES_ALIGNMENT = 4,

-   MEM_REQ_128BYTES_ALIGNMENT = 5,

-   MEM_REQ_NOALIGNMENT_REQ = 6, address binary mask :

-   MEM_REQ_2BYTES_ALIGNMENT = 7, mask = \~((1 \<\<
    (7&(mem_req_2bytes_alignment+2)) -1)

## B Scripts 

*TBD*

*Scripts provide a simple mean of taking decision, defining
state-machines, read and set parameters of SWC, trigger actions for the
application. Scripts are an interpreted language limited to very few
commands (inspiration from
[here](https://en.wikipedia.org/wiki/FOCAL_(programming_language)#Comparison_with_BASIC)
and [here](https://en.wikipedia.org/wiki/TI-57)), and with only the
capability to call CMSIS-Stream services.*

*Linked-List description for scripts :*

-   *Word 0 "header" : (32bits) , with arch=7*

-   *Word 1 "header2" : size of the script, register use*

-   *text script to be interpreted*

## C SWC delivery

A SWC delivery consists in :

-   a description of the parameter, and some presets

-   the input test pattern and expected results

-   a documentation for the GUI

-   the code of the SWC (source or binary) using one entry point with
    the template func(int,\*,\*,\*);
