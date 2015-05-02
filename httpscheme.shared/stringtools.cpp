#include "pch.h"
#include "common.h"
#include <map>
#include <algorithm>

//Content-Type: text/html; charset=utf-8
std::wstring http_content_utf8( const std::wstring, const std::vector<char>&data ) {
  std::vector<wchar_t> buf( data.size() );
  auto r = MultiByteToWideChar( CP_UTF8, 0
                                , data.data(), static_cast<int>( data.size() ), buf.data(), static_cast<int>( buf.size() ) );
  return std::wstring( buf.data(), r );
}


HRESULT content_json( const std::vector<char>&data, IJsonValue**v ) {
  ComPtr<IJsonValueStatics> parser;
  auto hr = RoGetActivationFactory( HStringReference( RuntimeClass_Windows_Data_Json_JsonValue ).Get(), __uuidof( IJsonValueStatics )
                                    , reinterpret_cast<void**>( parser.GetAddressOf() ) );
  auto body = http_content_utf8( L"application/json", data );
  if ( ok( hr ) )
    hr = parser->Parse( HStringReference( body.c_str() ).Get(), v );
  return hr;
}

std::vector<char> read_full( ISequentialStream *stream ) {
  std::vector<char> data;
  uint8_t buf[ 4096 ];
  ULONG readed = 0;
  auto hr = S_OK;
  do {
    hr = stream->Read( buf, sizeof( buf ), &readed );

    auto n = data.size();
    data.resize( n + readed );
    memcpy_s( data.data() + n, readed, buf, readed );
  } while ( ok( hr ) && readed > 0 );
  return std::move( data );
}


std::vector<std::wstring> string_lines( const wchar_t*content ) {
  std::vector<std::wstring> lines;
  const wchar_t* start = content, *end = content;
  while ( *end ) {
    while ( *end && *end != '\r' && *end != '\n' ) ++end;
    std::wstring line( start, end );
    lines.push_back( line );
    while ( *end && ( *end == '\r' || *end == '\n' ) ) ++end;
    start = end;
  }
  return std::move( lines );
}

std::tuple<std::wstring, std::wstring> string_split_2( const std::wstring &line, wchar_t sep ) {
  auto p = line.find_first_of( sep );
  return std::make_tuple( line.substr( 0, p ), line.substr( p ) );
}

bool is_not_space( wchar_t const&c ) {
  return c != ' ' && c != '\t';
}
std::wstring to_upper( std::wstring s ) {
  for ( auto &i : s ) i = towupper( i );
  return std::move( s );
}
std::wstring rtrim( std::wstring line ) {
  line.erase( std::find_if( line.rbegin(), line.rend(), is_not_space ).base(), line.end() );
  return std::move( line );
}

std::wstring ltrim( std::wstring line ) {
  line.erase( line.begin(), std::find_if( line.begin(), line.end(), is_not_space ) );
  return std::move( line );
}
std::wstring trim( std::wstring line ) {
  return rtrim( ltrim( line ) );
}
std::multimap<std::wstring, std::wstring> split_headers( const wchar_t*headers ) {
  std::multimap<std::wstring, std::wstring> v;
  auto fields = string_lines( headers );
  for ( auto i = fields.begin(); i != fields.end(); ++i ) {
    auto h = string_split_2( *i, L':' );
    v.emplace( to_upper( std::get<0>( h ) ), trim( std::get<1>( h ) ) );
  }
  return v;
}
