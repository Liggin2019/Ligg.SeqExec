#include <windows.h>
#include <string>


class SystemHelper
{
public:
	bool SystemHelper::IsProcessRunByAdmin();
	bool SystemHelper::IsCurrentUserAdmin();
	bool SystemHelper::IsUserInAdminGroup(WCHAR const *szUserName);
	BOOL SystemHelper::IsElevated( );
};