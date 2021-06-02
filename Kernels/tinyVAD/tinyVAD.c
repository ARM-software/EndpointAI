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
 * \file tinyVAD.c
 * \brief This file holds the subroutine estimating the voice activity
 *  Voice activity detection
 *
 *  Optimized for minimal computation load, 
 *   introduces 100ms latency 
 *   Tunable sensitivity from High (SNRTHR=2.5) to Low (SNRTHR=6)
 *
 *  Algorithm description
 *      The input samples a high-pass filtered 
 *      The energy of the results (short_term_E) is used to estimate peaks 
 *      (envelope) and floor noise (floor_noise)
 *      When the ratio envelope/floor_noise exceeds the threshold THR a counter 
 *      is used to wait for confirmation 
 * 
 *  Parameters : 
 *      command (init=0)
 *      pointer to the result (VAD = 0 or 1)
 *      pointer to the 16bits input sample array
 *      number of samples to process
 *
 */

void vad_estimation(long *command, long *vad, short *inputData, long nbSamples)
{
#define MIN(a,b) ((a)<(b))?(a):(b)
#define MAX(a,b) ((a)>(b))?(a):(b)
    long idxSamp;
    static float VADRISE, VADFALL, smoothPeak, smoothFloor, HPF, lowestS, SNRTHR, MAXVAD;
    static short decimation_counterMAX, decimation_counter, waitStableNoise;
    static float vad_accumulator, Flag;
    static float hpf_z, hpf_z2, hpf_z3, short_term_E, floor_noise, envelope;
    float input_data32, y;

    if (*command == 0) {
        SNRTHR = 3.0;           /* sensitivity of the detection */
        lowestS = 33;           /* floor noise cannot go lower than -60dB */
        MAXVAD = 0.98;          
        vad_accumulator = 0;
        Flag = 0;
        hpf_z = hpf_z2 = hpf_z3 = 0;
        waitStableNoise = 4800; /* wait 100ms before starting */
        VADRISE = 0.0001;
        VADFALL = 0.001;
        smoothPeak = 256;
        smoothFloor = 4096;
        decimation_counterMAX = 255; /* decimation avoids using sharp LPF */
        decimation_counter = decimation_counterMAX;

        /* no init next time */
        (*command) = 1;
    }
    
    for (idxSamp = 0; idxSamp < nbSamples; idxSamp++) {

        input_data32 = inputData[idxSamp];
        hpf_z2 = 0.9375 * hpf_z + input_data32;  /* 1st order HPF cut-off at 275Hz */
        hpf_z3 = hpf_z2 - hpf_z;
        hpf_z = hpf_z2;

        decimation_counter = MIN(MAX(decimation_counter - 1, 0), decimation_counterMAX);
        y = hpf_z3;
        y = (y < 0) ? (-y) : y;
        short_term_E = (y / smoothPeak) + (short_term_E - (short_term_E / smoothPeak));
        envelope = (short_term_E / smoothFloor) + (envelope - (envelope / smoothFloor));
        envelope = MAX(short_term_E, envelope);

        if (waitStableNoise > 0) {
            floor_noise = envelope;    /* after reset, wait until noise estimator rises */
            waitStableNoise--;
        } else {
            if (decimation_counter == 0) {
                floor_noise = (short_term_E / smoothFloor) + 
                    (floor_noise - (floor_noise / smoothFloor));
                floor_noise = MAX(lowestS, MIN(floor_noise, short_term_E));
                decimation_counter = decimation_counterMAX;
            }
        }

        if (envelope > floor_noise * SNRTHR) {
            vad_accumulator = MIN(MAXVAD, vad_accumulator + VADRISE);
        } else {
            vad_accumulator = MAX(0, vad_accumulator - VADFALL);;
        }

        if (vad_accumulator > 0.5) {
            Flag = MIN(0.98, Flag + VADFALL);
        } else {
            Flag = MAX(0, Flag - VADRISE);
        }
        *vad = (Flag > 0.25);
    }
}

