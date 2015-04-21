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
using xkey = params_t;

struct __declspec( novtable ) line_reader {
  virtual utf8string read_line(int *err) = 0;
  virtual bool eof() = 0;
};
params_t to_params2( utf8string const&line );

xkey decode_key( utf8string const&line );

// This structure represents a media segment included in a media playlist.
// Media segment may be encrypted.
struct media_segment {
  uint64 seqno;
  string title;// optional second parameter for EXTINF tag
  string uri;  //absolute
  uint64 duration;        //nano-secs        // first parameter for EXTINF tag, integer or float
  uint64  limit = 0-1ull; // EXT-X-BYTERANGE <n> is length in bytes for the file under URI
  uint64  offset = 0;     // EXT-X-BYTERANGE [@o] is offset from the start of the file under URI
  xkey    key;            // EXT-X-KEY displayed before the segment and means changing of encryption key (in theory each segment may have own key)
  bool   discontinuity = false; // EXT-X-DISCONTINUITY indicates an encoding discontinuity between the media segment that follows 
  // EXT-X-PROGRAM-DATE-TIME tag associates the first sample of a media segment with an absolute date and/or time
};

struct media_playlist{
  uint64   duration;  //second -> 100nano-secs
  uint64   seqno;     //EXT-X-MEDIA-SEQUENCE
  bool     iframe;    // EXT-X-I-FRAMES-ONLY
  bool     vod;       // vod or live else
  bool     has_key;
  bool     has_map;
  uint     winsize;   // max number of segments displayed in an encoded playlist; need set to zero for VOD playlists
  uint     count;     // number of segments added to the playlist
  uint     ver;       // 
  xkey     key;       // EXT-X-KEY is optional encryption key displayed before any segments (default key for the playlist)
  string   uri;  //absolute
  string    title;
  std::vector<media_segment> segments;
};

 struct master_playlist{
   uint64     ver;
   string     uri;
   uint64     duration;
   std::vector<media_playlist> medias;
 };
 master_playlist decode_playlist( line_reader*reader );
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
 
}
