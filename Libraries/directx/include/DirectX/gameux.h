















#pragma warning( disable: 4049 )



#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif


#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif

#ifndef __gameux_h__
#define __gameux_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif



#ifndef __IGameExplorer_FWD_DEFINED__
#define __IGameExplorer_FWD_DEFINED__
typedef interface IGameExplorer IGameExplorer;
#endif


#ifndef __IGameStatistics_FWD_DEFINED__
#define __IGameStatistics_FWD_DEFINED__
typedef interface IGameStatistics IGameStatistics;
#endif


#ifndef __IGameStatisticsMgr_FWD_DEFINED__
#define __IGameStatisticsMgr_FWD_DEFINED__
typedef interface IGameStatisticsMgr IGameStatisticsMgr;
#endif


#ifndef __IGameExplorer2_FWD_DEFINED__
#define __IGameExplorer2_FWD_DEFINED__
typedef interface IGameExplorer2 IGameExplorer2;
#endif


#ifndef __GameExplorer_FWD_DEFINED__
#define __GameExplorer_FWD_DEFINED__

#ifdef __cplusplus
typedef class GameExplorer GameExplorer;
#else
typedef struct GameExplorer GameExplorer;
#endif

#endif


#ifndef __GameStatistics_FWD_DEFINED__
#define __GameStatistics_FWD_DEFINED__

#ifdef __cplusplus
typedef class GameStatistics GameStatistics;
#else
typedef struct GameStatistics GameStatistics;
#endif

#endif



#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif





#define ID_GDF_XML __GDF_XML
#define ID_GDF_THUMBNAIL __GDF_THUMBNAIL
#define ID_ICON_ICO __ICON_ICO
#define ID_GDF_XML_STR L"__GDF_XML"
#define ID_GDF_THUMBNAIL_STR L"__GDF_THUMBNAIL"
typedef
enum GAME_INSTALL_SCOPE
    {	GIS_NOT_INSTALLED	= 1,
	GIS_CURRENT_USER	= 2,
	GIS_ALL_USERS	= 3
    } 	GAME_INSTALL_SCOPE;



extern RPC_IF_HANDLE __MIDL_itf_gameux_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_gameux_0000_0000_v0_0_s_ifspec;

#ifndef __IGameExplorer_INTERFACE_DEFINED__
#define __IGameExplorer_INTERFACE_DEFINED__





EXTERN_C const IID IID_IGameExplorer;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E7B2FB72-D728-49B3-A5F2-18EBF5F1349E")
    IGameExplorer : public IUnknown
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE AddGame(
             __RPC__in BSTR bstrGDFBinaryPath,
             __RPC__in BSTR bstrGameInstallDirectory,
             GAME_INSTALL_SCOPE installScope,
             __RPC__inout GUID *pguidInstanceID) = 0;

        virtual  HRESULT STDMETHODCALLTYPE RemoveGame(
             GUID guidInstanceID) = 0;

        virtual  HRESULT STDMETHODCALLTYPE UpdateGame(
             GUID guidInstanceID) = 0;

        virtual  HRESULT STDMETHODCALLTYPE VerifyAccess(
             __RPC__in BSTR bstrGDFBinaryPath,
             __RPC__out BOOL *pfHasAccess) = 0;

    };

#else

    typedef struct IGameExplorerVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
            __RPC__in IGameExplorer * This,
             __RPC__in REFIID riid,

            __RPC__deref_out  void **ppvObject);

        ULONG ( STDMETHODCALLTYPE *AddRef )(
            __RPC__in IGameExplorer * This);

        ULONG ( STDMETHODCALLTYPE *Release )(
            __RPC__in IGameExplorer * This);

         HRESULT ( STDMETHODCALLTYPE *AddGame )(
            __RPC__in IGameExplorer * This,
             __RPC__in BSTR bstrGDFBinaryPath,
             __RPC__in BSTR bstrGameInstallDirectory,
             GAME_INSTALL_SCOPE installScope,
             __RPC__inout GUID *pguidInstanceID);

         HRESULT ( STDMETHODCALLTYPE *RemoveGame )(
            __RPC__in IGameExplorer * This,
             GUID guidInstanceID);

         HRESULT ( STDMETHODCALLTYPE *UpdateGame )(
            __RPC__in IGameExplorer * This,
             GUID guidInstanceID);

         HRESULT ( STDMETHODCALLTYPE *VerifyAccess )(
            __RPC__in IGameExplorer * This,
             __RPC__in BSTR bstrGDFBinaryPath,
             __RPC__out BOOL *pfHasAccess);

        END_INTERFACE
    } IGameExplorerVtbl;

    interface IGameExplorer
    {
        CONST_VTBL struct IGameExplorerVtbl *lpVtbl;
    };



#ifdef COBJMACROS


#define IGameExplorer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )

#define IGameExplorer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) )

#define IGameExplorer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) )


#define IGameExplorer_AddGame(This,bstrGDFBinaryPath,bstrGameInstallDirectory,installScope,pguidInstanceID)	\
    ( (This)->lpVtbl -> AddGame(This,bstrGDFBinaryPath,bstrGameInstallDirectory,installScope,pguidInstanceID) )

#define IGameExplorer_RemoveGame(This,guidInstanceID)	\
    ( (This)->lpVtbl -> RemoveGame(This,guidInstanceID) )

#define IGameExplorer_UpdateGame(This,guidInstanceID)	\
    ( (This)->lpVtbl -> UpdateGame(This,guidInstanceID) )

#define IGameExplorer_VerifyAccess(This,bstrGDFBinaryPath,pfHasAccess)	\
    ( (This)->lpVtbl -> VerifyAccess(This,bstrGDFBinaryPath,pfHasAccess) )

#endif


#endif




#endif





typedef
enum GAMESTATS_OPEN_TYPE
    {	GAMESTATS_OPEN_OPENORCREATE	= 0,
	GAMESTATS_OPEN_OPENONLY	= 1
    } 	GAMESTATS_OPEN_TYPE;

typedef
enum GAMESTATS_OPEN_RESULT
    {	GAMESTATS_OPEN_CREATED	= 0,
	GAMESTATS_OPEN_OPENED	= 1
    } 	GAMESTATS_OPEN_RESULT;



extern RPC_IF_HANDLE __MIDL_itf_gameux_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_gameux_0000_0001_v0_0_s_ifspec;

#ifndef __IGameStatistics_INTERFACE_DEFINED__
#define __IGameStatistics_INTERFACE_DEFINED__





EXTERN_C const IID IID_IGameStatistics;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3887C9CA-04A0-42ae-BC4C-5FA6C7721145")
    IGameStatistics : public IUnknown
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE GetMaxCategoryLength(
             __RPC__out UINT *cch) = 0;

        virtual  HRESULT STDMETHODCALLTYPE GetMaxNameLength(
             __RPC__out UINT *cch) = 0;

        virtual  HRESULT STDMETHODCALLTYPE GetMaxValueLength(
             __RPC__out UINT *cch) = 0;

        virtual  HRESULT STDMETHODCALLTYPE GetMaxCategories(
             __RPC__out WORD *pMax) = 0;

        virtual  HRESULT STDMETHODCALLTYPE GetMaxStatsPerCategory(
             __RPC__out WORD *pMax) = 0;

        virtual  HRESULT STDMETHODCALLTYPE SetCategoryTitle(
             WORD categoryIndex,
             __RPC__in_string LPCWSTR title) = 0;

        virtual  HRESULT STDMETHODCALLTYPE GetCategoryTitle(
             WORD categoryIndex,
             __RPC__deref_out_opt_string LPWSTR *pTitle) = 0;

        virtual  HRESULT STDMETHODCALLTYPE GetStatistic(
             WORD categoryIndex,
             WORD statIndex,
             __RPC__deref_opt_inout_opt_string LPWSTR *pName,
             __RPC__deref_opt_inout_opt_string LPWSTR *pValue) = 0;

        virtual  HRESULT STDMETHODCALLTYPE SetStatistic(
             WORD categoryIndex,
             WORD statIndex,
             __RPC__in_string LPCWSTR name,
             __RPC__in_string LPCWSTR value) = 0;

        virtual  HRESULT STDMETHODCALLTYPE Save(
             BOOL trackChanges) = 0;

        virtual  HRESULT STDMETHODCALLTYPE SetLastPlayedCategory(
             UINT categoryIndex) = 0;

        virtual  HRESULT STDMETHODCALLTYPE GetLastPlayedCategory(
             __RPC__out UINT *pCategoryIndex) = 0;

    };

#else

    typedef struct IGameStatisticsVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
            __RPC__in IGameStatistics * This,
             __RPC__in REFIID riid,

            __RPC__deref_out  void **ppvObject);

        ULONG ( STDMETHODCALLTYPE *AddRef )(
            __RPC__in IGameStatistics * This);

        ULONG ( STDMETHODCALLTYPE *Release )(
            __RPC__in IGameStatistics * This);

         HRESULT ( STDMETHODCALLTYPE *GetMaxCategoryLength )(
            __RPC__in IGameStatistics * This,
             __RPC__out UINT *cch);

         HRESULT ( STDMETHODCALLTYPE *GetMaxNameLength )(
            __RPC__in IGameStatistics * This,
             __RPC__out UINT *cch);

         HRESULT ( STDMETHODCALLTYPE *GetMaxValueLength )(
            __RPC__in IGameStatistics * This,
             __RPC__out UINT *cch);

         HRESULT ( STDMETHODCALLTYPE *GetMaxCategories )(
            __RPC__in IGameStatistics * This,
             __RPC__out WORD *pMax);

         HRESULT ( STDMETHODCALLTYPE *GetMaxStatsPerCategory )(
            __RPC__in IGameStatistics * This,
             __RPC__out WORD *pMax);

         HRESULT ( STDMETHODCALLTYPE *SetCategoryTitle )(
            __RPC__in IGameStatistics * This,
             WORD categoryIndex,
             __RPC__in_string LPCWSTR title);

         HRESULT ( STDMETHODCALLTYPE *GetCategoryTitle )(
            __RPC__in IGameStatistics * This,
             WORD categoryIndex,
             __RPC__deref_out_opt_string LPWSTR *pTitle);

         HRESULT ( STDMETHODCALLTYPE *GetStatistic )(
            __RPC__in IGameStatistics * This,
             WORD categoryIndex,
             WORD statIndex,
             __RPC__deref_opt_inout_opt_string LPWSTR *pName,
             __RPC__deref_opt_inout_opt_string LPWSTR *pValue);

         HRESULT ( STDMETHODCALLTYPE *SetStatistic )(
            __RPC__in IGameStatistics * This,
             WORD categoryIndex,
             WORD statIndex,
             __RPC__in_string LPCWSTR name,
             __RPC__in_string LPCWSTR value);

         HRESULT ( STDMETHODCALLTYPE *Save )(
            __RPC__in IGameStatistics * This,
             BOOL trackChanges);

         HRESULT ( STDMETHODCALLTYPE *SetLastPlayedCategory )(
            __RPC__in IGameStatistics * This,
             UINT categoryIndex);

         HRESULT ( STDMETHODCALLTYPE *GetLastPlayedCategory )(
            __RPC__in IGameStatistics * This,
             __RPC__out UINT *pCategoryIndex);

        END_INTERFACE
    } IGameStatisticsVtbl;

    interface IGameStatistics
    {
        CONST_VTBL struct IGameStatisticsVtbl *lpVtbl;
    };



#ifdef COBJMACROS


#define IGameStatistics_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )

#define IGameStatistics_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) )

#define IGameStatistics_Release(This)	\
    ( (This)->lpVtbl -> Release(This) )


#define IGameStatistics_GetMaxCategoryLength(This,cch)	\
    ( (This)->lpVtbl -> GetMaxCategoryLength(This,cch) )

#define IGameStatistics_GetMaxNameLength(This,cch)	\
    ( (This)->lpVtbl -> GetMaxNameLength(This,cch) )

#define IGameStatistics_GetMaxValueLength(This,cch)	\
    ( (This)->lpVtbl -> GetMaxValueLength(This,cch) )

#define IGameStatistics_GetMaxCategories(This,pMax)	\
    ( (This)->lpVtbl -> GetMaxCategories(This,pMax) )

#define IGameStatistics_GetMaxStatsPerCategory(This,pMax)	\
    ( (This)->lpVtbl -> GetMaxStatsPerCategory(This,pMax) )

#define IGameStatistics_SetCategoryTitle(This,categoryIndex,title)	\
    ( (This)->lpVtbl -> SetCategoryTitle(This,categoryIndex,title) )

#define IGameStatistics_GetCategoryTitle(This,categoryIndex,pTitle)	\
    ( (This)->lpVtbl -> GetCategoryTitle(This,categoryIndex,pTitle) )

#define IGameStatistics_GetStatistic(This,categoryIndex,statIndex,pName,pValue)	\
    ( (This)->lpVtbl -> GetStatistic(This,categoryIndex,statIndex,pName,pValue) )

#define IGameStatistics_SetStatistic(This,categoryIndex,statIndex,name,value)	\
    ( (This)->lpVtbl -> SetStatistic(This,categoryIndex,statIndex,name,value) )

#define IGameStatistics_Save(This,trackChanges)	\
    ( (This)->lpVtbl -> Save(This,trackChanges) )

#define IGameStatistics_SetLastPlayedCategory(This,categoryIndex)	\
    ( (This)->lpVtbl -> SetLastPlayedCategory(This,categoryIndex) )

#define IGameStatistics_GetLastPlayedCategory(This,pCategoryIndex)	\
    ( (This)->lpVtbl -> GetLastPlayedCategory(This,pCategoryIndex) )

#endif


#endif




#endif


#ifndef __IGameStatisticsMgr_INTERFACE_DEFINED__
#define __IGameStatisticsMgr_INTERFACE_DEFINED__





EXTERN_C const IID IID_IGameStatisticsMgr;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("AFF3EA11-E70E-407d-95DD-35E612C41CE2")
    IGameStatisticsMgr : public IUnknown
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE GetGameStatistics(
             __RPC__in_string LPCWSTR GDFBinaryPath,
             GAMESTATS_OPEN_TYPE openType,
             __RPC__out GAMESTATS_OPEN_RESULT *pOpenResult,
             __RPC__deref_out_opt IGameStatistics **ppiStats) = 0;

        virtual  HRESULT STDMETHODCALLTYPE RemoveGameStatistics(
             __RPC__in_string LPCWSTR GDFBinaryPath) = 0;

    };

#else

    typedef struct IGameStatisticsMgrVtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
            __RPC__in IGameStatisticsMgr * This,
             __RPC__in REFIID riid,

            __RPC__deref_out  void **ppvObject);

        ULONG ( STDMETHODCALLTYPE *AddRef )(
            __RPC__in IGameStatisticsMgr * This);

        ULONG ( STDMETHODCALLTYPE *Release )(
            __RPC__in IGameStatisticsMgr * This);

         HRESULT ( STDMETHODCALLTYPE *GetGameStatistics )(
            __RPC__in IGameStatisticsMgr * This,
             __RPC__in_string LPCWSTR GDFBinaryPath,
             GAMESTATS_OPEN_TYPE openType,
             __RPC__out GAMESTATS_OPEN_RESULT *pOpenResult,
             __RPC__deref_out_opt IGameStatistics **ppiStats);

         HRESULT ( STDMETHODCALLTYPE *RemoveGameStatistics )(
            __RPC__in IGameStatisticsMgr * This,
             __RPC__in_string LPCWSTR GDFBinaryPath);

        END_INTERFACE
    } IGameStatisticsMgrVtbl;

    interface IGameStatisticsMgr
    {
        CONST_VTBL struct IGameStatisticsMgrVtbl *lpVtbl;
    };



#ifdef COBJMACROS


#define IGameStatisticsMgr_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )

#define IGameStatisticsMgr_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) )

#define IGameStatisticsMgr_Release(This)	\
    ( (This)->lpVtbl -> Release(This) )


#define IGameStatisticsMgr_GetGameStatistics(This,GDFBinaryPath,openType,pOpenResult,ppiStats)	\
    ( (This)->lpVtbl -> GetGameStatistics(This,GDFBinaryPath,openType,pOpenResult,ppiStats) )

#define IGameStatisticsMgr_RemoveGameStatistics(This,GDFBinaryPath)	\
    ( (This)->lpVtbl -> RemoveGameStatistics(This,GDFBinaryPath) )

#endif


#endif




#endif


#ifndef __IGameExplorer2_INTERFACE_DEFINED__
#define __IGameExplorer2_INTERFACE_DEFINED__





EXTERN_C const IID IID_IGameExplorer2;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("86874AA7-A1ED-450d-A7EB-B89E20B2FFF3")
    IGameExplorer2 : public IUnknown
    {
    public:
        virtual  HRESULT STDMETHODCALLTYPE InstallGame(
             __RPC__in_string LPCWSTR binaryGDFPath,
             __RPC__in_opt LPCWSTR installDirectory,
             GAME_INSTALL_SCOPE installScope) = 0;

        virtual  HRESULT STDMETHODCALLTYPE UninstallGame(
             __RPC__in_string LPCWSTR binaryGDFPath) = 0;

        virtual  HRESULT STDMETHODCALLTYPE CheckAccess(
             __RPC__in_string LPCWSTR binaryGDFPath,
             __RPC__out BOOL *pHasAccess) = 0;

    };

#else

    typedef struct IGameExplorer2Vtbl
    {
        BEGIN_INTERFACE

        HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
            __RPC__in IGameExplorer2 * This,
             __RPC__in REFIID riid,

            __RPC__deref_out  void **ppvObject);

        ULONG ( STDMETHODCALLTYPE *AddRef )(
            __RPC__in IGameExplorer2 * This);

        ULONG ( STDMETHODCALLTYPE *Release )(
            __RPC__in IGameExplorer2 * This);

         HRESULT ( STDMETHODCALLTYPE *InstallGame )(
            __RPC__in IGameExplorer2 * This,
             __RPC__in_string LPCWSTR binaryGDFPath,
             __RPC__in_opt LPCWSTR installDirectory,
             GAME_INSTALL_SCOPE installScope);

         HRESULT ( STDMETHODCALLTYPE *UninstallGame )(
            __RPC__in IGameExplorer2 * This,
             __RPC__in_string LPCWSTR binaryGDFPath);

         HRESULT ( STDMETHODCALLTYPE *CheckAccess )(
            __RPC__in IGameExplorer2 * This,
             __RPC__in_string LPCWSTR binaryGDFPath,
             __RPC__out BOOL *pHasAccess);

        END_INTERFACE
    } IGameExplorer2Vtbl;

    interface IGameExplorer2
    {
        CONST_VTBL struct IGameExplorer2Vtbl *lpVtbl;
    };



#ifdef COBJMACROS


#define IGameExplorer2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )

#define IGameExplorer2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) )

#define IGameExplorer2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) )


#define IGameExplorer2_InstallGame(This,binaryGDFPath,installDirectory,installScope)	\
    ( (This)->lpVtbl -> InstallGame(This,binaryGDFPath,installDirectory,installScope) )

#define IGameExplorer2_UninstallGame(This,binaryGDFPath)	\
    ( (This)->lpVtbl -> UninstallGame(This,binaryGDFPath) )

#define IGameExplorer2_CheckAccess(This,binaryGDFPath,pHasAccess)	\
    ( (This)->lpVtbl -> CheckAccess(This,binaryGDFPath,pHasAccess) )

#endif


#endif




#endif



#ifndef __gameuxLib_LIBRARY_DEFINED__
#define __gameuxLib_LIBRARY_DEFINED__





EXTERN_C const IID LIBID_gameuxLib;

EXTERN_C const CLSID CLSID_GameExplorer;

#ifdef __cplusplus

class DECLSPEC_UUID("9A5EA990-3034-4D6F-9128-01F3C61022BC")
GameExplorer;
#endif

EXTERN_C const CLSID CLSID_GameStatistics;

#ifdef __cplusplus

class DECLSPEC_UUID("DBC85A2C-C0DC-4961-B6E2-D28B62C11AD4")
GameStatistics;
#endif
#endif



unsigned long             __RPC_USER  BSTR_UserSize(     __RPC__in unsigned long *, unsigned long            , __RPC__in BSTR * );
unsigned char * __RPC_USER  BSTR_UserMarshal(  __RPC__in unsigned long *, __RPC__inout_xcount(0) unsigned char *, __RPC__in BSTR * );
unsigned char * __RPC_USER  BSTR_UserUnmarshal(__RPC__in unsigned long *, __RPC__in_xcount(0) unsigned char *, __RPC__out BSTR * );
void                      __RPC_USER  BSTR_UserFree(     __RPC__in unsigned long *, __RPC__in BSTR * );

unsigned long             __RPC_USER  BSTR_UserSize64(     __RPC__in unsigned long *, unsigned long            , __RPC__in BSTR * );
unsigned char * __RPC_USER  BSTR_UserMarshal64(  __RPC__in unsigned long *, __RPC__inout_xcount(0) unsigned char *, __RPC__in BSTR * );
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(__RPC__in unsigned long *, __RPC__in_xcount(0) unsigned char *, __RPC__out BSTR * );
void                      __RPC_USER  BSTR_UserFree64(     __RPC__in unsigned long *, __RPC__in BSTR * );



#ifdef __cplusplus
}
#endif

#endif



