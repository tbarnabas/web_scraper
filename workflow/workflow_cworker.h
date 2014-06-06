
/////////////////////////////////////////////////////////////////////////////
//
// workflow_cworker.h - ::WORKFLOW::CWorker<C, P> class header
// --------------------------------------------------------------------------
//
// WORKFLOW
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _WORKFLOW_CWORKER
#define _WORKFLOW_CWORKER

#include "workflow_configuration.h"

namespace WORKFLOW {

DERIVE_ABSTRACT_EXCEPTION(::THREAD::ELoopThread, EWorker);

template <class C, class P>
class WORKFLOW_EXPORT_IMPORT CWorker :
  public ::THREAD::CLoopThread {
public:
  MEMBER_GET_SET__REFERENCE(::DATASTRUCTURE::CQueue<C>, ConsumerQueue);
  MEMBER_GET_SET__REFERENCE(::DATASTRUCTURE::CQueue<P>, ProducerQueue);

private:
  //! construct
  void __construct(T_BOOLEAN bConsumerQueue, ::DATASTRUCTURE::CQueue<C> * pConsumerQueue, T_BOOLEAN bProducerQueue, ::DATASTRUCTURE::CQueue<P> * pProducerQueue);

  //! loop
  virtual void Loop();

protected:
  //! prepare
  virtual T_BOOLEAN Prepare();
  //! consume
  virtual void Consume(C & pElement);
  //! produce
  virtual P Produce(C & pElement);

public:
  //! constructor
  CWorker(::BASE::CObject::synchronizations synchronization = ::BASE::CObject::NON_BLOCKED, const T_TIME & tTimeOut = T_TIME(), T_BOOLEAN bConsumerQueue = false, ::DATASTRUCTURE::CQueue<C> * pConsumerQueue = NULL, T_BOOLEAN bProducerQueue = false, ::DATASTRUCTURE::CQueue<P> * pProducerQueue = NULL);
  //! destructor
  virtual ~CWorker();

  //! copy constructor
  CWorker(const CWorker<C, P> & tWorker);
  //! assignment operator
  CWorker<C, P> & operator=(const CWorker<C, P> & tWorker);
}; // class WORKFLOW_EXPORT_IMPORT CWorker

} // namespace WORKFLOW


/////////////////////////////////////////////////////////////////////////////
//
// ::WORKFLOW::CWorker<C, P> class source
//
/////////////////////////////////////////////////////////////////////////////

namespace WORKFLOW {

/////////////////////////////////////////////////////////////////////////////
template <class C, class P>
void CWorker<C, P>::__construct(T_BOOLEAN bConsumerQueue, ::DATASTRUCTURE::CQueue<C> * pConsumerQueue, T_BOOLEAN bProducerQueue, ::DATASTRUCTURE::CQueue<P> * pProducerQueue) {
  m_ConsumerQueue = pConsumerQueue;
  if (bConsumerQueue == true) {
    if (pConsumerQueue == NULL) {
      m_ConsumerQueue.Create(new ::DATASTRUCTURE::CQueue<C>());
      REFERENCE< ::THREAD::CObject> tSynchronizator;
      tSynchronizator.Create(new ::THREAD::CCondition());
      m_ConsumerQueue->SetSynchronizator(tSynchronizator);
      ::THREAD::CObject::SetSynchronizator(tSynchronizator);
    }
  } 

  m_ProducerQueue = pProducerQueue;
  if (bProducerQueue == true) {
    if (pProducerQueue == NULL) {
      m_ProducerQueue.Create(new ::DATASTRUCTURE::CQueue<P>());
      REFERENCE< ::THREAD::CObject> tSynchronizator;
      tSynchronizator.Create(new ::THREAD::CCondition());
      m_ProducerQueue->SetSynchronizator(tSynchronizator);
    }
  }
} // __construct


/////////////////////////////////////////////////////////////////////////////
template <class C, class P>
void CWorker<C, P>::Loop() {
  if (Prepare() == true) {
    REFERENCE< ::DATASTRUCTURE::CQueue<C> > tConsumerQueue;
    REFERENCE< ::DATASTRUCTURE::CQueue<P> > tProducerQueue;
    C tConsumedElement;
    P tProducedElement;

    {
      THREADGUARD __tGuard(* this);
      tConsumerQueue = m_ConsumerQueue;
      tProducerQueue = m_ProducerQueue;
    }

    // consume elements
    if (tConsumerQueue.IsValid() == true) {
      THREADGUARD __tGuard(* tConsumerQueue);

      // waiting for element
      while (tConsumerQueue->IsEmpty() == false) {
        tConsumedElement = tConsumerQueue->Pop();

        // consume element
        tConsumerQueue->Release();
        IGNORE_EXCEPTION(Consume(tConsumedElement));
        tConsumerQueue->Acquire();
      }
    }

    // produce element
    IGNORE_EXCEPTION(tProducedElement = Produce(tConsumedElement));

    // push element into producer queue
    if ((tProducedElement.IsValid() == true) && (tProducerQueue.IsValid() == true)) {
      THREADGUARD __tGuard(* tProducerQueue);

      // push element
      tProducerQueue->Push(tProducedElement);  

      // send signal
      tProducerQueue->Signal();
    }
  }
} // Loop


/////////////////////////////////////////////////////////////////////////////
template <class C, class P>
T_BOOLEAN CWorker<C, P>::Prepare() {
  return (true);
} // Prepare


/////////////////////////////////////////////////////////////////////////////
template <class C, class P>
void CWorker<C, P>::Consume(C & pElement) {
} // Consume


/////////////////////////////////////////////////////////////////////////////
template <class C, class P>
P CWorker<C, P>::Produce(C & pElement) {
  return(P(pElement));
} // Produce


/////////////////////////////////////////////////////////////////////////////
template <class C, class P>
CWorker<C, P>::CWorker(::BASE::CObject::synchronizations synchronization, const T_TIME & tTimeOut, T_BOOLEAN bConsumerQueue, ::DATASTRUCTURE::CQueue<C> * pConsumerQueue, T_BOOLEAN bProducerQueue, ::DATASTRUCTURE::CQueue<P> * pProducerQueue) :
  ::THREAD::CLoopThread(synchronization, tTimeOut) {
  __construct(bConsumerQueue, pConsumerQueue, bProducerQueue, pProducerQueue);
} // CWorker


/////////////////////////////////////////////////////////////////////////////
template <class C, class P>
CWorker<C, P>::~CWorker() {
} // ~CWorker


/////////////////////////////////////////////////////////////////////////////
template <class C, class P>
CWorker<C, P>::CWorker(const CWorker<C, P> & tWorker) :
  ::THREAD::CLoopThread(tWorker),
  m_ConsumerQueue(tWorker.m_ConsumerQueue),
  m_ProducerQueue(tWorker.m_ProducerQueue) {
} // CWorker


/////////////////////////////////////////////////////////////////////////////
template <class C, class P>
CWorker<C, P> & CWorker<C, P>::operator=(const CWorker<C, P> & tWorker) {
  ::THREAD::CLoopThread::operator=(tWorker);
  m_ConsumerQueue = tWorker.m_ConsumerQueue;
  m_ProducerQueue = tWorker.m_ProducerQueue;
  return (* this);
} // operator=

} // namespace WORKFLOW

#endif // #ifndef _WORKFLOW_CWORKER
