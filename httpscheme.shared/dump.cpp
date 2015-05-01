#include "pch.h"

void dump( const wchar_t*fmt, ... ) {
  wchar_t buf[ 4096 ];
  va_list va;
  va_start( va, fmt );
  auto r = StringCchVPrintfW( buf, _countof( buf ), fmt, va );  // ignore return value
  va_end( va );
  if ( ok( r ) || r == STRSAFE_E_INSUFFICIENT_BUFFER )
    OutputDebugStringW( buf );
}