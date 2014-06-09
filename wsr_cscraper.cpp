
/////////////////////////////////////////////////////////////////////////////
//
// wsr_cscraper.cpp - ::WSR::CScraper class source
//
/////////////////////////////////////////////////////////////////////////////

#include "wsr_cscraper.h"

namespace WSR {
/*
/////////////////////////////////////////////////////////////////////////////
T_ULONG CScraper::Modular(T_ULONG uBase, T_ULONG uExponent, T_ULONG uDivisor) {
  T_ULONG uResult = 1;
  T_ULONG c = 0;
  T_ULONG i = 0;

  for (i = 32; i > 0; i--) {
    c = c << 1;
    uResult = (uResult * uResult) % uDivisor;
    if ((uExponent & (1 << (i - 1))) != 0) {
      c = c + 1;
      uResult = (uResult * uBase) % uDivisor;
    }
  }

  return (uResult);
} // Modular


/////////////////////////////////////////////////////////////////////////////
#define __WSR__CSCraper__RabinKarp__DIVISOR 1031

T_BOOLEAN CScraper::RabinKarp(const T_BYTE * pBuffer, T_ULONG uBufferSize, const T_BYTE * pPattern, T_ULONG uPatternSize, T_ULONG & uIndex) {
  T_BOOLEAN bResult = false;
  uIndex = 0;
  
  T_LONG iFactor = Modular(256, uPatternSize - 1, __WSR__CSCraper__RabinKarp__DIVISOR);
  T_LONG iBufferValue = 0;
  T_LONG iPatternValue = 0;

  if (uPatternSize <= uBufferSize) {
    for (uIndex = 0; uIndex < uPatternSize; uIndex++) {
      iBufferValue = ((256 * iBufferValue) + (T_LONG)pBuffer[uIndex]) % __WSR__CSCraper__RabinKarp__DIVISOR;
      iPatternValue = ((256 * iPatternValue) + (T_LONG)pPattern[uIndex]) % __WSR__CSCraper__RabinKarp__DIVISOR;
    }
  }

  uIndex = 0;
  while ((bResult == false) && (uIndex < (uBufferSize - uPatternSize + 1))) {
    if (iBufferValue == iPatternValue) {
      T_ULONG j = 0;
      bResult = true;
    
      while ((bResult == true) && (j < uPatternSize)) {
        if (pBuffer[uIndex + j] != pPattern[j]) {
          bResult = false;
        } else {
          j = j + 1;
        }
      }
    }
    if ((bResult == false) && (uIndex < (uBufferSize - uPatternSize + 1))) {
      iBufferValue = ((256 * (iBufferValue - (iFactor * (T_LONG)pBuffer[uIndex]))) + (T_LONG)pBuffer[uIndex + uPatternSize]) % __WSR__CSCraper__RabinKarp__DIVISOR;
      if (iBufferValue < 0) {
        iBufferValue = iBufferValue + __WSR__CSCraper__RabinKarp__DIVISOR;
      }
      uIndex = uIndex + 1;
    }
  }
  
  return (bResult);
} // RabinKarp


/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CQueue<T_STRING> > CScraper::RecognizeStringContents(const T_CHAR * pBuffer, T_ULONG uBufferSize, ::DATASTRUCTURE::CQueue<T_STRING> * tPatterns) {
  REFERENCE< ::DATASTRUCTURE::CQueue<T_STRING> > tResult;
  T_ULONG uPosition1 = 0;
  T_ULONG uPosition2 = 0;
  T_ULONG i = 0;
  T_ULONG bMatch = true;

  // create a new array
  tResult.Create(new ::DATASTRUCTURE::CQueue<T_STRING>());

  while (bMatch == true) {
    ::DATASTRUCTURE::CQueue<T_STRING> tContents;

    // match patterns
    T_ULONG j = 0;

    while ((bMatch == true) && (tPatterns->IsEmpty() == false)) {
      T_STRING sPattern = tPatterns->Pop();

      if (sPattern.GetLength() > 0) {
        T_ULONG uPosition = 0;
        bMatch = RabinKarp((T_BYTE *)(pBuffer + uPosition2), uBufferSize - uPosition2, (T_BYTE *)C_STR(sPattern), sPattern.GetLength(), uPosition);
        if (bMatch == true) {
          uPosition2 = uPosition2 + uPosition;
          if ((uPosition1 < uPosition2) && (j > 0)) {
            REFERENCE< ::DATASTRUCTURE::CArray<T_CHAR> > tContent;

            tContent.Create(new ::DATASTRUCTURE::CArray<T_CHAR>(pBuffer + uPosition1, uPosition2 - uPosition1));
            tContent->SetSize(tContent->GetSize() + 1, true);
            tContent->operator[](tContent->GetSize() - 1) = '\0';

            tContents.Push(T_STRING(tContent->GetElements()));
          }

          uPosition2 = uPosition2 + sPattern.GetLength();
          uPosition1 = uPosition2;
        }

        j = j + 1;
      }
    }

    // push contents
    while (tContents.IsEmpty() == false) {
      tResult->Push(tContents.Pop());
    }

    i = i + 1;
  }

  return (tResult);
} // RecognizeStringContents
*/

/////////////////////////////////////////////////////////////////////////////
size_t CScraper::ReceiveData(void * pContents, size_t uSize, size_t uLength, void * pData) {
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tArray = static_cast< ::DATASTRUCTURE::CArray<T_BYTE> *>(pData);
  T_ULONG uOriginalSize = tArray->GetSize();
  T_ULONG uExtentSize = uSize * uLength;
  
  tArray->Append(NULL, uExtentSize + 1);

  ::DATASTRUCTURE::CArray<T_BYTE>::Copy((T_BYTE *)pContents, tArray->GetElements() + uOriginalSize, uExtentSize); 
  
  return (uExtentSize);
} // ReceiveData


/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > CScraper::DownloadHomePage(const T_STRING & sAddress) {
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tResult;
  T_STRING sDomain = sAddress;
  
  if (sDomain.Find("http") == false) {
    sDomain = T_STRING("http://") + sDomain;
  }
  
  CURL * pCURL= NULL;
  CURLcode tCURLResult;
  
  // create a new array
  tResult.Create(new ::DATASTRUCTURE::CArray<T_BYTE>());
  
  // initialize CURL session
  pCURL = curl_easy_init();
  if (pCURL != NULL) {
    // set CURL options
    curl_easy_setopt(pCURL, CURLOPT_URL, C_STR(sDomain));
    curl_easy_setopt(pCURL, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(pCURL, CURLOPT_USERAGENT, "wsr-agent/1.0");

    curl_easy_setopt(pCURL, CURLOPT_WRITEFUNCTION, ReceiveData);    
    curl_easy_setopt(pCURL, CURLOPT_WRITEDATA, (void *)tResult.__ptr());
    
    // perform CURL operation
    tCURLResult = curl_easy_perform(pCURL);
    
    // clean up CURL session
    curl_easy_cleanup(pCURL);
    
    if (tCURLResult != CURLE_OK) {
      EXCEPTION(WSR, ::WSR::CScraper, DownloadHomePage,
      MESSAGE("unable to download home page"));
      THROW(EScraper, UNABLE_TO_DOWNLOAD_HOME_PAGE);
    }
  }
  
  return (tResult);
} // DownloadHomePage

  
/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > CScraper::RecognizeEmails(::DATASTRUCTURE::CArray<T_BYTE> * pPage) {
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tResult;

  // create a new queue
  tResult.Create(new ::DATASTRUCTURE::CQueue<REFERENCE<CTask> >());
  
  regex_t tEmail;
  
  // compile regular expression
  if (regcomp(&tEmail, "[a-z][a-z0-9_]*@[a-z][a-z0-9_]*[.[a-z][a-z0-9_]*]*", REG_ICASE | REG_EXTENDED) == 0) { 
    regmatch_t tMatch;
    
    T_BOOLEAN bMatch = true;
    T_ULONG uPosition = 0;
    T_CHAR pBuffer[256];
    
    while (bMatch == true) {
      if (regexec(&tEmail, (T_CHAR *)pPage->GetElements() + uPosition, 1, &tMatch, 0) == 0) {
        T_ULONG uLength = tMatch.rm_eo - tMatch.rm_so + 1;
        if (uLength > sizeof(pBuffer) - 1) {
          uLength = sizeof(pBuffer) - 1;
        }
        snprintf(pBuffer, uLength, "%s", pPage->GetElements() + uPosition + tMatch.rm_so);
        pBuffer[uLength] = '\0';
        tResult->Push(REFERENCE<CTask>().Create(new CTask(pBuffer, 0)));
        uPosition = uPosition + tMatch.rm_eo;
      } else {
        bMatch = false;
      }
    }
  
    // free regular expression
    regfree(&tEmail);
  }
  
  return (tResult);
} // RecognizeEmails


/////////////////////////////////////////////////////////////////////////////
REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > CScraper::RecognizeDomains(::DATASTRUCTURE::CArray<T_BYTE> * pPage) {
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tResult;
  
  // create a new queue
  tResult.Create(new ::DATASTRUCTURE::CQueue<REFERENCE<CTask> >());
  
  
  return (tResult);
} // RecognizeDomains


/////////////////////////////////////////////////////////////////////////////
void CScraper::Loop() {
  REFERENCE<CTask> tDomain;
  REFERENCE< ::DATASTRUCTURE::CArray<T_BYTE> > tPage;
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tEmails;
  REFERENCE< ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > > tDomains;
  
  // dequeue domain
  m_DomainsConsumers->Acquire();
  {
    GUARD __tGuard(m_Domains);
    tDomain = m_Domains->Pop();
  }
  m_DomainsProducers->Release();
  
  // download home page
  tPage = DownloadHomePage(tDomain->GetAddress());

//  ::BASE::Dump(tPage);
  
  printf("CScraper::Loop() > page (address=%s) downloaded\n", C_STR(tDomain->GetAddress()));

  // recognize emails
  tEmails = RecognizeEmails(tPage);

  printf("CScraper::Loop() > %d email(s) recognized\n", tEmails->GetSize());

  // enqueue emails
  while (tEmails->IsEmpty() == false) {
    m_EmailsProducers->Acquire();
    {
      GUARD __tGuard(m_Emails);
      m_Emails->Push(tEmails->Pop());
    }
    m_EmailsConsumers->Release();
  }
  
  // recognize domains
  tDomains = RecognizeDomains(tPage);

  printf("CScraper::Loop() > %d domain(s) recognized\n", tDomains->GetSize());

  // enqueue emails
  while (tDomains->IsEmpty() == false) {
    m_DomainsProducers->Acquire();
    {
      GUARD __tGuard(m_Domains);
      m_Domains->Push(tDomains->Pop());
    }
    m_DomainsConsumers->Release();
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
CScraper::CScraper(::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pDomains, ::BASE::IObject * DomainsProducers, ::BASE::IObject * DomainsConsumers, ::DATASTRUCTURE::CQueue<REFERENCE<CTask> > * pEmails, ::BASE::IObject * EmailsProducers, ::BASE::IObject * EmailsConsumers) :
  ::BASE::CLoopThread(::BASE::IObject::NON_BLOCKED),
  m_Domains(pDomains),
  m_DomainsProducers(DomainsProducers),
  m_DomainsConsumers(DomainsConsumers),
  m_Emails(pEmails),
  m_EmailsProducers(EmailsProducers),
  m_EmailsConsumers(EmailsConsumers) {
} // CScraper


/////////////////////////////////////////////////////////////////////////////
CScraper::~CScraper() {
} // ~CScraper

} // namespace WSR
