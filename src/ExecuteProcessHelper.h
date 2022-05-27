#ifndef _EE_WIN_GLOBAL_EXECUTE_PROCESS_H_
#define _EE_WIN_GLOBAL_EXECUTE_PROCESS_H_

#include <string>

using namespace std;

class ExecuteProcessHelper
{
public:
	
	static BOOL  executeProcess(const wstring &applicationName, const wstring &commandLine);
	static DWORD executeProcessAndWait(const wstring &applicationName, const wstring &commandLine);
};

#endif