








#ifndef __XACT3WB_H__
#define __XACT3WB_H__

#ifdef _XBOX
#   include <xtl.h>
#else
#   include <math.h>
#endif

#include <audiodefs.h>
#include <xma2defs.h>

#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(disable:4214)

#pragma pack(push, 1)
#if !defined(_X86_)
    #define XACTUNALIGNED __unaligned
#else
    #define XACTUNALIGNED
#endif

#ifdef _M_PPCBE
#pragma bitfield_order(push, lsb_to_msb)
#endif

#define WAVEBANK_HEADER_SIGNATURE               'DNBW'
#define WAVEBANK_HEADER_VERSION                 44

#define WAVEBANK_BANKNAME_LENGTH                64
#define WAVEBANK_ENTRYNAME_LENGTH               64

#define WAVEBANK_MAX_DATA_SEGMENT_SIZE          0xFFFFFFFF
#define WAVEBANK_MAX_COMPACT_DATA_SEGMENT_SIZE  0x001FFFFF

typedef DWORD WAVEBANKOFFSET;





#define WAVEBANK_TYPE_BUFFER         0x00000000
#define WAVEBANK_TYPE_STREAMING      0x00000001
#define WAVEBANK_TYPE_MASK           0x00000001

#define WAVEBANK_FLAGS_ENTRYNAMES    0x00010000
#define WAVEBANK_FLAGS_COMPACT       0x00020000
#define WAVEBANK_FLAGS_SYNC_DISABLED 0x00040000
#define WAVEBANK_FLAGS_SEEKTABLES    0x00080000
#define WAVEBANK_FLAGS_MASK          0x000F0000





#define WAVEBANKENTRY_FLAGS_READAHEAD       0x00000001
#define WAVEBANKENTRY_FLAGS_LOOPCACHE       0x00000002
#define WAVEBANKENTRY_FLAGS_REMOVELOOPTAIL  0x00000004
#define WAVEBANKENTRY_FLAGS_IGNORELOOP      0x00000008
#define WAVEBANKENTRY_FLAGS_MASK            0x00000008





#define WAVEBANKMINIFORMAT_TAG_PCM      0x0
#define WAVEBANKMINIFORMAT_TAG_XMA      0x1
#define WAVEBANKMINIFORMAT_TAG_ADPCM    0x2
#define WAVEBANKMINIFORMAT_TAG_WMA      0x3

#define WAVEBANKMINIFORMAT_BITDEPTH_8   0x0
#define WAVEBANKMINIFORMAT_BITDEPTH_16  0x1




#define WAVEBANKENTRY_XMASTREAMS_MAX          3
#define WAVEBANKENTRY_XMACHANNELS_MAX         6





#define WAVEBANK_DVD_SECTOR_SIZE    2048
#define WAVEBANK_DVD_BLOCK_SIZE     (WAVEBANK_DVD_SECTOR_SIZE * 16)





#define WAVEBANK_ALIGNMENT_MIN  4
#define WAVEBANK_ALIGNMENT_DVD  WAVEBANK_DVD_SECTOR_SIZE





typedef enum WAVEBANKSEGIDX
{
    WAVEBANK_SEGIDX_BANKDATA = 0,
    WAVEBANK_SEGIDX_ENTRYMETADATA,
    WAVEBANK_SEGIDX_SEEKTABLES,
    WAVEBANK_SEGIDX_ENTRYNAMES,
    WAVEBANK_SEGIDX_ENTRYWAVEDATA,
    WAVEBANK_SEGIDX_COUNT
} WAVEBANKSEGIDX, *LPWAVEBANKSEGIDX;

typedef const WAVEBANKSEGIDX *LPCWAVEBANKSEGIDX;





#ifdef __cplusplus

namespace XACTWaveBank
{
    __inline void SwapBytes(XACTUNALIGNED DWORD &dw)
    {

#ifdef _X86_

        __asm
        {
            mov edi, dw
            mov eax, [edi]
            bswap eax
            mov [edi], eax
        }

#else

        dw = _byteswap_ulong(dw);

#endif

    }

    __inline void SwapBytes(XACTUNALIGNED WORD &w)
    {

#ifdef _X86_

        __asm
        {
            mov edi, w
            mov ax, [edi]
            xchg ah, al
            mov [edi], ax
        }

#else

        w = _byteswap_ushort(w);

#endif

    }

}

#endif





typedef struct WAVEBANKREGION
{
    DWORD       dwOffset;
    DWORD       dwLength;

#ifdef __cplusplus

    void SwapBytes(void)
    {
        XACTWaveBank::SwapBytes(dwOffset);
        XACTWaveBank::SwapBytes(dwLength);
    }

#endif

} WAVEBANKREGION, *LPWAVEBANKREGION;

typedef const WAVEBANKREGION *LPCWAVEBANKREGION;






typedef struct WAVEBANKSAMPLEREGION
{
    DWORD       dwStartSample;
    DWORD       dwTotalSamples;

#ifdef __cplusplus

    void SwapBytes(void)
    {
        XACTWaveBank::SwapBytes(dwStartSample);
        XACTWaveBank::SwapBytes(dwTotalSamples);
    }

#endif

} WAVEBANKSAMPLEREGION, *LPWAVEBANKSAMPLEREGION;

typedef const WAVEBANKSAMPLEREGION *LPCWAVEBANKSAMPLEREGION;






typedef struct WAVEBANKHEADER
{
    DWORD           dwSignature;
    DWORD           dwVersion;
    DWORD           dwHeaderVersion;
    WAVEBANKREGION  Segments[WAVEBANK_SEGIDX_COUNT];

#ifdef __cplusplus

    void SwapBytes(void)
    {
        XACTWaveBank::SwapBytes(dwSignature);
        XACTWaveBank::SwapBytes(dwVersion);
        XACTWaveBank::SwapBytes(dwHeaderVersion);

        for(int i = 0; i < WAVEBANK_SEGIDX_COUNT; i++)
        {
            Segments[i].SwapBytes();
        }
    }

#endif

} WAVEBANKHEADER, *LPWAVEBANKHEADER;

typedef const WAVEBANKHEADER *LPCWAVEBANKHEADER;






#define MAX_WMA_AVG_BYTES_PER_SEC_ENTRIES 7

static const DWORD aWMAAvgBytesPerSec[] =
{
    12000,
    24000,
    4000,
    6000,
    8000,
    20000,
    2500
};







#define MAX_WMA_BLOCK_ALIGN_ENTRIES 17

static const DWORD aWMABlockAlign[] =
{
    929,
    1487,
    1280,
    2230,
    8917,
    8192,
    4459,
    5945,
    2304,
    1536,
    1485,
    1008,
    2731,
    4096,
    6827,
    5462,
    1280
};

struct WAVEBANKENTRY;





typedef union WAVEBANKMINIWAVEFORMAT
{
    struct
    {
        DWORD       wFormatTag      : 2;
        DWORD       nChannels       : 3;
        DWORD       nSamplesPerSec  : 18;
        DWORD       wBlockAlign     : 8;
        DWORD       wBitsPerSample  : 1;
    };

    DWORD           dwValue;

#ifdef __cplusplus

    void SwapBytes(void)
    {
        XACTWaveBank::SwapBytes(dwValue);
    }

    WORD BitsPerSample() const
    {
        if (wFormatTag == WAVEBANKMINIFORMAT_TAG_XMA)
            return XMA_OUTPUT_SAMPLE_BITS;
        if (wFormatTag == WAVEBANKMINIFORMAT_TAG_WMA)
            return 16;
        if (wFormatTag == WAVEBANKMINIFORMAT_TAG_ADPCM)
            return 4;


        return (wBitsPerSample == WAVEBANKMINIFORMAT_BITDEPTH_16) ? 16 : 8;
    }

    #define ADPCM_MINIWAVEFORMAT_BLOCKALIGN_CONVERSION_OFFSET 22
    DWORD BlockAlign() const
    {
        DWORD dwReturn = 0;

        switch (wFormatTag)
        {
        case WAVEBANKMINIFORMAT_TAG_PCM:
            dwReturn = wBlockAlign;
            break;

        case WAVEBANKMINIFORMAT_TAG_XMA:
            dwReturn = nChannels * XMA_OUTPUT_SAMPLE_BITS / 8;
            break;

        case WAVEBANKMINIFORMAT_TAG_ADPCM:
            dwReturn = (wBlockAlign + ADPCM_MINIWAVEFORMAT_BLOCKALIGN_CONVERSION_OFFSET) * nChannels;
            break;

        case WAVEBANKMINIFORMAT_TAG_WMA:
            {
                DWORD dwBlockAlignIndex = wBlockAlign & 0x1F;
                if (dwBlockAlignIndex < MAX_WMA_BLOCK_ALIGN_ENTRIES)
                        dwReturn = aWMABlockAlign[dwBlockAlignIndex];
            }
            break;
        }

        return dwReturn;
    }

    DWORD AvgBytesPerSec() const
    {
        DWORD dwReturn = 0;

        switch (wFormatTag)
        {
        case WAVEBANKMINIFORMAT_TAG_PCM:
        case WAVEBANKMINIFORMAT_TAG_XMA:
            dwReturn = nSamplesPerSec * wBlockAlign;
            break;

        case WAVEBANKMINIFORMAT_TAG_ADPCM:
            {
                DWORD blockAlign = BlockAlign();
                DWORD samplesPerAdpcmBlock = AdpcmSamplesPerBlock();
                dwReturn = blockAlign * nSamplesPerSec / samplesPerAdpcmBlock;
            }
            break;

        case WAVEBANKMINIFORMAT_TAG_WMA:
            {
                DWORD dwBytesPerSecIndex = wBlockAlign >> 5;
                if (dwBytesPerSecIndex < MAX_WMA_AVG_BYTES_PER_SEC_ENTRIES)
                    dwReturn = aWMAAvgBytesPerSec[dwBytesPerSecIndex];
            }
            break;
        }

        return dwReturn;
    }

    DWORD EncodeWMABlockAlign(DWORD dwBlockAlign, DWORD dwAvgBytesPerSec) const
    {
        DWORD dwReturn = 0;
        DWORD dwBlockAlignIndex = 0;
        DWORD dwBytesPerSecIndex = 0;

        for (; dwBlockAlignIndex < MAX_WMA_BLOCK_ALIGN_ENTRIES && dwBlockAlign != aWMABlockAlign[dwBlockAlignIndex]; dwBlockAlignIndex++);

        if (dwBlockAlignIndex < MAX_WMA_BLOCK_ALIGN_ENTRIES)
        {
            for (; dwBytesPerSecIndex < MAX_WMA_AVG_BYTES_PER_SEC_ENTRIES && dwAvgBytesPerSec != aWMAAvgBytesPerSec[dwBytesPerSecIndex]; dwBytesPerSecIndex++);

            if (dwBytesPerSecIndex < MAX_WMA_AVG_BYTES_PER_SEC_ENTRIES)
            {
                dwReturn = dwBlockAlignIndex | (dwBytesPerSecIndex << 5);
            }
        }

        return dwReturn;
    }


    void XMA2FillFormatEx(XMA2WAVEFORMATEX *fmt, WORD blockCount, const struct WAVEBANKENTRY* entry) const;

    DWORD AdpcmSamplesPerBlock() const
    {
        DWORD nBlockAlign = (wBlockAlign + ADPCM_MINIWAVEFORMAT_BLOCKALIGN_CONVERSION_OFFSET) * nChannels;
        return nBlockAlign * 2 / (DWORD)nChannels - 12;
    }

    void AdpcmFillCoefficientTable(ADPCMWAVEFORMAT *fmt) const
    {

        fmt->wNumCoef = 7;

        static ADPCMCOEFSET aCoef[7] = { { 256, 0}, {512, -256}, {0,0}, {192,64}, {240,0}, {460, -208}, {392,-232} };
        memcpy( &fmt->aCoef, aCoef, sizeof(aCoef) );
    }

#endif

} WAVEBANKMINIWAVEFORMAT, *LPWAVEBANKMINIWAVEFORMAT;

typedef const WAVEBANKMINIWAVEFORMAT *LPCWAVEBANKMINIWAVEFORMAT;





typedef struct WAVEBANKENTRY
{
    union
    {
        struct
        {

            DWORD                   dwFlags  :  4;







            DWORD                   Duration : 28;
        };
        DWORD dwFlagsAndDuration;
    };

    WAVEBANKMINIWAVEFORMAT  Format;
    WAVEBANKREGION          PlayRegion;
    WAVEBANKSAMPLEREGION    LoopRegion;

#ifdef __cplusplus

    void SwapBytes(void)
    {
        XACTWaveBank::SwapBytes(dwFlagsAndDuration);
        Format.SwapBytes();
        PlayRegion.SwapBytes();
        LoopRegion.SwapBytes();
    }

#endif

} WAVEBANKENTRY, *LPWAVEBANKENTRY;

typedef const WAVEBANKENTRY *LPCWAVEBANKENTRY;





typedef struct WAVEBANKENTRYCOMPACT
{
    DWORD       dwOffset            : 21;
    DWORD       dwLengthDeviation   : 11;

#ifdef __cplusplus

    void SwapBytes(void)
    {
        XACTWaveBank::SwapBytes(*(LPDWORD)this);
    }

#endif

} WAVEBANKENTRYCOMPACT, *LPWAVEBANKENTRYCOMPACT;

typedef const WAVEBANKENTRYCOMPACT *LPCWAVEBANKENTRYCOMPACT;





typedef struct WAVEBANKDATA
{
    DWORD                   dwFlags;
    DWORD                   dwEntryCount;
    CHAR                    szBankName[WAVEBANK_BANKNAME_LENGTH];
    DWORD                   dwEntryMetaDataElementSize;
    DWORD                   dwEntryNameElementSize;
    DWORD                   dwAlignment;
    WAVEBANKMINIWAVEFORMAT  CompactFormat;
    FILETIME                BuildTime;

#ifdef __cplusplus

    void SwapBytes(void)
    {
        XACTWaveBank::SwapBytes(dwFlags);
        XACTWaveBank::SwapBytes(dwEntryCount);
        XACTWaveBank::SwapBytes(dwEntryMetaDataElementSize);
        XACTWaveBank::SwapBytes(dwEntryNameElementSize);
        XACTWaveBank::SwapBytes(dwAlignment);
        CompactFormat.SwapBytes();
        XACTWaveBank::SwapBytes(BuildTime.dwLowDateTime);
        XACTWaveBank::SwapBytes(BuildTime.dwHighDateTime);
    }

#endif

} WAVEBANKDATA, *LPWAVEBANKDATA;

typedef const WAVEBANKDATA *LPCWAVEBANKDATA;

inline void WAVEBANKMINIWAVEFORMAT::XMA2FillFormatEx(XMA2WAVEFORMATEX *fmt, WORD blockCount, const WAVEBANKENTRY* entry) const
{


    fmt->NumStreams = (WORD)( (nChannels + 1) / 2 );

    switch (nChannels)
    {
        case 1: fmt->ChannelMask =  SPEAKER_MONO; break;
        case 2: fmt->ChannelMask =  SPEAKER_STEREO; break;
        case 3: fmt->ChannelMask =  SPEAKER_2POINT1; break;
        case 4: fmt->ChannelMask =  SPEAKER_QUAD; break;
        case 5: fmt->ChannelMask =  SPEAKER_4POINT1; break;
        case 6: fmt->ChannelMask =  SPEAKER_5POINT1; break;
        case 7: fmt->ChannelMask =  SPEAKER_5POINT1 | SPEAKER_BACK_CENTER; break;
        case 8: fmt->ChannelMask =  SPEAKER_7POINT1; break;
        default: fmt->ChannelMask = 0; break;
    }

    fmt->SamplesEncoded = entry->Duration;
    fmt->BytesPerBlock = 65536;

    fmt->PlayBegin = entry->PlayRegion.dwOffset;
    fmt->PlayLength = entry->PlayRegion.dwLength;

    if (entry->LoopRegion.dwTotalSamples > 0)
    {
        fmt->LoopBegin = entry->LoopRegion.dwStartSample;
        fmt->LoopLength = entry->LoopRegion.dwTotalSamples;
        fmt->LoopCount = 0xff;
    }
    else
    {
        fmt->LoopBegin = 0;
        fmt->LoopLength = 0;
        fmt->LoopCount = 0;
    }

    fmt->EncoderVersion = 4;

    fmt->BlockCount = blockCount;
}

#ifdef _M_PPCBE
#pragma bitfield_order(pop)
#endif

#pragma warning(pop)
#pragma pack(pop)

#endif

