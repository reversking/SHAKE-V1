








#ifndef __D3D10SHADER_H__
#define __D3D10SHADER_H__

#include "d3d10.h"








#define D3D10_TX_VERSION(_Major,_Minor) (('T' << 24) | ('X' << 16) | ((_Major) << 8) | (_Minor))
































































#define D3D10_SHADER_DEBUG                          (1 << 0)
#define D3D10_SHADER_SKIP_VALIDATION                (1 << 1)
#define D3D10_SHADER_SKIP_OPTIMIZATION              (1 << 2)
#define D3D10_SHADER_PACK_MATRIX_ROW_MAJOR          (1 << 3)
#define D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR       (1 << 4)
#define D3D10_SHADER_PARTIAL_PRECISION              (1 << 5)
#define D3D10_SHADER_FORCE_VS_SOFTWARE_NO_OPT       (1 << 6)
#define D3D10_SHADER_FORCE_PS_SOFTWARE_NO_OPT       (1 << 7)
#define D3D10_SHADER_NO_PRESHADER                   (1 << 8)
#define D3D10_SHADER_AVOID_FLOW_CONTROL             (1 << 9)
#define D3D10_SHADER_PREFER_FLOW_CONTROL            (1 << 10)
#define D3D10_SHADER_ENABLE_STRICTNESS              (1 << 11)
#define D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY (1 << 12)
#define D3D10_SHADER_IEEE_STRICTNESS                (1 << 13)
#define D3D10_SHADER_WARNINGS_ARE_ERRORS            (1 << 18)



#define D3D10_SHADER_OPTIMIZATION_LEVEL0            (1 << 14)
#define D3D10_SHADER_OPTIMIZATION_LEVEL1            0
#define D3D10_SHADER_OPTIMIZATION_LEVEL2            ((1 << 14) | (1 << 15))
#define D3D10_SHADER_OPTIMIZATION_LEVEL3            (1 << 15)




typedef D3D_SHADER_MACRO D3D10_SHADER_MACRO;
typedef D3D10_SHADER_MACRO* LPD3D10_SHADER_MACRO;


typedef D3D_SHADER_VARIABLE_CLASS D3D10_SHADER_VARIABLE_CLASS;
typedef D3D10_SHADER_VARIABLE_CLASS* LPD3D10_SHADER_VARIABLE_CLASS;

typedef D3D_SHADER_VARIABLE_FLAGS D3D10_SHADER_VARIABLE_FLAGS;
typedef D3D10_SHADER_VARIABLE_FLAGS* LPD3D10_SHADER_VARIABLE_FLAGS;

typedef D3D_SHADER_VARIABLE_TYPE D3D10_SHADER_VARIABLE_TYPE;
typedef D3D10_SHADER_VARIABLE_TYPE* LPD3D10_SHADER_VARIABLE_TYPE;

typedef D3D_SHADER_INPUT_FLAGS D3D10_SHADER_INPUT_FLAGS;
typedef D3D10_SHADER_INPUT_FLAGS* LPD3D10_SHADER_INPUT_FLAGS;

typedef D3D_SHADER_INPUT_TYPE D3D10_SHADER_INPUT_TYPE;
typedef D3D10_SHADER_INPUT_TYPE* LPD3D10_SHADER_INPUT_TYPE;

typedef D3D_SHADER_CBUFFER_FLAGS D3D10_SHADER_CBUFFER_FLAGS;
typedef D3D10_SHADER_CBUFFER_FLAGS* LPD3D10_SHADER_CBUFFER_FLAGS;

typedef D3D_CBUFFER_TYPE D3D10_CBUFFER_TYPE;
typedef D3D10_CBUFFER_TYPE* LPD3D10_CBUFFER_TYPE;

typedef D3D_NAME D3D10_NAME;

typedef D3D_RESOURCE_RETURN_TYPE D3D10_RESOURCE_RETURN_TYPE;

typedef D3D_REGISTER_COMPONENT_TYPE D3D10_REGISTER_COMPONENT_TYPE;

typedef D3D_INCLUDE_TYPE D3D10_INCLUDE_TYPE;


typedef interface ID3DInclude ID3D10Include;
typedef interface ID3DInclude* LPD3D10INCLUDE;
#define IID_ID3D10Include IID_ID3DInclude










typedef struct _D3D10_SHADER_DESC
{
    UINT                    Version;
    LPCSTR                  Creator;
    UINT                    Flags;

    UINT                    ConstantBuffers;
    UINT                    BoundResources;
    UINT                    InputParameters;
    UINT                    OutputParameters;

    UINT                    InstructionCount;
    UINT                    TempRegisterCount;
    UINT                    TempArrayCount;
    UINT                    DefCount;
    UINT                    DclCount;
    UINT                    TextureNormalInstructions;
    UINT                    TextureLoadInstructions;
    UINT                    TextureCompInstructions;
    UINT                    TextureBiasInstructions;
    UINT                    TextureGradientInstructions;
    UINT                    FloatInstructionCount;
    UINT                    IntInstructionCount;
    UINT                    UintInstructionCount;
    UINT                    StaticFlowControlCount;
    UINT                    DynamicFlowControlCount;
    UINT                    MacroInstructionCount;
    UINT                    ArrayInstructionCount;
    UINT                    CutInstructionCount;
    UINT                    EmitInstructionCount;
    D3D10_PRIMITIVE_TOPOLOGY GSOutputTopology;
    UINT                    GSMaxOutputVertexCount;
} D3D10_SHADER_DESC;

typedef struct _D3D10_SHADER_BUFFER_DESC
{
    LPCSTR                  Name;
    D3D10_CBUFFER_TYPE      Type;
    UINT                    Variables;
    UINT                    Size;
    UINT                    uFlags;
} D3D10_SHADER_BUFFER_DESC;

typedef struct _D3D10_SHADER_VARIABLE_DESC
{
    LPCSTR                  Name;
    UINT                    StartOffset;
    UINT                    Size;
    UINT                    uFlags;
    LPVOID                  DefaultValue;
} D3D10_SHADER_VARIABLE_DESC;

typedef struct _D3D10_SHADER_TYPE_DESC
{
    D3D10_SHADER_VARIABLE_CLASS Class;
    D3D10_SHADER_VARIABLE_TYPE  Type;
    UINT                        Rows;
    UINT                        Columns;
    UINT                        Elements;
    UINT                        Members;
    UINT                        Offset;
} D3D10_SHADER_TYPE_DESC;

typedef struct _D3D10_SHADER_INPUT_BIND_DESC
{
    LPCSTR                      Name;
    D3D10_SHADER_INPUT_TYPE     Type;
    UINT                        BindPoint;
    UINT                        BindCount;

    UINT                        uFlags;
    D3D10_RESOURCE_RETURN_TYPE  ReturnType;
    D3D10_SRV_DIMENSION         Dimension;
    UINT                        NumSamples;
} D3D10_SHADER_INPUT_BIND_DESC;

typedef struct _D3D10_SIGNATURE_PARAMETER_DESC
{
    LPCSTR                      SemanticName;
    UINT                        SemanticIndex;
    UINT                        Register;
    D3D10_NAME                  SystemValueType;
    D3D10_REGISTER_COMPONENT_TYPE ComponentType;
    BYTE                        Mask;

    BYTE                        ReadWriteMask;




} D3D10_SIGNATURE_PARAMETER_DESC;






typedef interface ID3D10ShaderReflectionType ID3D10ShaderReflectionType;
typedef interface ID3D10ShaderReflectionType *LPD3D10SHADERREFLECTIONTYPE;


DEFINE_GUID(IID_ID3D10ShaderReflectionType,
0xc530ad7d, 0x9b16, 0x4395, 0xa9, 0x79, 0xba, 0x2e, 0xcf, 0xf8, 0x3a, 0xdd);

#undef INTERFACE
#define INTERFACE ID3D10ShaderReflectionType

DECLARE_INTERFACE(ID3D10ShaderReflectionType)
{
    STDMETHOD(GetDesc)(THIS_ D3D10_SHADER_TYPE_DESC *pDesc) PURE;

    STDMETHOD_(ID3D10ShaderReflectionType*, GetMemberTypeByIndex)(THIS_ UINT Index) PURE;
    STDMETHOD_(ID3D10ShaderReflectionType*, GetMemberTypeByName)(THIS_ LPCSTR Name) PURE;
    STDMETHOD_(LPCSTR, GetMemberTypeName)(THIS_ UINT Index) PURE;
};

typedef interface ID3D10ShaderReflectionVariable ID3D10ShaderReflectionVariable;
typedef interface ID3D10ShaderReflectionVariable *LPD3D10SHADERREFLECTIONVARIABLE;


DEFINE_GUID(IID_ID3D10ShaderReflectionVariable,
0x1bf63c95, 0x2650, 0x405d, 0x99, 0xc1, 0x36, 0x36, 0xbd, 0x1d, 0xa0, 0xa1);

#undef INTERFACE
#define INTERFACE ID3D10ShaderReflectionVariable

DECLARE_INTERFACE(ID3D10ShaderReflectionVariable)
{
    STDMETHOD(GetDesc)(THIS_ D3D10_SHADER_VARIABLE_DESC *pDesc) PURE;

    STDMETHOD_(ID3D10ShaderReflectionType*, GetType)(THIS) PURE;
};

typedef interface ID3D10ShaderReflectionConstantBuffer ID3D10ShaderReflectionConstantBuffer;
typedef interface ID3D10ShaderReflectionConstantBuffer *LPD3D10SHADERREFLECTIONCONSTANTBUFFER;


DEFINE_GUID(IID_ID3D10ShaderReflectionConstantBuffer,
0x66c66a94, 0xdddd, 0x4b62, 0xa6, 0x6a, 0xf0, 0xda, 0x33, 0xc2, 0xb4, 0xd0);

#undef INTERFACE
#define INTERFACE ID3D10ShaderReflectionConstantBuffer

DECLARE_INTERFACE(ID3D10ShaderReflectionConstantBuffer)
{
    STDMETHOD(GetDesc)(THIS_ D3D10_SHADER_BUFFER_DESC *pDesc) PURE;

    STDMETHOD_(ID3D10ShaderReflectionVariable*, GetVariableByIndex)(THIS_ UINT Index) PURE;
    STDMETHOD_(ID3D10ShaderReflectionVariable*, GetVariableByName)(THIS_ LPCSTR Name) PURE;
};

typedef interface ID3D10ShaderReflection ID3D10ShaderReflection;
typedef interface ID3D10ShaderReflection *LPD3D10SHADERREFLECTION;


DEFINE_GUID(IID_ID3D10ShaderReflection,
0xd40e20b6, 0xf8f7, 0x42ad, 0xab, 0x20, 0x4b, 0xaf, 0x8f, 0x15, 0xdf, 0xaa);

#undef INTERFACE
#define INTERFACE ID3D10ShaderReflection

DECLARE_INTERFACE_(ID3D10ShaderReflection, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    STDMETHOD(GetDesc)(THIS_ D3D10_SHADER_DESC *pDesc) PURE;

    STDMETHOD_(ID3D10ShaderReflectionConstantBuffer*, GetConstantBufferByIndex)(THIS_ UINT Index) PURE;
    STDMETHOD_(ID3D10ShaderReflectionConstantBuffer*, GetConstantBufferByName)(THIS_ LPCSTR Name) PURE;

    STDMETHOD(GetResourceBindingDesc)(THIS_ UINT ResourceIndex, D3D10_SHADER_INPUT_BIND_DESC *pDesc) PURE;

    STDMETHOD(GetInputParameterDesc)(THIS_ UINT ParameterIndex, D3D10_SIGNATURE_PARAMETER_DESC *pDesc) PURE;
    STDMETHOD(GetOutputParameterDesc)(THIS_ UINT ParameterIndex, D3D10_SIGNATURE_PARAMETER_DESC *pDesc) PURE;

};





#ifdef __cplusplus
extern "C" {
#endif









































HRESULT WINAPI D3D10CompileShader(LPCSTR pSrcData, SIZE_T SrcDataLen, LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude,
    LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs);



















HRESULT WINAPI D3D10DisassembleShader(CONST void *pShader, SIZE_T BytecodeLength, BOOL EnableColorCode, LPCSTR pComments, ID3D10Blob** ppDisassembly);












LPCSTR WINAPI D3D10GetPixelShaderProfile(ID3D10Device *pDevice);

LPCSTR WINAPI D3D10GetVertexShaderProfile(ID3D10Device *pDevice);

LPCSTR WINAPI D3D10GetGeometryShaderProfile(ID3D10Device *pDevice);



















HRESULT WINAPI D3D10ReflectShader(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10ShaderReflection **ppReflector);





























HRESULT WINAPI D3D10PreprocessShader(LPCSTR pSrcData, SIZE_T SrcDataSize, LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines,
    LPD3D10INCLUDE pInclude, ID3D10Blob** ppShaderText, ID3D10Blob** ppErrorMsgs);






























HRESULT WINAPI D3D10GetInputSignatureBlob(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob);
HRESULT WINAPI D3D10GetOutputSignatureBlob(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob);
HRESULT WINAPI D3D10GetInputAndOutputSignatureBlob(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob);

















HRESULT WINAPI D3D10GetShaderDebugInfo(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob** ppDebugInfo);

#ifdef __cplusplus
}
#endif

#endif

