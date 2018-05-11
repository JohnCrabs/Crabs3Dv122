#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "parameter_c3d.h"
#include "data_c3d.h"
#include <cstring>

using my_float_ = float;

class ColorRGB {
public:

    int R(void) {return red;}
    int G(void) {return green;}
    int B(void) {return blue;}

    void SetColor(ColorRGB &color, int r, int g, int b) {
        color.red = r;
        color.green = g;
        color.blue = b;
    }

private:
    int red;
    int green;
    int blue;

};

class Centroid {
public:
    inline my_float_ X(void) {return x;}
    inline my_float_ Y(void) {return y;}
    inline my_float_ Z(void) {return z;}
    inline int Size(void) {return size;}

    void SetCentroid(Centroid& centroid, my_float_ x_c, my_float_ y_c, my_float_ z_c) {
        centroid.x = x_c;
        centroid.y = y_c;
        centroid.z = z_c;
    }

    bool CentroidCmp(Centroid cent1, Centroid cent2) {
        if(cent1.x == cent2.x && cent1.y == cent2.y && cent1.z == cent2.z)
            return true;
        return false;
    }

    void CentroidCpy(Centroid& cent_to, Centroid cent_from) {
        cent_to.x = cent_from.x;
        cent_to.y = cent_from.y;
        cent_to.z = cent_from.z;
    }

    void SetSize(Centroid& cent_to, int sz) {cent_to.size = sz;}

private:
    my_float_ x = 0.0;
    my_float_ y = 0.0;
    my_float_ z = 0.0;
    int size;
};

class Cluster {
public:

    inline ColorRGB RGB_(void) {return rgb;}
    inline Centroid Centroid_(void) {return centroid;}
    inline my_float_ PointSize_(void) {return psize;}
    inline int ClusterSize_(void) {return csize;}
    inline int ID_(void) {return id;}
    inline std::string Name(void) {return name;}
    inline int PointsInCluster(void) {return pointInCluster;}
    inline bool Visible(void) {return visible;}

    inline void SetColor(Cluster &cluster, int r, int g, int b) {cluster.rgb.SetColor(cluster.rgb, r, g, b);}
    inline void SetCentroid(Cluster &cluster, my_float_ x, my_float_ y, my_float_ z) {
        cluster.centroid.SetCentroid(cluster.centroid, x, y, z);
    }
    inline void SetPointSize(Cluster &cluster, my_float_ sz) {cluster.psize = sz;}

    inline void SetClusterSize(Cluster &cluster, int sz) {cluster.csize = sz;}

    inline void SetId(Cluster &cluster, int id_) {cluster.id = id_;}

    inline void SetName(Cluster &cluster, const std::string& c_name) {
        int index = 0;
        for(auto i : c_name)
            cluster.name[index++] = i;
        cluster.name[index] = '\0';
    }

    inline void SetPointsInCluster(Cluster &cluster, int points_) {cluster.pointInCluster = points_;}

    inline void SetVisibility(Cluster &cluster, bool state) {cluster.visible = state;}

private:
    ColorRGB rgb;
    Centroid centroid;
    my_float_ psize;
    int csize;
    int id;
    char name[20];
    int pointInCluster;
    bool visible;
};

class ViewingPoint {
public:
    ViewingPoint();

    inline int ID(void) {return p_id;}
    inline int C_ID(void) {return c_id;}
    inline my_float_ X(void) {return x;}
    inline my_float_ Y(void) {return y;}
    inline my_float_ Z(void) {return z;}
    inline bool Visible(void) {return visibile;}
    inline std::string Name(void) {return name;}

    void SetPoint(ViewingPoint& point, const int id_, const int c_id_, const float coord_x,
                  const float coord_y, const float coord_z) {
        point.p_id = id_;
        point.c_id = c_id_;
        point.x = coord_x;
        point.y = coord_y;
        point.z = coord_z;
        point.visibile = true;
        std::string p_name = "point_" + std::to_string(id_);
        strcpy(point.name, p_name.c_str());
    }

    void SetVisibility(ViewingPoint& point, bool state) {point.visibile = state;}

    void Set_X(ViewingPoint &point, my_float_ x_) {point.x = x_;}
    void Set_Y(ViewingPoint &point, my_float_ y_) {point.y = y_;}
    void Set_Z(ViewingPoint &point, my_float_ z_) {point.z = z_;}

    void Set_CID(ViewingPoint &point, int c_id_) {point.c_id = c_id_;}

    void Set_Name(ViewingPoint &point, const std::string &p_name) {
        int index = 0;
        for(auto n:p_name) {
            if(index < 19)
                point.name[index++] = n;
        }
        point.name[index] = '\0';
    }

private:
    int p_id;
    int c_id;
    my_float_ x = 0.0;
    my_float_ y = 0.0;
    my_float_ z = 0.0;
    bool visibile = false;
    char name[20];
};

class Distance {
public:
    void SetDistance(Distance& d, my_float_ dx_, my_float_ dy_, my_float_ dz_, my_float_ id_start, my_float_ id_end);

    inline my_float_ DX_(void) {return dx;}
    inline my_float_ DY_(void) {return dy;}
    inline my_float_ DZ_(void) {return dz;}
    inline my_float_ DIST_(void) {return dist;}
    inline int ID_START_(void) {return id_start_point;}
    inline int ID_END_(void) {return id_end_point;}

private:
    my_float_ dx;
    my_float_ dy;
    my_float_ dz;
    my_float_ dist;
    int id_start_point;
    int id_end_point;
};

class Frame {
public:
    inline ViewingPoint Point_(const int index) {return point[index];}
    inline Distance Dist_(const int index) {return dist[index];}
    inline int DistSize_(void) {return distSize;}
    inline Cluster Cluster_(const int index) {return cluster[index];}

    void SetFrame(Frame& frame, Data_c3d data, Point point, const int frameIndex, const float multiplier);

    void SetPointVisibility(Frame& frame, int pointIndex, bool state) {frame.point->SetVisibility(frame.point[pointIndex], state);}

    void CleanMemory(Frame &frame) {
        free(frame.point);
        free(frame.dist);}

    void Set_X(Frame &frame, int pointIndex, my_float_ x_) {frame.point->Set_X(frame.point[pointIndex], x_);}
    void Set_Y(Frame &frame, int pointIndex, my_float_ y_) {frame.point->Set_Y(frame.point[pointIndex], y_);}
    void Set_Z(Frame &frame, int pointIndex, my_float_ z_) {frame.point->Set_Z(frame.point[pointIndex], z_);}

    void Set_CID(Frame &frame, int pointIndex, int c_id_) {frame.point->Set_CID(frame.point[pointIndex], c_id_);}

    int findMin_X(Frame &frame, int pointSize);
    int findMin_Y(Frame &frame, int pointSize);
    int findMin_Z(Frame &frame, int pointSize);

    int findMax_X(Frame &frame, int pointSize);
    int findMax_Y(Frame &frame, int pointSize);
    int findMax_Z(Frame &frame, int pointSize);

    inline void SetCentroid(Frame &frame, int c_index, my_float_ x, my_float_ y, my_float_ z) {
        frame.cluster->SetCentroid(frame.cluster[c_index], x, y, z);
    }

private:
    ViewingPoint* point;
    int distSize;
    bool distState;
    Distance* dist;
    Cluster* cluster;

};

class PointCloud
{
public:
    PointCloud();

    inline Frame Frame_(const int index) { return frame[index]; }
    inline int FrameSize(void) { return frameSize; }
    inline int PointSize(void) { return pointSize; }

    inline Cluster Cluster_(const int index) { return cluster[index]; }
    inline int ClusterSize(void) { return clusterSize; }

    void SetCloud(PointCloud& cloud, Data_c3d data, Point point, const float multiplier);

    inline void SetPointVisibility(PointCloud& cloud, int frameIndex, int pointIndex, bool state) {
        cloud.frame->SetPointVisibility(cloud.frame[frameIndex], pointIndex, state);
    }

    inline void ResetVisibility(PointCloud& cloud) {
        for(int i = 0; i != cloud.frameSize; ++i)
            for(int j = 0; j != cloud.pointSize; ++j)
                cloud.frame->SetPointVisibility(cloud.frame[i], j, true);
    }

    inline int VisiblePoints(PointCloud& cloud, int frame) {
        int cnt = 0;
        for(int i = 0; i < cloud.PointSize(); i++)
            if(cloud.Frame_(frame).Point_(i).Visible())
                ++cnt;
        return cnt;
    }

    inline void CleanMemory(void) {
        for(int i = 0; i != frameSize; ++i)
            frame->CleanMemory(frame[i]);
        free(frame);
        free(cluster);
    }

    void PrintCloud(void);

    void PrintCloud(int fromFrame, int toFrame, char seperator, std::string path);

    void PrintClusters(int fromFrame, int toFrame, char seperator, std::string path);

    void PrintDist(void);

    void AccumulatedNoiseRemoval(PointCloud& cloud, my_float_ thresh, my_float_ tolerance);

    void NearestNeighborRemoval(PointCloud& cloud, my_float_ thresh, int tolerance);

    void FixCloudLinear(PointCloud& cloud);

    void FixCloudLinearWithWeights(PointCloud& cloud);

    void K_means(PointCloud& cloud, int frameIndex, int clusterSize);

    void SetClusterColor(PointCloud& cloud, int clusterIndex, int red, int green, int blue) {
        cloud.cluster->SetColor(cloud.cluster[clusterIndex], red, green, blue);
    }

    void SetPointSize(PointCloud& cloud, int clusterIndex, my_float_ sz) {
        cloud.cluster->SetPointSize(cloud.cluster[clusterIndex], sz);
    }

    void SetClusterVisibility(PointCloud& cloud, int clusterIndex, bool state) {
        cloud.cluster->SetVisibility(cloud.cluster[clusterIndex], state);
    }

    void SetName(PointCloud& cloud, int clusterIndex, const std::string& c_name) {
        cloud.cluster->SetName(cloud.cluster[clusterIndex], c_name);
    }

private:
    Frame* frame;
    int frameSize;
    int pointSize;

    Cluster* cluster;
    int clusterSize;
};

#endif // POINTCLOUD_H
