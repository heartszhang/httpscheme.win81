﻿#pragma once
#include "hls.httpschemeproxy_h.h"  // generated by midl
#include <functional>

void dump( const wchar_t*fmt, ... );
void dump_pd( IMFPresentationDescriptor *pd );

template<typename T, typename F>
ComPtr<T> Cast( ComPtr<F> i ) {
  auto r = ComPtr<T>();
  i.As( &r );  // ignore hresult
  return r;
}
template<typename I>
ComPtr<I> AsyncResultGetObject( IMFAsyncResult*result ) {
  ComPtr<IUnknown> ui;
  ComPtr<I> v;
  (void)result->GetObject( &ui );
  ui.As( &v );
  return v;
}
DWORD FlagsWithoutLocal( DWORD flags );
using InvokeFunT = std::function<HRESULT( IMFAsyncResult* )>;

auto CreateAsyncCallback( InvokeFunT func )->ComPtr < IMFAsyncCallback > ;
auto CreateAsyncCallbackProxy( IMFAsyncCallback* outercb, IUnknown*outerstat )->ComPtr<IMFAsyncCallback>;
auto MakeByteStreamProxy( IUnknown* inner )->ComPtr < IUnknown > ;
auto MakeMediaSourceProxy( IUnknown* inner )->ComPtr < IUnknown > ;
auto MakeMediaStreamProxy(IUnknown*inner)->ComPtr < IUnknown > ;
auto InnerResultFromOuterAsyncResult( IMFAsyncResult *outer )->ComPtr < IMFAsyncResult >;

auto is_known_guid(GUID const&guid, wchar_t*buf, size_t len)->bool;
auto is_known_pkey( GUID const&guid, wchar_t*buf, size_t len )->bool;
auto is_known_h264_level( GUID const&guid, wchar_t*buf, size_t len )->bool;
auto is_known_h264_profile( GUID const&guid, wchar_t*buf, size_t len )->bool;
auto is_known_mfvim( GUID const&guid, wchar_t*buf, size_t len )->bool;

void dump_chars( DWORD chars );
void dump_met( const wchar_t*fmt, DWORD met );


struct PropVariant : PROPVARIANT {
  PropVariant() { PropVariantInit( this ); }
  ~PropVariant() { PropVariantClear( this ); }
  PropVariant( IUnknown*u ) : PropVariant(){
    vt = VT_UNKNOWN;
    this->punkVal = u;
    u->AddRef();
  }
};

template<typename I>
ComPtr<I> MediaEventGetValue( IMFMediaEvent*evt ) {
  PropVariant p;
  ComPtr<I> v;
  auto hr = evt->GetValue( &p );
  if ( ok( hr ) && p.vt == VT_UNKNOWN )
    p.punkVal->QueryInterface( v.ReleaseAndGetAddressOf() );
  return v;
}
void dump_msample( IMFSample* sample );
