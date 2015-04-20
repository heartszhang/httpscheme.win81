#include "pch.h"
#include "httpschemehandler.h"
#include <cassert>
//using namespace Platform;
using namespace ABI::hls::httpschemeproxy;
ActivatableClass(HttpSchemeHandler);

HttpSchemeHandler::HttpSchemeHandler() {
  dump( L"%p, %s created\n", this, RuntimeClass_hls_httpschemeproxy_HttpSchemeHandler );
}

HttpSchemeHandler::~HttpSchemeHandler() {
  dump( L"%p dtor\n", this );
}

HRESULT HttpSchemeHandler::RuntimeClassInitialize() {
  auto hr = MFCreateSourceResolver(&resolver);
  dump( L"hsh create source resolver 0x%X\n", hr );
  return hr;
}
HRESULT HttpSchemeHandler::BeginCreateObject(LPCWSTR url, DWORD flags, IPropertyStore * props, IUnknown ** cancel, IMFAsyncCallback * cb, IUnknown * s) {
  assert(resolver);
  auto innercb = CreateAsyncCallbackProxy( cb, s );
  auto hr = resolver->BeginCreateObjectFromURL( url
                                                , FlagsWithoutLocal( flags )
                                                , props, cancel, innercb.Get(), nullptr );
  dump( L"resolver begin %s, hr : %X, flags=%X\n", url, hr, flags);
  return hr;
}
HRESULT HttpSchemeHandler::EndCreateObject(IMFAsyncResult * result, MF_OBJECT_TYPE * objtype, IUnknown ** obj) {
  auto innerresult = InnerResultFromOuterAsyncResult( result );
  auto hr = resolver->EndCreateObjectFromURL( innerresult.Get(), objtype, obj );
  dump( L"end-create type: %d, %p\n", *objtype, *obj );
  if ( ok( hr ) && *objtype == MF_OBJECT_BYTESTREAM ) {
    *obj = MakeByteStreamProxy( *obj ).Detach();
  } else if ( ok( hr ) && *objtype == MF_OBJECT_MEDIASOURCE ) {
    *obj = MakeMediaSourceProxy( *obj ).Detach();
  }
  return hr;
}

HRESULT HttpSchemeHandler::CancelObjectCreation(IUnknown * cancel_cookie) {
  dump( L"cancel creation\n" );
  return resolver->CancelObjectCreation(cancel_cookie);
}

HRESULT HttpSchemeHandler::SetProperties(ABI::Windows::Foundation::Collections::IPropertySet * ) {
  dump( L"set-properties but we do nothing\n" );
  return S_OK;
}

DWORD FlagsWithoutLocal(DWORD flags) {
  return flags | MF_RESOLUTION_DISABLE_LOCAL_PLUGINS;
}

