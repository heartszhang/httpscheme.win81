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
  memanager->RegisterSchemeHandler( L"hls.httpschemeproxy.HttpSchemeHandler", L"http:" );
  media_element->Source = ref new Uri(L"http://techslides.com/demos/sample-videos/small.mp4");
}
