#include "pch.h"
#include <Windows.Foundation.h>
#include <string>
//#pragma comment(lib, "runtimeobject.lib")

using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL::Wrappers;
std::wstring uri_resolve( std::wstring const&url, std::wstring const&relative ) {
  if ( url.empty() )
    return relative;
  ComPtr<IUriRuntimeClassFactory> uriFactory;
  auto hr = GetActivationFactory( HStringReference( RuntimeClass_Windows_Foundation_Uri ).Get(), &uriFactory );
  ComPtr<IUriRuntimeClass> uri, v;
  if (ok(hr))
    hr = uriFactory->CreateUri( HStringReference( url.c_str() ).Get(), uri.ReleaseAndGetAddressOf() );
  if (ok(hr))
    hr = uri->CombineUri( HStringReference( relative.c_str() ).Get(), v.ReleaseAndGetAddressOf());
  HString urx;
  if (ok(hr))
    hr = v->get_AbsoluteUri( urx.GetAddressOf() );
  return ok(hr)?std::wstring( urx.GetRawBuffer( nullptr ) ) : relative;  
}