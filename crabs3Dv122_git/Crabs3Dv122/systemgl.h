#ifndef SYSTEMC3D_H
#define SYSTEMC3D_H

#include <QWidget>
#include <iostream>

#include "c3d.h"
#include "accumulation_noise_remover.h"
#include "nearest_neighbor_remover.h"
#include "units_dialog.h"
#include "export_cloud_csv.h"
#include "cluster_window.h"
#include "set_parameters_kmeans.h"

#include "point_list.h"

enum View {PILOT_VIEW, POLAR_VIEW};

class SystemGL
{
public:
    SystemGL();

    void init(void);

    void display(void);

    void reshape(int w, int h);

    //-------------------------------------------------------------------------------//

    /*~~~~~~~~~~~~~~~~~*/
    /* Transformations */
    /*~~~~~~~~~~~~~~~~~*/

    inline void SetEyeX(my_float_ dx) {eyeX += dx;}
    inline void SetEyeY(my_float_ dy) {eyeY += dy;}
    inline void SetEyeZ(my_float_ dz) {eyeZ += dz;}

    inline void SetCenterX(my_float_ dx) {centerX += dx;}
    inline void SetCenterY(my_float_ dy) {centerY += dy;}
    inline void SetCenterZ(my_float_ dz) {centerZ += dz;}

    inline void SetUpX(my_float_ dx) {upX += dx;}
    inline void SetUpY(my_float_ dy) {upY += dy;}
    inline void SetUpZ(my_float_ dz) {upZ += dz;}

    inline void SetRotX(my_float_ dx) {rotX += dx;}
    inline void SetRotY(my_float_ dy) {rotY += dy;}
    inline void SetRotZ(my_float_ dz) {rotZ += dz;}

    inline void SetCoordX(my_float_ dx) {coordX += dx;}
    inline void SetCoordY(my_float_ dy) {coordY += dy;}
    inline void SetCoordZ(my_float_ dz) {coordZ += dz;}

    inline void SetGrid_XY(bool state) {gridXY_is_On = state;}
    inline void SetGrid_XZ(bool state) {gridXZ_is_On = state;}
    inline void SetGrid_YZ(bool state) {gridYZ_is_On = state;}

    inline void setView(View v) {view = v;}

    void resetTransform(void);

    bool Open_C3D(std::string path);
    inline bool IsOpen_C3D(void) {return isOpen_c3d;}
    inline C3D C3D_File(void) { return c3d_f; } /* Return C3D object. */
    void CleanMemory(void) { c3d_f.CleanMemory(); }
    void Draw_C3D(void);

    inline bool Play(void) {return playOn;}
    void SetPlay(bool state) {playOn = state;}
    void SetFrameStep(const unsigned short int step) {frameStep = step;}

    void ResetVisibility(void) {
        c3d_f.ResetVisibility(c3d_f);
        *pointsOnScreen = c3d_f.VisiblePoints(c3d_f, 0);
    }

    void PointsOnScreen(int* points) {pointsOnScreen = points;}
    void CurrFrame(int* frame) {currFrame = frame;}
    void SetCurrFrame(int frameNum) {
        if(frameNum > maxFrameValue)
            frameNum = maxFrameValue - 1;
        *currFrame = frameNum;
    }


    /*~~~~~~~~~*/
    /* Dialogs */
    /*~~~~~~~~~*/

    void RemoveAccumulatedNoise(AccumulationNoiseRemover &accNsRmv);
    void RemoveNearestNeighborMethod(NearestNeighborRemover &nearNeigh);

    /*~~~~~~~*/
    /* Units */
    /*~~~~~~~*/
    void SetUnits(UnitsDialog &units);


    /*~~~~~~~~~~~~*/
    /* PrintCloud */
    /*~~~~~~~~~~~~*/
    void SetValuesForCloudPrint(ExportCloudCSV &cloudCSV);
    bool PrintCloud(std::string path) {
        if(IsOpen_C3D()) {
            if(toFrame > maxFrameValue)
                toFrame = maxFrameValue;
            c3d_f.Cloud().PrintCloud(fromFrame, toFrame, seperator, path);
            return true;
        }

        return false;
    }

    bool PrintClusters(std::string path) {
        if(IsOpen_C3D()) {
            if(toFrame > maxFrameValue)
                toFrame = maxFrameValue;
            c3d_f.Cloud().PrintClusters(fromFrame, toFrame, seperator, path);
            return true;
        }

        return false;
    }

    /*~~~~~~~~~~*/
    /* FixCloud */
    /*~~~~~~~~~~*/
    void FixCloudLinear(void);
    void FixCloudLinearWithWeights(void);

    /*~~~~~~~~~~~~~~~~*/
    /* Classification */
    /*~~~~~~~~~~~~~~~~*/
    void SetK_Means(SetParameters_KMeans &kmeans) {
        kmeans.SetValues(kmeans, &frameForKmeans, &clusterSizeForKmeans, c3d_f.Cloud().FrameSize());
    }

    void K_Means(void) {
        if(frameForKmeans >= c3d_f.Cloud().FrameSize())
            frameForKmeans = c3d_f.Cloud().FrameSize() - 1;
        c3d_f.K_Means(c3d_f, frameForKmeans, clusterSizeForKmeans);
    }

    void SetClusters(ClusterWindow &clusterWin, bool* okState, int index) {c3d_f.SetClusters(clusterWin, &c3d_f, okState, index);}

    void SetClusterVisibility(int index, bool state) {
        c3d_f.SetClusterVisibility(c3d_f, index, state);
    }

    void SetPointList(PointList &list) {
        if(isOpen_c3d)
            c3d_f.SetPointList(list, &c3d_f);
    }

private:

    /* gluPerspective Variable */
    my_float_ eyeX;
    my_float_ eyeY;
    my_float_ eyeZ;
    my_float_ centerX;
    my_float_ centerY;
    my_float_ centerZ;
    my_float_ upX;
    my_float_ upY;
    my_float_ upZ;

    /* Transformation Variables */
    my_float_ rotX;
    my_float_ rotY;
    my_float_ rotZ;
    my_float_ coordX;
    my_float_ coordY;
    my_float_ coordZ;

    /* Grid Variables */
    bool gridXY_is_On;
    bool gridXZ_is_On;
    bool gridYZ_is_On;

    /* View Variables */
    View view;

    /* C3D Variables */
    bool isOpen_c3d;
    int C3DframeNum;
    C3D c3d_f;

    /* Actions */
    bool playOn;
    unsigned short int frameStep;

    /*Noise Accumulation Removal*/
    my_float_ accumThreshold;
    my_float_ accumTolerance;
    bool accumRunState;

    /*Noise Nearest Neighbor Removal*/
    my_float_ nearestThreshold;
    int nearestTolerance;
    bool nearestRunState;

    /*Points on Screen and Current Frame*/
    int *pointsOnScreen;
    int *currFrame;

    /*UnitScale*/
    float unitScale;

    /*CloudPrint*/
    int fromFrame;
    int toFrame;
    char seperator;
    int maxFrameValue;

    /*Kmeans*/
    int frameForKmeans;
    int clusterSizeForKmeans;

    //-------------------------------------------------------------------------------//

    /*~~~~~~~~~~~~~~~~*/
    /* View Functions */
    /*~~~~~~~~~~~~~~~~*/

    void pilotView(void);
    void polarView(void);

    /*~~~~~~~~~~~~~~~~*/
    /* Draw Functions */
    /*~~~~~~~~~~~~~~~~*/

    void drawView(void);
    void drawGrids(void);


    void checkRunStates(void);

};

#endif // SYSTEMC3D_H
