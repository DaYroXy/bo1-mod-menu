#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

using namespace std;
const char* PROCESS_NAME = "plutonium-bootstrapper-win32.exe";

MODULEENTRY32 GetModule(const char* moduleName, unsigned long ProcessID)
{
    MODULEENTRY32 modEntry = { 0 };
 
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
 
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 curr = { 0 };
 
        curr.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &curr))
        {
            do
            {
                if (!strcmp(curr.szModule, moduleName))
                {
                    modEntry = curr;
                    break;
                }
            } while (Module32Next(hSnapshot, &curr));
        }
        CloseHandle(hSnapshot);
    }
    return modEntry;
}


int main () {

    PROCESSENTRY32 process_entry = {sizeof(PROCESSENTRY32)};
    HANDLE process_snapshot;
    DWORD process_id = 0;

    process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(Process32First(process_snapshot, &process_entry)) {
        // Loop threw all processes, and find the PROCESS_NAME
        do {
            if(!strcmp(process_entry.szExeFile, PROCESS_NAME)) {
                process_id = process_entry.th32ProcessID;
                break;
            }
        } while(Process32Next(process_snapshot, &process_entry));
    }

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    // Money Address : "plutonium-bootstrapper-win32.exe"+180A6C8
    float amountOfMoney = 5995;

    MODULEENTRY32 module = GetModule("plutonium-bootstrapper-win32.exe", process_id);

    unsigned long long result;
    cout << (void*)(unsigned long long )module.modBaseAddr;
    cout << (unsigned long long) module.modBaseAddr + 0x00109B74;

    ReadProcessMemory(handle, (LPVOID)(module.modBaseAddr + 0x00109B74), &result, sizeof(result), NULL);
    //00e9a108
    while (true) {
        WriteProcessMemory(handle, (LPVOID)(result+0x3C), &amountOfMoney, sizeof(amountOfMoney), NULL);
    }
    return 1;
}
