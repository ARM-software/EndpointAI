**Optimized trigonometry and logarithm functions in C**
| Function                           | Cycles | Comments                                                     |
| :--------------------------------- | :--------------------------- | :----------------------------------------------------------- |
| sine (sin(x))                      | 25                           | no constraint on argument                                   |
| cosine (cos(x))                    | 25                           | no constraint on argument                                   |
| sine (sinPi(x))                    | 17                           | argument in the range -pi .. +pi                             |
| cosine (cosPi(X))                  | 17                           | argument in the range -pi .. +pi                             |
| arc tangent (atan(x))              | 41                           | errors measured in {-50 .. +50}                              |
| hyperbolic tangent (tanh(x))       | 41                           | ||x||<1, errors measured in {-0.8 .. +0.8}                            |
| arc tangent hyperbolic (atanh(x))  | 18                           | errors measured in {-0.8 .. +0.8}                            |
| exponential (exp(x))               | 41                           | ||x||<1, error measured in {-[2 .. +2] |
| inverse of square-root (1/sqrt(x)) | 22                           | ASM, no coefficient table                                    |
| logarithm (log(x))                 | 38                           | x<1, errors measured in {0.1[ .. ]5.1} |
| cartesian arc tangent (atan2(x,y)) | 61                           | errors measured in {-50 .. +50}                              |
|                                    |                              |                                                              |

The code is compiled with arm llvm-v18.1.3, optimization -o1. The cycle-count is measured on a Cortex-M85 development board (FPB-RA8E1). In practice the user will find lower numbers because the subroutines are very small. When inlined with the other parts of the application codes, the compiler will interleave other instructions to remove any pipeline stalls. Additionally when a program calls two operations like sin(x) and cos(x) there are common constants and table pointer initializations which will be merged. Conclusion, the numbers are conservative.

The errors are computed from a Matlab reference. From the below table, the computation of sin(x) with a table of 100 points is accurate on 14.3bits, and you need 800 points to reach 20.3bits accuracy.

```
sin     table  100 error=1.01e-04 peak=2.9e-04 bias=-3.2e-07  (14.3 signed bits)
cos     table  100 error=1.12e-04 peak=2.9e-04 bias= 4.1e-06  (14.1 signed bits) 
sin     table  800 error=1.57e-06 peak=4.6e-06 bias= 1.9e-08  (20.3 signed bits) 
cos     table  800 error=1.76e-06 peak=4.6e-06 bias= 8.6e-08  (20.1 signed bits) 
sinPi   table  100 error=1.06e-04 peak=3.0e-04 bias= 1.0e-07  (14.2 signed bits) 
cosPi   table  100 error=1.07e-04 peak=2.9e-04 bias= 1.8e-06  (14.2 signed bits) 
sinPi   table  800 error=1.66e-06 peak=4.6e-06 bias= 1.5e-08  (20.2 signed bits) 
cosPi   table  800 error=1.67e-06 peak=4.6e-06 bias=-2.3e-08  (20.2 signed bits) 
atan    table  100 error=7.37e-06 peak=3.2e-05 bias=-5.0e-08  (18.1 signed bits) 
atan2   table  100 error=1.93e-05 peak=3.3e-05 bias= 3.1e-07  (16.7 signed bits) 
atan    table  800 error=1.31e-07 peak=6.5e-07 bias=-2.7e-09  (23.9 signed bits) 
atan2   table  800 error=3.19e-07 peak=7.7e-07 bias= 3.0e-09  (22.6 signed bits) 
atanh   table  100 error=5.93e-05 peak=3.1e-04 bias= 3.1e-08  (15.0 signed bits) 
tanh    table  100 error=2.19e-05 peak=3.9e-05 bias=-1.3e-10  (16.5 signed bits) 
atanh   table  800 error=9.27e-07 peak=4.9e-06 bias= 4.5e-10  (21.0 signed bits) 
tanh    table  800 error=3.43e-07 peak=7.1e-07 bias= 6.6e-11  (22.5 signed bits) 
invsqrt table    0 error=1.82e-04 peak=1.6e-02 bias=-1.5e-06  (13.4 signed bits) 
exp     table  100 error=1.70e-03 peak=1.1e-02 bias=-6.5e-04  (10.2 signed bits) 
exp     table  800 error=2.60e-05 peak=1.8e-04 bias=-9.8e-06  (16.2 signed bits) 
log     table  100 error=8.73e-05 peak=9.1e-04 bias=-3.4e-05  (14.5 signed bits) 
log     table  800 error=1.34e-06 peak=1.4e-05 bias=-5.4e-07  (20.5 signed bits)
```

