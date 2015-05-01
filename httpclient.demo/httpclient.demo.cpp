#include "pch.h"
#include <ppl.h>
#include <vector>
#include <map>
#include <windows.foundation.collections.h>
#include "httpclient.demo_h.h"
#include "httprequestcallback.h"
#include "httpresponseasync.h"
namespace bestv { namespace web {
} }

int main_imp() {
  ComPtr<IAsyncOperation<IHttpResponse*>> async;
  auto hr = HttpGetJson( L"http://testbox01.chinacloudapp.cn:8080/epg/XboxService/QueryPosition?Code=XBOX_POSITION_1", async.ReleaseAndGetAddressOf() );
  if ( ok( hr ) )
    hr = async->put_Completed( Callback<IAsyncOperationCompletedHandler<IHttpResponse*>>( []( IAsyncOperation<IHttpResponse*> *handler, AsyncStatus  ) {
    ComPtr<IHttpResponse> result;
    auto hr = handler->GetResults( result.ReleaseAndGetAddressOf() );
    int status = 0;
    if ( ok( hr ) )
      hr =  result->Status( &status, nullptr ) ;
    dump( L"http-status:%d\n", status );
    return hr;
  } ).Get() );
  return hr;
}
int wmain(int , wchar_t* [])
{
  //(void)CoInitializeEx( nullptr, COINIT_MULTITHREADED );
  RoInitializeWrapper initialize( RO_INIT_MULTITHREADED );
  auto r = main_imp();
  Sleep( 20000 );
  //CoUninitialize();
  return r;
}
#if 0

struct XhrCallback :  public RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback> {
  STDMETHODIMP RuntimeClassInitialize();

public: //IXMLHTTPRequest2Callback

  STDMETHODIMP    OnRedirect( IXMLHTTPRequest2 *pXHR, const WCHAR *pwszRedirectUrl );
  STDMETHODIMP    OnHeadersAvailable( IXMLHTTPRequest2 *pXHR, DWORD dwStatus, const WCHAR *pwszStatus );
  STDMETHODIMP    OnDataAvailable( IXMLHTTPRequest2 *pXHR, ISequentialStream *pResponseStream );
  STDMETHODIMP    OnResponseReceived( IXMLHTTPRequest2 *pXHR, ISequentialStream *pResponseStream );
  STDMETHODIMP    OnError( IXMLHTTPRequest2 *pXHR, HRESULT hrError );
};
HRESULT __stdcall XhrCallback::RuntimeClassInitialize(){
  return S_OK;
}

HRESULT __stdcall XhrCallback::OnRedirect( IXMLHTTPRequest2 *, const WCHAR * next) {
  dump( L"%s\n", next );
  return S_OK;
}

STDMETHODIMP XhrCallback::OnHeadersAvailable( IXMLHTTPRequest2 *, DWORD status, const WCHAR * reason) {
  dump( L"status: %d, reason: %s\n", status, reason );
  return S_OK;
}

STDMETHODIMP XhrCallback::OnDataAvailable( IXMLHTTPRequest2 *, ISequentialStream * ) {  return S_OK;}
STDMETHODIMP XhrCallback::OnResponseReceived( IXMLHTTPRequest2 *xhr, ISequentialStream * stream) {
  header_string contyp;
  (void)xhr->GetResponseHeader( L"Content-Type", &contyp.data );

  auto c = http_content_string( contyp.str(), read_full( stream ) );
//  auto jv = content_json( read_full( stream ) );
  dump( c.c_str() );
  return S_OK;
}

STDMETHODIMP XhrCallback::OnError( IXMLHTTPRequest2 *, HRESULT  hr) {
  dump( L"on-error 0x%X\n", hr );
  return S_OK;
}
#endif