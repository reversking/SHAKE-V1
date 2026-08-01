







#ifndef BROTLI_COMMON_SHARED_DICTIONARY_H_
#define BROTLI_COMMON_SHARED_DICTIONARY_H_

#include <brotli/port.h>
#include <brotli/types.h>

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define SHARED_BROTLI_MIN_DICTIONARY_WORD_LENGTH 4
#define SHARED_BROTLI_MAX_DICTIONARY_WORD_LENGTH 31
#define SHARED_BROTLI_NUM_DICTIONARY_CONTEXTS 64
#define SHARED_BROTLI_MAX_COMPOUND_DICTS 15







typedef struct BrotliSharedDictionaryStruct BrotliSharedDictionary;




typedef enum BrotliSharedDictionaryType {

  BROTLI_SHARED_DICTIONARY_RAW = 0,




  BROTLI_SHARED_DICTIONARY_SERIALIZED = 1
} BrotliSharedDictionaryType;


















BROTLI_COMMON_API BrotliSharedDictionary* BrotliSharedDictionaryCreateInstance(
    brotli_alloc_func alloc_func, brotli_free_func free_func, void* opaque);






BROTLI_COMMON_API void BrotliSharedDictionaryDestroyInstance(
    BrotliSharedDictionary* dict);




















BROTLI_COMMON_API BROTLI_BOOL BrotliSharedDictionaryAttach(
    BrotliSharedDictionary* dict, BrotliSharedDictionaryType type,
    size_t data_size, const uint8_t data[BROTLI_ARRAY_PARAM(data_size)]);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
