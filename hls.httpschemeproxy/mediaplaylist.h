#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct MediaPlaylist {
  std::wstring uri;
  std::vector<std::wstring> segments;
  uint64_t duration;
};

