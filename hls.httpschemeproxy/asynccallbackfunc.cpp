#include "pch.h"
#include "hls.httpschemeproxy.h"
#include <cassert>

struct AsyncCallbackFunc : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IMFAsyncCallback> {

  HRESULT RuntimeClassInitialize( InvokeFunT const& fn ) {
    func = fn;
    return S_OK;
  }
  STDMETHODIMP GetParameters( DWORD* flags, DWORD* q) {
    // Implementation of this method is optional.   
    assert( flags && q );
    *flags = 0;
    *q = MFASYNC_CALLBACK_QUEUE_STANDARD;
    return S_OK;
  }
  STDMETHODIMP Invoke( IMFAsyncResult* pAsyncResult ) {
    return func( pAsyncResult );
  }
private:
  InvokeFunT func;
};
auto CreateAsyncCallback( InvokeFunT func )-> ComPtr<IMFAsyncCallback> {
  ComPtr<IMFAsyncCallback> v;
  MakeAndInitialize<AsyncCallbackFunc>( &v, func );
  return v;
}


ComPtr < IMFAsyncResult > InnerResultFromOuterAsyncResult( IMFAsyncResult *outer ) {
  ComPtr<IUnknown> obj;
  ComPtr<IMFAsyncResult> innerresult;
  outer->GetObject( &obj );  // ignore return hr

  return Cast<IMFAsyncResult>( obj );
}
auto CreateAsyncCallbackProxy( IMFAsyncCallback* outercb, IUnknown*outerstat ) ->ComPtr<IMFAsyncCallback> {
  ComPtr<IMFAsyncCallback> outercbx( outercb );
  ComPtr<IUnknown> outerstatx( outerstat );
  auto innercb = CreateAsyncCallback( [outercbx, outerstatx]( IMFAsyncResult*innerresult )->HRESULT {
    ComPtr<IMFAsyncResult> outerresult;
    auto hr = MFCreateAsyncResult( innerresult, outercbx.Get(), outerstatx.Get(), &outerresult );
    if ( ok( hr ) ) hr = MFInvokeCallback( outerresult.Get() );
    return hr;
  } );
  return innercb;
}
