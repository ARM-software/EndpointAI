/* ----------------------------------------------------------------------
*
* Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef COMPLEX_H
#define COMPLEX_H

/*

Complex type. The C++ standard complex type is not used
because fixed point arithmetic implies the return type of
the product is different.

*/

template<typename T>
struct complex {
    T re;  
    T im;

    constexpr complex(const T& re = T(), const T& im = T()):re(re), im(im) {};
   
    constexpr complex(const complex& c):re(c.re), im(c.im){}; 

    T real() const {return re;};
    T imag() const {return im;};
    
    __FORCE_INLINE complex& operator=(const complex& c){
         re = c.re ;
         im = c.im;

         return(*this);
    };

    __FORCE_INLINE complex const operator-() const {
        return complex(-re , -im);
    }

};


#endif


