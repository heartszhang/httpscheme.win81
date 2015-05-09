// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <wrl\module.h>

extern "C" __declspec( dllexport ) BOOL WINAPI DllMain( _In_opt_ HINSTANCE, DWORD, _In_opt_ LPVOID );
extern "C" HRESULT WINAPI DllCanUnloadNow();  // __declspec(dllexport) will result redifination error
extern "C" __declspec( dllexport ) HRESULT WINAPI DllGetActivationFactory( _In_ HSTRING, _Deref_out_ IActivationFactory** );
extern "C" HRESULT WINAPI DllGetClassObject( REFCLSID, REFIID, _Deref_out_ LPVOID* );

BOOL __declspec( dllexport ) APIENTRY DllMain( HMODULE /* hModule */, DWORD ul_reason_for_call, LPVOID /* lpReserved */ )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" HRESULT WINAPI DllCanUnloadNow() {
  const auto &module = Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule();
  return module.GetObjectCount() == 0 ? S_OK : S_FALSE;
}

extern "C" __declspec( dllexport ) HRESULT WINAPI DllGetActivationFactory( _In_ HSTRING activatibleClassId, _Deref_out_ IActivationFactory** factory ) {
  auto &module = Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule();
  return module.GetActivationFactory( activatibleClassId, factory );
}

extern "C" HRESULT WINAPI DllGetClassObject( _In_ REFCLSID rclsid, _In_ REFIID riid, _COM_Outptr_ void **ppv ) {
  return Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule().GetClassObject( rclsid, riid, ppv );
}

#if defined(_M_IX86)
#pragma comment(linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject=_DllGetClassObject@12,PRIVATE")
#elif defined(_M_ARM) || defined(_M_AMD64)
#pragma comment(linker, "/EXPORT:DllCanUnloadNow,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject,PRIVATE")
#endif