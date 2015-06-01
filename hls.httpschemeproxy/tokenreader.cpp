#include "pch.h"
#include "attrreader.h"
#include <sstream>
inline bool is_name_char( char c ) {
  return ( c >= 'A'&&c <= 'Z' ) || c == '-';
}
namespace m3u8 {
//   AttributeName=AttributeValue[,AttributeName=AttributeValue]*
//   Certain extended M3U tags have values which are Attribute Lists.  An
//   Attribute List is a comma-separated list of attribute/value pairs
//   with no whitespace.
struct attrlist_reader {
  int err = 0;                        //-1:eof, 0: ok, other: failed
  int begin = 0;
  const utf8string &line;
  utf8string attrname() { //[A..Z-]    
    if ( err != 0 ) return utf8string();
    auto p = begin;
    for ( ; begin < line.size() && is_name_char(line[begin]); ++begin ) {}
    return line.substr( p, begin - p );    
  }
  explicit attrlist_reader( utf8string const&line ) :line( line ) {}
  void expect_eq() {
    if ( begin < line.size() && line[ begin ] == '=' )
      ++begin;
    else err = -2;
    return;
  }
  void expect_comma() {
    if ( err != 0 )return;
    if ( begin < line.size() && line[ begin ] == ',' )
      ++begin;
    else if ( begin < line.size() )
      err = -4;
  }
  utf8string attrvalue() {    
    if ( err != 0 ) return utf8string();
    if ( line[ begin ] == '"' )
      return dequote();
    auto p = begin;
    for ( ; begin < line.size() && line[ begin ] != ','; ++begin ) {}
    return line.substr( p, begin -p );    
  }
  utf8string dequote() {
    std::ostringstream os;
    ++begin;  //ignore the first "
    for ( char c = 0; begin < line.size() && (c = line[begin]) != '"'; ++begin ) {
      if ( c == '\\' ) {
        if ( ++begin >= line.size() )
          break;
        switch (c = line[ begin ]) {
        case 'r': c = '\r'; break;
        case 'n': c = '\n'; break;
        default:break;
        }
      }
      os << c;
    }
    return os.str();
  }
private:
  attrlist_reader operator=( const attrlist_reader& ) = delete;
};
}
using namespace m3u8;
params_t to_params( utf8string const&line ) {
  params_t v;
  attrlist_reader reader( line );
  while ( reader.err == 0 && reader.begin < line.size() ) {
    auto name = reader.attrname();
    reader.expect_eq();
    auto val = reader.attrvalue();
    if (reader.err == 0 )
      v[ name ] = val;
    reader.expect_comma();
  }
  return v;
}
