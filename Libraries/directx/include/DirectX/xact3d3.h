














#ifndef __XACT3D3_H__
#define __XACT3D3_H__


    #include <x3daudio.h>
    #include <xact3.h>

    #pragma warning(push)
    #pragma warning(disable: 4701)




















    #define LEFT_AZIMUTH                    (3*X3DAUDIO_PI/2)
    #define RIGHT_AZIMUTH                   (X3DAUDIO_PI/2)
    #define FRONT_LEFT_AZIMUTH              (7*X3DAUDIO_PI/4)
    #define FRONT_RIGHT_AZIMUTH             (X3DAUDIO_PI/4)
    #define FRONT_CENTER_AZIMUTH            0.0f
    #define LOW_FREQUENCY_AZIMUTH           X3DAUDIO_2PI
    #define BACK_LEFT_AZIMUTH               (5*X3DAUDIO_PI/4)
    #define BACK_RIGHT_AZIMUTH              (3*X3DAUDIO_PI/4)
    #define BACK_CENTER_AZIMUTH             X3DAUDIO_PI
    #define FRONT_LEFT_OF_CENTER_AZIMUTH    (15*X3DAUDIO_PI/8)
    #define FRONT_RIGHT_OF_CENTER_AZIMUTH   (X3DAUDIO_PI/8)




    static const float aStereoLayout[] =
    {
        LEFT_AZIMUTH,
        RIGHT_AZIMUTH
    };
    static const float a2Point1Layout[] =
    {
        LEFT_AZIMUTH,
        RIGHT_AZIMUTH,
        LOW_FREQUENCY_AZIMUTH
    };
    static const float aQuadLayout[] =
    {
        FRONT_LEFT_AZIMUTH,
        FRONT_RIGHT_AZIMUTH,
        BACK_LEFT_AZIMUTH,
        BACK_RIGHT_AZIMUTH
    };
    static const float a4Point1Layout[] =
    {
        FRONT_LEFT_AZIMUTH,
        FRONT_RIGHT_AZIMUTH,
        LOW_FREQUENCY_AZIMUTH,
        BACK_LEFT_AZIMUTH,
        BACK_RIGHT_AZIMUTH
    };
    static const float a5Point1Layout[] =
    {
        FRONT_LEFT_AZIMUTH,
        FRONT_RIGHT_AZIMUTH,
        FRONT_CENTER_AZIMUTH,
        LOW_FREQUENCY_AZIMUTH,
        BACK_LEFT_AZIMUTH,
        BACK_RIGHT_AZIMUTH
    };
    static const float a7Point1Layout[] =
    {
        FRONT_LEFT_AZIMUTH,
        FRONT_RIGHT_AZIMUTH,
        FRONT_CENTER_AZIMUTH,
        LOW_FREQUENCY_AZIMUTH,
        BACK_LEFT_AZIMUTH,
        BACK_RIGHT_AZIMUTH,
        LEFT_AZIMUTH,
        RIGHT_AZIMUTH
    };



















    EXTERN_C HRESULT inline XACT3DInitialize (__in IXACT3Engine* pEngine, __in X3DAUDIO_HANDLE X3DInstance)
    {
        HRESULT hr = S_OK;
        if (pEngine == NULL) {
            hr = E_POINTER;
        }

        XACTVARIABLEVALUE nSpeedOfSound;
        if (SUCCEEDED(hr)) {
            XACTVARIABLEINDEX xactSpeedOfSoundID = pEngine->GetGlobalVariableIndex("SpeedOfSound");
            hr = pEngine->GetGlobalVariable(xactSpeedOfSoundID, &nSpeedOfSound);
        }

        if (SUCCEEDED(hr)) {
            WAVEFORMATEXTENSIBLE wfxFinalMixFormat;
            hr = pEngine->GetFinalMixFormat(&wfxFinalMixFormat);
            if (SUCCEEDED(hr)) {
                X3DAudioInitialize(wfxFinalMixFormat.dwChannelMask, nSpeedOfSound, X3DInstance);
            }
        }
        return hr;
    }















































    EXTERN_C HRESULT inline XACT3DCalculate (__in X3DAUDIO_HANDLE X3DInstance, __in const X3DAUDIO_LISTENER* pListener, __inout X3DAUDIO_EMITTER* pEmitter, __inout X3DAUDIO_DSP_SETTINGS* pDSPSettings)
    {
        HRESULT hr = S_OK;
        if (pListener == NULL || pEmitter == NULL || pDSPSettings == NULL) {
            hr = E_POINTER;
        }

        if (SUCCEEDED(hr)) {
            if (pEmitter->ChannelCount > 1 && pEmitter->pChannelAzimuths == NULL) {
                pEmitter->ChannelRadius = 1.0f;

                switch (pEmitter->ChannelCount) {
                    case 2: pEmitter->pChannelAzimuths = (float*)&aStereoLayout[0]; break;
                    case 3: pEmitter->pChannelAzimuths = (float*)&a2Point1Layout[0]; break;
                    case 4: pEmitter->pChannelAzimuths = (float*)&aQuadLayout[0]; break;
                    case 5: pEmitter->pChannelAzimuths = (float*)&a4Point1Layout[0]; break;
                    case 6: pEmitter->pChannelAzimuths = (float*)&a5Point1Layout[0]; break;
                    case 8: pEmitter->pChannelAzimuths = (float*)&a7Point1Layout[0]; break;
                    default: hr = E_FAIL; break;
                }
            }
        }

        if (SUCCEEDED(hr)) {
            static X3DAUDIO_DISTANCE_CURVE_POINT DefaultCurvePoints[2] = { 0.0f, 1.0f, 1.0f, 1.0f };
            static X3DAUDIO_DISTANCE_CURVE       DefaultCurve          = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&DefaultCurvePoints[0], 2 };
            if (pEmitter->pVolumeCurve == NULL) {
                pEmitter->pVolumeCurve = &DefaultCurve;
            }
            if (pEmitter->pLFECurve == NULL) {
                pEmitter->pLFECurve = &DefaultCurve;
            }

            X3DAudioCalculate(X3DInstance, pListener, pEmitter, (X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_EMITTER_ANGLE), pDSPSettings);
        }

        return hr;
    }













    EXTERN_C HRESULT inline XACT3DApply (__in const X3DAUDIO_DSP_SETTINGS* pDSPSettings, __in IXACT3Cue* pCue)
    {
        HRESULT hr = S_OK;
        if (pDSPSettings == NULL || pCue == NULL) {
            hr = E_POINTER;
        }

        if (SUCCEEDED(hr)) {
            hr = pCue->SetMatrixCoefficients(pDSPSettings->SrcChannelCount, pDSPSettings->DstChannelCount, pDSPSettings->pMatrixCoefficients);
        }
        if (SUCCEEDED(hr)) {
            XACTVARIABLEINDEX xactDistanceID = pCue->GetVariableIndex("Distance");
            hr = pCue->SetVariable(xactDistanceID, pDSPSettings->EmitterToListenerDistance);
        }
        if (SUCCEEDED(hr)) {
            XACTVARIABLEINDEX xactDopplerID = pCue->GetVariableIndex("DopplerPitchScalar");
            hr = pCue->SetVariable(xactDopplerID, pDSPSettings->DopplerFactor);
        }
        if (SUCCEEDED(hr)) {
            XACTVARIABLEINDEX xactOrientationID = pCue->GetVariableIndex("OrientationAngle");
            hr = pCue->SetVariable(xactOrientationID, pDSPSettings->EmitterToListenerAngle * (180.0f / X3DAUDIO_PI));
        }

        return hr;
    }


    #pragma warning(pop)

#endif

