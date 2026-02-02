/*
** DELOS PROJECT, 2025
** include/defines
** File description:
** Defines Header file
*/

#ifndef DEFINES_H_
    #define DEFINES_H_

    #ifndef IN
        #define IN      /* @deprecated */
    #endif /* ifndef IN */

    #ifndef OUT
        #define OUT    /* @deprecated */
    #endif /* ifndef OUT */

    #ifndef OK_TRUE
        #define OK_TRUE 1
    #endif /* ifndef OK_TRUE */

    #ifndef KO_FALSE
        #define KO_FALSE 0
    #endif /* ifndef KO_FALSE */

    #ifndef NULL
        #define NULL 0
    #endif /* ifndef NULL */

    #ifndef UNUSED
        #define UNUSED __attribute__((unused))
    #endif /* ifndef UNUSED */

    #ifndef MAX
        #define MAX(x, y) ((x > y) ? x : y)
    #endif /* ifndef MAX */
    
    #ifndef MIN
        #define MIN(x, y) ((x < y) ? x : y)
    #endif /* ifndef MIN */

    #ifndef ARRAY_SIZE
        #define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
    #endif /* ifndef ARRAY_SIZE */

#endif /* ifndef DEFINES_H_ */
