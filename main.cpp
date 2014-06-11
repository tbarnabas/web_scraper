
/////////////////////////////////////////////////////////////////////////////
//
// main.cpp - main source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr.h"

static const char * STATIC_pUsage =
  "usage: wsr\n" \
  "  [ -s | --scrape ] <scrapers> <domains> <emails> <depth> - scrape emails\n" \
  "    <scrapers> : number of parallel downloader thread (default=1)\n" \
  "    <domains> : name of input file of domains (default=\"domains.txt\")\n" \
  "    <emails> : name of output file of emails (default=\"emails.txt\")\n" \
  "    <depth> : downloading depth (default=1)\n" \
  "  [ -h | --help ] - display help\n";

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
          
	    T_ULONG uScrapers = 1;
	    if (iArgc > 2) { 
	      uScrapers = atoi(pArgv[2]);
	    }
	    T_STRING sInput = "domains.txt";
		if (iArgc > 3) { 
	      sInput = pArgv[3];
	    }
	    T_STRING sOutput = "emails.txt";
		if (iArgc > 4) { 
	      sOutput = pArgv[4];
	    }
	    T_ULONG uDepth = 1;
	    if (iArgc > 5) { 
	      uDepth = atoi(pArgv[5]);
	    }
		
	    // create a new application
	    ::WSR::CApplication tApplication(uScrapers, sInput, sOutput, uDepth);
		
		// initialize application
		tApplication.Initialize();

		// run application
		tApplication.Run();

		// shutdown application
		tApplication.Shutdown(true);
    printf("SHUTDOWN\n");
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
