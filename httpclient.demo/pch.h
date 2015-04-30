#pragma once

#include "targetver.h"
#include <windows.foundation.h>
#include <windows.data.json.h>
#include <wrl.h>
#include <wrl/wrappers/corewrappers.h>
#include <wrl/async.h>
#include <strsafe.h>
#if !TV_API
#include <msxml6.h>
#else
#include <ixmlhttprequest2.h>
#endif


using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Data::Json;

using ulong_t = unsigned long;
using long_t = long;

inline bool ok( HRESULT hr ) { return SUCCEEDED( hr ); }
inline bool failed( HRESULT hr ) { return FAILED( hr ); }
