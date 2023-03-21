/****************************************************************************
*  Copyright 2018 Gorgon Meducer (Email:embedded_zhuoran@hotmail.com)       *
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

/*============================ INCLUDES ======================================*/
#include "fff_cfg.h"

#include "file_io.h"
#include <string.h>
#include <stdio.h>
#include <rt_sys.h>
#include <stdlib.h>
#if __IS_COMPILER_ARM_COMPILER_6__
#include <arm_compat.h>
#endif


#if defined(RTE_Compiler_EventRecorder) && defined(RTE_Compiler_IO_STDOUT_EVR)
#   include <EventRecorder.h>
#endif

#ifdef   __cplusplus
extern "C" {
#endif

/* suppress some warnings for user applications when using arm-2d.
 */
#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wgnu-variable-sized-type-not-at-end"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wextra-semi-stmt"
#   pragma clang diagnostic ignored "-Wcompound-token-split-by-macro"
#   pragma clang diagnostic ignored "-Winitializer-overrides"
#   pragma clang diagnostic ignored "-Wgcc-compat"
#   pragma clang diagnostic ignored "-Wundef"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wflexible-array-extensions"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wtautological-pointer-compare"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wimplicit-function-declaration"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_suppress 1296,174
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
//#if __IS_COMPILER_ARM_COMPILER_5__
//struct __FILE {
//    arm_file_node_t *ptStream;
//};
//#endif


/*============================ LOCAL VARIABLES ===============================*/ 

static volatile int getchar_undo =  0;
static volatile int getchar_ch   = -1;

/*============================ PROTOTYPES ====================================*/

/* enforced interface we relys on */
#if   (defined(RTE_Compiler_IO_STDOUT) && defined(RTE_Compiler_IO_STDOUT_User)) \
    || !defined(RTE_Compiler_IO_STDOUT)
extern int stdout_putchar (int ch) ;
#endif

extern int stderr_putchar (int ch) ;

#if   (defined(RTE_Compiler_IO_STDIN) && defined(RTE_Compiler_IO_STDIN_User)) \
    || !defined(RTE_Compiler_IO_STDIN)
extern int stdin_getchar (void);
#endif
/*============================ IMPLEMENTATION ================================*/
/*============================ GLOBAL VARIABLES ==============================*/

#if   (defined(RTE_Compiler_IO_STDOUT) && defined(RTE_Compiler_IO_STDOUT_User)) \
    || !defined(RTE_Compiler_IO_STDOUT)
__attribute__((weak)) 
int stdout_putchar(int_fast8_t chByte)
{

#if __FFF_CFG_IGNORE_NO_SEMIHOSTING__
    __semihost(0x03, &chByte);
#endif

    return chByte;
}

#endif




/**
  Put a character to the stdout
 
  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
#if   defined(RTE_Compiler_IO_STDOUT)
#if   defined(RTE_Compiler_IO_STDOUT_User)
extern int stdout_putchar (int ch);
#elif defined(RTE_Compiler_IO_STDOUT_ITM)
static int stdout_putchar (int ch) {
  return (ITM_SendChar(ch));
}
#elif defined(RTE_Compiler_IO_STDOUT_EVR)
static int stdout_putchar (int ch) {
  static uint32_t index = 0U;
  static uint8_t  buffer[8];
 
  if (index >= 8U) {
    return (-1);
  }
  buffer[index++] = (uint8_t)ch;
  if ((index == 8U) || (ch == '\n')) {
    EventRecordData(EventID(EventLevelOp, 0xFE, 0x00), buffer, index);
    index = 0U;
  }
  return (ch);
}
#elif defined(RTE_Compiler_IO_STDOUT_BKPT)
static int stdout_putchar (int ch) {
  __asm("BKPT 0");
  return (ch);
}
#endif
#endif

__attribute__((weak))  
int stderr_putchar(int_fast8_t chByte)
{
    return stdout_putchar(chByte);
}

__attribute__((weak)) 
void _ttywrch(int ch)
{
    stdout_putchar(ch);
}

int _sys_istty(FILEHANDLE fh)
{
    FFF_UNUSED(fh);
    return -1; /* buffered output */
}

/**
  Get a character from the stdio
 
  \return     The next character from the input, or -1 on read error.
*/
#if   defined(RTE_Compiler_IO_STDIN)
#if   defined(RTE_Compiler_IO_STDIN_User)
extern int stdin_getchar (void);
#elif defined(RTE_Compiler_IO_STDIN_ITM)
static int stdin_getchar (void) {
  int32_t ch;
 
  do {
    ch = ITM_ReceiveChar();
  } while (ch == -1);
  return (ch);
}
#elif defined(RTE_Compiler_IO_STDIN_BKPT)
static int stdin_getchar (void) {
  int32_t ch = -1;
 
  __asm("BKPT 0");
  return (ch);
}
#endif
#endif

#if !defined(RTE_Compiler_IO_STDIN) ||                                          \
    (defined(RTE_Compiler_IO_STDIN) && defined(RTE_Compiler_IO_STDIN_User))
__attribute__((weak))  
int stdin_getchar(void)
{
#if __FFF_CFG_IGNORE_NO_SEMIHOSTING__
    return __semihost(0x07, 0);
#else
    return -1;
#endif
}
#endif

/*
* opens the file whose name is the string pointed to by filename, and
* associates a stream with it.
* The argument mode points to a string beginning with one of the following
* sequences:
* "r"         open text file for reading
* "w"         create text file for writing, or truncate to zero length
* "a"         append; open text file or create for writing at eof
* "rb"        open binary file for reading
* "wb"        create binary file for writing, or truncate to zero length
* "ab"        append; open binary file or create for writing at eof
* "r+"        open text file for update (reading and writing)
* "w+"        create text file for update, or truncate to zero length
* "a+"        append; open text file or create for update, writing at eof
* "r+b"/"rb+" open binary file for update (reading and writing)
* "w+b"/"wb+" create binary file for update, or truncate to zero length
* "a+b"/"ab+" append; open binary file or create for update, writing at eof
*
* Opening a file with read mode ('r' as the first character in the mode
* argument) fails if the file does not exist or cannot be read.
* Opening a file with append mode ('a' as the first character in the mode
* argument) causes all subsequent writes to be forced to the current end of
* file, regardless of intervening calls to the fseek function. In some
* implementations, opening a binary file with append mode ('b' as the
* second or third character in the mode argument) may initially position
* the file position indicator beyond the last data written, because of the
* NUL padding.
* When a file is opened with update mode ('+' as the second or third
* character in the mode argument), both input and output may be performed
* on the associated stream. However, output may not be directly followed
* by input without an intervening call to the fflush fuction or to a file
* positioning function (fseek, fsetpos, or rewind), and input be not be
* directly followed by output without an intervening call to the fflush
* fuction or to a file positioning function, unless the input operation
* encounters end-of-file. Opening a file with update mode may open or
* create a binary stream in some implementations. When opened, a stream
* is fully buffered if and only if it does not refer to an interactive
* device. The error and end-of-file indicators for the stream are
* cleared.
* Returns: a pointer to the object controlling the stream. If the open
*          operation fails, fopen returns a null pointer.
*/
__attribute__((weak)) 
_ARMABI FILE *fopen(const char * __restrict pchName,
                    const char * __restrict pchMode)
{
    FILE *ptResult = NULL;
    do {
        if (NULL == pchName || NULL == pchMode) {
            break;
        }
        
        struct {
            const char *pchStr;
            uint8_t chMode;
        } const c_tLookupTable[] = {
        
/*
    #define OPEN_R 0
    #define OPEN_W 4
    #define OPEN_A 8
    #define OPEN_B 1
    #define OPEN_PLUS 2
*/
        
            {"r",   OPEN_R},
            {"w",           OPEN_W},
            {"a",           OPEN_W| OPEN_A},
            {"r+",  OPEN_R| OPEN_W|                 OPEN_PLUS},                          
            {"w+",  OPEN_R| OPEN_W|                 OPEN_PLUS},
            {"a+",  OPEN_R| OPEN_W| OPEN_A|         OPEN_PLUS},
            
            {"rb",  OPEN_R|                 OPEN_B},
            {"wb",          OPEN_W|         OPEN_B},
            {"ab",          OPEN_W| OPEN_A| OPEN_B},
            {"r+b", OPEN_R| OPEN_W|         OPEN_B| OPEN_PLUS},        
            {"rb+", OPEN_R| OPEN_W|         OPEN_B| OPEN_PLUS},        
            {"w+b", OPEN_R| OPEN_W|         OPEN_B| OPEN_PLUS},
            {"wb+", OPEN_R| OPEN_W|         OPEN_B| OPEN_PLUS},
            {"a+b", OPEN_R| OPEN_W| OPEN_A| OPEN_B| OPEN_PLUS},
            {"ab+", OPEN_R| OPEN_W| OPEN_A| OPEN_B| OPEN_PLUS},
        }, *ptItem = c_tLookupTable;

        uint_fast8_t n = dimof(c_tLookupTable);
        do {
            if (0 == strcmp(pchMode, ptItem->pchStr)) {
                
                //! call _sys_open to open file
                ptResult = (FILE *) (_sys_open(pchName, ptItem->chMode));
                break;
            }
            ptItem++;
        } while(--n);
        
    } while(false);
    
    return ptResult;
}

/*
* causes the stream pointed to by stream to be flushed and the associated
* file to be closed. Any unwritten buffered data for the stream are
* delivered to the host environment to be written to the file; any unread
* buffered data are discarded. The stream is disassociated from the file.
* If the associated buffer was automatically allocated, it is deallocated.
* Returns: zero if the stream was succesfully closed, or nonzero if any
*          errors were detected or if the stream was already closed.
*/

__attribute__((weak)) 
__attribute__((__nonnull__(1)))
_ARMABI int fclose(FILE * ptFile)
{
    return _sys_close((FILEHANDLE)ptFile);
}
    
/*
* If the stream points to an output or update stream in which the most
* recent operation was output, the fflush function causes any unwritten
* data for that stream to be delivered to the host environment to be
* written to the file. If the stream points to an input or update stream,
* the fflush function undoes the effect of any preceding ungetc operation
* on the stream.
* Returns: nonzero if a write error occurs.
*/
__attribute__((weak)) 
__attribute__((__nonnull__(1)))
_ARMABI int fflush(FILE * ptFile)
{
    return _sys_ensure((FILEHANDLE)ptFile);
}
   


/*
* writes, from the array pointed to by ptr up to nmemb members whose size
* is specified by size, to the stream pointed to by stream. The file
* position indicator (if defined) is advanced by the number of characters
* successfully written. If an error occurs, the resulting value of the file
* position indicator is indeterminate.
* Returns: the number of members successfully written, which will be less
*          than nmemb only if a write error is encountered.
*/
__attribute__((weak)) 
__attribute__((__nonnull__(1,4)))
_ARMABI size_t fwrite(  const void * __restrict pchBuffer,
                        size_t tElementSize, 
                        size_t tElementCount, 
                        FILE * __restrict ptFile) 
{
    int nResult = 
    _sys_write(( FILEHANDLE)ptFile, 
                        pchBuffer, 
                        tElementSize * tElementCount,
                        0);
    if (nResult < 0) {
        return 0;
    }
    return nResult;
}

/*
 * reads into the array pointed to by ptr, up to nmemb members whose size is
 * specified by size, from the stream pointed to by stream. The file
 * position indicator (if defined) is advanced by the number of characters
 * successfully read. If an error occurs, the resulting value of the file
 * position indicator is indeterminate. If a partial member is read, its
 * value is indeterminate. The ferror or feof function shall be used to
 * distinguish between a read error and end-of-file.
 * Returns: the number of members successfully read, which may be less than
 *          nmemb if a read error or end-of-file is encountered. If size or
 *          nmemb is zero, fread returns zero and the contents of the array
 *          and the state of the stream remain unchanged.
 */
__attribute__((weak)) 
__attribute__((__nonnull__(1,4)))
_ARMABI size_t fread(void * __restrict pchBuffer,
                    size_t tElementSize, size_t tElementCount, FILE * __restrict ptFile)
 {
    int nResult = 
    _sys_read(( FILEHANDLE)ptFile, 
                        pchBuffer, 
                        tElementSize * tElementCount,
                        0);
    if (nResult < 0) {
        return 0;
    }
    return nResult;
 }
 
 /*
 * tests the end-of-file indicator for the stream pointed to by stream.
 * Returns: nonzero iff the end-of-file indicator is set for stream.
 */
 __attribute__((__nonnull__(1)))
_ARMABI int feof(FILE * ptFile)
{
    FFF_ASSERT(NULL != FFF_IO.EndOfStream);
    return FFF_IO.EndOfStream((arm_file_node_t *)ptFile);
}

/* 
 * These names are used during library initialization as the
 * file names opened for stdin, stdout, and stderr.
 * As we define _sys_open() to always return the same file handle,
 * these can be left as their default values.
 */
/*WEAK const char __stdin_name[] =  ":tt";
WEAK const char __stdout_name[] =  ":tt";
WEAK const char __stderr_name[] =  ":tt";
*/

FILEHANDLE _sys_open(const char *pchName, int nOpenMode)
{

    FILEHANDLE tResult = -1;
    //uint_fast16_t hwFeature = 0;
    do {
        if (NULL == pchName) {
            break;
        } 
        
/*
 * openmode is a bitmap, whose bits are given below. They
 * correspond directly to the ANSI mode specification.

* "r"         open text file for reading
* "w"         create text file for writing, or truncate to zero length
* "a"         append; open text file or create for writing at eof
* "rb"        open binary file for reading
* "wb"        create binary file for writing, or truncate to zero length
* "ab"        append; open binary file or create for writing at eof
* "r+"        open text file for update (reading and writing)
* "w+"        create text file for update, or truncate to zero length
* "a+"        append; open text file or create for update, writing at eof
* "r+b"/"rb+" open binary file for update (reading and writing)
* "w+b"/"wb+" create binary file for update, or truncate to zero length
* "a+b"/"ab+" append; open binary file or create for update, writing at eof

#define OPEN_R 0
#define OPEN_W 4
#define OPEN_A 8
#define OPEN_B 1
#define OPEN_PLUS 2
 */

        FFF_ASSERT(NULL != FFF_IO.Open);
        tResult = (FILEHANDLE)(FFF_IO.Open(pchName, (uint16_t)nOpenMode));

    } while(false);
    return tResult; /* everything goes to the same output */
}
int _sys_close(FILEHANDLE fh)
{
    FFF_ASSERT(NULL != FFF_IO.Close);
    return !FFF_IO.Close((arm_file_node_t *)fh);
}



/**
   Writes the character specified by c (converted to an unsigned char) to
   the output stream pointed to by stream, at the position indicated by the
   associated file position indicator (if defined), and advances the
   indicator appropriately. If the file position indicator is not defined,
   the character is appended to the output stream.
 
  \param[in] c       Character
  \param[in] stream  Stream handle
 
  \return    The character written. If a write error occurs, the error
             indicator is set and fputc returns EOF.
*/
int fputc (int c, FILE * stream) {
#if (!defined(RTE_Compiler_IO_STDOUT) && !defined(RTE_Compiler_IO_STDERR))
    FFF_UNUSED(c);
    FFF_UNUSED(stream);
#endif
    
    if (stream == &__stdout) {
        return (stdout_putchar(c));
    } else if (stream == &__stderr) {
        return (stderr_putchar(c));
    } else {
        FFF_ASSERT(NULL != FFF_IO.WriteByte);
        if (false == FFF_IO.WriteByte((arm_file_node_t *)stream, c)) {
            return -1;
        }
    }
    return c;
}


__attribute__((weak)) 
int __stdout_string(const unsigned char *buf,
                    unsigned len)
{
    if (NULL == buf || 0 == len) {
        return -1;
    }
    
    do {
        int err = stdout_putchar(*buf++);
        if ( err < 0) {
            return err;
        }
    } while(--len);
    
    return 0;
}

__attribute__((weak)) 
int __stdin_data(unsigned char *buf,
                 unsigned len)
{
    if (NULL == buf || 0 == len) {
        return -1;
    }
    
    do {
        int data = stdin_getchar();
        if ( data < 0) {
            return data;
        }
        
        *buf++ = (uint8_t)data;
    } while(--len);
    
    return 0;
}


__attribute__((weak)) 
int __stderr_string(const unsigned char *buf,
                    unsigned len)
{
    if (NULL == buf || 0 == len) {
        return -1;
    }
    
    do {
        int err = stderr_putchar(*buf++);
        if ( err < 0) {
            return err;
        }
    } while(--len);
    
    return 0;
}

int _sys_write(FILEHANDLE fh, const unsigned char *buf,
               unsigned len, int mode)
{
    arm_file_node_t *ptFile = (arm_file_node_t *)fh;
    if (fh == (FILEHANDLE)&__stdout) {
        return __stdout_string(buf, len);
    } else if (fh == (FILEHANDLE)&__stderr) {
        return __stderr_string(buf, len);
    } else if (fh == (FILEHANDLE)&__stdin) {
        return -1;
    }
    FFF_ASSERT(NULL != FFF_IO.Write);
    return FFF_IO.Write(ptFile, (uint8_t *)buf, len);
}


/**
   Obtains the next character (if present) as an unsigned char converted to
   an int, from the input stream pointed to by stream, and advances the
   associated file position indicator (if defined).
 
  \param[in] stream  Stream handle
 
  \return    The next character from the input stream pointed to by stream.
             If the stream is at end-of-file, the end-of-file indicator is
             set and fgetc returns EOF. If a read error occurs, the error
             indicator is set and fgetc returns EOF.
*/
int fgetc (FILE * stream) {

    int ch = -1;

    do {
        if (stream == &__stdin) {
            if (getchar_undo) {
                ch = getchar_ch;
                getchar_ch = -1;
                getchar_undo = 0;
                break;
            }
            ch = stdin_getchar();
            getchar_ch = ch;
        } else {
            FFF_ASSERT(NULL != FFF_IO.ReadByte);
            ch = FFF_IO.ReadByte((arm_file_node_t *)stream);
        }
    } while(0);

    return ch;
}
 
 
/**
   The function __backspace() is used by the scanf family of functions, and must
   be re-implemented if you retarget the stdio arrangements at the fgetc() level.
 
  \param[in] stream  Stream handle
 
  \return    The value returned by __backspace() is either 0 (success) or EOF
             (failure). It returns EOF only if used incorrectly, for example,
             if no characters have been read from the stream. When used 
             correctly, __backspace() must always return 0, because the scanf
             family of functions do not check the error return.
*/
int __backspace(FILE *stream) 
{
    if (stream == &__stdin) {
        if (getchar_ch != -1) {
            getchar_undo = 1;
            return (0);
        }
        return (-1);
    }

    return (-1);
}
 
 

int _sys_read(FILEHANDLE fh, unsigned char *buf,
              unsigned len, int mode)
{
    arm_file_node_t *ptFile = (arm_file_node_t *)fh;
    if (fh == (FILEHANDLE)&__stdout) {
        return -1;
    } else if (fh == (FILEHANDLE)&__stderr) {
        return -1;
    } else if (fh == (FILEHANDLE)&__stdin) {
        return __stdin_data(buf, len);
    }

    FFF_ASSERT(NULL != FFF_IO.Read);
    int nResult = FFF_IO.Read(ptFile, buf, len);
    
    return nResult; /* not supported */
}


/**
  Called from FFF_ASSERT() and prints a message on stderr and calls abort().
 
  \param[in] expr  FFF_ASSERT expression that was not TRUE
  \param[in] file  source file of the assertion
  \param[in] line  source line of the assertion
*/
__attribute__((noreturn))
void __aeabi_assert (const char *expr, const char *file, int line) {
  char str[12], *p;
 
  fputs("*** assertion failed: ", stderr);
  fputs(expr, stderr);
  fputs(", file ", stderr);
  fputs(file, stderr);
  fputs(", line ", stderr);
 
  p = str + sizeof(str);
  *--p = '\0';
  *--p = '\n';
  while (line > 0) {
    *--p = '0' + (line % 10);
    line /= 10;
  }
  fputs(p, stderr);
 
  abort();
}

/*
 * Flush any OS buffers associated with fh, ensuring that the file
 * is up to date on disk. Result is >=0 if OK, negative for an
 * error.
 */
int _sys_ensure(FILEHANDLE fn)
{
    return FFF_IO.Flush((arm_file_node_t *)fn) ? 0 : -1;
}

int _sys_seek(FILEHANDLE fh, long pos)
{
    FFF_UNUSED(fh);
    FFF_UNUSED(pos);
    return -1; /* not supported */
}
long _sys_flen(FILEHANDLE fh)
{
    FFF_UNUSED(fh);
    return -1; /* not supported */
}

#ifdef   __cplusplus
}
#endif

