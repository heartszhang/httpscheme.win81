// Class1.cpp
#include "pch.h"
#include "HttpSchemeHandler.h"
#include <functional>
#include <cassert>
//using namespace Platform;
using namespace ABI::HlsWrt;

ActivatableClass(HttpSchemeHandler);

HttpSchemeHandler::HttpSchemeHandler() {}

HttpSchemeHandler::~HttpSchemeHandler() {}

HRESULT HttpSchemeHandler::RuntimeClassInitialize() {
  auto hr = MFCreateSourceResolver(&resolver);
  return hr;
}
DWORD FlagsWithoutLocal(DWORD flags);
using InvokeFunT = std::function<HRESULT(IMFAsyncResult*)>;
ComPtr<IMFAsyncCallback> CreateAsyncCallback(InvokeFunT func);
HRESULT HttpSchemeHandler::BeginCreateObject(LPCWSTR url, DWORD flags, IPropertyStore * props, IUnknown ** cancel, IMFAsyncCallback * cb, IUnknown * s) {
  assert(resolver);
  ComPtr<IMFAsyncCallback> pcb(cb);
  ComPtr<IUnknown> ps(s);
  auto iresolver = resolver;
  auto xcb = CreateAsyncCallback([iresolver, pcb, ps](IMFAsyncResult* x)mutable->HRESULT {
    ComPtr<IMFAsyncResult> result;
    auto hr = MFCreateAsyncResult(x, pcb.Get(), ComPtr<IUnknown>(ps).Get(), &result);
    if (ok(hr)) hr = MFInvokeCallback(result.Get());
    return hr;
  });
  auto hr = resolver->BeginCreateObjectFromURL(url, FlagsWithoutLocal(flags), props, cancel, xcb.Get(), nullptr);
  return hr;
}

template<typename T, typename F>
ComPtr<T> Cast(ComPtr<F> i) {
  auto r = ComPtr<T>();
  i.As(&r);  // ignore hresult
  return r;
}
HRESULT HttpSchemeHandler::EndCreateObject(IMFAsyncResult * result, MF_OBJECT_TYPE * objtype, IUnknown ** obj) {
  ComPtr<IUnknown> x;
  auto hr = result->GetObject(&x);// x : the true async result
  auto tresult = Cast<IMFAsyncResult>(x);
  hr = resolver->EndCreateObjectFromURL(tresult.Get(), objtype, obj);
  return hr;
}

HRESULT HttpSchemeHandler::CancelObjectCreation(IUnknown * cancel_cookie) {
  return resolver->CancelObjectCreation(cancel_cookie);
}

HRESULT HttpSchemeHandler::SetProperties(ABI::Windows::Foundation::Collections::IPropertySet * configuration) {
  return S_OK;
}

DWORD FlagsWithoutLocal(DWORD flags) {
  return flags | MF_RESOLUTION_DISABLE_LOCAL_PLUGINS;
}

struct AsyncCallbackFunc : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IMFAsyncCallback> {
  InvokeFunT func;

  HRESULT RuntimeClassInitialize(InvokeFunT const& fn) {
    func = fn;
    return S_OK;
  }
  STDMETHODIMP GetParameters(DWORD*, DWORD*) {
    // Implementation of this method is optional.
    return E_NOTIMPL;
  }
  STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult) {
    return func(pAsyncResult);
  }
};
auto CreateAsyncCallback(InvokeFunT func)-> ComPtr<IMFAsyncCallback> {
  ComPtr<IMFAsyncCallback> v;
  MakeAndInitialize<AsyncCallbackFunc>(&v, func);
  return v;
}
