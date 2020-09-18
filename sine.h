//#include "idds.h"

#define TABLE_SIZE 1024
//const char* CLSID_SineWave="3b36f6bc-fad7-4177-8959-647f80a595b6";

#include <com/COMInterfaces.h>

UUID_DEFINE(CLSID_SineWave,0x3b,0x36,0xf6,0xbc,0xfa,0xd7,0x41,0x77,0x89,0x59,0x64,0x7f,0x80,0xa5,0x95,0xb6);

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
