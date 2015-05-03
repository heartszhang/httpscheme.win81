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
  auto hr = HttpGetJsonAsync( L"http://www.cnbeta.com/articles/390467.htm", async.ReleaseAndGetAddressOf() );
  if ( failed( hr ) )
    return hr;
  hr = async->put_Completed( Callback<IAsyncOperationCompletedHandler<IHttpResponse*>>( []( IAsyncOperation<IHttpResponse*> *handler, AsyncStatus  ) {
    ComPtr<IHttpResponse> result;
    auto hr = handler->GetResults( result.ReleaseAndGetAddressOf() );
    int status = 0;
    if ( ok( hr ) )
      hr =  result->Status( &status, nullptr ) ;
    HString body;
    hr = result->BodyText(body.GetAddressOf());
    dump( body.GetRawBuffer( nullptr ) );
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
