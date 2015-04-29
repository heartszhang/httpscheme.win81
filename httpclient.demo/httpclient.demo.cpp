// httpclient.demo.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ppl.h>
#include <vector>
#include <windows.foundation.collections.h>
#include "httpclient.demo_h.h"

void dump( const wchar_t*fmt, ... ) {
  wchar_t buf[ 4096 ];
  va_list va;
  va_start( va, fmt );
  auto r = StringCchVPrintfW( buf, _countof( buf ), fmt, va );  // ignore return value
  va_end( va );
  if ( ok( r ) || r == STRSAFE_E_INSUFFICIENT_BUFFER )
    OutputDebugStringW( buf );
}

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
std::wstring content_string( const std::wstring, const std::vector<char>&data ) {
  std::vector<wchar_t> buf( data.size() );
  auto r = MultiByteToWideChar( CP_UTF8, 0
                                , data.data(), static_cast<int>( data.size() ), buf.data(), static_cast<int>( buf.size() ) );
  return std::wstring( buf.data(), r );
}
std::vector<char> read_full( ISequentialStream *stream ) {
  std::vector<char> data;
  uint8_t buf[ 4096 ];
  ULONG readed = 0;
  auto hr = S_OK;
  do {
    hr = stream->Read( buf, sizeof( buf ), &readed );

    auto n = data.size();
    data.resize( n + readed );
    memcpy_s( data.data() + n, readed, buf, readed );
  } while ( ok( hr ) && readed > 0 );
  return std::move( data );
}
using namespace ABI::bestv::web;

struct HttpResponseAsyncOperation : RuntimeClass<AsyncBase<IAsyncOperationCompletedHandler<IHttpResponse*>>, IAsyncOperation<IHttpResponse*>> {
  using HandlerType = IAsyncOperationCompletedHandler < IHttpResponse* > ;
public: //IAsyncOperation<IHttpResponse*>
  STDMETHODIMP put_Completed( HandlerType *handler ) { return PutOnComplete( handler ); }
  STDMETHODIMP get_Completed( HandlerType **handler ) { return GetOnComplete( handler ); }
  STDMETHODIMP GetResults( IHttpResponse **result ) {
    auto hr = CheckValidStateForResultsCall();
    if ( ok(hr) )
      *result = this->result.Detach();
    return hr;
  }
public:
  void WhenResponseReceived();
  void WhenError();
private:
  ComPtr<IHttpResponse> result;
};
namespace bestv { namespace web {
} }
#if 0
namespace web { namespace http {

struct HttpRequest : public RuntimeClass<IInspectable> {

};
//[Guid("08FF6CE4-ECF4-4333-9DD2-F9613CD82551")]
struct __declspec( uuid( "08FF6CE4-ECF4-4333-9DD2-F9613CD82551" ) ) __declspec( novtable ) IHttpFilter {
  virtual HRESULT __stdcall SendRequestAsync( HttpRequest*, IAsyncOperation<HttpResponse*> **) = 0;
};

class HttpRequestCallback;
struct HttpClient : public RuntimeClass<IInspectable> {
  virtual HRESULT __stdcall GetAsync( const wchar_t* url, IAsyncOperation<HttpResponse*> ** );
};
} }

#endif
#if TV_API
#define CLSCTX_GENERAL CLSCTX_SERVER
#else
#define CLSCTX_GENERAL CLSCTX_INPROC
#endif

using CompleteHandler = std::function<void(int status, std::wstring const& reason, std::wstring const&headers, std::wstring const&content)>;  //no except
using ErrorHandler = std::function<void(long hr)>;

struct HttpRequestCallback :  RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback, FtmBase> {  
  HttpRequestCallback( CompleteHandler ch, ErrorHandler eh):_complete(ch), _error(eh){}

public:  //IXMLHTTPRequest2Callback
  STDMETHODIMP OnRedirect(  IXMLHTTPRequest2*,  const WCHAR* ) { return S_OK; }
  STDMETHODIMP OnHeadersAvailable( IXMLHTTPRequest2* xhr, DWORD status, const WCHAR*  phrase) {
    _status = status;
    _phrase = phrase;
    header_string hs;
    xhr->GetAllResponseHeaders( &hs.data );
    _headers = std::move(hs.str());
    return S_OK;
  }
  STDMETHODIMP OnDataAvailable(  IXMLHTTPRequest2*,  ISequentialStream* ) { return S_OK; }
  STDMETHODIMP OnResponseReceived(IXMLHTTPRequest2*, ISequentialStream*stream) {
    auto content = ::content_string( std::wstring(), read_full( stream ) );
    _complete( _status, _phrase, _headers, content );
    // be aware of cyclic reference
    _complete = []( int, std::wstring const&, std::wstring const&, std::wstring const& ) {};
    return S_OK;
  }
  STDMETHODIMP OnError(IXMLHTTPRequest2*, HRESULT err) {
    if (_error) _error( err );
    _error = []( long ) {};
    return S_OK;
  }
private:
  CompleteHandler _complete;
  ErrorHandler _error;
  ulong_t   _status;
  std::wstring _phrase;
  std::wstring _headers;
};

struct XhrCallback :  public RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback> {
  STDMETHODIMP RuntimeClassInitialize();

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
  auto cb = Make<HttpRequestCallback>([](int s, std::wstring const&phrase, std::wstring const&headers, std::wstring const&body)
  {
    dump( L"%d, %s\n", s, phrase.c_str() );
    dump( L"%s\n", headers.c_str() );
    dump( L"%s\n", body.c_str() );
  }, []( int r )
  {
    dump( L"failed for 0x%X\n", r );
  });
  if ( ok( hr ) )
    hr = xhr->Open( L"GET", L"http://testbox01.chinacloudapp.cn:8080/epg/XboxService/QueryPosition?Code=XBOX_POSITION_1", cb.Get(), nullptr, nullptr, nullptr, nullptr );
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

  auto c = content_string( contyp.str(), read_full( stream ) );
  dump( c.c_str() );
  return S_OK;
}

STDMETHODIMP XhrCallback::OnError( IXMLHTTPRequest2 *, HRESULT  hr) {
  dump( L"on-error 0x%X\n", hr );
  return S_OK;
}
