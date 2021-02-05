#ifndef CONFIG_FUNC_H
#define CONFIG_FUNC_H






using namespace FFTSCI;

#define FUNC_CFFT_F32

extern template const cfftconfig<float32_t> *cfft_config<float32_t>(uint32_t nbSamples);

#define FUNC_CFFT_F16

#if defined(ARM_FLOAT16_SUPPORTED)
extern template const cfftconfig<float16_t> *cfft_config<float16_t>(uint32_t nbSamples);
#endif


#define FUNC_CFFT_Q31

extern template const cfftconfig<Q31> *cfft_config<Q31>(uint32_t nbSamples);

#define FUNC_CFFT_Q15

extern template const cfftconfig<Q15> *cfft_config<Q15>(uint32_t nbSamples);

#define FUNC_RFFT_F32

extern template const rfftconfig<float32_t> *rfft_config<float32_t>(uint32_t nbSamples);

#define FUNC_RFFT_F16

#if defined(ARM_FLOAT16_SUPPORTED)
extern template const rfftconfig<float16_t> *rfft_config<float16_t>(uint32_t nbSamples);
#endif


#define FUNC_RFFT_Q31

extern template const rfftconfig<Q31> *rfft_config<Q31>(uint32_t nbSamples);

#define FUNC_RFFT_Q15

extern template const rfftconfig<Q15> *rfft_config<Q15>(uint32_t nbSamples);



#endif /* #ifdef CONFIG_FUNC_H */

