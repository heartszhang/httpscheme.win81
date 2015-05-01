#pragma once
#include "common.h"
#include "httpclient.demo_h.h"
using namespace ABI::bestv::web;

struct HttpResponse : RuntimeClass < IHttpResponse > {
public:
  explicit HttpResponse( HRESULT hr ) :result( hr ) {}
  explicit HttpResponse( int status, std::wstring const&phrase, std::wstring const&headers, const std::vector<char> &body ) :body( body ), status( status ), phrase( phrase ) {
    this->headers = split_headers( headers.c_str() );
    //    std::tie<content_type,encoding> = parse_content_type(get_header(L"CONTENT-TYPE"));
  }
public://IHttpResponse
  STDMETHODIMP GetHeader( HSTRING name, HSTRING *value ) {
    auto i = headers.find( WindowsGetStringRawBuffer( name, nullptr ) );
    if ( i == headers.end() )
      return NTE_NOT_FOUND;
    auto hr = WindowsCreateString( i->second.c_str(), static_cast<UINT32>( i->second.size() ), value );
    return hr;
  }
  STDMETHODIMP BodyJson( ABI::Windows::Data::Json::IJsonValue **body ) {
    return content_json( this->body, body );
  }
  STDMETHODIMP Status( int *status, HSTRING *phrase ) {
    *status = this->status;
    return phrase?
       WindowsCreateString( this->phrase.c_str(), static_cast<UINT32>( this->phrase.size() ), phrase ) : S_OK;
  }

protected:
  std::multimap<std::wstring, std::wstring> headers;
  std::vector<char>                         body;
  int                                       status = 0; // 0: uninitialized
  std::wstring                              phrase;
  long                                      result = S_OK;
};
