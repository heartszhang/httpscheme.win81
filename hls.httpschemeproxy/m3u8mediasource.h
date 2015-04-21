#pragma once
#include "mediaplaylist.h"
#include "lock.h"
//struct MediaPlaylist;
enum class MediaSourceStatus : unsigned __int64{
  //status       set-source  play  pause stop   seek              exit condition
  closed = 0,  //opening     nop   nop   nop    nop
  opening = 1, //new-opening undef ud    ud     undefined(ud)     if autoplay goto buffering, if autoplay=false goto stopped(MeMediaOpened) if source is invalid, goto opening MeMediaFailed
  buffering = opening << 1,  //opening  playing paused  stopped buffering(new-pos)  if buffer-time reached goto playing
  playing = buffering << 1,  //opening  nop     paused  stopped buffering(new-pos)  if end-of-stream go to paused, if end of buffer goto buffering
  paused = playing << 1,     //opening  buffering nop   stopped paused(new-pos)
  stopped = paused << 1,     //opening  buffering paused  nop   paused(new-pos) 
  shutdown = stopped << 1
};
//[Guid("10E2CE5F-306C-4408-8480-BFC87215C6E4")]
struct __declspec( uuid( "10E2CE5F-306C-4408-8480-BFC87215C6E4" ) ) M3u8MediaSource : public RuntimeClass < RuntimeClassFlags<ClassicCom>, IMFMediaSource, ISourceLocker> {
public:
  explicit M3u8MediaSource( MediaPlaylist const&list );
  HRESULT RuntimeClassInitialize(  );
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

public:
  STDMETHODIMP BeginOpen( LPCWSTR url, DWORD flags, IPropertyStore *props, IUnknown**cancel, IMFAsyncCallback*, IUnknown*state );
  STDMETHODIMP EndOpen(IMFAsyncResult*result);

public: //ISourceLocker
  STDMETHODIMP Lock();
  STDMETHODIMP Unlock();
private:
  std::wstring uri;
  ComPtr<IMFSourceResolver> resolver;
  ComPtr<IMFMediaSource>  current;
  std::vector < ComPtr<IMFMediaSource>> sources;
  MediaPlaylist           playlist;
  MediaSourceStatus       status = MediaSourceStatus::closed;
  ::Lock                  lock;
};

