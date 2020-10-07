#ifndef _MACROS_H
#define _MACROS_H

#define f_update() \
 NQ0 = Q0 - 0.5*Q1*WX*dt - 0.5*Q2*WY*dt - 0.5*Q3*WZ*dt; \
 NQ1 = 0.5*Q0*WX*dt + Q1 + 0.5*Q2*WZ*dt - 0.5*Q3*WY*dt; \
 NQ2 = 0.5*Q0*WY*dt - 0.5*Q1*WZ*dt + Q2 + 0.5*Q3*WX*dt; \
 NQ3 = 0.5*Q0*WZ*dt + 0.5*Q1*WY*dt - 0.5*Q2*WX*dt + Q3;

#define F_update() \
 M(F, 0, 0) = 1; \
 M(F, 0, 1) = -0.5*WX*dt; \
 M(F, 0, 2) = -0.5*WY*dt; \
 M(F, 0, 3) = -0.5*WZ*dt; \
 M(F, 1, 0) = 0.5*WX*dt; \
 M(F, 1, 1) = 1; \
 M(F, 1, 2) = 0.5*WZ*dt; \
 M(F, 1, 3) = -0.5*WY*dt; \
 M(F, 2, 0) = 0.5*WY*dt; \
 M(F, 2, 1) = -0.5*WZ*dt; \
 M(F, 2, 2) = 1; \
 M(F, 2, 3) = 0.5*WX*dt; \
 M(F, 3, 0) = 0.5*WZ*dt; \
 M(F, 3, 1) = 0.5*WY*dt; \
 M(F, 3, 2) = -0.5*WX*dt; \
 M(F, 3, 3) = 1;

#define FQ_update() \
 M(FQ, 0, 0) = -0.5*Q1*dt; \
 M(FQ, 0, 1) = -0.5*Q2*dt; \
 M(FQ, 0, 2) = -0.5*Q3*dt; \
 M(FQ, 1, 0) = 0.5*Q0*dt; \
 M(FQ, 1, 1) = -0.5*Q3*dt; \
 M(FQ, 1, 2) = 0.5*Q2*dt; \
 M(FQ, 2, 0) = 0.5*Q3*dt; \
 M(FQ, 2, 1) = 0.5*Q0*dt; \
 M(FQ, 2, 2) = -0.5*Q1*dt; \
 M(FQ, 3, 0) = -0.5*Q2*dt; \
 M(FQ, 3, 1) = 0.5*Q1*dt; \
 M(FQ, 3, 2) = 0.5*Q0*dt;

#define h_update() \
 predictedobs[0] = 19.620000000000001*NQ0*NQ2 - 19.620000000000001*NQ1*NQ3; \
 predictedobs[1] = -19.620000000000001*NQ0*NQ1 - 19.620000000000001*NQ2*NQ3; \
 predictedobs[2] = -9.8100000000000005*SQ(NQ0) + 9.8100000000000005*SQ(NQ1) + 9.8100000000000005*SQ(NQ2) - 9.8100000000000005*SQ(NQ3); \
 predictedobs[3] = -SQ(NQ0) - SQ(NQ1) + SQ(NQ2) + SQ(NQ3); \
 predictedobs[4] = 2*NQ0*NQ3 - 2*NQ1*NQ2; \
 predictedobs[5] = -2*NQ0*NQ2 - 2*NQ1*NQ3;

#define H_update() \
 M(H, 0, 0) = 19.620000000000001*NQ2; \
 M(H, 0, 1) = -19.620000000000001*NQ3; \
 M(H, 0, 2) = 19.620000000000001*NQ0; \
 M(H, 0, 3) = -19.620000000000001*NQ1; \
 M(H, 1, 0) = -19.620000000000001*NQ1; \
 M(H, 1, 1) = -19.620000000000001*NQ0; \
 M(H, 1, 2) = -19.620000000000001*NQ3; \
 M(H, 1, 3) = -19.620000000000001*NQ2; \
 M(H, 2, 0) = -19.620000000000001*NQ0; \
 M(H, 2, 1) = 19.620000000000001*NQ1; \
 M(H, 2, 2) = 19.620000000000001*NQ2; \
 M(H, 2, 3) = -19.620000000000001*NQ3; \
 M(H, 3, 0) = -2*NQ0; \
 M(H, 3, 1) = -2*NQ1; \
 M(H, 3, 2) = 2*NQ2; \
 M(H, 3, 3) = 2*NQ3; \
 M(H, 4, 0) = 2*NQ3; \
 M(H, 4, 1) = -2*NQ2; \
 M(H, 4, 2) = -2*NQ1; \
 M(H, 4, 3) = 2*NQ0; \
 M(H, 5, 0) = -2*NQ2; \
 M(H, 5, 1) = -2*NQ3; \
 M(H, 5, 2) = -2*NQ0; \
 M(H, 5, 3) = -2*NQ1;

#endif /* #ifndef _MACROS_H */
