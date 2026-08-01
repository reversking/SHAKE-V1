













#pragma once

#ifndef _XACT3_H_
#define _XACT3_H_




#ifndef _XBOX
    #include <comdecl.h>
    DEFINE_CLSID(XACTEngine,         bcc782bc, 6492, 4c22, 8c, 35, f5, d7, 2f, e7, 3c, 6e);
    DEFINE_CLSID(XACTAuditionEngine, 9ecdd80d, 0e81, 40d8, 89, 03, 2b, f7, b1, 31, ac, 43);
    DEFINE_CLSID(XACTDebugEngine,    02860630, bf3b, 42a8, b1, 4e, 91, ed, a2, f5, 1e, a5);
    DEFINE_IID(IXACT3Engine,         b1ee676a, d9cd, 4d2a, 89, a8, fa, 53, eb, 9e, 48, 0b);
#endif


#ifndef GUID_DEFS_ONLY





#ifndef _XBOX
    #include <windows.h>
    #include <objbase.h>
    #include <float.h>
#endif
#include <limits.h>
#include <xact3wb.h>
#include <xaudio2.h>





typedef struct IXACT3SoundBank       IXACT3SoundBank;
typedef struct IXACT3WaveBank        IXACT3WaveBank;
typedef struct IXACT3Cue             IXACT3Cue;
typedef struct IXACT3Wave            IXACT3Wave;
typedef struct IXACT3Engine          IXACT3Engine;
typedef struct XACT_NOTIFICATION     XACT_NOTIFICATION;






typedef WORD  XACTINDEX;
typedef BYTE  XACTNOTIFICATIONTYPE;
typedef FLOAT XACTVARIABLEVALUE;
typedef WORD  XACTVARIABLEINDEX;
typedef WORD  XACTCATEGORY;
typedef BYTE  XACTCHANNEL;
typedef FLOAT XACTVOLUME;
typedef LONG  XACTTIME;
typedef SHORT XACTPITCH;
typedef BYTE  XACTLOOPCOUNT;
typedef BYTE  XACTVARIATIONWEIGHT;
typedef BYTE  XACTPRIORITY;
typedef BYTE  XACTINSTANCELIMIT;




#ifndef WAVE_FORMAT_IEEE_FLOAT
    #define  WAVE_FORMAT_IEEE_FLOAT 0x0003
#endif

#ifndef WAVE_FORMAT_EXTENSIBLE
    #define  WAVE_FORMAT_EXTENSIBLE 0xFFFE
#endif

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
    #pragma pack(push, 1)
    typedef struct tWAVEFORMATEX
    {
        WORD    wFormatTag;
        WORD    nChannels;
        DWORD   nSamplesPerSec;
        DWORD   nAvgBytesPerSec;
        WORD    nBlockAlign;
        WORD    wBitsPerSample;
        WORD    cbSize;

    } WAVEFORMATEX, *PWAVEFORMATEX;
    typedef WAVEFORMATEX NEAR *NPWAVEFORMATEX;
    typedef WAVEFORMATEX FAR  *LPWAVEFORMATEX;
    #pragma pack(pop)
#endif

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
    #pragma pack(push, 1)
    typedef struct
    {
        WAVEFORMATEX    Format;

        union
        {
            WORD        wValidBitsPerSample;
            WORD        wSamplesPerBlock;
            WORD        wReserved;
        } Samples;

        DWORD           dwChannelMask;
        GUID            SubFormat;
    } WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
    #pragma pack(pop)
#endif




static const XACTTIME               XACTTIME_MIN                    = LONG_MIN;
static const XACTTIME               XACTTIME_MAX                    = LONG_MAX;
static const XACTTIME               XACTTIME_INFINITE               = LONG_MAX;
static const XACTINSTANCELIMIT      XACTINSTANCELIMIT_INFINITE      = 0xff;
static const XACTINSTANCELIMIT      XACTINSTANCELIMIT_MIN           = 0x00;
static const XACTINSTANCELIMIT      XACTINSTANCELIMIT_MAX           = 0xfe;
static const XACTINDEX              XACTINDEX_MIN                   = 0x0;
static const XACTINDEX              XACTINDEX_MAX                   = 0xfffe;
static const XACTINDEX              XACTINDEX_INVALID               = 0xffff;
static const XACTNOTIFICATIONTYPE   XACTNOTIFICATIONTYPE_MIN        = 0x00;
static const XACTNOTIFICATIONTYPE   XACTNOTIFICATIONTYPE_MAX        = 0xff;
static const XACTVARIABLEVALUE      XACTVARIABLEVALUE_MIN           = -FLT_MAX;
static const XACTVARIABLEVALUE      XACTVARIABLEVALUE_MAX           = FLT_MAX;
static const XACTVARIABLEINDEX      XACTVARIABLEINDEX_MIN           = 0x0000;
static const XACTVARIABLEINDEX      XACTVARIABLEINDEX_MAX           = 0xfffe;
static const XACTVARIABLEINDEX      XACTVARIABLEINDEX_INVALID       = 0xffff;
static const XACTCATEGORY           XACTCATEGORY_MIN                = 0x0;
static const XACTCATEGORY           XACTCATEGORY_MAX                = 0xfffe;
static const XACTCATEGORY           XACTCATEGORY_INVALID            = 0xffff;
static const XACTCHANNEL            XACTCHANNEL_MIN                 = 0;
static const XACTCHANNEL            XACTCHANNEL_MAX                 = 0xFF;
static const XACTPITCH              XACTPITCH_MIN                   = -1200;
static const XACTPITCH              XACTPITCH_MAX                   = 1200;
static const XACTPITCH              XACTPITCH_MIN_TOTAL             = -2400;
static const XACTPITCH              XACTPITCH_MAX_TOTAL             = 2400;
static const XACTVOLUME             XACTVOLUME_MIN                  = 0.0f;
static const XACTVOLUME             XACTVOLUME_MAX                  = 16777216.0f;
static const XACTVARIABLEVALUE      XACTPARAMETERVALUE_MIN          = -FLT_MAX;
static const XACTVARIABLEVALUE      XACTPARAMETERVALUE_MAX          = FLT_MAX;
static const XACTLOOPCOUNT          XACTLOOPCOUNT_MIN               = 0x0;
static const XACTLOOPCOUNT          XACTLOOPCOUNT_MAX               = 0xfe;
static const XACTLOOPCOUNT          XACTLOOPCOUNT_INFINITE          = 0xff;
static const DWORD                  XACTWAVEALIGNMENT_MIN           = 2048;
#ifdef _XBOX
static const BYTE                   XACTMAXOUTPUTVOICECOUNT         = 3;
#endif





#define XACT_CUE_NAME_LENGTH        0xFF




#define XACT_CONTENT_VERSION        46




static const DWORD XACT_FLAG_STOP_RELEASE       = 0x00000000;
static const DWORD XACT_FLAG_STOP_IMMEDIATE     = 0x00000001;




static const DWORD XACT_FLAG_MANAGEDATA         = 0x00000001;




static const DWORD XACT_FLAG_BACKGROUND_MUSIC   = 0x00000002;
static const DWORD XACT_FLAG_UNITS_MS           = 0x00000004;
static const DWORD XACT_FLAG_UNITS_SAMPLES      = 0x00000008;




static const DWORD XACT_STATE_CREATED           = 0x00000001;
static const DWORD XACT_STATE_PREPARING         = 0x00000002;
static const DWORD XACT_STATE_PREPARED          = 0x00000004;
static const DWORD XACT_STATE_PLAYING           = 0x00000008;
static const DWORD XACT_STATE_STOPPING          = 0x00000010;
static const DWORD XACT_STATE_STOPPED           = 0x00000020;
static const DWORD XACT_STATE_PAUSED            = 0x00000040;
static const DWORD XACT_STATE_INUSE             = 0x00000080;
static const DWORD XACT_STATE_PREPAREFAILED     = 0x80000000;





#define XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA    XACT_FLAG_MANAGEDATA




typedef BOOL (__stdcall * XACT_READFILE_CALLBACK)(__in HANDLE hFile, __out_bcount(nNumberOfBytesToRead) LPVOID lpBuffer, DWORD nNumberOfBytesToRead, __out LPDWORD lpNumberOfBytesRead, __inout LPOVERLAPPED lpOverlapped);
typedef BOOL (__stdcall * XACT_GETOVERLAPPEDRESULT_CALLBACK)(__in HANDLE hFile, __inout LPOVERLAPPED lpOverlapped, __out LPDWORD lpNumberOfBytesTransferred, BOOL bWait);

typedef struct XACT_FILEIO_CALLBACKS
{
    XACT_READFILE_CALLBACK              readFileCallback;
    XACT_GETOVERLAPPEDRESULT_CALLBACK   getOverlappedResultCallback;

} XACT_FILEIO_CALLBACKS, *PXACT_FILEIO_CALLBACKS;
typedef const XACT_FILEIO_CALLBACKS *PCXACT_FILEIO_CALLBACKS;




typedef void (__stdcall * XACT_NOTIFICATION_CALLBACK)(__in const XACT_NOTIFICATION* pNotification);

#define XACT_RENDERER_ID_LENGTH                 0xff
#define XACT_RENDERER_NAME_LENGTH               0xff




typedef struct XACT_RENDERER_DETAILS
{
    WCHAR rendererID[XACT_RENDERER_ID_LENGTH];
    WCHAR displayName[XACT_RENDERER_NAME_LENGTH];
    BOOL  defaultDevice;

} XACT_RENDERER_DETAILS, *LPXACT_RENDERER_DETAILS;




#define XACT_ENGINE_LOOKAHEAD_DEFAULT           250




typedef struct XACT_RUNTIME_PARAMETERS
{
    DWORD                           lookAheadTime;
    void*                           pGlobalSettingsBuffer;
    DWORD                           globalSettingsBufferSize;
    DWORD                           globalSettingsFlags;
    DWORD                           globalSettingsAllocAttributes;
    XACT_FILEIO_CALLBACKS           fileIOCallbacks;
    XACT_NOTIFICATION_CALLBACK      fnNotificationCallback;
    PWSTR                           pRendererID;
    IXAudio2*                       pXAudio2;
    IXAudio2MasteringVoice*         pMasteringVoice;

} XACT_RUNTIME_PARAMETERS, *LPXACT_RUNTIME_PARAMETERS;
typedef const XACT_RUNTIME_PARAMETERS *LPCXACT_RUNTIME_PARAMETERS;





typedef struct XACT_STREAMING_PARAMETERS
{
    HANDLE  file;
    DWORD   offset;
    DWORD   flags;
    WORD    packetSize;



} XACT_WAVEBANK_STREAMING_PARAMETERS, *LPXACT_WAVEBANK_STREAMING_PARAMETERS, XACT_STREAMING_PARAMETERS, *LPXACT_STREAMING_PARAMETERS;
typedef const XACT_STREAMING_PARAMETERS *LPCXACT_STREAMING_PARAMETERS;
typedef const XACT_WAVEBANK_STREAMING_PARAMETERS *LPCXACT_WAVEBANK_STREAMING_PARAMETERS;


typedef struct XACT_CUE_PROPERTIES
{
    CHAR                friendlyName[XACT_CUE_NAME_LENGTH];
    BOOL                interactive;
    XACTINDEX           iaVariableIndex;
    XACTINDEX           numVariations;
    XACTINSTANCELIMIT   maxInstances;
    XACTINSTANCELIMIT   currentInstances;

} XACT_CUE_PROPERTIES, *LPXACT_CUE_PROPERTIES;


typedef struct XACT_TRACK_PROPERTIES
{
    XACTTIME        duration;
    XACTINDEX       numVariations;
    XACTCHANNEL     numChannels;
    XACTINDEX       waveVariation;
    XACTLOOPCOUNT   loopCount;

} XACT_TRACK_PROPERTIES, *LPXACT_TRACK_PROPERTIES;


typedef struct XACT_VARIATION_PROPERTIES
{
    XACTINDEX               index;
    XACTVARIATIONWEIGHT     weight;
    XACTVARIABLEVALUE       iaVariableMin;
    XACTVARIABLEVALUE       iaVariableMax;
    BOOL                    linger;

} XACT_VARIATION_PROPERTIES, *LPXACT_VARIATION_PROPERTIES;


typedef struct XACT_SOUND_PROPERTIES
{
    XACTCATEGORY            category;
    BYTE                    priority;
    XACTPITCH               pitch;
    XACTVOLUME              volume;
    XACTINDEX               numTracks;
    XACT_TRACK_PROPERTIES   arrTrackProperties[1];

} XACT_SOUND_PROPERTIES, *LPXACT_SOUND_PROPERTIES;


typedef struct XACT_SOUND_VARIATION_PROPERTIES
{
    XACT_VARIATION_PROPERTIES   variationProperties;
    XACT_SOUND_PROPERTIES       soundProperties;

} XACT_SOUND_VARIATION_PROPERTIES, *LPXACT_SOUND_VARIATION_PROPERTIES;


typedef struct XACT_CUE_INSTANCE_PROPERTIES
{
    DWORD                            allocAttributes;
    XACT_CUE_PROPERTIES              cueProperties;
    XACT_SOUND_VARIATION_PROPERTIES  activeVariationProperties;

} XACT_CUE_INSTANCE_PROPERTIES, *LPXACT_CUE_INSTANCE_PROPERTIES;


typedef struct XACT_WAVE_PROPERTIES
{
    char                    friendlyName[WAVEBANK_ENTRYNAME_LENGTH];
    WAVEBANKMINIWAVEFORMAT  format;
    DWORD                   durationInSamples;
    WAVEBANKSAMPLEREGION    loopRegion;
    BOOL                    streaming;

} XACT_WAVE_PROPERTIES, *LPXACT_WAVE_PROPERTIES;
typedef const XACT_WAVE_PROPERTIES* LPCXACT_WAVE_PROPERTIES;


typedef struct XACT_WAVE_INSTANCE_PROPERTIES
{
    XACT_WAVE_PROPERTIES    properties;
    BOOL                    backgroundMusic;

} XACT_WAVE_INSTANCE_PROPERTIES, *LPXACT_WAVE_INSTANCE_PROPERTIES;
typedef const XACT_WAVE_INSTANCE_PROPERTIES* LPCXACT_WAVE_INSTANCE_PROPERTIES;





typedef struct XACTCHANNELMAPENTRY
{
    XACTCHANNEL   InputChannel;
    XACTCHANNEL   OutputChannel;
    XACTVOLUME    Volume;

} XACTCHANNELMAPENTRY, *LPXACTCHANNELMAPENTRY;
typedef const XACTCHANNELMAPENTRY *LPCXACTCHANNELMAPENTRY;

typedef struct XACTCHANNELMAP
{
    XACTCHANNEL             EntryCount;
    XACTCHANNELMAPENTRY*    paEntries;

} XACTCHANNELMAP, *LPXACTCHANNELMAP;
typedef const XACTCHANNELMAP *LPCXACTCHANNELMAP;

typedef struct XACTCHANNELVOLUMEENTRY
{
    XACTCHANNEL   EntryIndex;
    XACTVOLUME    Volume;

} XACTCHANNELVOLUMEENTRY, *LPXACTCHANNELVOLUMEENTRY;
typedef const XACTCHANNELVOLUMEENTRY *LPCXACTCHANNELVOLUMEENTRY;

typedef struct XACTCHANNELVOLUME
{
    XACTCHANNEL             EntryCount;
    XACTCHANNELVOLUMEENTRY* paEntries;

} XACTCHANNELVOLUME, *LPXACTCHANNELVOLUME;
typedef const XACTCHANNELVOLUME *LPCXACTCHANNELVOLUME;





static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_CUEPREPARED                      = 1;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_CUEPLAY                          = 2;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_CUESTOP                          = 3;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_CUEDESTROYED                     = 4;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_MARKER                           = 5;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED               = 6;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEBANKDESTROYED                = 7;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_LOCALVARIABLECHANGED             = 8;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_GLOBALVARIABLECHANGED            = 9;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_GUICONNECTED                     = 10;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_GUIDISCONNECTED                  = 11;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEPREPARED                     = 12;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEPLAY                         = 13;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVESTOP                         = 14;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVELOOPED                       = 15;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEDESTROYED                    = 16;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEBANKPREPARED                 = 17;
static const XACTNOTIFICATIONTYPE XACTNOTIFICATIONTYPE_WAVEBANKSTREAMING_INVALIDCONTENT = 18;

static const BYTE XACT_FLAG_NOTIFICATION_PERSIST = 0x01;


#pragma pack(push, 1)


typedef struct XACT_NOTIFICATION_DESCRIPTION
{
    XACTNOTIFICATIONTYPE type;
    BYTE                 flags;
    IXACT3SoundBank*      pSoundBank;
    IXACT3WaveBank*       pWaveBank;
    IXACT3Cue*            pCue;
    IXACT3Wave*           pWave;
    XACTINDEX            cueIndex;
    XACTINDEX            waveIndex;
    PVOID                pvContext;

} XACT_NOTIFICATION_DESCRIPTION, *LPXACT_NOTIFICATION_DESCRIPTION;
typedef const XACT_NOTIFICATION_DESCRIPTION *LPCXACT_NOTIFICATION_DESCRIPTION;


typedef struct XACT_NOTIFICATION_CUE
{
    XACTINDEX       cueIndex;
    IXACT3SoundBank* pSoundBank;
    IXACT3Cue*       pCue;

} XACT_NOTIFICATION_CUE, *LPXACT_NOTIFICATION_CUE;
typedef const XACT_NOTIFICATION_CUE *LPCXACT_NOTIFICATION_CUE;


typedef struct XACT_NOTIFICATION_MARKER
{
    XACTINDEX       cueIndex;
    IXACT3SoundBank* pSoundBank;
    IXACT3Cue*       pCue;
    DWORD           marker;

} XACT_NOTIFICATION_MARKER, *LPXACT_NOTIFICATION_MARKER;
typedef const XACT_NOTIFICATION_MARKER *LPCXACT_NOTIFICATION_MARKER;


typedef struct XACT_NOTIFICATION_SOUNDBANK
{
    IXACT3SoundBank* pSoundBank;

} XACT_NOTIFICATION_SOUNDBANK, *LPXACT_NOTIFICATION_SOUNDBANK;
typedef const XACT_NOTIFICATION_SOUNDBANK *LPCXACT_NOTIFICATION_SOUNDBANK;


typedef struct XACT_NOTIFICATION_WAVEBANK
{
    IXACT3WaveBank*  pWaveBank;

} XACT_NOTIFICATION_WAVEBANK, *LPXACT_NOTIFICATION_WAVEBANK;
typedef const XACT_NOTIFICATION_WAVEBANK *LPCXACT_NOTIFICATION_WAVEBANK;


typedef struct XACT_NOTIFICATION_VARIABLE
{
    XACTINDEX           cueIndex;
    IXACT3SoundBank*     pSoundBank;
    IXACT3Cue*           pCue;
    XACTVARIABLEINDEX   variableIndex;
    XACTVARIABLEVALUE   variableValue;
    BOOL                local;

} XACT_NOTIFICATION_VARIABLE, *LPXACT_NOTIFICATION_VARIABLE;
typedef const XACT_NOTIFICATION_VARIABLE *LPCXACT_NOTIFICATION_VARIABLE;


typedef struct XACT_NOTIFICATION_GUI
{
    DWORD   reserved;
} XACT_NOTIFICATION_GUI, *LPXACT_NOTIFICATION_GUI;
typedef const XACT_NOTIFICATION_GUI *LPCXACT_NOTIFICATION_GUI;


typedef struct XACT_NOTIFICATION_WAVE
{
    IXACT3WaveBank*  pWaveBank;
    XACTINDEX       waveIndex;
    XACTINDEX       cueIndex;
    IXACT3SoundBank* pSoundBank;
    IXACT3Cue*       pCue;
    IXACT3Wave*      pWave;

} XACT_NOTIFICATION_WAVE, *LPXACT_NOTIFICATION_WAVE;
typedef const XACT_NOTIFICATION_WAVE *LPCXACT_NOTIFICATION_WAVE;


typedef struct XACT_NOTIFICATION
{
    XACTNOTIFICATIONTYPE    type;
    LONG                    timeStamp;
    PVOID                   pvContext;
    union
    {
        XACT_NOTIFICATION_CUE       cue;
        XACT_NOTIFICATION_MARKER    marker;
        XACT_NOTIFICATION_SOUNDBANK soundBank;
        XACT_NOTIFICATION_WAVEBANK  waveBank;
        XACT_NOTIFICATION_VARIABLE  variable;
        XACT_NOTIFICATION_GUI       gui;
        XACT_NOTIFICATION_WAVE      wave;
    };

} XACT_NOTIFICATION, *LPXACT_NOTIFICATION;
typedef const XACT_NOTIFICATION *LPCXACT_NOTIFICATION;

#pragma pack(pop)





#define XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE  XACT_FLAG_STOP_IMMEDIATE
#define XACT_SOUNDBANKSTATE_INUSE           XACT_STATE_INUSE

STDAPI_(XACTINDEX) IXACT3SoundBank_GetCueIndex(__in IXACT3SoundBank* pSoundBank, __in PCSTR szFriendlyName);
STDAPI IXACT3SoundBank_GetNumCues(__in IXACT3SoundBank* pSoundBank, __out XACTINDEX* pnNumCues);
STDAPI IXACT3SoundBank_GetCueProperties(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, __out LPXACT_CUE_PROPERTIES pProperties);
STDAPI IXACT3SoundBank_Prepare(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_out IXACT3Cue** ppCue);
STDAPI IXACT3SoundBank_Play(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_opt_out IXACT3Cue** ppCue);
STDAPI IXACT3SoundBank_Stop(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags);
STDAPI IXACT3SoundBank_Destroy(__in IXACT3SoundBank* pSoundBank);
STDAPI IXACT3SoundBank_GetState(__in IXACT3SoundBank* pSoundBank, __out DWORD* pdwState);

#undef INTERFACE
#define INTERFACE IXACT3SoundBank

DECLARE_INTERFACE(IXACT3SoundBank)
{
    STDMETHOD_(XACTINDEX, GetCueIndex)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(GetNumCues)(THIS_ __out XACTINDEX* pnNumCues) PURE;
    STDMETHOD(GetCueProperties)(THIS_ XACTINDEX nCueIndex, __out LPXACT_CUE_PROPERTIES pProperties) PURE;
    STDMETHOD(Prepare)(THIS_ XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_out IXACT3Cue** ppCue) PURE;
    STDMETHOD(Play)(THIS_ XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_opt_out IXACT3Cue** ppCue) PURE;
    STDMETHOD(Stop)(THIS_ XACTINDEX nCueIndex, DWORD dwFlags) PURE;
    STDMETHOD(Destroy)(THIS) PURE;
    STDMETHOD(GetState)(THIS_ __out DWORD* pdwState) PURE;
};

#ifdef __cplusplus

__inline HRESULT __stdcall IXACT3SoundBank_Destroy(__in IXACT3SoundBank* pSoundBank)
{
    return pSoundBank->Destroy();
}

__inline XACTINDEX __stdcall IXACT3SoundBank_GetCueIndex(__in IXACT3SoundBank* pSoundBank, __in PCSTR szFriendlyName)
{
    return pSoundBank->GetCueIndex(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetNumCues(__in IXACT3SoundBank* pSoundBank, __out XACTINDEX* pnNumCues)
{
    return pSoundBank->GetNumCues(pnNumCues);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetCueProperties(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, __out LPXACT_CUE_PROPERTIES pProperties)
{
    return pSoundBank->GetCueProperties(nCueIndex, pProperties);
}

__inline HRESULT __stdcall IXACT3SoundBank_Prepare(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_out IXACT3Cue** ppCue)
{
    return pSoundBank->Prepare(nCueIndex, dwFlags, timeOffset, ppCue);
}

__inline HRESULT __stdcall IXACT3SoundBank_Play(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_opt_out IXACT3Cue** ppCue)
{
    return pSoundBank->Play(nCueIndex, dwFlags, timeOffset, ppCue);
}

__inline HRESULT __stdcall IXACT3SoundBank_Stop(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags)
{
    return pSoundBank->Stop(nCueIndex, dwFlags);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetState(__in IXACT3SoundBank* pSoundBank, __out DWORD* pdwState)
{
    return pSoundBank->GetState(pdwState);
}

#else

__inline HRESULT __stdcall IXACT3SoundBank_Destroy(__in IXACT3SoundBank* pSoundBank)
{
    return pSoundBank->lpVtbl->Destroy(pSoundBank);
}

__inline XACTINDEX __stdcall IXACT3SoundBank_GetCueIndex(__in IXACT3SoundBank* pSoundBank, __in PCSTR szFriendlyName)
{
    return pSoundBank->lpVtbl->GetCueIndex(pSoundBank, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetNumCues(__in IXACT3SoundBank* pSoundBank, __out XACTINDEX* pnNumCues)
{
    return pSoundBank->lpVtbl->GetNumCues(pSoundBank, pnNumCues);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetCueProperties(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, __out LPXACT_CUE_PROPERTIES pProperties)
{
    return pSoundBank->lpVtbl->GetCueProperties(pSoundBank, nCueIndex, pProperties);
}

__inline HRESULT __stdcall IXACT3SoundBank_Prepare(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_out IXACT3Cue** ppCue)
{
    return pSoundBank->lpVtbl->Prepare(pSoundBank, nCueIndex, dwFlags, timeOffset, ppCue);
}

__inline HRESULT __stdcall IXACT3SoundBank_Play(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags, XACTTIME timeOffset, __deref_opt_out IXACT3Cue** ppCue)
{
    return pSoundBank->lpVtbl->Play(pSoundBank, nCueIndex, dwFlags, timeOffset, ppCue);
}

__inline HRESULT __stdcall IXACT3SoundBank_Stop(__in IXACT3SoundBank* pSoundBank, XACTINDEX nCueIndex, DWORD dwFlags)
{
    return pSoundBank->lpVtbl->Stop(pSoundBank, nCueIndex, dwFlags);
}

__inline HRESULT __stdcall IXACT3SoundBank_GetState(__in IXACT3SoundBank* pSoundBank, __out DWORD* pdwState)
{
    return pSoundBank->lpVtbl->GetState(pSoundBank, pdwState);
}

#endif




#define XACT_WAVEBANKSTATE_INUSE            XACT_STATE_INUSE
#define XACT_WAVEBANKSTATE_PREPARED         XACT_STATE_PREPARED
#define XACT_WAVEBANKSTATE_PREPAREFAILED    XACT_STATE_PREPAREFAILED


STDAPI IXACT3WaveBank_Destroy(__in IXACT3WaveBank* pWaveBank);
STDAPI IXACT3WaveBank_GetState(__in IXACT3WaveBank* pWaveBank, __out DWORD* pdwState);
STDAPI IXACT3WaveBank_GetNumWaves(__in IXACT3WaveBank* pWaveBank, __out XACTINDEX* pnNumWaves);
STDAPI_(XACTINDEX) IXACT3WaveBank_GetWaveIndex(__in IXACT3WaveBank* pWaveBank, __in PCSTR szFriendlyName);
STDAPI IXACT3WaveBank_GetWaveProperties(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, __out LPXACT_WAVE_PROPERTIES pWaveProperties);
STDAPI IXACT3WaveBank_Prepare(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3WaveBank_Play(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3WaveBank_Stop(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags);

#undef INTERFACE
#define INTERFACE IXACT3WaveBank

DECLARE_INTERFACE(IXACT3WaveBank)
{
    STDMETHOD(Destroy)(THIS) PURE;
    STDMETHOD(GetNumWaves)(THIS_ __out XACTINDEX* pnNumWaves) PURE;
    STDMETHOD_(XACTINDEX, GetWaveIndex)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(GetWaveProperties)(THIS_ XACTINDEX nWaveIndex, __out LPXACT_WAVE_PROPERTIES pWaveProperties) PURE;
    STDMETHOD(Prepare)(THIS_ XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;
    STDMETHOD(Play)(THIS_ XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;
    STDMETHOD(Stop)(THIS_ XACTINDEX nWaveIndex, DWORD dwFlags) PURE;
    STDMETHOD(GetState)(THIS_ __out DWORD* pdwState) PURE;
};

#ifdef __cplusplus

__inline HRESULT __stdcall IXACT3WaveBank_Destroy(__in IXACT3WaveBank* pWaveBank)
{
    return pWaveBank->Destroy();
}

__inline HRESULT __stdcall IXACT3WaveBank_GetNumWaves(__in IXACT3WaveBank* pWaveBank, __out XACTINDEX* pnNumWaves)
{
    return pWaveBank->GetNumWaves(pnNumWaves);
}

__inline XACTINDEX __stdcall IXACT3WaveBank_GetWaveIndex(__in IXACT3WaveBank* pWaveBank, __in PCSTR szFriendlyName)
{
    return pWaveBank->GetWaveIndex(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetWaveProperties(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, __out LPXACT_WAVE_PROPERTIES pWaveProperties)
{
    return pWaveBank->GetWaveProperties(nWaveIndex, pWaveProperties);
}

__inline HRESULT __stdcall IXACT3WaveBank_Prepare(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pWaveBank->Prepare(nWaveIndex, dwFlags, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3WaveBank_Play(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pWaveBank->Play(nWaveIndex, dwFlags, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3WaveBank_Stop(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags)
{
    return pWaveBank->Stop(nWaveIndex, dwFlags);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetState(__in IXACT3WaveBank* pWaveBank, __out DWORD* pdwState)
{
    return pWaveBank->GetState(pdwState);
}

#else

__inline HRESULT __stdcall IXACT3WaveBank_Destroy(__in IXACT3WaveBank* pWaveBank)
{
    return pWaveBank->lpVtbl->Destroy(pWaveBank);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetNumWaves(__in IXACT3WaveBank* pWaveBank, __out XACTINDEX* pnNumWaves)
{
    return pWaveBank->lpVtbl->GetNumWaves(pWaveBank, pnNumWaves);
}

__inline XACTINDEX __stdcall IXACT3WaveBank_GetWaveIndex(__in IXACT3WaveBank* pWaveBank, __in PCSTR szFriendlyName)
{
    return pWaveBank->lpVtbl->GetWaveIndex(pWaveBank, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetWaveProperties(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, __out LPXACT_WAVE_PROPERTIES pWaveProperties)
{
    return pWaveBank->lpVtbl->GetWaveProperties(pWaveBank, nWaveIndex, pWaveProperties);
}

__inline HRESULT __stdcall IXACT3WaveBank_Prepare(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pWaveBank->lpVtbl->Prepare(pWaveBank, nWaveIndex, dwFlags, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3WaveBank_Play(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pWaveBank->lpVtbl->Play(pWaveBank, nWaveIndex, dwFlags, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3WaveBank_Stop(__in IXACT3WaveBank* pWaveBank, XACTINDEX nWaveIndex, DWORD dwFlags)
{
    return pWaveBank->lpVtbl->Stop(pWaveBank, nWaveIndex, dwFlags);
}

__inline HRESULT __stdcall IXACT3WaveBank_GetState(__in IXACT3WaveBank* pWaveBank, __out DWORD* pdwState)
{
    return pWaveBank->lpVtbl->GetState(pWaveBank, pdwState);
}
#endif






STDAPI IXACT3Wave_Destroy(__in IXACT3Wave* pWave);
STDAPI IXACT3Wave_Play(__in IXACT3Wave* pWave);
STDAPI IXACT3Wave_Stop(__in IXACT3Wave* pWave, DWORD dwFlags);
STDAPI IXACT3Wave_Pause(__in IXACT3Wave* pWave, BOOL fPause);
STDAPI IXACT3Wave_GetState(__in IXACT3Wave* pWave, __out DWORD* pdwState);
STDAPI IXACT3Wave_SetPitch(__in IXACT3Wave* pWave, XACTPITCH pitch);
STDAPI IXACT3Wave_SetVolume(__in IXACT3Wave* pWave, XACTVOLUME volume);
STDAPI IXACT3Wave_SetMatrixCoefficients(__in IXACT3Wave* pWave, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients);
STDAPI IXACT3Wave_GetProperties(__in IXACT3Wave* pWave, __out LPXACT_WAVE_INSTANCE_PROPERTIES pProperties);

#undef INTERFACE
#define INTERFACE IXACT3Wave

DECLARE_INTERFACE(IXACT3Wave)
{
    STDMETHOD(Destroy)(THIS) PURE;
    STDMETHOD(Play)(THIS) PURE;
    STDMETHOD(Stop)(THIS_ DWORD dwFlags) PURE;
    STDMETHOD(Pause)(THIS_ BOOL fPause) PURE;
    STDMETHOD(GetState)(THIS_ __out DWORD* pdwState) PURE;
    STDMETHOD(SetPitch)(THIS_ XACTPITCH pitch) PURE;
    STDMETHOD(SetVolume)(THIS_ XACTVOLUME volume) PURE;
    STDMETHOD(SetMatrixCoefficients)(THIS_ UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients) PURE;
    STDMETHOD(GetProperties)(THIS_ __out LPXACT_WAVE_INSTANCE_PROPERTIES pProperties) PURE;
};

#ifdef __cplusplus

__inline HRESULT __stdcall IXACT3Wave_Destroy(__in IXACT3Wave* pWave)
{
    return pWave->Destroy();
}

__inline HRESULT __stdcall IXACT3Wave_Play(__in IXACT3Wave* pWave)
{
    return pWave->Play();
}

__inline HRESULT __stdcall IXACT3Wave_Stop(__in IXACT3Wave* pWave, DWORD dwFlags)
{
    return pWave->Stop(dwFlags);
}

__inline HRESULT __stdcall IXACT3Wave_Pause(__in IXACT3Wave* pWave, BOOL fPause)
{
    return pWave->Pause(fPause);
}

__inline HRESULT __stdcall IXACT3Wave_GetState(__in IXACT3Wave* pWave, __out DWORD* pdwState)
{
    return pWave->GetState(pdwState);
}

__inline HRESULT __stdcall IXACT3Wave_SetPitch(__in IXACT3Wave* pWave, XACTPITCH pitch)
{
    return pWave->SetPitch(pitch);
}

__inline HRESULT __stdcall IXACT3Wave_SetVolume(__in IXACT3Wave* pWave, XACTVOLUME volume)
{
    return pWave->SetVolume(volume);
}

__inline HRESULT __stdcall IXACT3Wave_SetMatrixCoefficients(__in IXACT3Wave* pWave, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients)
{
    return pWave->SetMatrixCoefficients(uSrcChannelCount, uDstChannelCount, pMatrixCoefficients);
}

__inline HRESULT __stdcall IXACT3Wave_GetProperties(__in IXACT3Wave* pWave, __out LPXACT_WAVE_INSTANCE_PROPERTIES pProperties)
{
    return pWave->GetProperties(pProperties);
}

#else

__inline HRESULT __stdcall IXACT3Wave_Destroy(__in IXACT3Wave* pWave)
{
    return pWave->lpVtbl->Destroy(pWave);
}

__inline HRESULT __stdcall IXACT3Wave_Play(__in IXACT3Wave* pWave)
{
    return pWave->lpVtbl->Play(pWave);
}

__inline HRESULT __stdcall IXACT3Wave_Stop(__in IXACT3Wave* pWave, DWORD dwFlags)
{
    return pWave->lpVtbl->Stop(pWave, dwFlags);
}

__inline HRESULT __stdcall IXACT3Wave_Pause(__in IXACT3Wave* pWave, BOOL fPause)
{
    return pWave->lpVtbl->Pause(pWave, fPause);
}

__inline HRESULT __stdcall IXACT3Wave_GetState(__in IXACT3Wave* pWave, __out DWORD* pdwState)
{
    return pWave->lpVtbl->GetState(pWave, pdwState);
}

__inline HRESULT __stdcall IXACT3Wave_SetPitch(__in IXACT3Wave* pWave, XACTPITCH pitch)
{
    return pWave->lpVtbl->SetPitch(pWave, pitch);
}

__inline HRESULT __stdcall IXACT3Wave_SetVolume(__in IXACT3Wave* pWave, XACTVOLUME volume)
{
    return pWave->lpVtbl->SetVolume(pWave, volume);
}

__inline HRESULT __stdcall IXACT3Wave_SetMatrixCoefficients(__in IXACT3Wave* pWave, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients)
{
    return pWave->lpVtbl->SetMatrixCoefficients(pWave, uSrcChannelCount, uDstChannelCount, pMatrixCoefficients);
}

__inline HRESULT __stdcall IXACT3Wave_GetProperties(__in IXACT3Wave* pWave, __out LPXACT_WAVE_INSTANCE_PROPERTIES pProperties)
{
    return pWave->lpVtbl->GetProperties(pWave, pProperties);
}
#endif






#define XACT_FLAG_CUE_STOP_RELEASE      XACT_FLAG_STOP_RELEASE
#define XACT_FLAG_CUE_STOP_IMMEDIATE    XACT_FLAG_STOP_IMMEDIATE


#define XACT_CUESTATE_CREATED           XACT_STATE_CREATED
#define XACT_CUESTATE_PREPARING         XACT_STATE_PREPARING
#define XACT_CUESTATE_PREPARED          XACT_STATE_PREPARED
#define XACT_CUESTATE_PLAYING           XACT_STATE_PLAYING
#define XACT_CUESTATE_STOPPING          XACT_STATE_STOPPING
#define XACT_CUESTATE_STOPPED           XACT_STATE_STOPPED
#define XACT_CUESTATE_PAUSED            XACT_STATE_PAUSED

STDAPI IXACT3Cue_Destroy(__in IXACT3Cue* pCue);
STDAPI IXACT3Cue_Play(__in IXACT3Cue* pCue);
STDAPI IXACT3Cue_Stop(__in IXACT3Cue* pCue, DWORD dwFlags);
STDAPI IXACT3Cue_GetState(__in IXACT3Cue* pCue, __out DWORD* pdwState);
STDAPI IXACT3Cue_SetMatrixCoefficients(__in IXACT3Cue*, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients);
STDAPI_(XACTVARIABLEINDEX) IXACT3Cue_GetVariableIndex(__in IXACT3Cue* pCue, __in PCSTR szFriendlyName);
STDAPI IXACT3Cue_SetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue);
STDAPI IXACT3Cue_GetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* nValue);
STDAPI IXACT3Cue_Pause(__in IXACT3Cue* pCue, BOOL fPause);
STDAPI IXACT3Cue_GetProperties(__in IXACT3Cue* pCue, __out LPXACT_CUE_INSTANCE_PROPERTIES* ppProperties);
STDAPI IXACT3Cue_SetOutputVoices(__in IXACT3Cue* pCue, __in_opt const XAUDIO2_VOICE_SENDS* pSendList);
STDAPI IXACT3Cue_SetOutputVoiceMatrix(__in IXACT3Cue* pCue, __in_opt IXAudio2Voice* pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, __in_ecount(SourceChannels * DestinationChannels) const float* pLevelMatrix);

#undef INTERFACE
#define INTERFACE IXACT3Cue

DECLARE_INTERFACE(IXACT3Cue)
{
    STDMETHOD(Play)(THIS) PURE;
    STDMETHOD(Stop)(THIS_ DWORD dwFlags) PURE;
    STDMETHOD(GetState)(THIS_ __out DWORD* pdwState) PURE;
    STDMETHOD(Destroy)(THIS) PURE;
    STDMETHOD(SetMatrixCoefficients)(THIS_ UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients) PURE;
    STDMETHOD_(XACTVARIABLEINDEX, GetVariableIndex)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(SetVariable)(THIS_ XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue) PURE;
    STDMETHOD(GetVariable)(THIS_ XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* nValue) PURE;
    STDMETHOD(Pause)(THIS_ BOOL fPause) PURE;
    STDMETHOD(GetProperties)(THIS_ __out LPXACT_CUE_INSTANCE_PROPERTIES* ppProperties) PURE;
    STDMETHOD(SetOutputVoices)(THIS_ __in_opt const XAUDIO2_VOICE_SENDS* pSendList) PURE;
    STDMETHOD(SetOutputVoiceMatrix)(THIS_ __in_opt IXAudio2Voice* pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, __in_ecount(SourceChannels * DestinationChannels) const float* pLevelMatrix) PURE;
};

#ifdef __cplusplus

__inline HRESULT __stdcall IXACT3Cue_Play(__in IXACT3Cue* pCue)
{
    return pCue->Play();
}

__inline HRESULT __stdcall IXACT3Cue_Stop(__in IXACT3Cue* pCue, DWORD dwFlags)
{
    return pCue->Stop(dwFlags);
}

__inline HRESULT __stdcall IXACT3Cue_GetState(__in IXACT3Cue* pCue, __out DWORD* pdwState)
{
    return pCue->GetState(pdwState);
}

__inline HRESULT __stdcall IXACT3Cue_Destroy(__in IXACT3Cue* pCue)
{
    return pCue->Destroy();
}

__inline HRESULT __stdcall IXACT3Cue_SetMatrixCoefficients(__in IXACT3Cue* pCue, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients)
{
    return pCue->SetMatrixCoefficients(uSrcChannelCount, uDstChannelCount, pMatrixCoefficients);
}

__inline XACTVARIABLEINDEX __stdcall IXACT3Cue_GetVariableIndex(__in IXACT3Cue* pCue, __in PCSTR szFriendlyName)
{
    return pCue->GetVariableIndex(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Cue_SetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue)
{
    return pCue->SetVariable(nIndex, nValue);
}

__inline HRESULT __stdcall IXACT3Cue_GetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* pnValue)
{
    return pCue->GetVariable(nIndex, pnValue);
}

__inline HRESULT __stdcall IXACT3Cue_Pause(__in IXACT3Cue* pCue, BOOL fPause)
{
    return pCue->Pause(fPause);
}

__inline HRESULT __stdcall IXACT3Cue_GetProperties(__in IXACT3Cue* pCue, __out LPXACT_CUE_INSTANCE_PROPERTIES* ppProperties)
{
    return pCue->GetProperties(ppProperties);
}

__inline HRESULT __stdcall IXACT3Cue_SetOutputVoices(__in IXACT3Cue* pCue, __in_opt const XAUDIO2_VOICE_SENDS* pSendList)
{
    return pCue->SetOutputVoices(pSendList);
}

__inline HRESULT __stdcall IXACT3Cue_SetOutputVoiceMatrix(__in IXACT3Cue* pCue, __in_opt IXAudio2Voice* pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, __in_ecount(SourceChannels * DestinationChannels) const float* pLevelMatrix)
{
    return pCue->SetOutputVoiceMatrix(pDestinationVoice, SourceChannels, DestinationChannels, pLevelMatrix);
}

#else

__inline HRESULT __stdcall IXACT3Cue_Play(__in IXACT3Cue* pCue)
{
    return pCue->lpVtbl->Play(pCue);
}

__inline HRESULT __stdcall IXACT3Cue_Stop(__in IXACT3Cue* pCue, DWORD dwFlags)
{
    return pCue->lpVtbl->Stop(pCue, dwFlags);
}

__inline HRESULT __stdcall IXACT3Cue_GetState(__in IXACT3Cue* pCue, __out DWORD* pdwState)
{
    return pCue->lpVtbl->GetState(pCue, pdwState);
}

__inline HRESULT __stdcall IXACT3Cue_Destroy(__in IXACT3Cue* pCue)
{
    return pCue->lpVtbl->Destroy(pCue);
}

__inline HRESULT __stdcall IXACT3Cue_SetMatrixCoefficients(__in IXACT3Cue* pCue, UINT32 uSrcChannelCount, UINT32 uDstChannelCount, __in float* pMatrixCoefficients)
{
    return pCue->lpVtbl->SetMatrixCoefficients(pCue, uSrcChannelCount, uDstChannelCount, pMatrixCoefficients);
}

__inline XACTVARIABLEINDEX __stdcall IXACT3Cue_GetVariableIndex(__in IXACT3Cue* pCue, __in PCSTR szFriendlyName)
{
    return pCue->lpVtbl->GetVariableIndex(pCue, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Cue_SetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue)
{
    return pCue->lpVtbl->SetVariable(pCue, nIndex, nValue);
}

__inline HRESULT __stdcall IXACT3Cue_GetVariable(__in IXACT3Cue* pCue, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* pnValue)
{
    return pCue->lpVtbl->GetVariable(pCue, nIndex, pnValue);
}

__inline HRESULT __stdcall IXACT3Cue_Pause(__in IXACT3Cue* pCue, BOOL fPause)
{
    return pCue->lpVtbl->Pause(pCue, fPause);
}

__inline HRESULT __stdcall IXACT3Cue_GetProperties(__in IXACT3Cue* pCue, __out LPXACT_CUE_INSTANCE_PROPERTIES* ppProperties)
{
    return pCue->lpVtbl->GetProperties(pCue, ppProperties);
}

__inline HRESULT __stdcall IXACT3Cue_SetOutputVoices(__in IXACT3Cue* pCue, __in_opt const XAUDIO2_VOICE_SENDS* pSendList)
{
    return pCue->lpVtbl->SetOutputVoices(pSendList);
}

__inline HRESULT __stdcall IXACT3Cue_SetOutputVoiceMatrix(__in IXACT3Cue* pCue, __in_opt IXAudio2Voice* pDestinationVoice, UINT32 SourceChannels, UINT32 DestinationChannels, __in_ecount(SourceChannels * DestinationChannels) const float* pLevelMatrix)
{
    return pCue->lpVtbl->SetOutputVoiceMatrix(pDestinationVoice, SourceChannels, DestinationChannels, pLevelMatrix);
}

#endif






#define XACT_FLAG_ENGINE_CREATE_MANAGEDATA    XACT_FLAG_MANAGEDATA
#define XACT_FLAG_ENGINE_STOP_IMMEDIATE       XACT_FLAG_STOP_IMMEDIATE

STDAPI_(ULONG) IXACT3Engine_AddRef(__in IXACT3Engine* pEngine);
STDAPI_(ULONG) IXACT3Engine_Release(__in IXACT3Engine* pEngine);
STDAPI IXACT3Engine_GetRendererCount(__in IXACT3Engine* pEngine, __out XACTINDEX* pnRendererCount);
STDAPI IXACT3Engine_GetRendererDetails(__in IXACT3Engine* pEngine, XACTINDEX nRendererIndex, __out LPXACT_RENDERER_DETAILS pRendererDetails);
STDAPI IXACT3Engine_GetFinalMixFormat(__in IXACT3Engine* pEngine, __out WAVEFORMATEXTENSIBLE* pFinalMixFormat);
STDAPI IXACT3Engine_Initialize(__in IXACT3Engine* pEngine, __in const XACT_RUNTIME_PARAMETERS* pParams);
STDAPI IXACT3Engine_ShutDown(__in IXACT3Engine* pEngine);
STDAPI IXACT3Engine_DoWork(__in IXACT3Engine* pEngine);
STDAPI IXACT3Engine_CreateSoundBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3SoundBank** ppSoundBank);
STDAPI IXACT3Engine_CreateInMemoryWaveBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3WaveBank** ppWaveBank);
STDAPI IXACT3Engine_CreateStreamingWaveBank(__in IXACT3Engine* pEngine, __in const XACT_WAVEBANK_STREAMING_PARAMETERS* pParms, __deref_out IXACT3WaveBank** ppWaveBank);
STDAPI IXACT3Engine_PrepareWave(__in IXACT3Engine* pEngine, DWORD dwFlags, __in PCSTR szWavePath, WORD wStreamingPacketSize, DWORD dwAlignment, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3Engine_PrepareInMemoryWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, __in_opt DWORD* pdwSeekTable, __in_opt BYTE* pbWaveData, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3Engine_PrepareStreamingWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, XACT_STREAMING_PARAMETERS streamingParams, DWORD dwAlignment, __in_opt DWORD* pdwSeekTable, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave);
STDAPI IXACT3Engine_RegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc);
STDAPI IXACT3Engine_UnRegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc);
STDAPI_(XACTCATEGORY) IXACT3Engine_GetCategory(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName);
STDAPI IXACT3Engine_Stop(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, DWORD dwFlags);
STDAPI IXACT3Engine_SetVolume(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, XACTVOLUME nVolume);
STDAPI IXACT3Engine_Pause(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, BOOL fPause);
STDAPI_(XACTVARIABLEINDEX) IXACT3Engine_GetGlobalVariableIndex(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName);
STDAPI IXACT3Engine_SetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue);
STDAPI IXACT3Engine_GetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* pnValue);

#undef INTERFACE
#define INTERFACE IXACT3Engine

#ifdef _XBOX
DECLARE_INTERFACE(IXACT3Engine)
{
#else
DECLARE_INTERFACE_(IXACT3Engine, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ __in REFIID riid, __deref_out void** ppvObj) PURE;
#endif

    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    STDMETHOD(GetRendererCount)(THIS_ __out XACTINDEX* pnRendererCount) PURE;
    STDMETHOD(GetRendererDetails)(THIS_ XACTINDEX nRendererIndex, __out LPXACT_RENDERER_DETAILS pRendererDetails) PURE;

    STDMETHOD(GetFinalMixFormat)(THIS_ __out WAVEFORMATEXTENSIBLE* pFinalMixFormat) PURE;
    STDMETHOD(Initialize)(THIS_ __in const XACT_RUNTIME_PARAMETERS* pParams) PURE;
    STDMETHOD(ShutDown)(THIS) PURE;

    STDMETHOD(DoWork)(THIS) PURE;

    STDMETHOD(CreateSoundBank)(THIS_ __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3SoundBank** ppSoundBank) PURE;
    STDMETHOD(CreateInMemoryWaveBank)(THIS_ __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3WaveBank** ppWaveBank) PURE;
    STDMETHOD(CreateStreamingWaveBank)(THIS_ __in const XACT_WAVEBANK_STREAMING_PARAMETERS* pParms, __deref_out IXACT3WaveBank** ppWaveBank) PURE;

    STDMETHOD(PrepareWave)(THIS_ DWORD dwFlags, __in PCSTR szWavePath, WORD wStreamingPacketSize, DWORD dwAlignment, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;
    STDMETHOD(PrepareInMemoryWave)(THIS_ DWORD dwFlags, WAVEBANKENTRY entry, __in_opt DWORD* pdwSeekTable, __in_opt BYTE* pbWaveData, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;
    STDMETHOD(PrepareStreamingWave)(THIS_ DWORD dwFlags, WAVEBANKENTRY entry, XACT_STREAMING_PARAMETERS streamingParams, DWORD dwAlignment, __in_opt DWORD* pdwSeekTable, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave) PURE;

    STDMETHOD(RegisterNotification)(THIS_ __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc) PURE;
    STDMETHOD(UnRegisterNotification)(THIS_ __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc) PURE;

    STDMETHOD_(XACTCATEGORY, GetCategory)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(Stop)(THIS_ XACTCATEGORY nCategory, DWORD dwFlags) PURE;
    STDMETHOD(SetVolume)(THIS_ XACTCATEGORY nCategory, XACTVOLUME nVolume) PURE;
    STDMETHOD(Pause)(THIS_ XACTCATEGORY nCategory, BOOL fPause) PURE;

    STDMETHOD_(XACTVARIABLEINDEX, GetGlobalVariableIndex)(THIS_ __in PCSTR szFriendlyName) PURE;
    STDMETHOD(SetGlobalVariable)(THIS_ XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue) PURE;
    STDMETHOD(GetGlobalVariable)(THIS_ XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* nValue) PURE;
};

#ifdef __cplusplus

__inline ULONG __stdcall IXACT3Engine_AddRef(__in IXACT3Engine* pEngine)
{
    return pEngine->AddRef();
}

__inline ULONG __stdcall IXACT3Engine_Release(__in IXACT3Engine* pEngine)
{
    return pEngine->Release();
}

__inline HRESULT __stdcall IXACT3Engine_GetRendererCount(__in IXACT3Engine* pEngine, __out XACTINDEX* pnRendererCount)
{
    return pEngine->GetRendererCount(pnRendererCount);
}

__inline HRESULT __stdcall IXACT3Engine_GetRendererDetails(__in IXACT3Engine* pEngine, XACTINDEX nRendererIndex, __out LPXACT_RENDERER_DETAILS pRendererDetails)
{
    return pEngine->GetRendererDetails(nRendererIndex, pRendererDetails);
}

__inline HRESULT __stdcall IXACT3Engine_GetFinalMixFormat(__in IXACT3Engine* pEngine, __out WAVEFORMATEXTENSIBLE* pFinalMixFormat)
{
    return pEngine->GetFinalMixFormat(pFinalMixFormat);
}

__inline HRESULT __stdcall IXACT3Engine_Initialize(__in IXACT3Engine* pEngine, __in const XACT_RUNTIME_PARAMETERS* pParams)
{
    return pEngine->Initialize(pParams);
}

__inline HRESULT __stdcall IXACT3Engine_ShutDown(__in IXACT3Engine* pEngine)
{
    return pEngine->ShutDown();
}

__inline HRESULT __stdcall IXACT3Engine_DoWork(__in IXACT3Engine* pEngine)
{
    return pEngine->DoWork();
}

__inline HRESULT __stdcall IXACT3Engine_CreateSoundBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3SoundBank** ppSoundBank)
{
    return pEngine->CreateSoundBank(pvBuffer, dwSize, dwFlags, dwAllocAttributes, ppSoundBank);
}

__inline HRESULT __stdcall IXACT3Engine_CreateInMemoryWaveBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3WaveBank** ppWaveBank)
{
    return pEngine->CreateInMemoryWaveBank(pvBuffer, dwSize, dwFlags, dwAllocAttributes, ppWaveBank);
}

__inline HRESULT __stdcall IXACT3Engine_CreateStreamingWaveBank(__in IXACT3Engine* pEngine, __in const XACT_WAVEBANK_STREAMING_PARAMETERS* pParms, __deref_out IXACT3WaveBank** ppWaveBank)
{
    return pEngine->CreateStreamingWaveBank(pParms, ppWaveBank);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareWave(__in IXACT3Engine* pEngine, DWORD dwFlags, __in PCSTR szWavePath, WORD wStreamingPacketSize, DWORD dwAlignment, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->PrepareWave(dwFlags, szWavePath, wStreamingPacketSize, dwAlignment, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareInMemoryWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, __in_opt DWORD* pdwSeekTable, __in_opt BYTE* pbWaveData, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->PrepareInMemoryWave(dwFlags, entry, pdwSeekTable, pbWaveData, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareStreamingWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, XACT_STREAMING_PARAMETERS streamingParams, DWORD dwAlignment, __in_opt DWORD* pdwSeekTable, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->PrepareStreamingWave(dwFlags, entry, streamingParams, dwAlignment, pdwSeekTable, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3Engine_RegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc)
{
    return pEngine->RegisterNotification(pNotificationDesc);
}

__inline HRESULT __stdcall IXACT3Engine_UnRegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc)
{
    return pEngine->UnRegisterNotification(pNotificationDesc);
}

__inline XACTCATEGORY __stdcall IXACT3Engine_GetCategory(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName)
{
    return pEngine->GetCategory(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Engine_Stop(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, DWORD dwFlags)
{
    return pEngine->Stop(nCategory, dwFlags);
}

__inline HRESULT __stdcall IXACT3Engine_SetVolume(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, XACTVOLUME nVolume)
{
    return pEngine->SetVolume(nCategory, nVolume);
}

__inline HRESULT __stdcall IXACT3Engine_Pause(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, BOOL fPause)
{
    return pEngine->Pause(nCategory, fPause);
}

__inline XACTVARIABLEINDEX __stdcall IXACT3Engine_GetGlobalVariableIndex(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName)
{
    return pEngine->GetGlobalVariableIndex(szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Engine_SetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue)
{
    return pEngine->SetGlobalVariable(nIndex, nValue);
}

__inline HRESULT __stdcall IXACT3Engine_GetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* nValue)
{
    return pEngine->GetGlobalVariable(nIndex, nValue);
}

#else

__inline ULONG __stdcall IXACT3Engine_AddRef(__in IXACT3Engine* pEngine)
{
    return pEngine->lpVtbl->AddRef(pEngine);
}

__inline ULONG __stdcall IXACT3Engine_Release(__in IXACT3Engine* pEngine)
{
    return pEngine->lpVtbl->Release(pEngine);
}

__inline HRESULT __stdcall IXACT3Engine_GetRendererCount(__in IXACT3Engine* pEngine, __out XACTINDEX* pnRendererCount)
{
    return pEngine->lpVtbl->GetRendererCount(pEngine, pnRendererCount);
}

__inline HRESULT __stdcall IXACT3Engine_GetRendererDetails(__in IXACT3Engine* pEngine, XACTINDEX nRendererIndex, __out LPXACT_RENDERER_DETAILS pRendererDetails)
{
    return pEngine->lpVtbl->GetRendererDetails(pEngine, nRendererIndex, pRendererDetails);
}

__inline HRESULT __stdcall IXACT3Engine_GetFinalMixFormat(__in IXACT3Engine* pEngine, __out WAVEFORMATEXTENSIBLE* pFinalMixFormat)
{
    return pEngine->lpVtbl->GetFinalMixFormat(pEngine, pFinalMixFormat);
}

__inline HRESULT __stdcall IXACT3Engine_Initialize(__in IXACT3Engine* pEngine, __in const XACT_RUNTIME_PARAMETERS* pParams)
{
    return pEngine->lpVtbl->Initialize(pEngine, pParams);
}

__inline HRESULT __stdcall IXACT3Engine_ShutDown(__in IXACT3Engine* pEngine)
{
    return pEngine->lpVtbl->ShutDown(pEngine);
}

__inline HRESULT __stdcall IXACT3Engine_DoWork(__in IXACT3Engine* pEngine)
{
    return pEngine->lpVtbl->DoWork(pEngine);
}

__inline HRESULT __stdcall IXACT3Engine_CreateSoundBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3SoundBank** ppSoundBank)
{
    return pEngine->lpVtbl->CreateSoundBank(pEngine, pvBuffer, dwSize, dwFlags, dwAllocAttributes, ppSoundBank);
}

__inline HRESULT __stdcall IXACT3Engine_CreateInMemoryWaveBank(__in IXACT3Engine* pEngine, __in const void* pvBuffer, DWORD dwSize, DWORD dwFlags, DWORD dwAllocAttributes, __deref_out IXACT3WaveBank** ppWaveBank)
{
    return pEngine->lpVtbl->CreateInMemoryWaveBank(pEngine, pvBuffer, dwSize, dwFlags, dwAllocAttributes, ppWaveBank);
}

__inline HRESULT __stdcall IXACT3Engine_CreateStreamingWaveBank(__in IXACT3Engine* pEngine, __in const XACT_WAVEBANK_STREAMING_PARAMETERS* pParms, __deref_out IXACT3WaveBank** ppWaveBank)
{
    return pEngine->lpVtbl->CreateStreamingWaveBank(pEngine, pParms, ppWaveBank);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareWave(__in IXACT3Engine* pEngine, DWORD dwFlags, __in PCSTR szWavePath, WORD wStreamingPacketSize, DWORD dwAlignment, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->lpVtbl->PrepareWave(pEngine, dwFlags, szWavePath, wStreamingPacketSize, dwAlignment, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareInMemoryWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, __in_opt DWORD* pdwSeekTable, __in_opt BYTE* pbWaveData, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->lpVtbl->PrepareInMemoryWave(pEngine, dwFlags, entry, pdwSeekTable, pbWaveData, dwPlayOffset, nLoopCount, ppWave);
}

__inline HRESULT __stdcall IXACT3Engine_PrepareStreamingWave(__in IXACT3Engine* pEngine, DWORD dwFlags, WAVEBANKENTRY entry, XACT_STREAMING_PARAMETERS streamingParams, DWORD dwAlignment, __in_opt DWORD* pdwSeekTable, DWORD dwPlayOffset, XACTLOOPCOUNT nLoopCount, __deref_out IXACT3Wave** ppWave)
{
    return pEngine->lpVtbl->PrepareStreamingWave(pEngine, dwFlags, entry, streamingParams, dwAlignment, pdwSeekTable, dwPlayOffset, nLoopCount, ppWave);
}


__inline HRESULT __stdcall IXACT3Engine_RegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc)
{
    return pEngine->lpVtbl->RegisterNotification(pEngine, pNotificationDesc);
}

__inline HRESULT __stdcall IXACT3Engine_UnRegisterNotification(__in IXACT3Engine* pEngine, __in const XACT_NOTIFICATION_DESCRIPTION* pNotificationDesc)
{
    return pEngine->lpVtbl->UnRegisterNotification(pEngine, pNotificationDesc);
}

__inline XACTCATEGORY __stdcall IXACT3Engine_GetCategory(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName)
{
    return pEngine->lpVtbl->GetCategory(pEngine, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Engine_Stop(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, DWORD dwFlags)
{
    return pEngine->lpVtbl->Stop(pEngine, nCategory, dwFlags);
}

__inline HRESULT __stdcall IXACT3Engine_SetVolume(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, XACTVOLUME nVolume)
{
    return pEngine->lpVtbl->SetVolume(pEngine, nCategory, nVolume);
}

__inline HRESULT __stdcall IXACT3Engine_Pause(__in IXACT3Engine* pEngine, XACTCATEGORY nCategory, BOOL fPause)
{
    return pEngine->lpVtbl->Pause(pEngine, nCategory, fPause);
}

__inline XACTVARIABLEINDEX __stdcall IXACT3Engine_GetGlobalVariableIndex(__in IXACT3Engine* pEngine, __in PCSTR szFriendlyName)
{
    return pEngine->lpVtbl->GetGlobalVariableIndex(pEngine, szFriendlyName);
}

__inline HRESULT __stdcall IXACT3Engine_SetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, XACTVARIABLEVALUE nValue)
{
    return pEngine->lpVtbl->SetGlobalVariable(pEngine, nIndex, nValue);
}

__inline HRESULT __stdcall IXACT3Engine_GetGlobalVariable(__in IXACT3Engine* pEngine, XACTVARIABLEINDEX nIndex, __out XACTVARIABLEVALUE* nValue)
{
    return pEngine->lpVtbl->GetGlobalVariable(pEngine, nIndex, nValue);
}

#endif








static const DWORD XACT_FLAG_API_AUDITION_MODE = 0x00000001;
static const DWORD XACT_FLAG_API_DEBUG_MODE    = 0x00000002;

#ifdef _XBOX

STDAPI XACT3CreateEngine(DWORD dwCreationFlags, __deref_out IXACT3Engine** ppEngine);

#else

#define XACT_DEBUGENGINE_REGISTRY_KEY   TEXT("Software\\Microsoft\\XACT")
#define XACT_DEBUGENGINE_REGISTRY_VALUE TEXT("DebugEngine")


#ifdef __cplusplus

__inline HRESULT __stdcall XACT3CreateEngine(DWORD dwCreationFlags, __deref_out IXACT3Engine** ppEngine)
{
    HRESULT hr;
    HKEY    key;
    DWORD   data;
    DWORD   type     = REG_DWORD;
    DWORD   dataSize = sizeof(DWORD);
    BOOL    debug    = (dwCreationFlags & XACT_FLAG_API_DEBUG_MODE) ? TRUE : FALSE;
    BOOL    audition = (dwCreationFlags & XACT_FLAG_API_AUDITION_MODE) ? TRUE : FALSE;


    if(!debug && !audition &&
       (RegOpenKeyEx(HKEY_LOCAL_MACHINE, XACT_DEBUGENGINE_REGISTRY_KEY, 0, KEY_READ, &key) == ERROR_SUCCESS))
    {
        if(RegQueryValueEx(key, XACT_DEBUGENGINE_REGISTRY_VALUE, NULL, &type, (LPBYTE)&data, &dataSize) == ERROR_SUCCESS)
        {
            if(data)
            {
                debug = TRUE;
            }
        }
        RegCloseKey(key);
    }


    hr = CoCreateInstance(audition ? __uuidof(XACTAuditionEngine)
                          : (debug ? __uuidof(XACTDebugEngine) : __uuidof(XACTEngine)),
                          NULL, CLSCTX_INPROC_SERVER, __uuidof(IXACT3Engine), (void**)ppEngine);


    if(FAILED(hr) && debug && !audition)
    {
        hr = CoCreateInstance(__uuidof(XACTEngine), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXACT3Engine), (void**)ppEngine);
    }

    return hr;
}

#else

__inline HRESULT __stdcall XACT3CreateEngine(DWORD dwCreationFlags, __deref_out IXACT3Engine** ppEngine)
{
    HRESULT hr;
    HKEY    key;
    DWORD   data;
    DWORD   type     = REG_DWORD;
    DWORD   dataSize = sizeof(DWORD);
    BOOL    debug    = (dwCreationFlags & XACT_FLAG_API_DEBUG_MODE) ? TRUE : FALSE;
    BOOL    audition = (dwCreationFlags & XACT_FLAG_API_AUDITION_MODE) ? TRUE : FALSE;


    if(!debug && !audition &&
       (RegOpenKeyEx(HKEY_LOCAL_MACHINE, XACT_DEBUGENGINE_REGISTRY_KEY, 0, KEY_READ, &key) == ERROR_SUCCESS))
    {
        if(RegQueryValueEx(key, XACT_DEBUGENGINE_REGISTRY_VALUE, NULL, &type, (LPBYTE)&data, &dataSize) == ERROR_SUCCESS)
        {
            if(data)
            {
                debug = TRUE;
            }
        }
        RegCloseKey(key);
    }


    hr = CoCreateInstance(audition ? &CLSID_XACTAuditionEngine
                          : (debug ? &CLSID_XACTDebugEngine : &CLSID_XACTEngine),
                          NULL, CLSCTX_INPROC_SERVER, &IID_IXACT3Engine, (void**)ppEngine);


    if(FAILED(hr) && debug && !audition)
    {
        hr = CoCreateInstance(&CLSID_XACTEngine, NULL, CLSCTX_INPROC_SERVER, &IID_IXACT3Engine, (void**)ppEngine);
    }

    return hr;
}

#endif

#endif





#define FACILITY_XACTENGINE 0xAC7
#define XACTENGINEERROR(n) MAKE_HRESULT(SEVERITY_ERROR, FACILITY_XACTENGINE, n)

#define XACTENGINE_E_OUTOFMEMORY               E_OUTOFMEMORY
#define XACTENGINE_E_INVALIDARG                E_INVALIDARG
#define XACTENGINE_E_NOTIMPL                   E_NOTIMPL
#define XACTENGINE_E_FAIL                      E_FAIL

#define XACTENGINE_E_ALREADYINITIALIZED        XACTENGINEERROR(0x001)
#define XACTENGINE_E_NOTINITIALIZED            XACTENGINEERROR(0x002)
#define XACTENGINE_E_EXPIRED                   XACTENGINEERROR(0x003)
#define XACTENGINE_E_NONOTIFICATIONCALLBACK    XACTENGINEERROR(0x004)
#define XACTENGINE_E_NOTIFICATIONREGISTERED    XACTENGINEERROR(0x005)
#define XACTENGINE_E_INVALIDUSAGE              XACTENGINEERROR(0x006)
#define XACTENGINE_E_INVALIDDATA               XACTENGINEERROR(0x007)
#define XACTENGINE_E_INSTANCELIMITFAILTOPLAY   XACTENGINEERROR(0x008)
#define XACTENGINE_E_NOGLOBALSETTINGS          XACTENGINEERROR(0x009)
#define XACTENGINE_E_INVALIDVARIABLEINDEX      XACTENGINEERROR(0x00a)
#define XACTENGINE_E_INVALIDCATEGORY           XACTENGINEERROR(0x00b)
#define XACTENGINE_E_INVALIDCUEINDEX           XACTENGINEERROR(0x00c)
#define XACTENGINE_E_INVALIDWAVEINDEX          XACTENGINEERROR(0x00d)
#define XACTENGINE_E_INVALIDTRACKINDEX         XACTENGINEERROR(0x00e)
#define XACTENGINE_E_INVALIDSOUNDOFFSETORINDEX XACTENGINEERROR(0x00f)
#define XACTENGINE_E_READFILE                  XACTENGINEERROR(0x010)
#define XACTENGINE_E_UNKNOWNEVENT              XACTENGINEERROR(0x011)
#define XACTENGINE_E_INCALLBACK                XACTENGINEERROR(0x012)
#define XACTENGINE_E_NOWAVEBANK                XACTENGINEERROR(0x013)
#define XACTENGINE_E_SELECTVARIATION           XACTENGINEERROR(0x014)
#define XACTENGINE_E_MULTIPLEAUDITIONENGINES   XACTENGINEERROR(0x015)
#define XACTENGINE_E_WAVEBANKNOTPREPARED       XACTENGINEERROR(0x016)
#define XACTENGINE_E_NORENDERER                XACTENGINEERROR(0x017)
#define XACTENGINE_E_INVALIDENTRYCOUNT         XACTENGINEERROR(0x018)
#define XACTENGINE_E_SEEKTIMEBEYONDCUEEND      XACTENGINEERROR(0x019)
#define XACTENGINE_E_SEEKTIMEBEYONDWAVEEND     XACTENGINEERROR(0x01a)
#define XACTENGINE_E_NOFRIENDLYNAMES           XACTENGINEERROR(0x01b)

#define XACTENGINE_E_AUDITION_WRITEFILE             XACTENGINEERROR(0x101)
#define XACTENGINE_E_AUDITION_NOSOUNDBANK           XACTENGINEERROR(0x102)
#define XACTENGINE_E_AUDITION_INVALIDRPCINDEX       XACTENGINEERROR(0x103)
#define XACTENGINE_E_AUDITION_MISSINGDATA           XACTENGINEERROR(0x104)
#define XACTENGINE_E_AUDITION_UNKNOWNCOMMAND        XACTENGINEERROR(0x105)
#define XACTENGINE_E_AUDITION_INVALIDDSPINDEX       XACTENGINEERROR(0x106)
#define XACTENGINE_E_AUDITION_MISSINGWAVE           XACTENGINEERROR(0x107)
#define XACTENGINE_E_AUDITION_CREATEDIRECTORYFAILED XACTENGINEERROR(0x108)
#define XACTENGINE_E_AUDITION_INVALIDSESSION        XACTENGINEERROR(0x109)

#endif

#endif
