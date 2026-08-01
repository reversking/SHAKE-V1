








#ifndef __XMA2DEFS_INCLUDED__
#define __XMA2DEFS_INCLUDED__

#include <sal.h>
#include <winerror.h>
#include <audiodefs.h>


























































































































#define XMA_OUTPUT_SAMPLE_BYTES         2u
#define XMA_OUTPUT_SAMPLE_BITS          (XMA_OUTPUT_SAMPLE_BYTES * 8u)


#define XMA_BYTES_PER_PACKET            2048u
#define XMA_BITS_PER_PACKET             (XMA_BYTES_PER_PACKET * 8u)


#define XMA_PACKET_HEADER_BYTES         4u
#define XMA_PACKET_HEADER_BITS          (XMA_PACKET_HEADER_BYTES * 8u)


#define XMA_SAMPLES_PER_FRAME           512u


#define XMA_SAMPLES_PER_SUBFRAME        128u


#define XMA_READBUFFER_MAX_PACKETS      4095u
#define XMA_READBUFFER_MAX_BYTES        (XMA_READBUFFER_MAX_PACKETS * XMA_BYTES_PER_PACKET)


#define XMA_WRITEBUFFER_MAX_BYTES       (31u * 256u)


#define XMA_WRITEBUFFER_BYTE_ALIGNMENT  256u


#define XMA_MIN_SUBFRAMES_TO_DECODE     1u
#define XMA_MAX_SUBFRAMES_TO_DECODE     8u
#define XMA_OPTIMAL_SUBFRAMES_TO_DECODE 4u


#define XMA_MAX_LOOPCOUNT               254u
#define XMA_INFINITE_LOOP               255u












#define WAVE_FORMAT_XMA2 0x166

typedef struct XMA2WAVEFORMATEX
{
    WAVEFORMATEX wfx;









    WORD  NumStreams;
    DWORD ChannelMask;

    DWORD SamplesEncoded;
    DWORD BytesPerBlock;
    DWORD PlayBegin;
    DWORD PlayLength;
    DWORD LoopBegin;
    DWORD LoopLength;
    BYTE  LoopCount;
    BYTE  EncoderVersion;
    WORD  BlockCount;
} XMA2WAVEFORMATEX, *PXMA2WAVEFORMATEX;







#ifndef WAVE_FORMAT_XMA
#define WAVE_FORMAT_XMA 0x0165



#ifndef XMA_SPEAKER_LEFT
    #define XMA_SPEAKER_LEFT            0x01
    #define XMA_SPEAKER_RIGHT           0x02
    #define XMA_SPEAKER_CENTER          0x04
    #define XMA_SPEAKER_LFE             0x08
    #define XMA_SPEAKER_LEFT_SURROUND   0x10
    #define XMA_SPEAKER_RIGHT_SURROUND  0x20
    #define XMA_SPEAKER_LEFT_BACK       0x40
    #define XMA_SPEAKER_RIGHT_BACK      0x80
#endif



typedef struct XMASTREAMFORMAT
{
    DWORD PsuedoBytesPerSec;
    DWORD SampleRate;

    DWORD LoopStart;

    DWORD LoopEnd;
    BYTE  SubframeData;





    BYTE  Channels;
    WORD  ChannelMask;
} XMASTREAMFORMAT;


typedef struct XMAWAVEFORMAT
{
    WORD FormatTag;
    WORD BitsPerSample;
    WORD EncodeOptions;
    WORD LargestSkip;
    WORD NumStreams;
    BYTE LoopCount;
    BYTE Version;

    XMASTREAMFORMAT XmaStreams[1];

} XMAWAVEFORMAT;



typedef struct XMA2STREAMFORMAT
{
    BYTE Channels;
    BYTE RESERVED;
    WORD ChannelMask;
} XMA2STREAMFORMAT;


typedef struct XMA2WAVEFORMAT
{
    BYTE  Version;

    BYTE  NumStreams;
    BYTE  RESERVED;
    BYTE  LoopCount;
    DWORD LoopBegin;
    DWORD LoopEnd;
    DWORD SampleRate;
    DWORD EncodeOptions;
    DWORD PsuedoBytesPerSec;
    DWORD BlockSizeInBytes;


    DWORD SamplesEncoded;
    DWORD SamplesInSource;

    DWORD BlockCount;

    XMA2STREAMFORMAT Streams[1];

} XMA2WAVEFORMAT;

#endif







typedef struct XMA2PACKET
{
    int FrameCount        :  6;
    int FrameOffsetInBits : 15;
    int PacketMetaData    :  3;
    int PacketSkipCount   :  8;

    BYTE XmaData[XMA_BYTES_PER_PACKET - sizeof(DWORD)];
} XMA2PACKET;














__inline DWORD GetXmaPacketFrameCount(__in_bcount(1) const BYTE* pPacket)
{
    return (DWORD)(pPacket[0] >> 2);
}

__inline DWORD GetXmaPacketFirstFrameOffsetInBits(__in_bcount(3) const BYTE* pPacket)
{
    return ((DWORD)(pPacket[0] & 0x3) << 13) |
           ((DWORD)(pPacket[1]) << 5) |
           ((DWORD)(pPacket[2]) >> 3);
}

__inline DWORD GetXmaPacketMetadata(__in_bcount(3) const BYTE* pPacket)
{
    return (DWORD)(pPacket[2] & 0x7);
}

__inline DWORD GetXmaPacketSkipCount(__in_bcount(4) const BYTE* pPacket)
{
    return (DWORD)(pPacket[3]);
}


















#define XMA_BITS_IN_FRAME_LENGTH_FIELD 15


#define XMA_FINAL_FRAME_MARKER 0x7FFF









#ifndef XMA2DEFS_ASSERT
    #ifdef ASSERT
        #define XMA2DEFS_ASSERT ASSERT
    #else
        #define XMA2DEFS_ASSERT(a)
    #endif
#endif







__inline HRESULT GetXmaBlockContainingSample
(
    DWORD nBlockCount,
    __in_ecount(nBlockCount) const DWORD* pSeekTable,
    DWORD nDesiredSample,
    __out DWORD* pnBlockContainingSample,
    __out DWORD* pnSampleOffsetWithinBlock
)
{
    DWORD nPreviousTotalSamples = 0;
    DWORD nBlock;
    DWORD nTotalSamplesSoFar;

    XMA2DEFS_ASSERT(pSeekTable);
    XMA2DEFS_ASSERT(pnBlockContainingSample);
    XMA2DEFS_ASSERT(pnSampleOffsetWithinBlock);

    for (nBlock = 0; nBlock < nBlockCount; ++nBlock)
    {
        nTotalSamplesSoFar = pSeekTable[nBlock];
        if (nTotalSamplesSoFar > nDesiredSample)
        {
            *pnBlockContainingSample = nBlock;
            *pnSampleOffsetWithinBlock = nDesiredSample - nPreviousTotalSamples;
            return S_OK;
        }
        nPreviousTotalSamples = nTotalSamplesSoFar;
    }

    return E_FAIL;
}




__inline DWORD GetXmaFrameLengthInBits
(
    __in_bcount(nBitPosition / 8 + 3)
    __in const BYTE* pPacket,
    DWORD nBitPosition
)
{
    DWORD nRegion;
    DWORD nBytePosition = nBitPosition / 8;
    DWORD nBitOffset = nBitPosition % 8;

    if (nBitOffset < 2)
    {
        nRegion = (DWORD)(pPacket[nBytePosition+0]) << 8 |
                  (DWORD)(pPacket[nBytePosition+1]);
        return (nRegion >> (1 - nBitOffset)) & 0x7FFF;
    }
    else
    {
        nRegion = (DWORD)(pPacket[nBytePosition+0]) << 16 |
                  (DWORD)(pPacket[nBytePosition+1]) << 8 |
                  (DWORD)(pPacket[nBytePosition+2]);
        return (nRegion >> (9 - nBitOffset)) & 0x7FFF;
    }
}





__inline DWORD GetXmaFrameBitPosition
(
    __in_bcount(nXmaDataBytes) const BYTE* pXmaData,
    DWORD nXmaDataBytes,
    DWORD nStreamIndex,
    DWORD nDesiredFrame
)
{
    const BYTE* pCurrentPacket;
    DWORD nPacketsExamined = 0;
    DWORD nFrameCountSoFar = 0;
    DWORD nFramesToSkip;
    DWORD nFrameBitOffset;

    XMA2DEFS_ASSERT(pXmaData);
    XMA2DEFS_ASSERT(nXmaDataBytes % XMA_BYTES_PER_PACKET == 0);





    pCurrentPacket = pXmaData + nStreamIndex * XMA_BYTES_PER_PACKET;
    for (;;)
    {

        if (pCurrentPacket + XMA_BYTES_PER_PACKET > pXmaData + nXmaDataBytes)
        {
            return 0;
        }


        if (nFrameCountSoFar + GetXmaPacketFrameCount(pCurrentPacket) > nDesiredFrame)
        {

            XMA2DEFS_ASSERT(nDesiredFrame >= nFrameCountSoFar);
            nFramesToSkip = nDesiredFrame - nFrameCountSoFar;


            nFrameBitOffset = XMA_PACKET_HEADER_BITS + GetXmaPacketFirstFrameOffsetInBits(pCurrentPacket);


            while (nFramesToSkip--)
            {
                nFrameBitOffset += GetXmaFrameLengthInBits(pCurrentPacket, nFrameBitOffset);
            }



            return (DWORD)(pCurrentPacket - pXmaData) * 8 + nFrameBitOffset;
        }


        ++nPacketsExamined;
        nFrameCountSoFar += GetXmaPacketFrameCount(pCurrentPacket);


        pCurrentPacket += XMA_BYTES_PER_PACKET * (GetXmaPacketSkipCount(pCurrentPacket) + 1);
    }
}





__inline DWORD GetLastXmaFrameBitPosition
(
    __in_bcount(nXmaDataBytes) const BYTE* pXmaData,
    DWORD nXmaDataBytes,
    DWORD nStreamIndex
)
{
    const BYTE* pLastPacket;
    DWORD nBytesToNextPacket;
    DWORD nFrameBitOffset;
    DWORD nFramesInLastPacket;

    XMA2DEFS_ASSERT(pXmaData);
    XMA2DEFS_ASSERT(nXmaDataBytes % XMA_BYTES_PER_PACKET == 0);
    XMA2DEFS_ASSERT(nXmaDataBytes >= XMA_BYTES_PER_PACKET * (nStreamIndex + 1));




    pLastPacket = pXmaData + nStreamIndex * XMA_BYTES_PER_PACKET;


    for (;;)
    {
        nBytesToNextPacket = XMA_BYTES_PER_PACKET * (GetXmaPacketSkipCount(pLastPacket) + 1);
        XMA2DEFS_ASSERT(nBytesToNextPacket);
        if (pLastPacket + nBytesToNextPacket + XMA_BYTES_PER_PACKET > pXmaData + nXmaDataBytes)
        {
            break;
        }
        pLastPacket += nBytesToNextPacket;
    }



    if (GetXmaPacketFrameCount(pLastPacket) == 0)
    {
        pLastPacket -= nBytesToNextPacket;
    }


    nFrameBitOffset = XMA_PACKET_HEADER_BITS + GetXmaPacketFirstFrameOffsetInBits(pLastPacket);


    nFramesInLastPacket = GetXmaPacketFrameCount(pLastPacket);
    while (--nFramesInLastPacket)
    {
        nFrameBitOffset += GetXmaFrameLengthInBits(pLastPacket, nFrameBitOffset);
    }



    return (DWORD)(pLastPacket - pXmaData) * 8 + nFrameBitOffset;
}








__inline HRESULT GetXmaDecodePositionForSample
(
    __in_bcount(nXmaDataBytes) const BYTE* pXmaData,
    DWORD nXmaDataBytes,
    DWORD nStreamIndex,
    DWORD nDesiredSample,
    __out DWORD* pnBitOffset,

    __out DWORD* pnSubFrame
)
{
    DWORD nDesiredFrame = nDesiredSample / XMA_SAMPLES_PER_FRAME;
    DWORD nSubFrame = (nDesiredSample % XMA_SAMPLES_PER_FRAME) / XMA_SAMPLES_PER_SUBFRAME;
    DWORD nBitOffset = GetXmaFrameBitPosition(pXmaData, nXmaDataBytes, nStreamIndex, nDesiredFrame);

    XMA2DEFS_ASSERT(pnBitOffset);
    XMA2DEFS_ASSERT(pnSubFrame);

    if (nBitOffset)
    {
        *pnBitOffset = nBitOffset;
        *pnSubFrame = nSubFrame;
        return S_OK;
    }
    else
    {
        return E_FAIL;
    }
}





__inline DWORD GetXmaSampleRate(DWORD dwGeneralRate)
{
    DWORD dwXmaRate = 48000;

    if (dwGeneralRate <= 24000)      dwXmaRate = 24000;
    else if (dwGeneralRate <= 32000) dwXmaRate = 32000;
    else if (dwGeneralRate <= 44100) dwXmaRate = 44100;

    return dwXmaRate;
}







__inline DWORD GetStandardChannelMaskFromXmaMask(BYTE bXmaMask)
{
    DWORD dwStandardMask = 0;

    if (bXmaMask & XMA_SPEAKER_LEFT)           dwStandardMask |= SPEAKER_FRONT_LEFT;
    if (bXmaMask & XMA_SPEAKER_RIGHT)          dwStandardMask |= SPEAKER_FRONT_RIGHT;
    if (bXmaMask & XMA_SPEAKER_CENTER)         dwStandardMask |= SPEAKER_FRONT_CENTER;
    if (bXmaMask & XMA_SPEAKER_LFE)            dwStandardMask |= SPEAKER_LOW_FREQUENCY;
    if (bXmaMask & XMA_SPEAKER_LEFT_SURROUND)  dwStandardMask |= SPEAKER_SIDE_LEFT;
    if (bXmaMask & XMA_SPEAKER_RIGHT_SURROUND) dwStandardMask |= SPEAKER_SIDE_RIGHT;
    if (bXmaMask & XMA_SPEAKER_LEFT_BACK)      dwStandardMask |= SPEAKER_BACK_LEFT;
    if (bXmaMask & XMA_SPEAKER_RIGHT_BACK)     dwStandardMask |= SPEAKER_BACK_RIGHT;

    return dwStandardMask;
}

__inline BYTE GetXmaChannelMaskFromStandardMask(DWORD dwStandardMask)
{
    BYTE bXmaMask = 0;

    if (dwStandardMask & SPEAKER_FRONT_LEFT)    bXmaMask |= XMA_SPEAKER_LEFT;
    if (dwStandardMask & SPEAKER_FRONT_RIGHT)   bXmaMask |= XMA_SPEAKER_RIGHT;
    if (dwStandardMask & SPEAKER_FRONT_CENTER)  bXmaMask |= XMA_SPEAKER_CENTER;
    if (dwStandardMask & SPEAKER_LOW_FREQUENCY) bXmaMask |= XMA_SPEAKER_LFE;
    if (dwStandardMask & SPEAKER_SIDE_LEFT)     bXmaMask |= XMA_SPEAKER_LEFT_SURROUND;
    if (dwStandardMask & SPEAKER_SIDE_RIGHT)    bXmaMask |= XMA_SPEAKER_RIGHT_SURROUND;
    if (dwStandardMask & SPEAKER_BACK_LEFT)     bXmaMask |= XMA_SPEAKER_LEFT_BACK;
    if (dwStandardMask & SPEAKER_BACK_RIGHT)    bXmaMask |= XMA_SPEAKER_RIGHT_BACK;

    return bXmaMask;
}





__inline HRESULT LocalizeXma2Format(__inout XMA2WAVEFORMATEX* pXma2Format)
{
    #define XMASWAP2BYTES(n) ((WORD)(((n) >> 8) | (((n) & 0xff) << 8)))
    #define XMASWAP4BYTES(n) ((DWORD)((n) >> 24 | (n) << 24 | ((n) & 0xff00) << 8 | ((n) & 0xff0000) >> 8))

    if (pXma2Format->wfx.wFormatTag == WAVE_FORMAT_XMA2)
    {
        return S_OK;
    }
    else if (XMASWAP2BYTES(pXma2Format->wfx.wFormatTag) == WAVE_FORMAT_XMA2)
    {
        pXma2Format->wfx.wFormatTag      = XMASWAP2BYTES(pXma2Format->wfx.wFormatTag);
        pXma2Format->wfx.nChannels       = XMASWAP2BYTES(pXma2Format->wfx.nChannels);
        pXma2Format->wfx.nSamplesPerSec  = XMASWAP4BYTES(pXma2Format->wfx.nSamplesPerSec);
        pXma2Format->wfx.nAvgBytesPerSec = XMASWAP4BYTES(pXma2Format->wfx.nAvgBytesPerSec);
        pXma2Format->wfx.nBlockAlign     = XMASWAP2BYTES(pXma2Format->wfx.nBlockAlign);
        pXma2Format->wfx.wBitsPerSample  = XMASWAP2BYTES(pXma2Format->wfx.wBitsPerSample);
        pXma2Format->wfx.cbSize          = XMASWAP2BYTES(pXma2Format->wfx.cbSize);
        pXma2Format->NumStreams          = XMASWAP2BYTES(pXma2Format->NumStreams);
        pXma2Format->ChannelMask         = XMASWAP4BYTES(pXma2Format->ChannelMask);
        pXma2Format->SamplesEncoded      = XMASWAP4BYTES(pXma2Format->SamplesEncoded);
        pXma2Format->BytesPerBlock       = XMASWAP4BYTES(pXma2Format->BytesPerBlock);
        pXma2Format->PlayBegin           = XMASWAP4BYTES(pXma2Format->PlayBegin);
        pXma2Format->PlayLength          = XMASWAP4BYTES(pXma2Format->PlayLength);
        pXma2Format->LoopBegin           = XMASWAP4BYTES(pXma2Format->LoopBegin);
        pXma2Format->LoopLength          = XMASWAP4BYTES(pXma2Format->LoopLength);
        pXma2Format->BlockCount          = XMASWAP2BYTES(pXma2Format->BlockCount);
        return S_OK;
    }
    else
    {
        return E_FAIL;
    }

    #undef XMASWAP2BYTES
    #undef XMASWAP4BYTES
}


#endif
