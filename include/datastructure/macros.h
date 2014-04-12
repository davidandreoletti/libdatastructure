//
//  macros.h
//  libdatastructure
//
//  Created by Andreoletti David.
//  Copyright 2012 IO Stark. All rights reserved.
//

#ifndef INCLUDE_DATASTRUCTURE_MACROS_H_
#define INCLUDE_DATASTRUCTURE_MACROS_H_

#include <assert.h>

#ifdef __cplusplus
    /*
     * Prevents name mangling (i.e name decoration)
     * when compiling C functions as C++ translation unit
     */
    #define EXTERN_C_BEGIN  extern "C" {
    #define EXTERN_C_END    }
#else
    #define EXTERN_C_BEGIN
    #define EXTERN_C_END
#endif

#ifdef DEBUG
    #define ASSERT(x)   assert(x);
#else
    #define ASSERT(x)
#endif

#endif  // INCLUDE_DATASTRUCTURE_MACROS_H_
