
// Author   KMS - Martin Dubois, ing.
// Product  OpenNet
// File     Samples/Radar/Radar_Controller.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== Includes ===========================================================
#include <OpenNet/Adapter.h>
#include <OpenNet/System.h>

// ===== Samples/Radar ======================================================
#include "Radar_Kernel.h"
#include "Radar_Types.h"

// Class
/////////////////////////////////////////////////////////////////////////////

class Radar_Controller
{

public:

    enum
    {
        ADAPTER_QTY = 2,

        EVENT_QTY = 6,

        PART_DST_AB = 0,
        PART_DST_CD = 1,
        PART_SRC_CD = 2,

        PART_QTY = 3,

        RGB_RED   = 0,
        RGB_GREEN = 1,
        RGB_BLUE  = 2,

        RGB_QTY = 3,

        RGB_MIN =   0,
        RGB_MAX = 255,
    };

    Radar_Controller();

    ~Radar_Controller();

    bool GetAnd() const;

    unsigned char GetDstA() const;
    unsigned char GetDstB() const;
    unsigned char GetDstC() const;
    unsigned char GetDstD() const;

    const unsigned char * GetDstAB() const;
    const unsigned char * GetDstCD() const;

    unsigned char GetFade() const;
    unsigned char GetGain() const;
    bool          GetOr  () const;

    unsigned char GetSrcA() const;
    unsigned char GetSrcB() const;
    unsigned char GetSrcC() const;
    unsigned char GetSrcD() const;

    const unsigned char * GetSrcAB() const;
    const unsigned char * GetSrcCD() const;

    const char * GetStatus() const;

    void SetAnd();

    void SetDstA(unsigned char aA);
    void SetDstB(unsigned char aB);
    void SetDstC(unsigned char aC);
    void SetDstD(unsigned char aD);

    void SetFade(unsigned char aFade);
    void SetGain(unsigned char aGain);
    void SetOr  ();

    void SetSrcA(unsigned char aA);
    void SetSrcB(unsigned char aB);
    void SetSrcC(unsigned char aC);
    void SetSrcD(unsigned char aD);

    bool Start();
    void Stop ();

    void Tick();

private:

    void Adapter_SetInputFilter();

    void Adapters_Connect();

    void Buffers_Allocate();
    void Buffers_Release ();

    void Erase_Create ();
    void Erase_DstAB  ();
    void Erase_DstCD  ();
    void Erase_SrcCD  ();
    void Erase_Release();

    void Fade_Create ();
    void Fade_Release();

    void Images_Draw  ();
    void Images_Update();

    void Processor_GetInfo();

    void UpdateStatus(const char * aMessage);

    void Work_Start();
    void Work_Wait ();

    void Write_In();

    OpenNet::Adapter   * mAdapters[ADAPTER_QTY];
    cl_command_queue     mCommandQueue;
    cl_context           mContext     ;
    unsigned int         mCount       ;
    cl_device_id         mDeviceId    ;
    unsigned char        mDstAB[RADAR_SIZE][RADAR_SIZE][RGB_QTY];
    unsigned char        mDstCD[RADAR_SIZE][RADAR_SIZE][RGB_QTY];
    bool                 mErase_Flags   [PART_QTY];
    cl_kernel            mErase_Kernels [PART_QTY];
    cl_program           mErase_Programs[PART_QTY];
    unsigned int         mEventCount  ;
    cl_event             mEvents[EVENT_QTY];
    cl_kernel            mFade_Kernel ;
    cl_program           mFade_Program;
    Radar_Kernel         mFilter      ;
    cl_mem               mIn          ;
    Radar_In             mInValue     ;
    cl_mem               mOut         ;
    Radar_Out            mOutValue    ;
    OpenNet::Processor * mProcessor   ;
    unsigned char        mSrcAB[RADAR_SIZE][RADAR_SIZE][RGB_QTY];
    unsigned char        mSrcCD[RADAR_SIZE][RADAR_SIZE][RGB_QTY];
    char                 mStatus  [64];
    OpenNet::System    * mSystem      ;
    bool                 mWriteIn_Flag;

};
