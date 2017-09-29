/**
 * @author AUTHOR 
 * @date MM_DD_YYYY
 * @file FILE_NAME 
 * @brief DESC
 * Copyright (c) 2016
 */
 
#ifndef RET_CODE_H
#define RET_CODE_H

/**
 * @brief Return codes we have to return as defined by spec.
 */
typedef enum {
    ERR_NONE            = 0,    /**< No error */
    ERR_LEX             = 1,    /**< Lexical analysis error */
    ERR_SYNTAX          = 2,    /**< Syntax analysis error */
    ERR_SEM_UNDEF  	    = 3,    /**< Undefined/redefined function/variable */
    ERR_SEM_TYPE_COMP   = 4,    /**< Type compatibility error */
    ERR_TYPE_DETECT     = 5,    /**< Data type detection error */
    ERR_SEMANTIC_OTHER  = 6,    /**< Other semantic error */
    ERR_RUN_INPUT_NUM   = 7,    /**< Failed to load number from stdin */
    ERR_RUN_UNINIT_VAR  = 8,    /**< Uninitialized variable error */
    ERR_RUN_DIV_ZERO    = 9,    /**< Division by zero error */
    ERR_RUN_OTHER       = 10,   /**< Other runtime error */
    END_FILE            = 41,   /**< End of File */
    ERR_INTERNAL        = 99    /**< Internal error */
} err_code_te;

#endif
