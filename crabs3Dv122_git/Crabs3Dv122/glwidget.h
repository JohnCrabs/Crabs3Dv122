/**************/
/* glwidget.h */
/**************/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <iostream>

#include <QGLWidget>
#include <QTimer>

#include "systemgl.h"
#include "accumulation_noise_remover.h"
#include "nearest_neighbor_remover.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);

    void initializeGL(void);

    void paintGL(void);

    void resizeGL(int w, int h);

    //SystemGl setFunctions(...)
    inline void setEyeX(float dx) {mySystem.SetEyeX(dx);}
    inline void setEyeY(float dy) {mySystem.SetEyeY(dy);}
    inline void setEyeZ(float dz) {mySystem.SetEyeZ(dz);}

    inline void setCenterX(float dx) {mySystem.SetCenterX(dx);}
    inline void setCenterY(float dy) {mySystem.SetCenterY(dy);}
    inline void setCenterZ(float dz) {mySystem.SetCenterZ(dz);}

    inline void setUpX(float dx) {mySystem.SetUpX(dx);}
    inline void setUpY(float dy) {mySystem.SetUpY(dy);}
    inline void setUpZ(float dz) {mySystem.SetUpZ(dz);}

    inline void setRotX(float dx) {mySystem.SetRotX(dx);}
    inline void setRotY(float dy) {mySystem.SetRotY(dy);}
    inline void setRotZ(float dz) {mySystem.SetRotZ(dz);}

    inline void setCoordX(float dx) {mySystem.SetCoordX(dx);}
    inline void setCoordY(float dy) {mySystem.SetCoordY(dy);}
    inline void setCoordZ(float dz) {mySystem.SetCoordZ(dz);}

    inline void setGrid_XY(bool state) {mySystem.SetGrid_XY(state);}
    inline void setGrid_XZ(bool state) {mySystem.SetGrid_XZ(state);}
    inline void setGrid_YZ(bool state) {mySystem.SetGrid_YZ(state);}

    inline void resetTransform(void) {mySystem.resetTransform();}

    inline void setView(View v) {mySystem.setView(v);}

    inline bool Open_C3D(std::string path) {return mySystem.Open_C3D(path);}

    inline void SetPlay(bool state) { mySystem.SetPlay(state); }
    inline void SetFrameStep(const unsigned short int step) {mySystem.SetFrameStep(step);}

    SystemGL System(void) {return mySystem;}

    View PILOT(void) {return PILOT_VIEW;}
    View POLAR(void) {return POLAR_VIEW;}

    void ResetVisibility(void){mySystem.ResetVisibility();}


    void PointsOnScreen(int* points) {mySystem.PointsOnScreen(points);}
    void CurrFrame(int* frame) {mySystem.CurrFrame(frame);}
    void SetCurrFrame(int frameNum) {mySystem.SetCurrFrame(frameNum);}

    /*~~~~~~~~~*/
    /* Dialogs */
    /*~~~~~~~~~*/
    void RemoveAccumulatedNoise(AccumulationNoiseRemover &accNsRmv) {mySystem.RemoveAccumulatedNoise(accNsRmv);}
    void RemoveNearestNeighborMethod(NearestNeighborRemover &nearNeigh) {mySystem.RemoveNearestNeighborMethod(nearNeigh);}

    /*~~~~~~~*/
    /* Units */
    /*~~~~~~~*/
    void SetUnits(UnitsDialog &units) {mySystem.SetUnits(units);}

    /*~~~~~~~~~~~~*/
    /* PrintCloud */
    /*~~~~~~~~~~~~*/
    void SetValuesForCloudPrint(ExportCloudCSV &cloudCSV) {mySystem.SetValuesForCloudPrint(cloudCSV);}
    bool PrintCloud(std::string path) {return mySystem.PrintCloud(path);}
    bool PrintClusters(std::string path) {return mySystem.PrintClusters(path);}

    /*~~~~~~~~~~*/
    /* FixCloud */
    /*~~~~~~~~~~*/
    void FixCloudLinear(void) {mySystem.FixCloudLinear();}
    void FixCloudLinearWithWeights(void) {mySystem.FixCloudLinearWithWeights();}

    /*~~~~~~~~~~~~~~~~*/
    /* Classification */
    /*~~~~~~~~~~~~~~~~*/
    void SetK_Means(SetParameters_KMeans &kmeans) {mySystem.SetK_Means(kmeans);}
    void K_Means(void) {mySystem.K_Means();}

    void SetClusters(ClusterWindow &clusterWin, bool* okState, int index) {mySystem.SetClusters(clusterWin, okState, index);}

    void SetClusterVisibility(int index, bool state) {mySystem.SetClusterVisibility(index, state);}

    void SetPointList(PointList &list) {
        mySystem.SetPointList(list);
    }

private:
    QTimer GLtimer;

    SystemGL mySystem;
};

#endif // GLWIDGET_H
