/* (C) COPYRIGHT 1994-2002 Xiph.Org Foundation */
/* Modified by Jean-Marc Valin */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/* opus_types.h based on ogg_types.h from libogg */

/**
   @file opus_lib.h
   @brief
    aggregated opus headers to make opus fft stand alone
        opus_types.h / fixed_generic / bands.c for random generation
*/


#ifndef OPUS_LIB_H
#define OPUS_LIB_H

#include <stdio.h>
#include <stdint.h>
#include <limits.h>


typedef int16_t opus_int16;
typedef uint16_t opus_uint16;
typedef int opus_int;
typedef int32_t opus_int32;
typedef uint32_t opus_uint32;

typedef opus_int16 opus_val16;
#define opus_int64       long long
typedef opus_int32 opus_val32;
typedef opus_val32 celt_sig;
typedef opus_val32 celt_ener;
typedef opus_val16 celt_norm;

#define opus_uint64      unsigned long long

#define OPUS_RESTRICT restrict
#define FIXED_POINT 1

#include "static_modes_fixed.h"

#define DEBUG 0

#if defined(DEBUG) && DEBUG > 0
#define DEBUG_PRINT(fmt, args...) printf( fmt, ##args)
#else
#define DEBUG_PRINT(fmt, args...)   /* Don't do anything in release builds */
#endif



#define OPUS_INLINE inline

#define QCONST16(x,bits) ((opus_val16)(.5+(x)*(((opus_val32)1)<<(bits))))
#define EXTRACT16(x) ((opus_val16)(x))
#define SHR32(a,shift) ((a) >> (shift))
/** Arithmetic shift-right of a 16-bit value */
#define SHR16(a,shift) ((a) >> (shift))

/** Arithmetic shift-left of a 16-bit value */
#define SHL16(a,shift) ((opus_int16)((opus_uint16)(a)<<(shift)))

#define SHR(a,b) SHR32(a,b)
/** Arithmetic shift-left of a 32-bit value */
#define SHL(a,shift) SHL32(a,shift)


#define SATURATE(x,a) (((x)>(a) ? (a) : (x)<-(a) ? -(a) : (x)))

 /** Shift by a and round-to-neareast 32-bit value. Result is a saturated 16-bit value */
#define SROUND16(x,a) EXTRACT16(SATURATE(PSHR32(x,a), 32767));

 /** Shift by a and round-to-neareast 32-bit value. Result is a 16-bit value */
#define ROUND16(x,a) (EXTRACT16(PSHR32((x),(a))))
#define NEG16(x) (-(x))

#define SHL32(a,shift) ((opus_int32)((opus_uint32)(a)<<(shift)))
#define celt_assert(cond)
#define celt_assert2(cond, message)


#if defined(__GNUC__)&&defined(__GNUC_MINOR__)
#define OPUS_GNUC_PREREQ(_maj,_min) \
 ((__GNUC__<<16)+__GNUC_MINOR__>=((_maj)<<16)+(_min))
#else
#define OPUS_GNUC_PREREQ(_maj,_min) 0
#endif

#if OPUS_GNUC_PREREQ(3, 0)
#define opus_likely(x)       (__builtin_expect(!!(x), 1))
#define opus_unlikely(x)     (__builtin_expect(!!(x), 0))
#else
#define opus_likely(x)       (!!(x))
#define opus_unlikely(x)     (!!(x))
#endif


static OPUS_INLINE opus_val32 MULT16_32_Q16_armv4(opus_val16 a, opus_val32 b)
{
    unsigned  rd_lo;
    int       rd_hi;
 __asm__("#MULT16_32_Q16\n\t" "smull %0, %1, %2, %3\n\t": "=&r"(rd_lo), "=&r"(rd_hi):"%r"(b), "r"(SHL32(a, 16)));
    return rd_hi;
}

#define MULT16_32_Q16(a, b) (MULT16_32_Q16_armv4(a, b))

static OPUS_INLINE opus_val32 MULT16_16_armv5e(opus_val16 a, opus_val16 b)
{
    int       res;
 __asm__("#MULT16_16\n\t" "smulbb %0, %1, %2;\n": "=r"(res):"r"(a), "r"(b));
    return res;
}

#define MULT16_16(a, b) (MULT16_16_armv5e(a, b))

static OPUS_INLINE opus_val32 MULT16_32_Q15_armv4(opus_val16 a, opus_val32 b)
{
    unsigned  rd_lo;
    int       rd_hi;
 __asm__("#MULT16_32_Q15\n\t" "smull %0, %1, %2, %3\n\t": "=&r"(rd_lo), "=&r"(rd_hi):"%r"(b), "r"(SHL32(a, 16)));
    /*
     * We intentionally don't OR in the high bit of rd_lo for speed.
     */
    return SHL32(rd_hi, 1);
}

#define MULT16_32_Q15(a, b) (MULT16_32_Q15_armv4(a, b))

#define MAC16_32_Q15(c, a, b) ADD32(c, MULT16_32_Q15(a, b))

#define MULT16_16_P15(a,b) (SHR(ADD32(16384,MULT16_16((a),(b))),15))


#define MULT16_16_Q15(a,b) (SHR(MULT16_16((a),(b)),15))

/** Add two 32-bit values */
#define ADD32(a,b) ((opus_val32)(a)+(opus_val32)(b))

/** Subtract two 32-bit values */
#define SUB32(a,b) ((opus_val32)(a)-(opus_val32)(b))

/** Change a 16-bit value into a 32-bit value */
#define EXTEND32(x) ((opus_val32)(x))
/** 32-bit arithmetic shift right where the argument can be negative */
#define VSHR32(a, shift) (((shift)>0) ? SHR32(a, shift) : SHL32(a, -(shift)))

/** 16x32 multiplication, followed by a 15-bit shift right. Results fits in 32 bits */
/** Multiply a 16-bit signed value by a 16-bit unsigned value. The result is a 32-bit signed value */
#define MULT16_16SU(a,b) ((opus_val32)(opus_val16)(a)*(opus_val32)(opus_uint16)(b))

#define MULT32_32_Q31(a,b) ADD32(ADD32(SHL32(MULT16_16(SHR32((a),16),SHR((b),16)),1), SHR32(MULT16_16SU(SHR32((a),16),((b)&0x0000ffff)),15)), SHR32(MULT16_16SU(SHR32((b),16),((a)&0x0000ffff)),15))

/** 32-bit arithmetic shift right with rounding-to-nearest instead of rounding down */
#define PSHR32(a,shift) (SHR32((a)+((EXTEND32(1)<<((shift))>>1)),shift))

/** Add two 32-bit values, ignore any overflows */
#define ADD32_ovflw(a,b) ((opus_val32)((opus_uint32)(a)+(opus_uint32)(b)))
/** Subtract two 32-bit values, ignore any overflows */
#define SUB32_ovflw(a,b) ((opus_val32)((opus_uint32)(a)-(opus_uint32)(b)))
/* Avoid MSVC warning C4146: unary minus operator applied to unsigned type */
/** Negate 32-bit value, ignore any overflows */
#define NEG32_ovflw(a) ((opus_val32)(0-(opus_uint32)(a)))

#define MULT16_16_Q14(a,b) (SHR(MULT16_16((a),(b)),14))

#ifndef OVERRIDE_OPUS_CLEAR
#define OPUS_CLEAR(dst, n) (memset((dst), 0, (n)*sizeof(*(dst))))
#endif

#ifndef OVERRIDE_OPUS_COPY
#define OPUS_COPY(dst, src, n) (memcpy((dst), (src), (n)*sizeof(*(dst)) + 0*((dst)-(src)) ))
#endif

#ifndef OVERRIDE_OPUS_MOVE
#define OPUS_MOVE(dst, src, n) (memmove((dst), (src), (n)*sizeof(*(dst)) + 0*((dst)-(src)) ))
#endif


static OPUS_INLINE opus_uint32 celt_lcg_rand(opus_uint32 seed)
{
    return 1664525 * seed + 1013904223;
}

static OPUS_INLINE opus_uint32 celt_get_fft_shift(opus_uint32 fft_size)
{
    int       shft = 0;
    switch (fft_size)
    {
    case 480:
        shft = 0;
        break;
    case 240:
        shft = 1;
        break;
    case 120:
        shft = 2;
        break;
    case 60:
        shft = 3;
        break;
    default:
        DEBUG_PRINT("\n\n!! unsupported FFT size, use default=480\n\n");
        break;
    }
    return shft;
}


#endif
