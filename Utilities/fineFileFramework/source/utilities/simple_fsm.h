/****************************************************************************
*  Copyright 2017 Gorgon Meducer (Email:embedded_zhuoran@hotmail.com)       *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/
#ifndef __SIMPLE_FSM_H__
#define __SIMPLE_FSM_H__

/*============================ INCLUDES ======================================*/
#include <stdint.h>
#include ".\ooc.h"

/*============================ MACROS ========================================*/


#ifndef this
#   define this    (*ptThis)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
#define def_states(...)                                             \
        enum {                                                      \
            START = 0,                                              \
            __VA_ARGS__                                             \
        };

#define def_params(...)         __VA_ARGS__

#define args(...)             ,__VA_ARGS__

#define fsm(__NAME) fsm_##__NAME##_t

#define __simple_fsm(__FSM_TYPE, ...)                                           \
        def_class(__FSM_TYPE,,                                                  \
            uint_fast8_t chState;                                               \
            __VA_ARGS__                                                         \
        )                                                                       \
        end_def_class(__FSM_TYPE)

#define simple_fsm(__NAME, ...)                                                 \
        declare_simple_fsm(__NAME);                                             \
        __simple_fsm(fsm(__NAME), __VA_ARGS__)
        
#define def_simple_fsm(__NAME, ...)                                             \
        __simple_fsm(fsm(__NAME), __VA_ARGS__)
        
#define end_def_simple_fsm(...)

#define def_fsm(__NAME, ...)                                                    \
        __simple_fsm(fsm(__NAME), __VA_ARGS__)

#define end_def_fsm(...)

#define __extern_simple_fsm(__FSM_TYPE, ...)                                    \
        declare_class(__FSM_TYPE)                                               \
        extern_class(__FSM_TYPE,,                                               \
            uint_fast8_t chState;                                               \
            __VA_ARGS__                                                         \
        )                                                                       \
        end_extern_class(__FSM_TYPE)                                

#define __declare_simple_fsm(__FSM_TYPE)                                        \
        declare_class(__FSM_TYPE)
#define declare_simple_fsm(__NAME)  __declare_simple_fsm(fsm(__NAME))

#define declare_fsm(__NAME)  __declare_simple_fsm(fsm(__NAME))

#define extern_simple_fsm(__NAME, ...)                                          \
        __extern_simple_fsm(fsm(__NAME), __VA_ARGS__)  

/*! \brief extern fsm initialisation function and provide function prototye 
           as <__NAME>_fn, E.g
           extern_fsm_initialiser( demo_fsm );
           we extern a function called:
           extern fsm_demo_fsm_t *demo_fsm_init( fsm_demo_fsm_t *ptFSM );
           and a prototype definition:
           typedef fsm_demo_fsm_t *demo_fsm_init_fn( fsm_demo_fsm_t *ptFSM );
           We can then use demo_fsm_init_fn to define function pointer
 */
#define __extern_fsm_initialiser(__NAME, ...)                                   \
        fsm(__NAME) *__NAME##_init(fsm(__NAME) *ptFSM __VA_ARGS__);             \
        typedef fsm(__NAME) *__NAME##_init_fn(fsm(__NAME) *ptFSM __VA_ARGS__);  
        
#define extern_fsm_initialiser(__NAME, ...)                                     \
            __extern_fsm_initialiser(__NAME, __VA_ARGS__)
            
#define declare_fsm_initialiser(__NAME, ...)                                    \
            __extern_fsm_initialiser(__NAME, __VA_ARGS__)

/*! \brief extern fsm task function and provide function prototye as <__NAME>_fn, E.g
           extern_fsm_implementation( demo_fsm );
           we extern a function called:
           fsm_rt_t demo_fsm( fsm_demo_fsm_t *ptFSM )
           and a prototype definition:
           typedef fsm_rt_t demo_fsm_fn (fsm_demo_fsm_t *ptFSM);
           We can then use demo_fsm_fn to define function pointer
 */
 //! @{
#define __extern_fsm_implementation_ex(__NAME,__TYPE, ...)                      \
        fsm_rt_t __NAME( __TYPE *ptFSM __VA_ARGS__ );                           \
        typedef fsm_rt_t __NAME##_fn( __TYPE *ptFSM __VA_ARGS__ );              
        
#define declare_fsm_implementation_ex(__NAME, __TYPE, ...)                      \
            __extern_fsm_implementation_ex(__NAME, __TYPE, __VA_ARGS__)

#define extern_fsm_implementation_ex(__NAME,__TYPE, ...)                        \
            __extern_fsm_implementation_ex(__NAME, __TYPE, __VA_ARGS__)

#define extern_fsm_implementation(__NAME, ...)                                  \
            __extern_fsm_implementation_ex(__NAME, fsm(__NAME), __VA_ARGS__)
            
#define declare_fsm_implementation(__NAME, ...)                                 \
            __extern_fsm_implementation_ex(__NAME, fsm(__NAME), __VA_ARGS__)
//! @}

#define call_fsm(__NAME, __FSM, ...)                                            \
        __NAME((__FSM) __VA_ARGS__)

#define call_simple_fsm(__NAME, __FSM, ...)                                     \
        __NAME((__FSM) __VA_ARGS__)

#define __state(__STATE, ...)                                                   \
            case __STATE:                                                       \
        __state_entry_##__STATE:{                                               \
                __VA_ARGS__;                                                    \
            };
            
#define state(__STATE, ...)                 break; __state(__STATE, __VA_ARGS__) 

#define on_start(...)                       {__VA_ARGS__;}


#define reset_fsm()         do { ptThis->chState = 0; } while(0);
#define fsm_cpl()           do {reset_fsm(); return fsm_rt_cpl;} while(0);
#define fsm_report(__ERROR) do {reset_fsm(); return (fsm_rt_t)(__ERROR); } while(0);
#define fsm_wait_for_obj()  return fsm_rt_wait_for_obj;
#define fsm_on_going()      return fsm_rt_on_going;

//! fsm_continue is deprecated, should not be used anymore
//#define fsm_continue()      break


#define update_state_to(__STATE)                                                \
        { ptThis->chState = (__STATE); goto __state_entry_##__STATE;}

#define transfer_to(__STATE)                                                    \
         { ptThis->chState = (__STATE); fsm_on_going() } 


#define __fsm_initialiser(__NAME, ...)                                          \
        fsm(__NAME) *__NAME##_init(fsm(__NAME) *ptFSM __VA_ARGS__)              \
        {                                                                       \
            class(fsm(__NAME)) *ptThis = ( class(fsm(__NAME)) *)ptFSM;          \
            if (NULL == ptThis) {                                               \
                return NULL;                                                    \
            }                                                                   \
            ptThis->chState = 0;
            
#define fsm_initialiser(__NAME, ...)                                            \
            __fsm_initialiser(__NAME, __VA_ARGS__)
            
            
#define abort_init()     return NULL;

#define init_body(...)                                                          \
            __VA_ARGS__                                                         \
            return ptFSM;                                                       \
        }
            

#define init_fsm(__NAME, __FSM, ...)                                            \
        __NAME##_init((__FSM) __VA_ARGS__)


#define init_simple_fsm(__NAME, __FSM, ...)                                     \
        init_fsm((__FSM) __VA_ARGS__)
        

#define __implement_fsm_ex(__NAME, __TYPE, ...)                                 \
    fsm_rt_t __NAME( __TYPE *ptFSM __VA_ARGS__ )                                \
    {                                                                           \
        class(__TYPE) *ptThis = (class(__TYPE) *)ptFSM;                         \
        if (NULL == ptThis) {                                                   \
            return fsm_rt_err;                                                  \
        }                          

#define __body(...)                                                             \
        switch (ptThis->chState) {                                              \
            case 0:                                                             \
                ptThis->chState++;                                              \
            __VA_ARGS__                                                         \
            break;                                                              \
            default:                                                            \
            return fsm_rt_err;                                                  \
        }                                                                       \
                                                                                \
        return fsm_rt_on_going;                                                 \
    }

#define body(...)               __body(__VA_ARGS__)

#define body_begin()                                                            \
            switch (ptThis->chState) {                                          \
                case 0:                                                         \
                    ptThis->chState++;                                              

#define body_end()                                                              \
                break;                                                          \
                default:                                                        \
                return fsm_rt_err;                                              \
            }                                                                   \
        }                                                                       \
        return fsm_rt_on_going;  
        
/*! \note Debug Support: You can use debug_body() together with debug_state()
 *!       to enable debug specified state. Which means you are ale to set break
 *!       points and single-step into those states. 
 *! 
 *! \note debug_state() must be used in debug_body().
 *!
 *!       Example:

        debug_body()
            state(xxxxx,
                //the range no debug is allowed
            )
            
            state(xxxxx){
                //the range you can debug with
            }
            
        end_debug_body()
*/        
        
        
#define implement_fsm_ex(__NAME, __TYPE, ...)                                   \
            __implement_fsm_ex(__NAME, __TYPE, __VA_ARGS__)

#define fsm_implementation(__NAME, ...)                                         \
        implement_fsm(__NAME, __VA_ARGS__)

#define fsm_implementation_ex(__NAME, __TYPE, ...)                              \
        implement_fsm_ex(__NAME, __TYPE, __VA_ARGS__)
        
#define __implement_fsm(__NAME, ...)                                            \
            implement_fsm_ex(__NAME, fsm(__NAME), __VA_ARGS__)
                                                               
#define implement_fsm(__NAME, ...)      __implement_fsm(__NAME, __VA_ARGS__)


        

#define __privilege_state(__STATE, ...)                                         \
            break;do {                                                          \
                do {                                                            \
                    __state(__STATE, __VA_ARGS__)                               \
                } while(0); /* add extra while(0) to catch the fsm_continue()*/ \
                if (this.chState != (__STATE)) {                                \
                    break;                                                      \
                }                                                               \
            } while(1);                                                         
            
#define privilege_state(__STATE, ...)                                           \
            __privilege_state(__STATE, __VA_ARGS__)                                      
            

#define privilege_group(...)  { __VA_ARGS__;}

#define privilege_body(...)                                                     \
        do {                                                                    \
            switch (ptThis->chState) {                                          \
                case 0:                                                         \
                    ptThis->chState++;                                          \
                __VA_ARGS__                                                     \
            }                                                                   \
        while(1);                                                               \
                                                                                \
        return fsm_rt_on_going;                                                 \
    }

        
/*============================ TYPES =========================================*/

#ifndef __FSM_RT_TYPE__
#define __FSM_RT_TYPE__
//! \name finit state machine state
//! @{
typedef enum {
    fsm_rt_err          = -1,    //!< fsm error, error code can be get from other interface
    fsm_rt_cpl          = 0,     //!< fsm complete
    fsm_rt_on_going     = 1,     //!< fsm on-going
    fsm_rt_wait_for_obj = 2,     //!< fsm wait for object
    fsm_rt_asyn         = 3,     //!< fsm asynchronose complete, you can check it later.
} fsm_rt_t;
//! @}

#endif

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/




#endif

/* EOF */
