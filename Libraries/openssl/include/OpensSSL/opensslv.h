











#ifndef OPENSSL_OPENSSLV_H
# define OPENSSL_OPENSSLV_H
# pragma once

# ifdef  __cplusplus
extern "C" {
# endif










# define OPENSSL_VERSION_MAJOR  3
# define OPENSSL_VERSION_MINOR  5
# define OPENSSL_VERSION_PATCH  0









# define OPENSSL_VERSION_PRE_RELEASE ""


# define OPENSSL_VERSION_BUILD_METADATA ""














# define OPENSSL_SHLIB_VERSION 3






# define OPENSSL_VERSION_PREREQ(maj,min)                                \
    ((OPENSSL_VERSION_MAJOR << 16) + OPENSSL_VERSION_MINOR >= ((maj) << 16) + (min))








# define OPENSSL_VERSION_STR "3.5.0"
# define OPENSSL_FULL_VERSION_STR "3.5.0"






# define OPENSSL_RELEASE_DATE "8 Apr 2025"





# define OPENSSL_VERSION_TEXT "OpenSSL 3.5.0 8 Apr 2025"


# ifdef OPENSSL_VERSION_PRE_RELEASE
#  define _OPENSSL_VERSION_PRE_RELEASE 0x0L
# else
#  define _OPENSSL_VERSION_PRE_RELEASE 0xfL
# endif
# define OPENSSL_VERSION_NUMBER          \
    ( (OPENSSL_VERSION_MAJOR<<28)        \
      |(OPENSSL_VERSION_MINOR<<20)       \
      |(OPENSSL_VERSION_PATCH<<4)        \
      |_OPENSSL_VERSION_PRE_RELEASE )

# ifdef  __cplusplus
}
# endif

# include <OpensSSL/macros.h>
# ifndef OPENSSL_NO_DEPRECATED_3_0
#  define HEADER_OPENSSLV_H
# endif

#endif
