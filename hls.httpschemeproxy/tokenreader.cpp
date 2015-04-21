#include "pch.h"
#include "m3u8.h"
namespace m3u8 {
struct token_reader {
  int err;
  int begin;
  const utf8string &line;
  utf8string token() {
    utf8string n;
    if ( err != 0 ) return n;
    for ( ; begin < line.size(); begin++ ) {
      if ( line[ begin ] != '=' && !is_space( line[ begin ] ) )
        n.push_back( line[ begin ] );
    }
    return n;
  }
  explicit token_reader( utf8string const&line ) :line( line ) {}
  void expect_eq() {
    if ( err != 0 )return;
    for ( ; begin < line.size() && is_space( line[ begin ] ); ++begin );
    if ( begin >= line.size() || line[ begin ] != '=' )
      err = -2;
    ++begin;
    for ( ; begin < line.size() && is_space( line[ begin ] ); ++begin );

    return;
  }
  void expect_comma() {
    if ( err != 0 )return;
    for ( ; begin < line.size() && is_space( line[ begin ] ); ++begin );
    if ( begin < line.size() && line[ begin ] == ',' )
      ++begin;
    for ( ; begin < line.size() && is_space( line[ begin ] ); ++begin );
  }
  utf8string value() {
    utf8string v;
    if ( err != 0 ) return v;
    if ( begin + 1 == line.size() || line[ begin ] == ',' ) {
      ++begin;
      return;
    }
    if ( line[ begin ] == '"' ) {
      v = string_value();
    } else {
      for ( ; begin < line.size() && line[ begin ] != ',' && !is_space( line[ begin ] ); ++begin ) {
        v.push_back( line[ begin ] );
      }
    }
    //   for ( ; begin < line.size() && is_space( line[ begin ] ); ++begin );
    //    if ( begin < line.size() ) ++begin;
  }
  utf8string string_value() {
    utf8string v;
    ++begin;
    auto x = false;
    for ( ; begin < line.size(); ++begin ) {
      if ( x == true ) {
        v.push_back( line[ begin ] );
        x = false;
        continue;
      }
      if ( line[ begin ] == '\\' ) {
        x = true;
        continue;
      }
      if ( line[ begin ] == '"' ) {
        break;
      }
      v.push_back( line[ begin ] );
    }
    if ( begin >= line.size() || line[ begin ] != '"' )
      err = -3;
    ++begin;
    return v;
  }
};
params_t to_params( utf8string const&line ) {
  params_t v;
  token_reader reader( line );
  while ( reader.err == 0 && reader.begin < line.size() ) {
    auto name = reader.token();
    reader.expect_eq();
    auto val = reader.value();
    v[ name ] = val;
    reader.expect_comma();
  }
  return v;
}
}
//comma in attribute-value should be escaped, but we dont process it
//AttributeName=AttributeValue,<AttributeName=AttributeValue>*
