/*============================================================================
  Project ......................... COMMON
  Component ....................... LIB
  File revision.................... 
  Last modification date .......... 
  -------------------------------------------------------------------------
  Copyright ... This software is JCI property. Duplication or
                disclosure is prohibited without JCI written permission
  ------------------------------------------------------------------------- 
    
  - This file should only contain data useful to the programmer to use
    this component
  - Component exported macros, constants and types are defined in this file
  - Component exported variables and functions are declared in this file 
    with the "extern" key word
  
  -------------------------------------------------------------------------
  lib.h file review :
  
  ==========================================================================*/

#ifndef I_LIB_H
#define I_LIB_H (1)


/*-------------------------------------------------------------------------
  Included files to resolve specific definitions in this file

  #include <system_file_name.h>
  #include "project_file_name.h"
  -------------------------------------------------------------------------*/
#include "deftypes.h"

#ifdef USE_LIBSTK
// This include file defines the starter-kit configuration
#include "lib_stk.h"
#endif

//#include "lib_itp.h"
typedef unsigned char         Bool;


#ifndef _QAC_
    #ifdef COMPILATION_NATIVE
      #include <string.h>
      #ifndef UTEST
        #include <assert.h>
      #else
        #include "Testeur.h"
      #endif //UTEST
    #else
      #include <string.h>

// if your compiler's libraries define correctly the function assert,
// you can include Assert.h. Else, you can use the default managment.

// #include <assert.h>

// The default management calls the function LIBAssertionFailed when a
// error is detected. The macro is not defined when DEBUG option is not
// set.
    #endif
#else
// System's libraries are not includes in QAC configuration
#endif

/*-------------------------------------------------------------------------
  Constant data

  #define cCMPConstantName   ((tType) ConstantValue)
  -------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
  Exported Macro

  #define mCMPMacroName   (MacroDefinition)
  -------------------------------------------------------------------------*/ 

#ifndef _QAC_

    #define mLIBmemset(ptr,mtf,len) memset( ptr, mtf, len )
    #define mLIBmemcpy(ptr,src,len) memcpy( ptr, src, len )
    #define mLIBmemcmp(ptr,src,len) memcmp( ptr, src, len )

    #ifdef COMPILATION_NATIVE
        #ifndef ATTOL_COV
            #ifndef UTEST
                #define mLIBassert(Condition)   assert( Condition )
            #else
                #define mLIBassert(Condition)   mTSTAssert(Condition)
            #endif
        #else
// In the configuration ATTOL COVERAGE
// Assert are not defined.
            #define mLIBassert(Condition)
        #endif
    #else
// If your compiler define assert, you can use this macro :
// #define mLIBassert(Condition)   assert( Condition )

// The default management calls the function LIBAssertionFailed when
// a error is detected. The macro is not defined when DEBUG option
// is not set.
           #ifdef DEBUG
                #define mLIBassert(Condition) \
                    if((Condition) == cFalse) \
                    {                         \
                        LIBAssertionFailed(); \
                    }
            #else
                #define mLIBassert(Condition)
            #endif
    #endif
#else
// QAC configuration
extern void NOP(void);
extern void *memconsumer;

extern int QACmemcmp( const void *ptr,const void *src, unsigned int len);
extern void * QACmemcpy(void *ptr, const void *src, unsigned int len);
extern void * QACmemset(void *ptr, int mtf, unsigned int len);
extern void QACassert(void);

    #define mLIBmemset(ptr,mtf,len) (memconsumer = QACmemset(ptr,mtf,len))
    #define mLIBmemcpy(ptr,src,len) (memconsumer = QACmemcpy(ptr, src, len))
    #define mLIBmemcmp(ptr,src,len) QACmemcmp(ptr,src,len)
    #define mLIBassert(Condition)   QACassert()
#endif

/*-------------------------------------------------------------------------
  Exported type

  struct  sCMPStructureName { ... };
  union   uCMPUnionName { ... };
  enum    eCMPEnumerationName { ... };
  typedef Expression tCMPTypeName;

  -------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
  Exported data

  extern  tType   CMPVariableName;
  extern  tType*  pCMPVariableName; 
  -------------------------------------------------------------------------*/ 

/*-------------------------------------------------------------------------
  Constant exported data

  extern const tType  CMPVariableName;
  -------------------------------------------------------------------------*/ 

/*-------------------------------------------------------------------------
  Exported functions

  extern tReturnType CMPFunctionName(tTypeArgument1 ArgumentName1, ... );
  -------------------------------------------------------------------------*/ 
extern void LIBAssertionFailed(void);

/*-------------------------------------------------------------------------
  End of includes non re entry
  -------------------------------------------------------------------------*/
#endif /* I_LIB_H */
