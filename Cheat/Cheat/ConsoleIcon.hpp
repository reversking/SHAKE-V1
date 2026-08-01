#pragma once
#include <windows.h>
#include <cstring>
#include <gdiplus.h>
#include <objidl.h>
#include "Brand.hpp"
REVERS_STAMP(ConsoleIcon_hpp)
#include "../../Libraries/imgui/include/ImGui/TransparentLogo.hpp"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "ole32.lib")

namespace ConsoleIcon
{
    inline HICON BitmapToIcon(Gdiplus::Bitmap* source, int size, float zoom = 1.45f)
    {
        Gdiplus::Bitmap scaled(size, size, PixelFormat32bppARGB);
        Gdiplus::Graphics graphics(&scaled);
        graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
        graphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
        graphics.Clear(Gdiplus::Color(0, 0, 0, 0));

        const int drawSize = static_cast<int>(size * zoom);
        const int offset = (size - drawSize) / 2;
        graphics.DrawImage(source, offset, offset, drawSize, drawSize);

        Gdiplus::BitmapData data{};
        Gdiplus::Rect rect(0, 0, size, size);
        if (scaled.LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data) != Gdiplus::Ok)
            return nullptr;

        BITMAPV5HEADER header{};
        header.bV5Size = sizeof(BITMAPV5HEADER);
        header.bV5Width = size;
        header.bV5Height = -size;
        header.bV5Planes = 1;
        header.bV5BitCount = 32;
        header.bV5Compression = BI_BITFIELDS;
        header.bV5RedMask = 0x00FF0000;
        header.bV5GreenMask = 0x0000FF00;
        header.bV5BlueMask = 0x000000FF;
        header.bV5AlphaMask = 0xFF000000;

        void* bits = nullptr;
        HDC screen = GetDC(nullptr);
        HBITMAP colorBitmap = CreateDIBSection(screen, reinterpret_cast<BITMAPINFO*>(&header), DIB_RGB_COLORS, &bits, nullptr, 0);
        ReleaseDC(nullptr, screen);

        if (!colorBitmap || !bits) {
            scaled.UnlockBits(&data);
            return nullptr;
        }

        const size_t copySize = static_cast<size_t>(size) * static_cast<size_t>(size) * 4;
        memcpy(bits, data.Scan0, copySize);
        scaled.UnlockBits(&data);

        HBITMAP maskBitmap = CreateBitmap(size, size, 1, 1, nullptr);

        ICONINFO iconInfo{};
        iconInfo.fIcon = TRUE;
        iconInfo.hbmColor = colorBitmap;
        iconInfo.hbmMask = maskBitmap;

        HICON icon = CreateIconIndirect(&iconInfo);
        DeleteObject(colorBitmap);
        DeleteObject(maskBitmap);
        return icon;
    }

    inline Gdiplus::Bitmap* LoadTransparentLogo()
    {
        HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE, sizeof(TransparentLogo));
        if (!memory)
            return nullptr;

        void* buffer = GlobalLock(memory);
        if (!buffer) {
            GlobalFree(memory);
            return nullptr;
        }

        memcpy(buffer, TransparentLogo, sizeof(TransparentLogo));
        GlobalUnlock(memory);

        IStream* stream = nullptr;
        if (CreateStreamOnHGlobal(memory, TRUE, &stream) != S_OK)
            return nullptr;

        auto* bitmap = Gdiplus::Bitmap::FromStream(stream);
        stream->Release();

        if (!bitmap || bitmap->GetLastStatus() != Gdiplus::Ok) {
            delete bitmap;
            return nullptr;
        }

        return bitmap;
    }

    inline void ApplyToConsole()
    {
        static bool gdiplusStarted = false;
        static ULONG_PTR gdiplusToken = 0;
        if (!gdiplusStarted) {
            Gdiplus::GdiplusStartupInput input;
            if (Gdiplus::GdiplusStartup(&gdiplusToken, &input, nullptr) != Gdiplus::Ok)
                return;
            gdiplusStarted = true;
        }

        HWND console = GetConsoleWindow();
        if (!console)
            return;

        Gdiplus::Bitmap* logo = LoadTransparentLogo();
        if (!logo)
            return;

        HICON smallIcon = BitmapToIcon(logo, 24, 1.55f);
        HICON bigIcon = BitmapToIcon(logo, 48, 1.45f);
        if (smallIcon)
            SendMessage(console, WM_SETICON, ICON_SMALL, (LPARAM)smallIcon);
        if (bigIcon)
            SendMessage(console, WM_SETICON, ICON_BIG, (LPARAM)bigIcon);

        delete logo;
    }
}
