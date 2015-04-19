#include "pch.h"
#include "hls.httpschemeproxy.h"
struct MediaStreamProxy : public RuntimeClass < RuntimeClassFlags<ClassicCom>, IMFMediaStream > {
  HRESULT RuntimeClassInitialize(IUnknown* inner) { 
    auto hr = inner->QueryInterface( stream.ReleaseAndGetAddressOf() );
    return hr;
  }
public:
  STDMETHODIMP GetMediaSource( IMFMediaSource **ppMediaSource ) {
    auto hr = stream->GetMediaSource( ppMediaSource );
    return hr;
  }
  STDMETHODIMP GetStreamDescriptor( IMFStreamDescriptor **ppStreamDescriptor ) {
    auto hr = stream->GetStreamDescriptor( ppStreamDescriptor );
    dump( L"media-stream get-stream-desc\n" );
    return hr;
  }
  STDMETHODIMP RequestSample( IUnknown *pToken ) {
    auto hr = stream->RequestSample( pToken );
    dump( L"media-stream request-sample\n" );
    return hr;
  }

public:  // IMFMediaEventGenerator
  STDMETHODIMP GetEvent( DWORD dwFlags, IMFMediaEvent **ppEvent ) {
    auto hr = stream->GetEvent( dwFlags, ppEvent );
    return hr;
  }
  STDMETHODIMP BeginGetEvent( IMFAsyncCallback *pCallback, IUnknown *punkState ) {
    auto hr = stream->BeginGetEvent( pCallback, punkState );
    return hr;
  }
  STDMETHODIMP EndGetEvent( IMFAsyncResult *pResult, IMFMediaEvent **ppEvent ) {
    auto hr = stream->EndGetEvent( pResult, ppEvent );
    if ( failed( hr ) )
      return hr;
    MediaEventType met;
    hr = ( *ppEvent )->GetType( &met );
    dump_met( L"media-stream event %s\n", met );
    return hr;
  }
  STDMETHODIMP QueueEvent( MediaEventType met
                           , REFGUID guidExtendedType
                           , HRESULT hrStatus
                           , const PROPVARIANT *pvValue ) {
    auto hr = stream->QueueEvent( met, guidExtendedType, hrStatus, pvValue );
    return hr;
  }
private:
  ComPtr<IMFMediaStream> stream;
};

auto MakeMediaStreamProxy( IUnknown*inner )->ComPtr < IUnknown > {
  ComPtr<IUnknown> v;
  ( void )MakeAndInitialize<MediaStreamProxy>( &v, inner );
  inner->Release();
  return v;
}
