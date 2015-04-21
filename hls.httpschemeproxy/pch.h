//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

// Windows Header Files:
#include <windows.h>
#include <windows.media.h>

#include <wrl.h>

#include <mfapi.h>
#include <mfidl.h>
#include <uuids.h>
#include <mfreadwrite.h>
#include <mferror.h>

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite")

using namespace Microsoft::WRL;
inline bool ok(HRESULT hr) { return SUCCEEDED(hr); }
inline bool failed(HRESULT hr) { return FAILED(hr); }
