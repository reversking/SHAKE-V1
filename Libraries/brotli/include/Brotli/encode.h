










#ifndef BROTLI_ENC_ENCODE_H_
#define BROTLI_ENC_ENCODE_H_

#include <brotli/port.h>
#include <brotli/shared_dictionary.h>
#include <brotli/types.h>

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


#define BROTLI_MIN_WINDOW_BITS 10





#define BROTLI_MAX_WINDOW_BITS 24




#define BROTLI_LARGE_MAX_WINDOW_BITS 30

#define BROTLI_MIN_INPUT_BLOCK_BITS 16

#define BROTLI_MAX_INPUT_BLOCK_BITS 24

#define BROTLI_MIN_QUALITY 0

#define BROTLI_MAX_QUALITY 11


typedef enum BrotliEncoderMode {






  BROTLI_MODE_GENERIC = 0,

  BROTLI_MODE_TEXT = 1,

  BROTLI_MODE_FONT = 2
} BrotliEncoderMode;


#define BROTLI_DEFAULT_QUALITY 11

#define BROTLI_DEFAULT_WINDOW 22

#define BROTLI_DEFAULT_MODE BROTLI_MODE_GENERIC


typedef enum BrotliEncoderOperation {





  BROTLI_OPERATION_PROCESS = 0,
















  BROTLI_OPERATION_FLUSH = 1,


















  BROTLI_OPERATION_FINISH = 2,




















  BROTLI_OPERATION_EMIT_METADATA = 3
} BrotliEncoderOperation;


typedef enum BrotliEncoderParameter {





  BROTLI_PARAM_MODE = 0,






  BROTLI_PARAM_QUALITY = 1,










  BROTLI_PARAM_LGWIN = 2,













  BROTLI_PARAM_LGBLOCK = 3,





  BROTLI_PARAM_DISABLE_LITERAL_CONTEXT_MODELING = 4,





  BROTLI_PARAM_SIZE_HINT = 5,



  BROTLI_PARAM_LARGE_WINDOW = 6,







  BROTLI_PARAM_NPOSTFIX = 7,







  BROTLI_PARAM_NDIRECT = 8,















  BROTLI_PARAM_STREAM_OFFSET = 9
} BrotliEncoderParameter;







typedef struct BrotliEncoderStateStruct BrotliEncoderState;















BROTLI_ENC_API BROTLI_BOOL BrotliEncoderSetParameter(
    BrotliEncoderState* state, BrotliEncoderParameter param, uint32_t value);















BROTLI_ENC_API BrotliEncoderState* BrotliEncoderCreateInstance(
    brotli_alloc_func alloc_func, brotli_free_func free_func, void* opaque);






BROTLI_ENC_API void BrotliEncoderDestroyInstance(BrotliEncoderState* state);



typedef struct BrotliEncoderPreparedDictionaryStruct
    BrotliEncoderPreparedDictionary;


















BROTLI_ENC_API BrotliEncoderPreparedDictionary*
BrotliEncoderPrepareDictionary(BrotliSharedDictionaryType type,
    size_t data_size, const uint8_t data[BROTLI_ARRAY_PARAM(data_size)],
    int quality,
    brotli_alloc_func alloc_func, brotli_free_func free_func, void* opaque);

BROTLI_ENC_API void BrotliEncoderDestroyPreparedDictionary(
    BrotliEncoderPreparedDictionary* dictionary);











BROTLI_ENC_API BROTLI_BOOL BrotliEncoderAttachPreparedDictionary(
    BrotliEncoderState* state,
    const BrotliEncoderPreparedDictionary* dictionary);











BROTLI_ENC_API size_t BrotliEncoderMaxCompressedSize(size_t input_size);





























BROTLI_ENC_API BROTLI_BOOL BrotliEncoderCompress(
    int quality, int lgwin, BrotliEncoderMode mode, size_t input_size,
    const uint8_t input_buffer[BROTLI_ARRAY_PARAM(input_size)],
    size_t* encoded_size,
    uint8_t encoded_buffer[BROTLI_ARRAY_PARAM(*encoded_size)]);































































BROTLI_ENC_API BROTLI_BOOL BrotliEncoderCompressStream(
    BrotliEncoderState* state, BrotliEncoderOperation op, size_t* available_in,
    const uint8_t** next_in, size_t* available_out, uint8_t** next_out,
    size_t* total_out);









BROTLI_ENC_API BROTLI_BOOL BrotliEncoderIsFinished(BrotliEncoderState* state);








BROTLI_ENC_API BROTLI_BOOL BrotliEncoderHasMoreOutput(
    BrotliEncoderState* state);





























BROTLI_ENC_API const uint8_t* BrotliEncoderTakeOutput(
    BrotliEncoderState* state, size_t* size);



BROTLI_ENC_EXTRA_API size_t BrotliEncoderEstimatePeakMemoryUsage(
    int quality, int lgwin, size_t input_size);

BROTLI_ENC_EXTRA_API size_t BrotliEncoderGetPreparedDictionarySize(
    const BrotliEncoderPreparedDictionary* dictionary);






BROTLI_ENC_API uint32_t BrotliEncoderVersion(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
