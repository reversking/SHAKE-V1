








#ifndef DWRITE_H_INCLUDED
#define DWRITE_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef DWRITE_NO_WINDOWS_H

#include <specstrings.h>
#include <unknwn.h>

#endif

#include <dcommon.h>

#ifndef DWRITE_DECLARE_INTERFACE
#define DWRITE_DECLARE_INTERFACE(iid) DECLSPEC_UUID(iid) DECLSPEC_NOVTABLE
#endif

#ifndef DWRITE_EXPORT
#define DWRITE_EXPORT __declspec(dllimport) WINAPI
#endif






enum DWRITE_FONT_FILE_TYPE
{



    DWRITE_FONT_FILE_TYPE_UNKNOWN,




    DWRITE_FONT_FILE_TYPE_CFF,




    DWRITE_FONT_FILE_TYPE_TRUETYPE,




    DWRITE_FONT_FILE_TYPE_TRUETYPE_COLLECTION,




    DWRITE_FONT_FILE_TYPE_TYPE1_PFM,




    DWRITE_FONT_FILE_TYPE_TYPE1_PFB,




    DWRITE_FONT_FILE_TYPE_VECTOR,




    DWRITE_FONT_FILE_TYPE_BITMAP
};






enum DWRITE_FONT_FACE_TYPE
{



    DWRITE_FONT_FACE_TYPE_CFF,




    DWRITE_FONT_FACE_TYPE_TRUETYPE,




    DWRITE_FONT_FACE_TYPE_TRUETYPE_COLLECTION,




    DWRITE_FONT_FACE_TYPE_TYPE1,




    DWRITE_FONT_FACE_TYPE_VECTOR,




    DWRITE_FONT_FACE_TYPE_BITMAP,




    DWRITE_FONT_FACE_TYPE_UNKNOWN
};





enum DWRITE_FONT_SIMULATIONS
{



    DWRITE_FONT_SIMULATIONS_NONE    = 0x0000,




    DWRITE_FONT_SIMULATIONS_BOLD    = 0x0001,




    DWRITE_FONT_SIMULATIONS_OBLIQUE = 0x0002
};

#ifdef DEFINE_ENUM_FLAG_OPERATORS
DEFINE_ENUM_FLAG_OPERATORS(DWRITE_FONT_SIMULATIONS);
#endif





enum DWRITE_FONT_WEIGHT
{



    DWRITE_FONT_WEIGHT_THIN = 100,




    DWRITE_FONT_WEIGHT_EXTRA_LIGHT = 200,




    DWRITE_FONT_WEIGHT_ULTRA_LIGHT = 200,




    DWRITE_FONT_WEIGHT_LIGHT = 300,




    DWRITE_FONT_WEIGHT_NORMAL = 400,




    DWRITE_FONT_WEIGHT_REGULAR = 400,




    DWRITE_FONT_WEIGHT_MEDIUM = 500,




    DWRITE_FONT_WEIGHT_DEMI_BOLD = 600,




    DWRITE_FONT_WEIGHT_SEMI_BOLD = 600,




    DWRITE_FONT_WEIGHT_BOLD = 700,




    DWRITE_FONT_WEIGHT_EXTRA_BOLD = 800,




    DWRITE_FONT_WEIGHT_ULTRA_BOLD = 800,




    DWRITE_FONT_WEIGHT_BLACK = 900,




    DWRITE_FONT_WEIGHT_HEAVY = 900,




    DWRITE_FONT_WEIGHT_EXTRA_BLACK = 950,




    DWRITE_FONT_WEIGHT_ULTRA_BLACK = 950
};






enum DWRITE_FONT_STRETCH
{



    DWRITE_FONT_STRETCH_UNDEFINED = 0,




    DWRITE_FONT_STRETCH_ULTRA_CONDENSED = 1,




    DWRITE_FONT_STRETCH_EXTRA_CONDENSED = 2,




    DWRITE_FONT_STRETCH_CONDENSED = 3,




    DWRITE_FONT_STRETCH_SEMI_CONDENSED = 4,




    DWRITE_FONT_STRETCH_NORMAL = 5,




    DWRITE_FONT_STRETCH_MEDIUM = 5,




    DWRITE_FONT_STRETCH_SEMI_EXPANDED = 6,




    DWRITE_FONT_STRETCH_EXPANDED = 7,




    DWRITE_FONT_STRETCH_EXTRA_EXPANDED = 8,




    DWRITE_FONT_STRETCH_ULTRA_EXPANDED = 9
};





enum DWRITE_FONT_STYLE
{



    DWRITE_FONT_STYLE_NORMAL,




    DWRITE_FONT_STYLE_OBLIQUE,




    DWRITE_FONT_STYLE_ITALIC

};




enum DWRITE_INFORMATIONAL_STRING_ID
{



    DWRITE_INFORMATIONAL_STRING_NONE,




    DWRITE_INFORMATIONAL_STRING_COPYRIGHT_NOTICE,




    DWRITE_INFORMATIONAL_STRING_VERSION_STRINGS,




    DWRITE_INFORMATIONAL_STRING_TRADEMARK,




    DWRITE_INFORMATIONAL_STRING_MANUFACTURER,




    DWRITE_INFORMATIONAL_STRING_DESIGNER,




    DWRITE_INFORMATIONAL_STRING_DESIGNER_URL,




    DWRITE_INFORMATIONAL_STRING_DESCRIPTION,




    DWRITE_INFORMATIONAL_STRING_FONT_VENDOR_URL,




    DWRITE_INFORMATIONAL_STRING_LICENSE_DESCRIPTION,




    DWRITE_INFORMATIONAL_STRING_LICENSE_INFO_URL,





    DWRITE_INFORMATIONAL_STRING_WIN32_FAMILY_NAMES,




    DWRITE_INFORMATIONAL_STRING_WIN32_SUBFAMILY_NAMES,





    DWRITE_INFORMATIONAL_STRING_PREFERRED_FAMILY_NAMES,




    DWRITE_INFORMATIONAL_STRING_PREFERRED_SUBFAMILY_NAMES,




    DWRITE_INFORMATIONAL_STRING_SAMPLE_TEXT
};






struct DWRITE_FONT_METRICS
{








    UINT16 designUnitsPerEm;





    UINT16 ascent;





    UINT16 descent;








    INT16 lineGap;






    UINT16 capHeight;





    UINT16 xHeight;






    INT16 underlinePosition;




    UINT16 underlineThickness;






    INT16 strikethroughPosition;




    UINT16 strikethroughThickness;
};





struct DWRITE_GLYPH_METRICS
{





    INT32 leftSideBearing;




    UINT32 advanceWidth;





    INT32 rightSideBearing;





    INT32 topSideBearing;







    UINT32 advanceHeight;






    INT32 bottomSideBearing;






    INT32 verticalOriginY;
};





struct DWRITE_GLYPH_OFFSET
{




    FLOAT advanceOffset;





    FLOAT ascenderOffset;
};











enum DWRITE_FACTORY_TYPE
{




    DWRITE_FACTORY_TYPE_SHARED,




    DWRITE_FACTORY_TYPE_ISOLATED
};










#define DWRITE_MAKE_OPENTYPE_TAG(a,b,c,d) ( \
    (static_cast<UINT32>(static_cast<UINT8>(d)) << 24) | \
    (static_cast<UINT32>(static_cast<UINT8>(c)) << 16) | \
    (static_cast<UINT32>(static_cast<UINT8>(b)) << 8)  | \
     static_cast<UINT32>(static_cast<UINT8>(a)))

interface IDWriteFontFileStream;










interface DWRITE_DECLARE_INTERFACE("727cad4e-d6af-4c9e-8a08-d695b11caa49") IDWriteFontFileLoader : public IUnknown
{











    STDMETHOD(CreateStreamFromKey)(
        __in_bcount(fontFileReferenceKeySize) void const* fontFileReferenceKey,
        UINT32 fontFileReferenceKeySize,
        __out IDWriteFontFileStream** fontFileStream
        ) PURE;
};






interface DWRITE_DECLARE_INTERFACE("b2d9f3ec-c9fe-4a11-a2ec-d86208f7c0a2") IDWriteLocalFontFileLoader : public IDWriteFontFileLoader
{










    STDMETHOD(GetFilePathLengthFromKey)(
        __in_bcount(fontFileReferenceKeySize) void const* fontFileReferenceKey,
        UINT32 fontFileReferenceKeySize,
        __out UINT32* filePathLength
        ) PURE;












    STDMETHOD(GetFilePathFromKey)(
        __in_bcount(fontFileReferenceKeySize) void const* fontFileReferenceKey,
        UINT32 fontFileReferenceKeySize,
        __out_ecount_z(filePathSize) WCHAR* filePath,
        UINT32 filePathSize
        ) PURE;











    STDMETHOD(GetLastWriteTimeFromKey)(
        __in_bcount(fontFileReferenceKeySize) void const* fontFileReferenceKey,
        UINT32 fontFileReferenceKeySize,
        __out FILETIME* lastWriteTime
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("6d4865fe-0ab8-4d91-8f62-5dd6be34a3e0") IDWriteFontFileStream : public IUnknown
{














    STDMETHOD(ReadFileFragment)(
        __deref_out_bcount(fragmentSize) void const** fragmentStart,
        UINT64 fileOffset,
        UINT64 fragmentSize,
        __out void** fragmentContext
        ) PURE;





    STDMETHOD_(void, ReleaseFileFragment)(
        void* fragmentContext
        ) PURE;














    STDMETHOD(GetFileSize)(
        __out UINT64* fileSize
        ) PURE;











    STDMETHOD(GetLastWriteTime)(
        __out UINT64* lastWriteTime
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("739d886a-cef5-47dc-8769-1a8b41bebbb0") IDWriteFontFile : public IUnknown
{









    STDMETHOD(GetReferenceKey)(
        __deref_out_bcount(*fontFileReferenceKeySize) void const** fontFileReferenceKey,
        __out UINT32* fontFileReferenceKeySize
        ) PURE;








    STDMETHOD(GetLoader)(
        __out IDWriteFontFileLoader** fontFileLoader
        ) PURE;




















    STDMETHOD(Analyze)(
        __out BOOL* isSupportedFontType,
        __out DWRITE_FONT_FILE_TYPE* fontFileType,
        __out_opt DWRITE_FONT_FACE_TYPE* fontFaceType,
        __out UINT32* numberOfFaces
        ) PURE;
};





#ifndef DWRITE_PIXEL_GEOMETRY_DEFINED
enum DWRITE_PIXEL_GEOMETRY
{



    DWRITE_PIXEL_GEOMETRY_FLAT,





    DWRITE_PIXEL_GEOMETRY_RGB,





    DWRITE_PIXEL_GEOMETRY_BGR
};
#define DWRITE_PIXEL_GEOMETRY_DEFINED
#endif




enum DWRITE_RENDERING_MODE
{



    DWRITE_RENDERING_MODE_DEFAULT,





    DWRITE_RENDERING_MODE_ALIASED,





    DWRITE_RENDERING_MODE_CLEARTYPE_GDI_CLASSIC,






    DWRITE_RENDERING_MODE_CLEARTYPE_GDI_NATURAL,





    DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL,






    DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL_SYMMETRIC,





    DWRITE_RENDERING_MODE_OUTLINE
};





struct DWRITE_MATRIX
{



    FLOAT m11;




    FLOAT m12;




    FLOAT m21;




    FLOAT m22;




    FLOAT dx;




    FLOAT dy;
};




interface DWRITE_DECLARE_INTERFACE("2f0da53a-2add-47cd-82ee-d9ec34688e75") IDWriteRenderingParams : public IUnknown
{




    STDMETHOD_(FLOAT, GetGamma)() PURE;





    STDMETHOD_(FLOAT, GetEnhancedContrast)() PURE;





    STDMETHOD_(FLOAT, GetClearTypeLevel)() PURE;




    STDMETHOD_(DWRITE_PIXEL_GEOMETRY, GetPixelGeometry)() PURE;




    STDMETHOD_(DWRITE_RENDERING_MODE, GetRenderingMode)() PURE;
};


interface ID2D1SimplifiedGeometrySink;

typedef ID2D1SimplifiedGeometrySink IDWriteGeometrySink;






interface DWRITE_DECLARE_INTERFACE("5f49804d-7024-4d43-bfa9-d25984f53849") IDWriteFontFace : public IUnknown
{



    STDMETHOD_(DWRITE_FONT_FACE_TYPE, GetType)() PURE;












    STDMETHOD(GetFiles)(
        __inout UINT32* numberOfFiles,
        __out_ecount_opt(*numberOfFiles) IDWriteFontFile** fontFiles
        ) PURE;





    STDMETHOD_(UINT32, GetIndex)() PURE;




    STDMETHOD_(DWRITE_FONT_SIMULATIONS, GetSimulations)() PURE;




    STDMETHOD_(BOOL, IsSymbolFont)() PURE;







    STDMETHOD_(void, GetMetrics)(
        __out DWRITE_FONT_METRICS* fontFaceMetrics
        ) PURE;




    STDMETHOD_(UINT16, GetGlyphCount)() PURE;















    STDMETHOD(GetDesignGlyphMetrics)(
        __in_ecount(glyphCount) UINT16 const* glyphIndices,
        UINT32 glyphCount,
        __out_ecount(glyphCount) DWRITE_GLYPH_METRICS* glyphMetrics,
        BOOL isSideways = FALSE
        ) PURE;















    STDMETHOD(GetGlyphIndices)(
        __in_ecount(codePointCount) UINT32 const* codePoints,
        UINT32 codePointCount,
        __out_ecount(codePointCount) UINT16* glyphIndices
        ) PURE;



































    STDMETHOD(TryGetFontTable)(
        __in UINT32 openTypeTableTag,
        __deref_out_bcount(*tableSize) const void** tableData,
        __out UINT32* tableSize,
        __out void** tableContext,
        __out BOOL* exists
        ) PURE;








    STDMETHOD_(void, ReleaseFontTable)(
        __in void* tableContext
        ) PURE;


















    STDMETHOD(GetGlyphRunOutline)(
        FLOAT emSize,
        __in_ecount(glyphCount) UINT16 const* glyphIndices,
        __in_ecount_opt(glyphCount) FLOAT const* glyphAdvances,
        __in_ecount_opt(glyphCount) DWRITE_GLYPH_OFFSET const* glyphOffsets,
        UINT32 glyphCount,
        BOOL isSideways,
        BOOL isRightToLeft,
        IDWriteGeometrySink* geometrySink
        ) PURE;




















    STDMETHOD(GetRecommendedRenderingMode)(
        FLOAT emSize,
        FLOAT pixelsPerDip,
        DWRITE_MEASURING_MODE measuringMode,
        IDWriteRenderingParams* renderingParams,
        __out DWRITE_RENDERING_MODE* renderingMode
        ) PURE;












    STDMETHOD(GetGdiCompatibleMetrics)(
        FLOAT emSize,
        FLOAT pixelsPerDip,
        __in_opt DWRITE_MATRIX const* transform,
        __out DWRITE_FONT_METRICS* fontFaceMetrics
        ) PURE;



























    STDMETHOD(GetGdiCompatibleGlyphMetrics)(
        FLOAT emSize,
        FLOAT pixelsPerDip,
        __in_opt DWRITE_MATRIX const* transform,
        BOOL useGdiNatural,
        __in_ecount(glyphCount) UINT16 const* glyphIndices,
        UINT32 glyphCount,
        __out_ecount(glyphCount) DWRITE_GLYPH_METRICS* glyphMetrics,
        BOOL isSideways = FALSE
        ) PURE;
};

interface IDWriteFactory;
interface IDWriteFontFileEnumerator;










interface DWRITE_DECLARE_INTERFACE("cca920e4-52f0-492b-bfa8-29c72ee0a468") IDWriteFontCollectionLoader : public IUnknown
{












    STDMETHOD(CreateEnumeratorFromKey)(
        IDWriteFactory* factory,
        __in_bcount(collectionKeySize) void const* collectionKey,
        UINT32 collectionKeySize,
        __out IDWriteFontFileEnumerator** fontFileEnumerator
        ) PURE;
};





interface DWRITE_DECLARE_INTERFACE("72755049-5ff7-435d-8348-4be97cfa6c7c") IDWriteFontFileEnumerator : public IUnknown
{









    STDMETHOD(MoveNext)(
        __out BOOL* hasCurrentFile
        ) PURE;








    STDMETHOD(GetCurrentFontFile)(
        __out IDWriteFontFile** fontFile
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("08256209-099a-4b34-b86d-c22b110e7771") IDWriteLocalizedStrings : public IUnknown
{



    STDMETHOD_(UINT32, GetCount)() PURE;











    STDMETHOD(FindLocaleName)(
        __in_z WCHAR const* localeName,
        __out UINT32* index,
        __out BOOL* exists
        ) PURE;









    STDMETHOD(GetLocaleNameLength)(
        UINT32 index,
        __out UINT32* length
        ) PURE;











    STDMETHOD(GetLocaleName)(
        UINT32 index,
        __out_ecount_z(size) WCHAR* localeName,
        UINT32 size
        ) PURE;









    STDMETHOD(GetStringLength)(
        UINT32 index,
        __out UINT32* length
        ) PURE;











    STDMETHOD(GetString)(
        UINT32 index,
        __out_ecount_z(size) WCHAR* stringBuffer,
        UINT32 size
        ) PURE;
};

interface IDWriteFontFamily;
interface IDWriteFont;




interface DWRITE_DECLARE_INTERFACE("a84cee02-3eea-4eee-a827-87c1a02a0fcc") IDWriteFontCollection : public IUnknown
{



    STDMETHOD_(UINT32, GetFontFamilyCount)() PURE;









    STDMETHOD(GetFontFamily)(
        UINT32 index,
        __out IDWriteFontFamily** fontFamily
        ) PURE;










    STDMETHOD(FindFamilyName)(
        __in_z WCHAR const* familyName,
        __out UINT32* index,
        __out BOOL* exists
        ) PURE;










    STDMETHOD(GetFontFromFontFace)(
        IDWriteFontFace* fontFace,
        __out IDWriteFont** font
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("1a0d8438-1d97-4ec1-aef9-a2fb86ed6acb") IDWriteFontList : public IUnknown
{







    STDMETHOD(GetFontCollection)(
        __out IDWriteFontCollection** fontCollection
        ) PURE;




    STDMETHOD_(UINT32, GetFontCount)() PURE;









    STDMETHOD(GetFont)(
        UINT32 index,
        __out IDWriteFont** font
        ) PURE;
};





interface DWRITE_DECLARE_INTERFACE("da20d8ef-812a-4c43-9802-62ec4abd7add") IDWriteFontFamily : public IDWriteFontList
{







    STDMETHOD(GetFamilyNames)(
        __out IDWriteLocalizedStrings** names
        ) PURE;











    STDMETHOD(GetFirstMatchingFont)(
        DWRITE_FONT_WEIGHT  weight,
        DWRITE_FONT_STRETCH stretch,
        DWRITE_FONT_STYLE   style,
        __out IDWriteFont** matchingFont
        ) PURE;











    STDMETHOD(GetMatchingFonts)(
        DWRITE_FONT_WEIGHT      weight,
        DWRITE_FONT_STRETCH     stretch,
        DWRITE_FONT_STYLE       style,
        __out IDWriteFontList** matchingFonts
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("acd16696-8c14-4f5d-877e-fe3fc1d32737") IDWriteFont : public IUnknown
{







    STDMETHOD(GetFontFamily)(
        __out IDWriteFontFamily** fontFamily
        ) PURE;




    STDMETHOD_(DWRITE_FONT_WEIGHT, GetWeight)() PURE;




    STDMETHOD_(DWRITE_FONT_STRETCH, GetStretch)() PURE;




    STDMETHOD_(DWRITE_FONT_STYLE, GetStyle)() PURE;




    STDMETHOD_(BOOL, IsSymbolFont)() PURE;








    STDMETHOD(GetFaceNames)(
        __out IDWriteLocalizedStrings** names
        ) PURE;











    STDMETHOD(GetInformationalStrings)(
        DWRITE_INFORMATIONAL_STRING_ID informationalStringID,
        __out IDWriteLocalizedStrings** informationalStrings,
        __out BOOL* exists
        ) PURE;




    STDMETHOD_(DWRITE_FONT_SIMULATIONS, GetSimulations)() PURE;





    STDMETHOD_(void, GetMetrics)(
        __out DWRITE_FONT_METRICS* fontMetrics
        ) PURE;









    STDMETHOD(HasCharacter)(
        UINT32 unicodeValue,
        __out BOOL* exists
        ) PURE;








    STDMETHOD(CreateFontFace)(
        __out IDWriteFontFace** fontFace
        ) PURE;
};




enum DWRITE_READING_DIRECTION
{



    DWRITE_READING_DIRECTION_LEFT_TO_RIGHT,




    DWRITE_READING_DIRECTION_RIGHT_TO_LEFT
};




enum DWRITE_FLOW_DIRECTION
{



    DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM
};





enum DWRITE_TEXT_ALIGNMENT
{



    DWRITE_TEXT_ALIGNMENT_LEADING,




    DWRITE_TEXT_ALIGNMENT_TRAILING,




    DWRITE_TEXT_ALIGNMENT_CENTER
};





enum DWRITE_PARAGRAPH_ALIGNMENT
{



    DWRITE_PARAGRAPH_ALIGNMENT_NEAR,




    DWRITE_PARAGRAPH_ALIGNMENT_FAR,




    DWRITE_PARAGRAPH_ALIGNMENT_CENTER
};




enum DWRITE_WORD_WRAPPING
{



    DWRITE_WORD_WRAPPING_WRAP,





    DWRITE_WORD_WRAPPING_NO_WRAP
};




enum DWRITE_LINE_SPACING_METHOD
{



    DWRITE_LINE_SPACING_METHOD_DEFAULT,





    DWRITE_LINE_SPACING_METHOD_UNIFORM
};




enum DWRITE_TRIMMING_GRANULARITY
{



    DWRITE_TRIMMING_GRANULARITY_NONE,




    DWRITE_TRIMMING_GRANULARITY_CHARACTER,




    DWRITE_TRIMMING_GRANULARITY_WORD
};




enum DWRITE_FONT_FEATURE_TAG
{
    DWRITE_FONT_FEATURE_TAG_ALTERNATIVE_FRACTIONS               = 0x63726661,
    DWRITE_FONT_FEATURE_TAG_PETITE_CAPITALS_FROM_CAPITALS       = 0x63703263,
    DWRITE_FONT_FEATURE_TAG_SMALL_CAPITALS_FROM_CAPITALS        = 0x63733263,
    DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_ALTERNATES               = 0x746c6163,
    DWRITE_FONT_FEATURE_TAG_CASE_SENSITIVE_FORMS                = 0x65736163,
    DWRITE_FONT_FEATURE_TAG_GLYPH_COMPOSITION_DECOMPOSITION     = 0x706d6363,
    DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_LIGATURES                = 0x67696c63,
    DWRITE_FONT_FEATURE_TAG_CAPITAL_SPACING                     = 0x70737063,
    DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_SWASH                    = 0x68777363,
    DWRITE_FONT_FEATURE_TAG_CURSIVE_POSITIONING                 = 0x73727563,
    DWRITE_FONT_FEATURE_TAG_DEFAULT                             = 0x746c6664,
    DWRITE_FONT_FEATURE_TAG_DISCRETIONARY_LIGATURES             = 0x67696c64,
    DWRITE_FONT_FEATURE_TAG_EXPERT_FORMS                        = 0x74707865,
    DWRITE_FONT_FEATURE_TAG_FRACTIONS                           = 0x63617266,
    DWRITE_FONT_FEATURE_TAG_FULL_WIDTH                          = 0x64697766,
    DWRITE_FONT_FEATURE_TAG_HALF_FORMS                          = 0x666c6168,
    DWRITE_FONT_FEATURE_TAG_HALANT_FORMS                        = 0x6e6c6168,
    DWRITE_FONT_FEATURE_TAG_ALTERNATE_HALF_WIDTH                = 0x746c6168,
    DWRITE_FONT_FEATURE_TAG_HISTORICAL_FORMS                    = 0x74736968,
    DWRITE_FONT_FEATURE_TAG_HORIZONTAL_KANA_ALTERNATES          = 0x616e6b68,
    DWRITE_FONT_FEATURE_TAG_HISTORICAL_LIGATURES                = 0x67696c68,
    DWRITE_FONT_FEATURE_TAG_HALF_WIDTH                          = 0x64697768,
    DWRITE_FONT_FEATURE_TAG_HOJO_KANJI_FORMS                    = 0x6f6a6f68,
    DWRITE_FONT_FEATURE_TAG_JIS04_FORMS                         = 0x3430706a,
    DWRITE_FONT_FEATURE_TAG_JIS78_FORMS                         = 0x3837706a,
    DWRITE_FONT_FEATURE_TAG_JIS83_FORMS                         = 0x3338706a,
    DWRITE_FONT_FEATURE_TAG_JIS90_FORMS                         = 0x3039706a,
    DWRITE_FONT_FEATURE_TAG_KERNING                             = 0x6e72656b,
    DWRITE_FONT_FEATURE_TAG_STANDARD_LIGATURES                  = 0x6167696c,
    DWRITE_FONT_FEATURE_TAG_LINING_FIGURES                      = 0x6d756e6c,
    DWRITE_FONT_FEATURE_TAG_LOCALIZED_FORMS                     = 0x6c636f6c,
    DWRITE_FONT_FEATURE_TAG_MARK_POSITIONING                    = 0x6b72616d,
    DWRITE_FONT_FEATURE_TAG_MATHEMATICAL_GREEK                  = 0x6b72676d,
    DWRITE_FONT_FEATURE_TAG_MARK_TO_MARK_POSITIONING            = 0x6b6d6b6d,
    DWRITE_FONT_FEATURE_TAG_ALTERNATE_ANNOTATION_FORMS          = 0x746c616e,
    DWRITE_FONT_FEATURE_TAG_NLC_KANJI_FORMS                     = 0x6b636c6e,
    DWRITE_FONT_FEATURE_TAG_OLD_STYLE_FIGURES                   = 0x6d756e6f,
    DWRITE_FONT_FEATURE_TAG_ORDINALS                            = 0x6e64726f,
    DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_ALTERNATE_WIDTH        = 0x746c6170,
    DWRITE_FONT_FEATURE_TAG_PETITE_CAPITALS                     = 0x70616370,
    DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_FIGURES                = 0x6d756e70,
    DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_WIDTHS                 = 0x64697770,
    DWRITE_FONT_FEATURE_TAG_QUARTER_WIDTHS                      = 0x64697771,
    DWRITE_FONT_FEATURE_TAG_REQUIRED_LIGATURES                  = 0x67696c72,
    DWRITE_FONT_FEATURE_TAG_RUBY_NOTATION_FORMS                 = 0x79627572,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_ALTERNATES                = 0x746c6173,
    DWRITE_FONT_FEATURE_TAG_SCIENTIFIC_INFERIORS                = 0x666e6973,
    DWRITE_FONT_FEATURE_TAG_SMALL_CAPITALS                      = 0x70636d73,
    DWRITE_FONT_FEATURE_TAG_SIMPLIFIED_FORMS                    = 0x6c706d73,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_1                     = 0x31307373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_2                     = 0x32307373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_3                     = 0x33307373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_4                     = 0x34307373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_5                     = 0x35307373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_6                     = 0x36307373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7                     = 0x37307373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_8                     = 0x38307373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_9                     = 0x39307373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_10                    = 0x30317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_11                    = 0x31317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_12                    = 0x32317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_13                    = 0x33317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_14                    = 0x34317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_15                    = 0x35317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_16                    = 0x36317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_17                    = 0x37317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_18                    = 0x38317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_19                    = 0x39317373,
    DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_20                    = 0x30327373,
    DWRITE_FONT_FEATURE_TAG_SUBSCRIPT                           = 0x73627573,
    DWRITE_FONT_FEATURE_TAG_SUPERSCRIPT                         = 0x73707573,
    DWRITE_FONT_FEATURE_TAG_SWASH                               = 0x68737773,
    DWRITE_FONT_FEATURE_TAG_TITLING                             = 0x6c746974,
    DWRITE_FONT_FEATURE_TAG_TRADITIONAL_NAME_FORMS              = 0x6d616e74,
    DWRITE_FONT_FEATURE_TAG_TABULAR_FIGURES                     = 0x6d756e74,
    DWRITE_FONT_FEATURE_TAG_TRADITIONAL_FORMS                   = 0x64617274,
    DWRITE_FONT_FEATURE_TAG_THIRD_WIDTHS                        = 0x64697774,
    DWRITE_FONT_FEATURE_TAG_UNICASE                             = 0x63696e75,
    DWRITE_FONT_FEATURE_TAG_SLASHED_ZERO                        = 0x6f72657a,
};




struct DWRITE_TEXT_RANGE
{



    UINT32 startPosition;




    UINT32 length;
};




struct DWRITE_FONT_FEATURE
{



    DWRITE_FONT_FEATURE_TAG nameTag;








    UINT32 parameter;
};






struct DWRITE_TYPOGRAPHIC_FEATURES
{



    __field_ecount(featureCount) DWRITE_FONT_FEATURE* features;




    UINT32 featureCount;
};




struct DWRITE_TRIMMING
{



    DWRITE_TRIMMING_GRANULARITY granularity;





    UINT32 delimiter;




    UINT32 delimiterCount;
};


interface IDWriteTypography;
interface IDWriteInlineObject;







interface DWRITE_DECLARE_INTERFACE("9c906818-31d7-4fd3-a151-7c5e225db55a") IDWriteTextFormat : public IUnknown
{







    STDMETHOD(SetTextAlignment)(
        DWRITE_TEXT_ALIGNMENT textAlignment
        ) PURE;








    STDMETHOD(SetParagraphAlignment)(
        DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment
        ) PURE;








    STDMETHOD(SetWordWrapping)(
        DWRITE_WORD_WRAPPING wordWrapping
        ) PURE;








    STDMETHOD(SetReadingDirection)(
        DWRITE_READING_DIRECTION readingDirection
        ) PURE;








    STDMETHOD(SetFlowDirection)(
        DWRITE_FLOW_DIRECTION flowDirection
        ) PURE;








    STDMETHOD(SetIncrementalTabStop)(
        FLOAT incrementalTabStop
        ) PURE;















    STDMETHOD(SetTrimming)(
        __in DWRITE_TRIMMING const* trimmingOptions,
        IDWriteInlineObject* trimmingSign
        ) PURE;














    STDMETHOD(SetLineSpacing)(
        DWRITE_LINE_SPACING_METHOD lineSpacingMethod,
        FLOAT lineSpacing,
        FLOAT baseline
        ) PURE;




    STDMETHOD_(DWRITE_TEXT_ALIGNMENT, GetTextAlignment)() PURE;




    STDMETHOD_(DWRITE_PARAGRAPH_ALIGNMENT, GetParagraphAlignment)() PURE;




    STDMETHOD_(DWRITE_WORD_WRAPPING, GetWordWrapping)() PURE;




    STDMETHOD_(DWRITE_READING_DIRECTION, GetReadingDirection)() PURE;




    STDMETHOD_(DWRITE_FLOW_DIRECTION, GetFlowDirection)() PURE;




    STDMETHOD_(FLOAT, GetIncrementalTabStop)() PURE;









    STDMETHOD(GetTrimming)(
        __out DWRITE_TRIMMING* trimmingOptions,
        __out IDWriteInlineObject** trimmingSign
        ) PURE;










    STDMETHOD(GetLineSpacing)(
        __out DWRITE_LINE_SPACING_METHOD* lineSpacingMethod,
        __out FLOAT* lineSpacing,
        __out FLOAT* baseline
        ) PURE;








    STDMETHOD(GetFontCollection)(
        __out IDWriteFontCollection** fontCollection
        ) PURE;




    STDMETHOD_(UINT32, GetFontFamilyNameLength)() PURE;









    STDMETHOD(GetFontFamilyName)(
        __out_ecount_z(nameSize) WCHAR* fontFamilyName,
        UINT32 nameSize
        ) PURE;




    STDMETHOD_(DWRITE_FONT_WEIGHT, GetFontWeight)() PURE;




    STDMETHOD_(DWRITE_FONT_STYLE, GetFontStyle)() PURE;




    STDMETHOD_(DWRITE_FONT_STRETCH, GetFontStretch)() PURE;




    STDMETHOD_(FLOAT, GetFontSize)() PURE;




    STDMETHOD_(UINT32, GetLocaleNameLength)() PURE;









    STDMETHOD(GetLocaleName)(
        __out_ecount_z(nameSize) WCHAR* localeName,
        UINT32 nameSize
        ) PURE;
};





interface DWRITE_DECLARE_INTERFACE("55f1112b-1dc2-4b3c-9541-f46894ed85b6") IDWriteTypography : public IUnknown
{







    STDMETHOD(AddFontFeature)(
        DWRITE_FONT_FEATURE fontFeature
        ) PURE;




    STDMETHOD_(UINT32, GetFontFeatureCount)() PURE;









    STDMETHOD(GetFontFeature)(
        UINT32 fontFeatureIndex,
        __out DWRITE_FONT_FEATURE* fontFeature
        ) PURE;
};

enum DWRITE_SCRIPT_SHAPES
{



    DWRITE_SCRIPT_SHAPES_DEFAULT = 0,




    DWRITE_SCRIPT_SHAPES_NO_VISUAL = 1
};

#ifdef DEFINE_ENUM_FLAG_OPERATORS
DEFINE_ENUM_FLAG_OPERATORS(DWRITE_SCRIPT_SHAPES);
#endif




struct DWRITE_SCRIPT_ANALYSIS
{



    UINT16 script;




    DWRITE_SCRIPT_SHAPES shapes;
};





enum DWRITE_BREAK_CONDITION
{




    DWRITE_BREAK_CONDITION_NEUTRAL,






    DWRITE_BREAK_CONDITION_CAN_BREAK,





    DWRITE_BREAK_CONDITION_MAY_NOT_BREAK,





    DWRITE_BREAK_CONDITION_MUST_BREAK
};




struct DWRITE_LINE_BREAKPOINT
{



    UINT8 breakConditionBefore  : 2;




    UINT8 breakConditionAfter   : 2;





    UINT8 isWhitespace          : 1;





    UINT8 isSoftHyphen          : 1;

    UINT8 padding               : 2;
};




enum DWRITE_NUMBER_SUBSTITUTION_METHOD
{




    DWRITE_NUMBER_SUBSTITUTION_METHOD_FROM_CULTURE,







    DWRITE_NUMBER_SUBSTITUTION_METHOD_CONTEXTUAL,





    DWRITE_NUMBER_SUBSTITUTION_METHOD_NONE,





    DWRITE_NUMBER_SUBSTITUTION_METHOD_NATIONAL,








    DWRITE_NUMBER_SUBSTITUTION_METHOD_TRADITIONAL
};




interface DECLSPEC_UUID("14885CC9-BAB0-4f90-B6ED-5C366A2CD03D") DECLSPEC_NOVTABLE IDWriteNumberSubstitution : public IUnknown
{
};




struct DWRITE_SHAPING_TEXT_PROPERTIES
{




    UINT16  isShapedAlone       : 1;




    UINT16  reserved            : 15;
};




struct DWRITE_SHAPING_GLYPH_PROPERTIES
{





    UINT16  justification       : 4;




    UINT16  isClusterStart      : 1;




    UINT16  isDiacritic         : 1;




    UINT16  isZeroWidthSpace    : 1;




    UINT16  reserved            : 9;
};












interface DECLSPEC_UUID("688e1a58-5094-47c8-adc8-fbcea60ae92b") DECLSPEC_NOVTABLE IDWriteTextAnalysisSource : public IUnknown
{


























    STDMETHOD(GetTextAtPosition)(
        UINT32 textPosition,
        __out WCHAR const** textString,
        __out UINT32* textLength
        ) PURE;




















    STDMETHOD(GetTextBeforePosition)(
        UINT32 textPosition,
        __out WCHAR const** textString,
        __out UINT32* textLength
        ) PURE;




    STDMETHOD_(DWRITE_READING_DIRECTION, GetParagraphReadingDirection)() PURE;













    STDMETHOD(GetLocaleName)(
        UINT32 textPosition,
        __out UINT32* textLength,
        __out_z WCHAR const** localeName
        ) PURE;















    STDMETHOD(GetNumberSubstitution)(
        UINT32 textPosition,
        __out UINT32* textLength,
        __out IDWriteNumberSubstitution** numberSubstitution
        ) PURE;
};







interface DECLSPEC_UUID("5810cd44-0ca0-4701-b3fa-bec5182ae4f6") DECLSPEC_NOVTABLE IDWriteTextAnalysisSink : public IUnknown
{









    STDMETHOD(SetScriptAnalysis)(
        UINT32 textPosition,
        UINT32 textLength,
        __in DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis
        ) PURE;











    STDMETHOD(SetLineBreakpoints)(
        UINT32 textPosition,
        UINT32 textLength,
        __in_ecount(textLength) DWRITE_LINE_BREAKPOINT const* lineBreakpoints
        ) PURE;















    STDMETHOD(SetBidiLevel)(
        UINT32 textPosition,
        UINT32 textLength,
        UINT8 explicitLevel,
        UINT8 resolvedLevel
        ) PURE;


















    STDMETHOD(SetNumberSubstitution)(
        UINT32 textPosition,
        UINT32 textLength,
        __notnull IDWriteNumberSubstitution* numberSubstitution
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("b7e6163e-7f46-43b4-84b3-e4e6249c365d") IDWriteTextAnalyzer : public IUnknown
{












    STDMETHOD(AnalyzeScript)(
        IDWriteTextAnalysisSource* analysisSource,
        UINT32 textPosition,
        UINT32 textLength,
        IDWriteTextAnalysisSink* analysisSink
        ) PURE;





















    STDMETHOD(AnalyzeBidi)(
        IDWriteTextAnalysisSource* analysisSource,
        UINT32 textPosition,
        UINT32 textLength,
        IDWriteTextAnalysisSink* analysisSink
        ) PURE;






















    STDMETHOD(AnalyzeNumberSubstitution)(
        IDWriteTextAnalysisSource* analysisSource,
        UINT32 textPosition,
        UINT32 textLength,
        IDWriteTextAnalysisSink* analysisSink
        ) PURE;





























    STDMETHOD(AnalyzeLineBreakpoints)(
        IDWriteTextAnalysisSource* analysisSource,
        UINT32 textPosition,
        UINT32 textLength,
        IDWriteTextAnalysisSink* analysisSink
        ) PURE;















































    STDMETHOD(GetGlyphs)(
        __in_ecount(textLength) WCHAR const* textString,
        UINT32 textLength,
        IDWriteFontFace* fontFace,
        BOOL isSideways,
        BOOL isRightToLeft,
        __in DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis,
        __in_z_opt WCHAR const* localeName,
        __maybenull IDWriteNumberSubstitution* numberSubstitution,
        __in_ecount_opt(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** features,
        __in_ecount_opt(featureRanges) UINT32 const* featureRangeLengths,
        UINT32 featureRanges,
        UINT32 maxGlyphCount,
        __out_ecount(textLength) UINT16* clusterMap,
        __out_ecount(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* textProps,
        __out_ecount(maxGlyphCount) UINT16* glyphIndices,
        __out_ecount(maxGlyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES* glyphProps,
        __out UINT32* actualGlyphCount
        ) PURE;

































    STDMETHOD(GetGlyphPlacements)(
        __in_ecount(textLength) WCHAR const* textString,
        __in_ecount(textLength) UINT16 const* clusterMap,
        __in_ecount(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* textProps,
        UINT32 textLength,
        __in_ecount(glyphCount) UINT16 const* glyphIndices,
        __in_ecount(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const* glyphProps,
        UINT32 glyphCount,
        IDWriteFontFace * fontFace,
        FLOAT fontEmSize,
        BOOL isSideways,
        BOOL isRightToLeft,
        __in DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis,
        __in_z_opt WCHAR const* localeName,
        __in_ecount_opt(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** features,
        __in_ecount_opt(featureRanges) UINT32 const* featureRangeLengths,
        UINT32 featureRanges,
        __out_ecount(glyphCount) FLOAT* glyphAdvances,
        __out_ecount(glyphCount) DWRITE_GLYPH_OFFSET* glyphOffsets
        ) PURE;










































    STDMETHOD(GetGdiCompatibleGlyphPlacements)(
        __in_ecount(textLength) WCHAR const* textString,
        __in_ecount(textLength) UINT16 const* clusterMap,
        __in_ecount(textLength) DWRITE_SHAPING_TEXT_PROPERTIES* textProps,
        UINT32 textLength,
        __in_ecount(glyphCount) UINT16 const* glyphIndices,
        __in_ecount(glyphCount) DWRITE_SHAPING_GLYPH_PROPERTIES const* glyphProps,
        UINT32 glyphCount,
        IDWriteFontFace * fontFace,
        FLOAT fontEmSize,
        FLOAT pixelsPerDip,
        __in_opt DWRITE_MATRIX const* transform,
        BOOL useGdiNatural,
        BOOL isSideways,
        BOOL isRightToLeft,
        __in DWRITE_SCRIPT_ANALYSIS const* scriptAnalysis,
        __in_z_opt WCHAR const* localeName,
        __in_ecount_opt(featureRanges) DWRITE_TYPOGRAPHIC_FEATURES const** features,
        __in_ecount_opt(featureRanges) UINT32 const* featureRangeLengths,
        UINT32 featureRanges,
        __out_ecount(glyphCount) FLOAT* glyphAdvances,
        __out_ecount(glyphCount) DWRITE_GLYPH_OFFSET* glyphOffsets
        ) PURE;
};





struct DWRITE_GLYPH_RUN
{



    __notnull IDWriteFontFace* fontFace;




    FLOAT fontEmSize;




    UINT32 glyphCount;




    __field_ecount(glyphCount) UINT16 const* glyphIndices;




    __field_ecount_opt(glyphCount) FLOAT const* glyphAdvances;




    __field_ecount_opt(glyphCount) DWRITE_GLYPH_OFFSET const* glyphOffsets;







    BOOL isSideways;








    UINT32 bidiLevel;
};





struct DWRITE_GLYPH_RUN_DESCRIPTION
{



    __nullterminated WCHAR const* localeName;




    __field_ecount(stringLength) WCHAR const* string;





    UINT32 stringLength;





    __field_ecount(stringLength) UINT16 const* clusterMap;





    UINT32 textPosition;
};





struct DWRITE_UNDERLINE
{



    FLOAT width;





    FLOAT thickness;






    FLOAT offset;




    FLOAT runHeight;






    DWRITE_READING_DIRECTION readingDirection;






    DWRITE_FLOW_DIRECTION flowDirection;








    __nullterminated WCHAR const* localeName;






    DWRITE_MEASURING_MODE measuringMode;
};





struct DWRITE_STRIKETHROUGH
{



    FLOAT width;





    FLOAT thickness;






    FLOAT offset;






    DWRITE_READING_DIRECTION readingDirection;






    DWRITE_FLOW_DIRECTION flowDirection;




    __nullterminated WCHAR const* localeName;






    DWRITE_MEASURING_MODE measuringMode;
};





struct DWRITE_LINE_METRICS
{




    UINT32 length;





    UINT32 trailingWhitespaceLength;






    UINT32 newlineLength;




    FLOAT height;




    FLOAT baseline;




    BOOL isTrimmed;
};





struct DWRITE_CLUSTER_METRICS
{



    FLOAT width;




    UINT16 length;




    UINT16 canWrapLineAfter : 1;




    UINT16 isWhitespace : 1;




    UINT16 isNewline : 1;




    UINT16 isSoftHyphen : 1;




    UINT16 isRightToLeft : 1;

    UINT16 padding : 11;
};






struct DWRITE_TEXT_METRICS
{




    FLOAT left;





    FLOAT top;





    FLOAT width;





    FLOAT widthIncludingTrailingWhitespace;





    FLOAT height;






    FLOAT layoutWidth;





    FLOAT layoutHeight;







    UINT32 maxBidiReorderingDepth;




    UINT32 lineCount;
};






struct DWRITE_INLINE_OBJECT_METRICS
{



    FLOAT width;




    FLOAT height;





    FLOAT baseline;





    BOOL  supportsSideways;
};













struct DWRITE_OVERHANG_METRICS
{



    FLOAT left;




    FLOAT top;




    FLOAT right;




    FLOAT bottom;
};





struct DWRITE_HIT_TEST_METRICS
{



    UINT32 textPosition;




    UINT32 length;




    FLOAT left;




    FLOAT top;




    FLOAT width;




    FLOAT height;




    UINT32 bidiLevel;




    BOOL isText;




    BOOL isTrimmed;
};


interface IDWriteTextRenderer;






interface DWRITE_DECLARE_INTERFACE("8339FDE3-106F-47ab-8373-1C6295EB10B3") IDWriteInlineObject : public IUnknown
{















    STDMETHOD(Draw)(
        __maybenull void* clientDrawingContext,
        IDWriteTextRenderer* renderer,
        FLOAT originX,
        FLOAT originY,
        BOOL isSideways,
        BOOL isRightToLeft,
        __maybenull IUnknown* clientDrawingEffect
        ) PURE;








    STDMETHOD(GetMetrics)(
        __out DWRITE_INLINE_OBJECT_METRICS* metrics
        ) PURE;

















    STDMETHOD(GetOverhangMetrics)(
        __out DWRITE_OVERHANG_METRICS* overhangs
        ) PURE;










    STDMETHOD(GetBreakConditions)(
        __out DWRITE_BREAK_CONDITION* breakConditionBefore,
        __out DWRITE_BREAK_CONDITION* breakConditionAfter
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("eaf3a2da-ecf4-4d24-b644-b34f6842024b") IDWritePixelSnapping : public IUnknown
{









    STDMETHOD(IsPixelSnappingDisabled)(
        __maybenull void* clientDrawingContext,
        __out BOOL* isDisabled
        ) PURE;










    STDMETHOD(GetCurrentTransform)(
        __maybenull void* clientDrawingContext,
        __out DWRITE_MATRIX* transform
        ) PURE;











    STDMETHOD(GetPixelsPerDip)(
        __maybenull void* clientDrawingContext,
        __out FLOAT* pixelsPerDip
        ) PURE;
};






interface DWRITE_DECLARE_INTERFACE("ef8a8135-5cc6-45fe-8825-c5a0724eb819") IDWriteTextRenderer : public IDWritePixelSnapping
{























    STDMETHOD(DrawGlyphRun)(
        __maybenull void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        DWRITE_MEASURING_MODE measuringMode,
        __in DWRITE_GLYPH_RUN const* glyphRun,
        __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
        __maybenull IUnknown* clientDrawingEffect
        ) PURE;




























    STDMETHOD(DrawUnderline)(
        __maybenull void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        __in DWRITE_UNDERLINE const* underline,
        __maybenull IUnknown* clientDrawingEffect
        ) PURE;
























    STDMETHOD(DrawStrikethrough)(
        __maybenull void* clientDrawingContext,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        __in DWRITE_STRIKETHROUGH const* strikethrough,
        __maybenull IUnknown* clientDrawingEffect
        ) PURE;





















    STDMETHOD(DrawInlineObject)(
        __maybenull void* clientDrawingContext,
        FLOAT originX,
        FLOAT originY,
        IDWriteInlineObject* inlineObject,
        BOOL isSideways,
        BOOL isRightToLeft,
        __maybenull IUnknown* clientDrawingEffect
        ) PURE;
};







interface DWRITE_DECLARE_INTERFACE("53737037-6d14-410b-9bfe-0b182bb70961") IDWriteTextLayout : public IDWriteTextFormat
{







    STDMETHOD(SetMaxWidth)(
        FLOAT maxWidth
        ) PURE;








    STDMETHOD(SetMaxHeight)(
        FLOAT maxHeight
        ) PURE;









    STDMETHOD(SetFontCollection)(
        IDWriteFontCollection* fontCollection,
        DWRITE_TEXT_RANGE textRange
        ) PURE;









    STDMETHOD(SetFontFamilyName)(
        __in_z WCHAR const* fontFamilyName,
        DWRITE_TEXT_RANGE textRange
        ) PURE;









    STDMETHOD(SetFontWeight)(
        DWRITE_FONT_WEIGHT fontWeight,
        DWRITE_TEXT_RANGE textRange
        ) PURE;









    STDMETHOD(SetFontStyle)(
        DWRITE_FONT_STYLE fontStyle,
        DWRITE_TEXT_RANGE textRange
        ) PURE;









    STDMETHOD(SetFontStretch)(
        DWRITE_FONT_STRETCH fontStretch,
        DWRITE_TEXT_RANGE textRange
        ) PURE;









    STDMETHOD(SetFontSize)(
        FLOAT fontSize,
        DWRITE_TEXT_RANGE textRange
        ) PURE;









    STDMETHOD(SetUnderline)(
        BOOL hasUnderline,
        DWRITE_TEXT_RANGE textRange
        ) PURE;









    STDMETHOD(SetStrikethrough)(
        BOOL hasStrikethrough,
        DWRITE_TEXT_RANGE textRange
        ) PURE;













    STDMETHOD(SetDrawingEffect)(
        IUnknown* drawingEffect,
        DWRITE_TEXT_RANGE textRange
        ) PURE;














    STDMETHOD(SetInlineObject)(
        IDWriteInlineObject* inlineObject,
        DWRITE_TEXT_RANGE textRange
        ) PURE;









    STDMETHOD(SetTypography)(
        IDWriteTypography* typography,
        DWRITE_TEXT_RANGE textRange
        ) PURE;









    STDMETHOD(SetLocaleName)(
        __in_z WCHAR const* localeName,
        DWRITE_TEXT_RANGE textRange
        ) PURE;




    STDMETHOD_(FLOAT, GetMaxWidth)() PURE;




    STDMETHOD_(FLOAT, GetMaxHeight)() PURE;










    STDMETHOD(GetFontCollection)(
        UINT32 currentPosition,
        __out IDWriteFontCollection** fontCollection,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetFontFamilyNameLength)(
        UINT32 currentPosition,
        __out UINT32* nameLength,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;











    STDMETHOD(GetFontFamilyName)(
        UINT32 currentPosition,
        __out_ecount_z(nameSize) WCHAR* fontFamilyName,
        UINT32 nameSize,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetFontWeight)(
        UINT32 currentPosition,
        __out DWRITE_FONT_WEIGHT* fontWeight,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetFontStyle)(
        UINT32 currentPosition,
        __out DWRITE_FONT_STYLE* fontStyle,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetFontStretch)(
        UINT32 currentPosition,
        __out DWRITE_FONT_STRETCH* fontStretch,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetFontSize)(
        UINT32 currentPosition,
        __out FLOAT* fontSize,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetUnderline)(
        UINT32 currentPosition,
        __out BOOL* hasUnderline,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetStrikethrough)(
        UINT32 currentPosition,
        __out BOOL* hasStrikethrough,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetDrawingEffect)(
        UINT32 currentPosition,
        __out IUnknown** drawingEffect,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetInlineObject)(
        UINT32 currentPosition,
        __out IDWriteInlineObject** inlineObject,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetTypography)(
        UINT32 currentPosition,
        __out IDWriteTypography** typography,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;










    STDMETHOD(GetLocaleNameLength)(
        UINT32 currentPosition,
        __out UINT32* nameLength,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;











    STDMETHOD(GetLocaleName)(
        UINT32 currentPosition,
        __out_ecount_z(nameSize) WCHAR* localeName,
        UINT32 nameSize,
        __out_opt DWRITE_TEXT_RANGE* textRange = NULL
        ) PURE;













    STDMETHOD(Draw)(
        __maybenull void* clientDrawingContext,
        IDWriteTextRenderer* renderer,
        FLOAT originX,
        FLOAT originY
        ) PURE;

















    STDMETHOD(GetLineMetrics)(
        __out_ecount_opt(maxLineCount) DWRITE_LINE_METRICS* lineMetrics,
        UINT32 maxLineCount,
        __out UINT32* actualLineCount
        ) PURE;














    STDMETHOD(GetMetrics)(
        __out DWRITE_TEXT_METRICS* textMetrics
        ) PURE;














    STDMETHOD(GetOverhangMetrics)(
        __out DWRITE_OVERHANG_METRICS* overhangs
        ) PURE;
















    STDMETHOD(GetClusterMetrics)(
        __out_ecount_opt(maxClusterCount) DWRITE_CLUSTER_METRICS* clusterMetrics,
        UINT32 maxClusterCount,
        __out UINT32* actualClusterCount
        ) PURE;









    STDMETHOD(DetermineMinWidth)(
        __out FLOAT* minWidth
        ) PURE;





















    STDMETHOD(HitTestPoint)(
        FLOAT pointX,
        FLOAT pointY,
        __out BOOL* isTrailingHit,
        __out BOOL* isInside,
        __out DWRITE_HIT_TEST_METRICS* hitTestMetrics
        ) PURE;




























    STDMETHOD(HitTestTextPosition)(
        UINT32 textPosition,
        BOOL isTrailingHit,
        __out FLOAT* pointX,
        __out FLOAT* pointY,
        __out DWRITE_HIT_TEST_METRICS* hitTestMetrics
        ) PURE;









































    STDMETHOD(HitTestTextRange)(
        UINT32 textPosition,
        UINT32 textLength,
        FLOAT originX,
        FLOAT originY,
        __out_ecount_opt(maxHitTestMetricsCount) DWRITE_HIT_TEST_METRICS* hitTestMetrics,
        UINT32 maxHitTestMetricsCount,
        __out UINT32* actualHitTestMetricsCount
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("5e5a32a3-8dff-4773-9ff6-0696eab77267") IDWriteBitmapRenderTarget : public IUnknown
{



















    STDMETHOD(DrawGlyphRun)(
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        DWRITE_MEASURING_MODE measuringMode,
        __in DWRITE_GLYPH_RUN const* glyphRun,
        IDWriteRenderingParams* renderingParams,
        COLORREF textColor,
        __out_opt RECT* blackBoxRect = NULL
        ) PURE;













    STDMETHOD_(HDC, GetMemoryDC)() PURE;








    STDMETHOD_(FLOAT, GetPixelsPerDip)() PURE;









    STDMETHOD(SetPixelsPerDip)(
        FLOAT pixelsPerDip
        ) PURE;










    STDMETHOD(GetCurrentTransform)(
        __out DWRITE_MATRIX* transform
        ) PURE;










    STDMETHOD(SetCurrentTransform)(
        __in_opt DWRITE_MATRIX const* transform
        ) PURE;








    STDMETHOD(GetSize)(
        __out SIZE* size
        ) PURE;









    STDMETHOD(Resize)(
        UINT32 width,
        UINT32 height
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("1edd9491-9853-4299-898f-6432983b6f3a") IDWriteGdiInterop : public IUnknown
{








    STDMETHOD(CreateFontFromLOGFONT)(
        __in LOGFONTW const* logFont,
        __out IDWriteFont** font
        ) PURE;











    STDMETHOD(ConvertFontToLOGFONT)(
        IDWriteFont* font,
        __out LOGFONTW* logFont,
        __out BOOL* isSystemFont
        ) PURE;









    STDMETHOD(ConvertFontFaceToLOGFONT)(
        IDWriteFontFace* font,
        __out LOGFONTW* logFont
        ) PURE;











    STDMETHOD(CreateFontFaceFromHdc)(
        HDC hdc,
        __out IDWriteFontFace** fontFace
        ) PURE;








    STDMETHOD(CreateBitmapRenderTarget)(
        __in_opt HDC hdc,
        UINT32 width,
        UINT32 height,
        __out IDWriteBitmapRenderTarget** renderTarget
        ) PURE;
};





enum DWRITE_TEXTURE_TYPE
{




    DWRITE_TEXTURE_ALIASED_1x1,





    DWRITE_TEXTURE_CLEARTYPE_3x1
};




#define DWRITE_ALPHA_MAX 255




interface DWRITE_DECLARE_INTERFACE("7d97dbf7-e085-42d4-81e3-6a883bded118") IDWriteGlyphRunAnalysis : public IUnknown
{











    STDMETHOD(GetAlphaTextureBounds)(
        DWRITE_TEXTURE_TYPE textureType,
        __out RECT* textureBounds
        ) PURE;














    STDMETHOD(CreateAlphaTexture)(
        DWRITE_TEXTURE_TYPE textureType,
        __in RECT const* textureBounds,
        __out_bcount(bufferSize) BYTE* alphaValues,
        UINT32 bufferSize
        ) PURE;










    STDMETHOD(GetAlphaBlendParams)(
        IDWriteRenderingParams* renderingParams,
        __out FLOAT* blendGamma,
        __out FLOAT* blendEnhancedContrast,
        __out FLOAT* blendClearTypeLevel
        ) PURE;
};




interface DWRITE_DECLARE_INTERFACE("b859ee5a-d838-4b5b-a2e8-1adc7d93db48") IDWriteFactory : public IUnknown
{











    STDMETHOD(GetSystemFontCollection)(
        __out IDWriteFontCollection** fontCollection,
        BOOL checkForUpdates = FALSE
        ) PURE;












    STDMETHOD(CreateCustomFontCollection)(
        IDWriteFontCollectionLoader* collectionLoader,
        __in_bcount(collectionKeySize) void const* collectionKey,
        UINT32 collectionKeySize,
        __out IDWriteFontCollection** fontCollection
        ) PURE;








    STDMETHOD(RegisterFontCollectionLoader)(
        IDWriteFontCollectionLoader* fontCollectionLoader
        ) PURE;








    STDMETHOD(UnregisterFontCollectionLoader)(
        IDWriteFontCollectionLoader* fontCollectionLoader
        ) PURE;














    STDMETHOD(CreateFontFileReference)(
        __in_z WCHAR const* filePath,
        __in_opt FILETIME const* lastWriteTime,
        __out IDWriteFontFile** fontFile
        ) PURE;




















    STDMETHOD(CreateCustomFontFileReference)(
        __in_bcount(fontFileReferenceKeySize) void const* fontFileReferenceKey,
        UINT32 fontFileReferenceKeySize,
        IDWriteFontFileLoader* fontFileLoader,
        __out IDWriteFontFile** fontFile
        ) PURE;















    STDMETHOD(CreateFontFace)(
        DWRITE_FONT_FACE_TYPE fontFaceType,
        UINT32 numberOfFiles,
        __in_ecount(numberOfFiles) IDWriteFontFile* const* fontFiles,
        UINT32 faceIndex,
        DWRITE_FONT_SIMULATIONS fontFaceSimulationFlags,
        __out IDWriteFontFace** fontFace
        ) PURE;








    STDMETHOD(CreateRenderingParams)(
        __out IDWriteRenderingParams** renderingParams
        ) PURE;









    STDMETHOD(CreateMonitorRenderingParams)(
        HMONITOR monitor,
        __out IDWriteRenderingParams** renderingParams
        ) PURE;













    STDMETHOD(CreateCustomRenderingParams)(
        FLOAT gamma,
        FLOAT enhancedContrast,
        FLOAT clearTypeLevel,
        DWRITE_PIXEL_GEOMETRY pixelGeometry,
        DWRITE_RENDERING_MODE renderingMode,
        __out IDWriteRenderingParams** renderingParams
        ) PURE;




















    STDMETHOD(RegisterFontFileLoader)(
        IDWriteFontFileLoader* fontFileLoader
        ) PURE;



















    STDMETHOD(UnregisterFontFileLoader)(
        IDWriteFontFileLoader* fontFileLoader
        ) PURE;















    STDMETHOD(CreateTextFormat)(
        __in_z WCHAR const* fontFamilyName,
        __maybenull IDWriteFontCollection* fontCollection,
        DWRITE_FONT_WEIGHT fontWeight,
        DWRITE_FONT_STYLE fontStyle,
        DWRITE_FONT_STRETCH fontStretch,
        FLOAT fontSize,
        __in_z WCHAR const* localeName,
        __out IDWriteTextFormat** textFormat
        ) PURE;








    STDMETHOD(CreateTypography)(
        __out IDWriteTypography** typography
        ) PURE;








    STDMETHOD(GetGdiInterop)(
        __out IDWriteGdiInterop** gdiInterop
        ) PURE;















    STDMETHOD(CreateTextLayout)(
        __in_ecount(stringLength) WCHAR const* string,
        UINT32 stringLength,
        IDWriteTextFormat* textFormat,
        FLOAT maxWidth,
        FLOAT maxHeight,
        __out IDWriteTextLayout** textLayout
        ) PURE;

























    STDMETHOD(CreateGdiCompatibleTextLayout)(
        __in_ecount(stringLength) WCHAR const* string,
        UINT32 stringLength,
        IDWriteTextFormat* textFormat,
        FLOAT layoutWidth,
        FLOAT layoutHeight,
        FLOAT pixelsPerDip,
        __in_opt DWRITE_MATRIX const* transform,
        BOOL useGdiNatural,
        __out IDWriteTextLayout** textLayout
        ) PURE;











    STDMETHOD(CreateEllipsisTrimmingSign)(
        IDWriteTextFormat* textFormat,
        __out IDWriteInlineObject** trimmingSign
        ) PURE;








    STDMETHOD(CreateTextAnalyzer)(
        __out IDWriteTextAnalyzer** textAnalyzer
        ) PURE;










    STDMETHOD(CreateNumberSubstitution)(
        __in DWRITE_NUMBER_SUBSTITUTION_METHOD substitutionMethod,
        __in_z WCHAR const* localeName,
        __in BOOL ignoreUserOverride,
        __out IDWriteNumberSubstitution** numberSubstitution
        ) PURE;



















    STDMETHOD(CreateGlyphRunAnalysis)(
        __in DWRITE_GLYPH_RUN const* glyphRun,
        FLOAT pixelsPerDip,
        __in_opt DWRITE_MATRIX const* transform,
        DWRITE_RENDERING_MODE renderingMode,
        DWRITE_MEASURING_MODE measuringMode,
        FLOAT baselineOriginX,
        FLOAT baselineOriginY,
        __out IDWriteGlyphRunAnalysis** glyphRunAnalysis
        ) PURE;

};




















EXTERN_C HRESULT DWRITE_EXPORT DWriteCreateFactory(
    __in DWRITE_FACTORY_TYPE factoryType,
    __in REFIID iid,
    __out IUnknown **factory
    );


#define FACILITY_DWRITE 0x898
#define DWRITE_ERR_BASE 0x5000
#define MAKE_DWRITE_HR(severity, code) MAKE_HRESULT(severity, FACILITY_DWRITE, (DWRITE_ERR_BASE + code))
#define MAKE_DWRITE_HR_ERR(code) MAKE_DWRITE_HR(SEVERITY_ERROR, code)




#define DWRITE_E_FILEFORMAT             MAKE_DWRITE_HR_ERR(0x000)




#define DWRITE_E_UNEXPECTED             MAKE_DWRITE_HR_ERR(0x001)




#define DWRITE_E_NOFONT                 MAKE_DWRITE_HR_ERR(0x002)





#define DWRITE_E_FILENOTFOUND           MAKE_DWRITE_HR_ERR(0x003)




#define DWRITE_E_FILEACCESS             MAKE_DWRITE_HR_ERR(0x004)




#define DWRITE_E_FONTCOLLECTIONOBSOLETE MAKE_DWRITE_HR_ERR(0x005)




#define DWRITE_E_ALREADYREGISTERED      MAKE_DWRITE_HR_ERR(0x006)

#endif
