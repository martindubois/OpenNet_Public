
// Author   KMS - Martin Dubois, ing.
// Product  OpenNet
// File     Samples/Radar/Radar_Controller.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <string.h>

// ===== Windows ============================================================
#include <Windows.h>

// ===== Samples/Radar ======================================================
#include "Radar_Controller.h"

// OpenCL
/////////////////////////////////////////////////////////////////////////////

#define EOL "\n"

static const char * ERASE_NAMES[Radar_Controller::PART_QTY] =
{
    "Erase_DstAB",
    "Erase_DstCD",
    "Erase_SrcCD",
};

static const char * ERASE_DST_AB[] =
{
    RADAR_OUT_V
    "__kernel void Erase_DstAB( __global Radar_Out * aOut )"        EOL,
    "{"                                                             EOL,
    "    aOut->mDstAB[ get_global_id(0) ][ get_global_id(1) ] = 0;" EOL,
    "}"                                                             EOL,
};

static const char * ERASE_DST_CD[] =
{
    RADAR_OUT_V
    "__kernel void Erase_DstCD( __global Radar_Out * aOut )"        EOL,
    "{"                                                             EOL,
    "    aOut->mDstCD[ get_global_id(0) ][ get_global_id(1) ] = 0;" EOL,
    "}"                                                             EOL,
};

static const char * ERASE_SRC_CD[] =
{
    RADAR_OUT_V
    "__kernel void Erase_SrcCD( __global Radar_Out * aOut )"        EOL,
    "{"                                                             EOL,
    "    aOut->mSrcCD[ get_global_id(0) ][ get_global_id(1) ] = 0;" EOL,
    "}"                                                             EOL,
};

static const char ** ERASE_CODES[Radar_Controller::PART_QTY] =
{
    ERASE_DST_AB,
    ERASE_DST_CD,
    ERASE_SRC_CD,
};

static const unsigned int ERASE_CODE_LENGTHS[Radar_Controller::PART_QTY] =
{
    sizeof(ERASE_DST_AB) / sizeof(ERASE_DST_AB[0]),
    sizeof(ERASE_DST_CD) / sizeof(ERASE_DST_CD[0]),
    sizeof(ERASE_SRC_CD) / sizeof(ERASE_SRC_CD[0]),
};

static const char * FADE[] =
{
    RADAR_IN_V
    RADAR_OUT_V
    "__kernel void Fade( __constant Radar_In * aIn, __global Radar_Out * aOut )"  EOL,
    "{"                                                                           EOL,
    "    unsigned int x = get_global_id(0);"                                      EOL,
    "    unsigned int y = get_global_id(1);"                                      EOL,
                                                                                  EOL,
    "    aOut->mDstAB[ x ][ y ] = sub_sat( aOut->mDstAB[ x ][ y ], aIn->mFade );" EOL,
    "    aOut->mDstCD[ x ][ y ] = sub_sat( aOut->mDstCD[ x ][ y ], aIn->mFade );" EOL,
    "    aOut->mSrcAB[ x ][ y ] = sub_sat( aOut->mSrcAB[ x ][ y ], aIn->mFade );" EOL,
    "    aOut->mSrcCD[ x ][ y ] = sub_sat( aOut->mSrcCD[ x ][ y ], aIn->mFade );" EOL,
    "}"                                                                           EOL,
};

// Static function declarations
/////////////////////////////////////////////////////////////////////////////

static void Adapter_Config(OpenNet::Adapter * aAdapter);

static void Image_Draw  (unsigned char aImage[][RADAR_SIZE][Radar_Controller::RGB_QTY], unsigned char aX, unsigned char aY);
static void Image_Update(unsigned char aImage[][RADAR_SIZE][Radar_Controller::RGB_QTY], const unsigned char aData0[][RADAR_SIZE]);

// Public
/////////////////////////////////////////////////////////////////////////////

Radar_Controller::Radar_Controller()
    : mCommandQueue       (NULL )
    , mContext            (NULL )
    , mCount              (    0)
    , mDeviceId           (NULL )
    , mEventCount         (    0)
    , mFade_Kernel        (NULL )
    , mFade_Program       (NULL )
    , mIn                 (NULL )
    , mProcessor          (NULL )
    , mSystem             (NULL )
    , mWriteIn_Flag       (false)
{
    strcpy_s(mStatus, __FUNCTION__);

    memset(&mAdapters      , 0, sizeof(mAdapters      ));
    memset(&mDstAB         , 0, sizeof(mDstAB         ));
    memset(&mDstCD         , 0, sizeof(mDstCD         ));
    memset(&mErase_Flags   , 0, sizeof(mErase_Flags   ));
    memset(&mErase_Kernels , 0, sizeof(mErase_Kernels ));
    memset(&mErase_Programs, 0, sizeof(mErase_Programs));
    memset(&mInValue       , 0, sizeof(mInValue       ));
    memset(&mOut           , 0, sizeof(mOut           ));
    memset(&mOutValue      , 0, sizeof(mOutValue      ));
    memset(&mSrcAB         , 0, sizeof(mSrcAB         ));
    memset(&mSrcCD         , 0, sizeof(mSrcCD         ));

    mInValue.mAnd    = false;
    mInValue.mDst[0] =    31;
    mInValue.mDst[1] =    69;
    mInValue.mDst[2] =     0;
    mInValue.mDst[3] =     1;
    mInValue.mFade   =     7;
    mInValue.mGain   =    63;
    mInValue.mSrc[0] =   192;
    mInValue.mSrc[1] =   168;
    mInValue.mSrc[2] =     0;
    mInValue.mSrc[3] =     1;

    Images_Draw();
}

Radar_Controller::~Radar_Controller()
{
    strcpy_s(mStatus, __FUNCTION__);

    if (NULL != mSystem)
    {
        Stop();

        assert(NULL == mSystem);
    }
}

bool Radar_Controller::GetAnd() const { return mInValue.mAnd; }

unsigned char Radar_Controller::GetDstA() const { return mInValue.mDst[0]; }
unsigned char Radar_Controller::GetDstB() const { return mInValue.mDst[1]; }
unsigned char Radar_Controller::GetDstC() const { return mInValue.mDst[2]; }
unsigned char Radar_Controller::GetDstD() const { return mInValue.mDst[3]; }

const unsigned char * Radar_Controller::GetDstAB() const { return mDstAB[0][0]; }
const unsigned char * Radar_Controller::GetDstCD() const { return mDstCD[0][0]; }

unsigned char Radar_Controller::GetFade() const { return mInValue.mFade; }
unsigned char Radar_Controller::GetGain() const { return mInValue.mGain; }

bool Radar_Controller::GetOr() const { return (!mInValue.mAnd); }

unsigned char Radar_Controller::GetSrcA() const { return mInValue.mSrc[0]; }
unsigned char Radar_Controller::GetSrcB() const { return mInValue.mSrc[1]; }
unsigned char Radar_Controller::GetSrcC() const { return mInValue.mSrc[2]; }
unsigned char Radar_Controller::GetSrcD() const { return mInValue.mSrc[3]; }

const unsigned char * Radar_Controller::GetSrcAB() const { return mSrcAB[0][0]; }
const unsigned char * Radar_Controller::GetSrcCD() const { return mSrcCD[0][0]; }

void Radar_Controller::SetAnd() { mInValue.mAnd = true; Write_In(); Erase_DstAB(); Images_Update(); }

void Radar_Controller::SetDstA(unsigned char aA) { mInValue.mDst[0] = aA; Write_In(); Image_Draw(mDstAB, mInValue.mDst[0], mInValue.mDst[1]); Erase_DstCD(); Images_Update(); }
void Radar_Controller::SetDstB(unsigned char aB) { mInValue.mDst[1] = aB; Write_In(); Image_Draw(mDstAB, mInValue.mDst[0], mInValue.mDst[1]); Erase_DstCD(); Images_Update(); }
void Radar_Controller::SetDstC(unsigned char aC) { mInValue.mDst[2] = aC; Write_In(); Image_Draw(mDstCD, mInValue.mDst[2], mInValue.mDst[3]);                Images_Update(); }
void Radar_Controller::SetDstD(unsigned char aD) { mInValue.mDst[3] = aD; Write_In(); Image_Draw(mDstCD, mInValue.mDst[2], mInValue.mDst[3]);                Images_Update(); }

void Radar_Controller::SetFade(unsigned char aFade) { mInValue.mFade = aFade; Write_In(); }
void Radar_Controller::SetGain(unsigned char aGain) { mInValue.mGain = aGain; Write_In(); }

void Radar_Controller::SetOr() { mInValue.mAnd = false; Write_In(); Erase_DstAB(); Images_Update(); }

void Radar_Controller::SetSrcA(unsigned char aA) { mInValue.mSrc[0] = aA; Write_In(); Image_Draw(mSrcAB, mInValue.mSrc[0], mInValue.mSrc[1]); Erase_SrcCD(); Images_Update(); }
void Radar_Controller::SetSrcB(unsigned char aB) { mInValue.mSrc[1] = aB; Write_In(); Image_Draw(mSrcAB, mInValue.mSrc[0], mInValue.mSrc[1]); Erase_SrcCD(); Images_Update(); }
void Radar_Controller::SetSrcC(unsigned char aC) { mInValue.mSrc[2] = aC; Write_In(); Image_Draw(mSrcCD, mInValue.mSrc[2], mInValue.mSrc[3]); if (mInValue.mAnd) { Erase_DstAB(); } Images_Update(); }
void Radar_Controller::SetSrcD(unsigned char aD) { mInValue.mSrc[3] = aD; Write_In(); Image_Draw(mSrcCD, mInValue.mSrc[2], mInValue.mSrc[3]); if (mInValue.mAnd) { Erase_DstAB(); } Images_Update(); }

const char * Radar_Controller::GetStatus() const { return mStatus; };

// Return
//  false  Error
//  true   OK
bool Radar_Controller::Start()
{
    assert(NULL == mCommandQueue);
    assert(NULL == mIn          );
    assert(NULL == mOut         );
    assert(NULL == mSystem      );

    strcpy_s(mStatus, __FUNCTION__);

    mSystem = OpenNet::System::Create();
    assert(NULL != mSystem);

    OpenNet::Status lStatus;

    for (unsigned int i = 0; i < ADAPTER_QTY; i++)
    {
        assert(NULL == mAdapters[i]);

        mAdapters[i] = mSystem->Adapter_Get(i);
        if (NULL == mAdapters[i])
        {
            mSystem->Delete();

            mAdapters[0] = NULL;
            mSystem      = NULL;

            return false;
        }

        Adapter_Config(mAdapters[i]);
    }

    Processor_GetInfo();

    Buffers_Allocate();
    assert(NULL != mIn );
    assert(NULL != mOut);

    Erase_Create    ();
    Fade_Create     ();
    Adapters_Connect();

    mFilter.SetAdapter(mAdapters[1]);
    mFilter.SetIn     (mIn         );
    mFilter.SetOut    (mOut        );

    Adapter_SetInputFilter();

    memset(&mErase_Flags, 0, sizeof(mErase_Flags));
    mWriteIn_Flag = false;

    lStatus = mSystem->Start(0);
    assert(OpenNet::STATUS_OK == lStatus);

    mCommandQueue = reinterpret_cast<cl_command_queue>(mFilter.GetCommandQueue());
    assert(NULL != mCommandQueue);

    return true;
}

void Radar_Controller::Stop()
{
    assert(NULL != mAdapters [0]);
    assert(NULL != mCommandQueue);
    assert(NULL != mContext     );
    assert(NULL != mProcessor   );
    assert(NULL != mSystem      );

    strcpy_s(mStatus, __FUNCTION__);

    memset(&mErase_Flags, 0, sizeof(mErase_Flags));
    mWriteIn_Flag = false;

    OpenNet::Status lStatus = mSystem->Stop();
    assert(OpenNet::STATUS_OK == lStatus);

    lStatus = mAdapters[0]->ResetInputFilter();
    assert(OpenNet::STATUS_OK == lStatus);

    Fade_Release   ();
    Erase_Release  ();
    Buffers_Release();

    mSystem->Delete();

    for (unsigned int i = 0; i < 2; i++)
    {
        assert(NULL != mAdapters[i]);

        mAdapters[i] = NULL;
    }

    mCommandQueue = NULL;
    mContext      = NULL;
    mProcessor    = NULL;
    mSystem       = NULL;
}

void Radar_Controller::Tick()
{
    assert(EVENT_QTY >= mEventCount);

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    if (0 <  mEventCount) { Work_Wait (); }
    if (0 == mEventCount) { Work_Start(); }

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
}

// Private
/////////////////////////////////////////////////////////////////////////////

void Radar_Controller::Adapter_SetInputFilter()
{
    assert(NULL != mAdapters[0]);

    OpenNet::Status lStatus = mAdapters[0]->SetInputFilter(&mFilter);
    if (CL_SUCCESS != lStatus)
    {
        FILE * lFile;

        int lRet = fopen_s(&lFile, "Filter.txt", "w");
        assert(0 == lRet);
        assert(NULL != lFile);

        mFilter.Display(lFile);

        lRet = fclose(lFile);
        assert(0 == lRet);
    }

    assert(OpenNet::STATUS_OK == lStatus);
}

void Radar_Controller::Adapters_Connect()
{
    assert(NULL != mSystem);

    for (unsigned int i = 0; i < ADAPTER_QTY; i++)
    {
        assert(NULL != mAdapters[i]);

        OpenNet::Status lStatus = mSystem->Adapter_Connect(mAdapters[i]);
        assert(OpenNet::STATUS_OK == lStatus);
    }
}

void Radar_Controller::Buffers_Allocate()
{
    assert(NULL != mContext);

    cl_int lRet;

    mIn  = clCreateBuffer(mContext, CL_MEM_READ_ONLY  | CL_MEM_COPY_HOST_PTR, sizeof(mInValue ), &mInValue , &lRet);
    assert(CL_SUCCESS == lRet);
    assert(NULL       != mIn );

    mOut = clCreateBuffer(mContext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(mOutValue), &mOutValue, &lRet);
    assert(CL_SUCCESS == lRet);
    assert(NULL       != mOut);
}

void Radar_Controller::Buffers_Release()
{
    assert(NULL != mIn );
    assert(NULL != mOut);

    cl_int lRet = clReleaseMemObject(mIn);
    assert(CL_SUCCESS == lRet);

    lRet = clReleaseMemObject(mOut);

    mOut = NULL;
    mIn  = NULL;
}

void Radar_Controller::Erase_Create()
{
    assert(NULL != mContext );
    assert(NULL != mDeviceId);
    assert(NULL != mOut     );

    for (unsigned int i = 0; i < PART_QTY; i++)
    {
        assert(NULL == mErase_Kernels [i]);
        assert(NULL == mErase_Programs[i]);

        cl_int lRet;

        mErase_Programs[i] = clCreateProgramWithSource(mContext, ERASE_CODE_LENGTHS[i], ERASE_CODES[i], NULL, &lRet);
        assert(CL_SUCCESS == lRet              );
        assert(NULL       != mErase_Programs[i]);

        lRet = clBuildProgram(mErase_Programs[i], 1, &mDeviceId, NULL, NULL, NULL);
        assert(CL_SUCCESS == lRet);

        mErase_Kernels[i] = clCreateKernel(mErase_Programs[i], ERASE_NAMES[i], &lRet);
        assert(CL_SUCCESS == lRet             );
        assert(NULL       != mErase_Kernels[i]);

        lRet = clSetKernelArg(mErase_Kernels[i], 0, sizeof(cl_mem), &mOut);
        assert(CL_SUCCESS == lRet);
    }
}

void Radar_Controller::Erase_DstAB() { memset(&mOutValue.mDstAB, 0, sizeof(mOutValue.mDstAB)); mErase_Flags[PART_DST_AB] = true; Erase_DstCD(); }
void Radar_Controller::Erase_DstCD() { memset(&mOutValue.mDstCD, 0, sizeof(mOutValue.mDstCD)); mErase_Flags[PART_DST_CD] = true; }
void Radar_Controller::Erase_SrcCD() { memset(&mOutValue.mSrcCD, 0, sizeof(mOutValue.mSrcCD)); mErase_Flags[PART_SRC_CD] = true; if (mInValue.mAnd) { Erase_DstAB(); } }

void Radar_Controller::Erase_Release()
{
    for (unsigned int i = 0; i < PART_QTY; i++)
    {
        assert(NULL != mErase_Kernels [i]);
        assert(NULL != mErase_Programs[i]);

        cl_int lRet = clReleaseKernel(mErase_Kernels[i]);
        assert(CL_SUCCESS == lRet);

        lRet = clReleaseProgram(mErase_Programs[i]);
        assert(CL_SUCCESS == lRet);

        mErase_Kernels [i] = NULL;
        mErase_Programs[i] = NULL;
    }
}

void Radar_Controller::Fade_Create()
{
    assert(NULL != mContext     );
    assert(NULL != mDeviceId    );
    assert(NULL == mFade_Kernel );
    assert(NULL == mFade_Program);
    assert(NULL != mIn          );
    assert(NULL != mOut         );

    cl_int lRet;

    mFade_Program = clCreateProgramWithSource(mContext, sizeof(FADE) / sizeof(FADE[0]), FADE, NULL, &lRet);
    assert(CL_SUCCESS == lRet         );
    assert(NULL       != mFade_Program);

    lRet = clBuildProgram(mFade_Program, 1, &mDeviceId, NULL, NULL, NULL);
    assert(CL_SUCCESS == lRet);

    mFade_Kernel = clCreateKernel(mFade_Program, "Fade", &lRet);
    assert(CL_SUCCESS == lRet        );
    assert(NULL       != mFade_Kernel);

    lRet = clSetKernelArg(mFade_Kernel, 0, sizeof(cl_mem), &mIn);
    assert(CL_SUCCESS == lRet);

    lRet = clSetKernelArg(mFade_Kernel, 1, sizeof(cl_mem), &mOut);
    assert(CL_SUCCESS == lRet);
}

void Radar_Controller::Fade_Release()
{
    assert(NULL != mFade_Kernel );
    assert(NULL != mFade_Program);

    cl_int lRet = clReleaseKernel(mFade_Kernel);
    assert(CL_SUCCESS == lRet);

    lRet = clReleaseProgram(mFade_Program);
    assert(CL_SUCCESS == lRet);

    mFade_Kernel  = NULL;
    mFade_Program = NULL;
}

void Radar_Controller::Images_Draw()
{
    Image_Draw(mDstAB, mInValue.mDst[0], mInValue.mDst[1]);
    Image_Draw(mDstCD, mInValue.mDst[2], mInValue.mDst[3]);
    Image_Draw(mSrcAB, mInValue.mSrc[0], mInValue.mSrc[1]);
    Image_Draw(mSrcCD, mInValue.mSrc[2], mInValue.mSrc[3]);
}

void Radar_Controller::Images_Update()
{
    Image_Update(mDstAB, mOutValue.mDstAB);
    Image_Update(mDstCD, mOutValue.mDstCD);
    Image_Update(mSrcAB, mOutValue.mSrcAB);
    Image_Update(mSrcCD, mOutValue.mSrcCD);
}

void Radar_Controller::Processor_GetInfo()
{
    assert(NULL != mAdapters[0]);
    assert(NULL == mContext    );
    assert(NULL == mDeviceId   );
    assert(NULL == mProcessor  );

    mProcessor = mSystem->Processor_Get(0);
    assert(NULL != mProcessor);

    OpenNet::Status lStatus = mAdapters[0]->SetProcessor(mProcessor);
    assert(OpenNet::STATUS_OK == lStatus);

    mContext = reinterpret_cast<cl_context>(mProcessor->GetContext());
    assert(NULL != mContext);

    mDeviceId = reinterpret_cast<cl_device_id>(mProcessor->GetDevice());
    assert(NULL != mDeviceId);
}

// aMessage [---;R--] The message to add to the status
void Radar_Controller::UpdateStatus(const char * aMessage)
{
    assert(NULL != aMessage);

    assert(NULL != mAdapters[0]);

    unsigned int lStats[64 + 32];

    OpenNet::Status lStatus = mAdapters[0]->GetStatistics(lStats, sizeof(lStats));
    assert(OpenNet::STATUS_OK == lStatus);

    sprintf_s(mStatus, "%u packets - %u updates (%s)", lStats[64 + 30], mCount, aMessage);
}

void Radar_Controller::Work_Start()
{
    static const size_t OFFSETS[2] = {          0,          0 };
    static const size_t SIZES  [2] = { RADAR_SIZE, RADAR_SIZE };

    assert(NULL != mCommandQueue);
    assert(   0 == mEventCount  );
    assert(NULL != mFade_Kernel );
    assert(NULL != mOut         );

    cl_event   * lEvent      = NULL;
    unsigned int lEventCount =    0;
    cl_int       lRet;

    if (mWriteIn_Flag)
    {
        lRet = clEnqueueWriteBuffer(mCommandQueue, mIn, CL_FALSE, 0, sizeof(mInValue), &mInValue, lEventCount, lEvent, mEvents + mEventCount);
        assert(CL_SUCCESS == lRet                );
        assert(NULL       != mEvents[mEventCount]);

        mWriteIn_Flag = false; lEvent = mEvents + mEventCount; lEventCount = 1; mEventCount++;
    }

    for (unsigned int i = 0; i < PART_QTY; i++)
    {
        assert(NULL != mErase_Kernels[i]);

        if (mErase_Flags[i])
        {
            lRet = clEnqueueNDRangeKernel(mCommandQueue, mErase_Kernels[i], 2, OFFSETS, SIZES, NULL, lEventCount, lEvent, mEvents + mEventCount);
            assert(CL_SUCCESS == lRet                );
            assert(NULL       != mEvents[mEventCount]);

            mErase_Flags[i] = false; lEvent = mEvents + mEventCount; lEventCount = 1; mEventCount++;
        }
    }

    lRet = clEnqueueNDRangeKernel(mCommandQueue, mFade_Kernel, 2, OFFSETS, SIZES, NULL, lEventCount, lEvent, mEvents + mEventCount);
    assert(CL_SUCCESS == lRet                );
    assert(NULL       != mEvents[mEventCount]);

    lEvent = mEvents + mEventCount; lEventCount = 1; mEventCount++;

    lRet = clEnqueueReadBuffer(mCommandQueue, mOut, CL_FALSE, 0, sizeof(mOutValue), &mOutValue, lEventCount, lEvent, mEvents + mEventCount);
    assert(CL_SUCCESS == lRet                );
    assert(NULL       != mEvents[mEventCount]);

    lEvent = mEvents + mEventCount; lEventCount = 1; mEventCount++;

    lRet = clFlush(mCommandQueue);
    assert(CL_SUCCESS == lRet);
}

void Radar_Controller::Work_Wait()
{
    assert(        0 < mEventCount );
    assert(EVENT_QTY >= mEventCount);

    assert(NULL != mEvents[mEventCount - 1]);

    cl_int lStatus;

    cl_int lRet = clGetEventInfo(mEvents[mEventCount - 1], CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int), &lStatus, NULL);
    assert(CL_SUCCESS == lRet);

    switch (lStatus)
    {
    case CL_QUEUED   : UpdateStatus("Queued"   ); break;
    case CL_SUBMITTED: UpdateStatus("Submitted"); break;
    case CL_RUNNING  : UpdateStatus("Running"  ); break;
    case CL_COMPLETE:
        UpdateStatus("Complete");

        for (unsigned int i = 0; i < mEventCount; i++)
        {
            clReleaseEvent(mEvents[i]);
        }

        mEventCount = 0;

        Images_Update();

        mCount++;

        break;

    default: assert(false);
    }
}

void Radar_Controller::Write_In()
{
    mWriteIn_Flag = true;
}

// Static functions
/////////////////////////////////////////////////////////////////////////////

// aAdapter [---;RW-] The function configure this Adapter
void Adapter_Config(OpenNet::Adapter * aAdapter)
{
    assert(NULL != aAdapter);

    OpenNet::Status lStatus = aAdapter->ResetStatistics();
    assert(OpenNet::STATUS_OK == lStatus);

    OpenNet::Adapter::Config lConfig;

    lStatus = aAdapter->GetConfig(&lConfig);
    assert(OpenNet::STATUS_OK == lStatus);

    lConfig.mBufferQty = 2;

    lStatus = aAdapter->SetConfig(lConfig);
    assert(OpenNet::STATUS_OK == lStatus);
}

// aImage [---;-W-]
// aX
// aY
void Image_Draw(unsigned char aImage[][RADAR_SIZE][Radar_Controller::RGB_QTY], unsigned char aX, unsigned char aY)
{
    assert(NULL != aImage);

    memset(aImage, 0, RADAR_SIZE * RADAR_SIZE * Radar_Controller::RGB_QTY);

    for (unsigned int i = 0; i < RADAR_SIZE; i++)
    {
        aImage[aX][i ][Radar_Controller::RGB_BLUE] = Radar_Controller::RGB_MAX / 3;
        aImage[i ][aY][Radar_Controller::RGB_RED ] = Radar_Controller::RGB_MAX / 3;
    }
}

// aImage [---;-W-]
// aData  [---;R--]
void Image_Update(unsigned char aImage[][RADAR_SIZE][Radar_Controller::RGB_QTY], const unsigned char aData[][RADAR_SIZE])
{
    assert(NULL != aImage);
    assert(NULL != aData );

    for (unsigned int x = 0; x < RADAR_SIZE; x++)
    {
        for (unsigned int y = 0; y < RADAR_SIZE; y++)
        {
            aImage[x][y][Radar_Controller::RGB_GREEN] = aData[x][y];
        }
    }
}
