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

#define samp_t long
#define coef_t long
#define accu_t long
#define data_t long
#define iidx_t long 

#define samp_f float
#define coef_f float
#define accu_f float
#define data_f float

#define INIT_VAD    1
#define PROC_VAD    2

#define FIXED_POINT 1

#define MIN(a,b) ((a)<(b))?(a):(b)
#define MAX(a,b) ((a)>(b))?(a):(b)


#if FIXED_POINT

#define F2Q31(f) (long)(0x7FFFFFFL*(f))
#define F2Q15(f) (long)(0x7FFFL*(f))

#define ConvertSamp(f) (f<<15)
#define DIVBIN(s,n) (s>>n)


long decfMAX, decf;
long VADRISE, VADFALL, SHPF, S1, S2, lowestS, INVTHR, MAXVAD;
long accvad, Flag;
long z1, z2, z3, z6, z7, z8;

/* ====  Voice Activity Detector ====
* Parameters :
* Command (init / preocessing)
* pointer to the result (VAD = 0 or 1)
* pointer to the 16bits input sample array
* number of samples
* sampling rate
*/
void vad_estimation(long* command, long* vad, short* inputData, long nbSamples, long samplingRate)
{
    long input_data;
    long y, tmpz8, d;
    long idxSamp;

    if ((*command) == INIT_VAD)
    {
        lowestS = F2Q31(0.001);
        INVTHR = F2Q15(1 / 3.0);
        MAXVAD = F2Q31(0.98);
        z1 = z2 = z3 = z8 = accvad = Flag = 0;
        z6 = F2Q31(0.001);
        z7 = F2Q31(0.1);

        if (samplingRate <= 16000)
        {
            SHPF = 2;
            VADRISE = F2Q31(0.00031);
            VADFALL = F2Q31(0.003);
            S1 = 6;
            S2 = 12;
            decfMAX = 127;
            decf = decfMAX;
        }
        else
        {
            SHPF = 1;
            VADRISE = F2Q31(0.0001);
            VADFALL = F2Q31(0.001);
            S1 = 6;
            S2 = 12;
            decfMAX = 255;
            decf = decfMAX;
        }

        /* no init next time */
        (*command) = PROC_VAD;
    }

    for (idxSamp = 0; idxSamp < nbSamples; idxSamp++)
    {

        input_data = ConvertSamp(inputData[idxSamp]);
        d = DIVBIN(z1, SHPF);
        z2 = z1 - DIVBIN(z1, SHPF) + input_data;
        z3 = z2 - z1;
        z1 = z2;
        decf = MIN(MAX(decf - 1, 0), decfMAX);
        y = z3;
        y = (y < 0) ? (-y) : y;
        z6 = DIVBIN(y, S1) + (z6 - DIVBIN(z6, S1));
        z8 = DIVBIN(z6, S2) + (z8 - DIVBIN(z8, S2));
        z8 = MAX(z6, z8);

        if (decf == 0)
        {
            z7 = DIVBIN(z6, S2) + (z7 - DIVBIN(z7, S2));
            z7 = MAX(lowestS, MIN(z7, z6));
            decf = decfMAX;
        }

        tmpz8 = ((z8 >> 16) * INVTHR) >> 15;
        if (tmpz8 > (z7 >> 16))  accvad = MIN(MAXVAD, accvad + VADRISE);
        else
            accvad = MAX(0, accvad - VADFALL);

        if (accvad > F2Q31(0.3))
            Flag = MIN(F2Q31(0.98), Flag + VADFALL);
        else
            Flag = MAX(0, Flag - VADRISE);

        *vad = (Flag > F2Q31(0.5));
    }
}

#else
coef_f VADRISE, VADFALL, smoothPeak, smoothFloor, HPF, lowestS, THR, MAXVAD;
iidx_t decfMAX, decf, waitStableNoise;
data_f accvad, Flag;
samp_f z1, z2, z3, z6, z7, z8;

#define DIVBIN(s,n) (s/(1<<n))

void vad_estimation(data_t* command, data_t* vad, samp_t* inputData, iidx_t nbSamples, data_t samplingRate)
{
    samp_f input_data32, y;
    iidx_t idxSamp;
    iidx_t S1, S2;

    if (*command == INIT_VAD)
    {
        lowestS = 0.001;
        THR = 3.0;
        MAXVAD = 0.98;
        accvad = 0;
        Flag = 0;
        z1 = z2 = z3 = 0;
        z6 = 0.001;
        z7 = 0.1;
        z8 = 0;

        if (samplingRate <= 16000)
        {
            HPF = 0.75;
            VADRISE = 0.00031;
            VADFALL = 0.003;
            smoothPeak = 6;
            smoothFloor = 12;
            decfMAX = 127;
            decf = decfMAX;
            waitStableNoise = 1600;
        }
        else
        {
            HPF = 0.5;
            VADRISE = 0.0001;
            VADFALL = 0.001;
            smoothPeak = 6;
            smoothFloor = 12;
            decfMAX = 255;
            decf = decfMAX;
            waitStableNoise = 4800;
        }
        /* no init next time */
        (*command) = PROC_VAD;
    }
    else
    {
        for (idxSamp = 0; idxSamp < nbSamples; idxSamp++)
        {

            input_data32 = inputData[idxSamp] / 32768.0;
            z2 = HPF * z1 + input_data32;
            z3 = z2 - z1;
            z1 = z2;

            S1 = smoothPeak;
            S2 = smoothFloor;
            decf = MIN(MAX(decf - 1, 0), decfMAX);
            y = z3;
            y = (y < 0) ? (-y) : y;
            z6 = (y / (1 << S1)) + (z6 - (z6 / (1 << S1)));
            z8 = (z6 / (1 << S2)) + (z8 - (z8 / (1 << S2)));
            z8 = MAX(z6, z8);


            if (waitStableNoise > 0) {
                z7 = z8;    /* after reset, wait until noise estimator rises */
                waitStableNoise--;
            }
            else {
                if (decf == 0)
                {
                    z7 = (z6 / (1 << S2)) + (z7 - ((z7) / (1 << S2)));
                    z7 = MAX(lowestS, MIN(z7, z6));
                    decf = decfMAX;
                }
            }
            if (z8 > z7 * THR)
            {
                accvad = MIN(MAXVAD, accvad + VADRISE);
            }
            else
            {
                accvad = MAX(0, accvad - VADFALL);;
            }

            if (accvad > 0.3)
            {
                Flag = MIN(0.98, Flag + VADFALL);
            }
            else
            {
                Flag = MAX(0, Flag - VADRISE);
            }
            *vad = (Flag > 0.5);
        }
    }
}
#endif

