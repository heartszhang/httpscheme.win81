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
      *result = this->result.Detach();
    return hr;
  }
public:
  void WhenResponseReceived( int status, const wchar_t*phrase, const wchar_t*headers, std::vector<char> const&body ) {
    result= Make<HttpResponse>( status, phrase, headers, body );
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
  virtual void OnClose( void ) {};
  virtual void OnCancel( void ) {};
};

auto HttpGetJson( const wchar_t*url, IAsyncOperation<IHttpResponse*> **)->HRESULT;
#if 0
namespace web {
  namespace http {

    struct HttpRequest : public RuntimeClass<IInspectable> {

    };
    //[Guid("08FF6CE4-ECF4-4333-9DD2-F9613CD82551")]
    struct __declspec( uuid( "08FF6CE4-ECF4-4333-9DD2-F9613CD82551" ) ) __declspec( novtable ) IHttpFilter {
      virtual HRESULT __stdcall SendRequestAsync( HttpRequest*, IAsyncOperation<HttpResponse*> ** ) = 0;
    };

    class HttpRequestCallback;
    struct HttpClient : public RuntimeClass<IInspectable> {
      virtual HRESULT __stdcall GetAsync( const wchar_t* url, IAsyncOperation<HttpResponse*> ** );
    };
  }
}

#endif
