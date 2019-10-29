// See header file for copyright and license information
#include "ImpersonateUser.h"


//#pragma warning(push, 3)
//#pragma warning(disable : 4702)
ImpersonateUser::~ImpersonateUser()
{
	RevertToSelf();

	if(userToken_)
	{
		CloseHandle(userToken_); // Do not throw an exception here (as we are in the destructor)
		userToken_ = NULL;
	}

	init_ = false;
}
//#pragma warning(default : 4702)
//#pragma warning(pop)

/*lint -e534 -e818 */
bool ImpersonateUser::Logon(LPCSTR userName, LPCSTR domain, LPCSTR password)
{
	if(init_)
		Logoff();

	if(!userName) // Must at least specify a username
	{
		errorCode_ = ERROR_BAD_ARGUMENTS;
		return false;
	}

		// Attempt to log on as that user
	BOOL bLoggedOn = FALSE;

	//if(domain) // Domain name was specified
		bLoggedOn = LogonUser(userName, domain, password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &userToken_);
	//else
	//	bLoggedOn = LogonUser(userName, domain, password, LOGON32_LOGON_NEW_CREDENTIALS, LOGON32_PROVIDER_WINNT50, &userToken_);

	if(!bLoggedOn)
	{
		errorCode_ = GetLastError();
		return false;
	}

	 TCHAR szCurrentUserName[260] = {}; 
	  DWORD nSize = ARRAYSIZE(szCurrentUserName); 
	 GetUserName(szCurrentUserName, &nSize);

		 //Now impersonate them
	if(!ImpersonateLoggedOnUser(userToken_))
	{
		errorCode_ = GetLastError();
		return false;
	}

	 ZeroMemory(szCurrentUserName, sizeof(szCurrentUserName)); 
    nSize = ARRAYSIZE(szCurrentUserName); 
    GetUserName(szCurrentUserName, &nSize);
     
   
	//if(!DuplicateToken(userToken_, SecurityImpersonation,  &adminToken_))
	//{
	//	errorCode_ = GetLastError();
	//	return false;
	//}


	//WindowsIdentity fakeId = new WindowsIdentity(adminToken_);
 //   WindowsImpersonationContext _imperContext = fakeId.Impersonate();

	init_ = true;
	return true;
}

void ImpersonateUser::Logoff()
{
	if(!init_)
		return;

	RevertToSelf(); // Revert to our user

	if(userToken_)
	{
		if(!CloseHandle(userToken_))
			throw ("Impersonate::Logoff() - CloseHandle Failed");
		userToken_ = NULL;
	}

	init_ = false;
}
/*lint +e534 +e818 */
