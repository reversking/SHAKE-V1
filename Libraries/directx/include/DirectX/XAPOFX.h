

















#pragma once

#include "comdecl.h"


DEFINE_CLSID(FXEQ,               A90BC001, E897, E897, 74, 39, 43, 55, 00, 00, 00, 00);
DEFINE_CLSID(FXMasteringLimiter, A90BC001, E897, E897, 74, 39, 43, 55, 00, 00, 00, 01);
DEFINE_CLSID(FXReverb,           A90BC001, E897, E897, 74, 39, 43, 55, 00, 00, 00, 02);
DEFINE_CLSID(FXEcho,             A90BC001, E897, E897, 74, 39, 43, 55, 00, 00, 00, 03);


#if !defined(GUID_DEFS_ONLY)
    #if defined(_XBOX)
        #include <xtl.h>
        #include <xobjbase.h>
    #else
        #include <windows.h>
        #include <objbase.h>
    #endif
    #include <float.h>



    #define FXEQ_MIN_FRAMERATE 22000
    #define FXEQ_MAX_FRAMERATE 48000

    #define FXEQ_MIN_FREQUENCY_CENTER       20.0f
    #define FXEQ_MAX_FREQUENCY_CENTER       20000.0f
    #define FXEQ_DEFAULT_FREQUENCY_CENTER_0 100.0f
    #define FXEQ_DEFAULT_FREQUENCY_CENTER_1 800.0f
    #define FXEQ_DEFAULT_FREQUENCY_CENTER_2 2000.0f
    #define FXEQ_DEFAULT_FREQUENCY_CENTER_3 10000.0f

    #define FXEQ_MIN_GAIN     0.126f
    #define FXEQ_MAX_GAIN     7.94f
    #define FXEQ_DEFAULT_GAIN 1.0f

    #define FXEQ_MIN_BANDWIDTH     0.1f
    #define FXEQ_MAX_BANDWIDTH     2.0f
    #define FXEQ_DEFAULT_BANDWIDTH 1.0f



    #define FXMASTERINGLIMITER_MIN_RELEASE     1
    #define FXMASTERINGLIMITER_MAX_RELEASE     20
    #define FXMASTERINGLIMITER_DEFAULT_RELEASE 6

    #define FXMASTERINGLIMITER_MIN_LOUDNESS     1
    #define FXMASTERINGLIMITER_MAX_LOUDNESS     1800
    #define FXMASTERINGLIMITER_DEFAULT_LOUDNESS 1000



    #define FXREVERB_MIN_DIFFUSION     0.0f
    #define FXREVERB_MAX_DIFFUSION     1.0f
    #define FXREVERB_DEFAULT_DIFFUSION 0.9f

    #define FXREVERB_MIN_ROOMSIZE     0.0001f
    #define FXREVERB_MAX_ROOMSIZE     1.0f
    #define FXREVERB_DEFAULT_ROOMSIZE 0.6f



    #define FXECHO_MIN_WETDRYMIX     0.0f
    #define FXECHO_MAX_WETDRYMIX     1.0f
    #define FXECHO_DEFAULT_WETDRYMIX 0.5f

    #define FXECHO_MIN_FEEDBACK     0.0f
    #define FXECHO_MAX_FEEDBACK     1.0f
    #define FXECHO_DEFAULT_FEEDBACK 0.5f

    #define FXECHO_MIN_DELAY     1.0f
    #define FXECHO_MAX_DELAY     2000.0f
    #define FXECHO_DEFAULT_DELAY 500.0f



    #pragma pack(push, 1)





    typedef struct FXEQ_PARAMETERS {
        float FrequencyCenter0;
        float Gain0;
        float Bandwidth0;
        float FrequencyCenter1;
        float Gain1;
        float Bandwidth1;
        float FrequencyCenter2;
        float Gain2;
        float Bandwidth2;
        float FrequencyCenter3;
        float Gain3;
        float Bandwidth3;
    } FXEQ_PARAMETERS;




    typedef struct FXMASTERINGLIMITER_PARAMETERS {
        UINT32 Release;
        UINT32 Loudness;
    } FXMASTERINGLIMITER_PARAMETERS;







    typedef struct FXREVERB_PARAMETERS {
        float Diffusion;
        float RoomSize;
    } FXREVERB_PARAMETERS;




    typedef struct FXECHO_PARAMETERS {
        float WetDryMix;
        float Feedback;
        float Delay;
    } FXECHO_PARAMETERS;




    #if defined(_XBOX) || !defined(FXDLL)
        #define FX_API_(type) EXTERN_C type STDAPIVCALLTYPE
    #else
        #if defined(FXEXPORT)
            #define FX_API_(type) EXTERN_C __declspec(dllexport) type STDAPIVCALLTYPE
        #else
            #define FX_API_(type) EXTERN_C __declspec(dllimport) type STDAPIVCALLTYPE
        #endif
    #endif
    #define FX_IMP_(type) type STDMETHODVCALLTYPE




    FX_API_(HRESULT) CreateFX (REFCLSID clsid, __deref_out IUnknown** pEffect);


    #pragma pack(pop)
#endif

