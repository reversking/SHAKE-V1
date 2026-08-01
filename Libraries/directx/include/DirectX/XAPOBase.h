













#pragma once

#include "XAPO.h"


#define XAPOBASE_DEFAULT_FORMAT_TAG           WAVE_FORMAT_IEEE_FLOAT
#define XAPOBASE_DEFAULT_FORMAT_MIN_CHANNELS  XAPO_MIN_CHANNELS
#define XAPOBASE_DEFAULT_FORMAT_MAX_CHANNELS  XAPO_MAX_CHANNELS
#define XAPOBASE_DEFAULT_FORMAT_MIN_FRAMERATE XAPO_MIN_FRAMERATE
#define XAPOBASE_DEFAULT_FORMAT_MAX_FRAMERATE XAPO_MAX_FRAMERATE
#define XAPOBASE_DEFAULT_FORMAT_BITSPERSAMPLE 32


#define XAPOBASE_DEFAULT_FLAG (XAPO_FLAG_CHANNELS_MUST_MATCH | XAPO_FLAG_FRAMERATE_MUST_MATCH | XAPO_FLAG_BITSPERSAMPLE_MUST_MATCH | XAPO_FLAG_BUFFERCOUNT_MUST_MATCH | XAPO_FLAG_INPLACE_SUPPORTED)


#define XAPOBASE_DEFAULT_BUFFER_COUNT 1




#if !defined(XAPOASSERT)
    #if XAPODEBUG
        #define XAPOASSERT(exp) if (!(exp)) { OutputDebugStringA("XAPO ASSERT: " #exp ", {" __FUNCTION__ "}\n"); __debugbreak(); }
    #else
        #define XAPOASSERT(exp) __assume(exp)
    #endif
#endif



#pragma pack(push, 8)



typedef float FLOAT32;







class __declspec(novtable) CXAPOBase: public IXAPO {
private:
    const XAPO_REGISTRATION_PROPERTIES* m_pRegistrationProperties;

    void*    m_pfnMatrixMixFunction;
    FLOAT32* m_pfl32MatrixCoefficients;
    UINT32   m_nSrcFormatType;
    BOOL     m_fIsScalarMatrix;
    BOOL     m_fIsLocked;


protected:
    LONG m_lReferenceCount;





















    virtual HRESULT ValidateFormatDefault (__inout WAVEFORMATEX* pFormat, BOOL fOverwrite);























    HRESULT ValidateFormatPair (const WAVEFORMATEX* pSupportedFormat, __inout WAVEFORMATEX* pRequestedFormat, BOOL fOverwrite);




























    void ProcessThru (__in void* pInputBuffer, __inout FLOAT32* pOutputBuffer, UINT32 FrameCount, WORD InputChannelCount, WORD OutputChannelCount, BOOL MixWithOutput);


    const XAPO_REGISTRATION_PROPERTIES* GetRegistrationPropertiesInternal () { return m_pRegistrationProperties; }
    BOOL IsLocked  () { return m_fIsLocked; }


public:
    CXAPOBase (const XAPO_REGISTRATION_PROPERTIES* pRegistrationProperties);
    virtual ~CXAPOBase ();



    STDMETHOD(QueryInterface) (REFIID riid, __deref_out_opt void** ppInterface)
    {
        XAPOASSERT(ppInterface != NULL);
        HRESULT hr = S_OK;

        if (riid == __uuidof(IXAPO)) {
            *ppInterface = static_cast<IXAPO*>(this);
            AddRef();
        } else if (riid == __uuidof(IUnknown)) {
            *ppInterface = static_cast<IUnknown*>(this);
            AddRef();
        } else {
            *ppInterface = NULL;
            hr = E_NOINTERFACE;
        }

        return hr;
    }


    STDMETHOD_(ULONG, AddRef) ()
    {
        return (ULONG)InterlockedIncrement(&m_lReferenceCount);
    }


    STDMETHOD_(ULONG, Release) ()
    {
        ULONG uTmpReferenceCount = (ULONG)InterlockedDecrement(&m_lReferenceCount);
        if (uTmpReferenceCount == 0) {
            delete this;
        }
        return uTmpReferenceCount;
    }





    STDMETHOD(GetRegistrationProperties) (__deref_out XAPO_REGISTRATION_PROPERTIES** ppRegistrationProperties);




    STDMETHOD(IsInputFormatSupported) (const WAVEFORMATEX* pOutputFormat, const WAVEFORMATEX* pRequestedInputFormat, __deref_opt_out WAVEFORMATEX** ppSupportedInputFormat);




    STDMETHOD(IsOutputFormatSupported) (const WAVEFORMATEX* pInputFormat, const WAVEFORMATEX* pRequestedOutputFormat, __deref_opt_out WAVEFORMATEX** ppSupportedOutputFormat);



    STDMETHOD(Initialize) (__in_bcount_opt(DataByteSize) const void*, UINT32 DataByteSize)
    {
        UNREFERENCED_PARAMETER(DataByteSize);
        return S_OK;
    }




    STDMETHOD_(void, Reset) () { return; }





    STDMETHOD(LockForProcess) (UINT32 InputLockedParameterCount, __in_ecount_opt(InputLockedParameterCount) const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, __in_ecount_opt(OutputLockedParameterCount) const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters);



    STDMETHOD_(void, UnlockForProcess) ();



    STDMETHOD_(UINT32, CalcInputFrames) (UINT32 OutputFrameCount) { return OutputFrameCount; }



    STDMETHOD_(UINT32, CalcOutputFrames) (UINT32 InputFrameCount) { return InputFrameCount; }
};

















class __declspec(novtable) CXAPOParametersBase: public CXAPOBase, public IXAPOParameters {
private:
    BYTE*  m_pParameterBlocks;
    BYTE*  m_pCurrentParameters;
    BYTE*  m_pCurrentParametersInternal;
    UINT32 m_uCurrentParametersIndex;
    UINT32 m_uParameterBlockByteSize;
    BOOL   m_fNewerResultsReady;
    BOOL   m_fProducer;


public:







    CXAPOParametersBase (const XAPO_REGISTRATION_PROPERTIES* pRegistrationProperties, BYTE* pParameterBlocks, UINT32 uParameterBlockByteSize, BOOL fProducer);
    virtual ~CXAPOParametersBase ();



    STDMETHOD(QueryInterface) (REFIID riid, __deref_out_opt void** ppInterface)
    {
        XAPOASSERT(ppInterface != NULL);
        HRESULT hr = S_OK;

        if (riid == __uuidof(IXAPOParameters)) {
            *ppInterface = static_cast<IXAPOParameters*>(this);
            CXAPOBase::AddRef();
        } else {
            hr = CXAPOBase::QueryInterface(riid, ppInterface);
        }

        return hr;
    }


    STDMETHOD_(ULONG, AddRef)() { return CXAPOBase::AddRef(); }


    STDMETHOD_(ULONG, Release)() { return CXAPOBase::Release(); }




    STDMETHOD_(void, SetParameters) (__in_bcount(ParameterByteSize) const void* pParameters, UINT32 ParameterByteSize);




    STDMETHOD_(void, GetParameters) (__out_bcount(ParameterByteSize) void* pParameters, UINT32 ParameterByteSize);





    virtual void OnSetParameters (const void*, UINT32) { }




    BOOL ParametersChanged ();




    BYTE* BeginProcess ();





    void EndProcess ();
};


#pragma pack(pop)

