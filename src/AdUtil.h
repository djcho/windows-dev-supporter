#ifndef _ADUTIL_H_
#define _ADUTIL_H_

#include <Iads.h>
#include <Adshlp.h>
#include <atlbase.h>
#include "StringUtil.h"

#pragma comment(lib, "AdsiId.lib")
#pragma comment(lib, "ActiveDS.lib")

/**
*@class AdUtil
*
*@brief
*ADSI 프로그래밍에 사용되는 Helper Class
*
*@author Zuperman & SuKyoung.Chi
*@version 1.0.0.1
*@date 2008-06-13
*@warning
*@remark
*/
class AdUtil
{
public:
	AdUtil();
	virtual ~AdUtil();

	/**
	*@fn void setAccount(IN const tstring &userName, IN const tstring &password)
	*
	*@brief
	*LDAP Binding을 하기 위해 필요한 계정 정보를 설정한다.
	*도메인 로그온 시에는 도메인 사용자 권한으로 LDAP Binding이 가능하나 로컬 로그온 시에는
	*계정 정보가 필요하다.
	*
	*[in]const tstring* userName 사용자 계정명
	*[in]const tstring* password 암호
	*@return void
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*로컬 로그온 시 계정 정보에 추가적으로 DC IP 또는 DC 서버 이름을 setDC 함수를 호출하여 추가적으로 설정해야한다.
	*
	*/
	void setAccount(IN const tstring &userName, IN const tstring &password);

	/**
	*@fn void setDC(IN const tstring &domainController)
	*
	*@brief
	*로컬 로그온 시 LDAP Binding에 필요한 대상 DC 서버의 IP 또는 서버 정보를 설정한다.
	*
	*IN const tstring &domainController DC의 IP 또는 DC 서버명
	*@return void
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*로컬 로그온 시 계정 정보에 추가적으로 계정 정보를 SetAccount 함수를 호출하여 추가적으로 설정해야한다.
	*
	*/
	void setDC(IN const tstring &domainController);

	/**
	*@fn HRESULT getAdObjPath(IN const tstring &accountName, OUT tstring &adsPath)
	*
	*@brief
	*sAMAccountName을 이용하여 AD 상의 사용자 개체의 ADsPath 획득한다.
	*
	*IN const tstring &accountName 검색 기준이 되는 개체의 sAMAccountName
	*OUT tstring &adsPath 검색된 개체의 ADsPath
	*@return HRESULT 오류 코드
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*
	*/
	HRESULT getAdObjPath(IN const tstring &accountName, OUT tstring &adsPath);

	/**
	*@fn HRESULT authentification()
	*
	*@brief
	*AD에 사용자 인증을 받는다.
	*
	*@return HRESULT 오류 코드
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*사용자의 계정명과 암호를 통해 인증을 하고자 하는 경우 setAccountName 함수를 호출하여 계정 정보를 설정한다.
	*/
	HRESULT authentification();

	/**
	*@fn HRESULT changePassword(IN const tstring &accountName, IN const tstring &oldPassword, IN const tstring &newPassword)
	*
	*@brief
	*사용자의 암호를 변경한다.
	*
	*@return HRESULT 오류 코드
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*
	*/
	HRESULT changePassword(IN const tstring &accountName, IN const tstring &oldPassword, IN const tstring &newPassword);

	/**
	*@fn HRESULT setPassword(IN const tstring &accountName, IN const tstring &newPassword)
	*
	*@brief
	*사용자의 암호를 재설정한다. ( 기존 ChangePassword 코드와 동일)
	*
	*@return HRESULT 오류 코드
	*@remark
	*
	*@author hikim
	*@version 1.0.0.1
	*@date 2014-11-17
	*@warning
	*@remark
	*
	*/
	HRESULT setPassword(IN const tstring &accountName, IN const tstring &newPassword);

	/**
	*@fn HRESULT chkBinding()
	*
	*@brief
	*LDAP 바인딩을 체크한다.
	*
	*@return HRESULT 오류 코드
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*
	*/
	HRESULT chkBinding();

	/**
	*@fn HRESULT chkDomainUserInLocalAdmins(IN const tstring &domain, IN const tstring &accountName, OUT BOOL* isExist)
	*
	*@brief
	*도메인 사용자가 로컬 Administrators 그룹에 포함되어있는지 확인한다.
	*
	*IN const tstring &domain 계정의 도메인 (e.g. Feelanet)
	*IN const tstring &accountName 사용자 계정 이름
	*OUT BOOL* isExist 존재 유무
	*@return HRESULT 오류 코드
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*
	*/
	HRESULT chkDomainUserInLocalAdmins(IN const tstring &domain, IN const tstring &accountName, OUT BOOL* isExist);

	/**
	*@fn HRESULT addDomainUserInLocalAdmins(IN const tstring &domain, IN const tstring &accountName)
	*
	*@brief
	*로컬 Administrators 그룹에 도메인 계정을 포함시킨다.
	*
	*IN const tstring &domain 계정의 도메인 (e.g. PKI)
	*IN const tstring &accountName 사용자 계정 이름
	*@return HRESULT 오류 코드
	*@remark
	*
	*@author 정연욱
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*
	*/
	HRESULT addDomainUserInLocalAdmins(IN const tstring &domain, IN const tstring &accountName);

	/**
	*@fn HRESULT find(IN const tstring &filter, OUT BOOL* isExist)
	*
	*@brief
	*설정한 LDAP Query 문으로 개체가 존재하는지 검색한다.
	*
	*IN const tstring &filter LDAP Query 문 (e.g. (&(ObjectCategory=xxx)(cn=yyy)) )
	*OUT BOOL* isExist 검색 결과 존재 유무
	*@return HRESULT 오류 코드
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*
	*/
	HRESULT find(IN const tstring &filter, OUT BOOL* isExist);
	
	HRESULT getNetBios(IN const tstring &filter, OUT tstring &netBios);

	HRESULT setAdObjAttribute(IN const tstring &accountName, IN const tstring &attributeName, IN const VARIANT &varSetValue);

	HRESULT getAdObjAttribute(IN const tstring &accountName, IN const tstring &attributeName, OUT VARIANT* varGetValue);

	HRESULT getRootDseAttribute(IN const tstring &attributeName, OUT VARIANT* varGetValue);

private:

	HRESULT openRootDse(IADs **objrootDSE);
	VOID closeRootDse(IADs **objrootDSE);
	HRESULT getAdsUser(IN const tstring &adsPath, OUT IADsUser **adsUser);

	tstring	userName;
	tstring	password;
	tstring	domainController;
};
#endif