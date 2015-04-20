#include "pch.h"
#include "mediasourceproxy.h"
#include "hls.httpschemeproxy.h"
HRESULT MediaSourceProxy::GetCharacteristics(
  /* [out] */ __RPC__out DWORD *chars ) {
  auto hr = source->GetCharacteristics( chars );
  if ( ok( hr ) )
    dump_chars( *chars );
  return hr;
}

HRESULT MediaSourceProxy::CreatePresentationDescriptor(
    /* [annotation][out] */
    _Outptr_  IMFPresentationDescriptor **ppPresentationDescriptor ) {
  auto hr = source->CreatePresentationDescriptor( ppPresentationDescriptor );
  dump( L"source create-pd \n" );
  if ( ok( hr ) )
    dump_pd( *ppPresentationDescriptor );
  return hr;
}


HRESULT MediaSourceProxy::Start(
    /* [in] */ __RPC__in_opt IMFPresentationDescriptor *pPresentationDescriptor,
    /* [unique][in] */ __RPC__in_opt const GUID *pguidTimeFormat,
    /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarStartPosition ) {
  auto hr = source->Start( pPresentationDescriptor, pguidTimeFormat, pvarStartPosition );
  dump( L"source start\n" );
  if ( ok( hr ) )
    dump_pd( pPresentationDescriptor );
  return hr;
}


HRESULT MediaSourceProxy::Stop( void ) {
  auto hr = source->Stop();
  dump( L"source stop\n" );
  return hr;
}


HRESULT MediaSourceProxy::Pause( void ) {
  auto hr = source->Pause();
  dump( L"source pause\n" );
  return hr;
}


HRESULT MediaSourceProxy::Shutdown( void ) {
  auto hr = source->Shutdown();
  dump( L"source shutdown\n" );
  return hr;
}

HRESULT MediaSourceProxy::GetEvent(
    /* [in] */ DWORD dwFlags,
    /* [out] */ __RPC__deref_out_opt IMFMediaEvent **ppEvent ) {
  auto hr = source->GetEvent( dwFlags, ppEvent );
  dump( L"source get-event flags=%X\n", dwFlags );
  return hr;
}


HRESULT MediaSourceProxy::BeginGetEvent(
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *punkState ) {
  auto hr = source->BeginGetEvent( pCallback, punkState );
//  dump( L"source begin-get-event\n" );
  return hr;
}

HRESULT MediaSourceProxy::EndGetEvent(
    /* [in] */ IMFAsyncResult *pResult,
    /* [annotation][out] */
    _Out_  IMFMediaEvent **ppEvent ) {
  auto hr = source->EndGetEvent( pResult, ppEvent );
  MediaEventType met = MEUnknown;
  if ( ok( hr ) )
    ( *ppEvent )->GetType( &met );
  dump_met( L"media-source event %s\n", met );
  if ( met != MENewStream )
    return hr;
  auto stream = MediaEventGetValue<IMFMediaStream>( *ppEvent );
  if ( !stream )
    return hr;

  ComPtr<IMFMediaEvent> oute;
  GUID xt = GUID_NULL ;
  auto status = S_OK;
  hr = ( *ppEvent )->GetExtendedType( &xt );
  if (ok(hr)) hr = ( *ppEvent )->GetStatus( &status );
  PropVariant p( MakeMediaStreamProxy( stream.Get() ).Get() );
  if (ok(hr)) hr = MFCreateMediaEvent( met, xt, status, &p, &oute );
  if ( ok( hr ) ) {
    ( *ppEvent )->Release();
    ( *ppEvent ) = oute.Detach();
  }
  return hr;
}


HRESULT MediaSourceProxy::QueueEvent(
    /* [in] */ MediaEventType met,
    /* [in] */ __RPC__in REFGUID guidExtendedType,
    /* [in] */ HRESULT hrStatus,
    /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvValue ) {
  auto hr = source->QueueEvent( met, guidExtendedType, hrStatus, pvValue );
  dump( L"source queue-event %d, hr = %X\n", met, hrStatus );
  return hr;
}


HRESULT MediaSourceProxy::RuntimeClassInitialize( IUnknown*src ) {
  return src->QueryInterface( source.ReleaseAndGetAddressOf() );
}


auto MakeMediaSourceProxy( IUnknown* inner )-> ComPtr<IUnknown> {
  ComPtr<IUnknown> v;
  MakeAndInitialize<MediaSourceProxy>( &v, inner );  // ignore return value;
  inner->Release();
  return v;
}
