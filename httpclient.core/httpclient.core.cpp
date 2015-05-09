// demo.dll.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "demo.h"

#include <wrl.h>
using namespace Microsoft::WRL;

namespace ABI {
  namespace demo {
    namespace dot {
      class Dummy : public RuntimeClass < IDummy > {
        InspectableClass( RuntimeClass_demo_dot_Dummy, BaseTrust )
      public:
        Dummy() {}
        HRESULT __stdcall Hello( _In_ int a, _In_ int b, _Out_ int* value ) {
          OutputDebugStringW( L"what" );
          *value = a + b;
          return S_OK;
        }
      };
      ActivatableClass( Dummy );
    }
  }
}