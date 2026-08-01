









#ifndef __D3DX10ASYNC_H__
#define __D3DX10ASYNC_H__

#include "d3dx10.h"

#ifdef __cplusplus
extern "C" {
#endif
























































HRESULT WINAPI D3DX10CompileFromFileA(LPCSTR pSrcFile,CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
        LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2, ID3DX10ThreadPump* pPump, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);

HRESULT WINAPI D3DX10CompileFromFileW(LPCWSTR pSrcFile, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
        LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2, ID3DX10ThreadPump* pPump, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);

#ifdef UNICODE
#define D3DX10CompileFromFile D3DX10CompileFromFileW
#else
#define D3DX10CompileFromFile D3DX10CompileFromFileA
#endif

HRESULT WINAPI D3DX10CompileFromResourceA(HMODULE hSrcModule, LPCSTR pSrcResource, LPCSTR pSrcFileName, CONST D3D10_SHADER_MACRO* pDefines,
    LPD3D10INCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2, ID3DX10ThreadPump* pPump, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);

HRESULT WINAPI D3DX10CompileFromResourceW(HMODULE hSrcModule, LPCWSTR pSrcResource, LPCWSTR pSrcFileName, CONST D3D10_SHADER_MACRO* pDefines,
    LPD3D10INCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2, ID3DX10ThreadPump* pPump, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);

#ifdef UNICODE
#define D3DX10CompileFromResource D3DX10CompileFromResourceW
#else
#define D3DX10CompileFromResource D3DX10CompileFromResourceA
#endif

HRESULT WINAPI D3DX10CompileFromMemory(LPCSTR pSrcData, SIZE_T SrcDataLen, LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
    LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2, ID3DX10ThreadPump* pPump, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);




























































HRESULT WINAPI D3DX10CreateEffectFromFileA(LPCSTR pFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice,
    ID3D10EffectPool *pEffectPool, ID3DX10ThreadPump* pPump, ID3D10Effect **ppEffect, ID3D10Blob **ppErrors, HRESULT* pHResult);

HRESULT WINAPI D3DX10CreateEffectFromFileW(LPCWSTR pFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice,
    ID3D10EffectPool *pEffectPool, ID3DX10ThreadPump* pPump, ID3D10Effect **ppEffect, ID3D10Blob **ppErrors, HRESULT* pHResult);

HRESULT WINAPI D3DX10CreateEffectFromMemory(LPCVOID pData, SIZE_T DataLength, LPCSTR pSrcFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice,
    ID3D10EffectPool *pEffectPool, ID3DX10ThreadPump* pPump, ID3D10Effect **ppEffect, ID3D10Blob **ppErrors, HRESULT* pHResult);

HRESULT WINAPI D3DX10CreateEffectFromResourceA(HMODULE hModule, LPCSTR pResourceName, LPCSTR pSrcFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice,
    ID3D10EffectPool *pEffectPool, ID3DX10ThreadPump* pPump, ID3D10Effect **ppEffect, ID3D10Blob **ppErrors, HRESULT* pHResult);

HRESULT WINAPI D3DX10CreateEffectFromResourceW(HMODULE hModule, LPCWSTR pResourceName, LPCWSTR pSrcFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice,
    ID3D10EffectPool *pEffectPool, ID3DX10ThreadPump* pPump, ID3D10Effect **ppEffect, ID3D10Blob **ppErrors, HRESULT* pHResult);


#ifdef UNICODE
#define D3DX10CreateEffectFromFile          D3DX10CreateEffectFromFileW
#define D3DX10CreateEffectFromResource      D3DX10CreateEffectFromResourceW
#else
#define D3DX10CreateEffectFromFile          D3DX10CreateEffectFromFileA
#define D3DX10CreateEffectFromResource      D3DX10CreateEffectFromResourceA
#endif

HRESULT WINAPI D3DX10CreateEffectPoolFromFileA(LPCSTR pFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice, ID3DX10ThreadPump* pPump,
    ID3D10EffectPool **ppEffectPool, ID3D10Blob **ppErrors, HRESULT* pHResult);

HRESULT WINAPI D3DX10CreateEffectPoolFromFileW(LPCWSTR pFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice, ID3DX10ThreadPump* pPump,
    ID3D10EffectPool **ppEffectPool, ID3D10Blob **ppErrors, HRESULT* pHResult);

HRESULT WINAPI D3DX10CreateEffectPoolFromMemory(LPCVOID pData, SIZE_T DataLength, LPCSTR pSrcFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice,
    ID3DX10ThreadPump* pPump, ID3D10EffectPool **ppEffectPool, ID3D10Blob **ppErrors, HRESULT* pHResult);

HRESULT WINAPI D3DX10CreateEffectPoolFromResourceA(HMODULE hModule, LPCSTR pResourceName, LPCSTR pSrcFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice,
    ID3DX10ThreadPump* pPump, ID3D10EffectPool **ppEffectPool, ID3D10Blob **ppErrors, HRESULT* pHResult);

HRESULT WINAPI D3DX10CreateEffectPoolFromResourceW(HMODULE hModule, LPCWSTR pResourceName, LPCWSTR pSrcFileName, CONST D3D10_SHADER_MACRO *pDefines,
    ID3D10Include *pInclude, LPCSTR pProfile, UINT HLSLFlags, UINT FXFlags, ID3D10Device *pDevice,
    ID3DX10ThreadPump* pPump, ID3D10EffectPool **ppEffectPool, ID3D10Blob **ppErrors, HRESULT* pHResult);

#ifdef UNICODE
#define D3DX10CreateEffectPoolFromFile      D3DX10CreateEffectPoolFromFileW
#define D3DX10CreateEffectPoolFromResource  D3DX10CreateEffectPoolFromResourceW
#else
#define D3DX10CreateEffectPoolFromFile      D3DX10CreateEffectPoolFromFileA
#define D3DX10CreateEffectPoolFromResource  D3DX10CreateEffectPoolFromResourceA
#endif

HRESULT WINAPI D3DX10PreprocessShaderFromFileA(LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines,
    LPD3D10INCLUDE pInclude, ID3DX10ThreadPump *pPump, ID3D10Blob** ppShaderText, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);

HRESULT WINAPI D3DX10PreprocessShaderFromFileW(LPCWSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines,
    LPD3D10INCLUDE pInclude, ID3DX10ThreadPump *pPump, ID3D10Blob** ppShaderText, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);

HRESULT WINAPI D3DX10PreprocessShaderFromMemory(LPCSTR pSrcData, SIZE_T SrcDataSize, LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines,
    LPD3D10INCLUDE pInclude, ID3DX10ThreadPump *pPump, ID3D10Blob** ppShaderText, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);

HRESULT WINAPI D3DX10PreprocessShaderFromResourceA(HMODULE hModule, LPCSTR pResourceName, LPCSTR pSrcFileName, CONST D3D10_SHADER_MACRO* pDefines,
    LPD3D10INCLUDE pInclude, ID3DX10ThreadPump *pPump, ID3D10Blob** ppShaderText, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);

HRESULT WINAPI D3DX10PreprocessShaderFromResourceW(HMODULE hModule, LPCWSTR pResourceName, LPCWSTR pSrcFileName, CONST D3D10_SHADER_MACRO* pDefines,
    LPD3D10INCLUDE pInclude, ID3DX10ThreadPump *pPump, ID3D10Blob** ppShaderText, ID3D10Blob** ppErrorMsgs, HRESULT* pHResult);

#ifdef UNICODE
#define D3DX10PreprocessShaderFromFile      D3DX10PreprocessShaderFromFileW
#define D3DX10PreprocessShaderFromResource  D3DX10PreprocessShaderFromResourceW
#else
#define D3DX10PreprocessShaderFromFile      D3DX10PreprocessShaderFromFileA
#define D3DX10PreprocessShaderFromResource  D3DX10PreprocessShaderFromResourceA
#endif





HRESULT WINAPI D3DX10CreateAsyncCompilerProcessor(LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
        LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2,
        ID3D10Blob **ppCompiledShader, ID3D10Blob **ppErrorBuffer, ID3DX10DataProcessor **ppProcessor);

HRESULT WINAPI D3DX10CreateAsyncEffectCreateProcessor(LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
        LPCSTR pProfile, UINT Flags, UINT FXFlags, ID3D10Device *pDevice,
        ID3D10EffectPool *pPool, ID3D10Blob **ppErrorBuffer, ID3DX10DataProcessor **ppProcessor);

HRESULT WINAPI D3DX10CreateAsyncEffectPoolCreateProcessor(LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
        LPCSTR pProfile, UINT Flags, UINT FXFlags, ID3D10Device *pDevice,
        ID3D10Blob **ppErrorBuffer, ID3DX10DataProcessor **ppProcessor);

HRESULT WINAPI D3DX10CreateAsyncShaderPreprocessProcessor(LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
        ID3D10Blob** ppShaderText, ID3D10Blob **ppErrorBuffer, ID3DX10DataProcessor **ppProcessor);







HRESULT WINAPI D3DX10CreateAsyncFileLoaderW(LPCWSTR pFileName, ID3DX10DataLoader **ppDataLoader);
HRESULT WINAPI D3DX10CreateAsyncFileLoaderA(LPCSTR pFileName, ID3DX10DataLoader **ppDataLoader);
HRESULT WINAPI D3DX10CreateAsyncMemoryLoader(LPCVOID pData, SIZE_T cbData, ID3DX10DataLoader **ppDataLoader);
HRESULT WINAPI D3DX10CreateAsyncResourceLoaderW(HMODULE hSrcModule, LPCWSTR pSrcResource, ID3DX10DataLoader **ppDataLoader);
HRESULT WINAPI D3DX10CreateAsyncResourceLoaderA(HMODULE hSrcModule, LPCSTR pSrcResource, ID3DX10DataLoader **ppDataLoader);

#ifdef UNICODE
#define D3DX10CreateAsyncFileLoader D3DX10CreateAsyncFileLoaderW
#define D3DX10CreateAsyncResourceLoader D3DX10CreateAsyncResourceLoaderW
#else
#define D3DX10CreateAsyncFileLoader D3DX10CreateAsyncFileLoaderA
#define D3DX10CreateAsyncResourceLoader D3DX10CreateAsyncResourceLoaderA
#endif

HRESULT WINAPI D3DX10CreateAsyncTextureProcessor(ID3D10Device *pDevice, D3DX10_IMAGE_LOAD_INFO *pLoadInfo, ID3DX10DataProcessor **ppDataProcessor);
HRESULT WINAPI D3DX10CreateAsyncTextureInfoProcessor(D3DX10_IMAGE_INFO *pImageInfo, ID3DX10DataProcessor **ppDataProcessor);
HRESULT WINAPI D3DX10CreateAsyncShaderResourceViewProcessor(ID3D10Device *pDevice, D3DX10_IMAGE_LOAD_INFO *pLoadInfo, ID3DX10DataProcessor **ppDataProcessor);

#ifdef __cplusplus
}
#endif

#endif


