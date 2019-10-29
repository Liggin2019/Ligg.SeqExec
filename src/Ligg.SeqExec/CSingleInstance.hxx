// 
//   Copyright (c) Microsoft Corporation.  All rights reserved.
//
/// ==========================================================================
// Name    :    CSingleInstance.hxx
// Purpose :    Object for ensuring a single instance of the 
//              consumer application is executing

// includes
#include <windows.h>


// ==========================================================================
// CSingleInstance
//
// Purpose:
//  creates an object that will either detect or create a single instance
//  mutex. Mutex is freed when class goes out of scope. You are unable to 
//  create a default object.
// ==========================================================================
class CSingleInstance
{
public:
    CSingleInstance( const TCHAR* szMutexName ) : 
      m_hMutex( NULL ), 
      m_fFirstInstance( TRUE )
    {
        if( NULL == szMutexName )
        {
            _ASSERTE( !_T( "Error CSingleInstance ctor! NULL string passed to destructor." ) );
        }
        else
        {
            m_hMutex = CreateMutex( NULL, NULL, szMutexName );

            //check that the handle didn't already exist
            //
            if( ERROR_ALREADY_EXISTS == ::GetLastError() )
            {
                ::CloseHandle( m_hMutex );
                m_hMutex = NULL;

                m_fFirstInstance = FALSE;
            }
        }
    }
    ~CSingleInstance()
    {
        if( NULL != m_hMutex )
        {
            ::CloseHandle( m_hMutex );
            m_hMutex = NULL;
        }
    }

    //used to determine if there was already an instance
    // of this mutex in use
    //
    inline BOOL IsUnique( void ) { return m_fFirstInstance; }
    
    //used to determine if the CreateMutex call succeeded
    //
    inline BOOL IsHandleOK( void ) { return m_hMutex ? TRUE : FALSE; }

private:
    CSingleInstance( void )
    {
        //this is not used
    }
    HANDLE m_hMutex;
    BOOL   m_fFirstInstance;
};



