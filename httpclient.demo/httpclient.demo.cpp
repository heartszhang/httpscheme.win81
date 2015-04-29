// httpclient.demo.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ppl.h>
#include <vector>
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

struct XhrCallback :  public RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback> {
protected:
  // Return value from final callbacks, including OnResponseReceived or
  // OnError event handler. Once a final callback is called, no more callback
  // will be executed.
  HRESULT result  = 0;
  DWORD status = 0;// HTTP status code from OnHeadersAvailable event handler.

  STDMETHODIMP RuntimeClassInitialize();

  friend HRESULT MakeAndInitialize<XhrCallback>( XhrCallback ** );

public: //IXMLHTTPRequest2Callback

  STDMETHODIMP    OnRedirect( IXMLHTTPRequest2 *pXHR, const WCHAR *pwszRedirectUrl );
  STDMETHODIMP    OnHeadersAvailable( IXMLHTTPRequest2 *pXHR, DWORD dwStatus, const WCHAR *pwszStatus );
  STDMETHODIMP    OnDataAvailable( IXMLHTTPRequest2 *pXHR, ISequentialStream *pResponseStream );
  STDMETHODIMP    OnResponseReceived( IXMLHTTPRequest2 *pXHR, ISequentialStream *pResponseStream );
  STDMETHODIMP    OnError( IXMLHTTPRequest2 *pXHR, HRESULT hrError );
};

int main_imp() {
  ComPtr<IXMLHTTPRequest2> xhr;
  auto hr = CoCreateInstance( __uuidof( FreeThreadedXMLHTTP60 ), nullptr, CLSCTX_GENERAL
                         , __uuidof( IXMLHTTPRequest2 ), reinterpret_cast< void** >( xhr.GetAddressOf() ) );
  auto cb = Make<XhrCallback>();
  if ( ok( hr ) )
    hr = xhr->Open( L"GET", L"http://www.baidu.com.cn/", cb.Get(), nullptr, nullptr, nullptr, nullptr );
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
  Sleep( 10000 );
  return hr;
}
int wmain(int , wchar_t* [])
{
  (void)CoInitializeEx( nullptr, COINIT_MULTITHREADED );
//  RoInitializeWrapper initialize( RO_INIT_MULTITHREADED );
  auto r = main_imp();
  CoUninitialize();
  return r;
}


void dump( const wchar_t*fmt, ... ) {
  wchar_t buf[ 4096 ];
  va_list va;
  va_start( va, fmt );
  auto r = StringCchVPrintfW( buf, _countof( buf ), fmt, va );  // ignore return value
  va_end( va );
  if ( ok( r ) || r == STRSAFE_E_INSUFFICIENT_BUFFER )
    OutputDebugStringW( buf );
}
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

struct header_string {
  wchar_t *data = nullptr;
  ~header_string() {
    CoTaskMemFree( data );
  }
  std::wstring str() {
    return std::wstring( data );
  }
};
//Content-Type: text/html; charset=utf-8
std::wstring content(const std::wstring , const std::vector<char>&data) {
  std::vector<wchar_t> buf( data.size()  );
  auto r = MultiByteToWideChar( CP_UTF8, 0
                                , data.data(), static_cast<int>( data.size() ), buf.data(), static_cast<int>( buf.size() ) );
  return std::wstring( buf.data(), r );
}
STDMETHODIMP XhrCallback::OnResponseReceived( IXMLHTTPRequest2 *xhr, ISequentialStream * stream) {
  //header_string headers;
  //auto hr = xhr->GetResponseHeader( L"Content-Length", &cls );
  //auto hr = xhr->GetAllResponseHeaders( &headers.data );
  //dump( headers.str().c_str() );
  header_string contyp;
  auto hr = xhr->GetResponseHeader( L"Content-Type", &contyp.data );
  std::vector<char> data;
  uint8_t buf[ 4096 ];
  ULONG readed = 0;
  do {
    hr = stream->Read( buf, sizeof( buf ), &readed );

    auto n = data.size();
    data.resize( n + readed );
    memcpy_s( data.data()+n, readed, buf, readed );    
  } while ( ok( hr ) && readed > 0 );
  auto c = content( contyp.str(), data );
  dump( c.c_str() );
  return S_OK;
}

STDMETHODIMP XhrCallback::OnError( IXMLHTTPRequest2 *, HRESULT  hr) {
  dump( L"on-error 0x%X\n", hr );
  return S_OK;
}
