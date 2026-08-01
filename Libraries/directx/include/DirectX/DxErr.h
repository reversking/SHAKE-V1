







#ifndef _DXERR_H_
#define _DXERR_H_

#ifdef __cplusplus
extern "C" {
#endif











const char*  WINAPI DXGetErrorStringA(__in HRESULT hr);
const WCHAR* WINAPI DXGetErrorStringW(__in HRESULT hr);

#ifdef UNICODE
#define DXGetErrorString DXGetErrorStringW
#else
#define DXGetErrorString DXGetErrorStringA
#endif












const char*  WINAPI DXGetErrorDescriptionA(__in HRESULT hr);
const WCHAR* WINAPI DXGetErrorDescriptionW(__in HRESULT hr);

#ifdef UNICODE
    #define DXGetErrorDescription DXGetErrorDescriptionW
#else
    #define DXGetErrorDescription DXGetErrorDescriptionA
#endif

















HRESULT WINAPI DXTraceA( __in_z const char* strFile, __in DWORD dwLine, __in HRESULT hr, __in_z_opt const char* strMsg, __in BOOL bPopMsgBox );
HRESULT WINAPI DXTraceW( __in_z const char* strFile, __in DWORD dwLine, __in HRESULT hr, __in_z_opt const WCHAR* strMsg, __in BOOL bPopMsgBox );

#ifdef UNICODE
#define DXTrace DXTraceW
#else
#define DXTrace DXTraceA
#endif





#if defined(DEBUG) | defined(_DEBUG)
#define DXTRACE_MSG(str)              DXTrace( __FILE__, (DWORD)__LINE__, 0, str, FALSE )
#define DXTRACE_ERR(str,hr)           DXTrace( __FILE__, (DWORD)__LINE__, hr, str, FALSE )
#define DXTRACE_ERR_MSGBOX(str,hr)    DXTrace( __FILE__, (DWORD)__LINE__, hr, str, TRUE )
#else
#define DXTRACE_MSG(str)              (0L)
#define DXTRACE_ERR(str,hr)           (hr)
#define DXTRACE_ERR_MSGBOX(str,hr)    (hr)
#endif


#ifdef __cplusplus
}
#endif

#endif
