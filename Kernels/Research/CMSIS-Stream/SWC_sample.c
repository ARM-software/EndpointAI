/*
 * Copyright (C) 2022 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /* ----------------------------------------------------------------------
  * Project:      Arm C300 Voice Demo Front end
  * Title:        arm_beamformer_f32.c
  * Description:  software component for beam forming

  * $Date:        May 2022 for EEMBC AudioMark
  * $Revision:    V.0.0.1
  *
  * Target Processor:  Cortex-M cores
  * -------------------------------------------------------------------- */


                            .
                            .
                            .



/*
   reset instance ------------------------------------------
*/
void arm_beamformer_f32_reset(beamformer_f32_instance *pinstance)
{
    int i;
    extern const float w_hanning_div2[];
    long tmp_ptr;   /* to remove compilation warning */

    tmp_ptr = (long)rotation;
    pinstance->wrot = (float *) tmp_ptr;    /* rotation precomputed table */
    tmp_ptr = (long)w_hanning_div2;
    pinstance->window = (float *) tmp_ptr; 

    pinstance->st->FFT128 = 0;              /* reset the FFT instance */

    for (i = 0; i < NFFT*REAL; i++)         /* reset internal buffers */
    {   pinstance->st->old_left[i] = 
        pinstance->st->old_right[i] = 0;
    }
}


/*
   CMSIS-Stream's wrapper to the float32 beamformer -------------------------------------
*/
int32_t arm_beamformer_f32 (int32_t command, void *instance, void *data, void *parameters)
{
    int32_t swc_returned_status = 0;

    switch (command)
    {
        case STREAM_MEMREQ: 
        /* func (STREAM_MEMREQ, 
                 *pointer to memory_request    => the first pointer is the instance 
                 *in/out stacked STREAM_DOMAIN_FORMAT_SIZE_W32 
                        (buffer sizes) ending with 0x00,
                 parameters: input : the swc_execution_option code   
                        output : returned KEY256 (optional)
                ) 
           The first request is the instance which holds the other pointers 
        */

        {   uint32_t *memreq = (uint32_t *)instance;    /* table of memory requests */
            uint32_t *format_of_io = (uint32_t *)data;  /* not used here */
            uint32_t *u32_param = (uint32_t *)parameters, execution_option;

            /* 1st memory request = SWC instance , 2nd..= data pointers */
            *memreq++ = sizeof(beamformer_f32_instance); 
            *memreq++ = PACKSWCMEM(MEM_TYPE_STATIC,MEM_REQ_DATA,MEM_SPEED_REQ_CRITICAL_FAST,
                        MEM_REQ_4BYTES_ALIGNMENT,0,sizeof(beamformer_f32_fastdata_static));
            *memreq++ = PACKSWCMEM(MEM_TYPE_WORKING,MEM_REQ_DATA,MEM_SPEED_REQ_CRITICAL_FAST,
                        MEM_REQ_4BYTES_ALIGNMENT,0,sizeof(beamformer_f32_fastdata_working));
            *memreq++ = 0;  /* end */

            execution_option = *u32_param;  /* partial / full execution code */
            break;
        }
    
        case STREAM_RESET: 
        /* func(STREAM_RESET, 
               *instance,           pointer a size defined in the 1st parameter memreq
               *memory_results,     pointers defined in the 2nd.. parameters of memreq (above)
               *Stream)             address of Stream

               memory pointers are in the same order as in MEMREQ, 
               the SWC can access the system time and other services using the @Stream APIs 
        */
        {   
            uint32_t *memresults = (uint32_t *)data;
            beamformer_f32_instance *pinstance = (beamformer_f32_instance *)instance;

            pinstance->st = (beamformer_f32_fastdata_static *) *memresults++;
            pinstance->w = (beamformer_f32_fastdata_working *) *memresults++;
            pinstance->st->STREAM = parameters;

            arm_beamformer_f32_reset(instance);
            break;
        }

        case STREAM_SET_PARAMETER:  
        /* func(STREAM_SET_PARAMETER, 
                *instance, 
                *parameter(s) (one or all)
                Index of parameter : 
                index/tag of a single parameter to set
                    256+N means preset N from the SWC manifest
        */ 
        {   
            uint8_t *new_parameters = (uint8_t *)data;

            break;
        }

        case STREAM_READ_PARAMETER:  
        /* func(STREAM_SET_PARAMETER, 
                *instance, 
                *parameter(s) (one or all)
                Index of parameter : 
                index/tag of a single parameter to set
                    256  means "all the parameters are read"  
        */ 
        {   
            uint8_t *new_parameters = (uint8_t *)data;
            break;
        }
        
        case STREAM_RUN:   
        /*  When data format is FMT_INTERLEAVED
            Left and Right samples are in this order : LRLRLRLRLRLR ..
                func(STREAM_RUN, instance, ptr to stereo data, nBytes buffer size)    

            When data format is FMT_DEINTERLEAVED_1PTR
            Left and Right samples are in this order : LLLL..LLLLRRRR...RRRR
            Size of the first buffer :                 <--size-->
                func(STREAM_RUN, instance, ptr to the Left buffer, Size in Bytes)

            When data format is FMT_DEINTERLEAVED_NPTR
            Left and Right samples are independent : LLLLL..LLLLL  ..  RRRRR...RRRRR
            Size of the first buffer :               <--size L-->      <--size R--->
            Pointer to a structure : [ {*ptr_L, size L}, {*ptr_R, size R} ]
                func(STREAM_RUN, instance, XDAIS PTR_INT, 0)
        */         
        {
            uint32_t buffer1_size, buffer2_size, bufferout_free, increment, *pt_pt=0;
            int32_t nb_input_samples, input_samples_consumed, output_samples_produced;
            uint8_t *inBufs1stChannel=0, *inBufs2ndChannel=0, *outBufs;

            /* input pointers { (*,n),(*,n),..} updated at the end */
            pt_pt = (uint32_t *)data;
            inBufs1stChannel = (uint8_t *)(*pt_pt++);   /* FMT_DEINTERLEAVED_NPTR */
            buffer1_size =     (uint32_t )(*pt_pt++);
            inBufs2ndChannel = (uint8_t *)(*pt_pt++);
            buffer2_size =     (uint32_t )(*pt_pt++);
            outBufs = (uint8_t *)(*pt_pt++); 
            bufferout_free = (uint32_t) (*pt_pt++); 

            nb_input_samples = buffer1_size / sizeof(int16_t);
            arm_beamformer_f32_run(instance, (int16_t *)inBufs1stChannel,  
                                         (int16_t *)inBufs2ndChannel, nb_input_samples, 
                                         (int16_t *)outBufs, 
                                         &input_samples_consumed, 
                                         &output_samples_produced, 
                                         &swc_returned_status);

            pt_pt = (uint32_t *)data;
            increment = (input_samples_consumed * sizeof(int16_t)*MONO);
            *pt_pt++ = (uint32_t)(inBufs1stChannel += increment);
            *pt_pt++ = (buffer1_size -= increment);
            *pt_pt++ = (uint32_t)(inBufs2ndChannel += increment);
            *pt_pt++ = (buffer2_size -= increment);
            *pt_pt++ = (uint32_t)(outBufs += increment);
            *pt_pt++ = (bufferout_free -= increment);
            
            break;
        }

        case STREAM_END:             /* func(STREAM_END, instance, 0) */
        {   arm_beamformer_f32_memory_free(instance);
            break;
        }   
    }
    return swc_returned_status;
}
