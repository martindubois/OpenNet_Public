
// Author   KMS - Martin Dubois, ing.
// Product  OpenNet
// File     Samples/Radar/Radar_Kernel.h

#pragma once

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== OpenCL =============================================================
#include <CL/opencl.h>

// ===== Includes ===========================================================
#include <OpenNet/Adapter.h>
#include <OpenNet/Kernel.h>

// Class
/////////////////////////////////////////////////////////////////////////////

class Radar_Kernel : public OpenNet::Kernel
{

public:

    Radar_Kernel();

    void SetAdapter(OpenNet::Adapter * aAdapter);
    void SetIn     (cl_mem             aIn     );
    void SetOut    (cl_mem             aOut    );

    // ===== OpenNet::Kernel ================================================
    virtual void SetUserKernelArgs(void * aKernel);

private:

    cl_mem       mIn      ;
    cl_mem       mOut     ;

};
