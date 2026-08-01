








#ifndef __XAUDIO2FX_INCLUDED__
#define __XAUDIO2FX_INCLUDED__








#include "comdecl.h"












































DEFINE_CLSID(AudioVolumeMeter, cac1105f, 619b, 4d04, 83, 1a, 44, e1, cb, f1, 2d, 57);
DEFINE_CLSID(AudioVolumeMeter_Debug, 2d9a0f9c, e67b, 4b24, ab, 44, 92, b3, e7, 70, c0, 20);
DEFINE_CLSID(AudioReverb, 6a93130e, 1d53, 41d1, a9, cf, e7, 58, 80, 0b, b1, 79);
DEFINE_CLSID(AudioReverb_Debug, c4f82dd4, cb4e, 4ce1, 8b, db, ee, 32, d4, 19, 82, 69);


#ifndef GUID_DEFS_ONLY

#ifdef _XBOX
    #include <xobjbase.h>
#else
    #include <objbase.h>
#endif
#include <math.h>



#pragma pack(push, 1)














#ifdef __cplusplus
    #define DEFAULT(x) =x
#else
    #define DEFAULT(x)
#endif

#define XAUDIO2FX_DEBUG 1

#ifdef _XBOX

    STDAPI CreateAudioVolumeMeter(__deref_out IUnknown** ppApo);
    STDAPI CreateAudioReverb(__deref_out IUnknown** ppApo);

    __inline HRESULT XAudio2CreateVolumeMeter(__deref_out IUnknown** ppApo, UINT32  DEFAULT(0))
    {
        return CreateAudioVolumeMeter(ppApo);
    }

    __inline HRESULT XAudio2CreateReverb(__deref_out IUnknown** ppApo, UINT32  DEFAULT(0))
    {
        return CreateAudioReverb(ppApo);
    }

#else

    __inline HRESULT XAudio2CreateVolumeMeter(__deref_out IUnknown** ppApo, UINT32 Flags DEFAULT(0))
    {
        #ifdef __cplusplus
            return CoCreateInstance((Flags & XAUDIO2FX_DEBUG) ? __uuidof(AudioVolumeMeter_Debug)
                                                              : __uuidof(AudioVolumeMeter),
                                    NULL, CLSCTX_INPROC_SERVER, __uuidof(IUnknown), (void**)ppApo);
        #else
            return CoCreateInstance((Flags & XAUDIO2FX_DEBUG) ? &CLSID_AudioVolumeMeter_Debug
                                                              : &CLSID_AudioVolumeMeter,
                                    NULL, CLSCTX_INPROC_SERVER, &IID_IUnknown, (void**)ppApo);
        #endif
    }

    __inline HRESULT XAudio2CreateReverb(__deref_out IUnknown** ppApo, UINT32 Flags DEFAULT(0))
    {
        #ifdef __cplusplus
            return CoCreateInstance((Flags & XAUDIO2FX_DEBUG) ? __uuidof(AudioReverb_Debug)
                                                              : __uuidof(AudioReverb),
                                    NULL, CLSCTX_INPROC_SERVER, __uuidof(IUnknown), (void**)ppApo);
        #else
            return CoCreateInstance((Flags & XAUDIO2FX_DEBUG) ? &CLSID_AudioReverb_Debug
                                                              : &CLSID_AudioReverb,
                                    NULL, CLSCTX_INPROC_SERVER, &IID_IUnknown, (void**)ppApo);
        #endif
    }

#endif














typedef struct XAUDIO2FX_VOLUMEMETER_LEVELS
{
    float* pPeakLevels;


    float* pRMSLevels;


    UINT32 ChannelCount;
} XAUDIO2FX_VOLUMEMETER_LEVELS;





















#define XAUDIO2FX_REVERB_MIN_FRAMERATE 20000
#define XAUDIO2FX_REVERB_MAX_FRAMERATE 48000



typedef struct XAUDIO2FX_REVERB_PARAMETERS
{

    float WetDryMix;


    UINT32 ReflectionsDelay;
    BYTE ReverbDelay;
    BYTE RearDelay;


    BYTE PositionLeft;
    BYTE PositionRight;
    BYTE PositionMatrixLeft;
    BYTE PositionMatrixRight;
    BYTE EarlyDiffusion;
    BYTE LateDiffusion;
    BYTE LowEQGain;
    BYTE LowEQCutoff;
    BYTE HighEQGain;
    BYTE HighEQCutoff;


    float RoomFilterFreq;
    float RoomFilterMain;
    float RoomFilterHF;
    float ReflectionsGain;
    float ReverbGain;
    float DecayTime;
    float Density;
    float RoomSize;
} XAUDIO2FX_REVERB_PARAMETERS;



#define XAUDIO2FX_REVERB_MIN_WET_DRY_MIX            0.0f
#define XAUDIO2FX_REVERB_MIN_REFLECTIONS_DELAY      0
#define XAUDIO2FX_REVERB_MIN_REVERB_DELAY           0
#define XAUDIO2FX_REVERB_MIN_REAR_DELAY             0
#define XAUDIO2FX_REVERB_MIN_POSITION               0
#define XAUDIO2FX_REVERB_MIN_DIFFUSION              0
#define XAUDIO2FX_REVERB_MIN_LOW_EQ_GAIN            0
#define XAUDIO2FX_REVERB_MIN_LOW_EQ_CUTOFF          0
#define XAUDIO2FX_REVERB_MIN_HIGH_EQ_GAIN           0
#define XAUDIO2FX_REVERB_MIN_HIGH_EQ_CUTOFF         0
#define XAUDIO2FX_REVERB_MIN_ROOM_FILTER_FREQ       20.0f
#define XAUDIO2FX_REVERB_MIN_ROOM_FILTER_MAIN       -100.0f
#define XAUDIO2FX_REVERB_MIN_ROOM_FILTER_HF         -100.0f
#define XAUDIO2FX_REVERB_MIN_REFLECTIONS_GAIN       -100.0f
#define XAUDIO2FX_REVERB_MIN_REVERB_GAIN            -100.0f
#define XAUDIO2FX_REVERB_MIN_DECAY_TIME             0.1f
#define XAUDIO2FX_REVERB_MIN_DENSITY                0.0f
#define XAUDIO2FX_REVERB_MIN_ROOM_SIZE              0.0f

#define XAUDIO2FX_REVERB_MAX_WET_DRY_MIX            100.0f
#define XAUDIO2FX_REVERB_MAX_REFLECTIONS_DELAY      300
#define XAUDIO2FX_REVERB_MAX_REVERB_DELAY           85
#define XAUDIO2FX_REVERB_MAX_REAR_DELAY             5
#define XAUDIO2FX_REVERB_MAX_POSITION               30
#define XAUDIO2FX_REVERB_MAX_DIFFUSION              15
#define XAUDIO2FX_REVERB_MAX_LOW_EQ_GAIN            12
#define XAUDIO2FX_REVERB_MAX_LOW_EQ_CUTOFF          9
#define XAUDIO2FX_REVERB_MAX_HIGH_EQ_GAIN           8
#define XAUDIO2FX_REVERB_MAX_HIGH_EQ_CUTOFF         14
#define XAUDIO2FX_REVERB_MAX_ROOM_FILTER_FREQ       20000.0f
#define XAUDIO2FX_REVERB_MAX_ROOM_FILTER_MAIN       0.0f
#define XAUDIO2FX_REVERB_MAX_ROOM_FILTER_HF         0.0f
#define XAUDIO2FX_REVERB_MAX_REFLECTIONS_GAIN       20.0f
#define XAUDIO2FX_REVERB_MAX_REVERB_GAIN            20.0f
#define XAUDIO2FX_REVERB_MAX_DENSITY                100.0f
#define XAUDIO2FX_REVERB_MAX_ROOM_SIZE              100.0f

#define XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX        100.0f
#define XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY  5
#define XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY       5
#define XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY         5
#define XAUDIO2FX_REVERB_DEFAULT_POSITION           6
#define XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX    27
#define XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION    8
#define XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION     8
#define XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN        8
#define XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF      4
#define XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN       8
#define XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF     4
#define XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ   5000.0f
#define XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN   0.0f
#define XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF     0.0f
#define XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN   0.0f
#define XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN        0.0f
#define XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME         1.0f
#define XAUDIO2FX_REVERB_DEFAULT_DENSITY            100.0f
#define XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE          100.0f




typedef struct XAUDIO2FX_REVERB_I3DL2_PARAMETERS
{

    float WetDryMix;


    INT32 Room;
    INT32 RoomHF;
    float RoomRolloffFactor;
    float DecayTime;
    float DecayHFRatio;
    INT32 Reflections;
    float ReflectionsDelay;
    INT32 Reverb;
    float ReverbDelay;
    float Diffusion;
    float Density;
    float HFReference;
} XAUDIO2FX_REVERB_I3DL2_PARAMETERS;




__inline void ReverbConvertI3DL2ToNative
(
    __in const XAUDIO2FX_REVERB_I3DL2_PARAMETERS* pI3DL2,
    __out XAUDIO2FX_REVERB_PARAMETERS* pNative
)
{
    float reflectionsDelay;
    float reverbDelay;




    pNative->RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
    pNative->PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
    pNative->PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
    pNative->PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
    pNative->PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
    pNative->RoomSize = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE;
    pNative->LowEQCutoff = 4;
    pNative->HighEQCutoff = 6;


    pNative->RoomFilterMain = (float)pI3DL2->Room / 100.0f;
    pNative->RoomFilterHF = (float)pI3DL2->RoomHF / 100.0f;

    if (pI3DL2->DecayHFRatio >= 1.0f)
    {
        INT32 index = (INT32)(-4.0 * log10(pI3DL2->DecayHFRatio));
        if (index < -8) index = -8;
        pNative->LowEQGain = (BYTE)((index < 0) ? index + 8 : 8);
        pNative->HighEQGain = 8;
        pNative->DecayTime = pI3DL2->DecayTime * pI3DL2->DecayHFRatio;
    }
    else
    {
        INT32 index = (INT32)(4.0 * log10(pI3DL2->DecayHFRatio));
        if (index < -8) index = -8;
        pNative->LowEQGain = 8;
        pNative->HighEQGain = (BYTE)((index < 0) ? index + 8 : 8);
        pNative->DecayTime = pI3DL2->DecayTime;
    }

    reflectionsDelay = pI3DL2->ReflectionsDelay * 1000.0f;
    if (reflectionsDelay >= XAUDIO2FX_REVERB_MAX_REFLECTIONS_DELAY)
    {
        reflectionsDelay = (float)(XAUDIO2FX_REVERB_MAX_REFLECTIONS_DELAY - 1);
    }
    else if (reflectionsDelay <= 1)
    {
        reflectionsDelay = 1;
    }
    pNative->ReflectionsDelay = (UINT32)reflectionsDelay;

    reverbDelay = pI3DL2->ReverbDelay * 1000.0f;
    if (reverbDelay >= XAUDIO2FX_REVERB_MAX_REVERB_DELAY)
    {
        reverbDelay = (float)(XAUDIO2FX_REVERB_MAX_REVERB_DELAY - 1);
    }
    pNative->ReverbDelay = (BYTE)reverbDelay;

    pNative->ReflectionsGain = pI3DL2->Reflections / 100.0f;
    pNative->ReverbGain = pI3DL2->Reverb / 100.0f;
    pNative->EarlyDiffusion = (BYTE)(15.0f * pI3DL2->Diffusion / 100.0f);
    pNative->LateDiffusion = pNative->EarlyDiffusion;
    pNative->Density = pI3DL2->Density;
    pNative->RoomFilterFreq = pI3DL2->HFReference;

    pNative->WetDryMix = pI3DL2->WetDryMix;
}








#define XAUDIO2FX_I3DL2_PRESET_DEFAULT         {100,-10000,    0,0.0f, 1.00f,0.50f,-10000,0.020f,-10000,0.040f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_GENERIC         {100, -1000, -100,0.0f, 1.49f,0.83f, -2602,0.007f,   200,0.011f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_PADDEDCELL      {100, -1000,-6000,0.0f, 0.17f,0.10f, -1204,0.001f,   207,0.002f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_ROOM            {100, -1000, -454,0.0f, 0.40f,0.83f, -1646,0.002f,    53,0.003f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_BATHROOM        {100, -1000,-1200,0.0f, 1.49f,0.54f,  -370,0.007f,  1030,0.011f,100.0f, 60.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_LIVINGROOM      {100, -1000,-6000,0.0f, 0.50f,0.10f, -1376,0.003f, -1104,0.004f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_STONEROOM       {100, -1000, -300,0.0f, 2.31f,0.64f,  -711,0.012f,    83,0.017f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_AUDITORIUM      {100, -1000, -476,0.0f, 4.32f,0.59f,  -789,0.020f,  -289,0.030f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_CONCERTHALL     {100, -1000, -500,0.0f, 3.92f,0.70f, -1230,0.020f,    -2,0.029f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_CAVE            {100, -1000,    0,0.0f, 2.91f,1.30f,  -602,0.015f,  -302,0.022f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_ARENA           {100, -1000, -698,0.0f, 7.24f,0.33f, -1166,0.020f,    16,0.030f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_HANGAR          {100, -1000,-1000,0.0f,10.05f,0.23f,  -602,0.020f,   198,0.030f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY {100, -1000,-4000,0.0f, 0.30f,0.10f, -1831,0.002f, -1630,0.030f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_HALLWAY         {100, -1000, -300,0.0f, 1.49f,0.59f, -1219,0.007f,   441,0.011f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR   {100, -1000, -237,0.0f, 2.70f,0.79f, -1214,0.013f,   395,0.020f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_ALLEY           {100, -1000, -270,0.0f, 1.49f,0.86f, -1204,0.007f,    -4,0.011f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_FOREST          {100, -1000,-3300,0.0f, 1.49f,0.54f, -2560,0.162f,  -613,0.088f, 79.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_CITY            {100, -1000, -800,0.0f, 1.49f,0.67f, -2273,0.007f, -2217,0.011f, 50.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_MOUNTAINS       {100, -1000,-2500,0.0f, 1.49f,0.21f, -2780,0.300f, -2014,0.100f, 27.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_QUARRY          {100, -1000,-1000,0.0f, 1.49f,0.83f,-10000,0.061f,   500,0.025f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_PLAIN           {100, -1000,-2000,0.0f, 1.49f,0.50f, -2466,0.179f, -2514,0.100f, 21.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_PARKINGLOT      {100, -1000,    0,0.0f, 1.65f,1.50f, -1363,0.008f, -1153,0.012f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_SEWERPIPE       {100, -1000,-1000,0.0f, 2.81f,0.14f,   429,0.014f,   648,0.021f, 80.0f, 60.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_UNDERWATER      {100, -1000,-4000,0.0f, 1.49f,0.10f,  -449,0.007f,  1700,0.011f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_SMALLROOM       {100, -1000, -600,0.0f, 1.10f,0.83f,  -400,0.005f,   500,0.010f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM      {100, -1000, -600,0.0f, 1.30f,0.83f, -1000,0.010f,  -200,0.020f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_LARGEROOM       {100, -1000, -600,0.0f, 1.50f,0.83f, -1600,0.020f, -1000,0.040f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL      {100, -1000, -600,0.0f, 1.80f,0.70f, -1300,0.015f,  -800,0.030f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_LARGEHALL       {100, -1000, -600,0.0f, 1.80f,0.70f, -2000,0.030f, -1400,0.060f,100.0f,100.0f,5000.0f}
#define XAUDIO2FX_I3DL2_PRESET_PLATE           {100, -1000, -200,0.0f, 1.30f,0.90f,     0,0.002f,     0,0.010f,100.0f, 75.0f,5000.0f}



#pragma pack(pop)

#endif
#endif
