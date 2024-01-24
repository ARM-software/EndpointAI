## Table of contents

- Introduction
- Use-cases	
- Graph-description
- nanoApps	
- Scheduler	
- Graph creation	
- Create a nanoApps



## Introduction 

This document details the implementation of a graph of nodes (computing
elements called "**nanoApps**") connected with directed edges (called
"**arcs**" or "buffers"). The graph interfaces with the data streams at
its boundaries are called "**IO-interfaces**".

## Use-cases

CMSIS-Graph is a scheduler and interpreter of a graph of nanoApps,
designed for the following typical use-cases:

-   **Last minute tuning**. A graph of nanoApps needs to be quickly
    updated with new parameters and the insertion of a nanoApp, without
    recompiling the application.

-   **Low-code tuning**. A GPIO is triggered upon a change of energy
    level in an input stream, or a state of a nanoApp, without
    recompiling the application.

-   **Language agnostic**. The nanoApps are developed in various
    programming language and delivered in binary, with position
    independent compilation.

-   **Platform memory abstraction**. The nanoApps are delivered to the
    system integrator with memory requirements (static and scratch
    memory, speed and alignment requirements). For example a DTCM
    presence and size will be abstracted through opaque interface.

-   **Stream format conversion**. The nanoApps are delivered with
    manifests giving the data and stream formats at their interfaces.
    CMSIS-Graph is delivered with stream converters.

-   **Different physical domains**. The graph is mixing flows from
    different families of sensor (motion, temperature, audio, camera ..)
    and the stream formats are standardized at the IO-interfaces. For
    example a pressure sensor stream can be used to control the level of
    a sinewave generator in the audio domain.

-   **Data flows with drifts**. The graph is able to mix flows with flow
    error management. For example mixing streams of audio sources
    originated from different mixed-signal clock domains.

-   **Computing acceleration**. The scheduler proposes DSP and ML
    computing services to the nanoApps to facilitate performance scaling
    with code portability. For example a nanoApp compiled for Cortex-M4
    using FFT or convolutional networks will scale in performance on MVE
    architectures, without recompilation.

-   **Heterogenous architectures**. The graph is in a shared memory
    accessed by any processor of any architecture. The graph data
    structures are not using absolute addresses.

-   **Multiprocessing**. The graph allows some nodes to be executed from
    a specific processor, a specific architecture, or a specific task of
    an RTOS. For example, some nodes will have low-latency execution
    constraints, others can be executed in low-priority threads.

-   **Low memory consumption**. One targeted objective is to execute
    simple machine-learning operations on extreme low-cost platforms.
    For example a graph which consists of a DSP filter, a signal to
    noise detection and GPIO triggered on data thresholds can run on a
    device having 2kBytes of internal RAM and 16kBytes of Flash.

The CMSIS-Graph scheduler has one entry point. It takes four parameters:
a command (reset, run, set parameters), data pointers, length of data
and pointer the state of the scheduler:

-   The **Graph-Description** which is a table of the relations between
    nanoApps and arcs.

-   The **Abstraction-Layer** ("AL") interface to the platform
    capabilities and streams at the boundary of the graph.

-   A list of custom subroutines provided by the application
    ("**Application-Callbacks**").

-   The list of pointers to the nanoApps entry points.

## Graph-description

The graph is a table of 32bits words interpreted by a nanoApp scheduler.
There are seven sections in the graph:

1.  **Header.** Size of the following sections, their memory destination
    in the platform

2.  **IO-interfaces.** Index of the arc interfacing the graph
    ("arc-ID"), format of the data and the stream setting, operation to
    do for initiating data exchanges

3.  **data and stream formats**. Description of the raw sample format,
    frame-size, interleaving and time-stamp formats, sampling-rate,
    mapping of channels

4.  **scripts of byte codes**. Interpreted scripts used to control the
    flow, the parameters of nanoApps and implement state-machines

5.  **nanoApps** and their parameters used at reset time. Each nanoApp
    is described with an identification code, list of arcs it is
    connected to, index to the memory banks reserved for the execution,
    the preset and parameters to use at initialization time, the
    processor, RTOS thread and architecture it is assigned to

6.  **Buffer description of arcs**. Base address, length, read and write
    index, format of data, operation to execute for debug and during
    flow error

7.  **Memory sections**. Buffers and nanoApp scratch and static memory
    areas.

The scheduler receives a pointer to the Graph-description and interprets
it. It returns to the application caller when the data flow is completed
or after execution of each nanoApp.

Example of graph
[here](https://github.com/ARM-software/EndpointAI/blob/master/Nodes/Graph_Interpreter/graph_doc/graph_example.txt)

## nanoApps

A nanoApp is a subroutines (single entry point) with four parameters.
The declaration follows this template: *void nanoApp_name (int32_t
command, stream_handle_t instance, stream_xdmbuffer_t \*data, uint32_t
\*status)*

The "**command**" tells if the call is related to initialization/reset,
or reading/setting parameters, processing a stream of data or stopping
the processing.

The "**instance handle**" is a pointer to the static area of the
nanoApp.

The "**data**" is a structure of pointers and size of data for input and
output processed streams (see details [below](#create-a-nanoapps)).

## Scheduler

CMSIS-Graph has two entry points, one is called by the application for
the interpretation of the graph (*arm_graph_interpreter (command,
\*instance, \*data, size)*), the second is a call-back to notify the
transfer of data and to update the arcs at the boundary of the graph
(*platform_io_ack (IO, \*data, size)*).

The scheduler entry point has a prototype similar to nanoApps, with a
command telling to reset the graph, processing it and setting
parameters. The scheduler processing consists in scanning its
IO-interfaces, initiate data transfers in case of servant protocol.

IO-Interfaces operating in commander mode will call the call-back entry
point of the scheduler, to notify the readiness of new data, or to tell
the previous buffer was transferred, to update the read and write
indexes of the arc buffer descriptor.

Once the IO-interfaces are scanned, the scheduler is activating nanoApps
after checking they have enough data on the input arcs and output free
space on the output arcs.



## Graph creation

CMSIS-Graph makes no hypothesis on the method used to generate the graph
binary sequence described [here](#graph-description). A program is
proposed to translate an ASCII text file to the graph format.

The text to binary translation consists in reading the "manifests" files
of the processors (the memory map and processors details), of the
IO-interface peripheral (data and stream formats, available options for
set-up), and of the nanoApps (number of arcs and their characteristics,
memory allocation requirements).


## Create a nanoApps

Each nanoApp is seen from the scheduler as a pointer to a function using
the same prototypes : void nanoApp_name (int32_t command,
stream_handle_t instance, stream_xdmbuffer_t \*data, uint32_t \*status).
The scheduler makes no assumption on how the code was generated : it can
result of source-code compilation, linking a library, a list of opcodes
resulting from compilation with independent position in any programming
language.

The "command" is a bit-field:

-   4 bits reserved for the action: reset, set / read parameters, run,
    stop

-   4 bits for the number of data streams to process in the "data"
    structure

-   4 bits for the "**preset**" : the default values embedded in the
    code of the nanoApps. A "set parameter" command will patch the
    parameters on top preset.

-   8 bits for extra parameters

The "stream_handle" parameter has two meanings:

-   During a reset command it is a pointer to the list of memory
    addresses (of type \*\*intPtr_t, intPtr_t uint32_t or uint64_t
    depending on the processor architecture) requested by the nanoApp
    and described in its manifest. The first address of the list is the
    "instance": the static memory which differentiates this nanoApp for
    other replicas in the graph

-   For the other commands it is pointer to the instance (type
    \*intPtr_t)

The "stream_xdmbuffer" parameter has three meanings:

-   During the reset command it is function pointer to "**services**".
    The scheduler offers DSP, ML, a subset of standard libraries : stdio
    / stdlib / string / time / math, debug trace, and other functions
    reserved for scripts. The calls to services are initiated from a
    single place in the nanoApp to let the scheduler recover the return
    address and make security checks.

-   During "set parameters" commands it is an uint32_t\* pointer to the
    parameters to set. The first word is a header giving the "preset"
    configuration to consider before patching it with the new following
    parameters; the number of uint32_t words to read in the parameter
    set, and "tag" telling "all the nanoApp parameters will be updated"
    (code 0xFF) or "this specific parameter indexed from 0 to 0xFE will
    be updated". The following data is a byte stream of parameters.

-   For the other commands it is a pointer to the list of structures
    with a pointer to the data and the size of the data in Bytes. The
    number of structures corresponds to the number of arc given in the
    command. In usual situations the nanoApp has a fixed number of input
    and output arcs and the command field is not used.

The "status" field is used to tell the scheduler if the processing of
the nanoApp is terminated. If not, the scheduler has the opportunity to
scan the IO-Interfaces (usefull for platforms without RTOS) and it
returns to the nanoApp later for the termination of the processing.
