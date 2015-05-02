#include "pch.h"
#include "httpresponseasync.h"

#if TV_API
#define CLSCTX_GENERAL CLSCTX_SERVER
#else
#define CLSCTX_GENERAL CLSCTX_INPROC
#endif

auto http_get_json_async( wchar_t const*url
                          , std::function<HRESULT( IHttpResponse*resp, HRESULT hr )> worker)->HRESULT {
  ComPtr<IXMLHTTPRequest2> xhr;
  auto hr = CoCreateInstance( __uuidof( FreeThreadedXMLHTTP60 ), nullptr, CLSCTX_GENERAL
                              , __uuidof( IXMLHTTPRequest2 ), reinterpret_cast< void** >( xhr.GetAddressOf() ) );
  //xhr-->httpreqcallback-->(asyncop + xhr)
  //so we should break cyclic references
  auto cb = Make<HttpRequestCallback>( [xhr, worker]( int status, std::wstring const&phrase, std::wstring const&headers, std::vector<char> const&body ) mutable -> HRESULT {
    xhr.Reset();
    auto resp = Make<HttpResponse>( status, phrase, headers, body );
    return worker( resp.Get() , S_OK);
  }, [worker, xhr]( HRESULT hr )mutable->HRESULT {
    xhr.Reset();
    return worker( nullptr , hr);
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
  return hr;
}
auto HttpGetJsonAsync( const wchar_t*url, IAsyncOperation<IHttpResponse*> **aop )->HRESULT {
  auto asyncop = Make<HttpResponseAsyncOperation>();
  auto hr = http_get_json_async( url, [asyncop](IHttpResponse*resp, HRESULT hr)mutable->HRESULT {
    if ( ok( hr ) )
      asyncop->WhenResponseReceived( resp );
    else asyncop->WhenError( hr );
    return asyncop.Reset();
  } );
  *aop = asyncop.Detach();
  return hr;
}

auto http_get_json_async( std::wstring const&xurl, std::function<HRESULT( IHttpResponse*resp, HRESULT hr )> worker, int  )->HRESULT {
  //std::wstring xurl;
  auto when_first_try = [worker, xurl](IHttpResponse*resp, HRESULT hr)->HRESULT {
    if ( ok( hr ) ) {
      return worker( resp, hr );      
    }
    hr = http_get_json_async( xurl.c_str(), [worker](IHttpResponse*resp, HRESULT hr)->HRESULT {
      return worker( resp, hr );
    } );
    if ( failed( hr ) )
      hr = worker( nullptr, hr );
    return hr;
  };
  auto hr = http_get_json_async( xurl.c_str(), when_first_try);
  return hr;
}
