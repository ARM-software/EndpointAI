#include "fft_types.h"
#include "perm.h"
#include "twiddle.h"

const uint16_t factors1[4]={4,1,8,2};

const uint16_t lengths1[3]={64,8,1};

const uint16_t factors2[2]={4,6};

const uint16_t lengths2[6]={1024,256,64,16,4,1};

const uint16_t factors3[2]={2,1};

const uint16_t lengths3[1]={1};

const uint16_t factors4[2]={3,1};

const uint16_t lengths4[1]={1};

const uint16_t factors5[2]={4,1};

const uint16_t lengths5[1]={1};

const uint16_t factors6[2]={5,1};

const uint16_t lengths6[1]={1};

const uint16_t factors7[4]={2,1,3,1};

const uint16_t lengths7[2]={3,1};

const uint16_t factors8[2]={8,3};

const uint16_t lengths8[3]={64,8,1};

const uint16_t factors9[2]={8,1};

const uint16_t lengths9[1]={1};

const uint16_t factors10[2]={3,2};

const uint16_t lengths10[2]={3,1};

const uint16_t factors11[4]={2,1,5,1};

const uint16_t lengths11[2]={5,1};

const uint16_t factors12[4]={2,1,8,2};

const uint16_t lengths12[3]={64,8,1};

const uint16_t factors13[4]={3,1,4,1};

const uint16_t lengths13[2]={4,1};

const uint16_t factors14[6]={2,1,3,1,8,2};

const uint16_t lengths14[4]={192,64,8,1};

const uint16_t factors15[4]={3,1,5,1};

const uint16_t lengths15[2]={5,1};

const uint16_t factors16[4]={2,1,8,1};

const uint16_t lengths16[2]={8,1};

const uint16_t factors17[6]={2,1,3,2,8,1};

const uint16_t lengths17[4]={72,24,8,1};

const uint16_t factors18[4]={2,1,3,2};

const uint16_t lengths18[3]={9,3,1};

const uint16_t factors19[6]={2,1,5,2,8,1};

const uint16_t lengths19[4]={200,40,8,1};

const uint16_t factors20[4]={4,1,5,1};

const uint16_t lengths20[2]={5,1};

const uint16_t factors21[6]={2,1,3,1,5,2};

const uint16_t lengths21[4]={75,25,5,1};

const uint16_t factors22[4]={3,1,8,1};

const uint16_t lengths22[2]={8,1};

const uint16_t factors23[2]={5,2};

const uint16_t lengths23[2]={5,1};

const uint16_t factors24[2]={3,3};

const uint16_t lengths24[3]={9,3,1};

const uint16_t factors25[6]={2,1,3,1,5,1};

const uint16_t lengths25[3]={15,5,1};

const uint16_t factors26[4]={4,1,8,1};

const uint16_t lengths26[2]={8,1};

const uint16_t factors27[6]={3,2,4,1,8,1};

const uint16_t lengths27[4]={96,32,8,1};

const uint16_t factors28[2]={4,5};

const uint16_t lengths28[5]={256,64,16,4,1};

const uint16_t factors29[6]={4,1,5,1,8,1};

const uint16_t lengths29[3]={40,8,1};

const uint16_t factors30[4]={3,2,4,1};

const uint16_t lengths30[3]={12,4,1};

const uint16_t factors31[4]={5,1,8,1};

const uint16_t lengths31[2]={8,1};

const uint16_t factors32[4]={3,2,5,1};

const uint16_t lengths32[3]={15,5,1};

const uint16_t factors33[6]={2,1,3,1,8,1};

const uint16_t lengths33[3]={24,8,1};

const uint16_t factors34[4]={2,1,5,2};

const uint16_t lengths34[3]={25,5,1};

const uint16_t factors35[6]={3,2,4,1,5,1};

const uint16_t lengths35[4]={60,20,5,1};

const uint16_t factors36[4]={2,1,3,3};

const uint16_t lengths36[4]={27,9,3,1};

const uint16_t factors37[6]={3,1,4,1,5,1};

const uint16_t lengths37[3]={20,5,1};

const uint16_t factors38[2]={8,2};

const uint16_t lengths38[2]={8,1};

const uint16_t factors39[4]={5,1,8,2};

const uint16_t lengths39[3]={64,8,1};

const uint16_t factors40[4]={2,1,4,5};

const uint16_t lengths40[6]={1024,256,64,16,4,1};

const uint16_t factors41[4]={3,1,8,2};

const uint16_t lengths41[3]={64,8,1};

const uint16_t factors42[4]={3,2,8,1};

const uint16_t lengths42[3]={24,8,1};

const uint16_t factors43[4]={5,2,8,1};

const uint16_t lengths43[3]={40,8,1};

const uint16_t factors44[4]={3,1,5,2};

const uint16_t lengths44[3]={25,5,1};

const uint16_t factors45[6]={2,1,5,1,8,1};

const uint16_t lengths45[3]={40,8,1};

const uint16_t factors46[2]={3,4};

const uint16_t lengths46[4]={27,9,3,1};

const uint16_t factors47[4]={3,3,8,1};

const uint16_t lengths47[4]={72,24,8,1};

const uint16_t factors48[6]={2,1,3,2,5,1};

const uint16_t lengths48[4]={45,15,5,1};

const uint16_t factors49[6]={3,1,4,1,8,1};

const uint16_t lengths49[3]={32,8,1};

const uint16_t factors50[4]={4,1,5,2};

const uint16_t lengths50[3]={25,5,1};

const uint16_t factors51[4]={3,3,4,1};

const uint16_t lengths51[4]={36,12,4,1};

const uint16_t factors52[8]={2,1,3,1,5,1,8,1};

const uint16_t lengths52[4]={120,40,8,1};

const uint16_t factors53[6]={3,1,5,1,8,1};

const uint16_t lengths53[3]={40,8,1};

const uint16_t factors54[2]={5,3};

const uint16_t lengths54[3]={25,5,1};

const uint16_t factors55[2]={4,4};

const uint16_t lengths55[4]={64,16,4,1};

const uint16_t factors56[4]={2,1,4,4};

const uint16_t lengths56[5]={256,64,16,4,1};

const uint16_t factors57[4]={2,1,4,1};

const uint16_t lengths57[2]={4,1};

const uint16_t factors58[4]={2,1,4,3};

const uint16_t lengths58[4]={64,16,4,1};

const uint16_t factors59[6]={2,1,3,1,4,3};

const uint16_t lengths59[5]={192,64,16,4,1};

const uint16_t factors60[2]={4,2};

const uint16_t lengths60[2]={4,1};

const uint16_t factors61[4]={3,2,4,2};

const uint16_t lengths61[4]={48,16,4,1};

const uint16_t factors62[4]={4,2,5,2};

const uint16_t lengths62[4]={100,25,5,1};

const uint16_t factors63[6]={2,1,3,1,4,1};

const uint16_t lengths63[3]={12,4,1};

const uint16_t factors64[4]={2,1,4,2};

const uint16_t lengths64[3]={16,4,1};

const uint16_t factors65[6]={2,1,3,2,4,2};

const uint16_t lengths65[5]={144,48,16,4,1};

const uint16_t factors66[6]={2,1,4,2,5,1};

const uint16_t lengths66[4]={80,20,5,1};

const uint16_t factors67[6]={2,1,4,1,5,1};

const uint16_t lengths67[3]={20,5,1};

const uint16_t factors68[4]={3,1,4,2};

const uint16_t lengths68[3]={16,4,1};

const uint16_t factors69[2]={4,3};

const uint16_t lengths69[3]={16,4,1};

const uint16_t factors70[4]={4,3,5,1};

const uint16_t lengths70[4]={80,20,5,1};

const uint16_t factors71[4]={3,1,4,3};

const uint16_t lengths71[4]={64,16,4,1};

const uint16_t factors72[6]={2,1,3,2,4,1};

const uint16_t lengths72[4]={36,12,4,1};

const uint16_t factors73[6]={2,1,4,1,5,2};

const uint16_t lengths73[4]={100,25,5,1};

const uint16_t factors74[4]={4,2,5,1};

const uint16_t lengths74[3]={20,5,1};

const uint16_t factors75[6]={2,1,3,3,4,1};

const uint16_t lengths75[5]={108,36,12,4,1};

const uint16_t factors76[6]={2,1,3,1,4,2};

const uint16_t lengths76[4]={48,16,4,1};

const uint16_t factors77[6]={3,1,4,2,5,1};

const uint16_t lengths77[4]={80,20,5,1};

const uint16_t factors78[8]={2,1,3,1,4,1,5,1};

const uint16_t lengths78[4]={60,20,5,1};

cfftconfig<float32_t> config1={
   .normalization=0.00390625000000000000f,
   .nbPerms=NB_PERM1,
   .perms=perm1,
   .nbTwiddle=NB_TWIDDLE1,
   .twiddle=twiddle1,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors1,
   .lengths=lengths1,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config2={
   .normalization=0.00024414062500000000f,
   .nbPerms=NB_PERM2,
   .perms=perm2,
   .nbTwiddle=NB_TWIDDLE2,
   .twiddle=twiddle2,
   .nbGroupedFactors=1,
   .nbFactors=6,
   .factors=factors2,
   .lengths=lengths2,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config3={
   .normalization=0.50000000000000000000f,
   .nbPerms=NB_PERM3,
   .perms=perm3,
   .nbTwiddle=NB_TWIDDLE3,
   .twiddle=twiddle3,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors3,
   .lengths=lengths3,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config4={
   .normalization=0.33333333333333331483f,
   .nbPerms=NB_PERM4,
   .perms=perm4,
   .nbTwiddle=NB_TWIDDLE4,
   .twiddle=twiddle4,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors4,
   .lengths=lengths4,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config5={
   .normalization=0.25000000000000000000f,
   .nbPerms=NB_PERM5,
   .perms=perm5,
   .nbTwiddle=NB_TWIDDLE5,
   .twiddle=twiddle5,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors5,
   .lengths=lengths5,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config6={
   .normalization=0.20000000000000001110f,
   .nbPerms=NB_PERM6,
   .perms=perm6,
   .nbTwiddle=NB_TWIDDLE6,
   .twiddle=twiddle6,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors6,
   .lengths=lengths6,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config7={
   .normalization=0.16666666666666665741f,
   .nbPerms=NB_PERM7,
   .perms=perm7,
   .nbTwiddle=NB_TWIDDLE7,
   .twiddle=twiddle7,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors7,
   .lengths=lengths7,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config8={
   .normalization=0.00195312500000000000f,
   .nbPerms=NB_PERM8,
   .perms=perm8,
   .nbTwiddle=NB_TWIDDLE8,
   .twiddle=twiddle8,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors8,
   .lengths=lengths8,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config9={
   .normalization=0.12500000000000000000f,
   .nbPerms=NB_PERM9,
   .perms=perm9,
   .nbTwiddle=NB_TWIDDLE9,
   .twiddle=twiddle9,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors9,
   .lengths=lengths9,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config10={
   .normalization=0.11111111111111110494f,
   .nbPerms=NB_PERM10,
   .perms=perm10,
   .nbTwiddle=NB_TWIDDLE10,
   .twiddle=twiddle10,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors10,
   .lengths=lengths10,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config11={
   .normalization=0.10000000000000000555f,
   .nbPerms=NB_PERM11,
   .perms=perm11,
   .nbTwiddle=NB_TWIDDLE11,
   .twiddle=twiddle11,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors11,
   .lengths=lengths11,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config12={
   .normalization=0.00781250000000000000f,
   .nbPerms=NB_PERM12,
   .perms=perm12,
   .nbTwiddle=NB_TWIDDLE12,
   .twiddle=twiddle12,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors12,
   .lengths=lengths12,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config13={
   .normalization=0.08333333333333332871f,
   .nbPerms=NB_PERM13,
   .perms=perm13,
   .nbTwiddle=NB_TWIDDLE13,
   .twiddle=twiddle13,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors13,
   .lengths=lengths13,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config14={
   .normalization=0.00260416666666666652f,
   .nbPerms=NB_PERM14,
   .perms=perm14,
   .nbTwiddle=NB_TWIDDLE14,
   .twiddle=twiddle14,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors14,
   .lengths=lengths14,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config15={
   .normalization=0.06666666666666666574f,
   .nbPerms=NB_PERM15,
   .perms=perm15,
   .nbTwiddle=NB_TWIDDLE15,
   .twiddle=twiddle15,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors15,
   .lengths=lengths15,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config16={
   .normalization=0.06250000000000000000f,
   .nbPerms=NB_PERM16,
   .perms=perm16,
   .nbTwiddle=NB_TWIDDLE16,
   .twiddle=twiddle16,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors16,
   .lengths=lengths16,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config17={
   .normalization=0.00694444444444444406f,
   .nbPerms=NB_PERM17,
   .perms=perm17,
   .nbTwiddle=NB_TWIDDLE17,
   .twiddle=twiddle17,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors17,
   .lengths=lengths17,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config18={
   .normalization=0.05555555555555555247f,
   .nbPerms=NB_PERM18,
   .perms=perm18,
   .nbTwiddle=NB_TWIDDLE18,
   .twiddle=twiddle18,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors18,
   .lengths=lengths18,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config19={
   .normalization=0.00250000000000000005f,
   .nbPerms=NB_PERM19,
   .perms=perm19,
   .nbTwiddle=NB_TWIDDLE19,
   .twiddle=twiddle19,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors19,
   .lengths=lengths19,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config20={
   .normalization=0.05000000000000000278f,
   .nbPerms=NB_PERM20,
   .perms=perm20,
   .nbTwiddle=NB_TWIDDLE20,
   .twiddle=twiddle20,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors20,
   .lengths=lengths20,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config21={
   .normalization=0.00666666666666666709f,
   .nbPerms=NB_PERM21,
   .perms=perm21,
   .nbTwiddle=NB_TWIDDLE21,
   .twiddle=twiddle21,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors21,
   .lengths=lengths21,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config22={
   .normalization=0.04166666666666666435f,
   .nbPerms=NB_PERM22,
   .perms=perm22,
   .nbTwiddle=NB_TWIDDLE22,
   .twiddle=twiddle22,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors22,
   .lengths=lengths22,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config23={
   .normalization=0.04000000000000000083f,
   .nbPerms=NB_PERM23,
   .perms=perm23,
   .nbTwiddle=NB_TWIDDLE23,
   .twiddle=twiddle23,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors23,
   .lengths=lengths23,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config24={
   .normalization=0.03703703703703703498f,
   .nbPerms=NB_PERM24,
   .perms=perm24,
   .nbTwiddle=NB_TWIDDLE24,
   .twiddle=twiddle24,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors24,
   .lengths=lengths24,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config25={
   .normalization=0.03333333333333333287f,
   .nbPerms=NB_PERM25,
   .perms=perm25,
   .nbTwiddle=NB_TWIDDLE25,
   .twiddle=twiddle25,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors25,
   .lengths=lengths25,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config26={
   .normalization=0.03125000000000000000f,
   .nbPerms=NB_PERM26,
   .perms=perm26,
   .nbTwiddle=NB_TWIDDLE26,
   .twiddle=twiddle26,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors26,
   .lengths=lengths26,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config27={
   .normalization=0.00347222222222222203f,
   .nbPerms=NB_PERM27,
   .perms=perm27,
   .nbTwiddle=NB_TWIDDLE27,
   .twiddle=twiddle27,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors27,
   .lengths=lengths27,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config28={
   .normalization=0.00097656250000000000f,
   .nbPerms=NB_PERM28,
   .perms=perm28,
   .nbTwiddle=NB_TWIDDLE28,
   .twiddle=twiddle28,
   .nbGroupedFactors=1,
   .nbFactors=5,
   .factors=factors28,
   .lengths=lengths28,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config29={
   .normalization=0.00625000000000000035f,
   .nbPerms=NB_PERM29,
   .perms=perm29,
   .nbTwiddle=NB_TWIDDLE29,
   .twiddle=twiddle29,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors29,
   .lengths=lengths29,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config30={
   .normalization=0.02777777777777777624f,
   .nbPerms=NB_PERM30,
   .perms=perm30,
   .nbTwiddle=NB_TWIDDLE30,
   .twiddle=twiddle30,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors30,
   .lengths=lengths30,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config31={
   .normalization=0.02500000000000000139f,
   .nbPerms=NB_PERM31,
   .perms=perm31,
   .nbTwiddle=NB_TWIDDLE31,
   .twiddle=twiddle31,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors31,
   .lengths=lengths31,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config32={
   .normalization=0.02222222222222222307f,
   .nbPerms=NB_PERM32,
   .perms=perm32,
   .nbTwiddle=NB_TWIDDLE32,
   .twiddle=twiddle32,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors32,
   .lengths=lengths32,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config33={
   .normalization=0.02083333333333333218f,
   .nbPerms=NB_PERM33,
   .perms=perm33,
   .nbTwiddle=NB_TWIDDLE33,
   .twiddle=twiddle33,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors33,
   .lengths=lengths33,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config34={
   .normalization=0.02000000000000000042f,
   .nbPerms=NB_PERM34,
   .perms=perm34,
   .nbTwiddle=NB_TWIDDLE34,
   .twiddle=twiddle34,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors34,
   .lengths=lengths34,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config35={
   .normalization=0.00555555555555555577f,
   .nbPerms=NB_PERM35,
   .perms=perm35,
   .nbTwiddle=NB_TWIDDLE35,
   .twiddle=twiddle35,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors35,
   .lengths=lengths35,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config36={
   .normalization=0.01851851851851851749f,
   .nbPerms=NB_PERM36,
   .perms=perm36,
   .nbTwiddle=NB_TWIDDLE36,
   .twiddle=twiddle36,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors36,
   .lengths=lengths36,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config37={
   .normalization=0.01666666666666666644f,
   .nbPerms=NB_PERM37,
   .perms=perm37,
   .nbTwiddle=NB_TWIDDLE37,
   .twiddle=twiddle37,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors37,
   .lengths=lengths37,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config38={
   .normalization=0.01562500000000000000f,
   .nbPerms=NB_PERM38,
   .perms=perm38,
   .nbTwiddle=NB_TWIDDLE38,
   .twiddle=twiddle38,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors38,
   .lengths=lengths38,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config39={
   .normalization=0.00312500000000000017f,
   .nbPerms=NB_PERM39,
   .perms=perm39,
   .nbTwiddle=NB_TWIDDLE39,
   .twiddle=twiddle39,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors39,
   .lengths=lengths39,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config40={
   .normalization=0.00048828125000000000f,
   .nbPerms=NB_PERM40,
   .perms=perm40,
   .nbTwiddle=NB_TWIDDLE40,
   .twiddle=twiddle40,
   .nbGroupedFactors=2,
   .nbFactors=6,
   .factors=factors40,
   .lengths=lengths40,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config41={
   .normalization=0.00520833333333333304f,
   .nbPerms=NB_PERM41,
   .perms=perm41,
   .nbTwiddle=NB_TWIDDLE41,
   .twiddle=twiddle41,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors41,
   .lengths=lengths41,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config42={
   .normalization=0.01388888888888888812f,
   .nbPerms=NB_PERM42,
   .perms=perm42,
   .nbTwiddle=NB_TWIDDLE42,
   .twiddle=twiddle42,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors42,
   .lengths=lengths42,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config43={
   .normalization=0.00500000000000000010f,
   .nbPerms=NB_PERM43,
   .perms=perm43,
   .nbTwiddle=NB_TWIDDLE43,
   .twiddle=twiddle43,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors43,
   .lengths=lengths43,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config44={
   .normalization=0.01333333333333333419f,
   .nbPerms=NB_PERM44,
   .perms=perm44,
   .nbTwiddle=NB_TWIDDLE44,
   .twiddle=twiddle44,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors44,
   .lengths=lengths44,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config45={
   .normalization=0.01250000000000000069f,
   .nbPerms=NB_PERM45,
   .perms=perm45,
   .nbTwiddle=NB_TWIDDLE45,
   .twiddle=twiddle45,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors45,
   .lengths=lengths45,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config46={
   .normalization=0.01234567901234567833f,
   .nbPerms=NB_PERM46,
   .perms=perm46,
   .nbTwiddle=NB_TWIDDLE46,
   .twiddle=twiddle46,
   .nbGroupedFactors=1,
   .nbFactors=4,
   .factors=factors46,
   .lengths=lengths46,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float32_t> config47={
   .normalization=0.00462962962962962937f,
   .nbPerms=NB_PERM47,
   .perms=perm47,
   .nbTwiddle=NB_TWIDDLE47,
   .twiddle=twiddle47,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors47,
   .lengths=lengths47,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config48={
   .normalization=0.01111111111111111154f,
   .nbPerms=NB_PERM48,
   .perms=perm48,
   .nbTwiddle=NB_TWIDDLE48,
   .twiddle=twiddle48,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors48,
   .lengths=lengths48,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config49={
   .normalization=0.01041666666666666609f,
   .nbPerms=NB_PERM49,
   .perms=perm49,
   .nbTwiddle=NB_TWIDDLE49,
   .twiddle=twiddle49,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors49,
   .lengths=lengths49,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config50={
   .normalization=0.01000000000000000021f,
   .nbPerms=NB_PERM50,
   .perms=perm50,
   .nbTwiddle=NB_TWIDDLE50,
   .twiddle=twiddle50,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors50,
   .lengths=lengths50,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config51={
   .normalization=0.00925925925925925875f,
   .nbPerms=NB_PERM51,
   .perms=perm51,
   .nbTwiddle=NB_TWIDDLE51,
   .twiddle=twiddle51,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors51,
   .lengths=lengths51,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config52={
   .normalization=0.00416666666666666661f,
   .nbPerms=NB_PERM52,
   .perms=perm52,
   .nbTwiddle=NB_TWIDDLE52,
   .twiddle=twiddle52,
   .nbGroupedFactors=4,
   .nbFactors=4,
   .factors=factors52,
   .lengths=lengths52,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config53={
   .normalization=0.00833333333333333322f,
   .nbPerms=NB_PERM53,
   .perms=perm53,
   .nbTwiddle=NB_TWIDDLE53,
   .twiddle=twiddle53,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors53,
   .lengths=lengths53,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float32_t> config54={
   .normalization=0.00800000000000000017f,
   .nbPerms=NB_PERM54,
   .perms=perm54,
   .nbTwiddle=NB_TWIDDLE54,
   .twiddle=twiddle54,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors54,
   .lengths=lengths54,
   .format=0,
   .reversalVectorizable=1
   };

template<>
const cfftconfig<float32_t> *cfft_config<float32_t>(uint32_t nbSamples)
{
    switch(nbSamples)
    {
        case 2:
            return(&config3);
        break;
        case 3:
            return(&config4);
        break;
        case 4:
            return(&config5);
        break;
        case 5:
            return(&config6);
        break;
        case 6:
            return(&config7);
        break;
        case 8:
            return(&config9);
        break;
        case 9:
            return(&config10);
        break;
        case 10:
            return(&config11);
        break;
        case 12:
            return(&config13);
        break;
        case 15:
            return(&config15);
        break;
        case 16:
            return(&config16);
        break;
        case 18:
            return(&config18);
        break;
        case 20:
            return(&config20);
        break;
        case 24:
            return(&config22);
        break;
        case 25:
            return(&config23);
        break;
        case 27:
            return(&config24);
        break;
        case 30:
            return(&config25);
        break;
        case 32:
            return(&config26);
        break;
        case 36:
            return(&config30);
        break;
        case 40:
            return(&config31);
        break;
        case 45:
            return(&config32);
        break;
        case 48:
            return(&config33);
        break;
        case 50:
            return(&config34);
        break;
        case 54:
            return(&config36);
        break;
        case 60:
            return(&config37);
        break;
        case 64:
            return(&config38);
        break;
        case 72:
            return(&config42);
        break;
        case 75:
            return(&config44);
        break;
        case 80:
            return(&config45);
        break;
        case 81:
            return(&config46);
        break;
        case 90:
            return(&config48);
        break;
        case 96:
            return(&config49);
        break;
        case 100:
            return(&config50);
        break;
        case 108:
            return(&config51);
        break;
        case 120:
            return(&config53);
        break;
        case 125:
            return(&config54);
        break;
        case 128:
            return(&config12);
        break;
        case 144:
            return(&config17);
        break;
        case 150:
            return(&config21);
        break;
        case 160:
            return(&config29);
        break;
        case 180:
            return(&config35);
        break;
        case 192:
            return(&config41);
        break;
        case 200:
            return(&config43);
        break;
        case 216:
            return(&config47);
        break;
        case 240:
            return(&config52);
        break;
        case 256:
            return(&config1);
        break;
        case 288:
            return(&config27);
        break;
        case 320:
            return(&config39);
        break;
        case 384:
            return(&config14);
        break;
        case 400:
            return(&config19);
        break;
        case 512:
            return(&config8);
        break;
        case 1024:
            return(&config28);
        break;
        case 2048:
            return(&config40);
        break;
        case 4096:
            return(&config2);
        break;

    }
    return(NULL);
}

#if defined(ARM_FLOAT16_SUPPORTED)
cfftconfig<float16_t> config55={
   .normalization=0.00390625000000000000f16,
   .nbPerms=NB_PERM1,
   .perms=perm1,
   .nbTwiddle=NB_TWIDDLE55,
   .twiddle=twiddle55,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors1,
   .lengths=lengths1,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config56={
   .normalization=0.00024414062500000000f16,
   .nbPerms=NB_PERM2,
   .perms=perm2,
   .nbTwiddle=NB_TWIDDLE56,
   .twiddle=twiddle56,
   .nbGroupedFactors=1,
   .nbFactors=6,
   .factors=factors2,
   .lengths=lengths2,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config57={
   .normalization=0.50000000000000000000f16,
   .nbPerms=NB_PERM3,
   .perms=perm3,
   .nbTwiddle=NB_TWIDDLE57,
   .twiddle=twiddle57,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors3,
   .lengths=lengths3,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config58={
   .normalization=0.33333333333333331483f16,
   .nbPerms=NB_PERM4,
   .perms=perm4,
   .nbTwiddle=NB_TWIDDLE58,
   .twiddle=twiddle58,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors4,
   .lengths=lengths4,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config59={
   .normalization=0.25000000000000000000f16,
   .nbPerms=NB_PERM5,
   .perms=perm5,
   .nbTwiddle=NB_TWIDDLE59,
   .twiddle=twiddle59,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors5,
   .lengths=lengths5,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config60={
   .normalization=0.20000000000000001110f16,
   .nbPerms=NB_PERM6,
   .perms=perm6,
   .nbTwiddle=NB_TWIDDLE60,
   .twiddle=twiddle60,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors6,
   .lengths=lengths6,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config61={
   .normalization=0.16666666666666665741f16,
   .nbPerms=NB_PERM7,
   .perms=perm7,
   .nbTwiddle=NB_TWIDDLE61,
   .twiddle=twiddle61,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors7,
   .lengths=lengths7,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config62={
   .normalization=0.00195312500000000000f16,
   .nbPerms=NB_PERM8,
   .perms=perm8,
   .nbTwiddle=NB_TWIDDLE62,
   .twiddle=twiddle62,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors8,
   .lengths=lengths8,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config63={
   .normalization=0.12500000000000000000f16,
   .nbPerms=NB_PERM9,
   .perms=perm9,
   .nbTwiddle=NB_TWIDDLE63,
   .twiddle=twiddle63,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors9,
   .lengths=lengths9,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config64={
   .normalization=0.11111111111111110494f16,
   .nbPerms=NB_PERM10,
   .perms=perm10,
   .nbTwiddle=NB_TWIDDLE64,
   .twiddle=twiddle64,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors10,
   .lengths=lengths10,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config65={
   .normalization=0.10000000000000000555f16,
   .nbPerms=NB_PERM11,
   .perms=perm11,
   .nbTwiddle=NB_TWIDDLE65,
   .twiddle=twiddle65,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors11,
   .lengths=lengths11,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config66={
   .normalization=0.00781250000000000000f16,
   .nbPerms=NB_PERM12,
   .perms=perm12,
   .nbTwiddle=NB_TWIDDLE66,
   .twiddle=twiddle66,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors12,
   .lengths=lengths12,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config67={
   .normalization=0.08333333333333332871f16,
   .nbPerms=NB_PERM13,
   .perms=perm13,
   .nbTwiddle=NB_TWIDDLE67,
   .twiddle=twiddle67,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors13,
   .lengths=lengths13,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config68={
   .normalization=0.00260416666666666652f16,
   .nbPerms=NB_PERM14,
   .perms=perm14,
   .nbTwiddle=NB_TWIDDLE68,
   .twiddle=twiddle68,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors14,
   .lengths=lengths14,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config69={
   .normalization=0.06666666666666666574f16,
   .nbPerms=NB_PERM15,
   .perms=perm15,
   .nbTwiddle=NB_TWIDDLE69,
   .twiddle=twiddle69,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors15,
   .lengths=lengths15,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config70={
   .normalization=0.06250000000000000000f16,
   .nbPerms=NB_PERM16,
   .perms=perm16,
   .nbTwiddle=NB_TWIDDLE70,
   .twiddle=twiddle70,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors16,
   .lengths=lengths16,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config71={
   .normalization=0.00694444444444444406f16,
   .nbPerms=NB_PERM17,
   .perms=perm17,
   .nbTwiddle=NB_TWIDDLE71,
   .twiddle=twiddle71,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors17,
   .lengths=lengths17,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config72={
   .normalization=0.05555555555555555247f16,
   .nbPerms=NB_PERM18,
   .perms=perm18,
   .nbTwiddle=NB_TWIDDLE72,
   .twiddle=twiddle72,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors18,
   .lengths=lengths18,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config73={
   .normalization=0.00250000000000000005f16,
   .nbPerms=NB_PERM19,
   .perms=perm19,
   .nbTwiddle=NB_TWIDDLE73,
   .twiddle=twiddle73,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors19,
   .lengths=lengths19,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config74={
   .normalization=0.05000000000000000278f16,
   .nbPerms=NB_PERM20,
   .perms=perm20,
   .nbTwiddle=NB_TWIDDLE74,
   .twiddle=twiddle74,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors20,
   .lengths=lengths20,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config75={
   .normalization=0.00666666666666666709f16,
   .nbPerms=NB_PERM21,
   .perms=perm21,
   .nbTwiddle=NB_TWIDDLE75,
   .twiddle=twiddle75,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors21,
   .lengths=lengths21,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config76={
   .normalization=0.04166666666666666435f16,
   .nbPerms=NB_PERM22,
   .perms=perm22,
   .nbTwiddle=NB_TWIDDLE76,
   .twiddle=twiddle76,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors22,
   .lengths=lengths22,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config77={
   .normalization=0.04000000000000000083f16,
   .nbPerms=NB_PERM23,
   .perms=perm23,
   .nbTwiddle=NB_TWIDDLE77,
   .twiddle=twiddle77,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors23,
   .lengths=lengths23,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config78={
   .normalization=0.03703703703703703498f16,
   .nbPerms=NB_PERM24,
   .perms=perm24,
   .nbTwiddle=NB_TWIDDLE78,
   .twiddle=twiddle78,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors24,
   .lengths=lengths24,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config79={
   .normalization=0.03333333333333333287f16,
   .nbPerms=NB_PERM25,
   .perms=perm25,
   .nbTwiddle=NB_TWIDDLE79,
   .twiddle=twiddle79,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors25,
   .lengths=lengths25,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config80={
   .normalization=0.03125000000000000000f16,
   .nbPerms=NB_PERM26,
   .perms=perm26,
   .nbTwiddle=NB_TWIDDLE80,
   .twiddle=twiddle80,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors26,
   .lengths=lengths26,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config81={
   .normalization=0.00347222222222222203f16,
   .nbPerms=NB_PERM27,
   .perms=perm27,
   .nbTwiddle=NB_TWIDDLE81,
   .twiddle=twiddle81,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors27,
   .lengths=lengths27,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config82={
   .normalization=0.00097656250000000000f16,
   .nbPerms=NB_PERM28,
   .perms=perm28,
   .nbTwiddle=NB_TWIDDLE82,
   .twiddle=twiddle82,
   .nbGroupedFactors=1,
   .nbFactors=5,
   .factors=factors28,
   .lengths=lengths28,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config83={
   .normalization=0.00625000000000000035f16,
   .nbPerms=NB_PERM29,
   .perms=perm29,
   .nbTwiddle=NB_TWIDDLE83,
   .twiddle=twiddle83,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors29,
   .lengths=lengths29,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config84={
   .normalization=0.02777777777777777624f16,
   .nbPerms=NB_PERM30,
   .perms=perm30,
   .nbTwiddle=NB_TWIDDLE84,
   .twiddle=twiddle84,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors30,
   .lengths=lengths30,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config85={
   .normalization=0.02500000000000000139f16,
   .nbPerms=NB_PERM31,
   .perms=perm31,
   .nbTwiddle=NB_TWIDDLE85,
   .twiddle=twiddle85,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors31,
   .lengths=lengths31,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config86={
   .normalization=0.02222222222222222307f16,
   .nbPerms=NB_PERM32,
   .perms=perm32,
   .nbTwiddle=NB_TWIDDLE86,
   .twiddle=twiddle86,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors32,
   .lengths=lengths32,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config87={
   .normalization=0.02083333333333333218f16,
   .nbPerms=NB_PERM33,
   .perms=perm33,
   .nbTwiddle=NB_TWIDDLE87,
   .twiddle=twiddle87,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors33,
   .lengths=lengths33,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config88={
   .normalization=0.02000000000000000042f16,
   .nbPerms=NB_PERM34,
   .perms=perm34,
   .nbTwiddle=NB_TWIDDLE88,
   .twiddle=twiddle88,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors34,
   .lengths=lengths34,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config89={
   .normalization=0.00555555555555555577f16,
   .nbPerms=NB_PERM35,
   .perms=perm35,
   .nbTwiddle=NB_TWIDDLE89,
   .twiddle=twiddle89,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors35,
   .lengths=lengths35,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config90={
   .normalization=0.01851851851851851749f16,
   .nbPerms=NB_PERM36,
   .perms=perm36,
   .nbTwiddle=NB_TWIDDLE90,
   .twiddle=twiddle90,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors36,
   .lengths=lengths36,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config91={
   .normalization=0.01666666666666666644f16,
   .nbPerms=NB_PERM37,
   .perms=perm37,
   .nbTwiddle=NB_TWIDDLE91,
   .twiddle=twiddle91,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors37,
   .lengths=lengths37,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config92={
   .normalization=0.01562500000000000000f16,
   .nbPerms=NB_PERM38,
   .perms=perm38,
   .nbTwiddle=NB_TWIDDLE92,
   .twiddle=twiddle92,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors38,
   .lengths=lengths38,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config93={
   .normalization=0.00312500000000000017f16,
   .nbPerms=NB_PERM39,
   .perms=perm39,
   .nbTwiddle=NB_TWIDDLE93,
   .twiddle=twiddle93,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors39,
   .lengths=lengths39,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config94={
   .normalization=0.00048828125000000000f16,
   .nbPerms=NB_PERM40,
   .perms=perm40,
   .nbTwiddle=NB_TWIDDLE94,
   .twiddle=twiddle94,
   .nbGroupedFactors=2,
   .nbFactors=6,
   .factors=factors40,
   .lengths=lengths40,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config95={
   .normalization=0.00520833333333333304f16,
   .nbPerms=NB_PERM41,
   .perms=perm41,
   .nbTwiddle=NB_TWIDDLE95,
   .twiddle=twiddle95,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors41,
   .lengths=lengths41,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config96={
   .normalization=0.01388888888888888812f16,
   .nbPerms=NB_PERM42,
   .perms=perm42,
   .nbTwiddle=NB_TWIDDLE96,
   .twiddle=twiddle96,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors42,
   .lengths=lengths42,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config97={
   .normalization=0.00500000000000000010f16,
   .nbPerms=NB_PERM43,
   .perms=perm43,
   .nbTwiddle=NB_TWIDDLE97,
   .twiddle=twiddle97,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors43,
   .lengths=lengths43,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config98={
   .normalization=0.01333333333333333419f16,
   .nbPerms=NB_PERM44,
   .perms=perm44,
   .nbTwiddle=NB_TWIDDLE98,
   .twiddle=twiddle98,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors44,
   .lengths=lengths44,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config99={
   .normalization=0.01250000000000000069f16,
   .nbPerms=NB_PERM45,
   .perms=perm45,
   .nbTwiddle=NB_TWIDDLE99,
   .twiddle=twiddle99,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors45,
   .lengths=lengths45,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config100={
   .normalization=0.01234567901234567833f16,
   .nbPerms=NB_PERM46,
   .perms=perm46,
   .nbTwiddle=NB_TWIDDLE100,
   .twiddle=twiddle100,
   .nbGroupedFactors=1,
   .nbFactors=4,
   .factors=factors46,
   .lengths=lengths46,
   .format=0,
   .reversalVectorizable=1
   };
cfftconfig<float16_t> config101={
   .normalization=0.00462962962962962937f16,
   .nbPerms=NB_PERM47,
   .perms=perm47,
   .nbTwiddle=NB_TWIDDLE101,
   .twiddle=twiddle101,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors47,
   .lengths=lengths47,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config102={
   .normalization=0.01111111111111111154f16,
   .nbPerms=NB_PERM48,
   .perms=perm48,
   .nbTwiddle=NB_TWIDDLE102,
   .twiddle=twiddle102,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors48,
   .lengths=lengths48,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config103={
   .normalization=0.01041666666666666609f16,
   .nbPerms=NB_PERM49,
   .perms=perm49,
   .nbTwiddle=NB_TWIDDLE103,
   .twiddle=twiddle103,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors49,
   .lengths=lengths49,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config104={
   .normalization=0.01000000000000000021f16,
   .nbPerms=NB_PERM50,
   .perms=perm50,
   .nbTwiddle=NB_TWIDDLE104,
   .twiddle=twiddle104,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors50,
   .lengths=lengths50,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config105={
   .normalization=0.00925925925925925875f16,
   .nbPerms=NB_PERM51,
   .perms=perm51,
   .nbTwiddle=NB_TWIDDLE105,
   .twiddle=twiddle105,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors51,
   .lengths=lengths51,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config106={
   .normalization=0.00416666666666666661f16,
   .nbPerms=NB_PERM52,
   .perms=perm52,
   .nbTwiddle=NB_TWIDDLE106,
   .twiddle=twiddle106,
   .nbGroupedFactors=4,
   .nbFactors=4,
   .factors=factors52,
   .lengths=lengths52,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config107={
   .normalization=0.00833333333333333322f16,
   .nbPerms=NB_PERM53,
   .perms=perm53,
   .nbTwiddle=NB_TWIDDLE107,
   .twiddle=twiddle107,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors53,
   .lengths=lengths53,
   .format=0,
   .reversalVectorizable=0
   };
cfftconfig<float16_t> config108={
   .normalization=0.00800000000000000017f16,
   .nbPerms=NB_PERM54,
   .perms=perm54,
   .nbTwiddle=NB_TWIDDLE108,
   .twiddle=twiddle108,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors54,
   .lengths=lengths54,
   .format=0,
   .reversalVectorizable=1
   };

template<>
const cfftconfig<float16_t> *cfft_config<float16_t>(uint32_t nbSamples)
{
    switch(nbSamples)
    {
        case 2:
            return(&config57);
        break;
        case 3:
            return(&config58);
        break;
        case 4:
            return(&config59);
        break;
        case 5:
            return(&config60);
        break;
        case 6:
            return(&config61);
        break;
        case 8:
            return(&config63);
        break;
        case 9:
            return(&config64);
        break;
        case 10:
            return(&config65);
        break;
        case 12:
            return(&config67);
        break;
        case 15:
            return(&config69);
        break;
        case 16:
            return(&config70);
        break;
        case 18:
            return(&config72);
        break;
        case 20:
            return(&config74);
        break;
        case 24:
            return(&config76);
        break;
        case 25:
            return(&config77);
        break;
        case 27:
            return(&config78);
        break;
        case 30:
            return(&config79);
        break;
        case 32:
            return(&config80);
        break;
        case 36:
            return(&config84);
        break;
        case 40:
            return(&config85);
        break;
        case 45:
            return(&config86);
        break;
        case 48:
            return(&config87);
        break;
        case 50:
            return(&config88);
        break;
        case 54:
            return(&config90);
        break;
        case 60:
            return(&config91);
        break;
        case 64:
            return(&config92);
        break;
        case 72:
            return(&config96);
        break;
        case 75:
            return(&config98);
        break;
        case 80:
            return(&config99);
        break;
        case 81:
            return(&config100);
        break;
        case 90:
            return(&config102);
        break;
        case 96:
            return(&config103);
        break;
        case 100:
            return(&config104);
        break;
        case 108:
            return(&config105);
        break;
        case 120:
            return(&config107);
        break;
        case 125:
            return(&config108);
        break;
        case 128:
            return(&config66);
        break;
        case 144:
            return(&config71);
        break;
        case 150:
            return(&config75);
        break;
        case 160:
            return(&config83);
        break;
        case 180:
            return(&config89);
        break;
        case 192:
            return(&config95);
        break;
        case 200:
            return(&config97);
        break;
        case 216:
            return(&config101);
        break;
        case 240:
            return(&config106);
        break;
        case 256:
            return(&config55);
        break;
        case 288:
            return(&config81);
        break;
        case 320:
            return(&config93);
        break;
        case 384:
            return(&config68);
        break;
        case 400:
            return(&config73);
        break;
        case 512:
            return(&config62);
        break;
        case 1024:
            return(&config82);
        break;
        case 2048:
            return(&config94);
        break;
        case 4096:
            return(&config56);
        break;

    }
    return(NULL);
}

#endif

cfftconfig<Q31> config109={
   .normalization=Q31(0x00800000),
   .nbPerms=NB_PERM55,
   .perms=perm55,
   .nbTwiddle=NB_TWIDDLE109,
   .twiddle=twiddle109,
   .nbGroupedFactors=1,
   .nbFactors=4,
   .factors=factors55,
   .lengths=lengths55,
   .format=8,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config110={
   .normalization=Q31(0x00080000),
   .nbPerms=NB_PERM2,
   .perms=perm2,
   .nbTwiddle=NB_TWIDDLE110,
   .twiddle=twiddle110,
   .nbGroupedFactors=1,
   .nbFactors=6,
   .factors=factors2,
   .lengths=lengths2,
   .format=12,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config111={
   .normalization=Q31(0x40000000),
   .nbPerms=NB_PERM3,
   .perms=perm3,
   .nbTwiddle=NB_TWIDDLE111,
   .twiddle=twiddle111,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors3,
   .lengths=lengths3,
   .format=1,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config112={
   .normalization=Q31(0x2AAAAAAB),
   .nbPerms=NB_PERM4,
   .perms=perm4,
   .nbTwiddle=NB_TWIDDLE112,
   .twiddle=twiddle112,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors4,
   .lengths=lengths4,
   .format=2,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config113={
   .normalization=Q31(0x20000000),
   .nbPerms=NB_PERM5,
   .perms=perm5,
   .nbTwiddle=NB_TWIDDLE113,
   .twiddle=twiddle113,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors5,
   .lengths=lengths5,
   .format=2,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config114={
   .normalization=Q31(0x1999999A),
   .nbPerms=NB_PERM6,
   .perms=perm6,
   .nbTwiddle=NB_TWIDDLE114,
   .twiddle=twiddle114,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors6,
   .lengths=lengths6,
   .format=4,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config115={
   .normalization=Q31(0x15555555),
   .nbPerms=NB_PERM7,
   .perms=perm7,
   .nbTwiddle=NB_TWIDDLE115,
   .twiddle=twiddle115,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors7,
   .lengths=lengths7,
   .format=3,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config116={
   .normalization=Q31(0x00400000),
   .nbPerms=NB_PERM56,
   .perms=perm56,
   .nbTwiddle=NB_TWIDDLE116,
   .twiddle=twiddle116,
   .nbGroupedFactors=2,
   .nbFactors=5,
   .factors=factors56,
   .lengths=lengths56,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config117={
   .normalization=Q31(0x10000000),
   .nbPerms=NB_PERM57,
   .perms=perm57,
   .nbTwiddle=NB_TWIDDLE117,
   .twiddle=twiddle117,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors57,
   .lengths=lengths57,
   .format=3,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config118={
   .normalization=Q31(0x0E38E38E),
   .nbPerms=NB_PERM10,
   .perms=perm10,
   .nbTwiddle=NB_TWIDDLE118,
   .twiddle=twiddle118,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors10,
   .lengths=lengths10,
   .format=4,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config119={
   .normalization=Q31(0x0CCCCCCD),
   .nbPerms=NB_PERM11,
   .perms=perm11,
   .nbTwiddle=NB_TWIDDLE119,
   .twiddle=twiddle119,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors11,
   .lengths=lengths11,
   .format=5,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config120={
   .normalization=Q31(0x01000000),
   .nbPerms=NB_PERM58,
   .perms=perm58,
   .nbTwiddle=NB_TWIDDLE120,
   .twiddle=twiddle120,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors58,
   .lengths=lengths58,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config121={
   .normalization=Q31(0x0AAAAAAB),
   .nbPerms=NB_PERM13,
   .perms=perm13,
   .nbTwiddle=NB_TWIDDLE121,
   .twiddle=twiddle121,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors13,
   .lengths=lengths13,
   .format=4,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config122={
   .normalization=Q31(0x00555555),
   .nbPerms=NB_PERM59,
   .perms=perm59,
   .nbTwiddle=NB_TWIDDLE122,
   .twiddle=twiddle122,
   .nbGroupedFactors=3,
   .nbFactors=5,
   .factors=factors59,
   .lengths=lengths59,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config123={
   .normalization=Q31(0x08888889),
   .nbPerms=NB_PERM15,
   .perms=perm15,
   .nbTwiddle=NB_TWIDDLE123,
   .twiddle=twiddle123,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors15,
   .lengths=lengths15,
   .format=6,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config124={
   .normalization=Q31(0x08000000),
   .nbPerms=NB_PERM60,
   .perms=perm60,
   .nbTwiddle=NB_TWIDDLE124,
   .twiddle=twiddle124,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors60,
   .lengths=lengths60,
   .format=4,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config125={
   .normalization=Q31(0x00E38E39),
   .nbPerms=NB_PERM61,
   .perms=perm61,
   .nbTwiddle=NB_TWIDDLE125,
   .twiddle=twiddle125,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors61,
   .lengths=lengths61,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config126={
   .normalization=Q31(0x071C71C7),
   .nbPerms=NB_PERM18,
   .perms=perm18,
   .nbTwiddle=NB_TWIDDLE126,
   .twiddle=twiddle126,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors18,
   .lengths=lengths18,
   .format=5,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config127={
   .normalization=Q31(0x0051EB85),
   .nbPerms=NB_PERM62,
   .perms=perm62,
   .nbTwiddle=NB_TWIDDLE127,
   .twiddle=twiddle127,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors62,
   .lengths=lengths62,
   .format=12,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config128={
   .normalization=Q31(0x06666666),
   .nbPerms=NB_PERM20,
   .perms=perm20,
   .nbTwiddle=NB_TWIDDLE128,
   .twiddle=twiddle128,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors20,
   .lengths=lengths20,
   .format=6,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config129={
   .normalization=Q31(0x00DA740E),
   .nbPerms=NB_PERM21,
   .perms=perm21,
   .nbTwiddle=NB_TWIDDLE129,
   .twiddle=twiddle129,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors21,
   .lengths=lengths21,
   .format=11,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config130={
   .normalization=Q31(0x05555555),
   .nbPerms=NB_PERM63,
   .perms=perm63,
   .nbTwiddle=NB_TWIDDLE130,
   .twiddle=twiddle130,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors63,
   .lengths=lengths63,
   .format=5,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config131={
   .normalization=Q31(0x051EB852),
   .nbPerms=NB_PERM23,
   .perms=perm23,
   .nbTwiddle=NB_TWIDDLE131,
   .twiddle=twiddle131,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors23,
   .lengths=lengths23,
   .format=8,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config132={
   .normalization=Q31(0x04BDA12F),
   .nbPerms=NB_PERM24,
   .perms=perm24,
   .nbTwiddle=NB_TWIDDLE132,
   .twiddle=twiddle132,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors24,
   .lengths=lengths24,
   .format=6,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config133={
   .normalization=Q31(0x04444444),
   .nbPerms=NB_PERM25,
   .perms=perm25,
   .nbTwiddle=NB_TWIDDLE133,
   .twiddle=twiddle133,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors25,
   .lengths=lengths25,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config134={
   .normalization=Q31(0x04000000),
   .nbPerms=NB_PERM64,
   .perms=perm64,
   .nbTwiddle=NB_TWIDDLE134,
   .twiddle=twiddle134,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors64,
   .lengths=lengths64,
   .format=5,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config135={
   .normalization=Q31(0x0071C71C),
   .nbPerms=NB_PERM65,
   .perms=perm65,
   .nbTwiddle=NB_TWIDDLE135,
   .twiddle=twiddle135,
   .nbGroupedFactors=3,
   .nbFactors=5,
   .factors=factors65,
   .lengths=lengths65,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config136={
   .normalization=Q31(0x00200000),
   .nbPerms=NB_PERM28,
   .perms=perm28,
   .nbTwiddle=NB_TWIDDLE136,
   .twiddle=twiddle136,
   .nbGroupedFactors=1,
   .nbFactors=5,
   .factors=factors28,
   .lengths=lengths28,
   .format=10,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config137={
   .normalization=Q31(0x00CCCCCD),
   .nbPerms=NB_PERM66,
   .perms=perm66,
   .nbTwiddle=NB_TWIDDLE137,
   .twiddle=twiddle137,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors66,
   .lengths=lengths66,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config138={
   .normalization=Q31(0x038E38E4),
   .nbPerms=NB_PERM30,
   .perms=perm30,
   .nbTwiddle=NB_TWIDDLE138,
   .twiddle=twiddle138,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors30,
   .lengths=lengths30,
   .format=6,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config139={
   .normalization=Q31(0x03333333),
   .nbPerms=NB_PERM67,
   .perms=perm67,
   .nbTwiddle=NB_TWIDDLE139,
   .twiddle=twiddle139,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors67,
   .lengths=lengths67,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config140={
   .normalization=Q31(0x02D82D83),
   .nbPerms=NB_PERM32,
   .perms=perm32,
   .nbTwiddle=NB_TWIDDLE140,
   .twiddle=twiddle140,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors32,
   .lengths=lengths32,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config141={
   .normalization=Q31(0x02AAAAAB),
   .nbPerms=NB_PERM68,
   .perms=perm68,
   .nbTwiddle=NB_TWIDDLE141,
   .twiddle=twiddle141,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors68,
   .lengths=lengths68,
   .format=6,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config142={
   .normalization=Q31(0x028F5C29),
   .nbPerms=NB_PERM34,
   .perms=perm34,
   .nbTwiddle=NB_TWIDDLE142,
   .twiddle=twiddle142,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors34,
   .lengths=lengths34,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config143={
   .normalization=Q31(0x00B60B61),
   .nbPerms=NB_PERM35,
   .perms=perm35,
   .nbTwiddle=NB_TWIDDLE143,
   .twiddle=twiddle143,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors35,
   .lengths=lengths35,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config144={
   .normalization=Q31(0x025ED098),
   .nbPerms=NB_PERM36,
   .perms=perm36,
   .nbTwiddle=NB_TWIDDLE144,
   .twiddle=twiddle144,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors36,
   .lengths=lengths36,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config145={
   .normalization=Q31(0x02222222),
   .nbPerms=NB_PERM37,
   .perms=perm37,
   .nbTwiddle=NB_TWIDDLE145,
   .twiddle=twiddle145,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors37,
   .lengths=lengths37,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config146={
   .normalization=Q31(0x02000000),
   .nbPerms=NB_PERM69,
   .perms=perm69,
   .nbTwiddle=NB_TWIDDLE146,
   .twiddle=twiddle146,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors69,
   .lengths=lengths69,
   .format=6,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config147={
   .normalization=Q31(0x00666666),
   .nbPerms=NB_PERM70,
   .perms=perm70,
   .nbTwiddle=NB_TWIDDLE147,
   .twiddle=twiddle147,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors70,
   .lengths=lengths70,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config148={
   .normalization=Q31(0x00100000),
   .nbPerms=NB_PERM40,
   .perms=perm40,
   .nbTwiddle=NB_TWIDDLE148,
   .twiddle=twiddle148,
   .nbGroupedFactors=2,
   .nbFactors=6,
   .factors=factors40,
   .lengths=lengths40,
   .format=11,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config149={
   .normalization=Q31(0x00AAAAAB),
   .nbPerms=NB_PERM71,
   .perms=perm71,
   .nbTwiddle=NB_TWIDDLE149,
   .twiddle=twiddle149,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors71,
   .lengths=lengths71,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config150={
   .normalization=Q31(0x01C71C72),
   .nbPerms=NB_PERM72,
   .perms=perm72,
   .nbTwiddle=NB_TWIDDLE150,
   .twiddle=twiddle150,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors72,
   .lengths=lengths72,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config151={
   .normalization=Q31(0x00A3D70A),
   .nbPerms=NB_PERM73,
   .perms=perm73,
   .nbTwiddle=NB_TWIDDLE151,
   .twiddle=twiddle151,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors73,
   .lengths=lengths73,
   .format=11,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config152={
   .normalization=Q31(0x01B4E81B),
   .nbPerms=NB_PERM44,
   .perms=perm44,
   .nbTwiddle=NB_TWIDDLE152,
   .twiddle=twiddle152,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors44,
   .lengths=lengths44,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config153={
   .normalization=Q31(0x0199999A),
   .nbPerms=NB_PERM74,
   .perms=perm74,
   .nbTwiddle=NB_TWIDDLE153,
   .twiddle=twiddle153,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors74,
   .lengths=lengths74,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config154={
   .normalization=Q31(0x01948B10),
   .nbPerms=NB_PERM46,
   .perms=perm46,
   .nbTwiddle=NB_TWIDDLE154,
   .twiddle=twiddle154,
   .nbGroupedFactors=1,
   .nbFactors=4,
   .factors=factors46,
   .lengths=lengths46,
   .format=8,
   .reversalVectorizable=1
   };
cfftconfig<Q31> config155={
   .normalization=Q31(0x0097B426),
   .nbPerms=NB_PERM75,
   .perms=perm75,
   .nbTwiddle=NB_TWIDDLE155,
   .twiddle=twiddle155,
   .nbGroupedFactors=3,
   .nbFactors=5,
   .factors=factors75,
   .lengths=lengths75,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config156={
   .normalization=Q31(0x016C16C1),
   .nbPerms=NB_PERM48,
   .perms=perm48,
   .nbTwiddle=NB_TWIDDLE156,
   .twiddle=twiddle156,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors48,
   .lengths=lengths48,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config157={
   .normalization=Q31(0x01555555),
   .nbPerms=NB_PERM76,
   .perms=perm76,
   .nbTwiddle=NB_TWIDDLE157,
   .twiddle=twiddle157,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors76,
   .lengths=lengths76,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config158={
   .normalization=Q31(0x0147AE14),
   .nbPerms=NB_PERM50,
   .perms=perm50,
   .nbTwiddle=NB_TWIDDLE158,
   .twiddle=twiddle158,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors50,
   .lengths=lengths50,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config159={
   .normalization=Q31(0x012F684C),
   .nbPerms=NB_PERM51,
   .perms=perm51,
   .nbTwiddle=NB_TWIDDLE159,
   .twiddle=twiddle159,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors51,
   .lengths=lengths51,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config160={
   .normalization=Q31(0x00888889),
   .nbPerms=NB_PERM77,
   .perms=perm77,
   .nbTwiddle=NB_TWIDDLE160,
   .twiddle=twiddle160,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors77,
   .lengths=lengths77,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config161={
   .normalization=Q31(0x01111111),
   .nbPerms=NB_PERM78,
   .perms=perm78,
   .nbTwiddle=NB_TWIDDLE161,
   .twiddle=twiddle161,
   .nbGroupedFactors=4,
   .nbFactors=4,
   .factors=factors78,
   .lengths=lengths78,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q31> config162={
   .normalization=Q31(0x010624DD),
   .nbPerms=NB_PERM54,
   .perms=perm54,
   .nbTwiddle=NB_TWIDDLE162,
   .twiddle=twiddle162,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors54,
   .lengths=lengths54,
   .format=12,
   .reversalVectorizable=1
   };

template<>
const cfftconfig<Q31> *cfft_config<Q31>(uint32_t nbSamples)
{
    switch(nbSamples)
    {
        case 2:
            return(&config111);
        break;
        case 3:
            return(&config112);
        break;
        case 4:
            return(&config113);
        break;
        case 5:
            return(&config114);
        break;
        case 6:
            return(&config115);
        break;
        case 8:
            return(&config117);
        break;
        case 9:
            return(&config118);
        break;
        case 10:
            return(&config119);
        break;
        case 12:
            return(&config121);
        break;
        case 15:
            return(&config123);
        break;
        case 16:
            return(&config124);
        break;
        case 18:
            return(&config126);
        break;
        case 20:
            return(&config128);
        break;
        case 24:
            return(&config130);
        break;
        case 25:
            return(&config131);
        break;
        case 27:
            return(&config132);
        break;
        case 30:
            return(&config133);
        break;
        case 32:
            return(&config134);
        break;
        case 36:
            return(&config138);
        break;
        case 40:
            return(&config139);
        break;
        case 45:
            return(&config140);
        break;
        case 48:
            return(&config141);
        break;
        case 50:
            return(&config142);
        break;
        case 54:
            return(&config144);
        break;
        case 60:
            return(&config145);
        break;
        case 64:
            return(&config146);
        break;
        case 72:
            return(&config150);
        break;
        case 75:
            return(&config152);
        break;
        case 80:
            return(&config153);
        break;
        case 81:
            return(&config154);
        break;
        case 90:
            return(&config156);
        break;
        case 96:
            return(&config157);
        break;
        case 100:
            return(&config158);
        break;
        case 108:
            return(&config159);
        break;
        case 120:
            return(&config161);
        break;
        case 125:
            return(&config162);
        break;
        case 128:
            return(&config120);
        break;
        case 144:
            return(&config125);
        break;
        case 150:
            return(&config129);
        break;
        case 160:
            return(&config137);
        break;
        case 180:
            return(&config143);
        break;
        case 192:
            return(&config149);
        break;
        case 200:
            return(&config151);
        break;
        case 216:
            return(&config155);
        break;
        case 240:
            return(&config160);
        break;
        case 256:
            return(&config109);
        break;
        case 288:
            return(&config135);
        break;
        case 320:
            return(&config147);
        break;
        case 384:
            return(&config122);
        break;
        case 400:
            return(&config127);
        break;
        case 512:
            return(&config116);
        break;
        case 1024:
            return(&config136);
        break;
        case 2048:
            return(&config148);
        break;
        case 4096:
            return(&config110);
        break;

    }
    return(NULL);
}

cfftconfig<Q15> config163={
   .normalization=Q15(0x0080),
   .nbPerms=NB_PERM55,
   .perms=perm55,
   .nbTwiddle=NB_TWIDDLE163,
   .twiddle=twiddle163,
   .nbGroupedFactors=1,
   .nbFactors=4,
   .factors=factors55,
   .lengths=lengths55,
   .format=8,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config164={
   .normalization=Q15(0x0008),
   .nbPerms=NB_PERM2,
   .perms=perm2,
   .nbTwiddle=NB_TWIDDLE164,
   .twiddle=twiddle164,
   .nbGroupedFactors=1,
   .nbFactors=6,
   .factors=factors2,
   .lengths=lengths2,
   .format=12,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config165={
   .normalization=Q15(0x4000),
   .nbPerms=NB_PERM3,
   .perms=perm3,
   .nbTwiddle=NB_TWIDDLE165,
   .twiddle=twiddle165,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors3,
   .lengths=lengths3,
   .format=1,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config166={
   .normalization=Q15(0x2AAB),
   .nbPerms=NB_PERM4,
   .perms=perm4,
   .nbTwiddle=NB_TWIDDLE166,
   .twiddle=twiddle166,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors4,
   .lengths=lengths4,
   .format=2,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config167={
   .normalization=Q15(0x2000),
   .nbPerms=NB_PERM5,
   .perms=perm5,
   .nbTwiddle=NB_TWIDDLE167,
   .twiddle=twiddle167,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors5,
   .lengths=lengths5,
   .format=2,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config168={
   .normalization=Q15(0x199A),
   .nbPerms=NB_PERM6,
   .perms=perm6,
   .nbTwiddle=NB_TWIDDLE168,
   .twiddle=twiddle168,
   .nbGroupedFactors=1,
   .nbFactors=1,
   .factors=factors6,
   .lengths=lengths6,
   .format=4,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config169={
   .normalization=Q15(0x1555),
   .nbPerms=NB_PERM7,
   .perms=perm7,
   .nbTwiddle=NB_TWIDDLE169,
   .twiddle=twiddle169,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors7,
   .lengths=lengths7,
   .format=3,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config170={
   .normalization=Q15(0x0040),
   .nbPerms=NB_PERM56,
   .perms=perm56,
   .nbTwiddle=NB_TWIDDLE170,
   .twiddle=twiddle170,
   .nbGroupedFactors=2,
   .nbFactors=5,
   .factors=factors56,
   .lengths=lengths56,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config171={
   .normalization=Q15(0x1000),
   .nbPerms=NB_PERM57,
   .perms=perm57,
   .nbTwiddle=NB_TWIDDLE171,
   .twiddle=twiddle171,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors57,
   .lengths=lengths57,
   .format=3,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config172={
   .normalization=Q15(0x0E39),
   .nbPerms=NB_PERM10,
   .perms=perm10,
   .nbTwiddle=NB_TWIDDLE172,
   .twiddle=twiddle172,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors10,
   .lengths=lengths10,
   .format=4,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config173={
   .normalization=Q15(0x0CCD),
   .nbPerms=NB_PERM11,
   .perms=perm11,
   .nbTwiddle=NB_TWIDDLE173,
   .twiddle=twiddle173,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors11,
   .lengths=lengths11,
   .format=5,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config174={
   .normalization=Q15(0x0100),
   .nbPerms=NB_PERM58,
   .perms=perm58,
   .nbTwiddle=NB_TWIDDLE174,
   .twiddle=twiddle174,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors58,
   .lengths=lengths58,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config175={
   .normalization=Q15(0x0AAB),
   .nbPerms=NB_PERM13,
   .perms=perm13,
   .nbTwiddle=NB_TWIDDLE175,
   .twiddle=twiddle175,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors13,
   .lengths=lengths13,
   .format=4,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config176={
   .normalization=Q15(0x0055),
   .nbPerms=NB_PERM59,
   .perms=perm59,
   .nbTwiddle=NB_TWIDDLE176,
   .twiddle=twiddle176,
   .nbGroupedFactors=3,
   .nbFactors=5,
   .factors=factors59,
   .lengths=lengths59,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config177={
   .normalization=Q15(0x0889),
   .nbPerms=NB_PERM15,
   .perms=perm15,
   .nbTwiddle=NB_TWIDDLE177,
   .twiddle=twiddle177,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors15,
   .lengths=lengths15,
   .format=6,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config178={
   .normalization=Q15(0x0800),
   .nbPerms=NB_PERM60,
   .perms=perm60,
   .nbTwiddle=NB_TWIDDLE178,
   .twiddle=twiddle178,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors60,
   .lengths=lengths60,
   .format=4,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config179={
   .normalization=Q15(0x00E4),
   .nbPerms=NB_PERM61,
   .perms=perm61,
   .nbTwiddle=NB_TWIDDLE179,
   .twiddle=twiddle179,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors61,
   .lengths=lengths61,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config180={
   .normalization=Q15(0x071C),
   .nbPerms=NB_PERM18,
   .perms=perm18,
   .nbTwiddle=NB_TWIDDLE180,
   .twiddle=twiddle180,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors18,
   .lengths=lengths18,
   .format=5,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config181={
   .normalization=Q15(0x0052),
   .nbPerms=NB_PERM62,
   .perms=perm62,
   .nbTwiddle=NB_TWIDDLE181,
   .twiddle=twiddle181,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors62,
   .lengths=lengths62,
   .format=12,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config182={
   .normalization=Q15(0x0666),
   .nbPerms=NB_PERM20,
   .perms=perm20,
   .nbTwiddle=NB_TWIDDLE182,
   .twiddle=twiddle182,
   .nbGroupedFactors=2,
   .nbFactors=2,
   .factors=factors20,
   .lengths=lengths20,
   .format=6,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config183={
   .normalization=Q15(0x00DA),
   .nbPerms=NB_PERM21,
   .perms=perm21,
   .nbTwiddle=NB_TWIDDLE183,
   .twiddle=twiddle183,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors21,
   .lengths=lengths21,
   .format=11,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config184={
   .normalization=Q15(0x0555),
   .nbPerms=NB_PERM63,
   .perms=perm63,
   .nbTwiddle=NB_TWIDDLE184,
   .twiddle=twiddle184,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors63,
   .lengths=lengths63,
   .format=5,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config185={
   .normalization=Q15(0x051F),
   .nbPerms=NB_PERM23,
   .perms=perm23,
   .nbTwiddle=NB_TWIDDLE185,
   .twiddle=twiddle185,
   .nbGroupedFactors=1,
   .nbFactors=2,
   .factors=factors23,
   .lengths=lengths23,
   .format=8,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config186={
   .normalization=Q15(0x04BE),
   .nbPerms=NB_PERM24,
   .perms=perm24,
   .nbTwiddle=NB_TWIDDLE186,
   .twiddle=twiddle186,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors24,
   .lengths=lengths24,
   .format=6,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config187={
   .normalization=Q15(0x0444),
   .nbPerms=NB_PERM25,
   .perms=perm25,
   .nbTwiddle=NB_TWIDDLE187,
   .twiddle=twiddle187,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors25,
   .lengths=lengths25,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config188={
   .normalization=Q15(0x0400),
   .nbPerms=NB_PERM64,
   .perms=perm64,
   .nbTwiddle=NB_TWIDDLE188,
   .twiddle=twiddle188,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors64,
   .lengths=lengths64,
   .format=5,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config189={
   .normalization=Q15(0x0072),
   .nbPerms=NB_PERM65,
   .perms=perm65,
   .nbTwiddle=NB_TWIDDLE189,
   .twiddle=twiddle189,
   .nbGroupedFactors=3,
   .nbFactors=5,
   .factors=factors65,
   .lengths=lengths65,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config190={
   .normalization=Q15(0x0020),
   .nbPerms=NB_PERM28,
   .perms=perm28,
   .nbTwiddle=NB_TWIDDLE190,
   .twiddle=twiddle190,
   .nbGroupedFactors=1,
   .nbFactors=5,
   .factors=factors28,
   .lengths=lengths28,
   .format=10,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config191={
   .normalization=Q15(0x00CD),
   .nbPerms=NB_PERM66,
   .perms=perm66,
   .nbTwiddle=NB_TWIDDLE191,
   .twiddle=twiddle191,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors66,
   .lengths=lengths66,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config192={
   .normalization=Q15(0x038E),
   .nbPerms=NB_PERM30,
   .perms=perm30,
   .nbTwiddle=NB_TWIDDLE192,
   .twiddle=twiddle192,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors30,
   .lengths=lengths30,
   .format=6,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config193={
   .normalization=Q15(0x0333),
   .nbPerms=NB_PERM67,
   .perms=perm67,
   .nbTwiddle=NB_TWIDDLE193,
   .twiddle=twiddle193,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors67,
   .lengths=lengths67,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config194={
   .normalization=Q15(0x02D8),
   .nbPerms=NB_PERM32,
   .perms=perm32,
   .nbTwiddle=NB_TWIDDLE194,
   .twiddle=twiddle194,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors32,
   .lengths=lengths32,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config195={
   .normalization=Q15(0x02AB),
   .nbPerms=NB_PERM68,
   .perms=perm68,
   .nbTwiddle=NB_TWIDDLE195,
   .twiddle=twiddle195,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors68,
   .lengths=lengths68,
   .format=6,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config196={
   .normalization=Q15(0x028F),
   .nbPerms=NB_PERM34,
   .perms=perm34,
   .nbTwiddle=NB_TWIDDLE196,
   .twiddle=twiddle196,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors34,
   .lengths=lengths34,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config197={
   .normalization=Q15(0x00B6),
   .nbPerms=NB_PERM35,
   .perms=perm35,
   .nbTwiddle=NB_TWIDDLE197,
   .twiddle=twiddle197,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors35,
   .lengths=lengths35,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config198={
   .normalization=Q15(0x025F),
   .nbPerms=NB_PERM36,
   .perms=perm36,
   .nbTwiddle=NB_TWIDDLE198,
   .twiddle=twiddle198,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors36,
   .lengths=lengths36,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config199={
   .normalization=Q15(0x0222),
   .nbPerms=NB_PERM37,
   .perms=perm37,
   .nbTwiddle=NB_TWIDDLE199,
   .twiddle=twiddle199,
   .nbGroupedFactors=3,
   .nbFactors=3,
   .factors=factors37,
   .lengths=lengths37,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config200={
   .normalization=Q15(0x0200),
   .nbPerms=NB_PERM69,
   .perms=perm69,
   .nbTwiddle=NB_TWIDDLE200,
   .twiddle=twiddle200,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors69,
   .lengths=lengths69,
   .format=6,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config201={
   .normalization=Q15(0x0066),
   .nbPerms=NB_PERM70,
   .perms=perm70,
   .nbTwiddle=NB_TWIDDLE201,
   .twiddle=twiddle201,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors70,
   .lengths=lengths70,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config202={
   .normalization=Q15(0x0010),
   .nbPerms=NB_PERM40,
   .perms=perm40,
   .nbTwiddle=NB_TWIDDLE202,
   .twiddle=twiddle202,
   .nbGroupedFactors=2,
   .nbFactors=6,
   .factors=factors40,
   .lengths=lengths40,
   .format=11,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config203={
   .normalization=Q15(0x00AB),
   .nbPerms=NB_PERM71,
   .perms=perm71,
   .nbTwiddle=NB_TWIDDLE203,
   .twiddle=twiddle203,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors71,
   .lengths=lengths71,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config204={
   .normalization=Q15(0x01C7),
   .nbPerms=NB_PERM72,
   .perms=perm72,
   .nbTwiddle=NB_TWIDDLE204,
   .twiddle=twiddle204,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors72,
   .lengths=lengths72,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config205={
   .normalization=Q15(0x00A4),
   .nbPerms=NB_PERM73,
   .perms=perm73,
   .nbTwiddle=NB_TWIDDLE205,
   .twiddle=twiddle205,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors73,
   .lengths=lengths73,
   .format=11,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config206={
   .normalization=Q15(0x01B5),
   .nbPerms=NB_PERM44,
   .perms=perm44,
   .nbTwiddle=NB_TWIDDLE206,
   .twiddle=twiddle206,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors44,
   .lengths=lengths44,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config207={
   .normalization=Q15(0x019A),
   .nbPerms=NB_PERM74,
   .perms=perm74,
   .nbTwiddle=NB_TWIDDLE207,
   .twiddle=twiddle207,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors74,
   .lengths=lengths74,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config208={
   .normalization=Q15(0x0195),
   .nbPerms=NB_PERM46,
   .perms=perm46,
   .nbTwiddle=NB_TWIDDLE208,
   .twiddle=twiddle208,
   .nbGroupedFactors=1,
   .nbFactors=4,
   .factors=factors46,
   .lengths=lengths46,
   .format=8,
   .reversalVectorizable=1
   };
cfftconfig<Q15> config209={
   .normalization=Q15(0x0098),
   .nbPerms=NB_PERM75,
   .perms=perm75,
   .nbTwiddle=NB_TWIDDLE209,
   .twiddle=twiddle209,
   .nbGroupedFactors=3,
   .nbFactors=5,
   .factors=factors75,
   .lengths=lengths75,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config210={
   .normalization=Q15(0x016C),
   .nbPerms=NB_PERM48,
   .perms=perm48,
   .nbTwiddle=NB_TWIDDLE210,
   .twiddle=twiddle210,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors48,
   .lengths=lengths48,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config211={
   .normalization=Q15(0x0155),
   .nbPerms=NB_PERM76,
   .perms=perm76,
   .nbTwiddle=NB_TWIDDLE211,
   .twiddle=twiddle211,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors76,
   .lengths=lengths76,
   .format=7,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config212={
   .normalization=Q15(0x0148),
   .nbPerms=NB_PERM50,
   .perms=perm50,
   .nbTwiddle=NB_TWIDDLE212,
   .twiddle=twiddle212,
   .nbGroupedFactors=2,
   .nbFactors=3,
   .factors=factors50,
   .lengths=lengths50,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config213={
   .normalization=Q15(0x012F),
   .nbPerms=NB_PERM51,
   .perms=perm51,
   .nbTwiddle=NB_TWIDDLE213,
   .twiddle=twiddle213,
   .nbGroupedFactors=2,
   .nbFactors=4,
   .factors=factors51,
   .lengths=lengths51,
   .format=8,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config214={
   .normalization=Q15(0x0089),
   .nbPerms=NB_PERM77,
   .perms=perm77,
   .nbTwiddle=NB_TWIDDLE214,
   .twiddle=twiddle214,
   .nbGroupedFactors=3,
   .nbFactors=4,
   .factors=factors77,
   .lengths=lengths77,
   .format=10,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config215={
   .normalization=Q15(0x0111),
   .nbPerms=NB_PERM78,
   .perms=perm78,
   .nbTwiddle=NB_TWIDDLE215,
   .twiddle=twiddle215,
   .nbGroupedFactors=4,
   .nbFactors=4,
   .factors=factors78,
   .lengths=lengths78,
   .format=9,
   .reversalVectorizable=0
   };
cfftconfig<Q15> config216={
   .normalization=Q15(0x0106),
   .nbPerms=NB_PERM54,
   .perms=perm54,
   .nbTwiddle=NB_TWIDDLE216,
   .twiddle=twiddle216,
   .nbGroupedFactors=1,
   .nbFactors=3,
   .factors=factors54,
   .lengths=lengths54,
   .format=12,
   .reversalVectorizable=1
   };

template<>
const cfftconfig<Q15> *cfft_config<Q15>(uint32_t nbSamples)
{
    switch(nbSamples)
    {
        case 2:
            return(&config165);
        break;
        case 3:
            return(&config166);
        break;
        case 4:
            return(&config167);
        break;
        case 5:
            return(&config168);
        break;
        case 6:
            return(&config169);
        break;
        case 8:
            return(&config171);
        break;
        case 9:
            return(&config172);
        break;
        case 10:
            return(&config173);
        break;
        case 12:
            return(&config175);
        break;
        case 15:
            return(&config177);
        break;
        case 16:
            return(&config178);
        break;
        case 18:
            return(&config180);
        break;
        case 20:
            return(&config182);
        break;
        case 24:
            return(&config184);
        break;
        case 25:
            return(&config185);
        break;
        case 27:
            return(&config186);
        break;
        case 30:
            return(&config187);
        break;
        case 32:
            return(&config188);
        break;
        case 36:
            return(&config192);
        break;
        case 40:
            return(&config193);
        break;
        case 45:
            return(&config194);
        break;
        case 48:
            return(&config195);
        break;
        case 50:
            return(&config196);
        break;
        case 54:
            return(&config198);
        break;
        case 60:
            return(&config199);
        break;
        case 64:
            return(&config200);
        break;
        case 72:
            return(&config204);
        break;
        case 75:
            return(&config206);
        break;
        case 80:
            return(&config207);
        break;
        case 81:
            return(&config208);
        break;
        case 90:
            return(&config210);
        break;
        case 96:
            return(&config211);
        break;
        case 100:
            return(&config212);
        break;
        case 108:
            return(&config213);
        break;
        case 120:
            return(&config215);
        break;
        case 125:
            return(&config216);
        break;
        case 128:
            return(&config174);
        break;
        case 144:
            return(&config179);
        break;
        case 150:
            return(&config183);
        break;
        case 160:
            return(&config191);
        break;
        case 180:
            return(&config197);
        break;
        case 192:
            return(&config203);
        break;
        case 200:
            return(&config205);
        break;
        case 216:
            return(&config209);
        break;
        case 240:
            return(&config214);
        break;
        case 256:
            return(&config163);
        break;
        case 288:
            return(&config189);
        break;
        case 320:
            return(&config201);
        break;
        case 384:
            return(&config176);
        break;
        case 400:
            return(&config181);
        break;
        case 512:
            return(&config170);
        break;
        case 1024:
            return(&config190);
        break;
        case 2048:
            return(&config202);
        break;
        case 4096:
            return(&config164);
        break;

    }
    return(NULL);
}

rfftconfig<float32_t> config217={
   .nbTwiddle=NB_TWIDDLE217,
   .twiddle=twiddle217
   };
rfftconfig<float32_t> config218={
   .nbTwiddle=NB_TWIDDLE218,
   .twiddle=twiddle218
   };
rfftconfig<float32_t> config219={
   .nbTwiddle=NB_TWIDDLE219,
   .twiddle=twiddle219
   };
rfftconfig<float32_t> config220={
   .nbTwiddle=NB_TWIDDLE220,
   .twiddle=twiddle220
   };
rfftconfig<float32_t> config221={
   .nbTwiddle=NB_TWIDDLE221,
   .twiddle=twiddle221
   };
rfftconfig<float32_t> config222={
   .nbTwiddle=NB_TWIDDLE222,
   .twiddle=twiddle222
   };
rfftconfig<float32_t> config223={
   .nbTwiddle=NB_TWIDDLE223,
   .twiddle=twiddle223
   };
rfftconfig<float32_t> config224={
   .nbTwiddle=NB_TWIDDLE224,
   .twiddle=twiddle224
   };
rfftconfig<float32_t> config225={
   .nbTwiddle=NB_TWIDDLE225,
   .twiddle=twiddle225
   };
rfftconfig<float32_t> config226={
   .nbTwiddle=NB_TWIDDLE226,
   .twiddle=twiddle226
   };
rfftconfig<float32_t> config227={
   .nbTwiddle=NB_TWIDDLE227,
   .twiddle=twiddle227
   };
rfftconfig<float32_t> config228={
   .nbTwiddle=NB_TWIDDLE228,
   .twiddle=twiddle228
   };
rfftconfig<float32_t> config229={
   .nbTwiddle=NB_TWIDDLE229,
   .twiddle=twiddle229
   };
rfftconfig<float32_t> config230={
   .nbTwiddle=NB_TWIDDLE230,
   .twiddle=twiddle230
   };
rfftconfig<float32_t> config231={
   .nbTwiddle=NB_TWIDDLE231,
   .twiddle=twiddle231
   };
rfftconfig<float32_t> config232={
   .nbTwiddle=NB_TWIDDLE232,
   .twiddle=twiddle232
   };
rfftconfig<float32_t> config233={
   .nbTwiddle=NB_TWIDDLE233,
   .twiddle=twiddle233
   };
rfftconfig<float32_t> config234={
   .nbTwiddle=NB_TWIDDLE234,
   .twiddle=twiddle234
   };
rfftconfig<float32_t> config235={
   .nbTwiddle=NB_TWIDDLE235,
   .twiddle=twiddle235
   };
rfftconfig<float32_t> config236={
   .nbTwiddle=NB_TWIDDLE236,
   .twiddle=twiddle236
   };
rfftconfig<float32_t> config237={
   .nbTwiddle=NB_TWIDDLE237,
   .twiddle=twiddle237
   };
rfftconfig<float32_t> config238={
   .nbTwiddle=NB_TWIDDLE238,
   .twiddle=twiddle238
   };
rfftconfig<float32_t> config239={
   .nbTwiddle=NB_TWIDDLE239,
   .twiddle=twiddle239
   };
rfftconfig<float32_t> config240={
   .nbTwiddle=NB_TWIDDLE240,
   .twiddle=twiddle240
   };
rfftconfig<float32_t> config241={
   .nbTwiddle=NB_TWIDDLE241,
   .twiddle=twiddle241
   };
rfftconfig<float32_t> config242={
   .nbTwiddle=NB_TWIDDLE242,
   .twiddle=twiddle242
   };
rfftconfig<float32_t> config243={
   .nbTwiddle=NB_TWIDDLE243,
   .twiddle=twiddle243
   };
rfftconfig<float32_t> config244={
   .nbTwiddle=NB_TWIDDLE244,
   .twiddle=twiddle244
   };
rfftconfig<float32_t> config245={
   .nbTwiddle=NB_TWIDDLE245,
   .twiddle=twiddle245
   };
rfftconfig<float32_t> config246={
   .nbTwiddle=NB_TWIDDLE246,
   .twiddle=twiddle246
   };
rfftconfig<float32_t> config247={
   .nbTwiddle=NB_TWIDDLE247,
   .twiddle=twiddle247
   };
rfftconfig<float32_t> config248={
   .nbTwiddle=NB_TWIDDLE248,
   .twiddle=twiddle248
   };
rfftconfig<float32_t> config249={
   .nbTwiddle=NB_TWIDDLE249,
   .twiddle=twiddle249
   };
rfftconfig<float32_t> config250={
   .nbTwiddle=NB_TWIDDLE250,
   .twiddle=twiddle250
   };
rfftconfig<float32_t> config251={
   .nbTwiddle=NB_TWIDDLE251,
   .twiddle=twiddle251
   };
rfftconfig<float32_t> config252={
   .nbTwiddle=NB_TWIDDLE252,
   .twiddle=twiddle252
   };
rfftconfig<float32_t> config253={
   .nbTwiddle=NB_TWIDDLE253,
   .twiddle=twiddle253
   };
rfftconfig<float32_t> config254={
   .nbTwiddle=NB_TWIDDLE254,
   .twiddle=twiddle254
   };
rfftconfig<float32_t> config255={
   .nbTwiddle=NB_TWIDDLE255,
   .twiddle=twiddle255
   };
rfftconfig<float32_t> config256={
   .nbTwiddle=NB_TWIDDLE256,
   .twiddle=twiddle256
   };

template<>
const rfftconfig<float32_t> *rfft_config<float32_t>(uint32_t nbSamples)
{
    switch(nbSamples)
    {
        case 4:
            return(&config217);
        break;
        case 6:
            return(&config218);
        break;
        case 8:
            return(&config219);
        break;
        case 10:
            return(&config220);
        break;
        case 12:
            return(&config221);
        break;
        case 16:
            return(&config222);
        break;
        case 18:
            return(&config223);
        break;
        case 20:
            return(&config224);
        break;
        case 24:
            return(&config225);
        break;
        case 30:
            return(&config226);
        break;
        case 32:
            return(&config227);
        break;
        case 36:
            return(&config228);
        break;
        case 40:
            return(&config229);
        break;
        case 48:
            return(&config230);
        break;
        case 50:
            return(&config231);
        break;
        case 54:
            return(&config232);
        break;
        case 60:
            return(&config233);
        break;
        case 64:
            return(&config234);
        break;
        case 72:
            return(&config235);
        break;
        case 80:
            return(&config236);
        break;
        case 90:
            return(&config237);
        break;
        case 96:
            return(&config238);
        break;
        case 100:
            return(&config239);
        break;
        case 108:
            return(&config240);
        break;
        case 120:
            return(&config241);
        break;
        case 128:
            return(&config242);
        break;
        case 144:
            return(&config243);
        break;
        case 150:
            return(&config244);
        break;
        case 160:
            return(&config245);
        break;
        case 180:
            return(&config246);
        break;
        case 192:
            return(&config247);
        break;
        case 200:
            return(&config248);
        break;
        case 216:
            return(&config249);
        break;
        case 240:
            return(&config250);
        break;
        case 256:
            return(&config251);
        break;
        case 288:
            return(&config252);
        break;
        case 320:
            return(&config253);
        break;
        case 384:
            return(&config254);
        break;
        case 512:
            return(&config255);
        break;
        case 1024:
            return(&config256);
        break;

    }
    return(NULL);
}

#if defined(ARM_FLOAT16_SUPPORTED)
rfftconfig<float16_t> config257={
   .nbTwiddle=NB_TWIDDLE257,
   .twiddle=twiddle257
   };
rfftconfig<float16_t> config258={
   .nbTwiddle=NB_TWIDDLE258,
   .twiddle=twiddle258
   };
rfftconfig<float16_t> config259={
   .nbTwiddle=NB_TWIDDLE259,
   .twiddle=twiddle259
   };
rfftconfig<float16_t> config260={
   .nbTwiddle=NB_TWIDDLE260,
   .twiddle=twiddle260
   };
rfftconfig<float16_t> config261={
   .nbTwiddle=NB_TWIDDLE261,
   .twiddle=twiddle261
   };
rfftconfig<float16_t> config262={
   .nbTwiddle=NB_TWIDDLE262,
   .twiddle=twiddle262
   };
rfftconfig<float16_t> config263={
   .nbTwiddle=NB_TWIDDLE263,
   .twiddle=twiddle263
   };
rfftconfig<float16_t> config264={
   .nbTwiddle=NB_TWIDDLE264,
   .twiddle=twiddle264
   };
rfftconfig<float16_t> config265={
   .nbTwiddle=NB_TWIDDLE265,
   .twiddle=twiddle265
   };
rfftconfig<float16_t> config266={
   .nbTwiddle=NB_TWIDDLE266,
   .twiddle=twiddle266
   };
rfftconfig<float16_t> config267={
   .nbTwiddle=NB_TWIDDLE267,
   .twiddle=twiddle267
   };
rfftconfig<float16_t> config268={
   .nbTwiddle=NB_TWIDDLE268,
   .twiddle=twiddle268
   };
rfftconfig<float16_t> config269={
   .nbTwiddle=NB_TWIDDLE269,
   .twiddle=twiddle269
   };
rfftconfig<float16_t> config270={
   .nbTwiddle=NB_TWIDDLE270,
   .twiddle=twiddle270
   };
rfftconfig<float16_t> config271={
   .nbTwiddle=NB_TWIDDLE271,
   .twiddle=twiddle271
   };
rfftconfig<float16_t> config272={
   .nbTwiddle=NB_TWIDDLE272,
   .twiddle=twiddle272
   };
rfftconfig<float16_t> config273={
   .nbTwiddle=NB_TWIDDLE273,
   .twiddle=twiddle273
   };
rfftconfig<float16_t> config274={
   .nbTwiddle=NB_TWIDDLE274,
   .twiddle=twiddle274
   };
rfftconfig<float16_t> config275={
   .nbTwiddle=NB_TWIDDLE275,
   .twiddle=twiddle275
   };
rfftconfig<float16_t> config276={
   .nbTwiddle=NB_TWIDDLE276,
   .twiddle=twiddle276
   };
rfftconfig<float16_t> config277={
   .nbTwiddle=NB_TWIDDLE277,
   .twiddle=twiddle277
   };
rfftconfig<float16_t> config278={
   .nbTwiddle=NB_TWIDDLE278,
   .twiddle=twiddle278
   };
rfftconfig<float16_t> config279={
   .nbTwiddle=NB_TWIDDLE279,
   .twiddle=twiddle279
   };
rfftconfig<float16_t> config280={
   .nbTwiddle=NB_TWIDDLE280,
   .twiddle=twiddle280
   };
rfftconfig<float16_t> config281={
   .nbTwiddle=NB_TWIDDLE281,
   .twiddle=twiddle281
   };
rfftconfig<float16_t> config282={
   .nbTwiddle=NB_TWIDDLE282,
   .twiddle=twiddle282
   };
rfftconfig<float16_t> config283={
   .nbTwiddle=NB_TWIDDLE283,
   .twiddle=twiddle283
   };
rfftconfig<float16_t> config284={
   .nbTwiddle=NB_TWIDDLE284,
   .twiddle=twiddle284
   };
rfftconfig<float16_t> config285={
   .nbTwiddle=NB_TWIDDLE285,
   .twiddle=twiddle285
   };
rfftconfig<float16_t> config286={
   .nbTwiddle=NB_TWIDDLE286,
   .twiddle=twiddle286
   };
rfftconfig<float16_t> config287={
   .nbTwiddle=NB_TWIDDLE287,
   .twiddle=twiddle287
   };
rfftconfig<float16_t> config288={
   .nbTwiddle=NB_TWIDDLE288,
   .twiddle=twiddle288
   };
rfftconfig<float16_t> config289={
   .nbTwiddle=NB_TWIDDLE289,
   .twiddle=twiddle289
   };
rfftconfig<float16_t> config290={
   .nbTwiddle=NB_TWIDDLE290,
   .twiddle=twiddle290
   };
rfftconfig<float16_t> config291={
   .nbTwiddle=NB_TWIDDLE291,
   .twiddle=twiddle291
   };
rfftconfig<float16_t> config292={
   .nbTwiddle=NB_TWIDDLE292,
   .twiddle=twiddle292
   };
rfftconfig<float16_t> config293={
   .nbTwiddle=NB_TWIDDLE293,
   .twiddle=twiddle293
   };
rfftconfig<float16_t> config294={
   .nbTwiddle=NB_TWIDDLE294,
   .twiddle=twiddle294
   };
rfftconfig<float16_t> config295={
   .nbTwiddle=NB_TWIDDLE295,
   .twiddle=twiddle295
   };
rfftconfig<float16_t> config296={
   .nbTwiddle=NB_TWIDDLE296,
   .twiddle=twiddle296
   };

template<>
const rfftconfig<float16_t> *rfft_config<float16_t>(uint32_t nbSamples)
{
    switch(nbSamples)
    {
        case 4:
            return(&config257);
        break;
        case 6:
            return(&config258);
        break;
        case 8:
            return(&config259);
        break;
        case 10:
            return(&config260);
        break;
        case 12:
            return(&config261);
        break;
        case 16:
            return(&config262);
        break;
        case 18:
            return(&config263);
        break;
        case 20:
            return(&config264);
        break;
        case 24:
            return(&config265);
        break;
        case 30:
            return(&config266);
        break;
        case 32:
            return(&config267);
        break;
        case 36:
            return(&config268);
        break;
        case 40:
            return(&config269);
        break;
        case 48:
            return(&config270);
        break;
        case 50:
            return(&config271);
        break;
        case 54:
            return(&config272);
        break;
        case 60:
            return(&config273);
        break;
        case 64:
            return(&config274);
        break;
        case 72:
            return(&config275);
        break;
        case 80:
            return(&config276);
        break;
        case 90:
            return(&config277);
        break;
        case 96:
            return(&config278);
        break;
        case 100:
            return(&config279);
        break;
        case 108:
            return(&config280);
        break;
        case 120:
            return(&config281);
        break;
        case 128:
            return(&config282);
        break;
        case 144:
            return(&config283);
        break;
        case 150:
            return(&config284);
        break;
        case 160:
            return(&config285);
        break;
        case 180:
            return(&config286);
        break;
        case 192:
            return(&config287);
        break;
        case 200:
            return(&config288);
        break;
        case 216:
            return(&config289);
        break;
        case 240:
            return(&config290);
        break;
        case 256:
            return(&config291);
        break;
        case 288:
            return(&config292);
        break;
        case 320:
            return(&config293);
        break;
        case 384:
            return(&config294);
        break;
        case 512:
            return(&config295);
        break;
        case 1024:
            return(&config296);
        break;

    }
    return(NULL);
}

#endif

rfftconfig<Q31> config297={
   .nbTwiddle=NB_TWIDDLE297,
   .twiddle=twiddle297
   };
rfftconfig<Q31> config298={
   .nbTwiddle=NB_TWIDDLE298,
   .twiddle=twiddle298
   };
rfftconfig<Q31> config299={
   .nbTwiddle=NB_TWIDDLE299,
   .twiddle=twiddle299
   };
rfftconfig<Q31> config300={
   .nbTwiddle=NB_TWIDDLE300,
   .twiddle=twiddle300
   };
rfftconfig<Q31> config301={
   .nbTwiddle=NB_TWIDDLE301,
   .twiddle=twiddle301
   };
rfftconfig<Q31> config302={
   .nbTwiddle=NB_TWIDDLE302,
   .twiddle=twiddle302
   };
rfftconfig<Q31> config303={
   .nbTwiddle=NB_TWIDDLE303,
   .twiddle=twiddle303
   };
rfftconfig<Q31> config304={
   .nbTwiddle=NB_TWIDDLE304,
   .twiddle=twiddle304
   };
rfftconfig<Q31> config305={
   .nbTwiddle=NB_TWIDDLE305,
   .twiddle=twiddle305
   };
rfftconfig<Q31> config306={
   .nbTwiddle=NB_TWIDDLE306,
   .twiddle=twiddle306
   };
rfftconfig<Q31> config307={
   .nbTwiddle=NB_TWIDDLE307,
   .twiddle=twiddle307
   };
rfftconfig<Q31> config308={
   .nbTwiddle=NB_TWIDDLE308,
   .twiddle=twiddle308
   };
rfftconfig<Q31> config309={
   .nbTwiddle=NB_TWIDDLE309,
   .twiddle=twiddle309
   };
rfftconfig<Q31> config310={
   .nbTwiddle=NB_TWIDDLE310,
   .twiddle=twiddle310
   };
rfftconfig<Q31> config311={
   .nbTwiddle=NB_TWIDDLE311,
   .twiddle=twiddle311
   };
rfftconfig<Q31> config312={
   .nbTwiddle=NB_TWIDDLE312,
   .twiddle=twiddle312
   };
rfftconfig<Q31> config313={
   .nbTwiddle=NB_TWIDDLE313,
   .twiddle=twiddle313
   };
rfftconfig<Q31> config314={
   .nbTwiddle=NB_TWIDDLE314,
   .twiddle=twiddle314
   };
rfftconfig<Q31> config315={
   .nbTwiddle=NB_TWIDDLE315,
   .twiddle=twiddle315
   };
rfftconfig<Q31> config316={
   .nbTwiddle=NB_TWIDDLE316,
   .twiddle=twiddle316
   };
rfftconfig<Q31> config317={
   .nbTwiddle=NB_TWIDDLE317,
   .twiddle=twiddle317
   };
rfftconfig<Q31> config318={
   .nbTwiddle=NB_TWIDDLE318,
   .twiddle=twiddle318
   };
rfftconfig<Q31> config319={
   .nbTwiddle=NB_TWIDDLE319,
   .twiddle=twiddle319
   };
rfftconfig<Q31> config320={
   .nbTwiddle=NB_TWIDDLE320,
   .twiddle=twiddle320
   };
rfftconfig<Q31> config321={
   .nbTwiddle=NB_TWIDDLE321,
   .twiddle=twiddle321
   };
rfftconfig<Q31> config322={
   .nbTwiddle=NB_TWIDDLE322,
   .twiddle=twiddle322
   };
rfftconfig<Q31> config323={
   .nbTwiddle=NB_TWIDDLE323,
   .twiddle=twiddle323
   };
rfftconfig<Q31> config324={
   .nbTwiddle=NB_TWIDDLE324,
   .twiddle=twiddle324
   };
rfftconfig<Q31> config325={
   .nbTwiddle=NB_TWIDDLE325,
   .twiddle=twiddle325
   };
rfftconfig<Q31> config326={
   .nbTwiddle=NB_TWIDDLE326,
   .twiddle=twiddle326
   };
rfftconfig<Q31> config327={
   .nbTwiddle=NB_TWIDDLE327,
   .twiddle=twiddle327
   };
rfftconfig<Q31> config328={
   .nbTwiddle=NB_TWIDDLE328,
   .twiddle=twiddle328
   };
rfftconfig<Q31> config329={
   .nbTwiddle=NB_TWIDDLE329,
   .twiddle=twiddle329
   };
rfftconfig<Q31> config330={
   .nbTwiddle=NB_TWIDDLE330,
   .twiddle=twiddle330
   };
rfftconfig<Q31> config331={
   .nbTwiddle=NB_TWIDDLE331,
   .twiddle=twiddle331
   };
rfftconfig<Q31> config332={
   .nbTwiddle=NB_TWIDDLE332,
   .twiddle=twiddle332
   };
rfftconfig<Q31> config333={
   .nbTwiddle=NB_TWIDDLE333,
   .twiddle=twiddle333
   };
rfftconfig<Q31> config334={
   .nbTwiddle=NB_TWIDDLE334,
   .twiddle=twiddle334
   };
rfftconfig<Q31> config335={
   .nbTwiddle=NB_TWIDDLE335,
   .twiddle=twiddle335
   };
rfftconfig<Q31> config336={
   .nbTwiddle=NB_TWIDDLE336,
   .twiddle=twiddle336
   };

template<>
const rfftconfig<Q31> *rfft_config<Q31>(uint32_t nbSamples)
{
    switch(nbSamples)
    {
        case 4:
            return(&config297);
        break;
        case 6:
            return(&config298);
        break;
        case 8:
            return(&config299);
        break;
        case 10:
            return(&config300);
        break;
        case 12:
            return(&config301);
        break;
        case 16:
            return(&config302);
        break;
        case 18:
            return(&config303);
        break;
        case 20:
            return(&config304);
        break;
        case 24:
            return(&config305);
        break;
        case 30:
            return(&config306);
        break;
        case 32:
            return(&config307);
        break;
        case 36:
            return(&config308);
        break;
        case 40:
            return(&config309);
        break;
        case 48:
            return(&config310);
        break;
        case 50:
            return(&config311);
        break;
        case 54:
            return(&config312);
        break;
        case 60:
            return(&config313);
        break;
        case 64:
            return(&config314);
        break;
        case 72:
            return(&config315);
        break;
        case 80:
            return(&config316);
        break;
        case 90:
            return(&config317);
        break;
        case 96:
            return(&config318);
        break;
        case 100:
            return(&config319);
        break;
        case 108:
            return(&config320);
        break;
        case 120:
            return(&config321);
        break;
        case 128:
            return(&config322);
        break;
        case 144:
            return(&config323);
        break;
        case 150:
            return(&config324);
        break;
        case 160:
            return(&config325);
        break;
        case 180:
            return(&config326);
        break;
        case 192:
            return(&config327);
        break;
        case 200:
            return(&config328);
        break;
        case 216:
            return(&config329);
        break;
        case 240:
            return(&config330);
        break;
        case 256:
            return(&config331);
        break;
        case 288:
            return(&config332);
        break;
        case 320:
            return(&config333);
        break;
        case 384:
            return(&config334);
        break;
        case 512:
            return(&config335);
        break;
        case 1024:
            return(&config336);
        break;

    }
    return(NULL);
}

rfftconfig<Q15> config337={
   .nbTwiddle=NB_TWIDDLE337,
   .twiddle=twiddle337
   };
rfftconfig<Q15> config338={
   .nbTwiddle=NB_TWIDDLE338,
   .twiddle=twiddle338
   };
rfftconfig<Q15> config339={
   .nbTwiddle=NB_TWIDDLE339,
   .twiddle=twiddle339
   };
rfftconfig<Q15> config340={
   .nbTwiddle=NB_TWIDDLE340,
   .twiddle=twiddle340
   };
rfftconfig<Q15> config341={
   .nbTwiddle=NB_TWIDDLE341,
   .twiddle=twiddle341
   };
rfftconfig<Q15> config342={
   .nbTwiddle=NB_TWIDDLE342,
   .twiddle=twiddle342
   };
rfftconfig<Q15> config343={
   .nbTwiddle=NB_TWIDDLE343,
   .twiddle=twiddle343
   };
rfftconfig<Q15> config344={
   .nbTwiddle=NB_TWIDDLE344,
   .twiddle=twiddle344
   };
rfftconfig<Q15> config345={
   .nbTwiddle=NB_TWIDDLE345,
   .twiddle=twiddle345
   };
rfftconfig<Q15> config346={
   .nbTwiddle=NB_TWIDDLE346,
   .twiddle=twiddle346
   };
rfftconfig<Q15> config347={
   .nbTwiddle=NB_TWIDDLE347,
   .twiddle=twiddle347
   };
rfftconfig<Q15> config348={
   .nbTwiddle=NB_TWIDDLE348,
   .twiddle=twiddle348
   };
rfftconfig<Q15> config349={
   .nbTwiddle=NB_TWIDDLE349,
   .twiddle=twiddle349
   };
rfftconfig<Q15> config350={
   .nbTwiddle=NB_TWIDDLE350,
   .twiddle=twiddle350
   };
rfftconfig<Q15> config351={
   .nbTwiddle=NB_TWIDDLE351,
   .twiddle=twiddle351
   };
rfftconfig<Q15> config352={
   .nbTwiddle=NB_TWIDDLE352,
   .twiddle=twiddle352
   };
rfftconfig<Q15> config353={
   .nbTwiddle=NB_TWIDDLE353,
   .twiddle=twiddle353
   };
rfftconfig<Q15> config354={
   .nbTwiddle=NB_TWIDDLE354,
   .twiddle=twiddle354
   };
rfftconfig<Q15> config355={
   .nbTwiddle=NB_TWIDDLE355,
   .twiddle=twiddle355
   };
rfftconfig<Q15> config356={
   .nbTwiddle=NB_TWIDDLE356,
   .twiddle=twiddle356
   };
rfftconfig<Q15> config357={
   .nbTwiddle=NB_TWIDDLE357,
   .twiddle=twiddle357
   };
rfftconfig<Q15> config358={
   .nbTwiddle=NB_TWIDDLE358,
   .twiddle=twiddle358
   };
rfftconfig<Q15> config359={
   .nbTwiddle=NB_TWIDDLE359,
   .twiddle=twiddle359
   };
rfftconfig<Q15> config360={
   .nbTwiddle=NB_TWIDDLE360,
   .twiddle=twiddle360
   };
rfftconfig<Q15> config361={
   .nbTwiddle=NB_TWIDDLE361,
   .twiddle=twiddle361
   };
rfftconfig<Q15> config362={
   .nbTwiddle=NB_TWIDDLE362,
   .twiddle=twiddle362
   };
rfftconfig<Q15> config363={
   .nbTwiddle=NB_TWIDDLE363,
   .twiddle=twiddle363
   };
rfftconfig<Q15> config364={
   .nbTwiddle=NB_TWIDDLE364,
   .twiddle=twiddle364
   };
rfftconfig<Q15> config365={
   .nbTwiddle=NB_TWIDDLE365,
   .twiddle=twiddle365
   };
rfftconfig<Q15> config366={
   .nbTwiddle=NB_TWIDDLE366,
   .twiddle=twiddle366
   };
rfftconfig<Q15> config367={
   .nbTwiddle=NB_TWIDDLE367,
   .twiddle=twiddle367
   };
rfftconfig<Q15> config368={
   .nbTwiddle=NB_TWIDDLE368,
   .twiddle=twiddle368
   };
rfftconfig<Q15> config369={
   .nbTwiddle=NB_TWIDDLE369,
   .twiddle=twiddle369
   };
rfftconfig<Q15> config370={
   .nbTwiddle=NB_TWIDDLE370,
   .twiddle=twiddle370
   };
rfftconfig<Q15> config371={
   .nbTwiddle=NB_TWIDDLE371,
   .twiddle=twiddle371
   };
rfftconfig<Q15> config372={
   .nbTwiddle=NB_TWIDDLE372,
   .twiddle=twiddle372
   };
rfftconfig<Q15> config373={
   .nbTwiddle=NB_TWIDDLE373,
   .twiddle=twiddle373
   };
rfftconfig<Q15> config374={
   .nbTwiddle=NB_TWIDDLE374,
   .twiddle=twiddle374
   };
rfftconfig<Q15> config375={
   .nbTwiddle=NB_TWIDDLE375,
   .twiddle=twiddle375
   };
rfftconfig<Q15> config376={
   .nbTwiddle=NB_TWIDDLE376,
   .twiddle=twiddle376
   };

template<>
const rfftconfig<Q15> *rfft_config<Q15>(uint32_t nbSamples)
{
    switch(nbSamples)
    {
        case 4:
            return(&config337);
        break;
        case 6:
            return(&config338);
        break;
        case 8:
            return(&config339);
        break;
        case 10:
            return(&config340);
        break;
        case 12:
            return(&config341);
        break;
        case 16:
            return(&config342);
        break;
        case 18:
            return(&config343);
        break;
        case 20:
            return(&config344);
        break;
        case 24:
            return(&config345);
        break;
        case 30:
            return(&config346);
        break;
        case 32:
            return(&config347);
        break;
        case 36:
            return(&config348);
        break;
        case 40:
            return(&config349);
        break;
        case 48:
            return(&config350);
        break;
        case 50:
            return(&config351);
        break;
        case 54:
            return(&config352);
        break;
        case 60:
            return(&config353);
        break;
        case 64:
            return(&config354);
        break;
        case 72:
            return(&config355);
        break;
        case 80:
            return(&config356);
        break;
        case 90:
            return(&config357);
        break;
        case 96:
            return(&config358);
        break;
        case 100:
            return(&config359);
        break;
        case 108:
            return(&config360);
        break;
        case 120:
            return(&config361);
        break;
        case 128:
            return(&config362);
        break;
        case 144:
            return(&config363);
        break;
        case 150:
            return(&config364);
        break;
        case 160:
            return(&config365);
        break;
        case 180:
            return(&config366);
        break;
        case 192:
            return(&config367);
        break;
        case 200:
            return(&config368);
        break;
        case 216:
            return(&config369);
        break;
        case 240:
            return(&config370);
        break;
        case 256:
            return(&config371);
        break;
        case 288:
            return(&config372);
        break;
        case 320:
            return(&config373);
        break;
        case 384:
            return(&config374);
        break;
        case 512:
            return(&config375);
        break;
        case 1024:
            return(&config376);
        break;

    }
    return(NULL);
}


