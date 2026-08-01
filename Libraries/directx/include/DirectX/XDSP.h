





















#pragma once

#include <windef.h>
#include <math.h>
#if defined(_XBOX)
    #include <ppcintrinsics.h>
#else
    #include <emmintrin.h>
#endif




#if !defined(DSPASSERT)
    #if DBG
        #define DSPASSERT(exp) if (!(exp)) { OutputDebugStringA("XDSP ASSERT: " #exp ", {" __FUNCTION__ "}\n"); __debugbreak(); }
    #else
        #define DSPASSERT(exp) __assume(exp)
    #endif
#endif


#if !defined(ISPOWEROF2)
    #define ISPOWEROF2(n) ( ((n)&((n)-1)) == 0 && (n) != 0 )
#endif



namespace XDSP {
#pragma warning(push)
#pragma warning(disable: 4328 4640)






    typedef __m128 XVECTOR;
    typedef XVECTOR& XVECTORREF;
    typedef const XVECTOR& XVECTORREFC;




    __forceinline void vmulComplex (__out XVECTORREF rResult, __out XVECTORREF iResult, __in XVECTORREFC r1, __in XVECTORREFC i1, __in XVECTORREFC r2, __in XVECTORREFC i2)
    {

        XVECTOR vi1i2 = _mm_mul_ps(i1, i2);
        XVECTOR vr1r2 = _mm_mul_ps(r1, r2);
        XVECTOR vr1i2 = _mm_mul_ps(r1, i2);
        XVECTOR vr2i1 = _mm_mul_ps(r2, i1);
        rResult = _mm_sub_ps(vr1r2, vi1i2);
        iResult = _mm_add_ps(vr1i2, vr2i1);
    }
    __forceinline void vmulComplex (__inout XVECTORREF r1, __inout XVECTORREF i1, __in XVECTORREFC r2, __in XVECTORREFC i2)
    {

        XVECTOR vi1i2 = _mm_mul_ps(i1, i2);
        XVECTOR vr1r2 = _mm_mul_ps(r1, r2);
        XVECTOR vr1i2 = _mm_mul_ps(r1, i2);
        XVECTOR vr2i1 = _mm_mul_ps(r2, i1);
        r1 = _mm_sub_ps(vr1r2, vi1i2);
        i1 = _mm_add_ps(vr1i2, vr2i1);
    }






























    __forceinline void ButterflyDIT4_1 (__inout XVECTORREF r1, __inout XVECTORREF i1)
    {

        const static XVECTOR vDFT4SignBits1 = { 0.0f, -0.0f,  0.0f, -0.0f };
        const static XVECTOR vDFT4SignBits2 = { 0.0f,  0.0f, -0.0f, -0.0f };
        const static XVECTOR vDFT4SignBits3 = { 0.0f, -0.0f, -0.0f,  0.0f };



        XVECTOR rTemp = _mm_add_ps( _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(1, 1, 0, 0)),
                                    _mm_xor_ps(_mm_shuffle_ps(r1, r1, _MM_SHUFFLE(3, 3, 2, 2)), vDFT4SignBits1) );
        XVECTOR iTemp = _mm_add_ps( _mm_shuffle_ps(i1, i1, _MM_SHUFFLE(1, 1, 0, 0)),
                                    _mm_xor_ps(_mm_shuffle_ps(i1, i1, _MM_SHUFFLE(3, 3, 2, 2)), vDFT4SignBits1) );


        XVECTOR rZrWiZiW = _mm_shuffle_ps(rTemp, iTemp, _MM_SHUFFLE(3, 2, 3, 2));
        XVECTOR rZiWrZiW = _mm_shuffle_ps(rZrWiZiW, rZrWiZiW, _MM_SHUFFLE(3, 0, 3, 0));
        XVECTOR iZrWiZrW = _mm_shuffle_ps(rZrWiZiW, rZrWiZiW, _MM_SHUFFLE(1, 2, 1, 2));
        r1 = _mm_add_ps( _mm_shuffle_ps(rTemp, rTemp, _MM_SHUFFLE(1, 0, 1, 0)),
                         _mm_xor_ps(rZiWrZiW, vDFT4SignBits2) );
        i1 = _mm_add_ps( _mm_shuffle_ps(iTemp, iTemp, _MM_SHUFFLE(1, 0, 1, 0)),
                         _mm_xor_ps(iZrWiZrW, vDFT4SignBits3) );
    }




















    __forceinline void ButterflyDIT4_4 (__inout XVECTORREF r0,
                                        __inout XVECTORREF r1,
                                        __inout XVECTORREF r2,
                                        __inout XVECTORREF r3,
                                        __inout XVECTORREF i0,
                                        __inout XVECTORREF i1,
                                        __inout XVECTORREF i2,
                                        __inout XVECTORREF i3,
                                        __in_ecount(uStride*4) const XVECTOR* __restrict pUnityTableReal,
                                        __in_ecount(uStride*4) const XVECTOR* __restrict pUnityTableImaginary,
                                        const UINT32 uStride, const BOOL fLast)
    {
        DSPASSERT(pUnityTableReal != NULL);
        DSPASSERT(pUnityTableImaginary != NULL);
        DSPASSERT((UINT_PTR)pUnityTableReal % 16 == 0);
        DSPASSERT((UINT_PTR)pUnityTableImaginary % 16 == 0);
        DSPASSERT(ISPOWEROF2(uStride));

        XVECTOR rTemp0, rTemp1, rTemp2, rTemp3, rTemp4, rTemp5, rTemp6, rTemp7;
        XVECTOR iTemp0, iTemp1, iTemp2, iTemp3, iTemp4, iTemp5, iTemp6, iTemp7;



        rTemp0 = _mm_add_ps(r0, r2);          iTemp0 = _mm_add_ps(i0, i2);
        rTemp2 = _mm_add_ps(r1, r3);          iTemp2 = _mm_add_ps(i1, i3);
        rTemp1 = _mm_sub_ps(r0, r2);          iTemp1 = _mm_sub_ps(i0, i2);
        rTemp3 = _mm_sub_ps(r1, r3);          iTemp3 = _mm_sub_ps(i1, i3);
        rTemp4 = _mm_add_ps(rTemp0, rTemp2);  iTemp4 = _mm_add_ps(iTemp0, iTemp2);
        rTemp5 = _mm_add_ps(rTemp1, iTemp3);  iTemp5 = _mm_sub_ps(iTemp1, rTemp3);
        rTemp6 = _mm_sub_ps(rTemp0, rTemp2);  iTemp6 = _mm_sub_ps(iTemp0, iTemp2);
        rTemp7 = _mm_sub_ps(rTemp1, iTemp3);  iTemp7 = _mm_add_ps(iTemp1, rTemp3);



        vmulComplex(rTemp5, iTemp5, pUnityTableReal[uStride], pUnityTableImaginary[uStride]);
        vmulComplex(rTemp6, iTemp6, pUnityTableReal[uStride*2], pUnityTableImaginary[uStride*2]);
        vmulComplex(rTemp7, iTemp7, pUnityTableReal[uStride*3], pUnityTableImaginary[uStride*3]);
        if (fLast) {
            ButterflyDIT4_1(rTemp4, iTemp4);
            ButterflyDIT4_1(rTemp5, iTemp5);
            ButterflyDIT4_1(rTemp6, iTemp6);
            ButterflyDIT4_1(rTemp7, iTemp7);
        }


        r0 = rTemp4;    i0 = iTemp4;
        r1 = rTemp5;    i1 = iTemp5;
        r2 = rTemp6;    i2 = iTemp6;
        r3 = rTemp7;    i3 = iTemp7;
    }















    __forceinline void FFT4 (__inout_ecount(uCount) XVECTOR* __restrict pReal, __inout_ecount(uCount) XVECTOR* __restrict pImaginary, const UINT32 uCount=1)
    {
        DSPASSERT(pReal != NULL);
        DSPASSERT(pImaginary != NULL);
        DSPASSERT((UINT_PTR)pReal % 16 == 0);
        DSPASSERT((UINT_PTR)pImaginary % 16 == 0);
        DSPASSERT(ISPOWEROF2(uCount));

        for (UINT32 uIndex=0; uIndex<uCount; ++uIndex) {
            ButterflyDIT4_1(pReal[uIndex], pImaginary[uIndex]);
        }
    }















    __forceinline void FFT8 (__inout_ecount(uCount*2) XVECTOR* __restrict pReal, __inout_ecount(uCount*2) XVECTOR* __restrict pImaginary, const UINT32 uCount=1)
    {
        DSPASSERT(pReal != NULL);
        DSPASSERT(pImaginary != NULL);
        DSPASSERT((UINT_PTR)pReal % 16 == 0);
        DSPASSERT((UINT_PTR)pImaginary % 16 == 0);
        DSPASSERT(ISPOWEROF2(uCount));

        static XVECTOR wr1 = {  1.0f,  0.70710677f,  0.0f, -0.70710677f };
        static XVECTOR wi1 = {  0.0f, -0.70710677f, -1.0f, -0.70710677f };
        static XVECTOR wr2 = { -1.0f, -0.70710677f,  0.0f,  0.70710677f };
        static XVECTOR wi2 = {  0.0f,  0.70710677f,  1.0f,  0.70710677f };


        for (UINT32 uIndex=0; uIndex<uCount; ++uIndex) {
            XVECTOR* __restrict pR = pReal      + uIndex*2;
            XVECTOR* __restrict pI = pImaginary + uIndex*2;

            XVECTOR oddsR  = _mm_shuffle_ps(pR[0], pR[1], _MM_SHUFFLE(3, 1, 3, 1));
            XVECTOR evensR = _mm_shuffle_ps(pR[0], pR[1], _MM_SHUFFLE(2, 0, 2, 0));
            XVECTOR oddsI  = _mm_shuffle_ps(pI[0], pI[1], _MM_SHUFFLE(3, 1, 3, 1));
            XVECTOR evensI = _mm_shuffle_ps(pI[0], pI[1], _MM_SHUFFLE(2, 0, 2, 0));
            ButterflyDIT4_1(oddsR, oddsI);
            ButterflyDIT4_1(evensR, evensI);

            XVECTOR r, i;
            vmulComplex(r, i, oddsR, oddsI, wr1, wi1);
            pR[0] = _mm_add_ps(evensR, r);
            pI[0] = _mm_add_ps(evensI, i);

            vmulComplex(r, i, oddsR, oddsI, wr2, wi2);
            pR[1] = _mm_add_ps(evensR, r);
            pI[1] = _mm_add_ps(evensI, i);
        }
    }















    __forceinline void FFT16 (__inout_ecount(uCount*4) XVECTOR* __restrict pReal, __inout_ecount(uCount*4) XVECTOR* __restrict pImaginary, const UINT32 uCount=1)
    {
        DSPASSERT(pReal != NULL);
        DSPASSERT(pImaginary != NULL);
        DSPASSERT((UINT_PTR)pReal % 16 == 0);
        DSPASSERT((UINT_PTR)pImaginary % 16 == 0);
        DSPASSERT(ISPOWEROF2(uCount));

        XVECTOR aUnityTableReal[4]      = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.92387950f, 0.70710677f, 0.38268343f, 1.0f, 0.70710677f, -4.3711388e-008f, -0.70710677f, 1.0f, 0.38268343f, -0.70710677f, -0.92387950f };
        XVECTOR aUnityTableImaginary[4] = { -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.38268343f, -0.70710677f, -0.92387950f, -0.0f, -0.70710677f, -1.0f, -0.70710677f, -0.0f, -0.92387950f, -0.70710677f, 0.38268343f };


        for (UINT32 uIndex=0; uIndex<uCount; ++uIndex) {
            ButterflyDIT4_4(pReal[uIndex*4],
                            pReal[uIndex*4 + 1],
                            pReal[uIndex*4 + 2],
                            pReal[uIndex*4 + 3],
                            pImaginary[uIndex*4],
                            pImaginary[uIndex*4 + 1],
                            pImaginary[uIndex*4 + 2],
                            pImaginary[uIndex*4 + 3],
                            aUnityTableReal,
                            aUnityTableImaginary,
                            1, TRUE);
        }
    }




















    inline void FFT (__inout_ecount((uLength*uCount)/4) XVECTOR* __restrict pReal, __inout_ecount((uLength*uCount)/4) XVECTOR* __restrict pImaginary, __in_ecount(uLength*uCount) const XVECTOR* __restrict pUnityTable, const UINT32 uLength, const UINT32 uCount=1)
    {
        DSPASSERT(pReal != NULL);
        DSPASSERT(pImaginary != NULL);
        DSPASSERT(pUnityTable != NULL);
        DSPASSERT((UINT_PTR)pReal % 16 == 0);
        DSPASSERT((UINT_PTR)pImaginary % 16 == 0);
        DSPASSERT((UINT_PTR)pUnityTable % 16 == 0);
        DSPASSERT(uLength > 16);
        DSPASSERT(ISPOWEROF2(uLength));
        DSPASSERT(ISPOWEROF2(uCount));

        const XVECTOR* __restrict pUnityTableReal      = pUnityTable;
        const XVECTOR* __restrict pUnityTableImaginary = pUnityTable + (uLength>>2);
        const UINT32 uTotal              = uCount * uLength;
        const UINT32 uTotal_vectors      = uTotal >> 2;
        const UINT32 uStage_vectors      = uLength >> 2;
        const UINT32 uStage_vectors_mask = uStage_vectors - 1;
        const UINT32 uStride        = uLength >> 4;
        const UINT32 uStrideMask    = uStride - 1;
        const UINT32 uStride2       = uStride * 2;
        const UINT32 uStride3       = uStride * 3;
        const UINT32 uStrideInvMask = ~uStrideMask;


        for (UINT32 uIndex=0; uIndex<(uTotal_vectors>>2); ++uIndex) {
            const UINT32 n = ((uIndex & uStrideInvMask) << 2) + (uIndex & uStrideMask);
            ButterflyDIT4_4(pReal[n],
                            pReal[n + uStride],
                            pReal[n + uStride2],
                            pReal[n + uStride3],
                            pImaginary[n ],
                            pImaginary[n + uStride],
                            pImaginary[n + uStride2],
                            pImaginary[n + uStride3],
                            pUnityTableReal      + (n & uStage_vectors_mask),
                            pUnityTableImaginary + (n & uStage_vectors_mask),
                            uStride, FALSE);
        }


        if (uLength > 16*4) {
            FFT(pReal, pImaginary, pUnityTable+(uLength>>1), uLength>>2, uCount*4);
        } else if (uLength == 16*4) {
            FFT16(pReal, pImaginary, uCount*4);
        } else if (uLength == 8*4) {
            FFT8(pReal, pImaginary, uCount*4);
        } else if (uLength == 4*4) {
            FFT4(pReal, pImaginary, uCount*4);
        }
    }



















inline void FFTInitializeUnityTable (__out_ecount(uLength) XVECTOR* __restrict pUnityTable, UINT32 uLength)
{
    DSPASSERT(pUnityTable != NULL);
    DSPASSERT(uLength > 16);
    DSPASSERT(ISPOWEROF2(uLength));

    FLOAT32* __restrict pfUnityTable = (FLOAT32* __restrict)pUnityTable;



    do {
        FLOAT32 flStep = 6.283185307f / uLength;
        uLength >>= 2;



        for (UINT32 i=0; i<4; ++i) {
            for (UINT32 j=0; j<uLength; ++j) {
                UINT32 uIndex = (i*uLength) + j;
                pfUnityTable[uIndex]             = cosf(FLOAT32(i)*FLOAT32(j)*flStep);
                pfUnityTable[uIndex + uLength*4] = -sinf(FLOAT32(i)*FLOAT32(j)*flStep);
            }
        }
        pfUnityTable += uLength*8;
    } while (uLength > 16);
}

















inline void FFTUnswizzle (__out_ecount((1<<uLog2Length)/4) XVECTOR* __restrict pOutput, __in_ecount((1<<uLog2Length)/4) const XVECTOR* __restrict pInput, const UINT32 uLog2Length)
{
    DSPASSERT(pOutput != NULL);
    DSPASSERT(pInput != NULL);
    DSPASSERT(uLog2Length >= 2);

    FLOAT32*       __restrict pfOutput = (FLOAT32* __restrict)pOutput;
    const FLOAT32* __restrict pfInput  = (const FLOAT32* __restrict)pInput;
    const UINT32 uLength = UINT32(1 << uLog2Length);


    if ((uLog2Length & 0x1) == 0) {

        for (UINT32 uIndex=0; uIndex<uLength; ++uIndex) {
            UINT32 n = uIndex;
            n = ( (n & 0xcccccccc) >> 2 )  | ( (n & 0x33333333) << 2 );
            n = ( (n & 0xf0f0f0f0) >> 4 )  | ( (n & 0x0f0f0f0f) << 4 );
            n = ( (n & 0xff00ff00) >> 8 )  | ( (n & 0x00ff00ff) << 8 );
            n = ( (n & 0xffff0000) >> 16 ) | ( (n & 0x0000ffff) << 16 );
            n >>= (32 - uLog2Length);
            pfOutput[n] = pfInput[uIndex];
        }
    } else {

        for (UINT32 uIndex=0; uIndex<uLength; ++uIndex) {
            UINT32 n = (uIndex>>3);
            n = ( (n & 0xcccccccc) >> 2 )  | ( (n & 0x33333333) << 2 );
            n = ( (n & 0xf0f0f0f0) >> 4 )  | ( (n & 0x0f0f0f0f) << 4 );
            n = ( (n & 0xff00ff00) >> 8 )  | ( (n & 0x00ff00ff) << 8 );
            n = ( (n & 0xffff0000) >> 16 ) | ( (n & 0x0000ffff) << 16 );
            n >>= (32 - (uLog2Length-3));
            n |= ((uIndex & 0x7) << (uLog2Length - 3));
            pfOutput[n] = pfInput[uIndex];
        }
    }
}















inline void FFTPolar (__out_ecount(uLength/4) XVECTOR* __restrict pOutput, __in_ecount(uLength/4) const XVECTOR* __restrict pInputReal, __in_ecount(uLength/4) const XVECTOR* __restrict pInputImaginary, const UINT32 uLength)
{
    DSPASSERT(pOutput != NULL);
    DSPASSERT(pInputReal != NULL);
    DSPASSERT(pInputImaginary != NULL);
    DSPASSERT(uLength >= 4);
    DSPASSERT(ISPOWEROF2(uLength));

    FLOAT32 flOneOverLength = 1.0f / uLength;



        XVECTOR vOneOverLength = _mm_set_ps1(flOneOverLength);

        for (UINT32 uIndex=0; uIndex<(uLength>>2); ++uIndex) {
            XVECTOR vReal      = _mm_mul_ps(pInputReal[uIndex], vOneOverLength);
            XVECTOR vImaginary = _mm_mul_ps(pInputImaginary[uIndex], vOneOverLength);
            XVECTOR vRR        = _mm_mul_ps(vReal, vReal);
            XVECTOR vII        = _mm_mul_ps(vImaginary, vImaginary);
            XVECTOR vRRplusII  = _mm_add_ps(vRR, vII);
            XVECTOR vTotal  = _mm_sqrt_ps(vRRplusII);
            pOutput[uIndex] = _mm_add_ps(vTotal, vTotal);
        }
}























inline void Deinterleave (__out_ecount((uChannelCount*uFrameCount)/4) XVECTOR* __restrict pOutput, __in_ecount((uChannelCount*uFrameCount)/4) const XVECTOR* __restrict pInput, const UINT32 uChannelCount, const UINT32 uFrameCount)
{
    DSPASSERT(pOutput != NULL);
    DSPASSERT(pInput != NULL);
    DSPASSERT(uChannelCount > 1);
    DSPASSERT(uFrameCount > 0);

    FLOAT32*       __restrict pfOutput = (FLOAT32* __restrict)pOutput;
    const FLOAT32* __restrict pfInput  = (const FLOAT32* __restrict)pInput;


    for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
        for (UINT32 uFrame=0; uFrame<uFrameCount; ++uFrame) {
            pfOutput[uChannel * uFrameCount + uFrame] = pfInput[uFrame * uChannelCount + uChannel];
        }
    }
}



















inline void Interleave (__out_ecount((uChannelCount*uFrameCount)/4) XVECTOR* __restrict pOutput, __in_ecount((uChannelCount*uFrameCount)/4) const XVECTOR* __restrict pInput, const UINT32 uChannelCount, const UINT32 uFrameCount)
{
    DSPASSERT(pOutput != NULL);
    DSPASSERT(pInput != NULL);
    DSPASSERT(uChannelCount > 1);
    DSPASSERT(uFrameCount > 0);

    FLOAT32*       __restrict pfOutput = (FLOAT32* __restrict)pOutput;
    const FLOAT32* __restrict pfInput  = (const FLOAT32* __restrict)pInput;


    for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
        for (UINT32 uFrame=0; uFrame<uFrameCount; ++uFrame) {
            pfOutput[uFrame * uChannelCount + uChannel] = pfInput[uChannel * uFrameCount + uFrame];
        }
    }
}






















inline void FFTInterleaved (__inout_ecount((1<<uLog2Length*uChannelCount)/4) XVECTOR* __restrict pReal, __out_ecount((1<<uLog2Length*uChannelCount)/4) XVECTOR* __restrict pImaginary, __in_ecount(1<<uLog2Length) const XVECTOR* __restrict pUnityTable, const UINT32 uChannelCount, const UINT32 uLog2Length)
{
    DSPASSERT(pReal != NULL);
    DSPASSERT(pImaginary != NULL);
    DSPASSERT(pUnityTable != NULL);
    DSPASSERT((UINT_PTR)pReal % 16 == 0);
    DSPASSERT((UINT_PTR)pImaginary % 16 == 0);
    DSPASSERT((UINT_PTR)pUnityTable % 16 == 0);
    DSPASSERT(uChannelCount > 0 && uChannelCount <= 6);
    DSPASSERT(uLog2Length >= 2 && uLog2Length <= 9);

    XVECTOR vRealTemp[768];
    XVECTOR vImaginaryTemp[768];
    const UINT32 uLength = UINT32(1 << uLog2Length);


    if (uChannelCount > 1) {
        Deinterleave(vRealTemp, pReal, uChannelCount, uLength);
    } else {
        CopyMemory(vRealTemp, pReal, (uLength>>2)*sizeof(XVECTOR));
    }
        for (UINT32 u=0; u<uChannelCount*(uLength>>2); u++) {
            vImaginaryTemp[u] = _mm_setzero_ps();
        }

    if (uLength > 16) {
        for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
            FFT(&vRealTemp[uChannel*(uLength>>2)], &vImaginaryTemp[uChannel*(uLength>>2)], pUnityTable, uLength);
        }
    } else if (uLength == 16) {
        for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
            FFT16(&vRealTemp[uChannel*(uLength>>2)], &vImaginaryTemp[uChannel*(uLength>>2)]);
        }
    } else if (uLength == 8) {
        for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
            FFT8(&vRealTemp[uChannel*(uLength>>2)], &vImaginaryTemp[uChannel*(uLength>>2)]);
        }
    } else if (uLength == 4) {
        for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
            FFT4(&vRealTemp[uChannel*(uLength>>2)], &vImaginaryTemp[uChannel*(uLength>>2)]);
        }
    }

    for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
        FFTUnswizzle(&pReal[uChannel*(uLength>>2)], &vRealTemp[uChannel*(uLength>>2)], uLog2Length);
        FFTUnswizzle(&pImaginary[uChannel*(uLength>>2)], &vImaginaryTemp[uChannel*(uLength>>2)], uLog2Length);
    }
}

















inline void IFFTDeinterleaved (__inout_ecount((1<<uLog2Length*uChannelCount)/4) XVECTOR* __restrict pReal, __out_ecount((1<<uLog2Length*uChannelCount)/4) XVECTOR* __restrict pImaginary, __in_ecount(1<<uLog2Length) const XVECTOR* __restrict pUnityTable, const UINT32 uChannelCount, const UINT32 uLog2Length)
{
    DSPASSERT(pReal != NULL);
    DSPASSERT(pImaginary != NULL);
    DSPASSERT(pUnityTable != NULL);
    DSPASSERT((UINT_PTR)pReal % 16 == 0);
    DSPASSERT((UINT_PTR)pImaginary % 16 == 0);
    DSPASSERT((UINT_PTR)pUnityTable % 16 == 0);
    DSPASSERT(uChannelCount > 0 && uChannelCount <= 6);
    DSPASSERT(uLog2Length >= 2 && uLog2Length <= 9);

    XVECTOR vRealTemp[768];
    XVECTOR vImaginaryTemp[768];
    const UINT32 uLength = UINT32(1 << uLog2Length);


        const XVECTOR vRnp = _mm_set_ps1(1.0f/uLength);
        const XVECTOR vRnm = _mm_set_ps1(-1.0f/uLength);
        for (UINT32 u=0; u<uChannelCount*(uLength>>2); u++) {
            vRealTemp[u]      = _mm_mul_ps(pReal[u], vRnp);
            vImaginaryTemp[u] = _mm_mul_ps(pImaginary[u], vRnm);
        }

    if (uLength > 16) {
        for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
            FFT(&vRealTemp[uChannel*(uLength>>2)], &vImaginaryTemp[uChannel*(uLength>>2)], pUnityTable, uLength);
        }
    } else if (uLength == 16) {
        for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
            FFT16(&vRealTemp[uChannel*(uLength>>2)], &vImaginaryTemp[uChannel*(uLength>>2)]);
        }
    } else if (uLength == 8) {
        for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
            FFT8(&vRealTemp[uChannel*(uLength>>2)], &vImaginaryTemp[uChannel*(uLength>>2)]);
        }
    } else if (uLength == 4) {
        for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
            FFT4(&vRealTemp[uChannel*(uLength>>2)], &vImaginaryTemp[uChannel*(uLength>>2)]);
        }
    }

    for (UINT32 uChannel=0; uChannel<uChannelCount; ++uChannel) {
        FFTUnswizzle(&vImaginaryTemp[uChannel*(uLength>>2)], &vRealTemp[uChannel*(uLength>>2)], uLog2Length);
    }
    if (uChannelCount > 1) {
        Interleave(pReal, vImaginaryTemp, uChannelCount, uLength);
    } else {
        CopyMemory(pReal, vImaginaryTemp, (uLength>>2)*sizeof(XVECTOR));
    }
}


#pragma warning(pop)
};


