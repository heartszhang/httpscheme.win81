#include "pch.h"
#include "m3u8mediasource.h"
#include "hls.httpschemeproxy.h"

M3u8MediaSource::M3u8MediaSource( MediaPlaylist const&list):playlist(list) {}


//set-source at any status
HRESULT M3u8MediaSource::BeginOpen( LPCWSTR url
                                    , DWORD flags
                                    , IPropertyStore *props
                                    , IUnknown**cancel
                                    , IMFAsyncCallback*cb, IUnknown*state ) {
  Locker _( this ); 
  //should we check input parameters
  //todo: what if do beginopen when playing?
  if ( status != MediaSourceStatus::closed || status != MediaSourceStatus::shutdown )
    return MF_E_STATE_TRANSITION_PENDING;
  uri = url;
  status = MediaSourceStatus::opening;
  auto hr = resolver->BeginCreateObjectFromURL( playlist.segments[ 0 ].c_str(), flags, props, cancel, cb, state );
  return hr;
}

HRESULT M3u8MediaSource::EndOpen( IMFAsyncResult*result ) {
  Locker _( this );
  auto ot = MF_OBJECT_INVALID;
  ComPtr<IUnknown> first;
  auto hr = resolver->EndCreateObjectFromURL( result, &ot, &first );
  if ( ot == MF_OBJECT_MEDIASOURCE )
    sources.push_back( Cast<IMFMediaSource>(first) );
  current = Cast<IMFMediaSource>( first );
  return hr;
}

HRESULT M3u8MediaSource::GetCharacteristics(DWORD *chars ) {
  Locker _( this );
  //MF_E_SHUTDOWN
  //MFMEDIASOURCE_HAS_MULTIPLE_PRESENTATIONS
  *chars = MFMEDIASOURCE_CAN_SEEK | MFMEDIASOURCE_CAN_PAUSE | MFMEDIASOURCE_HAS_SLOW_SEEK | MFMEDIASOURCE_CAN_SKIPFORWARD | MFMEDIASOURCE_CAN_SKIPBACKWARD | MFMEDIASOURCE_HAS_MULTIPLE_PRESENTATIONS;
  return S_OK;
  
}

HRESULT M3u8MediaSource::CreatePresentationDescriptor(IMFPresentationDescriptor **pd ) {
  Locker _( this );
  if ( current == nullptr )
    return MF_E_NOT_INITIALIZED;
  //MF_E_SHUTDOWN
  auto hr = current->CreatePresentationDescriptor( pd );
  dump( L"media-source CreatePresentationDescriptor \n" );
  if ( ok( hr ) )
    dump_pd( *pd );
  return hr;
}


HRESULT M3u8MediaSource::Start( IMFPresentationDescriptor *pPresentationDescriptor
                                , const GUID *pguidTimeFormat
                                , const PROPVARIANT *pvarStartPosition ) {
  Locker _( this );
  if ( current == nullptr )
    return MF_E_NOT_INITIALIZED;
  //MF_E_SHUTDOWN
  auto hr = current->Start( pPresentationDescriptor, pguidTimeFormat, pvarStartPosition );
  dump( L"source start\n" );
  if ( ok( hr ) )
    dump_pd( pPresentationDescriptor );
  return hr;
}


HRESULT M3u8MediaSource::Stop( void ) {
  Locker _( this );
  if ( current == nullptr )
    return MF_E_NOT_INITIALIZED;
  auto hr = current->Stop();
  dump( L"source stop\n" );
  return hr;
}


HRESULT M3u8MediaSource::Pause( void ) {
  Locker _( this );
  if ( current == nullptr )
    return MF_E_NOT_INITIALIZED;
  auto hr = current->Pause();
  dump( L"source pause\n" );
  return hr;
}


HRESULT M3u8MediaSource::Shutdown( void ) {
  Locker _( this );
  if ( current == nullptr )
    return MF_E_NOT_INITIALIZED;
  auto hr = current->Shutdown();
  dump( L"source shutdown\n" );
  return hr;
}

HRESULT M3u8MediaSource::GetEvent( DWORD dwFlags, IMFMediaEvent **ppEvent ) {
  //todo: should we fetch current and release the lock and then GetEvent
  auto hr = current->GetEvent( dwFlags, ppEvent );
  return hr;
}


HRESULT M3u8MediaSource::BeginGetEvent(IMFAsyncCallback *pCallback,IUnknown *punkState ) {
  Locker _( this );
  auto hr = current->BeginGetEvent( pCallback, punkState );
  //  dump( L"source begin-get-event\n" );
  return hr;
}

HRESULT M3u8MediaSource::EndGetEvent(IMFAsyncResult *pResult,IMFMediaEvent **ppEvent ) {
  Locker _( this );
  auto hr = current->EndGetEvent( pResult, ppEvent );
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
  auto xt = GUID_NULL;
  auto status = S_OK;
  hr = ( *ppEvent )->GetExtendedType( &xt );
  if ( ok( hr ) ) hr = ( *ppEvent )->GetStatus( &status );
  PropVariant p( MakeMediaStreamProxy( stream.Get() ).Get() );
  if ( ok( hr ) ) hr = MFCreateMediaEvent( met, xt, status, &p, oute.ReleaseAndGetAddressOf() );
  if ( ok( hr ) ) {
    ( *ppEvent )->Release();
    ( *ppEvent ) = oute.Detach();
  }
  return hr;
}


HRESULT M3u8MediaSource::QueueEvent(MediaEventType met,REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT *pvValue ) {
  Locker _( this );
  auto hr = current->QueueEvent( met, guidExtendedType, hrStatus, pvValue );
  dump( L"source queue-event %d, hr = %X\n", met, hrStatus );
  return hr;
}

HRESULT M3u8MediaSource::Lock() {
  lock.lock();
  return S_OK;
}
HRESULT M3u8MediaSource::Unlock() {
  lock.unlock();
  return S_OK;
}


//MENewStream
//MEUpdatedStream
//MESourceStarted 
//MESourceSeeked 
