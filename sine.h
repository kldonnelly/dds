//#include "idds.h"

#define TABLE_SIZE 1024
//const char* CLSID_SineWave="3b36f6bc-fad7-4177-8959-647f80a595b6";

#ifdef __cplusplus
#include <com/COMInterfaces.h>
#endif


#ifdef __cplusplus
class SineWave : public IWaveForm ,public IClassFactory
{

public:
	// IUnknown functions
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID interfaceid, void** objectInterface);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* outerAggregateIUnknown, REFIID interfaceid, void** objectInterface);
	virtual HRESULT  STDMETHODCALLTYPE LockServer(BOOL shouldLock);
	SineWave(float freq);
	void Generate();
private:
	volatile LONG m_referenceCount;

};

#endif
