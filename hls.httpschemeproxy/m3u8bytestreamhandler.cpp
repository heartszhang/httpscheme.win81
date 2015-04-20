#include "pch.h"
#include "m3u8bytestreamhandler.h"
#include "m3u8mediasource.h"
using namespace ABI::hls::httpschemeproxy;
ActivatableClass( M3u8ByteStreamHandler );

HRESULT M3u8ByteStreamHandler::RuntimeClassInitialize() {
  playlist.uri = L"http://devimages.apple.com/iphone/samples/bipbop/gear1/prog_index.m3u8";
  playlist.segments.push_back(L"http://devimages.apple.com/iphone/samples/bipbop/gear1/fileSequence0.ts");
  playlist.segments.push_back( L"http://devimages.apple.com/iphone/samples/bipbop/gear1/fileSequence1.ts" );
//  auto hr = MFCreateSourceResolver( &resolver );
  return S_OK;
}

HRESULT M3u8ByteStreamHandler::BeginCreateObject(
  /* [in] */ IMFByteStream *,
  /* [in] */ LPCWSTR url,
  /* [in] */ DWORD flags,
  /* [in] */ IPropertyStore *props,
  /* [annotation][out] */
  _Outptr_opt_  IUnknown **cancel,
  /* [in] */ IMFAsyncCallback *cb,
  /* [in] */ IUnknown *state ) {
  dump( L"ByteStreamHandler BeginCreateObject %s, flags=%x\n", url, flags );
  auto src = Make<M3u8MediaSource>( playlist );
  auto inner = CreateAsyncCallback( [src, cb, state]( IMFAsyncResult* result ) ->HRESULT {
    ComPtr<IMFAsyncResult> outer;
    auto hr = src->EndOpen( result );
    auto x = MFCreateAsyncResult( src.Get(), cb, state, &outer );
    outer->SetStatus( hr );
    if ( ok( x ) )
      x = MFPutWorkItemEx2( MFASYNC_CALLBACK_QUEUE_MULTITHREADED, 0, outer.Get() );
    return x;
  } );
  auto hr = src->BeginOpen(url, flags, props, cancel, cb, state);
  
/*  auto hr = resolver->BeginCreateObjectFromByteStream( pByteStream
                                                       , pwszURL
                                                       , FlagsWithoutLocal( dwFlags )
                                                       , pProps
                                                       , ppIUnknownCancelCookie, inner.Get(), nullptr );
                                                       */
  //auto hr = resolver->BeginCreateObjectFromURL( playlist.segments[ 0 ].c_str(), flags, props, cancel, inner.Get(), nullptr );
  return hr;
}
HRESULT M3u8ByteStreamHandler::EndCreateObject(  IMFAsyncResult *result,MF_OBJECT_TYPE *ot,IUnknown **obj ) {  
  auto src = AsyncResultGetObject<M3u8MediaSource>( result );
  auto hr = result->GetStatus();
  if ( failed( hr ) )
    return hr;
  *ot = MF_OBJECT_MEDIASOURCE;
  *obj = src.Detach();
  return hr;
}

HRESULT M3u8ByteStreamHandler::CancelObjectCreation(
  /* [in] */ IUnknown * ) {
  return E_NOTIMPL;
}

HRESULT M3u8ByteStreamHandler::GetMaxNumberOfBytesRequiredForResolution(
  /* [annotation][out] */
  _Out_  QWORD *pqwBytes ) {
  *pqwBytes = 128;// why 128?
  dump( L"ByteStreamHandlerProxy GetMax : 128\n" );
  return S_OK;
}
HRESULT M3u8ByteStreamHandler::SetProperties( ABI::Windows::Foundation::Collections::IPropertySet *  ) {
  dump( L"set-properties but we do nothing\n" );
  return S_OK;
}

