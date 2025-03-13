Optimized trigonometry and logarithm functions for Cortex-M85.

+-----------------------------------+-----------------------------------+
| Function prototypes               | Comments                          |
+===================================+===================================+
| float sin_l1_c(float x);          | sin(x) and cos(x) with any        |
|                                   | argument                          |
| float cos_l1_c(float x);          |                                   |
|                                   | \-                                |
| float sin_pi_c(float x);          |                                   |
|                                   | sin(x) and cos(x) in the -pi..+pi |
| float cos_pi_c(float x);          | range                             |
|                                   |                                   |
| float atan_l1_c(float x);         | \-                                |
|                                   |                                   |
| float atan2_l1_c(float y, float   | Arctangent                        |
| x);                               |                                   |
|                                   | Cartesian arctangent              |
| float tanh_l1_c(float x);         |                                   |
|                                   | Hyperbolic tangent                |
| float atanh_l1_c(float x);        |                                   |
|                                   | Hyperbolic arctangent             |
| float exp_l1_c(float x);          |                                   |
|                                   | Exponential                       |
| float invsqrt_l1_c(float x);      |                                   |
|                                   | Inverse of square root            |
| float log_l1_c(float x);          |                                   |
|                                   | Logarithm                         |
+-----------------------------------+-----------------------------------+
