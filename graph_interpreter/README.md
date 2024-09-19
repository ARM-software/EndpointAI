## PRELIMINARY DATA -- IN DEVELOPMENT 

***WHAT***

**Graph-Interpreter** is a scheduler of **DSP/ML nanoApps** designed with three objectives: 

1. **Accelerate time to market**

Graph-Interpreter helps system integrators and OEM who develop complex DSP/ML stream processing. It allows going fast from prototypes validated on a computer to the final tuning steps on production boards, by updating a graph of computing nodes and their coefficients without device recompilation.  

2. **NanoApps repositories**

It provides an opaque interface of the platform memory hierarchy to the computing nodes. It arranges the data flow is translated to the desired formats of each node. It prepares the conditions where nodes will be delivered from a Store. 

3. **Portability, scalability**

Use the same stream-based processing methodology from devices using 1 Kbytes of internal RAM to multiprocessor heterogeneous architectures. Nodes can be produced in any programming languages. The Graph are portable when interpreted on another platform.

 

 **Use-case example :** 

 **Tuning interfaces for AI preprocessing**

Example of a graph implementing a cascade of DSP/ML algorithms and signal feature extraction before using a classifier (NPU). The system integration task consists in tuning the signal levels and the coefficients of several filters. The system integrator tunes the nodes in charge of rescaling and triggers a GPIO based on level detection. The system integrator updates the parameters of the nodes without recompilation. The memory mapping of the node is tuned on target without recompilation. The task dispatching between processors is tuned for performance optimization, without code recompilation.

-----------------------------------------------------------------------------

***HOW***

Graph-Interpreter is a scheduler and interpreter of a graph description,
sitting on top of a minimal platform abstraction layer to memory and
input/output stream interfaces. Graph-Interpreter manages the data flow
of "arcs" between "nodes".

The *graph description* is a compact data structure using indexes to tables of 
physical addresses provided by the abstraction layer (AL). This graph
description is generated in three steps:

1.  **A graphical user interface** (GUI) generates a high-level
    description of the connexions between nodes and with the platform
    data streams. This graph representation is using the *YML format*
    used in the CMSIS-DSP ComputeGraph
    ([link](https://github.com/ARM-software/CMSIS-Stream)),
    with extra fields to set board's presets. "No code" is objective number 1 of the project: go
    directly from this GUI to tests on target, without recompilation of
    the code running in target .

2.  **A targeted platform is selected with its associated list of
    "*manifests*"**. Each platform using Graph-Interpreter has a
    "*platform manifest*" giving the processing details (processor
    architecture, minimum guaranteed amount of memory
    per RAM blocks and their speed, TCM sizes). And "*nanoApp manifests*" for each of the installed processing Nodes,
    giving the developer identification, input/output data formats,
    memory consumption, documentation of the parameters and a list of
    "presets", test-patterns and expected results. This second
    processing step generates the text file representation of the graph
    ("*graphTxt*") used by the scheduler, it is in a readable format and can be manually modified.

3.  **Finally, the binary file used by the target is generated**. The
    file format is either a C source file, or a *binary* hashed data to
    load in a specific flash memory block, to allow quick tuning without
    need of recompilation.

**The abstraction layer (AL) provides the following services:**

1.  **Share the physical memory map base addresses**. The graph is using
    indexes to base addresses of 8 different *memory types*: shared
    external memory, fast shared internal, fast private per processor (TCM). The AL
    has access to the entry points of the nodes installed in the
    device.

2.  **Call the graph boundary functions generating/consuming data
    streams,** declared in the platform manifest and addressed as
    indexes from the scheduler when the FIFOs at the boundary of the
    graph are full or empty.

3.  **Share time and low-level information** **for scripts**. The graphTxt embeds byte-codes of
    "Scripts" used to implement state-machines, to change nodes
    parameters, to check the arcs data content, trigger GPIO connected
    to the graph, generated strings of characters to the application,
    etc. The  

Graph-Interpreter is delivered with a generic implementation of the
above services, with device drivers emulated with precomputed data, time
information emulated with systick counters.

------------------------------------------------------------------------------

***DETAILS***

Stream-based processing is facilitated using Graph-Interpreter:

1.  Nodes can be written in **any computer languages**. The scheduler is
    addressing the nodes from a single entry point, respecting the
    Graph-Interpreter 4-parameters API format (examples below). There is
    no restriction in having the nodes delivered in binary format,
    compiled with "**position independent execution**" option. There is
    no dynamic linking issue: the nodes delivered in binary can still
    have access to a subset of the C standard libraries through a Graph-Interpreter
    service. The nodes are offered the access to DSP/ML CMSIS kernels
    (compiled without the position-independent option) : the execution
    speed will scale with the targeted processor capabilities without
    recompilation.

2.  **Drift management.** The streams don't need to be perfectly
    isochronous. Rate conversion service is provided by the scheduler
    when the sampling conversion can be expressed in ratio of integers.
    The scheduler defines different quality of services (QoS) when a
    main stream is processed with drifting secondary streams. The
    time-base is adjusted to the highest-QoS streams (minimum latency
    and distortion), leaving the secondary streams managed with
    interpolators in case of flow issues.

3.  Graph-Interpreter manages **TCM access**. When a nanoApp declares, in its manifest, the need for a "critical speed memory
    bank" of small size (ideally less than 16kBytes), the above "step 2,
    generation of *graphTxt* " will allocate TCM area.

4.  **Backup/Retention SRAM**. Some applications are requiring a fast recovery in
    case of failures ("warm boot") or when the system restores itself
    after deep-sleep periods. One of the 8 memory types allows
    developers to save the state of algorithms for fast return to normal
    operations.

5.  *graphTxt* translation to binary allows memory size optimization
    with overlays of different nodes' working/scratch memory banks.

6.  **Multiprocessing** SMP and AMP with 32+64bits. The graph
    description is placed in a shared memory, and any processor having
    access to the shared memory can contribute to the processing. The
    node reservation protocol is lock-free. The nodes are described with
    bit-fields to map execution on specific processor and architectures.
    The buffers associated to arcs can be allocated to processor's private
    memory-banks.

7.  **Scripting** are designed to avoid going back and forth with the
    application interfaces for simple decisions, without the need to
    recompile the application (Low-code/No-code strategy, for example
    toggling a GPIO, changing node parameter, building a JSON
    string\...) the graph scheduler interprets a compact byte-stream of
    codes to execute simple scripts.

8.  **Process isolation**. The nodes never read the graph description
    data, and the memory mapping is made for operations with hardware
    memory protection.

9.  **Format conversions**. The developer declares, in the manifests,
    the input and data formats of the node. Graph-Interpreter makes the
    translation between nodes: sampling-rates conversions, changes of
    raw data, removal of time-stamps, channels de-interleaving. Specific conversion nodes are inserted in the graph during its binary file translation.

10. Graph-Interpreter manages the various **methods of controlling I/O**
    with 3 functions: parameters setting and buffer allocation, data
    move, stop. The platform manifest mixed-signal components settings,
    flow-control). This abstraction layer facilitates the control of
    streams with DMA or polling schemes.

11. The **tiny wrapper** format allows having a single interface (as
    experimented in EEMBC "AudioMark") for all the nodes, including
    those having multiple arcs connexions.

12. Graph-Interpreter is **open-source**, and portable to Cortex-M,
    Cortex-R, Cortex-A and computers.

13. Example of nanoApp: image and voice codec, data
    conditioning, motion classifiers, data mixers. Graph-Interpreter
    comes with short list of componets doing data routing, mixing,
    conversion and detection.

14. **From the developer point of view**, it creates opaque memory
    interfaces to the input/output streams of a graph, and arranges data
    are exchanged in the desired formats of each nanoApp.
    Graph-Interpreter manages the memory mapping with speed constraints,
    provided by the developer, at instance creation. This lets software
    run with maximum performance in rising situations of memory bounded
    problems. Graph-Interpreter accepts code in binary format activated
    with keys (TBC). It is designed for memory protection
    between nanoApps.

15. **From the system integrator view**, it eases the tuning and the
    replacement of one nanoApp by another one and is made to ease
    processing split with multiprocessors. The stream is described with
    a graph (a text file) designed with a graphical tool. The
    development of DSP/ML processing will be possible without need to
    write code and allow graph changes and tuning without recompilation.

16. **Graph-Interpreter design objectives**: Low RAM footprint. Graph
    descriptor can be placed in Flash with a small portion in RAM.
    Use-cases go from small Cortex-M0 with 2kBytes RAM to SMP/AMP/coprocessor and mix of 32/64bits thanks to the concept of
    shared RAM and indexes to memory banks provided by the local
    processor abstraction layer. Each arc descriptors can address buffer sizes of up to 1GBytes. 
    
17. **Computing libraries** are provided under compilation options, to
    avoid replicating CMSIS-DSP in nodes with binary code deliveries,
    and to allow arm-v7M codes able to benefit from arm-v8.1M vector
    processing extensions without the need for code recompilation.
