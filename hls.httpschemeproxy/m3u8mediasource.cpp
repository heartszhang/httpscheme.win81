#include "pch.h"
#include "m3u8mediasource.h"
#include "hls.httpschemeproxy.h"

M3u8MediaSource::M3u8MediaSource( MediaPlaylist const&list):playlist(list) {}


HRESULT M3u8MediaSource::BeginOpen( LPCWSTR url
                                    , DWORD flags
                                    , IPropertyStore *props
                                    , IUnknown**cancel
                                    , IMFAsyncCallback*cb, IUnknown*state ) {
  uri = url;
  auto hr = resolver->BeginCreateObjectFromURL( playlist.segments[ 0 ].c_str(), flags, props, cancel, cb, state );
  return hr;
}

HRESULT M3u8MediaSource::EndOpen( IMFAsyncResult*result ) {
  auto ot = MF_OBJECT_INVALID;
  ComPtr<IUnknown> first;
  auto hr = resolver->EndCreateObjectFromURL( result, &ot, &first );
  if ( ot == MF_OBJECT_MEDIASOURCE )
    sources.push_back( Cast<IMFMediaSource>(first) );
  
  return hr;
}

HRESULT M3u8MediaSource::GetCharacteristics(DWORD *chars ) {
  auto hr = source->GetCharacteristics( chars );
  if ( ok( hr ) )
    dump_chars( *chars );
  return hr;
}

HRESULT M3u8MediaSource::CreatePresentationDescriptor(IMFPresentationDescriptor **pd ) {
  auto hr = source->CreatePresentationDescriptor( pd );
  dump( L"source create-pd \n" );
  if ( ok( hr ) )
    dump_pd( *pd );
  return hr;
}


HRESULT M3u8MediaSource::Start( IMFPresentationDescriptor *pPresentationDescriptor
                                , const GUID *pguidTimeFormat
                                , const PROPVARIANT *pvarStartPosition ) {
  auto hr = source->Start( pPresentationDescriptor, pguidTimeFormat, pvarStartPosition );
  dump( L"source start\n" );
  if ( ok( hr ) )
    dump_pd( pPresentationDescriptor );
  return hr;
}


HRESULT M3u8MediaSource::Stop( void ) {
  auto hr = source->Stop();
  dump( L"source stop\n" );
  return hr;
}


HRESULT M3u8MediaSource::Pause( void ) {
  auto hr = source->Pause();
  dump( L"source pause\n" );
  return hr;
}


HRESULT M3u8MediaSource::Shutdown( void ) {
  auto hr = source->Shutdown();
  dump( L"source shutdown\n" );
  return hr;
}

HRESULT M3u8MediaSource::GetEvent( DWORD dwFlags, IMFMediaEvent **ppEvent ) {
  auto hr = source->GetEvent( dwFlags, ppEvent );
  dump( L"source get-event flags=%X\n", dwFlags );
  return hr;
}


HRESULT M3u8MediaSource::BeginGetEvent(IMFAsyncCallback *pCallback,IUnknown *punkState ) {
  auto hr = source->BeginGetEvent( pCallback, punkState );
  //  dump( L"source begin-get-event\n" );
  return hr;
}

HRESULT M3u8MediaSource::EndGetEvent(IMFAsyncResult *pResult,IMFMediaEvent **ppEvent ) {
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
  GUID xt = GUID_NULL;
  auto status = S_OK;
  hr = ( *ppEvent )->GetExtendedType( &xt );
  if ( ok( hr ) ) hr = ( *ppEvent )->GetStatus( &status );
  PropVariant p( MakeMediaStreamProxy( stream.Get() ).Get() );
  if ( ok( hr ) ) hr = MFCreateMediaEvent( met, xt, status, &p, &oute );
  if ( ok( hr ) ) {
    ( *ppEvent )->Release();
    ( *ppEvent ) = oute.Detach();
  }
  return hr;
}


HRESULT M3u8MediaSource::QueueEvent(MediaEventType met,REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT *pvValue ) {x
  auto hr = source->QueueEvent( met, guidExtendedType, hrStatus, pvValue );
  dump( L"source queue-event %d, hr = %X\n", met, hrStatus );
  return hr;
}

