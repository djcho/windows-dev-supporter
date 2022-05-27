#include <Windows.h>
#include "ExecuteProcessHelper.h"


DWORD ExecuteProcessHelper::executeProcessAndWait(const wstring &applicationName, const wstring &commandLine)
{
	SHELLEXECUTEINFOW exeInfo;
	ZeroMemory(&exeInfo, sizeof(SHELLEXECUTEINFOW));

	exeInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
	exeInfo.lpFile = applicationName.c_str();
	exeInfo.lpParameters = commandLine.c_str();
	exeInfo.nShow = SW_SHOWNORMAL;// SW_SHOW;
	exeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	exeInfo.lpVerb = L"open";
	exeInfo.hInstApp = NULL;

	DWORD result = -1;
	ShellExecuteExW(&exeInfo);

	if (exeInfo.hProcess != NULL)
	{
		if (WaitForSingleObject(exeInfo.hProcess, INFINITE) == WAIT_OBJECT_0)
		{
			GetExitCodeProcess(exeInfo.hProcess, &result);
		}
		CloseHandle(exeInfo.hProcess);
	}

	return result;
}

BOOL ExecuteProcessHelper:: executeProcess(const wstring &applicationName, const wstring &commandLine)
{
	SHELLEXECUTEINFOW sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFOW));

	sei.cbSize = sizeof(SHELLEXECUTEINFOW);
	sei.lpFile = applicationName.c_str();
	sei.lpParameters = commandLine.c_str();
	sei.nShow = SW_SHOWNORMAL;
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = L"open";
	sei.hInstApp = NULL;

	return ShellExecuteExW(&sei);
}