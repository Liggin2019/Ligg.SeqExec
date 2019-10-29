using namespace std;

std::string FormatSysError(DWORD lastErrorDw, LPCTSTR errStrPt)
{
	LPVOID msgBufLp;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, lastErrorDw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msgBufLp, 0, NULL);

	std::string errStr;
	if(errStrPt != NULL)
		errStr = errStrPt;

	errStr += (LPTSTR)msgBufLp;

	LocalFree(msgBufLp);
	return errStr;
}