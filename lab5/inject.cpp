#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>




int main() {
	char processName[] = "notepad.exe";
	//char Dll[] = "C:\\IATHookdll.dll";
	char Dll[] = "E:\\VS2017\\Debug\\IATHookdll.dll";


	//get process id by process name
	DWORD ProcessId = 0;
	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSanpshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSanpshot) {
		printf("Error\n");
		return 0;
	}

	bool ret = Process32First(hSanpshot, &ProcessEntry);
	while (ret) {
		if (strcmp(ProcessEntry.szExeFile, processName) == 0) {
			//ProcessId = ProcessEntry.th32ParentProcessID;//
			ProcessId = ProcessEntry.th32ProcessID;
			printf("%s\n", ProcessEntry.szExeFile);
			break;
		}
		ret = Process32Next(hSanpshot, &ProcessEntry);
	}
	printf("process id is : %lu\n", ProcessId);
	if (ProcessId == 0) {
		printf("no such process\n");
		return 0;
	}
	CloseHandle(hSanpshot);


	//get process handle by process id

	HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ProcessId);
	if (NULL == ProcessHandle) {
		printf("get process handle failed\n");
		return 0;
	}



	size_t len = strlen(Dll) + 1;

	//allocate memory in the target process

	LPVOID baseAdd = VirtualAllocEx(ProcessHandle, NULL, len, MEM_COMMIT, PAGE_READWRITE);
	if (NULL == baseAdd) {
		printf("Allocate memory failed!\n");
		return 0;
	}

	// write memory,if failed,return 0

	BOOL write = WriteProcessMemory(ProcessHandle, baseAdd, (LPCVOID)Dll, len, NULL);
	if (write == 0) {
		printf("write memory failed\n");
		return 0;
	}



	

	//create remote thread

	HANDLE ThreadHandle = CreateRemoteThread(ProcessHandle, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, baseAdd, 0, NULL);
	if (0 == ThreadHandle) {
		printf("create remote thread failed\n");
		return 0;
	}


	WaitForSingleObject(ThreadHandle, INFINITE);
	printf("thread end\n");


	VirtualFreeEx(ProcessHandle, baseAdd, 0, MEM_RELEASE);
	CloseHandle(ProcessHandle);
	CloseHandle(ThreadHandle);

}