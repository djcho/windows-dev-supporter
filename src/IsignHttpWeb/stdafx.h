// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <tchar.h>
#include <map>
#include <string>

#pragma warning(disable : 4251)

#ifdef _UNICODE
typedef std::wstring tstring;
#define to_tstring std::to_wstring
#else
typedef std::string tstring;
#define to_tstring to_string
#endif

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

