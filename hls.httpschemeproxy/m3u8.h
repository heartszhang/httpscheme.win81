#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace m3u8 {
using uint = uint32_t;
using string = std::wstring;
using uint64 = uint64_t;

 struct media_segment{
   string uri;  //absolute
 };
 struct media_playlist{
   string uri;  //absolute
   uint64 duration; //100 nano-secs
   std::vector<media_segment> segments;
 }
 struct master_playlist{
   string uri;
   uint64 duration;
   std::vector<media_playlist> medias;
 };

 auto decode(stream_reader *reader)->master_playlist;
// It applies to every Media Segment that appears after it in the
// Playlist until the next EXT-X-MAP tag or until the end of the
// playlist.
//
// Realizes EXT-MAP tag.
struct Map {
  string uri;
  int64  limit = 0;                 // <n> is length in bytes for the file under URI
  int64  offset = 0;                // [@o] is offset from the start of the file under URI
};

// This structure represents information about stream encryption.
//
// Realizes EXT-X-KEY tag.
struct Key {
  string method;
  string uri;
  string iv;
  string keyformat;
  string keyformatversions;
};

// This structure represents a media segment included in a media playlist.
// Media segment may be encrypted.
// Widevine supports own tags for encryption metadata.
struct MediaSegment {
  uint64 seqid;
  string title;         // optional second parameter for EXTINF tag
  string uri;
  uint64 duration;      //nano-secs        // first parameter for EXTINF tag, integer or float
  int64  limit;         // EXT-X-BYTERANGE <n> is length in bytes for the file under URI
  int64  offset;        // EXT-X-BYTERANGE [@o] is offset from the start of the file under URI
  bool   has_key = false;
  Key    key;           // EXT-X-KEY displayed before the segment and means changing of encryption key (in theory each segment may have own key)
  bool   has_map = false;
  Map    map  ;         // EXT-X-MAP displayed before the segment
  bool   discontinuity = false;         // EXT-X-DISCONTINUITY indicates an encoding discontinuity between the media segment that follows it and the one that preceded it (i.e. file format, number and type of tracks, encoding parameters, encoding sequence, timestamp sequence)
  //	time    programdatetime; // EXT-X-PROGRAM-DATE-TIME tag associates the first sample of a media segment with an absolute date and/or time
};

 using Segments = std::vector<MediaSegment>;
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
struct MediaPlaylist {
  uint64   duration;  //second -> 100nano-secs
  uint64   seqno;     //EXT-X-MEDIA-SEQUENCE
  string   args;      // null terminated // optional arguments placed after URIs (URI?Args)
  bool     iframe;    // EXT-X-I-FRAMES-ONLY
  bool     closed;    // is this VOD (closed) or Live (sliding) playlist?
  bool     is_vod = true;
  bool     is_live = false;
  bool     has_key;
  bool     has_map;
  int      keyformat; //aes-128?
  uint     winsize;   // max number of segments displayed in an encoded playlist; need set to zero for VOD playlists
  uint     capacity;  // total capacity of slice used for the playlist
  uint     head;      // head of FIFO, we add segments to head  // for live
  uint     tail;      // tail of FIFO, we remove segments from tail // for live
  uint     count;     // number of segments added to the playlist
  uint     ver;       // 
  Key      key;       // EXT-X-KEY is optional encryption key displayed before any segments (default key for the playlist)
  Map      map;       // EXT-X-MAP is optional tag specifies how to obtain the Media Initialization Section (default map for the playlist)
  Segments segments;
};

// This structure represents EXT-X-MEDIA tag in variants.
struct Alternative {
  string groupid;
  string uri;
  string type;
  string language;
  string name;
  bool   isdefault;
  string autoselect;
  string forced;
  string characteristics;
  string subtitles;
};
 using Alternatives = std::vector<Alternative>;
// This stucture represents additional parameters for a variant
// used in EXT-X-STREAM-INF and EXT-X-I-FRAME-STREAM-INF
struct VariantParams {
  uint         programid;
  uint         bandwidth;
  string       codecs;
  string       resolution;
  string       audio;           // EXT-X-STREAM-INF only
  string       video;
  string       subtitles;       // EXT-X-STREAM-INF only
  string       captions;        // EXT-X-STREAM-INF only
  bool         iframe;          // EXT-X-I-FRAME-STREAM-INF
  Alternatives alternatives;
};

// This structure represents variants for master playlist.
// Variants included in a master playlist and point to media playlists.
 struct Variant : VariantParams{
  string uri;
  MediaPlaylist chunk;
};

 using Variants = std::vector<Variant>;
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
//<duration>, <title>
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

uint64 decode_target_duration(utf8string line){
  return to_uint64(line) * 10000;  //100 nano-secs
}
uint64 decode_media_sequence(utf8string line){
  return to_uint64(line);
}

enum class xkey_method : uint{
  xkey_method_nothing = 0,
  xkey_method_none = 1,
  xkey_method_aes128 = 2
};
struct xkey{
  xkey_method method = xkey_method_none;
  std::vector<uint8_t> iv;
  string uri;
};
xkey decode_key(utf8string line){
  xkey v;
  auto params = to_params(line);
  if (params["METHOD"] == "AES-128"){
    v.method = xkey_method_aes128;
    v.iv = to_bytes(params["IV"]);
    v.uri = params["URI"];
  }
  return v;
}

uint64 decode_program_datetime(utf8string line){
  return to_unixtime(line);
}

bool decode_yesno(utf8string line){
  return line == "YES";
}

enum class playlist_type : uint{
  playlist_type_none = 0,
  playlist_type_event = 1,
  playlist_type_live = playlist_type_event,
  playlist_type_vod = 2,
};

playlist_type decode_playlist_type(utf8string line){
  auto v = playlist_type::playlist_type_none;
  if (line == "EVENT")
    v = playlist_type::playlist_type_event;
  if (line == "VOD")
    v = playlist_type::playlist_type_vod;
  return v;
}

struct xmedia{
  string uri;
  xmedia_type typ;
  
};
auto m3u8::decode_playlist(stream_reader*reader)->master_playlist{
  const char *t = nullptr;
  m3u8stack ctx;
  auto err = 0;
  auto line = reader->read_line(&err).trim();
  while(err >=0){    
    if (is_blank(line))continue;
    if (line == "#EXTM3U"){
      ctx.m3u8 = true;
    }else if(has_prefix(line, t = "#EXTINF:")){
      ctx.is_media_list = true;
      ctx.inf = decode_inf(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-BYTERANGE:")){
      //      ctx.is_media_list = true;
      ctx.target_duration = decode_target_duration(suffix(line, t));
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
      ctx.media = decode_media(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-STREAM-INF:")){
      ctx.stream_inf = decode_stream_inf(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-DISCONTINUITY:")){
      ctx.discontinuity = true;
    }else if(has_prefix(line, t = "#EXT-X-I-FRAMES-ONLY:")){
      ctx.iframes_only = true;
    }else if(has_prefix(line, t = "#EXT-X-I-FRAMES-STREAM-INF:")){
      ctx.iframe_inf = decode_iframe_stream_inf(suffix(line, t));
    }else if(has_prefix(line, t = "#EXT-X-VERSION:")){
      ctx.ver = decode_version(suffix(line, t));
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
