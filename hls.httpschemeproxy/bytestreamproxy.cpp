#include "pch.h"
#include "hls.httpschemeproxy.h"
#include <cassert>
struct ByteStreamProxy : public RuntimeClass < RuntimeClassFlags<ClassicCom>, IMFByteStream > {
  ComPtr<IMFByteStream> stream;
  HRESULT RuntimeClassInitialize( IUnknown*obj ) {
    //stream = obj;
    obj->QueryInterface( stream.ReleaseAndGetAddressOf() );
    return S_OK;
  }
public://IMFByteStream
  virtual HRESULT STDMETHODCALLTYPE GetCapabilities(
    /* [out] */ __RPC__out DWORD *pdwCapabilities ) override {
    auto hr = stream->GetCapabilities( pdwCapabilities );
    dump( L"MFByteStream GetCapabilities %d\n", *pdwCapabilities );
    return hr;
  }

  virtual HRESULT STDMETHODCALLTYPE GetLength(
    /* [out] */ __RPC__out QWORD *pqwLength ) override {
    auto hr = stream->GetLength( pqwLength );
    dump( L"MFByteStream GetLength %d\n", *pqwLength );
    return hr;
  }

  virtual HRESULT STDMETHODCALLTYPE SetLength(
    /* [in] */ QWORD qwLength ) override { return stream->SetLength( qwLength ); }

  virtual HRESULT STDMETHODCALLTYPE GetCurrentPosition(
    /* [out] */ __RPC__out QWORD *pqwPosition ) override {
    auto hr = stream->GetCurrentPosition( pqwPosition );
    dump( L"MFByteStream GetCurrentPosition %d\n", *pqwPosition );
    return hr;
  }

  virtual HRESULT STDMETHODCALLTYPE SetCurrentPosition(
    /* [in] */ QWORD qwPosition ) override { return stream->SetCurrentPosition( qwPosition ); }

  virtual HRESULT STDMETHODCALLTYPE IsEndOfStream(
    /* [out] */ __RPC__out BOOL *pfEndOfStream ) { return stream->IsEndOfStream( pfEndOfStream ); }

  virtual HRESULT STDMETHODCALLTYPE Read(
    /* [size_is][out] */ __RPC__out_ecount_full( cb ) BYTE *pb,
    /* [in] */ ULONG cb,
    /* [out] */ __RPC__out ULONG *pcbRead ) override {
    return stream->Read( pb, cb, pcbRead );
  }

  virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginRead(
    /* [annotation][out] */
    _Out_writes_bytes_( cb )  BYTE *pb,
    /* [in] */ ULONG cb,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *punkState ) override {
    dump( L"MFByteStream BeginRead %d\n", cb );
    auto innercb = CreateAsyncCallbackProxy( pCallback, punkState );
    return stream->BeginRead( pb, cb, innercb.Get(), nullptr );
  }
  virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndRead(
    /* [in] */ IMFAsyncResult *pResult,
    /* [annotation][out] */
    _Out_  ULONG *pcbRead ) override {
    auto inner = InnerResultFromOuterAsyncResult( pResult );
    auto hr = stream->EndRead( inner.Get(), pcbRead );
    dump( L"MFByteStream EndRead %d\n", *pcbRead );
    return hr;
  }

  virtual HRESULT STDMETHODCALLTYPE Write(
    /* [size_is][in] */ __RPC__in_ecount_full( cb ) const BYTE *pb,
    /* [in] */ ULONG cb,
    /* [out] */ __RPC__out ULONG *pcbWritten ) override {
    return stream->Write( pb, cb, pcbWritten );
  }

  virtual /* [local] */ HRESULT STDMETHODCALLTYPE BeginWrite(
    /* [annotation][in] */
    _In_reads_bytes_( cb )  const BYTE *pb,
    /* [in] */ ULONG cb,
    /* [in] */ IMFAsyncCallback *pCallback,
    /* [in] */ IUnknown *punkState ) override {
    auto innercb = CreateAsyncCallbackProxy( pCallback, punkState );
    return stream->BeginWrite( pb, cb, innercb.Get(), nullptr );
  }

  virtual /* [local] */ HRESULT STDMETHODCALLTYPE EndWrite(
    /* [in] */ IMFAsyncResult *pResult,
    /* [annotation][out] */
    _Out_  ULONG *pcbWritten ) {
    auto x = InnerResultFromOuterAsyncResult( pResult );
    return stream->EndWrite( x.Get(), pcbWritten );
  }

  virtual HRESULT STDMETHODCALLTYPE Seek(
    /* [in] */ MFBYTESTREAM_SEEK_ORIGIN SeekOrigin,
    /* [in] */ LONGLONG llSeekOffset,
    /* [in] */ DWORD dwSeekFlags,
    /* [out] */ __RPC__out QWORD *pqwCurrentPosition ) override {
    auto hr = stream->Seek( SeekOrigin, llSeekOffset, dwSeekFlags, pqwCurrentPosition );
    dump( L"MFByteStream Seek %ld, flags=%d\n", llSeekOffset, dwSeekFlags );
    return hr;
  }

  virtual HRESULT STDMETHODCALLTYPE Flush( void ) override { return stream->Flush(); }

  virtual HRESULT STDMETHODCALLTYPE Close( void ) override { return stream->Close(); }
};
auto MakeByteStreamProxy( IUnknown* inner )-> ComPtr<IUnknown> {
  ComPtr<IUnknown> v;
  MakeAndInitialize<ByteStreamProxy>( &v, inner );  // ignore return value;
  return v;
}

