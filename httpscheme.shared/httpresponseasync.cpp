#include "pch.h"
#include "httpresponseasync.h"

#if TV_API
#define CLSCTX_GENERAL CLSCTX_SERVER
#else
#define CLSCTX_GENERAL CLSCTX_INPROC
#endif

ActivatableClass( HttpClient );

auto http_post_async( wchar_t const*url, std::function<HRESULT( IHttpResponse*resp, HRESULT hr )> worker )->HRESULT {
  return http_request_async( url, L"POST", worker );
}
auto http_request_async( wchar_t const*url
                         , wchar_t const*method // GET/POST/HEAD/TRACE
                         , std::function<HRESULT( IHttpResponse*resp, HRESULT hr )> worker ) ->HRESULT {
  ComPtr<IXMLHTTPRequest2> xhr;
  auto hr = CoCreateInstance( __uuidof( FreeThreadedXMLHTTP60 ), nullptr, CLSCTX_GENERAL
                              , __uuidof( IXMLHTTPRequest2 ), reinterpret_cast< void** >( xhr.GetAddressOf() ) );
  //xhr-->httpreqcallback-->(asyncop + xhr)
  //so we should break cyclic references
  auto cb = Make<HttpRequestCallback>( [xhr, worker]( int status, std::wstring const&phrase, std::wstring const&headers, std::vector<char> const&body ) mutable -> HRESULT {
    xhr.Reset();
    auto resp = Make<HttpResponse>( status, phrase, headers, body );
    return worker( resp.Get(), S_OK );
  }, [worker, xhr]( HRESULT hr )mutable->HRESULT {
    xhr.Reset();
    return worker( nullptr, hr );
  } );

  if ( ok( hr ) )
    hr = xhr->Open( method, url, cb.Get(), nullptr, nullptr, nullptr, nullptr );
  if ( ok( hr ) )
    hr = xhr->SetProperty( XHR_PROP_NO_CRED_PROMPT, 1 );
  if ( ok( hr ) )
    hr = xhr->SetProperty( XHR_PROP_TIMEOUT, 30 * 1000 );//30 secs
#ifdef XHR_PROP_ONDATA_NEVER
  if ( ok( hr ) )
    hr = xhr->SetProperty( XHR_PROP_ONDATA_THRESHOLD, XHR_PROP_ONDATA_NEVER );
#endif
  //xhr->SetRequestHeader()
  if ( ok( hr ) )
    hr = xhr->Send( nullptr, 0 );
  return hr;
}
auto http_get_async( wchar_t const*url
                          , std::function<HRESULT( IHttpResponse*resp, HRESULT hr )> worker)->HRESULT {
  return http_request_async( url, L"GET", worker );
}
auto HttpGetAsync( const wchar_t*url, IAsyncOperation<IHttpResponse*> **aop )->HRESULT {
  auto asyncop = Make<HttpResponseAsyncOperation>();
  auto hr = http_get_async( url, [asyncop]( IHttpResponse*resp, HRESULT hr )mutable->HRESULT {
    if ( ok( hr ) )
      asyncop->WhenResponseReceived( resp );
    else asyncop->WhenError( hr );
    return asyncop.Reset();
  } );
  *aop = asyncop.Detach();
  return hr;
}

HRESULT HttpClient::GetAsync( HSTRING url, IAsyncOperation<IHttpResponse*> ** ret) {
  return HttpGetAsync( WindowsGetStringRawBuffer( url, nullptr ), ret );
}

HRESULT HttpClient::PostAsync( HSTRING url, IAsyncOperation<IHttpResponse*> ** ret ) {
  url; ret;
  return S_OK;
}

auto http_get_async( std::wstring const&xurl, std::function<HRESULT( IHttpResponse*resp, HRESULT hr )> worker, int  retries )->HRESULT {
  //std::wstring xurl;
  auto when_first_try = [worker, xurl, retries](IHttpResponse*resp, HRESULT hr)->HRESULT {
    if ( ok( hr ) || retries <= 0) {
      return worker( resp, hr );      
    }
    hr = http_get_async( xurl.c_str(), [worker]( IHttpResponse*resp, HRESULT hr )->HRESULT {
      return worker( resp, hr );
    }, retries - 1 );
    if ( failed( hr ) )
      hr = worker( nullptr, hr );
    return hr;
  };
  auto hr = http_get_async( xurl.c_str(), when_first_try );
  return hr;
}
