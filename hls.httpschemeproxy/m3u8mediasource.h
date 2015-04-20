#pragma once
#include "mediaplaylist.h"
//struct MediaPlaylist;

//[Guid("10E2CE5F-306C-4408-8480-BFC87215C6E4")]
struct __declspec(uuid("10E2CE5F-306C-4408-8480-BFC87215C6E4")) M3u8MediaSource : public RuntimeClass < RuntimeClassFlags<ClassicCom>, IMFMediaSource > {
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

private:
  std::wstring uri;
  ComPtr<IMFSourceResolver> resolver;
  ComPtr<IMFMediaSource>  current;
  std::vector < ComPtr<IMFMediaSource>> sources;
  MediaPlaylist           playlist;
};
