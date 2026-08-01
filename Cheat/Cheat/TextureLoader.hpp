#pragma once

#include <windows.h>
#include <dxgitype.h>
#include <dxgi.h>
#include <d3d11.h>
#include <wincodec.h>
#include <vector>
#include "Brand.hpp"
REVERS_STAMP(TextureLoader_hpp)

#pragma comment(lib, "windowscodecs.lib")

inline HRESULT CreateShaderResourceViewFromMemory(
    ID3D11Device* device,
    const void* data,
    size_t dataSize,
    ID3D11ShaderResourceView** outSrv,
    UINT* outWidth = nullptr,
    UINT* outHeight = nullptr)
{
    if (!device || !data || !dataSize || !outSrv)
        return E_INVALIDARG;

    *outSrv = nullptr;
    if (outWidth) *outWidth = 0;
    if (outHeight) *outHeight = 0;

    IWICImagingFactory* factory = nullptr;
    IWICStream* stream = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;
    ID3D11Texture2D* texture = nullptr;

    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&factory));

    if (SUCCEEDED(hr))
        hr = factory->CreateStream(&stream);

    if (SUCCEEDED(hr))
        hr = stream->InitializeFromMemory(reinterpret_cast<BYTE*>(const_cast<void*>(data)), static_cast<DWORD>(dataSize));

    if (SUCCEEDED(hr)) {
        hr = factory->CreateDecoderFromStream(
            stream,
            nullptr,
            WICDecodeMetadataCacheOnLoad,
            &decoder);
    }

    if (SUCCEEDED(hr))
        hr = decoder->GetFrame(0, &frame);

    if (SUCCEEDED(hr))
        hr = factory->CreateFormatConverter(&converter);

    if (SUCCEEDED(hr)) {
        hr = converter->Initialize(
            frame,
            GUID_WICPixelFormat32bppRGBA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeCustom);
    }

    UINT width = 0;
    UINT height = 0;
    if (SUCCEEDED(hr))
        hr = converter->GetSize(&width, &height);

    if (FAILED(hr) || width == 0 || height == 0) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (stream) stream->Release();
        if (factory) factory->Release();
        return FAILED(hr) ? hr : E_FAIL;
    }

    std::vector<BYTE> pixels(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
    if (SUCCEEDED(hr)) {
        hr = converter->CopyPixels(
            nullptr,
            width * 4,
            static_cast<UINT>(pixels.size()),
            pixels.data());
    }

    if (converter) converter->Release();
    if (frame) frame->Release();
    if (decoder) decoder->Release();
    if (stream) stream->Release();
    if (factory) factory->Release();

    if (FAILED(hr))
        return hr;

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA subresource = {};
    subresource.pSysMem = pixels.data();
    subresource.SysMemPitch = width * 4;

    hr = device->CreateTexture2D(&desc, &subresource, &texture);
    if (SUCCEEDED(hr))
        hr = device->CreateShaderResourceView(texture, nullptr, outSrv);

    if (texture)
        texture->Release();

    if (SUCCEEDED(hr)) {
        if (outWidth) *outWidth = width;
        if (outHeight) *outHeight = height;
    }

    return hr;
}
