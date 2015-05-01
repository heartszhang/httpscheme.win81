#include "pch.h"
#include "httpresponseasync.h"


#if TV_API
#define CLSCTX_GENERAL CLSCTX_SERVER
#else
#define CLSCTX_GENERAL CLSCTX_INPROC
#endif

auto HttpGetJson( const wchar_t*url, IAsyncOperation<IHttpResponse*> **aop )->HRESULT {
  ComPtr<IXMLHTTPRequest2> xhr;
  auto hr = CoCreateInstance( __uuidof( FreeThreadedXMLHTTP60 ), nullptr, CLSCTX_GENERAL
                              , __uuidof( IXMLHTTPRequest2 ), reinterpret_cast< void** >( xhr.GetAddressOf() ) );
  auto asyncop = Make<HttpResponseAsyncOperation>();

  auto cb = Make<HttpRequestCallback>( [asyncop,xhr]( int status, std::wstring const&phrase, std::wstring const&headers, std::vector<char> const&body ) {
    asyncop->WhenResponseReceived( status, phrase.c_str(), headers.c_str(), body );
  }, [asyncop]( HRESULT hr ) {
    asyncop->WhenError( hr );
  } );

  if ( ok( hr ) )
    hr = xhr->Open( L"GET", url, cb.Get(), nullptr, nullptr, nullptr, nullptr );
  if ( ok( hr ) )
    hr = xhr->SetProperty( XHR_PROP_NO_CRED_PROMPT, 1 );
  if ( ok( hr ) )
    hr = xhr->SetProperty( XHR_PROP_TIMEOUT, 10 * 1000 );//10 secs
#ifdef XHR_PROP_ONDATA_NEVER
  if ( ok( hr ) )
    hr = xhr->SetProperty( XHR_PROP_ONDATA_THRESHOLD, XHR_PROP_ONDATA_NEVER );
#endif
  //xhr->SetRequestHeader()
  if ( ok( hr ) )
    hr = xhr->Send( nullptr, 0 );
  *aop = asyncop.Detach();
  return hr;
}
