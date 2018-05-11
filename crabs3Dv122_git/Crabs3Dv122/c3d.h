#ifndef C3D_H
#define C3D_H

#include "header_c3d.h"
#include "parameter_c3d.h"
#include "data_c3d.h"
#include "pointcloud.h"
#include "types.h"

#include "cluster_window.h"

#include "point_list.h"

class C3D
{
public:
    C3D();

    inline Header_c3d Header(void) {return header;}
    inline Parameter_c3d Parameter(void) {return parameter;}
    inline Data_c3d Data(void) {return data;}

    inline Trial getTrial(void) {return trial;}
    inline Subjects getSubjects(void) {return subjects;}
    inline Point getPoint(void) {return point;}
    inline Manufacturer getManufacturer(void) {return manufacturer;}

    inline PointCloud Cloud(void) {return cloud;}

    inline bool IsOpen(void) {return isOpen;}
    inline int SysEndian(void) {return sysEndian;}
    inline float Multiplier(void) {return multiplier;}

    bool Read_C3D_File(std::string filePath, C3D &c3d_f);

    void CleanMemory(void) {
        isOpen = false;
        parameter.CleanMemory();
        data.CleanMemory();
        cloud.CleanMemory();
    }

    int VisiblePoints(C3D& c3d_f, int frame) {return c3d_f.cloud.VisiblePoints(c3d_f.cloud, frame);}

    //Noise Remove Cloud Calling Methods
    void AccumulatedNoiseRemoval(C3D& c3d_f, my_float_ thresh, my_float_ tolerance) {
        c3d_f.cloud.AccumulatedNoiseRemoval(c3d_f.cloud, thresh, tolerance);
    }

    void NearestNeighborRemoval(C3D& c3d_f, my_float_ thresh, int tolerance) {
        c3d_f.cloud.NearestNeighborRemoval(c3d_f.cloud, thresh, tolerance);
    }

    void ResetVisibility(C3D& c3d_f) { c3d_f.cloud.ResetVisibility(c3d_f.cloud); }

    void FixCloudLinear(C3D& c3d_f) { c3d_f.cloud.FixCloudLinear(c3d_f.cloud); }

    void FixCloudLinearWithWeights(C3D& c3d_f) { c3d_f.cloud.FixCloudLinearWithWeights(c3d_f.cloud); }

    void K_Means(C3D& c3d_f, int frameIndex, int clusterSize) {c3d_f.cloud.K_means(c3d_f.cloud, frameIndex, clusterSize);}

    void SetClusters(ClusterWindow &clusterWin, C3D* c3d_f, bool* okState, int index) {
        clusterWin.SetClusters(clusterWin, &c3d_f->cloud, okState, index);
    }

    void SetPointList(PointList &list, C3D* c3d_f) {
        list.SetList(list, &c3d_f->cloud);
    }

    void SetClusterVisibility(C3D& c3d_f, int clusterIndex, bool state) {
        c3d_f.cloud.SetClusterVisibility(c3d_f.cloud, clusterIndex, state);
    }

private:
    Header_c3d header;
    Parameter_c3d parameter;
    Data_c3d data;

    Trial trial;
    Subjects subjects;
    Point point;
    Manufacturer manufacturer;

    PointCloud cloud;

    bool isOpen = false;
    int sysEndian;
    int fileEndian;
    float multiplier;
};

#endif // C3D_H
