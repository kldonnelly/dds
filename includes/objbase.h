/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
//#include <rpc.h>
//#include <rpcndr.h>

#ifndef _OBJBASE_H_
#define _OBJBASE_H_

//#include <pshpack8.h>



#if defined(__cplusplus) && !defined(CINTERFACE)

#ifndef __OBJC__
#undef interface
#define interface struct
#endif

#define __STRUCT__ struct
#define STDMETHOD(method) virtual HRESULT WINAPI method
#define STDMETHOD_(type,method) virtual type WINAPI method
#define STDMETHODV(method) virtual HRESULT STDMETHODVCALLTYPE method
#define STDMETHODV_(type,method) virtual type STDMETHODVCALLTYPE method
#define PURE = 0
#define THIS_
#define THIS void
#define DECLARE_INTERFACE(iface) __STRUCT__ iface
#define DECLARE_INTERFACE_(iface,baseiface) __STRUCT__ iface : public baseiface
#define DECLARE_INTERFACE_IID_(iface,baseiface,iidiface) __STRUCT__ iface : public baseiface

#if !defined(BEGIN_INTERFACE)
#define BEGIN_INTERFACE
#define END_INTERFACE
#endif
#else

#ifndef __OBJC__
#undef interface
#define interface struct
#endif

#define STDMETHOD(method) HRESULT (WINAPI *method)
#define STDMETHOD_(type,method) type (WINAPI *method)
#define STDMETHODV(method) HRESULT (STDMETHODVCALLTYPE *method)
#define STDMETHODV_(type,method) type (STDMETHODVCALLTYPE *method)

#if !defined(BEGIN_INTERFACE)
#define BEGIN_INTERFACE
#define END_INTERFACE
#endif

#define PURE
#define THIS_ INTERFACE *This,
#define THIS INTERFACE *This
#ifdef CONST_VTABLE
#undef CONST_VTBL
#define CONST_VTBL const
#define DECLARE_INTERFACE(iface) typedef struct iface { const struct iface##Vtbl *lpVtbl; } iface; typedef const struct iface##Vtbl iface##Vtbl; const struct iface##Vtbl
#else
#undef CONST_VTBL
#define CONST_VTBL
#define DECLARE_INTERFACE(iface) typedef struct iface { struct iface##Vtbl *lpVtbl; } iface; typedef struct iface##Vtbl iface##Vtbl; struct iface##Vtbl
#endif
#define DECLARE_INTERFACE_(iface,baseiface) DECLARE_INTERFACE(iface)
#define DECLARE_INTERFACE_IID_(iface,baseiface,iidiface) DECLARE_INTERFACE(iface)

#endif

#define IFACEMETHOD(method)         STDMETHOD(method)
#define IFACEMETHOD_(type,method)   STDMETHOD_(type,method)
#define IFACEMETHODV(method)        STDMETHODV(method)
#define IFACEMETHODV_(type,method)  STDMETHODV_(type,method)

#ifndef FARSTRUCT
#define FARSTRUCT
#endif

#ifndef HUGEP
#define HUGEP
#endif

#include <stdlib.h>

#define LISet32(li,v) ((li).HighPart = ((LONG) (v)) < 0 ? -1 : 0,(li).LowPart = (v))
#define ULISet32(li,v) ((li).HighPart = 0,(li).LowPart = (v))
#define CLSCTX_INPROC (CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER)

#define CLSCTX_ALL (CLSCTX_INPROC_SERVER| CLSCTX_INPROC_HANDLER| CLSCTX_LOCAL_SERVER| CLSCTX_REMOTE_SERVER)
#define CLSCTX_SERVER (CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER|CLSCTX_REMOTE_SERVER)

typedef enum tagREGCLS {
  REGCLS_SINGLEUSE = 0,REGCLS_MULTIPLEUSE = 1,REGCLS_MULTI_SEPARATE = 2,REGCLS_SUSPENDED = 4,REGCLS_SURROGATE = 8
} REGCLS;

#define MARSHALINTERFACE_MIN 500

#define CWCSTORAGENAME 32

#define STGM_DIRECT __MSABI_LONG(0x00000000)
#define STGM_TRANSACTED __MSABI_LONG(0x00010000)
#define STGM_SIMPLE __MSABI_LONG(0x08000000)

#define STGM_READ __MSABI_LONG(0x00000000)
#define STGM_WRITE __MSABI_LONG(0x00000001)
#define STGM_READWRITE __MSABI_LONG(0x00000002)

#define STGM_SHARE_DENY_NONE __MSABI_LONG(0x00000040)
#define STGM_SHARE_DENY_READ __MSABI_LONG(0x00000030)
#define STGM_SHARE_DENY_WRITE __MSABI_LONG(0x00000020)
#define STGM_SHARE_EXCLUSIVE __MSABI_LONG(0x00000010)

#define STGM_PRIORITY __MSABI_LONG(0x00040000)
#define STGM_DELETEONRELEASE __MSABI_LONG(0x04000000)
#define STGM_NOSCRATCH __MSABI_LONG(0x00100000)
#define STGM_CREATE __MSABI_LONG(0x00001000)
#define STGM_CONVERT __MSABI_LONG(0x00020000)
#define STGM_FAILIFTHERE __MSABI_LONG(0x00000000)
#define STGM_NOSNAPSHOT __MSABI_LONG(0x00200000)
#define STGM_DIRECT_SWMR __MSABI_LONG(0x00400000)

#define ASYNC_MODE_COMPATIBILITY __MSABI_LONG(0x00000001)
#define ASYNC_MODE_DEFAULT __MSABI_LONG(0x00000000)

#define STGTY_REPEAT __MSABI_LONG(0x00000100)
#define STG_TOEND __MSABI_LONG(0xFFFFFFFF)

#define STG_LAYOUT_SEQUENTIAL __MSABI_LONG(0x00000000)
#define STG_LAYOUT_INTERLEAVED __MSABI_LONG(0x00000001)

#define STGFMT_STORAGE 0
#define STGFMT_NATIVE 1
#define STGFMT_FILE 3
#define STGFMT_ANY 4
#define STGFMT_DOCFILE 5

#define STGFMT_DOCUMENT 0

#ifndef __IRpcStubBuffer_FWD_DEFINED__
#define __IRpcStubBuffer_FWD_DEFINED__
typedef struct IRpcStubBuffer IRpcStubBuffer;
#endif
#ifndef __IRpcChannelBuffer_FWD_DEFINED__
#define __IRpcChannelBuffer_FWD_DEFINED__
typedef struct IRpcChannelBuffer IRpcChannelBuffer;
#endif

//#include <wtypes.h>
#include <unknwn.h>
//#include <objidl.h>

#include <guiddef.h>

#ifndef INITGUID
//#include <cguid.h>
#endif

#if defined(__cplusplus) && !defined(CINTERFACE)

extern "C++" {
    template<typename T> void **IID_PPV_ARGS_Helper(T **iface)    {
        static_cast<IUnknown*>(*iface);
        return reinterpret_cast<void**>(iface);
    }
}

#define IID_PPV_ARGS(iface) __uuidof(**(iface)), IID_PPV_ARGS_Helper(iface)

#endif

typedef enum tagCOINIT {
  COINIT_APARTMENTTHREADED = 0x2,COINIT_MULTITHREADED = 0x0,COINIT_DISABLE_OLE1DDE = 0x4,COINIT_SPEED_OVER_MEMORY = 0x8
} COINIT;


typedef enum tagCOMSD {
  SD_LAUNCHPERMISSIONS = 0,SD_ACCESSPERMISSIONS = 1,SD_LAUNCHRESTRICTIONS = 2,SD_ACCESSRESTRICTIONS = 3
} COMSD;



typedef enum tagSTDMSHLFLAGS {
  SMEXF_SERVER = 0x01,SMEXF_HANDLER = 0x02
} STDMSHLFLAGS;


#define COM_RIGHTS_EXECUTE 1
#define COM_RIGHTS_EXECUTE_LOCAL 2
#define COM_RIGHTS_EXECUTE_REMOTE 4
#define COM_RIGHTS_ACTIVATE_LOCAL 8
#define COM_RIGHTS_ACTIVATE_REMOTE 16



typedef enum tagCOWAIT_FLAGS {
  COWAIT_WAITALL = 1,COWAIT_ALERTABLE = 2,COWAIT_INPUTAVAILABLE = 4
} COWAIT_FLAGS;


#define STGOPTIONS_VERSION 2





#if (_WIN32_WINNT >= 0x0600)
WINOLEAPI CoDisconnectContext(DWORD dwTimeout);
#endif /*(_WIN32_WINNT >= 0x0600)*/
#if (_WIN32_WINNT >= 0x0601)

WINOLEAPI CoGetApartmentType(
  APTTYPE *pAptType,
  APTTYPEQUALIFIER *pAptQualifier
);

#endif /*(_WIN32_WINNT >= 0x0601)*/


#endif
