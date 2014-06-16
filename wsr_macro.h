
/////////////////////////////////////////////////////////////////////////////
//
// wsr_macro.h - macro header
// --------------------------------------------------------------------------
//
// WEB SCRAPER
//
// MACRO
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WSR_MACRO
#define _WSR_MACRO

#include "wsr_platform.h"

#define __WSR__STATUS_LOG \
  printf("WSR::StatusLog() > -----------------------\n"); \
  printf("WSR::StatusLog() > WORKER   : running=%d, wait=%d, work=%d, down=%d, rec_dom=%d, rec_email=%d\n", ::WSR::CScraper::STATIC_uRunningScrapers, ::WSR::CScraper::STATIC_uWaitingScrapers, ::WSR::CScraper::STATIC_uWorkingScrapers, ::WSR::CScraper::STATIC_uDownloadingScrapers, ::WSR::CScraper::STATIC_uRecognizingDomainScrapers, ::WSR::CScraper::STATIC_uRecognizingEmailScrapers); \
  printf("WSR::StatusLog() > RESULT   : in_dom=%d, rec_dom=%d, rec_email=%d\n", ::WSR::CScraper::STATIC_uEnqueuedDomains, ::WSR::CScraper::STATIC_uRecognizedDomains, ::WSR::CScraper::STATIC_uRecognizedEmails); \
  printf("WSR::StatusLog() > DOWNLOAD : total=%d, access=%d, unreach=%d\n", ::WSR::CScraper::STATIC_uTotalDownloadTry, ::WSR::CScraper::STATIC_uAccessedHomePages, ::WSR::CScraper::STATIC_uUnreachableHomePages); \
  printf("WSR::StatusLog() > NETWORK  : max_tw=%d, curr_tw=%d, limit_tw=%d\n", ::WSR::CScraper::STATIC_uMaximumTIMEWAITSockets, ::WSR::CScraper::STATIC_uCurrentTIMEWAITSockets, ::WSR::CScraper::STATIC_uTIMEWAITSockets); 

#define __WSR__ENQUEUE_DOMAINS(task) \
  m_DomainsProducers->Acquire(); \
  { \
    GUARD __tGuard(m_Domains); \
    m_Domains->Push(task); \
  } \
  m_DomainsConsumers->Release();

#define __WSR__DEQUEUE_DOMAINS(task) \
  m_DomainsConsumers->Acquire(); \
  { \
    GUARD __tGuard(m_Domains); \
    task = m_Domains->Pop(); \
  } \
  m_DomainsProducers->Release();

#endif // #ifndef _WSR_MACRO
