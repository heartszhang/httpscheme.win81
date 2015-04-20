#pragma once
#include <string>
#include <vector>

struct MediaPlaylist {
  std::wstring uri;
  std::vector<std::wstring> segments;
};
