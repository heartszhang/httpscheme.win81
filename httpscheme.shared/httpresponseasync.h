#pragma once
#include "httpresponse.h"
#include "httprequestcallback.h"

using HttpResponseHandlerType = IAsyncOperationCompletedHandler < IHttpResponse* >;

struct HttpResponseAsyncOperation : RuntimeClass<AsyncBase<HttpResponseHandlerType>, IAsyncOperation<IHttpResponse*>> {
  HttpResponseAsyncOperation() {
    Start();
  }

public: //IAsyncOperation<IHttpResponse*>
  STDMETHODIMP put_Completed( HttpResponseHandlerType *handler ) { return PutOnComplete( handler ); }
  STDMETHODIMP get_Completed( HttpResponseHandlerType **handler ) { return GetOnComplete( handler ); }
  STDMETHODIMP GetResults( IHttpResponse **result ) {
    auto hr = CheckValidStateForResultsCall();
    if ( ok( hr ) )
      hr = this->result.CopyTo( result );
    
    return hr;
  }
public:
  void WhenResponseReceived( IHttpResponse*resp ) {
    result = resp;
    if (TryTransitionToCompleted())
      ( void )FireCompletion();
  }
  void WhenError( HRESULT hr ) {
    if (TryTransitionToError( hr ))
      (void)FireCompletion();
  }
protected:
  ComPtr<IHttpResponse> result;

protected://AsyncBase
  virtual HRESULT OnStart( void ) { return S_OK; }
  virtual void OnClose( void ) {  }; // 
  virtual void OnCancel( void ) {  }; // xhr->Cancel();
};
auto http_request_async( wchar_t const*url
                         , wchar_t const*method // GET/POST/HEAD/TRACE
                         , std::function<HRESULT( IHttpResponse*resp, HRESULT hr )> worker )->HRESULT;

auto HttpGetAsync( const wchar_t*url, IAsyncOperation<IHttpResponse*> ** )->HRESULT;
auto http_get_async( wchar_t const*url, std::function<HRESULT(IHttpResponse*resp, HRESULT hr)>)->HRESULT;

using IHttpClient = ABI::bestv::web::IHttpClient;
class HttpClient : public RuntimeClass < IHttpClient > {
  InspectableClass( RuntimeClass_bestv_web_HttpClient, BaseTrust )
public: // IHttpClient
  STDMETHODIMP PostAsync( HSTRING, IAsyncOperation<IHttpResponse*> ** );
  STDMETHODIMP GetAsync( HSTRING, IAsyncOperation<IHttpResponse*> ** );
};
