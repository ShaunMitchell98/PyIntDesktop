#include "Interpreter.h"
#include "../CharConverter/CharConverter.h"

extern "C" {
#include <VMFunctions.h>
}

static Settings InitialiseTestSettings() {
    Settings settings;
    settings.output.location = LOCATION_STRING;
    settings.bytecode.enabled = false;
    settings.execution.enabled = false;
    settings.garbage.enabled = false;
    settings.output.string = (char*)malloc(100);
    settings.runMode = RUN_FILE;
    settings.output.string[0] = '\0';
    return settings;
}

static char* RunInterpreter(char* code)
{
    Settings settings = InitialiseTestSettings();
    Interpret(code, &settings);
    return settings.output.string;
}

static DWORD WINAPI InterpretInternal(LPVOID lpParam) 
{
    InterpreterData* interpreterData = (InterpreterData*)lpParam;
    CharConverter charConverter;
    char* codeChar = charConverter.LptstrToCharPointer(interpreterData->input, CP_UTF8);
    char* output = RunInterpreter(codeChar);
    interpreterData->output = charConverter.CharPointerToLpwstr(output, CP_UTF8);
    return 0;
}

LPWSTR Interpreter::Interpret(LPTSTR input)
{
    _interpreterData = (InterpreterData*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(InterpreterData));

    if (_interpreterData == NULL) {
        ExitProcess(3);
    }

    _interpreterData->input = input;

    HANDLE hThread = CreateThread(NULL, 0, InterpretInternal, _interpreterData, 0, NULL);

    if (hThread == NULL) {
        ExitProcess(3);
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return _interpreterData->output;
}

Interpreter::~Interpreter() {
    HeapFree(GetProcessHeap(), 0, _interpreterData->output);
    HeapFree(GetProcessHeap(), 0, _interpreterData);
}