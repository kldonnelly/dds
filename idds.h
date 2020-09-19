
#define __com
//#define __cplusplus 1
#include <stdint.h>
#include "dds_defs.h"
#ifdef __com

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
#endif


#define TABLE_SIZE 1024
struct IWaveForm;

#ifdef __cplusplus
#include <com/COM.h>
#include <com/WinError.h>
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
//#include <uuid/uuid.h>
#include "WinError.h"
#include "types.h"
#endif

#define STDMETHODCALLTYPE

#ifdef __com

typedef const IID *REFCLSID;
typedef const IID *REFIID;
#include "objbase.h"


DEFINE_GUID(CLSID_SineWave,0xE2E9CAE6,0x1E7B,0x4B8E,0xBA,0xBD,0xE9,0xBF,0x62,0x92,0xAC,0x29);
DEFINE_GUID(IID_IWaveForm,0x304e42cb,0xe6d8,0x40c0,0xa0,0x9f,0xa3,0x62,0xf1,0x2b,0x42,0x8d);


#define STDMETHODCALLTYPE

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


float DDS_calculate_channel_out(IWaveForm *ptr, uint32_t *phaseAccumulator,
		uint32_t phaseIncrement, uint32_t index);
#endif
