

#include "guiddef.h"
#include "idds.h"



#ifdef __com

HRESULT  QueryInterface(IWaveForm *this,
                          REFIID vTableGuid, void **ppv)
{
   // Check if the GUID matches IExample
   // VTable's GUID. Remember that we gave the
   // C variable name IID_IExample to our
   // VTable GUID. We can use an OLE function called
   // IsEqualIID to do the comparison for us.
   if (!IsEqualGUID(vTableGuid, &IID_IWaveForm))
   {
      // We don't recognize the GUID passed
      // to us. Let the caller know this,
      // by clearing his handle,
      // and returning E_NOINTERFACE.
      *ppv = 0;
      return(E_NOINTERFACE);
   }

   // It's a match!

   // First, we fill in his handle with
   // the same object pointer he passed us. That's
   // our IExample we created/initialized,
   // and he obtained from us.
   *ppv = this;

   // Now we call our own AddRef function,
   // passing the IExample.
   this->lpVtbl->AddRef(this);

   // Let him know he indeed has a IExample.
   return(NOERROR);
}

ULONG  AddRef(IWaveForm *this)
{
   // Increment the reference count (count member).
   ++this->count;

   // We're supposed to return the updated count.
   return(this->count);
}
// IExample's Release()

ULONG  Release(IWaveForm *this)
{
	// Decrement IExample's reference count. If 0, then we can safely free
	// this IExample now

}

#endif

float DDS_calculate_channel_out(IWaveForm *this,uint32_t *phaseAccumulator,
                                uint32_t phaseIncrement, uint32_t index)
{
     /* Increment the phase accumulator */
    *phaseAccumulator += phaseIncrement;
    *phaseAccumulator &= TABLE_SIZE * (1 << 16) - 1;
    index = *phaseAccumulator >> 16;

    /* interpolation */
    float v1 = this->table[index];
    float v2 = this->table[index + 1];
    float fmul = (*phaseAccumulator & 65535) / 65536.0f;
    //		float fmul = fix32_frac(phaseAccumulator, 16);
    float out = v1 + (v2 - v1) * fmul;

    out = out * 32768.0f;
    if (out > 32767)
        out = 32767;
    if (out < -32768)
        out = -32768;

    return (out);
}


void Calculate(IWaveForm *this,dmabuf_t *buffer, uint16_t buffer_size)
{

    uint32_t index_L = 0;
    uint32_t index_R = 0;

    int i = 0;

    for (i = 0; i < buffer_size; i++)
    {
        /* Left channel */
        buffer[i].LEFT.W16 = (int16_t)DDS_calculate_channel_out(this,&this->phaseAccumulator_L, this->phaseIncrement_L, index_L);
        /* Right channel */
        buffer[i].RIGHT.W16 = (int16_t)DDS_calculate_channel_out(this,&this->phaseAccumulator_R, this->phaseIncrement_R, index_R);
    }
}
