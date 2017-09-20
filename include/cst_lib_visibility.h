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

/*

### Seven definitions:

a) A *core executable* is an executable compiled by this repository (e.g. `mimic-core`, `t2p`)
b) A *non-core executable* is any executable that is not a *core executable*.
c) A *public header* is a header that is installed, so all executables can use it.
d) A *private header* is a header that is not installed, so *non-core executables* can't even see them.
e) A *public symbol* is a symbol that can be used by any executable.
f) A *private symbol* is a symbol that is only used by the *libmimiccore library*.
g) A *protected symbol* is a symbol that is used by the *libmimiccore library* and *core executables*.

### Ten rules:

1. Each symbol is only found in one header.
2. Public symbols should be tagged with `MIMIC_CORE_PUBLIC`
3. Private symbols should be tagged with `MIMIC_CORE_PRIVATE`.
4. Protected symbols have to be tagged with `MIMIC_CORE_PROTECTED`
5. Headers with at least one *public symbol* are *public headers*.
6. Any header included by a *public header* is also a *public header*.
7. Public headers should try to only include public symbols, not protected or private symbols.
8. Private headers can include public headers.
9. Private headers should have a file name like `*_internal.h`.
10. Tests should be allowed to call any kind of symbol (to test it)

### Notes:

- I am ignoring rule 10 because I have not found a way to comply with it. If I need to test
  the symbol I make it protected.
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
 * by using a syntax similar to the one below in your plugin:
 */
#if defined(COMPILE_LIB_MIMIC_CORE) /* Define this when compiling the shared library */
  #define MIMIC_CORE_PUBLIC DLLEXPORT
  #define MIMIC_CORE_PROTECTED MIMIC_CORE_PUBLIC
  #define MIMIC_CORE_PRIVATE DLLPRIVATE
#elif defined(COMPILE_EXE_MIMIC_CORE) /* Define this when compiling core-executables */
  #define MIMIC_CORE_PUBLIC DLLIMPORT
  #define MIMIC_CORE_PROTECTED MIMIC_CORE_PUBLIC
  #define MIMIC_CORE_PRIVATE DLLPRIVATE
#else /* When non-core executables are compiled they only get public symbols: */
  #define MIMIC_CORE_PUBLIC DLLIMPORT
  #undef MIMIC_CORE_PROTECTED
  #undef MIMIC_CORE_PRIVATE
#endif

#endif /* CST_LIB_VISIBILITY_H header guard */

