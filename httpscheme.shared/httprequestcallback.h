#pragma once
#include "common.h"
#include <functional>

using CompleteHandler = std::function<HRESULT ( int status, wchar_t const*phrase, wchar_t const*headers, std::vector<char> const&body )>;  
using ErrorHandler = std::function<HRESULT ( HRESULT hr )>;

//FtmBase should be used for tv app
struct HttpRequestCallback : RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback, FtmBase> {
  HttpRequestCallback( CompleteHandler ch, ErrorHandler eh ) :_complete( ch ), _error( eh ) {}
public:  //IXMLHTTPRequest2Callback
  STDMETHODIMP OnRedirect( IXMLHTTPRequest2*, const WCHAR* ) { return S_OK; }
  STDMETHODIMP OnHeadersAvailable( IXMLHTTPRequest2* xhr, DWORD status, const WCHAR*  phrase ) {
    _status = status;
    _phrase = phrase;
    wchar_t* hs = nullptr;
    auto hr = xhr->GetAllResponseHeaders( &hs );
    if (ok(hr)) _headers = hs;
    CoTaskMemFree( hs );
    return hr;
  }
  STDMETHODIMP OnDataAvailable( IXMLHTTPRequest2*, ISequentialStream* ) { return S_OK; }
  STDMETHODIMP OnResponseReceived( IXMLHTTPRequest2* xhr, ISequentialStream*stream ) {
    auto body = read_full( stream );
    auto hr = _complete ? _complete( _status, _phrase.c_str(), _headers.c_str(), body ) : E_ILLEGAL_METHOD_CALL;
    _complete = nullptr; // break cyclic reference
    return hr;
  }
  STDMETHODIMP OnError( IXMLHTTPRequest2*, HRESULT err ) {
    auto hr = _error ? _error( err ): E_ILLEGAL_METHOD_CALL;
    _error = nullptr;
    return hr;
  }
private:
  CompleteHandler _complete;
  ErrorHandler    _error;
  ulong_t         _status;
  std::wstring    _phrase;
  std::wstring    _headers;
};
