#pragma once
//[Guid("DEC79B79-E78D-4D4A-9398-C522B3476FB0")]
struct __declspec( uuid( "DEC79B79-E78D-4D4A-9398-C522B3476FB0" ) ) __declspec( novtable ) ISourceLocker : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE Lock() = 0;
  virtual HRESULT STDMETHODCALLTYPE Unlock() = 0;
};

struct Locker {
  ComPtr<ISourceLocker> locker;
  Locker( ComPtr<ISourceLocker> locker ) :locker( locker ) { locker->Lock(); }
  ~Locker() { locker->Unlock(); }
};

struct Lock : CRITICAL_SECTION {
public:
  Lock() { InitializeCriticalSectionEx( this, 100, 0 ); } // why 100?
  ~Lock() { DeleteCriticalSection( this ); }

  void lock() { EnterCriticalSection( this ); }// different with ctor
  void unlock() { LeaveCriticalSection( this ); }
};
