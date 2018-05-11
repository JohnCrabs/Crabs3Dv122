#ifndef HEADER_C3D_H
#define HEADER_C3D_H

#include <cstdio>
#include <string>
#include <vector>

#include "types.h"

class Header_c3d
{
public:
    const static int futureBlock_1_size = 135;
    const static int eventTime_size = 18;
    const static int displayFlag_size = 9;
    const static int eventLabel_size = 18;
    const static int futureBlock_4_size = 22;

    Header_c3d(); /* Constructor */

    inline byte ParameterBlock(void) {return parameterBlock;}
    inline byte NumberID(void) {return idNumber;}
    inline byte NumberOfPoints(void) {return pointsNumber;}
    inline byte NumberOfAnalog(void) {return analogNumber;}
    inline byte FirstFrame(void) {return firstFrame;}
    inline byte LastFrame(void) {return lastFrame;}
    inline byte MaxGap(void) {return maximumInterpolationGap;}
    inline real ScaleFactor(void) {return scaleFactor;}
    inline byte DataStart(void) {return dataStart;}
    inline byte AnalogPerFrame(void) {return analogPerFrame;}
    inline real FrameRate(void) {return frameRate;}

    inline byte FutureBlock_1(const int index) {
        if(index < 0 || index >= futureBlock_1_size)
            return (byte)NULL;
        else
            return futureUseBlock_1[index];
    }

    inline byte KeyValue_1(void) {return keyValue_1;}
    inline byte FirstLabelRange(void) {return firstLabelRange;}
    inline byte KeyValue_2(void) {return keyValue_2;}
    inline byte EventTimeSize(void) {return eventTimeSize;}
    inline byte FutureBlock_2(void) {return futureUseBlock_2;}

    inline real EventTimeInSeconds(const int index) {
        if(index < 0 || index >= eventTime_size)
            return (real)NULL;
        else
            return eventTimeInSeconds[index];
    }

    inline byte EventDisplayFlags(const int index) {
        if(index < 0 || index >= displayFlag_size)
            return (byte)NULL;
        else
            return eventDisplayFlags[index];
    }

    inline byte FutureBlock_3(void) {return futureUseBlock_3;}

    inline std::string EventLabel(const int index) {
        if(index < 0 || index >= eventLabel_size)
            return (byte)NULL;
        else {
            std::string str(eventLabels[index]);
            return str;
        }
    }

    inline byte FutureBlock_4(const int index) {
        if(index < 0 || index >= displayFlag_size)
            return (byte)NULL;
        else
            return futureUseBlock_4[index];
    }

    /* Read the 512-bytes Header block */
    Header_c3d ReadHeaderBlock(FILE* file);

    /* Swap the Endian Reading System */
    Header_c3d SwapHeaderEndianSystem(Header_c3d header);

    /* Print Header to File */
    bool PrintToFile(std::string path);

    virtual ~Header_c3d() {} /* Destructor */

private:
    byte parameterBlock; /* byte 1: Points to the first block of the parameter section. */
    byte idNumber; /* byte 2: Key value 0x50hex indicating a C3D file */
    byte pointsNumber; /* Number of 3D points in the C3D file (i.e. the number of the stored trajectories). */
    byte analogNumber; /* Total number of analog measurements per 3D frame, i.e. number of
                             * channels multiplied by the samples per channel.
                             */
    byte firstFrame; /* Number of the first frame of the 3D data. */
    byte lastFrame; /* Number of the last frame of 3D data. */
    byte maximumInterpolationGap; /* Maximum interpolation gap in 3D frames. */
    real scaleFactor; /* The 3D scale factor (realing-point) that converts signed integer 3D data to reference system
                        * measurement unit. If this is negative then the file is scaled in realing-point.
                        */
    byte dataStart; /* DATA_START - the number of the first block of the 3D and Analog data section. */
    byte analogPerFrame; /* The number of Analog samples per 3D frame. */
    real frameRate; /* The 3D frame rate in Hz (realing-point) */
    std::vector<byte> futureUseBlock_1; /* Reserved for future use. */
    byte keyValue_1; /* A key value (12345 decimal) is written here if Label and Range data is present,
                           * otherwise write 0x00.
                           */
    byte firstLabelRange; /* The first block of the Label and Range section (if present). */
    byte keyValue_2; /* A key value (12345 decimal) present if this file supports 4 char event labels.
                           * An older format supported only 2 character labels.
                           */
    byte eventTimeSize; /* Number of defined time event (0 to 18). */
    byte futureUseBlock_2; /* Reserved for future use. */
    std::vector<real> eventTimeInSeconds; /* Event time (realing-point) measured in seconds (up to 18 events). */
    std::vector<byte> eventDisplayFlags; /* 18-byte Event display Flags 0x00 = ON or 0x01 = OFF. */
    byte futureUseBlock_3; /* Reserved for future use. */
    std::vector<std::string> eventLabels; /* Event labels. Each label is 4 character long. */
    std::vector<byte> futureUseBlock_4; /* Reserved for future use. */
};

#endif // HEADER_C3D_H
