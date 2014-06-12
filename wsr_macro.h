
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

#define __ENQUEUE_DOMAINS(task) \
  m_DomainsProducers->Acquire(); \
  { \
    GUARD __tGuard(m_Domains); \
    m_Domains->Push(task); \
  } \
  m_DomainsConsumers->Release();

#define __DEQUEUE_DOMAINS(task) \
  m_DomainsConsumers->Acquire(); \
  { \
    GUARD __tGuard(m_Domains); \
    task = m_Domains->Pop(); \
  } \
  m_DomainsProducers->Release();

#define __ENQUEUE_EMAILS(task) \
  m_EmailsProducers->Acquire(); \
  { \
    GUARD __tGuard(m_Emails); \
    m_Emails->Push(task); \
  } \
  m_EmailsConsumers->Release();

#define __DEQUEUE_EMAILS(task) \
  m_EmailsConsumers->Acquire(); \
  { \
    GUARD __tGuard(m_Emails); \
    task = m_Emails->Pop(); \
  } \
  m_EmailsProducers->Release();

#endif // #ifndef _WSR_MACRO
