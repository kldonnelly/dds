#include "idds.h"
#include "sine.h"
#include <math.h>
/* Math definitions */
#define _2PI 6.283185307f
#define _PI 3.14159265f
#define SAMPLE_RATE DDS_SAMPLE_RATE
/* Q16.16 */
#define FRAC 16

#define Q16(X) (X * (float)(1 << FRAC))

#ifdef __com


struct IClassFactory;

struct IUnknown;

struct IWaveFormVtbl;

void Generate_Sine(IWaveForm *this);

static const IWaveFormVtbl IWaveForm_Vtbl = {QueryInterface,AddRef,Release,Calculate, Generate_Sine};

ULONG  classAddRef(struct IClassFactory *this)
{
   return(1);
}

ULONG  classRelease(struct IClassFactory *this)
{
   return(1);
}

HRESULT  classCreateInstance(struct IClassFactory *this,
        struct IUnknown *punkOuter, REFIID vTableGuid, void **ppv)
{
   HRESULT          hr;
   struct IWaveForm *thisobj;

   // Assume an error by clearing caller's handle.
   *ppv = 0;

   // We don't support aggregation in IExample.
   if (punkOuter)
      hr = CLASS_E_NOAGGREGATION;
   else
   {
      // Create our IExample object, and initialize it.
      if (!(thisobj = malloc(sizeof(struct IWaveForm))))
         hr = E_OUTOFMEMORY;
      else
      {
         // Store IExample's VTable. We declared it
         // as a static variable IExample_Vtbl.
         thisobj->lpVtbl = &IWaveForm_Vtbl;
         thisobj->frequency_L=1000;
         thisobj->frequency_R=1000;

         // Increment reference count so we
         // can call Release() below and it will
         // deallocate only if there
         // is an error with QueryInterface().
         thisobj->count = 1;

         // Fill in the caller's handle
         // with a pointer to the IExample we just
         // allocated above. We'll let IExample's
         // QueryInterface do that, because
         // it also checks the GUID the caller
         // passed, and also increments the
         // reference count (to 2) if all goes well.

         hr = IWaveForm_Vtbl.QueryInterface(thisobj, vTableGuid, ppv);

         // Decrement reference count.
         // NOTE: If there was an error in QueryInterface()
         // then Release() will be decrementing
         // the count back to 0 and will free the
         // IExample for us. One error that may
         // occur is that the caller is asking for
         // some sort of object that we don't
         // support (ie, it's a GUID we don't recognize).
         IWaveForm_Vtbl.Release(thisobj);
      }
   }

   return(hr);
}

// IClassFactory's QueryInterface()
static HRESULT  classQueryInterface(struct IClassFactory *this, REFIID factoryGuid, void **ppv)
{
	// Make sure the caller wants either an IUnknown or an IClassFactory.
	// In either case, we return the same IClassFactory pointer passed to
	// us since it can also masquerade as an IUnknown
	if (IsEqualGUID(factoryGuid, &IID_IUnknown) || IsEqualGUID(factoryGuid, &IID_IClassFactory))
	{
		// Call my IClassFactory's AddRef
		this->lpVtbl->AddRef(this);

		// Return (to the caller) a ptr to my IClassFactory
		*ppv = this;

		return(NOERROR);
	}

	// We don't know about any other GUIDs
	*ppv = 0;
	return(E_NOINTERFACE);
}

static DWORD LockCount = 0;

HRESULT  classLockServer(IClassFactory *this, BOOL flock)
{
 //  if (flock) InterlockedIncrement(&LockCount);
 //  else InterlockedDecrement(&LockCount);

   return(NOERROR);
}

static const IClassFactoryVtbl IClassFactory_Vtbl = {classQueryInterface,
classAddRef,
classRelease,
classCreateInstance,
classLockServer};

static IClassFactory MyIClassFactoryObj = {&IClassFactory_Vtbl};


// efbffae-0153-4cbe-b175-03f59ddfe880



HRESULT  DllGetClassObject(REFCLSID objGuid,
		REFIID factoryGuid, void **factoryHandle)
{
	HRESULT  hr;

	// Check that the caller is passing
	// our IExample GUID. That's the COM
	// object our DLL implements.
	if (IsEqualGUID(objGuid, &CLSID_SineWave))
	{
		// Fill in the caller's handle
		// with a pointer to our IClassFactory object.
		// We'll let our IClassFactory's
		// QueryInterface do that, because it also
		// checks the IClassFactory GUID and does other book-keeping.
		hr = classQueryInterface(&MyIClassFactoryObj,
				factoryGuid, factoryHandle);
	}
	else
	{
		// We don't understand this GUID.
		// It's obviously not for our DLL.
		// Let the caller know this by
		// clearing his handle and returning
		// CLASS_E_CLASSNOTAVAILABLE.
		*factoryHandle = 0;
		hr = CLASS_E_CLASSNOTAVAILABLE;
	}

	return(hr);
}
#endif


void Generate_Sine(IWaveForm *this)
{
    // populate table table[k]=sin(2*pi*k/N)
    int i = 0;
    for (i = 0; i < TABLE_SIZE; i++)
    {
        // calculating sine wave
    	this->table[i] = sinf(_2PI * ((float)i / TABLE_SIZE));
    }
    /* set the last byte equal to first for interpolation */
    this->table[TABLE_SIZE] =  this->table[0];
    this->phaseAccumulator_L = 0;
    this->phaseAccumulator_R = 0;


    this->phaseIncrement_L = Q16(
        (float)this->frequency_L * TABLE_SIZE / SAMPLE_RATE);
    this->phaseIncrement_R = Q16(
        (float)this->frequency_R * TABLE_SIZE / SAMPLE_RATE);
}

