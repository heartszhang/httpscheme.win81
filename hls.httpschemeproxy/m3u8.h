#include "pch.h"
#include "m3u8.h"

using namespace m3u8;

struct m3u8stack {
  bool m3u8 = false;
  bool is_media_list = false;
  uint64 inf_duration = 0;
  string inf_title;
  uint64 byte_range_n = 0;
  uint64 byte_range_offset = 0;
};

utf8string trim( utf8string line );
bool has_prefix( utf8string const&line, utf8string const&prefix );
std::vector<utf8string> string_split( utf8string const&line, char sep );
string utf82unicode( utf8string const& );
double to_float64( utf8string const&line );
utf8string suffix( utf8string const&line, utf8string const&prefix );
uint64 to_uint64( utf8string const&line );

std::tuple<uint64, string> decode_inf( utf8string const&line );
std::tuple<uint64, uint64> decode_byte_range( utf8string const&line ); // n = 0 : means no byte-range, offset = -1 means, use previous offset
// uint64 decode_media_sequence( utf8string const&line );
uint64 decode_program_datetime( utf8string const&line );
bool   decode_yesno( utf8string const&line );


//#EXT-X-MEDIA-SEQUENCE:<number>
uint64 decode_media_sequence( utf8string const&line ) {
  return to_uint64( line );
}

// #EXT-X-PROGRAM-DATE-TIME:<YYYY-MM-DDThh:mm:ssZ>
uint64 decode_program_datetime( utf8string const&line ) {
  return 0; // not implemented yet
  //return to_unixtime( line );
}

bool decode_yesno( utf8string const&line ) {
  return trim(line) == "YES";
}

//#EXT-X-TARGETDURATION:<s>
uint64 decode_target_duration( utf8string line ) {
  return to_uint64( line ) * 10000;  //100 nano-secs
}
//#EXTINF:<duration>,<title> , duration is integer or float number in decimal, seconds
std::tuple<uint64, string> decode_inf( utf8string const&line ) {
  auto fields = string_split( line, ',' );
  uint64 d = 0;
  string t;
  if ( fields.size() >= 2 ) {
    t = utf82unicode( fields[ 1 ] );
  } else if ( fields.size() == 1 ) {
    d = static_cast<uint64>( to_float64( fields[ 0 ] ) * 10000 ); // 100 nano-secs
  }
  return std::make_tuple(d, t);
}
master_playlist m3u8::decode_playlist( line_reader*reader ) {
  const char *t = nullptr;
  m3u8stack ctx;
  auto err = 0;
  auto line = trim(reader->read_line( &err ));
  while ( err >= 0 ) {
    if ( line.empty() )continue;
    if ( line == "#EXTM3U" ) {
      ctx.m3u8 = true;
    } else if ( has_prefix( line, t = "#EXTINF:" ) ) {//#EXTINF:<duration>,<title>
      ctx.is_media_list = true;
      std::tie(ctx.inf_duration, ctx.inf_title) = decode_inf( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-BYTERANGE:" ) ) {
      //      ctx.is_media_list = true;
      auto br = decode_byte_range( suffix( line, t ) );
      if ( std::get<1>(br) != 0 - 1ull ) {// offset
        ctx.byte_range_offset = std::get<1>( br );        
      }
      ctx.byte_range_n = std::get<0>( br );
    } else if ( has_prefix( line, t = "#EXT-X-TARGETDURATION:" ) ) {
      //      ctx.is_media_list = true;
      ctx.target_duration = decode_target_duration( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-MEDIA-SEQUENCE:" ) ) {
      //ctx.is_media_list = true;
      ctx.media_sequence = decode_media_sequence( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-KEY:" ) ) {
      ctx.key = decode_key( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-PROGRAM-DATE-TIME:" ) ) {
      ctx.program_datetime = decode_program_datetime( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-ALLOW-CACHE:" ) ) {
      ctx.allow_cache = decode_yesno( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-PLAYLIST-TYPE:" ) ) {
      ctx.playlist_type = decode_playlist_type( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-ENDLIST:" ) ) {
      ctx.end_list = true; // media-playlist 
    } else if ( has_prefix( line, t = "#EXT-X-MEDIA:" ) ) {
      ctx.medias.push_back( decode_params( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-STREAM-INF:" ) ) {
      ctx.stream_inf = decode_params( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-DISCONTINUITY:" ) ) {
      ctx.discontinuity = true;
    } else if ( has_prefix( line, t = "#EXT-X-I-FRAMES-ONLY:" ) ) {
      ctx.iframes_only = true;
    } else if ( has_prefix( line, t = "#EXT-X-I-FRAMES-STREAM-INF:" ) ) {
      ctx.iframe_inf = decode_params( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#EXT-X-VERSION:" ) ) {
      ctx.ver = to_uint64( suffix( line, t ) );
    } else if ( has_prefix( line, t = "#" ) ) {
    } else {
      //segment
      //media-playlist
      if ( ctx.is_media_list ) {
        auto p = make_segment( ctx, line );
        ctx.media_playlist.segments.push_back( p );
        ctx.media_sequence++;
      }
      if ( ctx.is_master ) {
        auto p = make_media_list( ctx, line );
        ctx.master_playlist.media_list.push_back( p );
      }
      ctx.byte_range_n = 0;
      ctx.discontinuty = false;  // reset
      ctx.program_datetime = 0;  //unixtimestamp
    }
    line = reader->read_line( &err ).trim();
  }
  if ( err != eof ) {
    ctx.master_playlist.err = err;
  }
  if ( ctx.is_media_list ) {
    ctx.master_playlist.medias.push_back( ctx.media_playlist );
  }

  return ctx.master_playlit;
}

params_t m3u8::to_params2( utf8string const&line ) {
  params_t v;
  auto fields = string_split( line, ',' );
  for ( auto i = fields.begin(); i != fields.end(); ++i ) {
    auto nv = string_split( *i, '=' );
    if ( nv.length() != 2 )
      continue;
    v[ trim( nv[ 0 ] ) ] = nv[ 1 ];
  }
  return v;
}

xkey decode_key( utf8string line ) {
  return to_params( line );
}
// #EXT-X-BYTERANGE:<n>[@o]
std::tuple<uint64, uint64> decode_byte_range( utf8string const&line ) {
  uint64 n = 0, offset = 0 - 1ull;
  auto fields = string_split( line, '@' );
  if ( fields.size() == 2 ) {
    offset = to_uint64( fields[ 1 ] );
  }
  n = to_uint64( fields[ 0 ] );

  return std::make_tuple( n, offset );
}
