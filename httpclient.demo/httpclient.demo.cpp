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
  auto hr = HttpGetJsonAsync( L"http://testbox01.chinacloudapp.cn:8080/epg/XboxService/QueryPosition?Code=XBOX_POSITION_1", async.ReleaseAndGetAddressOf() );
  if ( failed( hr ) )
    return hr;
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
