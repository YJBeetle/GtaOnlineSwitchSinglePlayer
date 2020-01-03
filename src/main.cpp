#include <iostream>
#include <string>
#include <cstring>
#include <windows.h>
#include <psapi.h>

using namespace std;

const string GtaExe("GTA5.exe");

DWORD getGtaProcessId()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		for (int i = 0; i < 1024; i++)
		{
			DWORD processID = aProcesses[i];
			WCHAR szProcessName[512];

			// Get a handle to the process.
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

			if (hProcess != nullptr)
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
				{
					// Get the process name.
					GetModuleFileNameExW(hProcess, hMod, szProcessName, sizeof(szProcessName));

					wstring ws(szProcessName);
					string s(ws.begin(), ws.end());

					if (s.substr(s.length() - GtaExe.length()) == GtaExe)
						return processID;
				}
			}

		}
	}
	return 0;
}

typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI* NtResumeProcess)(IN HANDLE ProcessHandle);

int main(int argC, char **argV)
{
	DWORD processID = getGtaProcessId();
	if (!processID)
	{
		cout << "No GTA process" << endl;
		exit(0);
	}

	cout << "GTA processID: " << processID << endl;

	NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(GetModuleHandle("ntdll"), "NtSuspendProcess");
	NtResumeProcess pfnNtResumeProcess = (NtResumeProcess)GetProcAddress(GetModuleHandle("ntdll"), "NtResumeProcess");

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

	cout << "Suspend" << endl;
	pfnNtSuspendProcess(processHandle);
	cout << "Wait 12s" << endl;
	Sleep(12000);
	cout << "Resume" << endl;
	pfnNtResumeProcess(processHandle);

	CloseHandle(processHandle);

	return 0;
}