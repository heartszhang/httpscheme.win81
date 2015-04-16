#pragma once
#include "hls.httpschemeproxy.h"
namespace ABI {
namespace hls {
namespace httpschemeproxy {
  struct ByteStreamHandlerProxy : public RuntimeClass < RuntimeClassFlags<WinRtClassicComMix>
    , ABI::Windows::Media::IMediaExtension
    , IMFByteStreamHandler > {
    InspectableClass( RuntimeClass_hls_httpschemeproxy_ByteStreamProxyHandler, BaseTrust )

  public:
    HRESULT RuntimeClassInitialize();
  public://IMFByteStreamHandler
    virtual HRESULT STDMETHODCALLTYPE BeginCreateObject(
      /* [in] */ IMFByteStream *pByteStream,
      /* [in] */ LPCWSTR pwszURL,
      /* [in] */ DWORD dwFlags,
      /* [in] */ IPropertyStore *pProps,
      /* [annotation][out] */
      _Outptr_opt_  IUnknown **ppIUnknownCancelCookie,
      /* [in] */ IMFAsyncCallback *pCallback,
      /* [in] */ IUnknown *punkState ) override;

    virtual HRESULT STDMETHODCALLTYPE EndCreateObject(
      /* [in] */ IMFAsyncResult *pResult,
      /* [annotation][out] */
      _Out_  MF_OBJECT_TYPE *pObjectType,
      /* [annotation][out] */
      _Outptr_  IUnknown **ppObject ) override;

    virtual HRESULT STDMETHODCALLTYPE CancelObjectCreation(
      /* [in] */ IUnknown *pIUnknownCancelCookie ) override;

    virtual HRESULT STDMETHODCALLTYPE GetMaxNumberOfBytesRequiredForResolution(
      /* [annotation][out] */
      _Out_  QWORD *pqwBytes ) override;
  public:  //IMediaExtension
    virtual HRESULT STDMETHODCALLTYPE SetProperties(
      /* [in] */ __RPC__in_opt ABI::Windows::Foundation::Collections::IPropertySet *configuration ) override;

  private:
    ComPtr<IMFSourceResolver> resolver;
  };
}  }}