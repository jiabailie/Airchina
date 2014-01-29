/* 0 normal, 1 debug */
#define EXECUTE_DEBUG 0

#if EXECUTE_DEBUG == 0
    #define NORMAL_MODE
#elif EXECUTE_DEBUG == 1
    #define DEBUG_MODE
#endif