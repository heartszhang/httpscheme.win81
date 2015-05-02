

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri May 01 18:07:16 2015
 */
/* Compiler settings for C:\Users\Hearts\AppData\Local\Temp\httpclient.demo.idl-ad4a2637:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __httpclient2Edemo_h_h__
#define __httpclient2Edemo_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ____x_ABI_Cbestv_Cweb_CIHttpResponse_FWD_DEFINED__
#define ____x_ABI_Cbestv_Cweb_CIHttpResponse_FWD_DEFINED__
typedef interface __x_ABI_Cbestv_Cweb_CIHttpResponse __x_ABI_Cbestv_Cweb_CIHttpResponse;

#ifdef __cplusplus
namespace ABI {
    namespace bestv {
        namespace web {
            interface IHttpResponse;
        } /* end namespace */
    } /* end namespace */
} /* end namespace */

#endif /* __cplusplus */

#endif 	/* ____x_ABI_Cbestv_Cweb_CIHttpResponse_FWD_DEFINED__ */


#ifndef ____FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_FWD_DEFINED__
#define ____FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_FWD_DEFINED__
typedef interface __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse;

#endif 	/* ____FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_FWD_DEFINED__ */


#ifndef ____FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_FWD_DEFINED__
#define ____FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_FWD_DEFINED__
typedef interface __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse;

#endif 	/* ____FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_FWD_DEFINED__ */


/* header files for imported files */
#include "inspectable.h"
#include "Windows.Foundation.h"
#include "Windows.Data.Json.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_httpclient2Edemo_0000_0000 */
/* [local] */ 

#ifdef __cplusplus
} /*extern "C"*/ 
#endif
#include <windows.foundation.collections.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
namespace ABI {
namespace bestv {
namespace web {
interface IHttpResponse;
} /*web*/
} /*bestv*/
}
#endif


/* interface __MIDL_itf_httpclient2Edemo_0000_0000 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0000_v0_0_s_ifspec;

/* interface __MIDL_itf_httpclient2Edemo_0000_0262 */




/* interface __MIDL_itf_httpclient2Edemo_0000_0262 */




extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0262_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0262_v0_0_s_ifspec;

/* interface __MIDL_itf_httpclient2Edemo_0000_0001 */
/* [local] */ 

#ifndef DEF___FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_USE
#define DEF___FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("9f48e760-0635-595f-8cc3-fcf4cce700cc"))
IAsyncOperationCompletedHandler<ABI::bestv::web::IHttpResponse*> : IAsyncOperationCompletedHandler_impl<ABI::bestv::web::IHttpResponse*> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.AsyncOperationCompletedHandler`1<bestv.web.IHttpResponse>"; }
};
typedef IAsyncOperationCompletedHandler<ABI::bestv::web::IHttpResponse*> __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_t;
#define ____FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_FWD_DEFINED__
#define __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse ABI::Windows::Foundation::__FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_t

/* ABI */ } /* Windows */ } /* Foundation */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_USE */


/* interface __MIDL_itf_httpclient2Edemo_0000_0001 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0001_v0_0_s_ifspec;

/* interface __MIDL_itf_httpclient2Edemo_0000_0263 */




/* interface __MIDL_itf_httpclient2Edemo_0000_0263 */




extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0263_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0263_v0_0_s_ifspec;

/* interface __MIDL_itf_httpclient2Edemo_0000_0002 */
/* [local] */ 

#ifndef DEF___FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_USE
#define DEF___FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_USE
#if defined(__cplusplus) && !defined(RO_NO_TEMPLATE_NAME)
} /*extern "C"*/ 
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("273b425d-1e46-5cb6-b491-362068bdf361"))
IAsyncOperation<ABI::bestv::web::IHttpResponse*> : IAsyncOperation_impl<ABI::bestv::web::IHttpResponse*> {
static const wchar_t* z_get_rc_name_impl() {
return L"Windows.Foundation.IAsyncOperation`1<bestv.web.IHttpResponse>"; }
};
typedef IAsyncOperation<ABI::bestv::web::IHttpResponse*> __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_t;
#define ____FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_FWD_DEFINED__
#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse ABI::Windows::Foundation::__FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_t

/* ABI */ } /* Windows */ } /* Foundation */ }
extern "C" {
#endif //__cplusplus
#endif /* DEF___FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_USE */
#pragma warning(push)
#pragma warning(disable:4001) 
#pragma once
#pragma warning(pop)

#if !defined(____x_ABI_Cbestv_Cweb_CIHttpResponse_INTERFACE_DEFINED__)
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_bestv_web_IHttpResponse[] = L"bestv.web.IHttpResponse";
#endif /* !defined(____x_ABI_Cbestv_Cweb_CIHttpResponse_INTERFACE_DEFINED__) */


/* interface __MIDL_itf_httpclient2Edemo_0000_0002 */
/* [local] */ 





extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0002_v0_0_s_ifspec;

#ifndef ____x_ABI_Cbestv_Cweb_CIHttpResponse_INTERFACE_DEFINED__
#define ____x_ABI_Cbestv_Cweb_CIHttpResponse_INTERFACE_DEFINED__

/* interface __x_ABI_Cbestv_Cweb_CIHttpResponse */
/* [uuid][object] */ 



/* interface ABI::bestv::web::IHttpResponse */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_Cbestv_Cweb_CIHttpResponse;

#if defined(__cplusplus) && !defined(CINTERFACE)
    } /* end extern "C" */
    namespace ABI {
        namespace bestv {
            namespace web {
                
                MIDL_INTERFACE("497783FC-D66D-4DF6-AAFC-C4D879AB22F1")
                IHttpResponse : public IInspectable
                {
                public:
                    virtual HRESULT STDMETHODCALLTYPE GetHeader( 
                        /* [in] */ HSTRING name,
                        /* [out][retval] */ HSTRING *value) = 0;
                    
                    virtual HRESULT STDMETHODCALLTYPE BodyJson( 
                        /* [out] */ ABI::Windows::Data::Json::IJsonValue **body) = 0;
                    
                    virtual HRESULT STDMETHODCALLTYPE Status( 
                        /* [out] */ int *status,
                        /* [out] */ HSTRING *phrase) = 0;
                    
                };

                extern const __declspec(selectany) IID & IID_IHttpResponse = __uuidof(IHttpResponse);

                
            }  /* end namespace */
        }  /* end namespace */
    }  /* end namespace */
    extern "C" { 
    
#else 	/* C style interface */

    typedef struct __x_ABI_Cbestv_Cweb_CIHttpResponseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_Cbestv_Cweb_CIHttpResponse * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_Cbestv_Cweb_CIHttpResponse * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_Cbestv_Cweb_CIHttpResponse * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_Cbestv_Cweb_CIHttpResponse * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_Cbestv_Cweb_CIHttpResponse * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_Cbestv_Cweb_CIHttpResponse * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *GetHeader )( 
            __x_ABI_Cbestv_Cweb_CIHttpResponse * This,
            /* [in] */ HSTRING name,
            /* [out][retval] */ HSTRING *value);
        
        HRESULT ( STDMETHODCALLTYPE *BodyJson )( 
            __x_ABI_Cbestv_Cweb_CIHttpResponse * This,
            /* [out] */ __x_ABI_CWindows_CData_CJson_CIJsonValue **body);
        
        HRESULT ( STDMETHODCALLTYPE *Status )( 
            __x_ABI_Cbestv_Cweb_CIHttpResponse * This,
            /* [out] */ int *status,
            /* [out] */ HSTRING *phrase);
        
        END_INTERFACE
    } __x_ABI_Cbestv_Cweb_CIHttpResponseVtbl;

    interface __x_ABI_Cbestv_Cweb_CIHttpResponse
    {
        CONST_VTBL struct __x_ABI_Cbestv_Cweb_CIHttpResponseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_Cbestv_Cweb_CIHttpResponse_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_Cbestv_Cweb_CIHttpResponse_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_Cbestv_Cweb_CIHttpResponse_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_Cbestv_Cweb_CIHttpResponse_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_Cbestv_Cweb_CIHttpResponse_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_Cbestv_Cweb_CIHttpResponse_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_Cbestv_Cweb_CIHttpResponse_GetHeader(This,name,value)	\
    ( (This)->lpVtbl -> GetHeader(This,name,value) ) 

#define __x_ABI_Cbestv_Cweb_CIHttpResponse_BodyJson(This,body)	\
    ( (This)->lpVtbl -> BodyJson(This,body) ) 

#define __x_ABI_Cbestv_Cweb_CIHttpResponse_Status(This,status,phrase)	\
    ( (This)->lpVtbl -> Status(This,status,phrase) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_Cbestv_Cweb_CIHttpResponse_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_httpclient2Edemo_0000_0264 */




/* interface __MIDL_itf_httpclient2Edemo_0000_0264 */




extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0264_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0264_v0_0_s_ifspec;

/* interface __MIDL_itf_httpclient2Edemo_0000_0004 */
/* [local] */ 

#ifndef DEF___FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse
#define DEF___FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)



/* interface __MIDL_itf_httpclient2Edemo_0000_0004 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0004_v0_0_s_ifspec;

#ifndef ____FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_INTERFACE_DEFINED__
#define ____FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_INTERFACE_DEFINED__

/* interface __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse */
/* [unique][uuid][object] */ 



/* interface __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9f48e760-0635-595f-8cc3-fcf4cce700cc")
    __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse *asyncInfo,
            /* [in] */ AsyncStatus status) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse * This,
            /* [in] */ __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse *asyncInfo,
            /* [in] */ AsyncStatus status);
        
        END_INTERFACE
    } __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponseVtbl;

    interface __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse
    {
        CONST_VTBL struct __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_Invoke(This,asyncInfo,status)	\
    ( (This)->lpVtbl -> Invoke(This,asyncInfo,status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_httpclient2Edemo_0000_0005 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse */


/* interface __MIDL_itf_httpclient2Edemo_0000_0005 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0005_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0005_v0_0_s_ifspec;

/* interface __MIDL_itf_httpclient2Edemo_0000_0265 */




/* interface __MIDL_itf_httpclient2Edemo_0000_0265 */




extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0265_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0265_v0_0_s_ifspec;

/* interface __MIDL_itf_httpclient2Edemo_0000_0006 */
/* [local] */ 

#ifndef DEF___FIAsyncOperation_1_bestv__Cweb__CIHttpResponse
#define DEF___FIAsyncOperation_1_bestv__Cweb__CIHttpResponse
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


/* interface __MIDL_itf_httpclient2Edemo_0000_0006 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0006_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0006_v0_0_s_ifspec;

#ifndef ____FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_INTERFACE_DEFINED__
#define ____FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_INTERFACE_DEFINED__

/* interface __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse */
/* [unique][uuid][object] */ 



/* interface __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIAsyncOperation_1_bestv__Cweb__CIHttpResponse;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("273b425d-1e46-5cb6-b491-362068bdf361")
    __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse : public IInspectable
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Completed( 
            /* [in] */ __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse *handler) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Completed( 
            /* [retval][out] */ __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse **handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetResults( 
            /* [retval][out] */ ABI::bestv::web::IHttpResponse **results) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIAsyncOperation_1_bestv__Cweb__CIHttpResponseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Completed )( 
            __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse * This,
            /* [in] */ __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse *handler);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Completed )( 
            __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse * This,
            /* [retval][out] */ __FIAsyncOperationCompletedHandler_1_bestv__Cweb__CIHttpResponse **handler);
        
        HRESULT ( STDMETHODCALLTYPE *GetResults )( 
            __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse * This,
            /* [retval][out] */ __x_ABI_Cbestv_Cweb_CIHttpResponse **results);
        
        END_INTERFACE
    } __FIAsyncOperation_1_bestv__Cweb__CIHttpResponseVtbl;

    interface __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse
    {
        CONST_VTBL struct __FIAsyncOperation_1_bestv__Cweb__CIHttpResponseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_put_Completed(This,handler)	\
    ( (This)->lpVtbl -> put_Completed(This,handler) ) 

#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_get_Completed(This,handler)	\
    ( (This)->lpVtbl -> get_Completed(This,handler) ) 

#define __FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_GetResults(This,results)	\
    ( (This)->lpVtbl -> GetResults(This,results) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIAsyncOperation_1_bestv__Cweb__CIHttpResponse_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_httpclient2Edemo_0000_0007 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIAsyncOperation_1_bestv__Cweb__CIHttpResponse */


/* interface __MIDL_itf_httpclient2Edemo_0000_0007 */
/* [local] */ 



extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0007_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_httpclient2Edemo_0000_0007_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  HSTRING_UserSize(     unsigned long *, unsigned long            , HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserMarshal(  unsigned long *, unsigned char *, HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserUnmarshal(unsigned long *, unsigned char *, HSTRING * ); 
void                      __RPC_USER  HSTRING_UserFree(     unsigned long *, HSTRING * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


