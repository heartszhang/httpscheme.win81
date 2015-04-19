#pragma once

struct MediaSourceProxy : public RuntimeClass < RuntimeClassFlags<ClassicCom>, IMFMediaSource > {
public:
  HRESULT RuntimeClassInitialize( IUnknown*src );
public: //IMFMediaSource
  STDMETHODIMP GetCharacteristics( DWORD *pdwCharacteristics );
  STDMETHODIMP CreatePresentationDescriptor( IMFPresentationDescriptor **ppd );
  STDMETHODIMP Start( IMFPresentationDescriptor *pPresentationDescriptor
                                           , const GUID *pguidTimeFormat
                                           , const PROPVARIANT *pvarStartPosition );

  STDMETHODIMP Stop( void );
  STDMETHODIMP Pause( void );
  STDMETHODIMP Shutdown( void );

public:  // IMFMediaEventGenerator
  STDMETHODIMP GetEvent( DWORD dwFlags, IMFMediaEvent **ppEvent );
  STDMETHODIMP BeginGetEvent( IMFAsyncCallback *pCallback, IUnknown *punkState );
  STDMETHODIMP EndGetEvent( IMFAsyncResult *pResult, IMFMediaEvent **ppEvent );
  STDMETHODIMP QueueEvent( MediaEventType met
                           , REFGUID guidExtendedType
                           , HRESULT hrStatus
                           , const PROPVARIANT *pvValue );
private:
  ComPtr<IMFMediaSource> source;
};


