#pragma once

namespace globals
{
    namespace brand
    {
        inline constexpr const char* signature = "Shake V1 Revers";
        inline constexpr const char* name = "Shake V1 Revers";
        inline constexpr const char* version = "Revers Edition";
        inline constexpr const char* author = "Revers";
        inline constexpr const char* createdBy = "Created By Revers";
        inline constexpr const char* fullTitle = "Shake V1 | Created By Revers";
        inline constexpr const char* consoleTitle = "Shake V1 Revers | Created By Revers";
        inline constexpr const char* welcome = "Welcome Back Revers";
        inline constexpr const char* logPrefix = "Revers Shake V1";
        inline constexpr const char* watermark = "Shake V1 Revers";
        inline constexpr const char* activeHeader = "Shake V1 Revers";
        inline constexpr const char* codeSignature = "Revers - Shake V1";
        inline constexpr const char* codeStamp = "Revers";
        inline constexpr const char* footer = "Created By Revers";
        inline constexpr const char* bannerLine = "Revers - Shake V1";
    }
}

namespace revers_brand
{
    inline constexpr const char* kSignature = globals::brand::signature;
    inline constexpr const char* kAuthor = globals::brand::author;
    inline constexpr const char* kCreatedBy = globals::brand::createdBy;
    inline constexpr const char* kFullTitle = globals::brand::fullTitle;
    inline constexpr const char* kWelcome = globals::brand::welcome;
    inline constexpr const char* kWatermark = globals::brand::watermark;
    inline constexpr const char* kStamp = globals::brand::codeStamp;
    inline constexpr const char* kCodeSignature = globals::brand::codeSignature;
}

#define REVERS_STAMP(tag) \
    static const char revers_shake_v1_stamp_##tag[] = "Revers - Shake V1";

static const char revers_shake_v1_stamp_Brand_hpp[] = "Revers - Shake V1";
