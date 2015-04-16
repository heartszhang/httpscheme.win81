#include <pch.h>
#include "hls.httpschemeproxy.h"
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
//#include <propvarutil.h>

void dump( const wchar_t*fmt, ... ) {
  wchar_t buf[ 4096 ];
  va_list va;
  va_start( va, fmt );
  auto r = _vsnwprintf_s( buf, _countof( buf ), _TRUNCATE, fmt, va );  // ignore return value
  va_end( va );
  OutputDebugStringW( buf );
}

struct PropVariant : PROPVARIANT {
  PropVariant() {    PropVariantInit( this );  }
  ~PropVariant() { PropVariantClear( this ); }
};

void mfguid_string( GUID const &guid, wchar_t *buf, size_t len ) {
  auto found = is_known_guid( guid, buf, len );
  if ( !found )
    StringFromGUID2( guid, buf, len );
}
void mfprop_string(PROPVARIANT const*val, wchar_t*buf, size_t len) {
  //PropVariantToString();
}
void dump_prop( GUID const &key, PROPVARIANT*val ) {
  wchar_t name[ 128 ] = L"";
  wchar_t value[ 4096 ] = L"";
  mfguid_string( key, name, _countof( name ) );
  mfprop_string( val, value, _countof( value ) );
  dump( L"%s = %s\n", name, value );
}
void dump_attrs( IMFAttributes *attrs ) {
  UINT32 count = 0;
  GUID key;
  auto hr = attrs->GetCount( &count );
  for ( auto i = 0; i < count; i++ ) {
    PropVariant prop{};
    auto hr = attrs->GetItemByIndex( i, &key, &prop );
    if ( ok( hr ) )
      dump_prop( key, &prop );
  }
}
void dump_mediatype( IMFMediaType *mtype ) {
  dump_attrs( mtype );
}
void dump_sd(IMFStreamDescriptor* sd) {
  dump_attrs( sd );
  DWORD mtcount = 0;
  ComPtr<IMFMediaTypeHandler> mthandler;
  auto hr = sd->GetMediaTypeHandler( &mthandler );
  if ( ok( hr ) )
    hr = mthandler->GetMediaTypeCount( &mtcount );
  for ( auto i = 0; i < mtcount; i++ ) {
    ComPtr<IMFMediaType> mtype;
    auto hr = mthandler->GetMediaTypeByIndex( i, &mtype );
    if ( ok( hr ) )
      dump_mediatype( mtype.Get() );
  }

}
void dump_pd( IMFPresentationDescriptor *pd ) {
  dump_attrs( pd );
  DWORD scount = 0;
  auto hr = pd->GetStreamDescriptorCount( &scount );
  for ( auto i = 0; i < scount; i++ ) {
    ComPtr<IMFStreamDescriptor> sd;
    BOOL selected;

    auto hr = pd->GetStreamDescriptorByIndex( i, &selected, &sd );
    if ( ok( hr ) )
      dump_sd( sd.Get() );
  }
}
