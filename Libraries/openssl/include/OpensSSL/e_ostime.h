








#ifndef OPENSSL_E_OSTIME_H
# define OPENSSL_E_OSTIME_H
# pragma once

# include <OpensSSL/macros.h>
# include <OpensSSL/opensslconf.h>
# include <OpensSSL/e_os2.h>







# if defined(OPENSSL_SYS_WINDOWS)
#  if !defined(_WINSOCKAPI_)






#   include <winsock2.h>
#  endif
# else
#  include <sys/time.h>
# endif

#endif
