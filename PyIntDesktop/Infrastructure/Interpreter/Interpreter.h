#pragma once
#include "../../framework.h"

namespace Infrastructure
{
    typedef struct {
        LPTSTR input;
        LPWSTR output;
    } InterpreterData;


    class Interpreter
    {
        InterpreterData* _interpreterData;
    public:
        LPWSTR Interpret(LPTSTR input);
        ~Interpreter();
    };
}


