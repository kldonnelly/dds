
#define __com
//#define __cplusplus 1
#include <stdint.h>
#include "dds_defs.h"
#ifdef __com
#include <uuid/uuid.h>
#include <com/WinError.h>
#include <com/COM.h>

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif


MIDL_DEFINE_GUID(PlainGUID, IID_IWaveForm,0x304e42cb,0xe6d8,0x40c0,0xa0,0x9f,0xa3,0x62,0xf1,0x2b,0x42,0x8d);

//MIDL_DEFINE_GUID(IID, IID_IWaveForm,0x304e42cb,0xe6d8,0x40c0,0xa0,0x9f,0xa3,0x62,0xf1,0x2b,0x42,0x8d);

//UUID_DEFINE(IID_IUnknown,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x046);
//MMIDL_DEFINE_GUID(IID, IID_IWaveForm,0x304e42cb,0xe6d8,0x40c0,0xa0,0x9f,0xa3,0x62,0xf1,0x2b,0x42,0x8d);

#define TABLE_SIZE 1024
struct IWaveForm;

#ifdef __cplusplus
class IWaveForm
{
protected:
	float frequency_L;
	float frequency_R;
	uint32_t phaseAccumulator_L;
	uint32_t phaseAccumulator_R;
	uint32_t phaseIncrement_L;
	uint32_t phaseIncrement_R;
	float Table[TABLE_SIZE + 1]; // +1 for interpolation
	float DDS_calculate_channel_out(uint32_t *phaseAccumulator,
			uint32_t phaseIncrement, uint32_t index);
public:

//	IWaveForm();
	IWaveForm(float freq);
	void SetFreq(float freq);
	virtual ~IWaveForm() {}
	virtual void Generate() = 0; // "= 0" part makes this method pure virtual, and
	// also makes this class abstract.
	void Calculate(dmabuf_t *buffer, uint16_t buffer_size);
};
#else
#ifdef __com
#include <uuid/uuid.h>
//#include "WinError.h"
//#include "types.h"
#endif

#ifdef __com

UUID_DEFINE(IID_IUnknown,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x046);
UUID_DEFINE(IID_IClassFactory,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);

#include "objbase.h"

#define STDMETHODCALLTYPE
typedef struct _IUnknown IUnknown;
typedef uuid_t IID;
typedef uuid_t GUID;
typedef const IID *REFCLSID;
typedef const IID *REFIID;

struct IUnknown;

/*
 * IUnknown interface
 */
typedef struct IUnknown_vt
{
    /* IUnknown methods */
    long ( *QueryInterface)(struct IUnknown *This, const GUID *riid,
                                   void **ppvObject);
    long ( *AddRef)(struct IUnknown *This);
    long ( *Release)(struct IUnknown *This);

} IUnknown_vt;

struct _IUnknown { IUnknown_vt* vt; };

typedef struct IUnknown* LPUNKNOWN;

#undef  INTERFACE
#define INTERFACE   IClassFactory

DECLARE_INTERFACE_(IClassFactory, IUnknown)
{
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid,LPVOID* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef) (THIS) PURE;
	STDMETHOD_(ULONG,Release) (THIS) PURE;

	// *** IClassFactory methods ***
	STDMETHOD(CreateInstance) (THIS_ LPUNKNOWN pUnkOuter,REFIID riid,LPVOID* ppvObject) PURE;

};

typedef HRESULT QueryInterfacePtr(struct IWaveForm *, REFIID, void **);
typedef ULONG AddRefPtr(struct IWaveForm *);
typedef ULONG ReleasePtr(struct IWaveForm *);

#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))

HRESULT DllGetClassObject(REFCLSID objGuid, REFIID factoryGuid, void **factoryHandle);
#endif


typedef long CalculatePtr(struct IWaveForm *, dmabuf_t *, uint16_t);

typedef void GeneratePtr(struct IWaveForm *);

typedef struct IWaveFormVtbl
{
#ifdef __com
	QueryInterfacePtr *QueryInterface;
	AddRefPtr *AddRef;
	ReleasePtr *Release;
#endif
	CalculatePtr *Calculate;
	GeneratePtr *Generate;
} IWaveFormVtbl;

typedef struct IWaveForm
{
	uint32_t phaseAccumulator_L;
	uint32_t phaseAccumulator_R;
	uint32_t phaseIncrement_L;
	uint32_t phaseIncrement_R;
	float frequency_L;
	float frequency_R;
	float table[TABLE_SIZE + 1]; // +1 for interpolation
	IWaveFormVtbl *lpVtbl;
	int count;

} IWaveForm;

HRESULT  QueryInterface(IWaveForm *this,
		REFIID vTableGuid, void **ppv);

ULONG  AddRef(IWaveForm *this);
ULONG  Release(IWaveForm *this);
void Calculate(IWaveForm *this,dmabuf_t *buffer, uint16_t buffer_size);

//extern void Generate_Sine(IWaveForm *this);

//static const IWaveFormVtbl IWaveForm_Vtbl = {QueryInterface,AddRef,Release,Calculate, Generate_Sine};


float DDS_calculate_channel_out(IWaveForm *ptr, uint32_t *phaseAccumulator,
		uint32_t phaseIncrement, uint32_t index);
#endif
