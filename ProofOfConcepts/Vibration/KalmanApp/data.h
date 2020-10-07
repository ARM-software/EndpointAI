#ifndef _DATA_H
#define _DATA_H
#include "arm_math_types.h"
#ifdef   __cplusplus
extern "C"
{
#endif

extern float32_t initQ[4];
extern float32_t initP[16];
extern float32_t inputData[5680];
#define BLOCKSIZE 10
#define NBSAMPLES 568
#ifdef   __cplusplus
}
#endif

#endif /* #ifdef  _DATA_H */
