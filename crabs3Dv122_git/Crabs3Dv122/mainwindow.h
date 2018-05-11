#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QString>
#include <QTimer>
#include <QListWidget>

#include "accumulation_noise_remover.h"
#include "nearest_neighbor_remover.h"
#include "units_dialog.h"
#include "export_cloud_csv.h"
#include "cluster_window.h"
#include "set_parameters_kmeans.h"
#include "crabs_editor.h"
#include "point_list.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void Clock();

    void SetNumOfPoints();

    void SetCurrFrame();

    void SetCluster();

    void MousePosition();

    /*********/
    /* Grids */
    /*********/

    void on_actionXY_triggered();

    void on_actionXZ_triggered();

    void on_actionYZ_triggered();

    /*************/
    /* Movements */
    /*************/

    void on_actionUp_triggered();

    void on_actionDown_triggered();

    void on_actionLeft_triggered();

    void on_actionRight_triggered();

    void on_actionFront_triggered();

    void on_actionBack_triggered();

    /*************/
    /* Rotations */
    /*************/

    void on_actionXp_triggered();

    void on_actionXm_triggered();

    void on_actionYp_triggered();

    void on_actionYm_triggered();

    void on_actionZp_triggered();

    void on_actionZm_triggered();

    /********/
    /* View */
    /********/

    void on_actionPilot_View_triggered();

    void on_actionPolar_View_triggered();

    void on_actionResetView_triggered();

    /***********/
    /* Actions */
    /***********/

    void on_actionExit_triggered();

    void on_actionC3D_triggered();

    void on_actionHeader_triggered();

    void on_actionParameter_triggered();

    void on_actionPoint_csv_triggered();

    void on_actionAnalog_csv_triggered();

    void on_actionPlay_Pause_triggered();

    void on_actionAbout_Crabs3D_triggered();

    void on_Velocity_valueChanged(int value);

    void on_actionAccumulated_Error_Remover_triggered();

    void on_actionResetPointCloud_triggered();

    void on_actionMin_Nearest_Neighbour_Remover_triggered();

    void on_actionUnits_triggered();

    void on_actionCloudCSV_triggered();

    void on_actionCloudSet_Parameters_triggered();

    void on_actionFix_Cloud_Irreversible_triggered();

    void on_currentFrame_valueChanged(int arg1);

    void on_actionClusters_triggered();

    void on_actionKMeansRun_triggered();

    void on_actionFix_Cloud_Weighted_Linear_Irreversible_triggered();

    void on_actionKMeansSet_Parameters_triggered();

    void on_clusterList_itemDoubleClicked(QListWidgetItem *item);

    void on_clusterList_itemClicked(QListWidgetItem *item);

    void on_actionCrabs_Editor_triggered();

    void on_actionPrintClusters_triggered();

    void on_actionPoint_List_triggered();

private:
    Ui::MainWindow *ui;

    //Timer
    QTimer ClockTimer;
    QTimer NumOfPoints;
    QTimer CurrFrame;
    QTimer clusterTimer;

    //Points On Screen and Current Frame
    int pointsOnScreen;
    int currFrame;

    //Dialogs
    AccumulationNoiseRemover accumNoiseRemover;
    NearestNeighborRemover nearNeigh;
    UnitsDialog unitsDialog;
    ExportCloudCSV cloudCSV;
    ClusterWindow clusterWin;
    SetParameters_KMeans kmeansPar;
    CrabsEditor crabsEditor;
    PointList pointList;

    //Cluster
    int clusterSizeCheck;
    bool isOpen_Cluster;
    bool clusterIsChanged;
    int itemListSelected;

};

#endif // MAINWINDOW_H
