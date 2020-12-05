#include "Interpreter.h"

extern "C" {
#include <VMFunctions.h>
}

typedef struct {
    LPTSTR input;
    LPWSTR output;
} InterpreterData;

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

static char* LptstrToCharPointer(const wchar_t* wstr, unsigned int codePage)
{
    int sizeNeeded = WideCharToMultiByte(codePage, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* encodedStr = new char[sizeNeeded];
    WideCharToMultiByte(codePage, 0, wstr, -1, encodedStr, sizeNeeded, NULL, NULL);
    return encodedStr;
}

static LPWSTR CharPointerToLpwstr(char* string, unsigned int codePage)
{
    LPWSTR text = (LPTSTR)malloc(25);
    MultiByteToWideChar(codePage, 0, string, -1, text, 12);
    return text;
}

static DWORD WINAPI InterpretInternal(LPVOID lpParam) 
{
    InterpreterData* interpreterData = (InterpreterData*)lpParam;
    char* codeChar = LptstrToCharPointer(interpreterData->input, CP_UTF8);
    char* output = RunInterpreter(codeChar);
    interpreterData->output = CharPointerToLpwstr(output, CP_UTF8);
    return 0;
}

LPWSTR Interpreter::Interpret(LPTSTR input)
{
    InterpreterData* interpreterData = (InterpreterData*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(InterpreterData));

    if (interpreterData == NULL) {
        ExitProcess(3);
    }

    interpreterData->input = input;

    HANDLE hThread = CreateThread(NULL, 0, InterpretInternal, interpreterData, 0, NULL);

    if (hThread == NULL) {
        ExitProcess(3);
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    LPWSTR output = interpreterData->output;
    HeapFree(GetProcessHeap(), 0, interpreterData);
    return output;
}