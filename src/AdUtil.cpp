#include "ADUtil.h"

using namespace eewin_util;

AdUtil::AdUtil()
{
}

AdUtil::~AdUtil()
{
}

void AdUtil::setAccount(IN const tstring &userName, IN const tstring &password)
{
	this->userName = userName;
	this->password = password;
}

void AdUtil::setDC(IN const tstring &domainController)
{
	this->domainController = domainController;
}

HRESULT AdUtil::getAdObjPath(IN const tstring &accountName, OUT tstring &adsPath)
{
	CoInitialize(NULL);

	IADs *objRootDsePtr = NULL;
	HRESULT hr = S_OK;

	hr = this->openRootDse(&objRootDsePtr);

	if (SUCCEEDED(hr))
	{
		// 도메인 정보 획득 (DC=xxxx,DC=yyy)
		VARIANT var;

		hr = objRootDsePtr->Get(CComBSTR("defaultNamingContext"), &var);

		if (SUCCEEDED(hr))
		{
			// LDAP://DC=xxx,DC=yyy 형태로 구성하여 사용자 개체 검색을 하도록 함
			IDirectorySearch *pContainerToSearch = NULL;
			tstring dcPath = _T("LDAP://");

			if (this->domainController.length() > 0)
			{
				dcPath += this->domainController + _T("/");
			}

			dcPath += StringUtil::unicodeToTstring(var.bstrVal);

			if (this->userName.length() > 0)
			{
				hr = ADsOpenObject(StringUtil::tstringToUnicode(dcPath).c_str(),
					StringUtil::tstringToUnicode(this->userName).c_str(),
					StringUtil::tstringToUnicode(this->password).c_str(),
					ADS_SECURE_AUTHENTICATION,
					IID_IDirectorySearch,
					(void**)&pContainerToSearch);
			}
			else
			{
				hr = ADsOpenObject(StringUtil::tstringToUnicode(dcPath).c_str(),
					NULL,
					NULL,
					ADS_SECURE_AUTHENTICATION,
					IID_IDirectorySearch,
					(void**)&pContainerToSearch);
			}

			if (SUCCEEDED(hr))
			{
				tstring filter = _T("(&(objectCategory=user)(samAccountName=") + accountName + _T("))");
				WCHAR* wszAttr[] = { L"ADsPath" };
				ADS_SEARCH_HANDLE hSearch;
				DWORD dwCount = 1;

				hr = pContainerToSearch->ExecuteSearch(const_cast<wchar_t*>(StringUtil::tstringToUnicode(filter).c_str()),
					wszAttr, dwCount, &hSearch);

				if (SUCCEEDED(hr))
				{
					hr = pContainerToSearch->GetFirstRow(hSearch);

					if (SUCCEEDED(hr))
					{
						ADS_SEARCH_COLUMN col;

						hr = pContainerToSearch->GetColumn(hSearch, wszAttr[0], &col);

						if (SUCCEEDED(hr))
						{
							switch (col.dwADsType)
							{
							case ADSTYPE_CASE_IGNORE_STRING:
								adsPath = StringUtil::unicodeToTstring(col.pADsValues->CaseIgnoreString);
								break;
							}
							pContainerToSearch->FreeColumn(&col);
						}
					}
					pContainerToSearch->CloseSearchHandle(hSearch);
				}
			}
			if (pContainerToSearch) pContainerToSearch->Release();
			pContainerToSearch = NULL;
		}
	}

	this->closeRootDse(&objRootDsePtr);
	CoUninitialize();

	return hr;
}

HRESULT AdUtil::chkBinding()
{
	CoInitialize(NULL);

	IADs *pObject = NULL;
	HRESULT hr = S_OK;

	hr = this->openRootDse(&pObject);

	this->closeRootDse(&pObject);

	CoUninitialize();

	return hr;
}

HRESULT AdUtil::authentification()
{
	return chkBinding();
}

HRESULT AdUtil::changePassword(IN const tstring &accountName, IN const tstring &oldPassword, IN const tstring &newPassword)
{
	::CoInitialize(NULL);
	HRESULT hr = S_OK;
	tstring adsPath;
	bool isChanged = false;

	hr = this->getAdObjPath(accountName, adsPath);
	if (SUCCEEDED(hr))
	{
		IADsUser *pUser = NULL;

		hr = this->getAdsUser(adsPath, &pUser);
		if (SUCCEEDED(hr))
		{
			BSTR bstrOldPassword = ::SysAllocString(StringUtil::tstringToUnicode(oldPassword).c_str());
			BSTR bstrNewPassword = ::SysAllocString(StringUtil::tstringToUnicode(newPassword).c_str());
			if (bstrOldPassword == NULL || bstrNewPassword == NULL)
				return -1;

			hr = pUser->ChangePassword(bstrOldPassword, bstrNewPassword);
			if (SUCCEEDED(hr))
			{
				isChanged = true;
				hr = pUser->SetInfo();
			}

			::SysFreeString(bstrOldPassword);
			::SysFreeString(bstrNewPassword);
		}
		if (pUser) pUser->Release();
		pUser = NULL;
	}

	::CoUninitialize();

	if (!isChanged)
		return hr;

	return S_OK;
}

HRESULT AdUtil::setPassword(IN const tstring &accountName, IN const tstring &newPassword)
{
	::CoInitialize(NULL);

	HRESULT hr = S_OK;
	tstring adsPath;

	hr = this->getAdObjPath(accountName, adsPath);
	if (SUCCEEDED(hr))
	{
		IADsUser *pUser = NULL;

		hr = this->getAdsUser(adsPath, &pUser);

		if (SUCCEEDED(hr))
		{
			BSTR bstrNewPassword = ::SysAllocString(StringUtil::tstringToUnicode(newPassword).c_str());
			if (bstrNewPassword == NULL)
				return -1;

			hr = pUser->SetPassword(bstrNewPassword);

			if (SUCCEEDED(hr))
			{
				hr = pUser->SetInfo();
			}
			::SysFreeString(bstrNewPassword);
		}
		if (pUser) pUser->Release();
		pUser = NULL;
	}

	::CoUninitialize();

	return hr;
}

HRESULT AdUtil::chkDomainUserInLocalAdmins(IN const tstring &domain, IN const tstring &accountName, OUT BOOL* isExist)
{
	CoInitialize(NULL);

	IADsGroup* pGroup = NULL;
	HRESULT hr = S_OK;

	*isExist = FALSE;

	tstring groupPath = _T("WinNT://./Administrators");

	hr = ADsGetObject(StringUtil::tstringToUnicode(groupPath).c_str(), IID_IADsGroup, (void**)&pGroup);

	if (SUCCEEDED(hr))
	{
		VARIANT_BOOL vbExist;
		tstring objPath = _T("WinNT://") + domain + _T("/") + accountName;

		hr = pGroup->IsMember(CComBSTR(StringUtil::tstringToUnicode(objPath).c_str()), &vbExist);

		if (SUCCEEDED(hr))
		{
			if (vbExist == VARIANT_TRUE)
			{
				*isExist = TRUE;
			}
			else
			{
				*isExist = FALSE;
			}
		}
		else
		{
			*isExist = FALSE;
		}
	}

	if (pGroup)
		pGroup->Release();
	pGroup = NULL;

	CoUninitialize();

	return hr;
}


HRESULT AdUtil::addDomainUserInLocalAdmins(IN const tstring &domain, IN const tstring &accountName)
{
	CoInitialize(NULL);

	tstring groupPath = _T("WinNT://./Administrators");

	IADsGroup*  pGroup = NULL;
	HRESULT		hr = S_OK;

	hr = ADsGetObject(StringUtil::tstringToUnicode(groupPath).c_str(), IID_IADsGroup, (void**)&pGroup);

	if (SUCCEEDED(hr))
	{
		tstring objPath = _T("WinNT://") + domain + _T("/") + accountName;

		int i = 0;
		while (i < 30)
		{
			hr = pGroup->Add(CComBSTR(StringUtil::tstringToUnicode(objPath).c_str()));

			if (SUCCEEDED(hr))
			{
				//MessageBox( NULL, _T("계정 추가 성공"), _T("AddDomainUserInLocalAdmins"), MB_OK | MB_TASKMODAL | MB_ICONINFORMATION );
				break;
			}
			else
			{
				if (hr == 0x8007056bL)
				{
					//MessageBox( NULL, _T("AD에 계정이 존재하지 않습니다."), _T("AddDomainUserInLocalAdmins"), MB_OK | MB_TASKMODAL | MB_ICONINFORMATION );
					break;
				}

				if (hr == 0x80071392L || hr == 0x80070562L)
				{
					//MessageBox( NULL, _T("이미 계정이 Administrator 그룹의 멤버입니다."), _T("AddDomainUserInLocalAdmins"), MB_OK | MB_TASKMODAL | MB_ICONINFORMATION );
					hr = S_OK;
					break;
				}

				i++;
				Sleep(5000);
			}
		}
	}
	if (pGroup)
		pGroup->Release();
	pGroup = NULL;

	CoUninitialize();

	return hr;
}


HRESULT AdUtil::find(IN const tstring &filter, OUT BOOL* isExist)
{
	CoInitialize(NULL);

	IADs *pObject = NULL;
	HRESULT hr = S_OK;

	*isExist = FALSE;

	hr = this->openRootDse(&pObject);

	if (SUCCEEDED(hr))
	{
		// 도메인 정보 획득 (DC=xxxx,DC=yyy)
		VARIANT var;

		hr = pObject->Get(CComBSTR("defaultNamingContext"), &var);

		if (SUCCEEDED(hr))
		{
			// LDAP://DC=xxx,DC=yyy 형태로 구성하여 사용자 개체 검색을 하도록 함
			tstring dcPath = _T("LDAP://");
			IDirectorySearch *pContainerToSearch = NULL;

			if (this->domainController.length() > 0)
			{
				dcPath += this->domainController + _T("/");
			}

			dcPath += StringUtil::unicodeToTstring(var.bstrVal);

			if (this->userName.length() > 0)
			{
				hr = ADsOpenObject(StringUtil::tstringToUnicode(dcPath).c_str(),
					StringUtil::tstringToUnicode(this->userName).c_str(),
					StringUtil::tstringToUnicode(this->password).c_str(),
					ADS_SECURE_AUTHENTICATION,
					IID_IDirectorySearch,
					(void**)&pContainerToSearch);
			}
			else
			{
				hr = ADsOpenObject(StringUtil::tstringToUnicode(dcPath).c_str(), NULL,
					NULL,
					ADS_SECURE_AUTHENTICATION,
					IID_IDirectorySearch,
					(void**)&pContainerToSearch);
			}

			if (SUCCEEDED(hr))
			{
				WCHAR* pszAttr[] = { L"cn" };
				ADS_SEARCH_HANDLE hSearch;
				DWORD dwCount = 1;

				hr = pContainerToSearch->ExecuteSearch(const_cast<wchar_t *>(StringUtil::tstringToUnicode(filter).c_str()),
					pszAttr, dwCount, &hSearch);

				if (SUCCEEDED(hr))
				{
					hr = pContainerToSearch->GetFirstRow(hSearch);

					if (SUCCEEDED(hr))
					{
						ADS_SEARCH_COLUMN col;

						hr = pContainerToSearch->GetColumn(hSearch, pszAttr[0], &col);

						if (SUCCEEDED(hr))
						{
							*isExist = TRUE;

							pContainerToSearch->FreeColumn(&col);
						}
					}

					pContainerToSearch->CloseSearchHandle(hSearch);
				}

			}
			if (pContainerToSearch)
				pContainerToSearch->Release();
			pContainerToSearch = NULL;
		}
	}

	if (pObject)
		pObject->Release();
	pObject = NULL;

	CoUninitialize();

	return hr;
}
HRESULT AdUtil::getNetBios(IN const tstring &filter, OUT tstring &netBios)
{
	CoInitialize(NULL);

	IADs *pObject = NULL;
	HRESULT hr = S_OK;
	//WCHAR ErrMsg[1024] = { 0, };

	hr = this->openRootDse(&pObject);

	if (SUCCEEDED(hr))
	{
		// 도메인 정보 획득 (DC=xxxx,DC=yyy)
		VARIANT var;

		hr = pObject->Get(CComBSTR("configurationNamingContext"), &var);

		if (SUCCEEDED(hr))
		{
			// LDAP://DC=xxx,DC=yyy 형태로 구성하여 사용자 개체 검색을 하도록 함
			tstring dcPath = _T("LDAP://");
			IDirectorySearch *pContainerToSearch = NULL;

			if (this->domainController.length() > 0)
			{
				dcPath += this->domainController + _T("/");
			}

			dcPath += _T("cn=Partitions,") + StringUtil::unicodeToTstring(var.bstrVal);

			if (this->userName.length() > 0)
			{
				hr = ADsOpenObject(StringUtil::tstringToUnicode(dcPath).c_str(),
					StringUtil::tstringToUnicode(this->userName).c_str(),
					StringUtil::tstringToUnicode(this->password).c_str(),
					ADS_SECURE_AUTHENTICATION,
					IID_IDirectorySearch,
					(void**)&pContainerToSearch);
			}
			else
			{
				hr = ADsOpenObject(StringUtil::tstringToUnicode(dcPath).c_str(),
					NULL,
					NULL,
					ADS_SECURE_AUTHENTICATION,
					IID_IDirectorySearch,
					(void**)&pContainerToSearch);
			}

			if (SUCCEEDED(hr))
			{
				WCHAR* pszAttr[] = { L"netbiosname" };
				DWORD dwCount = 1;
				ADS_SEARCH_HANDLE hSearch;

				hr = pContainerToSearch->ExecuteSearch(const_cast<wchar_t *>(StringUtil::tstringToUnicode(filter).c_str()),
					pszAttr, dwCount, &hSearch);

				if (SUCCEEDED(hr))
				{
					hr = pContainerToSearch->GetFirstRow(hSearch);

					if (SUCCEEDED(hr))
					{
						ADS_SEARCH_COLUMN col;

						hr = pContainerToSearch->GetColumn(hSearch, pszAttr[0], &col);

						if (SUCCEEDED(hr))
						{
							switch (col.dwADsType)
							{
							case ADSTYPE_CASE_IGNORE_STRING:

								//MessageBoxW(NULL, col.pADsValues->CaseIgnoreString, L"", MB_OK);
								netBios = StringUtil::unicodeToTstring(col.pADsValues->CaseIgnoreString);
								break;

							case ADSTYPE_PROV_SPECIFIC:
								//MessageBoxW(NULL, (PWCHAR)col.pADsValues->ProviderSpecific.lpValue, L"", MB_OK);
								break;

							default:
								//swprintf(ErrMsg, L"Unexpected ADsType: %d", col.dwADsType);
								//MessageBoxW(NULL, ErrMsg, L"", MB_OK);
								break;
							}
							pContainerToSearch->FreeColumn(&col);
						}
					}
					pContainerToSearch->CloseSearchHandle(hSearch);
				}
			}
			if (pContainerToSearch) pContainerToSearch->Release();
			pContainerToSearch = NULL;
		}
	}

	closeRootDse(&pObject);
	CoUninitialize();

	return hr;
}

HRESULT AdUtil::setAdObjAttribute(IN const tstring &accountName, IN const tstring &attributeName, IN const VARIANT &varSetValue)
{
	CoInitialize(NULL);

	
	HRESULT hr = S_OK;
	IADs* pObject = NULL;

	tstring adsPath;

	hr = getAdObjPath(accountName, adsPath);

	if (SUCCEEDED(hr))
	{
		if (this->userName.length() > 0)
		{
			hr = ADsOpenObject(StringUtil::tstringToUnicode(adsPath).c_str(),
				StringUtil::tstringToUnicode(this->userName).c_str(),
				StringUtil::tstringToUnicode(this->password).c_str(),
				ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
				IID_IADs,
				(void**)&pObject);
		}
		else
		{
			hr = ADsOpenObject(StringUtil::tstringToUnicode(adsPath).c_str(),
				NULL,
				NULL,
				ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
				IID_IADs,
				(void**)&pObject);
		}
		
		if (SUCCEEDED(hr))
		{
			BSTR bstrAttributeName = ::SysAllocString(StringUtil::tstringToUnicode(attributeName).c_str());
			if (bstrAttributeName == NULL)
				return -1;

			hr = pObject->Put(bstrAttributeName, varSetValue);			

			if (SUCCEEDED(hr))
			{
				hr = pObject->SetInfo();
			}
			::SysFreeString(bstrAttributeName);
		}
	}

	if (pObject) pObject->Release();
	pObject = NULL;

	CoUninitialize();
	return hr;
}

HRESULT AdUtil::getAdObjAttribute(IN const tstring &accountName, IN const tstring &attributeName, OUT VARIANT* varGetValue)
{
	CoInitialize(NULL);

	HRESULT hr = S_OK;
	IADs* pObject = NULL;
	
	tstring adsPath;

	hr = getAdObjPath(accountName, adsPath);

	if (SUCCEEDED(hr))
	{
		if (this->userName.length() > 0)
		{
			hr = ADsOpenObject(StringUtil::tstringToUnicode(adsPath).c_str(),
				StringUtil::tstringToUnicode(this->userName).c_str(),
				StringUtil::tstringToUnicode(this->password).c_str(),
				ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
				IID_IADs,
				(void**)&pObject);
		}
		else
		{
			hr = ADsOpenObject(StringUtil::tstringToUnicode(adsPath).c_str(),
				NULL,
				NULL,
				ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
				IID_IADs,
				(void**)&pObject);
		}

		if (SUCCEEDED(hr))
		{
			BSTR bstrAttributeName = ::SysAllocString(StringUtil::tstringToUnicode(attributeName).c_str());
			if (bstrAttributeName == NULL)
				return -1;

			hr = pObject->Get(bstrAttributeName, varGetValue);
			::SysFreeString(bstrAttributeName);
		}
	}

	if (pObject) pObject->Release();
	pObject = NULL;

	CoUninitialize();
	return hr;
}

HRESULT AdUtil::getRootDseAttribute(IN const tstring &attributeName, OUT VARIANT* varGetValue)
{
	CoInitialize(NULL);

	HRESULT hr = S_OK;
	IADs* pObject = NULL;

	hr = ADsOpenObject(L"LDAP://rootDSE",
		NULL,
		NULL,
		ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
		IID_IADs,
		(void**)&pObject);

	if (SUCCEEDED(hr))
	{
		BSTR bstrAttributeName = ::SysAllocString(StringUtil::tstringToUnicode(attributeName).c_str());
		if (bstrAttributeName == NULL)
			return -1;

		hr = pObject->Get(bstrAttributeName, varGetValue);
		::SysFreeString(bstrAttributeName);
	}

	if (pObject) pObject->Release();
	pObject = NULL;

	CoUninitialize();
	return hr;

}

HRESULT AdUtil::openRootDse(IADs ** objRootDse)
{
	HRESULT hr = S_OK;
	tstring rootDsePath = _T("LDAP://");

	// rootDSE LDAP Path 구성
	if (this->domainController.length() > 0)
	{
		rootDsePath += this->domainController + _T("/");
	}

	rootDsePath += _T("rootDSE");
	// rootDSE에 LDAP Binding
	if (this->userName.length() > 0)
	{
		hr = ADsOpenObject(StringUtil::tstringToUnicode(rootDsePath).c_str(),
			StringUtil::tstringToUnicode(this->userName).c_str(),
			StringUtil::tstringToUnicode(this->password).c_str(),
			ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
			IID_IADs,
			(void**)objRootDse);
	}
	else
	{
		hr = ADsOpenObject(StringUtil::tstringToUnicode(rootDsePath).c_str(),
			NULL,
			NULL,
			ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
			IID_IADs,
			(void**)objRootDse);
	}

	return hr;
}

VOID AdUtil::closeRootDse(IADs ** objRootDse)
{
	if (*objRootDse)
		(*objRootDse)->Release();

	*objRootDse = NULL;
}

HRESULT AdUtil::getAdsUser(IN const tstring & adsPath, OUT IADsUser ** adsUser)
{
	HRESULT hr = 0;

	if (this->userName.length() > 0)
	{
		hr = ADsOpenObject(StringUtil::tstringToUnicode(adsPath).c_str(),
			StringUtil::tstringToUnicode(this->userName).c_str(),
			StringUtil::tstringToUnicode(this->password).c_str(),
			ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
			IID_IADsUser,
			(void**)adsUser);
	}
	else
	{
		hr = ADsOpenObject(StringUtil::tstringToUnicode(adsPath).c_str(),
			NULL,
			NULL,
			ADS_SECURE_AUTHENTICATION, //Use Secure Authentication
			IID_IADsUser,
			(void**)adsUser);
	}
	return hr;
}
