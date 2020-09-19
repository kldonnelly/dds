#include "initguid.h"
#include "idds.h"
#include "sine.h"
#include <math.h>
#include <com/COMInterfaces.h>
/* Math definitions */
#define _2PI 6.283185307f
#define _PI 3.14159265f
#define SAMPLE_RATE DDS_SAMPLE_RATE
/* Q16.16 */
#define FRAC 16
#define Q16(X) (X * (float)(1 << FRAC))

/** DllGetClassObject */
 HRESULT DllGetClassObject(REFCLSID requestedClassID, REFIID requestedInterfaceID, LPVOID * objectInterface)
{

//	GUID IID_Bag = __uuidof(Bag);

	if (requestedClassID !=SmartGUID( CLSID_SineWave) && requestedClassID != GUID_NULL )
		return CLASS_E_CLASSNOTAVAILABLE;

	SineWave* factory = new SineWave(1000);
	if (NULL == factory)
		return E_OUTOFMEMORY;

//	*objectInterface=factory;
	HRESULT hr = factory->QueryInterface(requestedInterfaceID, objectInterface);
//	factory->Release();

	return S_OK;
}


extern inline LONG WINAPI InterlockedIncrement( LONG volatile *dest );
extern inline LONG WINAPI InterlockedIncrement( LONG volatile *dest )
{
    return ++*dest;
}

extern inline LONG WINAPI InterlockedDecrement( LONG volatile *dest );
extern inline LONG WINAPI InterlockedDecrement( LONG volatile *dest )
{
    return --*dest;
}

SineWave::SineWave(float freq):IWaveForm(freq){

	  Generate();
}

void SineWave::Generate()
{
    // populate table table[k]=sin(2*pi*k/N)
    int i = 0;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        // calculating sine wave
    	Table[i] = sinf(_2PI * ((float)i / TABLE_SIZE));
    }
    /* set the last byte equal to first for interpolation */
    Table[TABLE_SIZE] = Table[0];
    phaseAccumulator_L = 0;
    phaseAccumulator_R = 0;

     phaseIncrement_L = Q16(
        (float)frequency_L * TABLE_SIZE / SAMPLE_RATE);
     phaseIncrement_R = Q16(
        (float)frequency_R * TABLE_SIZE / SAMPLE_RATE);
}

HRESULT  SineWave::QueryInterface(REFIID interfaceid, void** objectInterface) {
	if (IID_IUnknown== interfaceid) {
		*objectInterface = static_cast<IWaveForm*>(this);
	} else if (SmartGUID(IID_IWaveForm) == interfaceid) {
		*objectInterface = static_cast<IWaveForm*>(this);
	}
	else if (IID_IClassFactory == interfaceid) {
			*objectInterface = static_cast<IClassFactory*>(this);
		}else
		{
		*objectInterface = NULL;
		return E_NOINTERFACE;
	}

	// Increment the reference count on the interface
//	reinterpret_cast<IUnknown*>(*objectInterface)->AddRef();
	AddRef();
	return S_OK;
}

/**
 * Bag::AddRef
 *
 * Increment the refence count on Bag.
 */
ULONG  SineWave::AddRef() {
	return InterlockedIncrement(&m_referenceCount);
}

/**
 * Bag::Release
 *
 * Decrement the reference count on Bag. Bag will be destroyed if
 * the reference count drops to zero.
 */
ULONG  SineWave::Release() {
	if (0 == InterlockedDecrement(&m_referenceCount))
	{
		delete this;
		return 0;
	}
	return m_referenceCount;
}

HRESULT STDMETHODCALLTYPE SineWave::CreateInstance(IUnknown* outerAggregateIUnknown,
		REFIID interfaceid, void** objectInterface) {
	// We don't support aggregation.
	if (outerAggregateIUnknown != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}


	// Get the desired interface on Bag
	HRESULT hr = QueryInterface(interfaceid, objectInterface);

	// TODO If we should release Bag upon successful QueryInterface, as in Bag::ComCreate, then do likewise here.

	return hr;
}

/**
 * BagCFactory::LockServer
 *
 * http://msdn2.microsoft.com/en-us/library/ms682332.aspx
 */
HRESULT STDMETHODCALLTYPE SineWave::LockServer(BOOL shouldLock) {
	if (shouldLock) {
		InterlockedIncrement(&m_referenceCount);
	} else {
		InterlockedDecrement(&m_referenceCount);
	}
	return S_OK;
}


