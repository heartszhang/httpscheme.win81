#pragma once
#include <string>
#include <map>
#include <vector>

auto dump( const wchar_t*fmt, ... )->void;
auto http_content_string( const std::wstring, const std::vector<char>&data )->std::wstring;
auto split_headers( const wchar_t*headers )->std::multimap<std::wstring, std::wstring>;
auto content_json( const std::vector<char>&data, IJsonValue**v )->HRESULT;
auto read_full( ISequentialStream *stream )->std::vector<char>;
