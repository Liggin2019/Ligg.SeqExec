#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <wtypes.h>
#include <Lm.h>
#include <crtdbg.h>
#include <assert.h>
#pragma comment(lib, "netapi32.lib")

#include "SystemHelper.h"

bool SystemHelper::IsProcessRunByAdmin()
{
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;

	BOOL b = AllocateAndInitializeSid(&NtAuthority,2,SECURITY_BUILTIN_DOMAIN_RID,DOMAIN_ALIAS_RID_ADMINS,0, 0, 0, 0, 0, 0,&AdministratorsGroup);

	if(b)
	{
		CheckTokenMembership(NULL, AdministratorsGroup, &b);
		FreeSid(AdministratorsGroup);
	}
	return b == TRUE;  
}

//no use
bool SystemHelper::IsCurrentUserAdmin() //判断是否管理员模式
{
	BOOL bIsElevated = FALSE;
	HANDLE hToken = NULL;
	UINT16 uWinVer = LOWORD(GetVersion());
	uWinVer = MAKEWORD(HIBYTE(uWinVer),LOBYTE(uWinVer));

	if (uWinVer < 0x0600)//不是VISTA、Windows7
		return(FALSE);

	if (OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken)) {

		struct {
			DWORD TokenIsElevated;
		} /*TOKEN_ELEVATION*/te;
		DWORD dwReturnLength = 0;

		if (GetTokenInformation(hToken,/*TokenElevation*/(_TOKEN_INFORMATION_CLASS)20,&te,sizeof(te),&dwReturnLength)) {
			if (dwReturnLength == sizeof(te))
				bIsElevated = te.TokenIsElevated;
		}
		CloseHandle( hToken );
	}
	return bIsElevated;
}

bool SystemHelper::IsUserInAdminGroup(WCHAR const *szUserName) 
{
	_ASSERT(szUserName);
	bool bAdmin = FALSE;
	LOCALGROUP_USERS_INFO_0* localGroups;
	DWORD entriesread, totalentries;
	NET_API_STATUS nts = NetUserGetLocalGroups(NULL, szUserName, 0, 0, (unsigned char**)&localGroups, MAX_PREFERRED_LENGTH, &entriesread, &totalentries);

	if( nts != NERR_Success ) {
		NetApiBufferFree(localGroups);
		return FALSE;
	}
	// Retrieve the Administrators group well-known SID
	// For some reason CreateWellKnownSid generates error C3861 on Developer Studio .NET:
	// error C3861: 'CreateWellKnownSid': identifier not found, even with argument-dependent lookup
	BYTE    SidAuth[] = SECURITY_NT_AUTHORITY;
	PSID    pAdminSid;
	AllocateAndInitializeSid( (PSID_IDENTIFIER_AUTHORITY)SidAuth, 
		2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 
		NULL, NULL, NULL, NULL, NULL, NULL, &pAdminSid );

	// Will use this to retrieve the SID of the group
	BYTE    buffSid[SECURITY_MAX_SID_SIZE];
	wchar_t    buffDomain[DNLEN+1];
	DWORD    dwSidSize;
	DWORD    dwDomainSize;
	SID_NAME_USE m_sidnameuse;
	for( DWORD i = 0; i < entriesread; ++i ) {
		dwSidSize = sizeof(buffSid);
		dwDomainSize = DNLEN;

		// Although in general is a bad idea to call directly the W or A versions of API
		// we do it here to avoid converting the localGroups[i].lgrui0_name back to ANSI
		// This kind of security API is only present on NT/2000/XP family only, so
		// the W version is present and safe to use
		if( LookupAccountNameW( NULL, localGroups[i].lgrui0_name, buffSid, &dwSidSize, (LPWSTR)buffDomain, &dwDomainSize, &m_sidnameuse) ) // no sid for the actual group
			if( EqualSid( buffSid, pAdminSid ) ) {
				bAdmin = TRUE;
				break;
			}
	}
	FreeSid( pAdminSid );
	NetApiBufferFree(localGroups);

	return bAdmin;
}

BOOL SystemHelper::IsElevated( ) {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if( OpenProcessToken( GetCurrentProcess( ),TOKEN_QUERY,&hToken ) ) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof( TOKEN_ELEVATION );
        if( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof( Elevation ), &cbSize ) ) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if( hToken ) {
        CloseHandle( hToken );
    }
    return fRet;
}

	









