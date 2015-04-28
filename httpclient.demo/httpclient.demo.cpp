// httpclient.demo.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#if 0
namespace web { namespace http {
struct HttpResponse : public RuntimeClass<IInspectable> {

};
struct HttpRequest : public RuntimeClass<IInspectable> {

};
//[Guid("08FF6CE4-ECF4-4333-9DD2-F9613CD82551")]
struct __declspec( uuid( "08FF6CE4-ECF4-4333-9DD2-F9613CD82551" ) ) __declspec( novtable ) IHttpFilter {
  virtual HRESULT __stdcall SendRequestAsync( HttpRequest*, IAsyncOperation<HttpResponse*> **) = 0;
};

class HttpRequestCallback;
struct HttpClient : public RuntimeClass<IInspectable> {
  virtual HRESULT __stdcall SendRequestAsync( HttpRequest*, IAsyncOperation<HttpResponse*> ** );
};
} }
class HttpRequestCallback :
  public RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback, FtmBase> {
public:
  HttpRequestCallback( );

  // Called when the HTTP request is being redirected to a new URL.
  STDMETHODIMP OnRedirect( _In_opt_ IXMLHTTPRequest2*, __RPC__in_string const WCHAR* ) {    return S_OK;  }

  // Called when HTTP headers have been received and processed.
  STDMETHODIMP OnHeadersAvailable( _In_ IXMLHTTPRequest2* xmlReq, DWORD dw, __RPC__in_string const WCHAR* phrase );

  // Called when a portion of the entity body has been received.
  STDMETHODIMP OnDataAvailable( _In_opt_ IXMLHTTPRequest2*, _In_opt_ ISequentialStream* ) {    return S_OK;  }

  // Called when the entire entity response has been received.
  STDMETHODIMP OnResponseReceived( _In_opt_ IXMLHTTPRequest2*, _In_opt_ ISequentialStream* );

  // Called when an error occurs during the HTTP request.
  STDMETHODIMP OnError( _In_opt_ IXMLHTTPRequest2*, HRESULT hrError );

private:

};
#endif
#if TV_API
#define CLSCTX_GENERAL CLSCTX_SERVER
#else
#define CLSCTX_GENERAL CLSCTX_INPROC
#endif

int wmain(int argc, wchar_t* argv[])
{
//  hr = CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED );
//  CoUninitialize();
  RoInitializeWrapper initialize( RO_INIT_MULTITHREADED );
  ComPtr<IXMLHTTPRequest2> xhr;
  auto hr = CoCreateInstance( __uuidof( FreeThreadedXMLHTTP60 ), nullptr, CLSCTX_GENERAL
                              , __uuidof( IXMLHTTPRequest2 ), reinterpret_cast< void** >( xhr.GetAddressOf() ) );
  if ( ok( hr ) )
    hr = xhr->Open( L"GET", L"http://www.baidu.com/", callback, nullptr, nullptr, nullptr, nullptr );
  if ( ok( hr ) )
    hr = xhr->SetProperty( XHR_PROP_NO_CRED_PROMPT, 1 );
  if ( ok( hr ) )
    hr = xhr->SetProperty( XHR_PROP_TIMEOUT, 10 * 1000 );//10 secs
#ifdef XHR_PROP_ONDATA_NEVER
  if ( ok( hr ) )
    hr = xhr->SetProperty( XHR_PROP_ONDATA_THRESHOLD, XHR_PROP_ONDATA_NEVER );
#endif
  return 0;
}

