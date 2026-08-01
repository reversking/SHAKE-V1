








#ifndef __XAUDIO2_INCLUDED__
#define __XAUDIO2_INCLUDED__








#include <comdecl.h>






























DEFINE_CLSID(XAudio2, 5a508685, a254, 4fba, 9b, 82, 9a, 24, b0, 03, 06, af);
DEFINE_CLSID(XAudio2_Debug, db05ea35, 0329, 4d4b, a5, 3a, 6d, ea, d0, 3d, 38, 52);
DEFINE_IID(IXAudio2, 8bcf1f58, 9fe7, 4583, 8a, c6, e2, ad, c4, 65, c8, bb);



#ifndef GUID_DEFS_ONLY

#ifdef _XBOX
    #include <xobjbase.h>
#else
    #include <objbase.h>
#endif

#include <sal.h>
#include <audiodefs.h>
#include <xma2defs.h>


#pragma pack(push, 1)









#define XAUDIO2_MAX_BUFFER_BYTES        0x80000000
#define XAUDIO2_MAX_QUEUED_BUFFERS      64
#define XAUDIO2_MAX_BUFFERS_SYSTEM      2
#define XAUDIO2_MAX_AUDIO_CHANNELS      64
#define XAUDIO2_MIN_SAMPLE_RATE         1000
#define XAUDIO2_MAX_SAMPLE_RATE         200000
#define XAUDIO2_MAX_VOLUME_LEVEL        16777216.0f
#define XAUDIO2_MIN_FREQ_RATIO          (1/1024.0f)
#define XAUDIO2_MAX_FREQ_RATIO          1024.0f
#define XAUDIO2_DEFAULT_FREQ_RATIO      2.0f
#define XAUDIO2_MAX_FILTER_ONEOVERQ     1.5f
#define XAUDIO2_MAX_FILTER_FREQUENCY    1.0f
#define XAUDIO2_MAX_LOOP_COUNT          254
#define XAUDIO2_MAX_INSTANCES           8




#define XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MONO         600000
#define XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MULTICHANNEL 300000


#define XAUDIO2_COMMIT_NOW              0
#define XAUDIO2_COMMIT_ALL              0
#define XAUDIO2_INVALID_OPSET           (UINT32)(-1)
#define XAUDIO2_NO_LOOP_REGION          0
#define XAUDIO2_LOOP_INFINITE           255
#define XAUDIO2_DEFAULT_CHANNELS        0
#define XAUDIO2_DEFAULT_SAMPLERATE      0


#define XAUDIO2_DEBUG_ENGINE            0x0001
#define XAUDIO2_VOICE_NOPITCH           0x0002
#define XAUDIO2_VOICE_NOSRC             0x0004
#define XAUDIO2_VOICE_USEFILTER         0x0008
#define XAUDIO2_VOICE_MUSIC             0x0010
#define XAUDIO2_PLAY_TAILS              0x0020
#define XAUDIO2_END_OF_STREAM           0x0040
#define XAUDIO2_SEND_USEFILTER          0x0080


#define XAUDIO2_DEFAULT_FILTER_TYPE     LowPassFilter
#define XAUDIO2_DEFAULT_FILTER_FREQUENCY XAUDIO2_MAX_FILTER_FREQUENCY
#define XAUDIO2_DEFAULT_FILTER_ONEOVERQ 1.0f


#ifdef _XBOX
    #define XAUDIO2_QUANTUM_NUMERATOR   2
    #define XAUDIO2_QUANTUM_DENOMINATOR 375
#else
    #define XAUDIO2_QUANTUM_NUMERATOR   1
    #define XAUDIO2_QUANTUM_DENOMINATOR 100
#endif
#define XAUDIO2_QUANTUM_MS (1000.0f * XAUDIO2_QUANTUM_NUMERATOR / XAUDIO2_QUANTUM_DENOMINATOR)


#define FACILITY_XAUDIO2 0x896
#define XAUDIO2_E_INVALID_CALL          0x88960001
#define XAUDIO2_E_XMA_DECODER_ERROR     0x88960002
#define XAUDIO2_E_XAPO_CREATION_FAILED  0x88960003
#define XAUDIO2_E_DEVICE_INVALIDATED    0x88960004








#ifdef __cplusplus
    #define FWD_DECLARE(x) interface x
#else
    #define FWD_DECLARE(x) typedef interface x x
#endif

FWD_DECLARE(IXAudio2);
FWD_DECLARE(IXAudio2Voice);
FWD_DECLARE(IXAudio2SourceVoice);
FWD_DECLARE(IXAudio2SubmixVoice);
FWD_DECLARE(IXAudio2MasteringVoice);
FWD_DECLARE(IXAudio2EngineCallback);
FWD_DECLARE(IXAudio2VoiceCallback);









#ifdef _XBOX
    typedef enum XAUDIO2_XBOX_HWTHREAD_SPECIFIER
    {
        XboxThread0 = 0x01,
        XboxThread1 = 0x02,
        XboxThread2 = 0x04,
        XboxThread3 = 0x08,
        XboxThread4 = 0x10,
        XboxThread5 = 0x20,
        XAUDIO2_ANY_PROCESSOR = XboxThread4,
        XAUDIO2_DEFAULT_PROCESSOR = XAUDIO2_ANY_PROCESSOR
    } XAUDIO2_XBOX_HWTHREAD_SPECIFIER, XAUDIO2_PROCESSOR;
#else
    typedef enum XAUDIO2_WINDOWS_PROCESSOR_SPECIFIER
    {
        Processor1  = 0x00000001,
        Processor2  = 0x00000002,
        Processor3  = 0x00000004,
        Processor4  = 0x00000008,
        Processor5  = 0x00000010,
        Processor6  = 0x00000020,
        Processor7  = 0x00000040,
        Processor8  = 0x00000080,
        Processor9  = 0x00000100,
        Processor10 = 0x00000200,
        Processor11 = 0x00000400,
        Processor12 = 0x00000800,
        Processor13 = 0x00001000,
        Processor14 = 0x00002000,
        Processor15 = 0x00004000,
        Processor16 = 0x00008000,
        Processor17 = 0x00010000,
        Processor18 = 0x00020000,
        Processor19 = 0x00040000,
        Processor20 = 0x00080000,
        Processor21 = 0x00100000,
        Processor22 = 0x00200000,
        Processor23 = 0x00400000,
        Processor24 = 0x00800000,
        Processor25 = 0x01000000,
        Processor26 = 0x02000000,
        Processor27 = 0x04000000,
        Processor28 = 0x08000000,
        Processor29 = 0x10000000,
        Processor30 = 0x20000000,
        Processor31 = 0x40000000,
        Processor32 = 0x80000000,
        XAUDIO2_ANY_PROCESSOR = 0xffffffff,
        XAUDIO2_DEFAULT_PROCESSOR = XAUDIO2_ANY_PROCESSOR
    } XAUDIO2_WINDOWS_PROCESSOR_SPECIFIER, XAUDIO2_PROCESSOR;
#endif




typedef enum XAUDIO2_DEVICE_ROLE
{
    NotDefaultDevice            = 0x0,
    DefaultConsoleDevice        = 0x1,
    DefaultMultimediaDevice     = 0x2,
    DefaultCommunicationsDevice = 0x4,
    DefaultGameDevice           = 0x8,
    GlobalDefaultDevice         = 0xf,
    InvalidDeviceRole = ~GlobalDefaultDevice
} XAUDIO2_DEVICE_ROLE;


typedef struct XAUDIO2_DEVICE_DETAILS
{
    WCHAR DeviceID[256];
    WCHAR DisplayName[256];
    XAUDIO2_DEVICE_ROLE Role;
    WAVEFORMATEXTENSIBLE OutputFormat;
} XAUDIO2_DEVICE_DETAILS;


typedef struct XAUDIO2_VOICE_DETAILS
{
    UINT32 CreationFlags;
    UINT32 InputChannels;
    UINT32 InputSampleRate;
} XAUDIO2_VOICE_DETAILS;


typedef struct XAUDIO2_SEND_DESCRIPTOR
{
    UINT32 Flags;
    IXAudio2Voice* pOutputVoice;
} XAUDIO2_SEND_DESCRIPTOR;


typedef struct XAUDIO2_VOICE_SENDS
{
    UINT32 SendCount;
    XAUDIO2_SEND_DESCRIPTOR* pSends;
} XAUDIO2_VOICE_SENDS;


typedef struct XAUDIO2_EFFECT_DESCRIPTOR
{
    IUnknown* pEffect;
    BOOL InitialState;
    UINT32 OutputChannels;
} XAUDIO2_EFFECT_DESCRIPTOR;


typedef struct XAUDIO2_EFFECT_CHAIN
{
    UINT32 EffectCount;
    XAUDIO2_EFFECT_DESCRIPTOR* pEffectDescriptors;
} XAUDIO2_EFFECT_CHAIN;


typedef enum XAUDIO2_FILTER_TYPE
{
    LowPassFilter,
    BandPassFilter,
    HighPassFilter,
    NotchFilter
} XAUDIO2_FILTER_TYPE;


typedef struct XAUDIO2_FILTER_PARAMETERS
{
    XAUDIO2_FILTER_TYPE Type;
    float Frequency;


    float OneOverQ;

} XAUDIO2_FILTER_PARAMETERS;


typedef struct XAUDIO2_BUFFER
{
    UINT32 Flags;
    UINT32 AudioBytes;
    const BYTE* pAudioData;
    UINT32 PlayBegin;
    UINT32 PlayLength;

    UINT32 LoopBegin;
    UINT32 LoopLength;

    UINT32 LoopCount;

    void* pContext;
} XAUDIO2_BUFFER;








typedef struct XAUDIO2_BUFFER_WMA
{
    const UINT32* pDecodedPacketCumulativeBytes;



    UINT32 PacketCount;

} XAUDIO2_BUFFER_WMA;


typedef struct XAUDIO2_VOICE_STATE
{
    void* pCurrentBufferContext;


    UINT32 BuffersQueued;

    UINT64 SamplesPlayed;

} XAUDIO2_VOICE_STATE;


typedef struct XAUDIO2_PERFORMANCE_DATA
{

    UINT64 AudioCyclesSinceLastQuery;

    UINT64 TotalCyclesSinceLastQuery;

    UINT32 MinimumCyclesPerQuantum;

    UINT32 MaximumCyclesPerQuantum;



    UINT32 MemoryUsageInBytes;


    UINT32 CurrentLatencyInSamples;

    UINT32 GlitchesSinceEngineStarted;


    UINT32 ActiveSourceVoiceCount;
    UINT32 TotalSourceVoiceCount;
    UINT32 ActiveSubmixVoiceCount;

    UINT32 ActiveResamplerCount;
    UINT32 ActiveMatrixMixCount;


    UINT32 ActiveXmaSourceVoices;
    UINT32 ActiveXmaStreams;
} XAUDIO2_PERFORMANCE_DATA;


typedef struct XAUDIO2_DEBUG_CONFIGURATION
{
    UINT32 TraceMask;
    UINT32 BreakMask;
    BOOL LogThreadID;
    BOOL LogFileline;
    BOOL LogFunctionName;
    BOOL LogTiming;
} XAUDIO2_DEBUG_CONFIGURATION;





#define XAUDIO2_LOG_ERRORS     0x0001
#define XAUDIO2_LOG_WARNINGS   0x0002
#define XAUDIO2_LOG_INFO       0x0004
#define XAUDIO2_LOG_DETAIL     0x0008
#define XAUDIO2_LOG_API_CALLS  0x0010
#define XAUDIO2_LOG_FUNC_CALLS 0x0020
#define XAUDIO2_LOG_TIMING     0x0040
#define XAUDIO2_LOG_LOCKS      0x0080
#define XAUDIO2_LOG_MEMORY     0x0100
#define XAUDIO2_LOG_STREAMING  0x1000









#ifdef __cplusplus
    #define X2DEFAULT(x) =x
#else
    #define X2DEFAULT(x)
#endif

#undef INTERFACE
#define INTERFACE IXAudio2
DECLARE_INTERFACE_(IXAudio2, IUnknown)
{








    STDMETHOD(QueryInterface) (THIS_ REFIID riid, __deref_out void** ppvInterface) PURE;




    STDMETHOD_(ULONG, AddRef) (THIS) PURE;




    STDMETHOD_(ULONG, Release) (THIS) PURE;







    STDMETHOD(GetDeviceCount) (THIS_ __out UINT32* pCount) PURE;








    STDMETHOD(GetDeviceDetails) (THIS_ UINT32 Index, __out XAUDIO2_DEVICE_DETAILS* pDeviceDetails) PURE;











    STDMETHOD(Initialize) (THIS_ UINT32 Flags X2DEFAULT(0),
                           XAUDIO2_PROCESSOR XAudio2Processor X2DEFAULT(XAUDIO2_DEFAULT_PROCESSOR)) PURE;







    STDMETHOD(RegisterForCallbacks) (__in IXAudio2EngineCallback* pCallback) PURE;







    STDMETHOD_(void, UnregisterForCallbacks) (__in IXAudio2EngineCallback* pCallback) PURE;













    STDMETHOD(CreateSourceVoice) (THIS_ __deref_out IXAudio2SourceVoice** ppSourceVoice,
                                  __in const WAVEFORMATEX* pSourceFormat,
                                  UINT32 Flags X2DEFAULT(0),
                                  float MaxFrequencyRatio X2DEFAULT(XAUDIO2_DEFAULT_FREQ_RATIO),
                                  __in_opt IXAudio2VoiceCallback* pCallback X2DEFAULT(NULL),
                                  __in_opt const XAUDIO2_VOICE_SENDS* pSendList X2DEFAULT(NULL),
                                  __in_opt const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL)) PURE;













    STDMETHOD(CreateSubmixVoice) (THIS_ __deref_out IXAudio2SubmixVoice** ppSubmixVoice,
                                  UINT32 InputChannels, UINT32 InputSampleRate,
                                  UINT32 Flags X2DEFAULT(0), UINT32 ProcessingStage X2DEFAULT(0),
                                  __in_opt const XAUDIO2_VOICE_SENDS* pSendList X2DEFAULT(NULL),
                                  __in_opt const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL)) PURE;













    STDMETHOD(CreateMasteringVoice) (THIS_ __deref_out IXAudio2MasteringVoice** ppMasteringVoice,
                                     UINT32 InputChannels X2DEFAULT(XAUDIO2_DEFAULT_CHANNELS),
                                     UINT32 InputSampleRate X2DEFAULT(XAUDIO2_DEFAULT_SAMPLERATE),
                                     UINT32 Flags X2DEFAULT(0), UINT32 DeviceIndex X2DEFAULT(0),
                                     __in_opt const XAUDIO2_EFFECT_CHAIN* pEffectChain X2DEFAULT(NULL)) PURE;




    STDMETHOD(StartEngine) (THIS) PURE;




    STDMETHOD_(void, StopEngine) (THIS) PURE;








    STDMETHOD(CommitChanges) (THIS_ UINT32 OperationSet) PURE;







    STDMETHOD_(void, GetPerformanceData) (THIS_ __out XAUDIO2_PERFORMANCE_DATA* pPerfData) PURE;








    STDMETHOD_(void, SetDebugConfiguration) (THIS_ __in_opt const XAUDIO2_DEBUG_CONFIGURATION* pDebugConfiguration,
                                             __in_opt __reserved void* pReserved X2DEFAULT(NULL)) PURE;
};








#undef INTERFACE
#define INTERFACE IXAudio2Voice
DECLARE_INTERFACE(IXAudio2Voice)
{



    #define Declare_IXAudio2Voice_Methods() \
    \





\
    STDMETHOD_(void, GetVoiceDetails) (THIS_ __out XAUDIO2_VOICE_DETAILS* pVoiceDetails) PURE; \
    \






\
    STDMETHOD(SetOutputVoices) (THIS_ __in_opt const XAUDIO2_VOICE_SENDS* pSendList) PURE; \
    \





\
    STDMETHOD(SetEffectChain) (THIS_ __in_opt const XAUDIO2_EFFECT_CHAIN* pEffectChain) PURE; \
    \






\
    STDMETHOD(EnableEffect) (THIS_ UINT32 EffectIndex, \
                             UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE; \
    \






\
    STDMETHOD(DisableEffect) (THIS_ UINT32 EffectIndex, \
                              UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE; \
    \






\
    STDMETHOD_(void, GetEffectState) (THIS_ UINT32 EffectIndex, __out BOOL* pEnabled) PURE; \
    \













\
    STDMETHOD(SetEffectParameters) (THIS_ UINT32 EffectIndex, \
                                    __in_bcount(ParametersByteSize) const void* pParameters, \
                                    UINT32 ParametersByteSize, \
                                    UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE; \
    \







\
    STDMETHOD(GetEffectParameters) (THIS_ UINT32 EffectIndex, \
                                    __out_bcount(ParametersByteSize) void* pParameters, \
                                    UINT32 ParametersByteSize) PURE; \
    \






\
    STDMETHOD(SetFilterParameters) (THIS_ __in const XAUDIO2_FILTER_PARAMETERS* pParameters, \
                                    UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE; \
    \





\
    STDMETHOD_(void, GetFilterParameters) (THIS_ __out XAUDIO2_FILTER_PARAMETERS* pParameters) PURE; \
    \







\
    STDMETHOD(SetOutputFilterParameters) (THIS_ __in_opt IXAudio2Voice* pDestinationVoice, \
                                          __in const XAUDIO2_FILTER_PARAMETERS* pParameters, \
                                          UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE; \
    \






\
    STDMETHOD_(void, GetOutputFilterParameters) (THIS_ __in_opt IXAudio2Voice* pDestinationVoice, \
                                                 __out XAUDIO2_FILTER_PARAMETERS* pParameters) PURE; \
    \






\
    STDMETHOD(SetVolume) (THIS_ float Volume, \
                          UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE; \
    \





\
    STDMETHOD_(void, GetVolume) (THIS_ __out float* pVolume) PURE; \
    \







\
    STDMETHOD(SetChannelVolumes) (THIS_ UINT32 Channels, __in_ecount(Channels) const float* pVolumes, \
                                  UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE; \
    \






\
    STDMETHOD_(void, GetChannelVolumes) (THIS_ UINT32 Channels, __out_ecount(Channels) float* pVolumes) PURE; \
    \














\
    STDMETHOD(SetOutputMatrix) (THIS_ __in_opt IXAudio2Voice* pDestinationVoice, \
                                UINT32 SourceChannels, UINT32 DestinationChannels, \
                                __in_ecount(SourceChannels * DestinationChannels) const float* pLevelMatrix, \
                                UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE; \
    \











\
    STDMETHOD_(void, GetOutputMatrix) (THIS_ __in_opt IXAudio2Voice* pDestinationVoice, \
                                       UINT32 SourceChannels, UINT32 DestinationChannels, \
                                       __out_ecount(SourceChannels * DestinationChannels) float* pLevelMatrix) PURE; \
    \



\
    STDMETHOD_(void, DestroyVoice) (THIS) PURE

    Declare_IXAudio2Voice_Methods();
};








#undef INTERFACE
#define INTERFACE IXAudio2SourceVoice
DECLARE_INTERFACE_(IXAudio2SourceVoice, IXAudio2Voice)
{

    Declare_IXAudio2Voice_Methods();








    STDMETHOD(Start) (THIS_ UINT32 Flags X2DEFAULT(0), UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE;








    STDMETHOD(Stop) (THIS_ UINT32 Flags X2DEFAULT(0), UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE;








    STDMETHOD(SubmitSourceBuffer) (THIS_ __in const XAUDIO2_BUFFER* pBuffer, __in_opt const XAUDIO2_BUFFER_WMA* pBufferWMA X2DEFAULT(NULL)) PURE;




    STDMETHOD(FlushSourceBuffers) (THIS) PURE;






    STDMETHOD(Discontinuity) (THIS) PURE;







    STDMETHOD(ExitLoop) (THIS_ UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE;









    STDMETHOD_(void, GetState) (THIS_ __out XAUDIO2_VOICE_STATE* pVoiceState) PURE;








    STDMETHOD(SetFrequencyRatio) (THIS_ float Ratio,
                                  UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE;







    STDMETHOD_(void, GetFrequencyRatio) (THIS_ __out float* pRatio) PURE;









    STDMETHOD(SetSourceSampleRate) (THIS_ UINT32 NewSourceSampleRate) PURE;
};








#undef INTERFACE
#define INTERFACE IXAudio2SubmixVoice
DECLARE_INTERFACE_(IXAudio2SubmixVoice, IXAudio2Voice)
{

    Declare_IXAudio2Voice_Methods();


};








#undef INTERFACE
#define INTERFACE IXAudio2MasteringVoice
DECLARE_INTERFACE_(IXAudio2MasteringVoice, IXAudio2Voice)
{

    Declare_IXAudio2Voice_Methods();


};














#undef INTERFACE
#define INTERFACE IXAudio2EngineCallback
DECLARE_INTERFACE(IXAudio2EngineCallback)
{

    STDMETHOD_(void, OnProcessingPassStart) (THIS) PURE;


    STDMETHOD_(void, OnProcessingPassEnd) (THIS) PURE;



    STDMETHOD_(void, OnCriticalError) (THIS_ HRESULT Error) PURE;
};













#undef INTERFACE
#define INTERFACE IXAudio2VoiceCallback
DECLARE_INTERFACE(IXAudio2VoiceCallback)
{

    STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) PURE;


    STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) PURE;



    STDMETHOD_(void, OnStreamEnd) (THIS) PURE;


    STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) PURE;



    STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) PURE;


    STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) PURE;






    STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) PURE;
};








#ifndef __cplusplus


#define IXAudio2_QueryInterface(This,riid,ppvInterface) ((This)->lpVtbl->QueryInterface(This,riid,ppvInterface))
#define IXAudio2_AddRef(This) ((This)->lpVtbl->AddRef(This))
#define IXAudio2_Release(This) ((This)->lpVtbl->Release(This))
#define IXAudio2_GetDeviceCount(This,puCount) ((This)->lpVtbl->GetDeviceCount(This,puCount))
#define IXAudio2_GetDeviceDetails(This,Index,pDeviceDetails) ((This)->lpVtbl->GetDeviceDetails(This,Index,pDeviceDetails))
#define IXAudio2_Initialize(This,Flags,XAudio2Processor) ((This)->lpVtbl->Initialize(This,Flags,XAudio2Processor))
#define IXAudio2_CreateSourceVoice(This,ppSourceVoice,pSourceFormat,Flags,MaxFrequencyRatio,pCallback,pSendList,pEffectChain) ((This)->lpVtbl->CreateSourceVoice(This,ppSourceVoice,pSourceFormat,Flags,MaxFrequencyRatio,pCallback,pSendList,pEffectChain))
#define IXAudio2_CreateSubmixVoice(This,ppSubmixVoice,InputChannels,InputSampleRate,Flags,ProcessingStage,pSendList,pEffectChain) ((This)->lpVtbl->CreateSubmixVoice(This,ppSubmixVoice,InputChannels,InputSampleRate,Flags,ProcessingStage,pSendList,pEffectChain))
#define IXAudio2_CreateMasteringVoice(This,ppMasteringVoice,InputChannels,InputSampleRate,Flags,DeviceIndex,pEffectChain) ((This)->lpVtbl->CreateMasteringVoice(This,ppMasteringVoice,InputChannels,InputSampleRate,Flags,DeviceIndex,pEffectChain))
#define IXAudio2_StartEngine(This) ((This)->lpVtbl->StartEngine(This))
#define IXAudio2_StopEngine(This) ((This)->lpVtbl->StopEngine(This))
#define IXAudio2_CommitChanges(This,OperationSet) ((This)->lpVtbl->CommitChanges(This,OperationSet))
#define IXAudio2_GetPerformanceData(This,pPerfData) ((This)->lpVtbl->GetPerformanceData(This,pPerfData))
#define IXAudio2_SetDebugConfiguration(This,pDebugConfiguration,pReserved) ((This)->lpVtbl->SetDebugConfiguration(This,pDebugConfiguration,pReserved))


#define IXAudio2Voice_GetVoiceDetails(This,pVoiceDetails) ((This)->lpVtbl->GetVoiceDetails(This,pVoiceDetails))
#define IXAudio2Voice_SetOutputVoices(This,pSendList) ((This)->lpVtbl->SetOutputVoices(This,pSendList))
#define IXAudio2Voice_SetEffectChain(This,pEffectChain) ((This)->lpVtbl->SetEffectChain(This,pEffectChain))
#define IXAudio2Voice_EnableEffect(This,EffectIndex,OperationSet) ((This)->lpVtbl->EnableEffect(This,EffectIndex,OperationSet))
#define IXAudio2Voice_DisableEffect(This,EffectIndex,OperationSet) ((This)->lpVtbl->DisableEffect(This,EffectIndex,OperationSet))
#define IXAudio2Voice_GetEffectState(This,EffectIndex,pEnabled) ((This)->lpVtbl->GetEffectState(This,EffectIndex,pEnabled))
#define IXAudio2Voice_SetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize, OperationSet) ((This)->lpVtbl->SetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize,OperationSet))
#define IXAudio2Voice_GetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize) ((This)->lpVtbl->GetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize))
#define IXAudio2Voice_SetFilterParameters(This,pParameters,OperationSet) ((This)->lpVtbl->SetFilterParameters(This,pParameters,OperationSet))
#define IXAudio2Voice_GetFilterParameters(This,pParameters) ((This)->lpVtbl->GetFilterParameters(This,pParameters))
#define IXAudio2Voice_SetOutputFilterParameters(This,pDestinationVoice,pParameters,OperationSet) ((This)->lpVtbl->SetOutputFilterParameters(This,pDestinationVoice,pParameters,OperationSet))
#define IXAudio2Voice_GetOutputFilterParameters(This,pDestinationVoice,pParameters) ((This)->lpVtbl->GetOutputFilterParameters(This,pDestinationVoice,pParameters))
#define IXAudio2Voice_SetVolume(This,Volume,OperationSet) ((This)->lpVtbl->SetVolume(This,Volume,OperationSet))
#define IXAudio2Voice_GetVolume(This,pVolume) ((This)->lpVtbl->GetVolume(This,pVolume))
#define IXAudio2Voice_SetChannelVolumes(This,Channels,pVolumes,OperationSet) ((This)->lpVtbl->SetChannelVolumes(This,Channels,pVolumes,OperationSet))
#define IXAudio2Voice_GetChannelVolumes(This,Channels,pVolumes) ((This)->lpVtbl->GetChannelVolumes(This,Channels,pVolumes))
#define IXAudio2Voice_SetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix,OperationSet) ((This)->lpVtbl->SetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix,OperationSet))
#define IXAudio2Voice_GetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix) ((This)->lpVtbl->GetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix))
#define IXAudio2Voice_DestroyVoice(This) ((This)->lpVtbl->DestroyVoice(This))


#define IXAudio2SourceVoice_GetVoiceDetails IXAudio2Voice_GetVoiceDetails
#define IXAudio2SourceVoice_SetOutputVoices IXAudio2Voice_SetOutputVoices
#define IXAudio2SourceVoice_SetEffectChain IXAudio2Voice_SetEffectChain
#define IXAudio2SourceVoice_EnableEffect IXAudio2Voice_EnableEffect
#define IXAudio2SourceVoice_DisableEffect IXAudio2Voice_DisableEffect
#define IXAudio2SourceVoice_GetEffectState IXAudio2Voice_GetEffectState
#define IXAudio2SourceVoice_SetEffectParameters IXAudio2Voice_SetEffectParameters
#define IXAudio2SourceVoice_GetEffectParameters IXAudio2Voice_GetEffectParameters
#define IXAudio2SourceVoice_SetFilterParameters IXAudio2Voice_SetFilterParameters
#define IXAudio2SourceVoice_GetFilterParameters IXAudio2Voice_GetFilterParameters
#define IXAudio2SourceVoice_SetOutputFilterParameters IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2SourceVoice_GetOutputFilterParameters IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2SourceVoice_SetVolume IXAudio2Voice_SetVolume
#define IXAudio2SourceVoice_GetVolume IXAudio2Voice_GetVolume
#define IXAudio2SourceVoice_SetChannelVolumes IXAudio2Voice_SetChannelVolumes
#define IXAudio2SourceVoice_GetChannelVolumes IXAudio2Voice_GetChannelVolumes
#define IXAudio2SourceVoice_SetOutputMatrix IXAudio2Voice_SetOutputMatrix
#define IXAudio2SourceVoice_GetOutputMatrix IXAudio2Voice_GetOutputMatrix
#define IXAudio2SourceVoice_DestroyVoice IXAudio2Voice_DestroyVoice
#define IXAudio2SourceVoice_Start(This,Flags,OperationSet) ((This)->lpVtbl->Start(This,Flags,OperationSet))
#define IXAudio2SourceVoice_Stop(This,Flags,OperationSet) ((This)->lpVtbl->Stop(This,Flags,OperationSet))
#define IXAudio2SourceVoice_SubmitSourceBuffer(This,pBuffer,pBufferWMA) ((This)->lpVtbl->SubmitSourceBuffer(This,pBuffer,pBufferWMA))
#define IXAudio2SourceVoice_FlushSourceBuffers(This) ((This)->lpVtbl->FlushSourceBuffers(This))
#define IXAudio2SourceVoice_Discontinuity(This) ((This)->lpVtbl->Discontinuity(This))
#define IXAudio2SourceVoice_ExitLoop(This,OperationSet) ((This)->lpVtbl->ExitLoop(This,OperationSet))
#define IXAudio2SourceVoice_GetState(This,pVoiceState) ((This)->lpVtbl->GetState(This,pVoiceState))
#define IXAudio2SourceVoice_SetFrequencyRatio(This,Ratio,OperationSet) ((This)->lpVtbl->SetFrequencyRatio(This,Ratio,OperationSet))
#define IXAudio2SourceVoice_GetFrequencyRatio(This,pRatio) ((This)->lpVtbl->GetFrequencyRatio(This,pRatio))
#define IXAudio2SourceVoice_SetSourceSampleRate(This,NewSourceSampleRate) ((This)->lpVtbl->SetSourceSampleRate(This,NewSourceSampleRate))


#define IXAudio2SubmixVoice_GetVoiceDetails IXAudio2Voice_GetVoiceDetails
#define IXAudio2SubmixVoice_SetOutputVoices IXAudio2Voice_SetOutputVoices
#define IXAudio2SubmixVoice_SetEffectChain IXAudio2Voice_SetEffectChain
#define IXAudio2SubmixVoice_EnableEffect IXAudio2Voice_EnableEffect
#define IXAudio2SubmixVoice_DisableEffect IXAudio2Voice_DisableEffect
#define IXAudio2SubmixVoice_GetEffectState IXAudio2Voice_GetEffectState
#define IXAudio2SubmixVoice_SetEffectParameters IXAudio2Voice_SetEffectParameters
#define IXAudio2SubmixVoice_GetEffectParameters IXAudio2Voice_GetEffectParameters
#define IXAudio2SubmixVoice_SetFilterParameters IXAudio2Voice_SetFilterParameters
#define IXAudio2SubmixVoice_GetFilterParameters IXAudio2Voice_GetFilterParameters
#define IXAudio2SubmixVoice_SetOutputFilterParameters IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2SubmixVoice_GetOutputFilterParameters IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2SubmixVoice_SetVolume IXAudio2Voice_SetVolume
#define IXAudio2SubmixVoice_GetVolume IXAudio2Voice_GetVolume
#define IXAudio2SubmixVoice_SetChannelVolumes IXAudio2Voice_SetChannelVolumes
#define IXAudio2SubmixVoice_GetChannelVolumes IXAudio2Voice_GetChannelVolumes
#define IXAudio2SubmixVoice_SetOutputMatrix IXAudio2Voice_SetOutputMatrix
#define IXAudio2SubmixVoice_GetOutputMatrix IXAudio2Voice_GetOutputMatrix
#define IXAudio2SubmixVoice_DestroyVoice IXAudio2Voice_DestroyVoice


#define IXAudio2MasteringVoice_GetVoiceDetails IXAudio2Voice_GetVoiceDetails
#define IXAudio2MasteringVoice_SetOutputVoices IXAudio2Voice_SetOutputVoices
#define IXAudio2MasteringVoice_SetEffectChain IXAudio2Voice_SetEffectChain
#define IXAudio2MasteringVoice_EnableEffect IXAudio2Voice_EnableEffect
#define IXAudio2MasteringVoice_DisableEffect IXAudio2Voice_DisableEffect
#define IXAudio2MasteringVoice_GetEffectState IXAudio2Voice_GetEffectState
#define IXAudio2MasteringVoice_SetEffectParameters IXAudio2Voice_SetEffectParameters
#define IXAudio2MasteringVoice_GetEffectParameters IXAudio2Voice_GetEffectParameters
#define IXAudio2MasteringVoice_SetFilterParameters IXAudio2Voice_SetFilterParameters
#define IXAudio2MasteringVoice_GetFilterParameters IXAudio2Voice_GetFilterParameters
#define IXAudio2MasteringVoice_SetOutputFilterParameters IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2MasteringVoice_GetOutputFilterParameters IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2MasteringVoice_SetVolume IXAudio2Voice_SetVolume
#define IXAudio2MasteringVoice_GetVolume IXAudio2Voice_GetVolume
#define IXAudio2MasteringVoice_SetChannelVolumes IXAudio2Voice_SetChannelVolumes
#define IXAudio2MasteringVoice_GetChannelVolumes IXAudio2Voice_GetChannelVolumes
#define IXAudio2MasteringVoice_SetOutputMatrix IXAudio2Voice_SetOutputMatrix
#define IXAudio2MasteringVoice_GetOutputMatrix IXAudio2Voice_GetOutputMatrix
#define IXAudio2MasteringVoice_DestroyVoice IXAudio2Voice_DestroyVoice

#endif











#ifdef XAUDIO2_HELPER_FUNCTIONS

#define _USE_MATH_DEFINES
#include <math.h>


__inline float XAudio2DecibelsToAmplitudeRatio(float Decibels)
{
    return powf(10.0f, Decibels / 20.0f);
}


__inline float XAudio2AmplitudeRatioToDecibels(float Volume)
{
    if (Volume == 0)
    {
        return -3.402823466e+38f;
    }
    return 20.0f * log10f(Volume);
}


__inline float XAudio2SemitonesToFrequencyRatio(float Semitones)
{


    return powf(2.0f, Semitones / 12.0f);
}


__inline float XAudio2FrequencyRatioToSemitones(float FrequencyRatio)
{


    return 39.86313713864835f * log10f(FrequencyRatio);
}





__inline float XAudio2CutoffFrequencyToRadians(float CutoffFrequency, UINT32 SampleRate)
{
    if ((UINT32)(CutoffFrequency * 6.0f) >= SampleRate)
    {
        return XAUDIO2_MAX_FILTER_FREQUENCY;
    }
    return 2.0f * sinf((float)M_PI * CutoffFrequency / SampleRate);
}


__inline float XAudio2RadiansToCutoffFrequency(float Radians, float SampleRate)
{
    return SampleRate * asinf(Radians / 2.0f) / (float)M_PI;
}
#endif

















#ifdef _XBOX

STDAPI XAudio2Create(__deref_out IXAudio2** ppXAudio2, UINT32 Flags X2DEFAULT(0),
                     XAUDIO2_PROCESSOR XAudio2Processor X2DEFAULT(XAUDIO2_DEFAULT_PROCESSOR));

#else

__inline HRESULT XAudio2Create(__deref_out IXAudio2** ppXAudio2, UINT32 Flags X2DEFAULT(0),
                               XAUDIO2_PROCESSOR XAudio2Processor X2DEFAULT(XAUDIO2_DEFAULT_PROCESSOR))
{

    IXAudio2* pXAudio2;

    #ifdef __cplusplus

        HRESULT hr = CoCreateInstance((Flags & XAUDIO2_DEBUG_ENGINE) ? __uuidof(XAudio2_Debug) : __uuidof(XAudio2),
                                      NULL, CLSCTX_INPROC_SERVER, __uuidof(IXAudio2), (void**)&pXAudio2);
        if (SUCCEEDED(hr))
        {
            hr = pXAudio2->Initialize(Flags, XAudio2Processor);

            if (SUCCEEDED(hr))
            {
                *ppXAudio2 = pXAudio2;
            }
            else
            {
                pXAudio2->Release();
            }
        }

    #else

        HRESULT hr = CoCreateInstance((Flags & XAUDIO2_DEBUG_ENGINE) ? &CLSID_XAudio2_Debug : &CLSID_XAudio2,
                                      NULL, CLSCTX_INPROC_SERVER, &IID_IXAudio2, (void**)&pXAudio2);
        if (SUCCEEDED(hr))
        {
            hr = pXAudio2->lpVtbl->Initialize(pXAudio2, Flags, XAudio2Processor);

            if (SUCCEEDED(hr))
            {
                *ppXAudio2 = pXAudio2;
            }
            else
            {
                pXAudio2->lpVtbl->Release(pXAudio2);
            }
        }

    #endif

    return hr;
}

#endif



#pragma pack(pop)

#endif
#endif
