// 
//   Copyright (c) Microsoft Corporation.  All rights reserved.
//
/// ==========================================================================
// Name    :    CError.cpp
// Purpose :    simple wrapper class for displaying (error) msgs

// includes
#include <windows.h>
#include <stdio.h>

#include "CError.h"
#include "CSettings.h"
#include "resource.h"
#include "..\Share\CommonDefine.h" 

// Constructors
// ==========================================================================
CError::CError()
: m_nRetCode(ERROR_SUCCESS),
  m_nMessage(0), 
  m_nCaption(0), 
  m_nIconType(0),
  m_pszArg(NULL)
{
    
}

// ==========================================================================
// CError::CError()
// Inputs:
//  UINT msgResId: resourceId for the message to display
//  UINT captionResId: resourceId for the caption to display
//  UINT iconType: icon to use
//  int returnCodeId: return code to be returned to the caller of the wrapper
// Purpose:
//  constructs CError object with initial values
// ==========================================================================
CError::CError( UINT captionResId, UINT msgResId,UINT iconType, int returnCodeId, LPCTSTR argStr)
: m_nMessage(msgResId), 
  m_nCaption(captionResId), 
  m_nIconType(iconType),
  m_nRetCode(returnCodeId),
  m_pszArg(argStr)
{

}

// Operations
// ==========================================================================
// CError::ShowMessage()
//
// Purpose:
//  displays messagebox w/resource-based content
// ==========================================================================
int CError::ShowMessage()
{
    TCHAR captionFmtStr[MAX_PATH+1]  = EMPTY_BUFFER;
    TCHAR msgFmtStr[MAX_MSG_STR_LEN+1]   = EMPTY_BUFFER;
    TCHAR captionStr[MAX_PATH+1] = EMPTY_BUFFER;
    TCHAR messageStr[MAX_MSG_STR_LEN+1]  = EMPTY_BUFFER;

    int responseId = IDCANCEL;

    // if a caption resource id has been specified, load it
    if (m_nCaption > 0)
    {
        ::LoadString(settings.GetHInstance(), 
                      m_nCaption, 
                      captionFmtStr, 
                      LENGTH(captionFmtStr) );

        _sntprintf(captionStr, 
                    LENGTH(captionStr) - 1, 
					//_T("%s"),
                    captionFmtStr
					);
    }
    else
    {
        _sntprintf(captionStr, 
                    LENGTH(captionStr) - 1, 
                    settings.CurMsgCaptionText//
					);
	}

	if (m_nMessage > 0)
	{
		::LoadString(settings.GetHInstance(), 
			m_nMessage, 
			msgFmtStr, 
			LENGTH(msgFmtStr) );
	}

	if ( NULL != m_pszArg )
	{
		if(strstr(msgFmtStr,_T("%s"))!=NULL)
		{
			_sntprintf(messageStr, 
				LENGTH(messageStr) - 1,
				//_T("%s %s"),
				msgFmtStr, 
				m_pszArg);
		}
		else
		{
			_sntprintf(messageStr, 
				LENGTH(messageStr) - 1,
				_T("%s %s"),
				msgFmtStr, 
				m_pszArg);
		}
	}
	else
	{
		_sntprintf( messageStr, 
			LENGTH(messageStr) - 1, 
			msgFmtStr);
	}


    // this sample does not currently support setting quiet mode via a cmd-line switch or another mechanism, 
    // but it could easily be modified to do so.
    if ( !settings.IsQuietMode)
    {
        responseId = ::MessageBox( NULL, 
                                  (LPCTSTR)messageStr, 
                                  captionStr, 
                                  MB_OK | m_nIconType ) ;
    }
    // else print message to stderr
    else
    {
        //_ftprintf ( stderr, (LPCTSTR)messageStr ) ;
		responseId = ::MessageBox( NULL, 
                                  (LPCTSTR)messageStr, 
                                  captionStr, 
                                  MB_OK | m_nIconType ) ;
    }

    // for quiet mode, we assume 'yes' as response
    if (settings.IsQuietMode)
    {
        responseId = IDOK;
    }

    return responseId;
}


 //==========================================================================
 //CError::ShowHResultMessage()

 //Inputs: none
 //Purpose:
 // displays messagebox w/formatted content, including HRESULT text
 //==========================================================================
int CError::ShowHResultMessage(UINT captionResId, 
							   UINT msgResId, 
                               UINT iconType, 
                               HRESULT hr, 
                               LPCTSTR argStr)
{
    TCHAR captionFmtStr[MAX_PATH+1]  = EMPTY_BUFFER;
    TCHAR msgFmtStr[MAX_MSG_STR_LEN+1]   = EMPTY_BUFFER;
    TCHAR captionStr[MAX_PATH+1] = EMPTY_BUFFER;
    TCHAR messageStr[MAX_MSG_STR_LEN+1]  = EMPTY_BUFFER;
    TCHAR szHResult[1024]     = EMPTY_BUFFER;

    LPVOID pMessage = NULL;

    UINT responseId = IDCANCEL;

    // if a caption resource id has been specified, load it
    if (captionResId > 0)
    {
        ::LoadString( settings.GetHInstance(), 
                      captionResId, 
                      captionFmtStr, 
                      LENGTH(captionFmtStr) );

        _sntprintf( captionStr, 
                    LENGTH(captionStr) - 1, 
                    captionFmtStr);
    }
    // otherwise use the global caption string
    else
    {
        _sntprintf( captionStr, 
                    LENGTH(captionStr) - 1, 
                    settings.CurMsgCaptionText);
    }


   FormatMessage( 
      FORMAT_MESSAGE_FROM_SYSTEM, 
      NULL,
      hr,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      szHResult,
	  LENGTH(szHResult),
	  NULL 
	  );

   ::LoadString( settings.GetHInstance(), 
	   msgResId, 
	   msgFmtStr, 
	   LENGTH(msgFmtStr));

   if (NULL == argStr)
   {
	   LPCTSTR args[1] = {szHResult};

	   FormatMessage( 
		   FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		   FORMAT_MESSAGE_FROM_STRING |
		   FORMAT_MESSAGE_ARGUMENT_ARRAY,
		   msgFmtStr,
		   0,
		   0,
		   (LPTSTR)&pMessage,
		   0,
		   (va_list*)args
		   );
   }
   else
    {
        LPCTSTR args[2] = {argStr, szHResult};
        FormatMessage( 
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_STRING |
            FORMAT_MESSAGE_ARGUMENT_ARRAY,
            msgFmtStr,
            0,
            0,
            (LPTSTR)&pMessage,
            0,
            (va_list*)args
        );
    }

    if ( !settings.IsQuietMode)
    {
        responseId = MessageBox( NULL, (LPCTSTR)pMessage,captionStr, iconType ) ;
    }
    // else print message to stderr
    else
    {
        _ftprintf ( stderr, (LPCTSTR)messageStr ) ;
    }

    // for quiet mode, we assume 'yes' as response
    if (settings.IsQuietMode)
    {
        responseId = IDOK;
    }

    LocalFree(pMessage);
 
    return responseId;
}