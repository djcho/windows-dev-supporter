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
*ADSI ���α׷��ֿ� ���Ǵ� Helper Class
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
	*LDAP Binding�� �ϱ� ���� �ʿ��� ���� ������ �����Ѵ�.
	*������ �α׿� �ÿ��� ������ ����� �������� LDAP Binding�� �����ϳ� ���� �α׿� �ÿ���
	*���� ������ �ʿ��ϴ�.
	*
	*[in]const tstring* userName ����� ������
	*[in]const tstring* password ��ȣ
	*@return void
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*���� �α׿� �� ���� ������ �߰������� DC IP �Ǵ� DC ���� �̸��� setDC �Լ��� ȣ���Ͽ� �߰������� �����ؾ��Ѵ�.
	*
	*/
	void setAccount(IN const tstring &userName, IN const tstring &password);

	/**
	*@fn void setDC(IN const tstring &domainController)
	*
	*@brief
	*���� �α׿� �� LDAP Binding�� �ʿ��� ��� DC ������ IP �Ǵ� ���� ������ �����Ѵ�.
	*
	*IN const tstring &domainController DC�� IP �Ǵ� DC ������
	*@return void
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*���� �α׿� �� ���� ������ �߰������� ���� ������ SetAccount �Լ��� ȣ���Ͽ� �߰������� �����ؾ��Ѵ�.
	*
	*/
	void setDC(IN const tstring &domainController);

	/**
	*@fn HRESULT getAdObjPath(IN const tstring &accountName, OUT tstring &adsPath)
	*
	*@brief
	*sAMAccountName�� �̿��Ͽ� AD ���� ����� ��ü�� ADsPath ȹ���Ѵ�.
	*
	*IN const tstring &accountName �˻� ������ �Ǵ� ��ü�� sAMAccountName
	*OUT tstring &adsPath �˻��� ��ü�� ADsPath
	*@return HRESULT ���� �ڵ�
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
	*AD�� ����� ������ �޴´�.
	*
	*@return HRESULT ���� �ڵ�
	*@remark
	*
	*@author Zuperman & SuKyoung.Chi
	*@version 1.0.0.1
	*@date 2008-06-13
	*@warning
	*@remark
	*������� ������� ��ȣ�� ���� ������ �ϰ��� �ϴ� ��� setAccountName �Լ��� ȣ���Ͽ� ���� ������ �����Ѵ�.
	*/
	HRESULT authentification();

	/**
	*@fn HRESULT changePassword(IN const tstring &accountName, IN const tstring &oldPassword, IN const tstring &newPassword)
	*
	*@brief
	*������� ��ȣ�� �����Ѵ�.
	*
	*@return HRESULT ���� �ڵ�
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
	*������� ��ȣ�� �缳���Ѵ�. ( ���� ChangePassword �ڵ�� ����)
	*
	*@return HRESULT ���� �ڵ�
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
	*LDAP ���ε��� üũ�Ѵ�.
	*
	*@return HRESULT ���� �ڵ�
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
	*������ ����ڰ� ���� Administrators �׷쿡 ���ԵǾ��ִ��� Ȯ���Ѵ�.
	*
	*IN const tstring &domain ������ ������ (e.g. Feelanet)
	*IN const tstring &accountName ����� ���� �̸�
	*OUT BOOL* isExist ���� ����
	*@return HRESULT ���� �ڵ�
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
	*���� Administrators �׷쿡 ������ ������ ���Խ�Ų��.
	*
	*IN const tstring &domain ������ ������ (e.g. PKI)
	*IN const tstring &accountName ����� ���� �̸�
	*@return HRESULT ���� �ڵ�
	*@remark
	*
	*@author ������
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
	*������ LDAP Query ������ ��ü�� �����ϴ��� �˻��Ѵ�.
	*
	*IN const tstring &filter LDAP Query �� (e.g. (&(ObjectCategory=xxx)(cn=yyy)) )
	*OUT BOOL* isExist �˻� ��� ���� ����
	*@return HRESULT ���� �ڵ�
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