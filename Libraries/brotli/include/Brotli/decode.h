










#ifndef BROTLI_DEC_DECODE_H_
#define BROTLI_DEC_DECODE_H_

#include <brotli/port.h>
#include <brotli/shared_dictionary.h>
#include <brotli/types.h>

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif







typedef struct BrotliDecoderStateStruct BrotliDecoderState;





typedef enum {

  BROTLI_DECODER_RESULT_ERROR = 0,

  BROTLI_DECODER_RESULT_SUCCESS = 1,

  BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT = 2,

  BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT = 3
} BrotliDecoderResult;



















#define BROTLI_DECODER_ERROR_CODES_LIST(BROTLI_ERROR_CODE, SEPARATOR)      \
  BROTLI_ERROR_CODE(_, NO_ERROR, 0) SEPARATOR                              \
                                   \
  BROTLI_ERROR_CODE(_, SUCCESS, 1) SEPARATOR                               \
  BROTLI_ERROR_CODE(_, NEEDS_MORE_INPUT, 2) SEPARATOR                      \
  BROTLI_ERROR_CODE(_, NEEDS_MORE_OUTPUT, 3) SEPARATOR                     \
                                                                           \
                                       \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, EXUBERANT_NIBBLE, -1) SEPARATOR        \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, RESERVED, -2) SEPARATOR                \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, EXUBERANT_META_NIBBLE, -3) SEPARATOR   \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, SIMPLE_HUFFMAN_ALPHABET, -4) SEPARATOR \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, SIMPLE_HUFFMAN_SAME, -5) SEPARATOR     \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, CL_SPACE, -6) SEPARATOR                \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, HUFFMAN_SPACE, -7) SEPARATOR           \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, CONTEXT_MAP_REPEAT, -8) SEPARATOR      \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, BLOCK_LENGTH_1, -9) SEPARATOR          \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, BLOCK_LENGTH_2, -10) SEPARATOR         \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, TRANSFORM, -11) SEPARATOR              \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, DICTIONARY, -12) SEPARATOR             \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, WINDOW_BITS, -13) SEPARATOR            \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, PADDING_1, -14) SEPARATOR              \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, PADDING_2, -15) SEPARATOR              \
  BROTLI_ERROR_CODE(_ERROR_FORMAT_, DISTANCE, -16) SEPARATOR               \
                                                                           \
                                                 \
                                                                           \
  BROTLI_ERROR_CODE(_ERROR_, COMPOUND_DICTIONARY, -18) SEPARATOR           \
  BROTLI_ERROR_CODE(_ERROR_, DICTIONARY_NOT_SET, -19) SEPARATOR            \
  BROTLI_ERROR_CODE(_ERROR_, INVALID_ARGUMENTS, -20) SEPARATOR             \
                                                                           \
                                           \
  BROTLI_ERROR_CODE(_ERROR_ALLOC_, CONTEXT_MODES, -21) SEPARATOR           \
                          \
  BROTLI_ERROR_CODE(_ERROR_ALLOC_, TREE_GROUPS, -22) SEPARATOR             \
                 \
  BROTLI_ERROR_CODE(_ERROR_ALLOC_, CONTEXT_MAP, -25) SEPARATOR             \
  BROTLI_ERROR_CODE(_ERROR_ALLOC_, RING_BUFFER_1, -26) SEPARATOR           \
  BROTLI_ERROR_CODE(_ERROR_ALLOC_, RING_BUFFER_2, -27) SEPARATOR           \
       \
  BROTLI_ERROR_CODE(_ERROR_ALLOC_, BLOCK_TYPE_TREES, -30) SEPARATOR        \
                                                                           \
                                                  \
  BROTLI_ERROR_CODE(_ERROR_, UNREACHABLE, -31)






typedef enum {
#define BROTLI_COMMA_ ,
#define BROTLI_ERROR_CODE_ENUM_ITEM_(PREFIX, NAME, CODE) \
    BROTLI_DECODER ## PREFIX ## NAME = CODE
  BROTLI_DECODER_ERROR_CODES_LIST(BROTLI_ERROR_CODE_ENUM_ITEM_, BROTLI_COMMA_)
} BrotliDecoderErrorCode;
#undef BROTLI_ERROR_CODE_ENUM_ITEM_
#undef BROTLI_COMMA_








#define BROTLI_LAST_ERROR_CODE BROTLI_DECODER_ERROR_UNREACHABLE


typedef enum BrotliDecoderParameter {






  BROTLI_DECODER_PARAM_DISABLE_RING_BUFFER_REALLOCATION = 0,



  BROTLI_DECODER_PARAM_LARGE_WINDOW = 1
} BrotliDecoderParameter;










BROTLI_DEC_API BROTLI_BOOL BrotliDecoderSetParameter(
    BrotliDecoderState* state, BrotliDecoderParameter param, uint32_t value);



















BROTLI_DEC_API BROTLI_BOOL BrotliDecoderAttachDictionary(
    BrotliDecoderState* state, BrotliSharedDictionaryType type,
    size_t data_size, const uint8_t data[BROTLI_ARRAY_PARAM(data_size)]);



















BROTLI_DEC_API BrotliDecoderState* BrotliDecoderCreateInstance(
    brotli_alloc_func alloc_func, brotli_free_func free_func, void* opaque);






BROTLI_DEC_API void BrotliDecoderDestroyInstance(BrotliDecoderState* state);


















BROTLI_DEC_API BrotliDecoderResult BrotliDecoderDecompress(
    size_t encoded_size,
    const uint8_t encoded_buffer[BROTLI_ARRAY_PARAM(encoded_size)],
    size_t* decoded_size,
    uint8_t decoded_buffer[BROTLI_ARRAY_PARAM(*decoded_size)]);







































BROTLI_DEC_API BrotliDecoderResult BrotliDecoderDecompressStream(
  BrotliDecoderState* state, size_t* available_in, const uint8_t** next_in,
  size_t* available_out, uint8_t** next_out, size_t* total_out);








BROTLI_DEC_API BROTLI_BOOL BrotliDecoderHasMoreOutput(
    const BrotliDecoderState* state);





























BROTLI_DEC_API const uint8_t* BrotliDecoderTakeOutput(
    BrotliDecoderState* state, size_t* size);











BROTLI_DEC_API BROTLI_BOOL BrotliDecoderIsUsed(const BrotliDecoderState* state);









BROTLI_DEC_API BROTLI_BOOL BrotliDecoderIsFinished(
    const BrotliDecoderState* state);












BROTLI_DEC_API BrotliDecoderErrorCode BrotliDecoderGetErrorCode(
    const BrotliDecoderState* state);




BROTLI_DEC_API const char* BrotliDecoderErrorString(BrotliDecoderErrorCode c);






BROTLI_DEC_API uint32_t BrotliDecoderVersion(void);











typedef void (*brotli_decoder_metadata_start_func)(void* opaque, size_t size);













typedef void (*brotli_decoder_metadata_chunk_func)(void* opaque,
                                                   const uint8_t* data,
                                                   size_t size);









BROTLI_DEC_API void BrotliDecoderSetMetadataCallbacks(
    BrotliDecoderState* state,
    brotli_decoder_metadata_start_func start_func,
    brotli_decoder_metadata_chunk_func chunk_func, void* opaque);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
