#include "pch.h"
#include "bytestreamhandlerproxy.h"
using namespace ABI::hls::httpschemeproxy;
ActivatableClass( ByteStreamHandlerProxy );

HRESULT ByteStreamHandlerProxy::RuntimeClassInitialize() {
  auto hr = MFCreateSourceResolver( &resolver );
  return hr;
}

HRESULT ByteStreamHandlerProxy::BeginCreateObject(
  /* [in] */ IMFByteStream *pByteStream,
  /* [in] */ LPCWSTR pwszURL,
  /* [in] */ DWORD dwFlags,
  /* [in] */ IPropertyStore *pProps,
  /* [annotation][out] */
  _Outptr_opt_  IUnknown **ppIUnknownCancelCookie,
  /* [in] */ IMFAsyncCallback *pCallback,
  /* [in] */ IUnknown *punkState ) {
  dump( L"ByteStreamHandler BeginCreateObject %s, flags=%x\n", pwszURL, dwFlags );
  auto inner = CreateAsyncCallbackProxy( pCallback, punkState );
  auto hr = resolver->BeginCreateObjectFromByteStream( pByteStream
                                                       , pwszURL
                                                       , FlagsWithoutLocal(dwFlags)
                                                       , pProps
                                                       , ppIUnknownCancelCookie, inner.Get(), nullptr );
  return hr;
}
HRESULT ByteStreamHandlerProxy::EndCreateObject(
  /* [in] */ IMFAsyncResult *pResult,
  /* [annotation][out] */
  _Out_  MF_OBJECT_TYPE *pObjectType,
  /* [annotation][out] */
  _Outptr_  IUnknown **ppObject ) {
  auto inner = InnerResultFromOuterAsyncResult( pResult );
  auto hr = resolver->EndCreateObjectFromByteStream( inner.Get(), pObjectType, ppObject );
  if ( ok( hr ) && *pObjectType == MF_OBJECT_MEDIASOURCE ) {
    dump( L"ByteStreamHandler EndCreateObject %p, type=MF_OBJECT_MEDIASOURCE\n", *ppObject );
    *ppObject = MakeMediaSourceProxy( *ppObject ).Detach();
  }
  return hr;
}

HRESULT ByteStreamHandlerProxy::CancelObjectCreation(
  /* [in] */ IUnknown *pIUnknownCancelCookie ) {
  auto hr = resolver->CancelObjectCreation( pIUnknownCancelCookie );
  return hr;
}

HRESULT ByteStreamHandlerProxy::GetMaxNumberOfBytesRequiredForResolution(
  /* [annotation][out] */
  _Out_  QWORD *pqwBytes ) {
  *pqwBytes = 128;// why 128?
  dump( L"ByteStreamHandlerProxy GetMax : 128\n" );
  return S_OK;
}
HRESULT ByteStreamHandlerProxy::SetProperties( ABI::Windows::Foundation::Collections::IPropertySet *  ) {
  dump( L"set-properties but we do nothing\n" );
  return S_OK;
}

