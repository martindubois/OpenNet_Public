
// Author   KMS - Martin Dubois, ing
// Product  OpenNet
// File     Samples/Radar/Radar_Types.h

#pragma once

// Constants
/////////////////////////////////////////////////////////////////////////////

#define RADAR_SIZE (256)

#define RADAR_EOL "\n"

#define RADAR_IN                           \
    "typedef struct"             RADAR_EOL \
    "{"                          RADAR_EOL \
    "    unsigned char mDst[4];" RADAR_EOL \
    "    unsigned char mSrc[4];" RADAR_EOL \
                                 RADAR_EOL \
    "    unsigned char mAnd ;"   RADAR_EOL \
    "    unsigned char mFade;"   RADAR_EOL \
    "    unsigned char mGain;"   RADAR_EOL \
    "}"                          RADAR_EOL \
    "Radar_In;"                  RADAR_EOL

#define RADAR_IN_V                          \
    "typedef struct"             RADAR_EOL, \
    "{"                          RADAR_EOL, \
    "    unsigned char mDst[4];" RADAR_EOL, \
    "    unsigned char mSrc[4];" RADAR_EOL, \
                                 RADAR_EOL, \
    "    unsigned char mAnd ;"   RADAR_EOL, \
    "    unsigned char mFade;"   RADAR_EOL, \
    "    unsigned char mGain;"   RADAR_EOL, \
    "}"                          RADAR_EOL, \
    "Radar_In;"                  RADAR_EOL,

#define RADAR_OUT                                                 \
    "#define RADAR_SIZE (256)"                          RADAR_EOL \
    "typedef struct"                                    RADAR_EOL \
    "{"                                                 RADAR_EOL \
    "    unsigned char mDstAB[RADAR_SIZE][RADAR_SIZE];" RADAR_EOL \
    "    unsigned char mDstCD[RADAR_SIZE][RADAR_SIZE];" RADAR_EOL \
                                                        RADAR_EOL \
    "    unsigned char mSrcAB[RADAR_SIZE][RADAR_SIZE];" RADAR_EOL \
    "    unsigned char mSrcCD[RADAR_SIZE][RADAR_SIZE];" RADAR_EOL \
    "}"                                                 RADAR_EOL \
    "Radar_Out;"                                        RADAR_EOL

#define RADAR_OUT_V                                                \
    "#define RADAR_SIZE (256)"                          RADAR_EOL, \
    "typedef struct"                                    RADAR_EOL, \
    "{"                                                 RADAR_EOL, \
    "    unsigned char mDstAB[RADAR_SIZE][RADAR_SIZE];" RADAR_EOL, \
    "    unsigned char mDstCD[RADAR_SIZE][RADAR_SIZE];" RADAR_EOL, \
                                                        RADAR_EOL, \
    "    unsigned char mSrcAB[RADAR_SIZE][RADAR_SIZE];" RADAR_EOL, \
    "    unsigned char mSrcCD[RADAR_SIZE][RADAR_SIZE];" RADAR_EOL, \
    "}"                                                 RADAR_EOL, \
    "Radar_Out;"                                        RADAR_EOL,

// Data types
/////////////////////////////////////////////////////////////////////////////

typedef struct
{
    unsigned char mDst[4];
    unsigned char mSrc[4];

    unsigned char mAnd ;
    unsigned char mFade;
    unsigned char mGain;
}
Radar_In;

typedef struct
{
    unsigned char mDstAB[RADAR_SIZE][RADAR_SIZE];
    unsigned char mDstCD[RADAR_SIZE][RADAR_SIZE];

    unsigned char mSrcAB[RADAR_SIZE][RADAR_SIZE];
    unsigned char mSrcCD[RADAR_SIZE][RADAR_SIZE];
}
Radar_Out;
