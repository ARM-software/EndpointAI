enum stream_raw_data_type 
{
    STREAM_DATA_ARRAY = 0,         /*   see stream_array: [0NNNTT00] 0, type, nb */

    /* one bit per data */
    STREAM_S1,                     /*   S, one signed bit */
    STREAM_U1,                     /*   one bit unsigned */
                                      
    /* two bits per data */           
    STREAM_S2,                     /*   SX  */
    STREAM_U2,                     /*   XX  */
    STREAM_Q1,                     /*   Sx ~stream_s2 with saturation management*/
                                      
    /* four bits per data */          
    STREAM_S4,                     /*   Sxxx  */
    STREAM_U4,                     /*   xxxx  */
    STREAM_Q3,                     /*   Sxxx  */
    STREAM_FP4_E2M1,               /*   Seem  micro-float [8 .. 64] */
    STREAM_FP4_E3M0,               /*   Seee  [8 .. 512] */
                                      
    /* eight bits per data */         
    STREAM_S8,                     /*   Sxxxxxxx  */
    STREAM_U8,                     /*   xxxxxxxx  */
    STREAM_Q7,                     /*   Sxxxxxxx  */
    STREAM_CHAR,                   /*   xxxxxxxx  */
    STREAM_FP8_E4M3,               /*   Seeeemmm  tiny-float [0.02 .. 448] */
    STREAM_FP8_E5M2,               /*   Seeeeemm  [0.0001 .. 57344] for NN training */
                                     
    /* 2 bytes per data */            
    STREAM_S16,                    /*   Sxxxxxxx.xxxxxxxx  */
    STREAM_U16,                    /*   xxxxxxxx.xxxxxxxx  */
    STREAM_Q15,                    /*19 Sxxxxxxx.xxxxxxxx  */
    STREAM_FP16,                   /*   Seeeeemm.mmmmmmmm  half-precision float */
    STREAM_BF16,                   /*   Seeeeeee.mmmmmmmm  bfloat */

    /* 3 bytes per data */
    STREAM_Q23,                    /*   Sxxxxxxx.xxxxxxxx.xxxxxxxx in a 24bits container ! */
                                      
    /* 4 bytes per data */            
    STREAM_Q23_32,                 /*   SSSSSSSS.Sxxxxxxx.xxxxxxxx.xxxxxxxx  */
    STREAM_S32,                    /*   one long word  */
    STREAM_U32,                    /*   xxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx  */
    STREAM_Q31,                    /*   Sxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx  */
    STREAM_FP32,                   /*27 Seeeeeee.mmmmmmmm.mmmmmmmm.mmmmmmmm  float */
    STREAM_CQ15,                   /*   Sxxxxxxx.xxxxxxxx Sxxxxxxx.xxxxxxxx (I Q) */
    STREAM_CFP16,                  /*   Seeeeemm.mmmmmmmm Seeeeemm.mmmmmmmm (I Q)  */
                                      
    /* 8 bytes per data */            
    STREAM_S64,                    /*   long long */
    STREAM_U64,                    /*   unsigned  64 bits */
    STREAM_Q63,                    /*   Sxxxxxxx.xxxxxx ....... xxxxx.xxxxxxxx  */
    STREAM_CQ31,                   /*   Sxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx Sxxxxxxx.xxxxxxxx.. */
    STREAM_FP64,                   /*   Seeeeeee.eeemmmmm.mmmmmmm ... double  */
    STREAM_CFP32,                  /*   Seeeeeee.mmmmmmmm.mmmmmmmm.mmmmmmmm Seeeeeee.mmm.. (I Q)  */
                                      
    /* 16 bytes per data */           
    STREAM_FP128,                  /*   Seeeeeee.eeeeeeee.mmmmmmm ... quadruple precision */
    STREAM_CFP64,                  /*   fp64 fp64 (I Q)  */
                                      
    /* 32 bytes per data */           
    STREAM_FP256,                  /*   Seeeeeee.eeeeeeee.eeeeemm ... octuple precision  */
                                      
    /* 2D formats  */                 
    STREAM_YUV420P,                /*39 Luminance, Blue projection, Red projection, 6 bytes per 4 pixels, reordered */
    STREAM_YUV422P,                /*   8 bytes per 4 pixels, or 16bpp, Y0 Cb Y1 Cr (1 Cr & Cb sample per 2x1 Y samples) */
    STREAM_YUV444P,                /*   12 bytes per 4 pixels, or 24bpp, (1 Cr & Cb sample per 1x1 Y samples) */
    STREAM_CYM24,                  /*   cyan yellow magenta */
    STREAM_CYMK32,                 /*   cyan yellow magenta black */
    STREAM_RGB8,                   /*   RGB  3:3:2,  8bpp, (msb)2B 3G 3R(lsb) */
    STREAM_RGB16,                  /*   RGB  5:6:5, 16bpp, (msb)5R 6G 5B(lsb) */
    STREAM_RGBA16,                 /*   RGBA 4:4:4:4 32bpp (msb)4R */
    STREAM_RGB24,                  /*   BBGGRR 24bpp (msb)8B */
    STREAM_RGBA32,                 /*   BBGGRRAA 32bpp (msb)8B */
    STREAM_RGBA8888,               /*   AABBRRGG OpenGL/PNG format R=lsb A=MSB ("ABGR32" little endian) */
    STREAM_BW1B,                   /*   Y, 1bpp, 0 is black, 1 is white, ordered from the msb to the lsb.  */
    STREAM_BW1BINV,                /*   Y, 1bpp, 0 is white, 1 is black */
    STREAM_GREY8B,                 /*   Grey 8b */
    STREAM_GREY8BINV,              /*53 Grey 8b inverted 0 is white */
    
    LAST_RAW_TYPE  = 64,        /* coded on 6bits RAW_FMT0_LSB */
};

typedef uint32_t stream_array;      /* on 32b container, used for array of parameters */
#define     __UNUSED_ARRAY_MSB 31   
#define     __UNUSED_ARRAY_LSB 30   
#define       NBELEM_ARRAY_MSB 29 
#define       NBELEM_ARRAY_LSB 16   /* 14 bits linear */
#define    DATA_TYPE_ARRAY_MSB 15
#define    DATA_TYPE_ARRAY_LSB  8   /*  8 bits stream_raw_data_type */
#define         ZERO_ARRAY_MSB  7
#define         ZERO_ARRAY_LSB  0   /*  8 STREAM_DATA_ARRAY = 00 */ 