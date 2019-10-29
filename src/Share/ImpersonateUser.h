
#include <windows.h>
#include <string>

class ImpersonateUser
{
private:
	bool init_;
	HANDLE userToken_;
	HANDLE adminToken_;
	DWORD errorCode_;

public:
	ImpersonateUser() : init_(false), userToken_(NULL), errorCode_(0) {}
	~ImpersonateUser();

	bool Logon(LPCSTR userName, LPCSTR domain, LPCSTR password);
	void Logoff();

	// Misc Methods
	DWORD GetErrorCode() const { return errorCode_; }
};



