/*
 * Copyright (c) 2021 ARM Limited
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /**
  * \file main.c
  * \brief This file holds the subroutine estimating the voice activity
  */
  
  /*

    Test of the VAD

    The test pattern embedded is at 48kHz sampling rate Q15 format
    VAD introduces 100ms latency (4800 samples) : samples must be buffered 
        to recover the start of utterances
*/

#include <stdio.h>

const short AudioInputSamples[] = {
/* 
Fixed-point toggle times:
VAD toggled to 1 at 218953
VAD toggled to 0 at 275565
VAD toggled to 1 at 349572
VAD toggled to 0 at 405834

Floating-point toggle times:
VAD toggled to 1 at 218952
VAD toggled to 0 at 276285
VAD toggled to 1 at 349566
VAD toggled to 0 at 406267
*/
#include "TestPattern.txt"
#define FS 48000
};

extern void vad_estimation(long* command, long* vad, short* inputData, long nbSamples, long samplingRate);

void main (void)
{
#define nbSamples sizeof(AudioInputSamples)/2
    long NsampProcess, vad, previous_vad, command;
    long isamp;

    NsampProcess = 1;
    isamp = 0;
    vad = previous_vad = 0;
    /* first call makes init */
    command = 1; 

    while (isamp < nbSamples) {
        /* VAD processing 
        * Parameters : 
        * Command (init / processing)
        * pointer to the result (VAD = 0 or 1)
        * pointer to the 16bits input sample array
        * number of samples
        */
        vad_estimation(&command, &vad, (short *)&(AudioInputSamples[isamp]), NsampProcess, FS);
        
        isamp++;

        if (vad ^ previous_vad) {   
            fprintf(stdout, "VAD toggled to %d at %d \n", vad, isamp);
            previous_vad = vad;
        }
    }
}
