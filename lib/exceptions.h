/*
    ARM IP interface

    exceptions.h:
                 exception-handlers testboard

*/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <config.h>
#include <print.h>


void __HaltUndef(void);
void __HaltSwi(void);
void __HaltPabort(void);
void __HaltDabort(void);
void __HaltFiq(void);
void __HaltIrq(void);

void __HaltExit(int exitcode) __attribute__ ((noreturn));
void __HaltVICerr(void);

#endif /* EXCEPTIONS_H */
