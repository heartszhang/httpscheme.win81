#pragma once
#include "common.h"
#include <functional>

using CompleteHandler = std::function<void( int status, wchar_t const*phrase, wchar_t const*headers, std::vector<char> const&body )>;  //no except
using ErrorHandler = std::function<void( HRESULT hr )>;

struct HttpRequestCallback : RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback, FtmBase> {
  HttpRequestCallback( CompleteHandler ch, ErrorHandler eh ) :_complete( ch ), _error( eh ) {}

public:  //IXMLHTTPRequest2Callback
  STDMETHODIMP OnRedirect( IXMLHTTPRequest2*, const WCHAR* ) { return S_OK; }
  STDMETHODIMP OnHeadersAvailable( IXMLHTTPRequest2* xhr, DWORD status, const WCHAR*  phrase ) {
    _status = status;
    _phrase = phrase;
    header_string hs;
    xhr->GetAllResponseHeaders( &hs.data );
    _headers = std::move( hs.str() );
    return S_OK;
  }
  STDMETHODIMP OnDataAvailable( IXMLHTTPRequest2*, ISequentialStream* ) { return S_OK; }
  STDMETHODIMP OnResponseReceived( IXMLHTTPRequest2*, ISequentialStream*stream ) {
    auto body = read_full( stream );
    _complete( _status, _phrase.c_str(), _headers.c_str(), body );
    // be aware of cyclic reference
    _complete = nullptr;
    _error = nullptr;
    return S_OK;
  }
  STDMETHODIMP OnError( IXMLHTTPRequest2*, HRESULT err ) {
    if ( _error ) _error( err );
    _error = nullptr;// []( long ) {};
    _complete = nullptr;
    return S_OK;
  }
private:
  CompleteHandler _complete;
  ErrorHandler _error;
  ulong_t   _status;
  std::wstring _phrase;
  std::wstring _headers;
};
