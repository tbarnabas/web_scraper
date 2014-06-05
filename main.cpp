
/////////////////////////////////////////////////////////////////////////////
//
// main.cpp - main source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr.h"

static const char * STATIC_pUsage =
  "usage: wsr\n" \
  "  -s, --scrape - scrape contents\n" \
  "  -h, --help - display help\n";

/////////////////////////////////////////////////////////////////////////////
__T_INT main(__T_INT iArgc, __T_CHAR * * pArgv) {
  __T_INT iResult = 0;

  try {
    // create running environment
    ::WSR::Create();

    // initialize running environment
    ::WSR::Initialize();

    // parse command line
    if (iArgc != 1) {
      if (T_STRING(pArgv[1]) == "-s") {
        ;
      } else if ((T_STRING(pArgv[1]) == "-h") || (T_STRING(pArgv[1]) == "--help")) {
        printf(::STATIC_pUsage);
      }
    } else {
      printf(::STATIC_pUsage);
    }

    // shutdown running environment
    ::WSR::Shutdown(true);

    // destroy running environment
    ::WSR::Destroy();
  } catch (::BASE::EException & tException) {
    printf("EXCEPTION: %d, %s\n", tException.GetCode(), C_STR(tException.GetMessage()));
  } catch (...) {
    printf("FATAL ERROR\n");
  }

  return (iResult);
} // main
