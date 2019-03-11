
// Author   KMS - Martin Dubois, ing.
// Product  OpenNet
// File     Samples/Radar/Radar_Kernel.h

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <assert.h>
#include <stdint.h>
#include <string.h>

// ===== Samples/Radar ======================================================
#include "Radar_Types.h"

#include "Radar_Kernel.h"

// OpenCL
/////////////////////////////////////////////////////////////////////////////

#define EOL "\n"

static const char * FILTER =
    "#include <OpenNetK/Kernel.h>"                                                                          EOL
                                                                                                            EOL
    "#include <OpenNetK/ARP.h>"                                                                             EOL
    "#include <OpenNetK/Ethernet.h>"                                                                        EOL
    "#include <OpenNetK/IPv4.h>"                                                                            EOL
                                                                                                            EOL
    RADAR_IN
    RADAR_OUT
                                                                                                            EOL
    "__kernel void Filter( __global OpenNet_BufferHeader * aBufferHeader, __constant Radar_In * aIn, __global Radar_Out * aOut )" EOL
    "{"                                                                                                     EOL
    "    OPEN_NET_KERNEL_BEGIN"                                                                             EOL
                                                                                                            EOL
    "        unsigned int lResult = OPEN_NET_PACKET_PROCESSED;"                                             EOL
                                                                                                            EOL
    "        __global unsigned char * lEthData = Ethernet_Data( lBase, lPacketInfo );"                      EOL
    "        unsigned short           lEthType = Ethernet_Type( lBase, lPacketInfo );"                      EOL
                                                                                                            EOL
    "        __global unsigned char * lDst = 0;"                                                            EOL
    "        __global unsigned char * lSrc = 0;"                                                            EOL
                                                                                                            EOL
    "        if ( ARP_ETHERNET_TYPE == lEthType )"                                                          EOL
    "        {"                                                                                             EOL
    "            lDst = ARP_Destination( lEthData );"                                                       EOL
    "            lSrc = ARP_Source     ( lEthData );"                                                       EOL
    "        }"                                                                                             EOL
                                                                                                            EOL
    "        if ( IPv4_ETHERNET_TYPE == lEthType )"                                                         EOL
    "        {"                                                                                             EOL
    "            lDst = IPv4_Destination( lEthData );"                                                      EOL
    "            lSrc = IPv4_Source     ( lEthData );"                                                      EOL
    "        }"                                                                                             EOL
                                                                                                            EOL
    "        if ( 0 != lSrc )"                                                                              EOL
    "        {"                                                                                             EOL
    "            aOut->mSrcAB[ lSrc[ 0 ] ][ lSrc[ 1 ] ] = add_sat( aOut->mSrcAB[ lSrc[ 0 ] ][ lSrc[ 1 ] ], aIn->mGain );" EOL
                                                                                                            EOL
    "            if ( ( lSrc[ 0 ] == aIn->mSrc[ 0 ] ) && ( lSrc[ 1 ] == aIn->mSrc[ 1 ] ) )"                 EOL
    "            {"                                                                                         EOL
    "                aOut->mSrcCD[ lSrc[ 2 ] ][ lSrc[ 3 ] ] = add_sat( aOut->mSrcCD[ lSrc[ 2 ] ][ lSrc[ 3 ] ], aIn->mGain );" EOL
                                                                                                            EOL
    "                if ( ( lSrc[ 2 ] == aIn->mSrc[ 2 ] ) && ( lSrc[ 3 ] == aIn->mSrc[ 3 ] ) )"             EOL
    "                {"                                                                                     EOL
    "                    if ( aIn->mAnd )"                                                                  EOL
    "                    {"                                                                                 EOL
    "                        aOut->mDstAB[ lDst[ 0 ] ][ lDst[ 1 ] ] = add_sat( aOut->mDstAB[ lDst[ 0 ] ][ lDst[ 1 ] ], aIn->mGain );" EOL
                                                                                                            EOL
    "                        if ( ( lDst[ 0 ] == aIn->mDst[ 0 ] ) && ( lDst[ 1 ] == aIn->mDst[ 1 ] ) )"     EOL
    "                        {"                                                                             EOL
    "                            aOut->mDstCD[ lDst[ 2 ] ][ lDst[ 3 ] ] = add_sat( aOut->mDstCD[ lDst[ 2 ] ][ lDst[ 3 ] ], aIn->mGain );" EOL
                                                                                                            EOL
    "                            if ( ( lDst[ 2 ] == aIn->mDst[ 2 ] ) && ( lDst[ 3 ] == aIn->mDst[ 3 ] ) )" EOL
    "                            {"                                                                         EOL
    "                                lResult |= 1 << ADAPTER_INDEX;"                                        EOL
    "                            }"                                                                         EOL
    "                        }"                                                                             EOL
    "                    }"                                                                                 EOL
    "                    else"                                                                              EOL
    "                    {"                                                                                 EOL
    "                        lResult |= 1 << ADAPTER_INDEX;"                                                EOL
    "                    }"                                                                                 EOL
    "                }"                                                                                     EOL
    "            }"                                                                                         EOL
    "        }"                                                                                             EOL
                                                                                                            EOL
    "        if ( ( 0 != lDst ) && ( ! aIn->mAnd ) )"                                                       EOL
    "        {"                                                                                             EOL
    "            aOut->mDstAB[ lDst[ 0 ] ][ lDst[ 1 ] ] = add_sat( aOut->mDstAB[ lDst[ 0 ] ][ lDst[ 1 ] ], aIn->mGain);" EOL
                                                                                                            EOL
    "            if ( ( lDst[ 0 ] == aIn->mDst[ 0 ] ) && ( lDst[ 1 ] == aIn->mDst[ 1 ] ) )"                 EOL
    "            {"                                                                                         EOL
    "                aOut->mDstCD[ lDst[ 2 ] ][ lDst[ 3 ] ] = add_sat( aOut->mDstCD[ lDst[ 2 ] ][ lDst[ 3 ] ], aIn->mGain );" EOL
                                                                                                            EOL
    "                if ( ( lDst[ 2 ] == aIn->mDst[ 2 ] ) && ( lDst[ 3 ] == aIn->mDst[ 3 ] ) )"             EOL
    "                {"                                                                                     EOL
    "                    lResult |= 1 << ADAPTER_INDEX;"                                                    EOL
    "                }"                                                                                     EOL
    "            }"                                                                                         EOL
    "        }"                                                                                             EOL
                                                                                                            EOL
    "        lPacketInfo->mSendTo = lResult;"                                                               EOL
                                                                                                            EOL
    "    OPEN_NET_KERNEL_END"                                                                               EOL
    "}"                                                                                                     EOL;

// Public
/////////////////////////////////////////////////////////////////////////////

Radar_Kernel::Radar_Kernel() : mIn(NULL), mOut(NULL)
{
    unsigned int lLength = static_cast<unsigned int>(strlen(FILTER));
    assert(0 < lLength);

    OpenNet::Status lStatus = SetCode(FILTER, lLength);
    assert(OpenNet::STATUS_OK == lStatus);
}

// aAdapter [---;R--]
void Radar_Kernel::SetAdapter(OpenNet::Adapter * aAdapter)
{
    assert(NULL != aAdapter);

    unsigned int lAdapterNo;

    OpenNet::Status lStatus = aAdapter->GetAdapterNo(&lAdapterNo);
    assert(OpenNet::STATUS_OK == lStatus);

    char lStr[8];

    sprintf_s(lStr, "%u", lAdapterNo);

    unsigned int lRet = Edit_Replace("ADAPTER_INDEX", lStr);
    assert(3 == lRet);
}

// aIn [-K-;R--]
void Radar_Kernel::SetIn(cl_mem aIn)
{
    assert(NULL != aIn);

    assert(NULL == mIn);

    mIn = aIn;
}

// aOut [-K-;R--]
void Radar_Kernel::SetOut(cl_mem aOut)
{
    assert(NULL != aOut    );

    assert(NULL == mOut     );

    mOut = aOut;
}

// ===== OpenNet::Kernel ====================================================

void Radar_Kernel::SetUserKernelArgs(void * aKernel)
{
    assert(NULL != mIn );
    assert(NULL != mOut);

    cl_kernel lKernel = reinterpret_cast<cl_kernel>(aKernel);
    assert(NULL != lKernel);

    cl_int lRet = clSetKernelArg(lKernel, 1, sizeof(cl_mem), &mIn);
    assert(CL_SUCCESS == lRet);

    lRet = clSetKernelArg(lKernel, 2, sizeof(cl_mem), &mOut);
    assert(CL_SUCCESS == lRet);

    Kernel::SetUserKernelArgs(aKernel);
}
