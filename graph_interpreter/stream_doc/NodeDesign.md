[TOC]
--------------------------------------
# Introduction
All the programs can be used with the Graph-scheduler (also called computing "nodes") as soon as a minimal amount of description is given in a "manifest" and the program can be used through a single entry point with a wrapper using the prototype :

```
void (node) (uint32_t command, stream_handle_t instance, stream_xdmbuffer_t *data, uint32_t *state);
```

Where "command" tells to reset, run, set parameters, ..  "instance" is an opaque access to the static area of the node, "data" is a table of pointers+size pairs of all the arcs used by the node, and "state" returns information of computing completion of the subroutine for the data being shared through the arcs.

During "reset" sequence of the graph the node are initialized. Nothing prevents a node to call the standard library for memory allocations or math computing. But the context of the graph interpreter is Embedded IoT, with extreme optimization for costs and power consumption. 

--------------------------------------

# General recommendations

General programming guidelines of Node : 

- Nodes must be C callable, or respecting the EABI.
- Nodes are reentrant, or this must be mentioned in the manifest.
- Data are treated as little endian by default.
- Data references are relocatable, there is no “hard-coded” data memory locations.
- All Node code must be fully relocatable: there cannot be hard coded program memory locations.
- Nodes are independent of any particular I/O peripheral, there is no hard coded address.
- Nodes are characterized by their memory, and MIPS requirements when possible (with respect to buffer length to process).
- Nodes must characterize their ROM-ability; i.e., state whether or not they are ROM-able (no self-modifying code unless documented so).
- Run-time object creation should be avoid : memory reservation should be done once during the initialization step.
- Nodes are managing buffers with pointer using physical addresses and shared in the parameters.
- Processors have no MMU : there is no mean of mapping physically non-contiguous segments into a contiguous block.
- Cache coherency is managed in Graph-Interpreter at transitions from one node to the next one.
- Nodes should not use stack allocated buffers as the source or destination of any graph services for memory transfer.
- Static/persistent data, retention data used for warm-boot, scratch data, stack and heap usage will be documented.
- Manifest is detailing the memory allocation section with respect to latency/speed requirements. 



# Node parameters

A node is using the following prototype 

```
void (node) (uint32_t command, stream_handle_t instance, stream_xdmbuffer_t *data, uint32_t *state);
```

With following parameters: 

| Parameter name | Details         | Types                                                        |
| :------------- | :-------------- | ------------------------------------------------------------ |
| command        | input parameter | uint32_t                                                     |
| instance       | instance        | void *                                                       |
| data           | input data      | pointer to struct stream_xdmbuffer { int address;  int size; } |
| state          | returned state  | uint32_t *                                                   |

## Command parameter

Command bit-fields :

| Bit-fields | Name     | Details                                                      |
| :--------- | :------- | :----------------------------------------------------------- |
| 31-24      | reserved |                                                              |
| 16-23      | node tag | different roles depending on the command. With "set parameter" it gives the index of the parameter to update from the data* address  (if 0 then all the parameters are prepared in data*) |
| 15-12      | preset   | the node can define 16 "presets", preconfigured sets of parameters |
| 11-5       | reserved |                                                              |
| 4          | extended | set to 1 for a reset command with warm boot : static areas need to be initialized except the memory segments assigned to a retention memory in the manifest. When the processor has no retention memory those static areas area cleared by the scheduler. |
| 3-0 (LSB)  | command  | 1: reset</br> 2: set parameter</br> 3: read parameter</br>4: run</br>5: stop</br>6: update a relocatable memory segment</br> |

## Instance

Instance is an opaque memory pointer (void *) to the main static area of the node. The memory alignment requirement is specified in the node manifest. 

## Data

The multichannel data field is a pointer of arcs' data. This is pointer to list of structures of two "INTPTR_T" (32bits or 64bits wide depending on the processor architecture). The first INTPTR_T is a pointer to the data, the second tells the number of bytes to process (for an input arc) or the number of bytes available in the buffer (for output arcs). 

In the default situations all the "size" fields are identical : the number of bytes to process is equal to the amount of memory free to take the computation result. But the node manifest can specify node_variable_rxtx_data_rate=1 and the scheduler may call the node with different "size" fields.

A node can have **4 arcs**. Each of them can have individual format (number of channels, frame length, interleaving scheme, raw sample type, sampling rate, time-stamps).

## Status

Nodes return state is "0" unless the data processing is not finished, then the returned status of "1".



# Node calling sequence

The nodes are first called with the command *reset* followed by *set parameters* and finally *stop* to release memory. This paragraph details the content of the parameters of the node during "reset", "set parameter" and "run" :

```
void (node) (uint32_t command, stream_handle_t instance, stream_xdmbuffer_t *data, uint32_t *state); 
```

## Reset command

**The second parameter "instance"** is a pointer to the list of memory banks reserved by the scheduler for the node, in the same sequence order of the declarations made in the node manifest. The first element of the list is the instance of the node, followed by the pointers to the data (or program) memory reservations.

After this list addresses the scheduler will push (when the manifest command *node_using_arc_format* = 1) the arc data formats of all the arcs. The arc format is described in the chapter "IO control and stream data formats" of the file "Graph design". </br>

**The third parameter "data"** is used to share the address of function providing computing services. 

## Set Parameter command

The bit-field "Node Tag" tells which (or all) parameter will be updated.

**The third parameter "data"** is a pointer to the new parameters.

## Run command

The bit-field "Node Tag" tells which (or all) parameter will be updated.

**The third parameter "data"** is a pointer to the list buffer ("struct stream_xdmbuffer { int address;  int size; }") associated to each arc connected to the node.



## Test-bench and non-regression test-patterns

Nodes are delivered with a test-bench (code and non-regression database).

# Node example

    typedef struct
    {   q15_t coefs[MAX_NB_BIQUAD_Q15*6];
        q15_t state[MAX_NB_BIQUAD_Q15*4];    
     } arm_filter_memory;
    
    typedef struct
    {   arm_filter_memory *TCM;
    } arm_filter_instance;
    	
    void arm_stream_filter (int32_t command, stream_handle_t instance, stream_xdmbuffer_t *data, uint32_t *status)
    {
    	*status = NODE_TASKS_COMPLETED;    /* default return status, unless processing is not finished */
    
    	switch (RD(command,COMMAND_CMD))
    	{ 
        /* func(command = (STREAM_RESET, COLD, PRESET, TRACEID tag, NB ARCS IN/OUT)
                instance = memory_results and all memory banks following
                data = address of Stream function
                
                memresults are followed by 4 words of STREAM_FORMAT_SIZE_W32 of all the arcs 
                memory pointers are in the same order as described in the NODE manifest
    
                memresult[0] : instance of the component
                memresult[1] : pointer to the allocated memory (biquad states and coefs)
    
                memresult[2] : input arc Word 0 SIZSFTRAW_FMT0 (frame size..)
                memresult[ ] : input arc Word 1 SAMPINGNCHANM1_FMT1 
                ..
                memresult[ ] : output arc Word 0 SIZSFTRAW_FMT0 
                memresult[ ] : output arc Word 1 SAMPINGNCHANM1_FMT1 
    
                preset (8bits) : number of biquads in cascade, max = 4, from NODE manifest 
                tag (8bits)  : unused
        */
        case STREAM_RESET: 
        {   
            uint8_t *pt8b, i, n;
            intPtr_t *memreq;
            arm_filter_instance *pinstance;
            uint8_t preset = RD(command, PRESET_CMD);
            uint16_t *pt16dst;
    
            /* read memory banks */
            memreq = (intPtr_t *)instance;
            pinstance = (arm_filter_instance *) (*memreq++);        /* main instance */
            pinstance->TCM = (arm_filter_memory *) (*memreq);       /* second bank = fast memory */
    
            /* here reset */
            pt8b = (uint8_t *) (pinstance->TCM->state);
            n = sizeof(pinstance->TCM->state);
            for (i = 0; i < n; i++) { pt8b[i] = 0; }
    
            /* load presets */
            pt16dst = (uint16_t *)(&(pinstance->TCM->coefs[0]));
            switch (preset)
            {   default: 
                case 0:     /* by-pass*/
                    pt16dst[0] = 0x7FFF;
                    break;
            }
            break;
        }       
    
        /* func(command = bitfield (STREAM_SET_PARAMETER, PRESET, TAG, NB ARCS IN/OUT)
                    TAG of a parameter to set, NODE_ALL_PARAM means "set all the parameters" in a raw
                *instance, 
                data = (one or all)
        */ 
        case STREAM_SET_PARAMETER:   
        {   uint8_t *pt8bsrc, i, numStages;
            uint16_t *pt16src, *pt16dst;
            int8_t postShift;
            arm_filter_instance *pinstance = (arm_filter_instance *) instance;
    
            pt8bsrc = (uint8_t *) data;
            numStages = (*pt8bsrc++);
            postShift = (*pt8bsrc++);
    
            pt16src = (uint16_t *)pt8bsrc;
            pt16dst = (uint16_t *)(&(pinstance->TCM->coefs[0]));
            for (i = 0; i < numStages; i++)
            {   /* format:  {b10, 0, b11, b12, a11, a12, b20, 0, b21, b22, a21, a22, ...} */
                *pt16dst++ = *pt16src++;    // b10
                *pt16dst++ = 0;             // 0
                *pt16dst++ = *pt16src++;    // b11    
                *pt16dst++ = *pt16src++;    // b12
                *pt16dst++ = *pt16src++;    // a11
                *pt16dst++ = *pt16src++;    // a12
            }
    
            stream_filter_arm_biquad_cascade_df1_init_q15(
                &(pinstance->TCM->biquad_casd_df1_inst_q15),
                numStages,
                (const q15_t *)&(pinstance->TCM->coefs[0]),
                (q15_t *)&(pinstance->TCM->state),
                postShift);
            break;
        }


        /* func(command = STREAM_RUN, PRESET, TAG, NB ARCS IN/OUT)
               instance,  
               data = array of [{*input size} {*output size}]
    
               data format is given in the node's manifest used during the YML->graph translation
               this format can be FMT_INTERLEAVED or FMT_DEINTERLEAVED_1PTR
        */         
        case STREAM_RUN:   
        {
            arm_filter_instance *pinstance = (arm_filter_instance *) instance;
            intPtr_t nb_data, stream_xdmbuffer_size;
            stream_xdmbuffer_t *pt_pt;
            int16_t *inBuf, *outBuf;


            /* the node is declared with node_variable_rxtx_data_rate=0, there is no need to update stream_xdmbuffer_t after processing */
            pt_pt = data;   inBuf = (int16_t *)pt_pt->address;   
                            stream_xdmbuffer_size = pt_pt->size;  /* data amount in the input buffer */
            pt_pt++;        outBuf = (int16_t *)(pt_pt->address); 
            nb_data = stream_xdmbuffer_size / sizeof(int16_t);
    
            /* .. */
            break;
        }
    
        case STREAM_STOP:
        case STREAM_READ_PARAMETER:
        case STREAM_UPDATE_RELOCATABLE:
        default : break;
    }


# Conformance checks

Purpose: create an automatic process to incorporate new NODE in a large repository and have a scalable mean to check conformance: 

- verification of the conformance to the APIs
- injection of typical and non-typical data aligned with NODE description
- check of outbound parameter behavior
- check of stack consumption and memory leakage.

# Services to the nodes (to be completed)

The "service" function has the following prototype

```
typedef void    (services) (uint32_t service_command, uint8_t *ptr1, uint8_t *ptr2, uint8_t *ptr3, uint32_t n); 
```

Service command bit-fields :

| Bit-fields | Name     | Details                                                      |
| :--------- | :------- | :----------------------------------------------------------- |
| 31-28      | control  | set/init/run w/wo wait completion, in case of coprocessor usag |
| 27-24      | options  | compute accuracy, in-place processing, frame size            |
| 23-4       | function | Operation/function within the Group                          |
| 3-0 (LSB)  | Group    | index to the groups of services : <br/>SERV_INTERNAL     1 <br/>SERV_SCRIPT       2 <br/>SERV_CONVERSION   3  : raw data format conversion (fp32 to int16, etc..)<br/>SERV_STDLIB       4 : extract of string and stdlib (atof, memset, strstr, malloc..)<br/>SERV_MATH         5 : extract of math (srand, sin, tan, sqrt, log..)<br/>SERV_DSP_ML       6 : filtering, spectrum fixed point and integer<br/>SERV_DEEPL        7 : fully-connected and convolutional network <br/>SERV_MM_AUDIO     8 : TBD<br/>SERV_MM_IMAGE     9 : TBD |



