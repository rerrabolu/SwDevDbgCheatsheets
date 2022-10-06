
In C, one can get the name of current function by using "__func__" macro

    "__func__" is the predefine macro added in C99

It is a standard feature of C99.
    https://www.includehelp.com/c-programs/print-the-current-function-name-by-using__func__in-c.aspx

    Example Code:
    #include <stdio.h>

    // Function definition that will print its name
    void fun1(void) {
	    printf("Called function is: %s\n",__func__);
    }

    In a similar manner one can get and print the line number
    
    The "__LINE__" is an inbuilt Macro in C programming language,
        it returns current line number of the code.
    
        e.g. printf("Line: %d\n",__LINE__);

It appears that there are no standard C defined macros to get calling
function's name. One solution is to define a macro which will print the
caller function's name before calling the callee function. I don't have
the macro, but one could research about it. One example is given here.

    #define LOG(func, ...)                              \
        {                                               \
            printf("Calling Function: %s\n", __func__); \
            func ( __VA_ARGS__);                        \
        }


    An example code fragment and its output is given below:

    void func_macro(int x, double y, bool z) {
        printf("1. Current function is: %s, value of X is: %d\n", __func__, x);
        printf("2. Current function is: %s, value of Y is: %f\n", __func__, y);
        printf("3. Current function is: %s, value of Z is: %d\n", __func__, z);
    }

    int main(char * args, int arg_count) {
        printf("\n");
        printf("\n");
        LOG(func_macro, 108, 11.23, true);
        printf("\n");
        printf("\n");
        LOG(func_macro, 1008, 112.326, false);
        printf("\n");
        printf("\n");
    }
    
    // Output of the run

        Calling Function: main
        1. Current function is: func_macro, value of X is: 108
        2. Current function is: func_macro, value of Y is: 11.230000
        3. Current function is: func_macro, value of Z is: 1

        Calling Function: main
        1. Current function is: func_macro, value of X is: 1008
        2. Current function is: func_macro, value of Y is: 112.326000
        3. Current function is: func_macro, value of Z is: 0


In the Linux Kernel area we have the following macro:
    
    __builtin_return_address(int n);

    @note: Do not go back too far in trying to get the
    function name as it can lead to a crash

    @note: This macro does not work in user space. However my testing is
    not thorough i.e. I used gcc 9.4.0 to compile a simple C program. Only
    __func__ macro worked.

One can go up the stack frame to get the function name.

    0 will get the current function's address
    1 will get the parent function's address
    2 will get the grand-parent function's address


    pr_err("Sairam: Grandparent's function name is: %ps\n", __builtin_return_address(2));
    pr_err("Sairam: Parent's function name is: %ps\n", __builtin_return_address(1));
    pr_err("Sairam: Current function name is: %ps\n", __builtin_return_address(0));


The macro will get the address of the function. To print the name one
can use one of the following "%ps" or "%pS" format specifiers.

    https://stackoverflow.com/questions/4141324/function-caller-in-linux-kernel


Example Program To Print Calling and Callee Function Names:

    #include <stdio.h>
    #include <stdbool.h>

    #define LOG(func, ...)                      \
          {                                           \
            printf("Calling Function: %s\n", __func__);   \
            func ( __VA_ARGS__);                      \
          }


    void func_macro(int x, double y, bool z) {
        printf("1. Current function is: %s, value of X is: %d\n", __func__, x);
        printf("2. Current function is: %s, value of Y is: %f\n", __func__, y);
        printf("3. Current function is: %s, value of Z is: %d\n", __func__, z);
    }


    int main(char * args, int arg_count) {
        printf("\n");
        printf("\n");
        LOG(func_macro, 108, 11.23, true);
        printf("\n");
        printf("\n");
        LOG(func_macro, 1008, 112.326, false);
        printf("\n");
        printf("\n");
    }



