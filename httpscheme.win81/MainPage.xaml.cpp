//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace httpscheme_win81;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
  InitializeComponent();
  memanager = ref new Windows::Media::MediaExtensionManager();
//  memanager->RegisterSchemeHandler( L"hls.httpschemeproxy.HttpSchemeHandler", L"http:" );
//  memanager->RegisterByteStreamHandler( L"hls.httpschemeproxy.ByteStreamProxyHandler", L".ts", L"video/MP2T" );
  memanager->RegisterByteStreamHandler( L"hls.httpschemeproxy.ByteStreamProxyHandler", nullptr, L"application/x-mpegURL" );
//  memanager->RegisterByteStreamHandler( L"hls.httpschemeproxy.ByteStreamProxyHandler", L".ts", L"video/MP2T" );
//  memanager->RegisterByteStreamHandler( L"hls.httpschemeproxy.ByteStreamProxyHandler", L".ts", L"video/MP2T" );
  media_element->Source = ref new Uri( L"http://gslb.bestvcdn.com.cloudcdn.net/218.77.90.60/gslb/program/Dbackct_bestvcdn_comD/_9Xxr00bTIO_/FDN/FDNB1690862/700/stream.m3u8?_cp=1&_back=FASTWEB&taskID=pb02.idc.xbox.bestv.com.cn_1428984407481_023513000004901" );
//  media_element->Source = ref new Uri( L"http://devimages.apple.com/iphone/samples/bipbop/gear1/fileSequence0.ts" );
}
