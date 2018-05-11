#ifndef DATA_C3D_H
#define DATA_C3D_H

#include <cstdio>
#include <string>
#include <vector>
#include "types.h"


class Point_c3d {
public:

    inline float X(void) {return coord_x;}
    inline float Y(void) {return coord_y;}
    inline float Z(void) {return coord_z;}
    inline float Camera(void) {return camera;}
    inline float Residual(void) {return residual;}

    Point_c3d SetPoint(float coordinate_X, float coordinate_Y, float coordinate_Z,
                       float Camera, float Residual) {
        Point_c3d point;
        point.coord_x = coordinate_X;
        point.coord_y = coordinate_Y;
        point.coord_z = coordinate_Z;
        point.camera = Camera;
        point.residual = Residual;
        return point;
    }

private:
    float coord_x;
    float coord_y;
    float coord_z;

    float camera;
    float residual;

};

/**********************************************************/

class Analog_c3d {
public:
    inline float AnalogData(void) {return analog;}

    inline Analog_c3d SetAnalog(float Analog) {
        Analog_c3d analog;
        analog.analog = Analog;
        return analog;
    }

private:
    float analog;

};

/**********************************************************/

class Frame_c3d {
public:
    inline Point_c3d Point(const int index) {return point[index];}
    inline Analog_c3d Analog(const int index) {return analog[index];}

    void ReadFrame(Frame_c3d& frame, FILE* file, const float pSize, const float aSize,
                       const float pointScale, const int flag);

    void SetSizes(Frame_c3d& frame, const int pSize, const int aSize){
        frame.pointSize = pSize;
        frame.analogSize = aSize;
    }

    void CleanMemory(Frame_c3d& frame) {
        free(frame.point);
        free(frame.analog);
    }

private:
    Point_c3d* point;
    Analog_c3d* analog;

    int pointSize;
    int analogSize;

};

/**********************************************************/

class Data_c3d {
public:
    Data_c3d();

    inline Frame_c3d Frame(const int index) {return frame[index];}
    inline int PointSize(void) {return pointSize;}
    inline int FrameSize(void) {return frameSize;}
    inline int AnalogDataSize(void) {return analogDataSize;}

    Data_c3d ReadData(Data_c3d data, FILE* file, const float scale, const int flag);

    void SetSizes(Data_c3d& data, const int fSize, const int pSize, const int aSize){
        data.frameSize = fSize;
        data.pointSize = pSize;
        data.analogDataSize = aSize;
    }

    /* Print Points to CSV File */
    bool PrintPointsToFile_CSV(std::string path);

    /* Print Analog Data to CSV File */
    bool PrintAnalogToFile_CSV(std::string path);

    void CleanMemory() {
        for(int i = 0; i != frameSize; ++i)
            frame->CleanMemory(frame[i]);
        free(frame);
    }

private:
    Frame_c3d* frame;
    int pointSize;
    int frameSize;
    int analogDataSize;
};

#endif // DATA_C3D_H
