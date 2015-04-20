#pragma once
#include "hls.httpschemeproxy.h"
#include "mediaplaylist.h"

namespace ABI {
namespace hls {
namespace httpschemeproxy {
class M3u8ByteStreamHandler : public RuntimeClass < RuntimeClassFlags<WinRtClassicComMix>
  , ABI::Windows::Media::IMediaExtension
  , IMFByteStreamHandler > {
  InspectableClass( RuntimeClass_hls_httpschemeproxy_M3u8ByteStreamHandler, BaseTrust )

public:
  HRESULT RuntimeClassInitialize();
public://IMFByteStreamHandler
  STDMETHODIMP BeginCreateObject( IMFByteStream *pByteStream
                                  , LPCWSTR pwszURL
                                  , DWORD dwFlags
                                  , IPropertyStore *pProps
                                  , IUnknown **ppIUnknownCancelCookie
                                  , IMFAsyncCallback *pCallback
                                  , IUnknown *punkState ) override;
  STDMETHODIMP EndCreateObject( IMFAsyncResult *pResult, MF_OBJECT_TYPE *pObjectType, IUnknown **ppObject ) override;

  STDMETHODIMP CancelObjectCreation( IUnknown *pIUnknownCancelCookie )  override;

  STDMETHODIMP GetMaxNumberOfBytesRequiredForResolution( QWORD *pqwBytes )  override;
public:  //IMediaExtension
  STDMETHODIMP SetProperties( ABI::Windows::Foundation::Collections::IPropertySet *configuration ) override;

private:
  MediaPlaylist playlist;
};
}
}
}