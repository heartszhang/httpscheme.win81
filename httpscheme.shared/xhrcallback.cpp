#include "pch.h"
#include "xhrcallback.h"
#include "common.h"
HRESULT __stdcall XhrCallback::RuntimeClassInitialize() {
  return S_OK;
}

HRESULT __stdcall XhrCallback::OnRedirect( IXMLHTTPRequest2 *, const WCHAR *  ) {
  return S_OK;
}

STDMETHODIMP XhrCallback::OnHeadersAvailable( IXMLHTTPRequest2 *xhr, DWORD status, const WCHAR * phrase ) {
  _status = status;
  _phrase = phrase;
  wchar_t* hs = nullptr;
  auto hr = xhr->GetAllResponseHeaders( &hs );
  if ( ok( hr ) ) _headers = hs;
  CoTaskMemFree( hs );
  return hr;
}

STDMETHODIMP XhrCallback::OnDataAvailable( IXMLHTTPRequest2 *, ISequentialStream * ) { 
  return S_OK; 
}

STDMETHODIMP XhrCallback::OnResponseReceived( IXMLHTTPRequest2 *, ISequentialStream * stream ) {
  auto body = read_full( stream );
  auto hr = _complete ? _complete( _status, _phrase.c_str(), _headers.c_str(), body ) : E_ILLEGAL_METHOD_CALL;
  _complete = nullptr; // break cyclic reference
  return hr;
}

STDMETHODIMP XhrCallback::OnError( IXMLHTTPRequest2 *, HRESULT  err ) {
  auto hr = _error ? _error( err ) : E_ILLEGAL_METHOD_CALL;
  _error = nullptr;
  return hr;
}
