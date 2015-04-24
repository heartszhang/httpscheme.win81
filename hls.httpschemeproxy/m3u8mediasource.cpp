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
  if ( cb == nullptr )
    return E_INVALIDARG;
  Locker _( this ); 
  //should we check input parameters
  //todo: what if do beginopen when playing?
  if ( status != MediaSourceStatus::closed || status != MediaSourceStatus::shutdown )
    return MF_E_STATE_TRANSITION_PENDING;
  if ( playlist.IsInvalid() )
    return MF_E_CANNOT_PARSE_BYTESTREAM;
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
  if ( failed(hr)  || ot != MF_OBJECT_MEDIASOURCE){
//    PostError( hr );
    status = MediaSourceStatus::closed;
    
  } else {
    //status = MediaSourceStatus::buffering;  // autoplay = true
    status = MediaSourceStatus::opened;
    sources.push_back( Cast<IMFMediaSource>( first ) );
    current = Cast<IMFMediaSource>( first );
    //The media source should not forward these events before the source's IMFMediaSource::Start method is called
  }
  return hr;
}

HRESULT M3u8MediaSource::GetCharacteristics(DWORD *chars ) {
  if ( chars == nullptr )
    return E_INVALIDARG;
//  Locker _( this ); needn't lock this
  if ( status < MediaSourceStatus::opened )
    return MF_E_SHUTDOWN;
  //MFMEDIASOURCE_HAS_MULTIPLE_PRESENTATIONS
  *chars = MFMEDIASOURCE_CAN_SEEK | MFMEDIASOURCE_CAN_PAUSE | MFMEDIASOURCE_HAS_SLOW_SEEK | MFMEDIASOURCE_CAN_SKIPFORWARD | MFMEDIASOURCE_CAN_SKIPBACKWARD ;
  return S_OK;
  
}

HRESULT M3u8MediaSource::CreatePresentationDescriptor(IMFPresentationDescriptor **pd ) {
  if ( pd == nullptr )
    return E_INVALIDARG;
  Locker _( this );
  if ( status < MediaSourceStatus::opened || current == nullptr)
    return MF_E_SHUTDOWN;

  auto hr = current->CreatePresentationDescriptor( pd );
  dump( L"media-source CreatePresentationDescriptor \n" );
  if ( ok( hr ) )
    dump_pd( *pd );
  return hr;
}


HRESULT M3u8MediaSource::Start( IMFPresentationDescriptor *pd
                                , const GUID *tf
                                , const PROPVARIANT *startpos ) {
  if ( tf != nullptr &&  !IsEqualGUID( *tf, GUID_NULL ) )
    return E_INVALIDARG;
  auto spos = -1ll;
  if ( startpos != nullptr && startpos->vt == VT_I4 )
    spos = startpos->hVal.QuadPart;
  Locker _( this );
  if ( spos == -1 )
    spos = CurrentPlayPosition();

  if ( status < MediaSourceStatus::opened || current == nullptr )
    return MF_E_SHUTDOWN;

  auto hr = S_OK;
  if ( status == MediaSourceStatus::playing )
    hr = SeekPlay(pd, spos);  // nothing to do

  if ( status == MediaSourceStatus::buffering )
    hr = SeekPlay( pd, spos );

  if ( status == MediaSourceStatus::paused || status == MediaSourceStatus::stopped )// enter buffering
    hr = StartPlay( pd, spos ); // playing or seeking
//  auto hr = current->Start( pd, &GUID_NULL, &PropVariant(spos) );
  dump( L"source start\n" );
  return hr;
}


HRESULT M3u8MediaSource::Stop( void ) {
  Locker _( this );
  if ( status < MediaSourceStatus::opened )
    return MF_E_SHUTDOWN;
  if ( status == MediaSourceStatus::stopped )
    return S_OK;
  if ( status == MediaSourceStatus::buffering ) {
    CancelBuffering([]()  // stop streams
    {
      status = MediaSourceStatus::stopped;
      PostSourceStopped();
    } );
  }
  if ( status == MediaSourceStatus::paused ) {

  }
  if ( status == MediaSourceStatus::playing ) {
    
  }

  dump( L"source stop\n" );
  return S_OK;
}


HRESULT M3u8MediaSource::Pause( void ) {
  Locker _( this );
  if ( status < MediaSourceStatus::opened )
    return MF_E_SHUTDOWN;
  if ( status == MediaSourceStatus::paused )
    return S_OK;

  if ( status == MediaSourceStatus::buffering ) {
    
  }
  if ( status == MediaSourceStatus::playing ) {
    
  }
  if ( status == MediaSourceStatus::stopped ) {
    
  }
  return S_OK;
}


HRESULT M3u8MediaSource::Shutdown( void ) {
  Locker _( this );
  if ( status < MediaSourceStatus::opening ) {
    return S_OK;
  }
  ShutdownStreams();
  // when to trigger ME_SHUTDOWN
  return S_OK;
}

HRESULT M3u8MediaSource::GetEvent( DWORD flags, IMFMediaEvent **evt ) {
  //todo: should we fetch current and release the lock and then GetEvent
  decltype( equeue ) eq;
  do {
    Locker _( this );
    eq = equeue;
  } while ( false );
  return eq->GetEvent( flags, evt );
}


HRESULT M3u8MediaSource::BeginGetEvent(IMFAsyncCallback *cb,IUnknown *state ) {
  Locker _( this );
  return equeue->BeginGetEvent( cb, state );  
}

HRESULT M3u8MediaSource::EndGetEvent(IMFAsyncResult *result,IMFMediaEvent **evt ) {
  Locker _( this );
  return equeue->EndGetEvent( result, evt );
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
HRESULT M3u8MediaSource::RuntimeClassInitialize() {
  return MFCreateEventQueue( equeue.ReleaseAndGetAddressOf() );
}

//MENewStream
//MEUpdatedStream
//MESourceStarted 
//MESourceSeeked 
