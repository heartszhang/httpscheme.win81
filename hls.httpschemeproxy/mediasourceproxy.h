#pragma once

struct MediaSourceProxy : public RuntimeClass < RuntimeClassFlags<ClassicCom>, IMFMediaSource > {
public:
  HRESULT RuntimeClassInitialize( IUnknown*src );
public: //IMFMediaSource
  virtual HRESULT STDMETHODCALLTYPE GetCharacteristics(
    /* [out] */ __RPC__out DWORD *pdwCharacteristics ) ;

  virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreatePresentationDescriptor(
    /* [annotation][out] */
    _Outptr_  IMFPresentationDescriptor **ppPresentationDescriptor ) ;

  virtual HRESULT STDMETHODCALLTYPE Start(
    /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor,
    /* [unique][in] */ __RPC__in_opt const GUID *pguidTimeFormat,
    /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarStartPosition ) ;

  virtual HRESULT STDMETHODCALLTYPE Stop( void ) ;

  virtual HRESULT STDMETHODCALLTYPE Pause( void ) ;

  virtual HRESULT STDMETHODCALLTYPE Shutdown( void ) ;
public:  // IMFMediaEventGenerator
  virtual HRESULT STDMETHODCALLTYPE GetEvent(
    /* [in] */ DWORD dwFlags,
    /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent ) ;

  virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginGetEvent(
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *punkState ) ;

  virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndGetEvent(
    /* [in] */ IMFAsyncResult *pResult,
    /* [annotation][out] */
    _Out_  IMFMediaEvent **ppEvent ) ;

  virtual HRESULT STDMETHODCALLTYPE QueueEvent(
    /* [in] */ MediaEventType met,
    /* [in] */ __RPC__in REFGUID guidExtendedType,
    /* [in] */ HRESULT hrStatus,
    /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue ) ;
private:
  ComPtr<IMFMediaSource> source;
};


