/*
 * Macros to control mimic library visibility
 * 
 * Copyright 2017 Sergio Oller <sergioller@gmail.com>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef CST_LIB_VISIBILITY_H
#define CST_LIB_VISIBILITY_H
/* DLLEXPORT/DLLIMPORT: Used in public symbols.
    - DLLEXPORT: When the library is being compiled.
    - DLLIMPORT: When the library is being used.
   DLLPRIVATE: Used for private (hidden) symbols.
*/
#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(__GNUC__) || defined(__clang__)
    #define DLLEXPORT __attribute__ ((dllexport))
    #define DLLIMPORT  __attribute__ ((dllimport))
  #elif defined(_MSC_VER)
    #define DLLEXPORT __declspec(dllexport)
    #define DLLIMPORT __declspec(dllimport)
  #endif
  #define DLLPRIVATE 
#else
  #define DLLEXPORT __attribute__ ((visibility ("default")))
  #define DLLIMPORT
  #define DLLPRIVATE __attribute__ ((visibility ("hidden")))
#endif

/* In a mimic plugin feel free to reuse DLLEXPORT, DLLPRIVATE and DLLIMPORT
 * by using a syntax similar to this in your plugin includes:
 */
#if defined(COMPILE_MIMIC_CORE)
  #define MIMIC_CORE_PUBLIC DLLEXPORT
  #define MIMIC_CORE_PRIVATE DLLPRIVATE
#else
  #define MIMIC_CORE_PUBLIC DLLIMPORT
  #define MIMIC_CORE_PRIVATE 
#endif

#endif /* CST_LIB_VISIBILITY_H header guard */

