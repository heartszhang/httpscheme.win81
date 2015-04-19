#include <pch.h>
#include "hls.httpschemeproxy.h"
#include <strsafe.h>
//#include <propvarutil.h>

void dump( const wchar_t*fmt, ... ) {
  wchar_t buf[ 4096 ];
  va_list va;
  va_start( va, fmt );
  auto r = StringCchVPrintfW( buf, _countof( buf ), fmt, va );  // ignore return value
  va_end( va );
  if ( ok( r ) || r == STRSAFE_E_INSUFFICIENT_BUFFER )
    OutputDebugStringW( buf );
}

void dump_chars( DWORD chars ) {
  const auto p0 = L"-";
  auto p1 = L"MFMEDIASOURCE_IS_LIVE";
  auto p2 = L"MFMEDIASOURCE_CAN_SEEK";
  auto p4 = L"MFMEDIASOURCE_CAN_PAUSE";
  auto p8 = L"MFMEDIASOURCE_HAS_SLOW_SEEK";
  auto p10 = L"MFMEDIASOURCE_HAS_MULTIPLE_PRESENTATIONS";
  auto p20 = L"MFMEDIASOURCE_CAN_SKIPFORWARD";
  auto p40 = L"MFMEDIASOURCE_CAN_SKIPBACKWARD";
  auto p80 = L"MFMEDIASOURCE_DOES_NOT_USE_NETWORK";
  if ( ( chars & MFMEDIASOURCE_IS_LIVE ) == 0 ) p1 = p0;
  if ( ( chars & MFMEDIASOURCE_CAN_SEEK ) == 0 ) p2 = p0;
  if ( ( chars & MFMEDIASOURCE_CAN_PAUSE ) == 0 ) p4 = p0;
  if ( ( chars & MFMEDIASOURCE_HAS_SLOW_SEEK ) == 0 ) p8 = p0;
  if ( ( chars & MFMEDIASOURCE_HAS_MULTIPLE_PRESENTATIONS ) == 0 ) p10 = p0;
  if ( ( chars &MFMEDIASOURCE_CAN_SKIPFORWARD ) == 0 ) p20 = p0;
  if ( ( chars &MFMEDIASOURCE_CAN_SKIPBACKWARD ) == 0 ) p40 = p0;
  if ( ( chars & MFMEDIASOURCE_DOES_NOT_USE_NETWORK ) == 0 ) p80 = p0;

  dump( L"characters: %s %s %s %s %s %s %s %s\n", p1, p2, p4, p8, p10, p20, p40, p80 );
}

void mfguid_string( GUID const &guid, wchar_t *buf, size_t len ) {
  auto found = is_known_guid( guid, buf, len );
  if ( !found )
    StringFromGUID2( guid, buf, static_cast<int>(len ));
}
void mfprop_string(PROPVARIANT const*val, wchar_t*buf, size_t len) {
  auto hr = S_OK;
  switch ( val->vt ) {
  case VT_UI1:
  hr = StringCchPrintfW( buf, len, L"%d", val->bVal);  
  break;
  case VT_I2:
  hr = StringCchPrintfW( buf, len, L"%d", val->iVal );
  break;
  case VT_UI2:
  hr = StringCchPrintfW( buf, len, L"%d", val->uiVal );
  break;
  case VT_I4:
  hr = StringCchPrintfW( buf, len, L"%d", val->lVal );
  break;
  case VT_UI4:
  hr = StringCchPrintfW( buf, len, L"%ld", val->ulVal );
  break;
  case VT_I8:
  hr = StringCchPrintfW( buf, len, L"%llld", val->hVal );
  break;
  case VT_UI8:
  hr = StringCchPrintfW( buf, len, L"%lld", val->uhVal );
  break;
  case VT_R4:
  hr = StringCchPrintfW( buf, len, L"%f", val->fltVal );
  break;
  case VT_R8:
  hr = StringCchPrintfW( buf, len, L"%lf", val->dblVal );
  break;
  case VT_CLSID:
  //hr = StringCchPrintfW( buf, len, L"VT_CLSID:%d", val->puuid );
  mfguid_string( *val->puuid, buf, len );
  break;
  case VT_BSTR:
  hr = StringCchPrintfW( buf, len, L"%s", val->bstrVal );
  break;
  case VT_LPWSTR:
  hr = StringCchPrintfW( buf, len, L"%s", val->pwszVal );
  break;
  case VT_UNKNOWN:
  hr = StringCchPrintfW( buf, len, L"%p", val->punkVal );
  break;
  default:
  hr = StringCchPrintfW( buf, len, L"type:%d", val->vt );
  break;
  }
  hr;
}
void dump_prop( GUID const &key, PROPVARIANT*val ) {
  wchar_t name[ 128 ] = L"";
  wchar_t value[ 4096 ] = L"";
  mfguid_string( key, name, _countof( name ) );
  mfprop_string( val, value, _countof( value ) );
  dump( L"%s = %s\n", name, value );
}
void dump_attrs( IMFAttributes *attrs ) {
  UINT32 count = 0;
  GUID key;
  auto hr = attrs->GetCount( &count );
  for ( UINT32 i = 0; i < count; i++ ) {
    PropVariant prop{};
    auto hr = attrs->GetItemByIndex( i, &key, &prop );
    if ( ok( hr ) )
      dump_prop( key, &prop );
  }
}
void dump_mediatype( IMFMediaType *mtype ) {
  dump( L"media-type\n" );
  dump_attrs( mtype );
}
void dump_sd(IMFStreamDescriptor* sd) {
  dump( L"stream-desc\n" );
  dump_attrs( sd );
  DWORD mtcount = 0;
  ComPtr<IMFMediaTypeHandler> mthandler;
  auto hr = sd->GetMediaTypeHandler( &mthandler );
  if ( ok( hr ) )
    hr = mthandler->GetMediaTypeCount( &mtcount );
  for ( DWORD i = 0; i < mtcount; i++ ) {
    ComPtr<IMFMediaType> mtype;
    auto hr = mthandler->GetMediaTypeByIndex( i, &mtype );
    if ( ok( hr ) )
      dump_mediatype( mtype.Get() );
  }

}
void dump_pd( IMFPresentationDescriptor *pd ) {
  dump( L"pres-desc\n" );
  dump_attrs( pd );
  DWORD scount = 0;
  auto hr = pd->GetStreamDescriptorCount( &scount );
  for ( DWORD i = 0; i < scount; i++ ) {
    ComPtr<IMFStreamDescriptor> sd;
    BOOL selected;

    auto hr = pd->GetStreamDescriptorByIndex( i, &selected, &sd );
    if ( ok( hr ) )
      dump_sd( sd.Get() );
  }
}
static struct event_name {
  __MIDL___MIDL_itf_mfobjects_0000_0012_0001  id;
  const wchar_t* name;
}event_names[] = { { MEUnknown, L"MEUnknown" }
, { MEError, L"MEError" }
, { MEExtendedType, L"MEExtendedType" }
, { MENonFatalError, L"MENonFatalError" }
, { MEGenericV1Anchor, L"MEGenericV1Anchor" }
, { MESessionUnknown, L"MESessionUnknown" }
, { MESessionTopologySet, L"MESessionTopologySet" }
, { MESessionTopologiesCleared, L"MESessionTopologiesCleared" }
, { MESessionStarted, L"MESessionStarted" }
, { MESessionPaused, L"MESessionPaused" }
, { MESessionStopped, L"MESessionStopped" },
{MESessionClosed, L"MESessionClosed"},
{ MESessionEnded, L"MESessionEnded" },
{ MESessionRateChanged, L"MESessionRateChanged" },
{ MESessionScrubSampleComplete, L"MESessionScrubSampleComplete" },
{ MESessionCapabilitiesChanged, L"MESessionCapabilitiesChanged" },
{ MESessionTopologyStatus, L"MESessionTopologyStatus" },
{ MESessionNotifyPresentationTime, L"MESessionNotifyPresentationTime" },
{ MENewPresentation, L"MENewPresentation" },
{ MELicenseAcquisitionStart, L"MELicenseAcquisitionStart" },
{ MELicenseAcquisitionCompleted, L"MELicenseAcquisitionCompleted" },
{ MEIndividualizationStart, L"MEIndividualizationStart" },
{ MEIndividualizationCompleted, L"MEIndividualizationCompleted" },
{ MEEnablerProgress, L"MEEnablerProgress" },
{ MEEnablerCompleted, L"MEEnablerCompleted" },
{ MEPolicyError, L"MEPolicyError" },
{ MEPolicyReport, L"MEPolicyReport" },
{ MEBufferingStarted, L"MEBufferingStarted" },
{ MEBufferingStopped, L"MEBufferingStopped" },
{ MEConnectStart, L"MEConnectStart" },
{ MEConnectEnd, L"MEConnectEnd" },
{ MEReconnectStart, L"MEReconnectStart" },
{ MEReconnectEnd, L"MEReconnectEnd" },
{ MERendererEvent, L"MERendererEvent" },
{ MESessionStreamSinkFormatChanged, L"MESessionStreamSinkFormatChanged" },
{ MESourceUnknown, L"MESourceUnknown" },
{ MESourceStarted, L"MESourceStarted" },
{ MEStreamStarted, L"MEStreamStarted" },
{ MESourceSeeked, L"MESourceSeeked" },
{ MEStreamSeeked, L"MEStreamSeeked" },
{ MENewStream, L"MENewStream" },
{ MEUpdatedStream, L"MEUpdatedStream" },
{ MESourceStopped, L"MESourceStopped" },
{ MEStreamStopped, L"MEStreamStopped" },
{ MESourcePaused, L"MESourcePaused" },
{ MEStreamPaused, L"MEStreamPaused" },
{ MEEndOfPresentation, L"MEEndOfPresentation" },
{ MEEndOfStream, L"MEEndOfStream" },
{ MEMediaSample, L"MEMediaSample" },
{ MEStreamTick, L"MEStreamTick" },
{ MEStreamThinMode, L"MEStreamThinMode" },
{ MEStreamFormatChanged, L"MEStreamFormatChanged" },
{ MESourceRateChanged, L"MESourceRateChanged" },
{ MEEndOfPresentationSegment, L"MEEndOfPresentationSegment" },
{ MESourceCharacteristicsChanged, L"MESourceCharacteristicsChanged" },
{ MESourceRateChangeRequested, L"MESourceRateChangeRequested" },
{ MESourceMetadataChanged, L"MESourceMetadataChanged" },
{ MESequencerSourceTopologyUpdated, L"MESequencerSourceTopologyUpdated" },
{ MESinkUnknown, L"MESinkUnknown" },
{ MEStreamSinkStarted, L"MEStreamSinkStarted" },
{ MEStreamSinkStopped, L"MEStreamSinkStopped" },
{ MEStreamSinkPaused, L"MEStreamSinkPaused" },
{ MEStreamSinkRateChanged, L"MEStreamSinkRateChanged" },
{ MEStreamSinkRequestSample, L"MEStreamSinkRequestSample" },
{ MEStreamSinkMarker, L"MEStreamSinkMarker" },
{ MEStreamSinkPrerolled, L"MEStreamSinkPrerolled" },
{ MEStreamSinkScrubSampleComplete, L"MEStreamSinkScrubSampleComplete" },
{ MEStreamSinkFormatChanged, L"MEStreamSinkFormatChanged" },
{ MEStreamSinkDeviceChanged, L"MEStreamSinkDeviceChanged" },
{ MEQualityNotify, L"MEQualityNotify" },
{ MESinkInvalidated, L"MESinkInvalidated" },
{ MEAudioSessionNameChanged, L"MEAudioSessionNameChanged" },
{ MEAudioSessionVolumeChanged, L"MEAudioSessionVolumeChanged" },
{ MEAudioSessionDeviceRemoved, L"MEAudioSessionDeviceRemoved" },
{ MEAudioSessionServerShutdown, L"MEAudioSessionServerShutdown" },
{ MEAudioSessionGroupingParamChanged, L"MEAudioSessionGroupingParamChanged" },
{ MEAudioSessionIconChanged, L"MEAudioSessionIconChanged" },
{ MEAudioSessionFormatChanged, L"MEAudioSessionFormatChanged" },
{ MEAudioSessionDisconnected, L"MEAudioSessionDisconnected" },
{ MEAudioSessionExclusiveModeOverride, L"MEAudioSessionExclusiveModeOverride" },
{ MECaptureAudioSessionVolumeChanged, L"MECaptureAudioSessionVolumeChanged" },
{ MECaptureAudioSessionDeviceRemoved, L"MECaptureAudioSessionDeviceRemoved" },
{ MECaptureAudioSessionFormatChanged, L"MECaptureAudioSessionFormatChanged" },
{ MECaptureAudioSessionDisconnected, L"MECaptureAudioSessionDisconnected" },
{ MECaptureAudioSessionExclusiveModeOverride, L"MECaptureAudioSessionExclusiveModeOverride" },
{ MECaptureAudioSessionServerShutdown, L"MECaptureAudioSessionServerShutdown" },
{ METrustUnknown, L"METrustUnknown" },
{ MEPolicyChanged, L"MEPolicyChanged" },
{ MEContentProtectionMessage, L"MEContentProtectionMessage" },
{ MEPolicySet, L"MEPolicySet" },
{ MEWMDRMLicenseBackupCompleted, L"MEWMDRMLicenseBackupCompleted" },
{ MEWMDRMLicenseBackupProgress, L"MEWMDRMLicenseBackupProgress" },
{ MEWMDRMLicenseRestoreCompleted, L"MEWMDRMLicenseRestoreCompleted" },
{ MEWMDRMLicenseRestoreProgress, L"MEWMDRMLicenseRestoreProgress" },
{ MEWMDRMLicenseAcquisitionCompleted, L"MEWMDRMLicenseAcquisitionCompleted" },
{ MEWMDRMIndividualizationCompleted, L"MEWMDRMIndividualizationCompleted" },
{ MEWMDRMIndividualizationProgress, L"MEWMDRMIndividualizationProgress" },
{ MEWMDRMProximityCompleted, L"MEWMDRMProximityCompleted" },
{ MEWMDRMLicenseStoreCleaned, L"MEWMDRMLicenseStoreCleaned" },
{ MEWMDRMRevocationDownloadCompleted, L"MEWMDRMRevocationDownloadCompleted" },
{ METransformUnknown, L"METransformUnknown" },
{ METransformNeedInput ,L"METransformNeedInput"},
{ METransformHaveOutput, L"METransformHaveOutput" },
{ METransformDrainComplete, L"METransformDrainComplete" },
{ METransformMarker, L"METransformMarker" },
{ MEByteStreamCharacteristicsChanged, L"MEByteStreamCharacteristicsChanged" },
{ MEVideoCaptureDeviceRemoved, L"MEVideoCaptureDeviceRemoved" },
{ MEVideoCaptureDevicePreempted, L"MEVideoCaptureDevicePreempted" },
{ MEStreamSinkFormatInvalidated, L"MEStreamSinkFormatInvalidated" },
{ MEEncodingParameters, L"MEEncodingParameters" },
{ MEContentProtectionMetadata, L"MEContentProtectionMetadata" },
{ MEReservedMax, L"MEReservedMax" }
};


void dump_met( const wchar_t*fmt, DWORD met ) {
  for ( auto i = 0; i < _countof( event_names ); i++ ) {
    if ( event_names[ i ].id == met )
      dump( fmt, event_names[ i ].name );
  }
}