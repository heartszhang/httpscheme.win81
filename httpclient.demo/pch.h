#pragma once

#include "targetver.h"
#include <Windows.Foundation.h>
#include <wrl.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl/async.h>
#if !TV_API
#include <msxml6.h>
#else
#include <ixmlhttprequest2.h>
#endif


using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

inline bool ok( HRESULT hr ) { return SUCCEEDED( hr ); }
inline bool failed( HRESULT hr ) { return FAILED( hr ); }
