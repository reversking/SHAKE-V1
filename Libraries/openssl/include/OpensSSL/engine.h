









#ifndef OPENSSL_ENGINE_H
# define OPENSSL_ENGINE_H
# pragma once

# include <OpensSSL/macros.h>
# ifndef OPENSSL_NO_DEPRECATED_3_0
#  define HEADER_ENGINE_H
# endif

# include <OpensSSL/opensslconf.h>

# ifndef OPENSSL_NO_ENGINE
#  ifndef OPENSSL_NO_DEPRECATED_1_1_0
#   include <OpensSSL/bn.h>
#   include <OpensSSL/rsa.h>
#   include <OpensSSL/dsa.h>
#   include <OpensSSL/dh.h>
#   include <OpensSSL/ec.h>
#   include <OpensSSL/rand.h>
#   include <OpensSSL/ui.h>
#   include <OpensSSL/err.h>
#  endif
#  include <OpensSSL/types.h>
#  include <OpensSSL/symhacks.h>
#  include <OpensSSL/x509.h>
#  include <OpensSSL/engineerr.h>
#  ifdef  __cplusplus
extern "C" {
#  endif





#  define ENGINE_METHOD_RSA               (unsigned int)0x0001
#  define ENGINE_METHOD_DSA               (unsigned int)0x0002
#  define ENGINE_METHOD_DH                (unsigned int)0x0004
#  define ENGINE_METHOD_RAND              (unsigned int)0x0008
#  define ENGINE_METHOD_CIPHERS           (unsigned int)0x0040
#  define ENGINE_METHOD_DIGESTS           (unsigned int)0x0080
#  define ENGINE_METHOD_PKEY_METHS        (unsigned int)0x0200
#  define ENGINE_METHOD_PKEY_ASN1_METHS   (unsigned int)0x0400
#  define ENGINE_METHOD_EC                (unsigned int)0x0800

#  define ENGINE_METHOD_ALL               (unsigned int)0xFFFF
#  define ENGINE_METHOD_NONE              (unsigned int)0x0000







#  define ENGINE_TABLE_FLAG_NOINIT        (unsigned int)0x0001











#  define ENGINE_FLAGS_MANUAL_CMD_CTRL    (int)0x0002











#  define ENGINE_FLAGS_BY_ID_COPY         (int)0x0004







#  define ENGINE_FLAGS_NO_REGISTER_ALL    (int)0x0008














#  define ENGINE_CMD_FLAG_NUMERIC         (unsigned int)0x0001




#  define ENGINE_CMD_FLAG_STRING          (unsigned int)0x0002




#  define ENGINE_CMD_FLAG_NO_INPUT        (unsigned int)0x0004





#  define ENGINE_CMD_FLAG_INTERNAL        (unsigned int)0x0008


















#  define ENGINE_CTRL_SET_LOGSTREAM               1
#  define ENGINE_CTRL_SET_PASSWORD_CALLBACK       2
#  define ENGINE_CTRL_HUP                         3


#  define ENGINE_CTRL_SET_USER_INTERFACE          4
#  define ENGINE_CTRL_SET_CALLBACK_DATA           5



#  define ENGINE_CTRL_LOAD_CONFIGURATION          6



#  define ENGINE_CTRL_LOAD_SECTION                7























#  define ENGINE_CTRL_HAS_CTRL_FUNCTION           10




#  define ENGINE_CTRL_GET_FIRST_CMD_TYPE          11




#  define ENGINE_CTRL_GET_NEXT_CMD_TYPE           12




#  define ENGINE_CTRL_GET_CMD_FROM_NAME           13








#  define ENGINE_CTRL_GET_NAME_LEN_FROM_CMD       14
#  define ENGINE_CTRL_GET_NAME_FROM_CMD           15

#  define ENGINE_CTRL_GET_DESC_LEN_FROM_CMD       16
#  define ENGINE_CTRL_GET_DESC_FROM_CMD           17





#  define ENGINE_CTRL_GET_CMD_FLAGS               18





#  define ENGINE_CMD_BASE                         200










#  define ENGINE_CTRL_CHIL_SET_FORKCHECK          100





#  define ENGINE_CTRL_CHIL_NO_LOCKING             101















typedef struct ENGINE_CMD_DEFN_st {
    unsigned int cmd_num;
    const char *cmd_name;
    const char *cmd_desc;
    unsigned int cmd_flags;
} ENGINE_CMD_DEFN;


typedef int (*ENGINE_GEN_FUNC_PTR) (void);

typedef int (*ENGINE_GEN_INT_FUNC_PTR) (ENGINE *);

typedef int (*ENGINE_CTRL_FUNC_PTR) (ENGINE *, int, long, void *,
                                     void (*f) (void));

typedef EVP_PKEY *(*ENGINE_LOAD_KEY_PTR)(ENGINE *, const char *,
                                         UI_METHOD *ui_method,
                                         void *callback_data);
typedef int (*ENGINE_SSL_CLIENT_CERT_PTR) (ENGINE *, SSL *ssl,
                                           STACK_OF(X509_NAME) *ca_dn,
                                           X509 **pcert, EVP_PKEY **pkey,
                                           STACK_OF(X509) **pother,
                                           UI_METHOD *ui_method,
                                           void *callback_data);















typedef int (*ENGINE_CIPHERS_PTR) (ENGINE *, const EVP_CIPHER **,
                                   const int **, int);
typedef int (*ENGINE_DIGESTS_PTR) (ENGINE *, const EVP_MD **, const int **,
                                   int);
typedef int (*ENGINE_PKEY_METHS_PTR) (ENGINE *, EVP_PKEY_METHOD **,
                                      const int **, int);
typedef int (*ENGINE_PKEY_ASN1_METHS_PTR) (ENGINE *, EVP_PKEY_ASN1_METHOD **,
                                           const int **, int);













#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_first(void);
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_last(void);
#  endif

#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_next(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_prev(ENGINE *e);
#  endif

#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_add(ENGINE *e);
#  endif

#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_remove(ENGINE *e);
#  endif

#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_by_id(const char *id);
#  endif

#  ifndef OPENSSL_NO_DEPRECATED_1_1_0
#   define ENGINE_load_openssl() \
        OPENSSL_init_crypto(OPENSSL_INIT_ENGINE_OPENSSL, NULL)
#   define ENGINE_load_dynamic() \
        OPENSSL_init_crypto(OPENSSL_INIT_ENGINE_DYNAMIC, NULL)
#   ifndef OPENSSL_NO_STATIC_ENGINE
#    define ENGINE_load_padlock() \
        OPENSSL_init_crypto(OPENSSL_INIT_ENGINE_PADLOCK, NULL)
#    define ENGINE_load_capi() \
        OPENSSL_init_crypto(OPENSSL_INIT_ENGINE_CAPI, NULL)
#    define ENGINE_load_afalg() \
        OPENSSL_init_crypto(OPENSSL_INIT_ENGINE_AFALG, NULL)
#   endif
#   define ENGINE_load_cryptodev() \
        OPENSSL_init_crypto(OPENSSL_INIT_ENGINE_CRYPTODEV, NULL)
#   define ENGINE_load_rdrand() \
        OPENSSL_init_crypto(OPENSSL_INIT_ENGINE_RDRAND, NULL)
#  endif
#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 void ENGINE_load_builtin_engines(void);
#  endif





#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 unsigned int ENGINE_get_table_flags(void);
OSSL_DEPRECATEDIN_3_0 void ENGINE_set_table_flags(unsigned int flags);
#  endif








#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_RSA(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_unregister_RSA(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_register_all_RSA(void);
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_DSA(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_unregister_DSA(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_register_all_DSA(void);
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_EC(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_unregister_EC(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_register_all_EC(void);
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_DH(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_unregister_DH(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_register_all_DH(void);
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_RAND(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_unregister_RAND(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_register_all_RAND(void);
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_ciphers(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_unregister_ciphers(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_register_all_ciphers(void);
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_digests(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_unregister_digests(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_register_all_digests(void);
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_pkey_meths(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_unregister_pkey_meths(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_register_all_pkey_meths(void);
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_pkey_asn1_meths(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_unregister_pkey_asn1_meths(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 void ENGINE_register_all_pkey_asn1_meths(void);
#  endif







#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_complete(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_register_all_complete(void);
#  endif











#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_ctrl(ENGINE *e, int cmd, long i, void *p,
                                      void (*f) (void));
#  endif







#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_cmd_is_executable(ENGINE *e, int cmd);
#  endif







#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_ctrl_cmd(ENGINE *e, const char *cmd_name,
                                          long i, void *p, void (*f) (void),
                                          int cmd_optional);
#  endif























#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0
int ENGINE_ctrl_cmd_string(ENGINE *e, const char *cmd_name, const char *arg,
                           int cmd_optional);
#  endif









#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_new(void);
OSSL_DEPRECATEDIN_3_0 int ENGINE_free(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_up_ref(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_id(ENGINE *e, const char *id);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_name(ENGINE *e, const char *name);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_RSA(ENGINE *e, const RSA_METHOD *rsa_meth);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_DSA(ENGINE *e, const DSA_METHOD *dsa_meth);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_EC(ENGINE *e, const EC_KEY_METHOD *ecdsa_meth);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_DH(ENGINE *e, const DH_METHOD *dh_meth);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_RAND(ENGINE *e, const RAND_METHOD *rand_meth);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_destroy_function(ENGINE *e,ENGINE_GEN_INT_FUNC_PTR destroy_f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_init_function(ENGINE *e, ENGINE_GEN_INT_FUNC_PTR init_f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_finish_function(ENGINE *e, ENGINE_GEN_INT_FUNC_PTR finish_f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_ctrl_function(ENGINE *e, ENGINE_CTRL_FUNC_PTR ctrl_f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_load_privkey_function(ENGINE *e, ENGINE_LOAD_KEY_PTR loadpriv_f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_load_pubkey_function(ENGINE *e, ENGINE_LOAD_KEY_PTR loadpub_f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_load_ssl_client_cert_function(ENGINE *e,
                                             ENGINE_SSL_CLIENT_CERT_PTR loadssl_f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_ciphers(ENGINE *e, ENGINE_CIPHERS_PTR f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_digests(ENGINE *e, ENGINE_DIGESTS_PTR f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_pkey_meths(ENGINE *e, ENGINE_PKEY_METHS_PTR f);
OSSL_DEPRECATEDIN_3_0
int ENGINE_set_pkey_asn1_meths(ENGINE *e, ENGINE_PKEY_ASN1_METHS_PTR f);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_flags(ENGINE *e, int flags);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_cmd_defns(ENGINE *e,
                                               const ENGINE_CMD_DEFN *defns);
#  endif

#  define ENGINE_get_ex_new_index(l, p, newf, dupf, freef) \
    CRYPTO_get_ex_new_index(CRYPTO_EX_INDEX_ENGINE, l, p, newf, dupf, freef)
# ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_ex_data(ENGINE *e, int idx, void *arg);
OSSL_DEPRECATEDIN_3_0 void *ENGINE_get_ex_data(const ENGINE *e, int idx);
# endif

#  ifndef OPENSSL_NO_DEPRECATED_1_1_0




#   define ENGINE_cleanup() while(0) continue
#  endif







#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 const char *ENGINE_get_id(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0 const char *ENGINE_get_name(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0 const RSA_METHOD *ENGINE_get_RSA(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0 const DSA_METHOD *ENGINE_get_DSA(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0 const EC_KEY_METHOD *ENGINE_get_EC(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0 const DH_METHOD *ENGINE_get_DH(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0 const RAND_METHOD *ENGINE_get_RAND(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_GEN_INT_FUNC_PTR ENGINE_get_destroy_function(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_GEN_INT_FUNC_PTR ENGINE_get_init_function(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_GEN_INT_FUNC_PTR ENGINE_get_finish_function(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_CTRL_FUNC_PTR ENGINE_get_ctrl_function(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_LOAD_KEY_PTR ENGINE_get_load_privkey_function(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_LOAD_KEY_PTR ENGINE_get_load_pubkey_function(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_SSL_CLIENT_CERT_PTR ENGINE_get_ssl_client_cert_function(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_CIPHERS_PTR ENGINE_get_ciphers(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_DIGESTS_PTR ENGINE_get_digests(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_PKEY_METHS_PTR ENGINE_get_pkey_meths(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
ENGINE_PKEY_ASN1_METHS_PTR ENGINE_get_pkey_asn1_meths(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0
const EVP_CIPHER *ENGINE_get_cipher(ENGINE *e, int nid);
OSSL_DEPRECATEDIN_3_0
const EVP_MD *ENGINE_get_digest(ENGINE *e, int nid);
OSSL_DEPRECATEDIN_3_0
const EVP_PKEY_METHOD *ENGINE_get_pkey_meth(ENGINE *e, int nid);
OSSL_DEPRECATEDIN_3_0
const EVP_PKEY_ASN1_METHOD *ENGINE_get_pkey_asn1_meth(ENGINE *e, int nid);
OSSL_DEPRECATEDIN_3_0
const EVP_PKEY_ASN1_METHOD *ENGINE_get_pkey_asn1_meth_str(ENGINE *e,
                                                          const char *str,
                                                          int len);
OSSL_DEPRECATEDIN_3_0
const EVP_PKEY_ASN1_METHOD *ENGINE_pkey_asn1_find_str(ENGINE **pe,
                                                      const char *str, int len);
OSSL_DEPRECATEDIN_3_0
const ENGINE_CMD_DEFN *ENGINE_get_cmd_defns(const ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_get_flags(const ENGINE *e);
#  endif



















#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_init(ENGINE *e);
#  endif





#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_finish(ENGINE *e);
#  endif






#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0
EVP_PKEY *ENGINE_load_private_key(ENGINE *e, const char *key_id,
                                  UI_METHOD *ui_method, void *callback_data);
OSSL_DEPRECATEDIN_3_0
EVP_PKEY *ENGINE_load_public_key(ENGINE *e, const char *key_id,
                                 UI_METHOD *ui_method, void *callback_data);
OSSL_DEPRECATEDIN_3_0
int ENGINE_load_ssl_client_cert(ENGINE *e, SSL *s, STACK_OF(X509_NAME) *ca_dn,
                                X509 **pcert, EVP_PKEY **ppkey,
                                STACK_OF(X509) **pother,
                                UI_METHOD *ui_method, void *callback_data);
#  endif







#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_default_RSA(void);
#  endif

#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_default_DSA(void);
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_default_EC(void);
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_default_DH(void);
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_default_RAND(void);
#  endif




#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_cipher_engine(int nid);
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_digest_engine(int nid);
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_pkey_meth_engine(int nid);
OSSL_DEPRECATEDIN_3_0 ENGINE *ENGINE_get_pkey_asn1_meth_engine(int nid);
#  endif







#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_RSA(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_string(ENGINE *e,
                                                    const char *def_list);
#  endif

#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_DSA(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_EC(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_DH(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_RAND(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_ciphers(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_digests(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_pkey_meths(ENGINE *e);
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default_pkey_asn1_meths(ENGINE *e);
#  endif








#  ifndef OPENSSL_NO_DEPRECATED_3_0
OSSL_DEPRECATEDIN_3_0 int ENGINE_set_default(ENGINE *e, unsigned int flags);
OSSL_DEPRECATEDIN_3_0 void ENGINE_add_conf_module(void);
#  endif









#  define OSSL_DYNAMIC_VERSION            (unsigned long)0x00030000




#  define OSSL_DYNAMIC_OLDEST             (unsigned long)0x00030000












typedef void *(*dyn_MEM_malloc_fn) (size_t, const char *, int);
typedef void *(*dyn_MEM_realloc_fn) (void *, size_t, const char *, int);
typedef void (*dyn_MEM_free_fn) (void *, const char *, int);
typedef struct st_dynamic_MEM_fns {
    dyn_MEM_malloc_fn malloc_fn;
    dyn_MEM_realloc_fn realloc_fn;
    dyn_MEM_free_fn free_fn;
} dynamic_MEM_fns;





typedef struct st_dynamic_fns {
    void *static_state;
    dynamic_MEM_fns mem_fns;
} dynamic_fns;













typedef unsigned long (*dynamic_v_check_fn) (unsigned long ossl_version);
#  define IMPLEMENT_DYNAMIC_CHECK_FN() \
        OPENSSL_EXPORT unsigned long v_check(unsigned long v); \
        OPENSSL_EXPORT unsigned long v_check(unsigned long v) { \
                if (v >= OSSL_DYNAMIC_OLDEST) return OSSL_DYNAMIC_VERSION; \
                return 0; }



















typedef int (*dynamic_bind_engine) (ENGINE *e, const char *id,
                                    const dynamic_fns *fns);
#  define IMPLEMENT_DYNAMIC_BIND_FN(fn) \
        OPENSSL_EXPORT \
        int bind_engine(ENGINE *e, const char *id, const dynamic_fns *fns); \
        OPENSSL_EXPORT \
        int bind_engine(ENGINE *e, const char *id, const dynamic_fns *fns) { \
            if (ENGINE_get_static_state() == fns->static_state) goto skip_cbs; \
            CRYPTO_set_mem_functions(fns->mem_fns.malloc_fn, \
                                     fns->mem_fns.realloc_fn, \
                                     fns->mem_fns.free_fn); \
            OPENSSL_init_crypto(OPENSSL_INIT_NO_ATEXIT, NULL); \
        skip_cbs: \
            if (!fn(e, id)) return 0; \
            return 1; }












void *ENGINE_get_static_state(void);

#  if defined(__OpenBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
#   ifndef OPENSSL_NO_DEPRECATED_1_1_0
OSSL_DEPRECATEDIN_1_1_0 void ENGINE_setup_bsd_cryptodev(void);
#   endif
#  endif


#  ifdef  __cplusplus
}
#  endif
# endif
#endif
