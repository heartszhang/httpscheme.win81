#pragma once
#include <vector>
#include <map>
#include <string>
#include <cstdint>

namespace m3u8 {
using uint = uint32_t;
using string = std::wstring;
using uint64 = uint64_t;
using utf8string = std::string;

using params_t = std::map < utf8string, utf8string > ;
//METHOD, URI, IV
using xkey = params_t;

struct token_reader {
  int err;
  int begin;
  const utf8string &line;
  utf8string token() {
    utf8string n;
    if ( err != 0 ) return n;
    for ( ; begin < line.size(); begin++ ) {
      if ( line[ begin ] != '=' )
        n.push_back( line[ begin ] );
    }
    return n;
  }
  explicit token_reader(utf8string const&line) :line(line) {}
  void expect_eq() {
    if ( err != 0 )return;
    if ( begin >= line.size() || line[begin] != '=') 
      err = -2;   
    ++begin;
    return;
  }
  utf8string value() {
    utf8string v;
    if ( err != 0 ) return v;
    if ( begin + 1 == line.size() || line[begin] == ',') {
      ++begin;
      return;
    }
    if ( line[ begin ] == '"' ) {
      v = string_value();
      return;
    }
    for ( ; begin < line.size() && line[begin] != ','; ++begin ) {
      v.push_back( line[ begin ] );
    }
    if ( begin < line.size() ) ++begin;
  }
  utf8string string_value() {
    utf8string v;
    ++begin;
    for ( ; begin < line.size(); ++begin ) {

    }
  }
};
//comma in attribute-value should be escaped, but we dont process it
//AttributeName=AttributeValue,<AttributeName=AttributeValue>*
params_t to_params( utf8string const&line ) {
  params_t v;
  token_reader reader( line );
  while ( reader.err == 0 && reader.begin < line.size()) {
    auto name = reader.token();
    reader.expect_eq();
    auto val = reader.value();
    v[ name ] = val;
  }
  return v;
}

xkey decode_key(utf8string line){
  return to_params(line);
}

// This structure represents a media segment included in a media playlist.
// Media segment may be encrypted.
struct media_segment {
  uint64 seqno;
  string title;         // optional second parameter for EXTINF tag
  string uri;  //absolute
  uint64 duration;      //nano-secs        // first parameter for EXTINF tag, integer or float
  uint64  limit = 0-1ull;         // EXT-X-BYTERANGE <n> is length in bytes for the file under URI
  uint64  offset = 0;        // EXT-X-BYTERANGE [@o] is offset from the start of the file under URI
  xkey    key;           // EXT-X-KEY displayed before the segment and means changing of encryption key (in theory each segment may have own key)
  bool   discontinuity = false;         // EXT-X-DISCONTINUITY indicates an encoding discontinuity between the media segment that follows it and the one that preceded it (i.e. file format, number and type of tracks, encoding parameters, encoding sequence, timestamp sequence)
  //	time    programdatetime; // EXT-X-PROGRAM-DATE-TIME tag associates the first sample of a media segment with an absolute date and/or time
};
struct media_playlist{
  uint64   duration;  //second -> 100nano-secs
  uint64   seqno;     //EXT-X-MEDIA-SEQUENCE
  bool     iframe;    // EXT-X-I-FRAMES-ONLY
  bool     vod;    // vod or live else
  bool     has_key;
  bool     has_map;
  uint     winsize;   // max number of segments displayed in an encoded playlist; need set to zero for VOD playlists
  uint     count;     // number of segments added to the playlist
  uint     ver;       // 
  xkey     key;       // EXT-X-KEY is optional encryption key displayed before any segments (default key for the playlist)
  string uri;  //absolute
  std::vector<media_segment> segments;
}
 struct master_playlist{
   uint64     ver;
   string uri;
   uint64 duration;
   std::vector<media_playlist> medias;
 };

/*
This structure represents a single bitrate playlist aka media playlist.
It related to both a simple media playlists and a sliding window media playlists.
URI lines in the Playlist point to media segments.
Simple Media Playlist file sample:
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-TARGETDURATION:5220
#EXTINF:5219.2,
http://media.example.com/entire.ts
#EXT-X-ENDLIST
Sample of Sliding Window Media Playlist, using HTTPS:
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-TARGETDURATION:8
#EXT-X-MEDIA-SEQUENCE:2680
#EXTINF:7.975,
https://priv.example.com/fileSequence2680.ts
#EXTINF:7.941,
https://priv.example.com/fileSequence2681.ts
#EXTINF:7.975,
https://priv.example.com/fileSequence2682.ts
*/

/*
This structure represents a master playlist which combines media playlists for multiple bitrates.
URI lines in the playlist identify media playlists.
Sample of Master Playlist file:
#EXTM3U
#EXT-X-STREAM-INF:PROGRAM-ID=1,BANDWIDTH=1280000
http://example.com/low.m3u8
#EXT-X-STREAM-INF:PROGRAM-ID=1,BANDWIDTH=2560000
http://example.com/mid.m3u8
#EXT-X-STREAM-INF:PROGRAM-ID=1,BANDWIDTH=7680000
http://example.com/hi.m3u8
#EXT-X-STREAM-INF:PROGRAM-ID=1,BANDWIDTH=65000,CODECS="mp4a.40.5"
http://example.com/audio-only.m3u8
*/
struct MasterPlaylist {
  Variants variants;
  string   args;                // optional arguments placed after URI (URI?Args)
  string   cypherver;           // non-standard tag for Widevine (see also WV struct)
  uint     ver;
 };

// int DecodeMasterPlaylist(stream_reader* reader, MasterPlaylist* val);
// int DecodeMediaPlaylist(stream_reader*reader, MediaPlaylist* val);
 std::tuple<MasterPlaylist, int> DecodeMasterPlaylist(stream_reader *reader);
 std::tuple<MediaPlaylist, int> DecodeMediaPlaylist(stream_reader*reader);
 
 //std::tuple<MasterPlaylist, int> DecodePlaylist(stream_reader *reader);
 int DeterminePlaylist(stream_reader *reader);//<0 failed, 1: master, 2 :media, 0 undefined
}

int m3u8::DeterminePlaylist(StreamReader * reader){
  auto i = reader->ReadLine();
  while(std::get<1>(i) == 0){
    auto line = std::get<0>(i);
    if (is_blank(line))continue;
    if (has_prefix(line, L"#EXT-X-STREAM-INF"))return 1;
    if (has_prefix(line, L"#EXTINF")) return 2;
    i = reader->ReadLine();
  }
  return i.ok
}

std::tuple<MasterPlaylist, int> DecodeMasterPlaylist(StreamReader*reader){
  MasterPlaylist pl;
  int r = -1;
  auto i = reader->ReadLine();
  while(std::get<1>(i) == 0){
    
    i = reader->ReadLine();
  }
  return std::make_tuple(pl,r);
}

struct m3u8_status{
  bool m3u = false;
};
auto m3u8::decode_master_playlist(stream_reader*reader)->master_playlist{
  master_playlist ret;
  string line;
  auto i = reader->read_line(line);
  while (i>=0){  // no eof or error
    line = trim_space(line);
    if (line == L"#EXTM3U")
      state.m3u = true;
    else if(has_prefix(line, L"#EXT-X-VERSION:")){
      state.master = true;
      state.ver = to_int(suffix(line, 15));  // just ignore it, 15:length(L"#EXT-X-VERSION:")
      
    }else if(has_prefix(line, L"#EXT-X-MEDIA:")){  // ignore it
    }else if (has_prefix(line, L"#EXT-X-STREAM-INF:")){
      state.master = true;
      state.params = decode_params(suffix(line, 18)); // 18 = lenght(L"#EXT-X-STREAM-INF:");
    }
    i = reader->read_line(line);
  }
}

struct extinf{
  uint64 duration = 0;
  string title;
};
//#EXTINF:<duration>,<title> , duration is integer or float number in decimal, seconds
extinf decode_inf(utf8string line){
  auto fields = string_split(line, ",");
  extinf v;
  if (fields.length() >= 2){
    v.title = utf82unicode(fields[1]);
  }else if(fields.length() == 1){
    v.duration = static_cast<uint64>(to_float64(fields[0]) * 10000); // 100 nano-secs
  }
  return v;
}
//#EXT-X-TARGETDURATION:<s>
uint64 decode_target_duration(utf8string line){
  return to_uint64(line) * 10000;  //100 nano-secs
}
//offset == -1 means it isn't present
struct byte_range{
  uint64 bytes = 0ull;
  uint64 offset = 0-1ull;
};
// #EXT-X-BYTERANGE:<n>[@o]
byte_range decode_byte_range(utf8string line){
  byte_range v;
  auto fields = string_split(line, "@");
  if (fields.length() == 2){
    v.offset = to_uint64(fields[1]);
  }else if(fields.length() == 1){
    v.bytes = to_uint64(fields[0]);
  }
  return v;
}

//#EXT-X-MEDIA-SEQUENCE:<number>
uint64 decode_media_sequence(utf8string line){
  return to_uint64(line);
}

// #EXT-X-PROGRAM-DATE-TIME:<YYYY-MM-DDThh:mm:ssZ>
uint64 decode_program_datetime(utf8string line){
  return to_unixtime(line);
}

bool decode_yesno(utf8string line){
  return line == "YES";
}

// #EXT-X-PLAYLIST-TYPE:<EVENT|VOD>
playlist_type decode_playlist_type(utf8string line){
  return line;
}

using xmedia = std::map<utf8string,utf8string>;
using xstream_inf = std::map<utf8string, utf8string>;
using iframe_stream_inf = std::map<utf8string, utf8string>;

auto m3u8::decode_playlist(stream_reader*reader)->master_playlist{
  const char *t = nullptr;
  m3u8stack ctx;
  auto err = 0;
  auto line = reader->read_line(&err).trim();
  while(err >=0){    
    if (is_blank(line))continue;
    if (line == "#EXTM3U"){
      ctx.m3u8 = true;
    }else if(has_prefix(line, t = "#EXTINF:")){//#EXTINF:<duration>,<title>
      ctx.is_media_list = true;
      ctx.inf = decode_inf(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-BYTERANGE:")){
      //      ctx.is_media_list = true;
      auto br = decode_byte_range(suffix(line, t));
      if (br.offset == 0-1ull){
        br.offset = ctx.byte_range.offset + ctx.byte_range.bytes;
      }
    }else if(has_prefix(line, t = "#EXT-X-TARGETDURATION:")){
      //      ctx.is_media_list = true;
      ctx.target_duration = decode_target_duration(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-MEDIA-SEQUENCE:")){
      //ctx.is_media_list = true;
      ctx.media_sequence = decode_media_sequence(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-KEY:")){
      ctx.key = decode_key(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-PROGRAM-DATE-TIME:")){
      ctx.program_datetime = decode_program_datetime(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-ALLOW-CACHE:")){
      ctx.allow_cache = decode_yesno(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-PLAYLIST-TYPE:")){
      ctx.playlist_type = decode_playlist_type(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-ENDLIST:")){
      ctx.end_list = true; // media-playlist 
    }else if(has_prefix(line, t = "#EXT-X-MEDIA:")){
      ctx.medias.push_back(decode_params(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-STREAM-INF:")){
      ctx.stream_inf = decode_params(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-DISCONTINUITY:")){
      ctx.discontinuity = true;
    }else if(has_prefix(line, t = "#EXT-X-I-FRAMES-ONLY:")){
      ctx.iframes_only = true;
    }else if(has_prefix(line, t = "#EXT-X-I-FRAMES-STREAM-INF:")){
      ctx.iframe_inf = decode_params(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-VERSION:")){
      ctx.ver = to_uint64(suffix(line, t));
    }else if(has_prefix(line, t = "#")){
    }else{
      //segment
      //media-playlist
      if (ctx.is_media_list){
        auto p = make_segment(ctx, line);
        ctx.media_playlist.segments.push_back(p);
        ctx.media_sequence++;
      }
      if (ctx.is_master){
        auto p = make_media_list(ctx, line);
        ctx.master_playlist.media_list.push_back(p);
      }
      ctx.byte_range.reset();
      ctx.discontinuty = false;  // reset
      ctx.program_datetime = 0;  //unixtimestamp
    }
    line = reader->read_line(&err).trim();
  }
  if (err != eof){
    ctx.master_playlist.err = err;
  }
  if (ctx.is_media_list){
    ctx.master_playlist.medias.push_back(ctx.media_playlist);
  }
 
  return ctx.master_playlit;
}
