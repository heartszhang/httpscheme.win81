#pragma once
#include <vector>
#include <functional>

using CompleteHandler = std::function<HRESULT( int status, wchar_t const*phrase, wchar_t const*headers, std::vector<char> const&body )>;
using ErrorHandler = std::function<HRESULT( HRESULT hr )>;

struct XhrCallback : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IXMLHTTPRequest2Callback> {
  STDMETHODIMP RuntimeClassInitialize();

public: //IXMLHTTPRequest2Callback

  STDMETHODIMP    OnRedirect( IXMLHTTPRequest2 *pXHR, const WCHAR *pwszRedirectUrl );
  STDMETHODIMP    OnHeadersAvailable( IXMLHTTPRequest2 *pXHR, DWORD dwStatus, const WCHAR *pwszStatus );
  STDMETHODIMP    OnDataAvailable( IXMLHTTPRequest2 *pXHR, ISequentialStream *pResponseStream );
  STDMETHODIMP    OnResponseReceived( IXMLHTTPRequest2 *pXHR, ISequentialStream *pResponseStream );
  STDMETHODIMP    OnError( IXMLHTTPRequest2 *pXHR, HRESULT hrError );
private:
  CompleteHandler _complete;
  ErrorHandler    _error;
  ulong_t         _status;
  std::wstring    _phrase;
  std::wstring    _headers;
};
