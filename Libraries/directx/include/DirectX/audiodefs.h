



















#ifndef __AUDIODEFS_INCLUDED__
#define __AUDIODEFS_INCLUDED__

#include <windef.h>

#pragma pack(push, 1)










#ifndef _WAVEFORMATEX_

    #define _WAVEFORMATEX_
    typedef struct tWAVEFORMATEX
    {
        WORD wFormatTag;
        WORD nChannels;
        DWORD nSamplesPerSec;
        DWORD nAvgBytesPerSec;
        WORD nBlockAlign;
        WORD wBitsPerSample;
        WORD cbSize;
    } WAVEFORMATEX;

#endif




typedef WAVEFORMATEX *PWAVEFORMATEX, *NPWAVEFORMATEX, *LPWAVEFORMATEX;
typedef const WAVEFORMATEX *PCWAVEFORMATEX, *LPCWAVEFORMATEX;













#ifndef _WAVEFORMATEXTENSIBLE_

    #define _WAVEFORMATEXTENSIBLE_
    typedef struct
    {
        WAVEFORMATEX Format;
        union
        {
            WORD wValidBitsPerSample;
            WORD wSamplesPerBlock;

            WORD wReserved;
        } Samples;
        DWORD dwChannelMask;
        GUID SubFormat;
    } WAVEFORMATEXTENSIBLE;

#endif

typedef WAVEFORMATEXTENSIBLE *PWAVEFORMATEXTENSIBLE, *LPWAVEFORMATEXTENSIBLE;
typedef const WAVEFORMATEXTENSIBLE *PCWAVEFORMATEXTENSIBLE, *LPCWAVEFORMATEXTENSIBLE;









#ifndef WAVE_FORMAT_PCM





    typedef struct waveformat_tag
    {
        WORD wFormatTag;
        WORD nChannels;
        DWORD nSamplesPerSec;
        DWORD nAvgBytesPerSec;
        WORD nBlockAlign;
    } WAVEFORMAT, *PWAVEFORMAT, NEAR *NPWAVEFORMAT, FAR *LPWAVEFORMAT;


    typedef struct pcmwaveformat_tag
    {
        WAVEFORMAT wf;
        WORD wBitsPerSample;
    } PCMWAVEFORMAT, *PPCMWAVEFORMAT, NEAR *NPPCMWAVEFORMAT, FAR *LPPCMWAVEFORMAT;

    #define WAVE_FORMAT_PCM 0x0001

#endif

#ifndef WAVE_FORMAT_ADPCM



    typedef struct adpcmcoef_tag
    {
        short iCoef1;
        short iCoef2;
    } ADPCMCOEFSET;

    #pragma warning(push)
    #pragma warning(disable:4200)

    typedef struct adpcmwaveformat_tag {
        WAVEFORMATEX wfx;
        WORD wSamplesPerBlock;
        WORD wNumCoef;
        ADPCMCOEFSET aCoef[];
    } ADPCMWAVEFORMAT;

    #pragma warning(pop)

    #define WAVE_FORMAT_ADPCM 0x0002

#endif



#ifndef WAVE_FORMAT_UNKNOWN
    #define WAVE_FORMAT_UNKNOWN         0x0000
#endif

#ifndef WAVE_FORMAT_IEEE_FLOAT
    #define WAVE_FORMAT_IEEE_FLOAT      0x0003
#endif

#ifndef WAVE_FORMAT_MPEGLAYER3
    #define WAVE_FORMAT_MPEGLAYER3      0x0055
#endif

#ifndef WAVE_FORMAT_DOLBY_AC3_SPDIF
    #define WAVE_FORMAT_DOLBY_AC3_SPDIF 0x0092
#endif

#ifndef WAVE_FORMAT_WMAUDIO2
    #define WAVE_FORMAT_WMAUDIO2        0x0161
#endif

#ifndef WAVE_FORMAT_WMAUDIO3
    #define WAVE_FORMAT_WMAUDIO3        0x0162
#endif

#ifndef WAVE_FORMAT_WMASPDIF
    #define WAVE_FORMAT_WMASPDIF        0x0164
#endif

#ifndef WAVE_FORMAT_EXTENSIBLE
    #define WAVE_FORMAT_EXTENSIBLE      0xFFFE
#endif











#ifdef __cplusplus

    #ifndef KSDATAFORMAT_SUBTYPE_PCM
        struct __declspec(uuid("00000001-0000-0010-8000-00aa00389b71")) KSDATAFORMAT_SUBTYPE_PCM_STRUCT;
        #define KSDATAFORMAT_SUBTYPE_PCM __uuidof(KSDATAFORMAT_SUBTYPE_PCM_STRUCT)
    #endif

    #ifndef KSDATAFORMAT_SUBTYPE_ADPCM
        struct __declspec(uuid("00000002-0000-0010-8000-00aa00389b71")) KSDATAFORMAT_SUBTYPE_ADPCM_STRUCT;
        #define KSDATAFORMAT_SUBTYPE_ADPCM __uuidof(KSDATAFORMAT_SUBTYPE_ADPCM_STRUCT)
    #endif

    #ifndef KSDATAFORMAT_SUBTYPE_IEEE_FLOAT
        struct __declspec(uuid("00000003-0000-0010-8000-00aa00389b71")) KSDATAFORMAT_SUBTYPE_IEEE_FLOAT_STRUCT;
        #define KSDATAFORMAT_SUBTYPE_IEEE_FLOAT __uuidof(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT_STRUCT)
    #endif

#endif








#ifndef SPEAKER_FRONT_LEFT
    #define SPEAKER_FRONT_LEFT            0x00000001
    #define SPEAKER_FRONT_RIGHT           0x00000002
    #define SPEAKER_FRONT_CENTER          0x00000004
    #define SPEAKER_LOW_FREQUENCY         0x00000008
    #define SPEAKER_BACK_LEFT             0x00000010
    #define SPEAKER_BACK_RIGHT            0x00000020
    #define SPEAKER_FRONT_LEFT_OF_CENTER  0x00000040
    #define SPEAKER_FRONT_RIGHT_OF_CENTER 0x00000080
    #define SPEAKER_BACK_CENTER           0x00000100
    #define SPEAKER_SIDE_LEFT             0x00000200
    #define SPEAKER_SIDE_RIGHT            0x00000400
    #define SPEAKER_TOP_CENTER            0x00000800
    #define SPEAKER_TOP_FRONT_LEFT        0x00001000
    #define SPEAKER_TOP_FRONT_CENTER      0x00002000
    #define SPEAKER_TOP_FRONT_RIGHT       0x00004000
    #define SPEAKER_TOP_BACK_LEFT         0x00008000
    #define SPEAKER_TOP_BACK_CENTER       0x00010000
    #define SPEAKER_TOP_BACK_RIGHT        0x00020000
    #define SPEAKER_RESERVED              0x7FFC0000
    #define SPEAKER_ALL                   0x80000000
    #define _SPEAKER_POSITIONS_
#endif

#ifndef SPEAKER_STEREO
    #define SPEAKER_MONO             (SPEAKER_FRONT_CENTER)
    #define SPEAKER_STEREO           (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT)
    #define SPEAKER_2POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_LOW_FREQUENCY)
    #define SPEAKER_SURROUND         (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_CENTER)
    #define SPEAKER_QUAD             (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
    #define SPEAKER_4POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
    #define SPEAKER_5POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
    #define SPEAKER_7POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_FRONT_LEFT_OF_CENTER | SPEAKER_FRONT_RIGHT_OF_CENTER)
    #define SPEAKER_5POINT1_SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT)
    #define SPEAKER_7POINT1_SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_SIDE_LEFT  | SPEAKER_SIDE_RIGHT)
#endif


#pragma pack(pop)

#endif
