#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QDebug>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QApplication>
#include <QLabel>
#include <QSpinBox>
#include <QBrush>
#include <QColor>
#include <QMouseEvent>

#define ALL_FORMAT "All Files (*.*)"
#define TXT_FORMAT "Text File (*.txt)"
#define CSV_FORMAT "Comma Separated Values File (*.csv)"
#define C3D_FORMAT "Coordinate 3D File (*.c3d)"

#define DIALOG_OPTION QFileDialog::DontUseNativeDialog
//#define DIALOG_OPTION 0

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pointsOnScreen = 0;
    ui->GraphicWidget->PointsOnScreen(&pointsOnScreen);

    currFrame = 0;
    ui->GraphicWidget->CurrFrame(&currFrame);

    clusterSizeCheck = 0;
    isOpen_Cluster = false;
    clusterIsChanged = false;
    itemListSelected = 0;

    //Create Clock in Status Bar
    connect(&ClockTimer, SIGNAL(timeout()), this, SLOT(Clock()));
    ClockTimer.start(1000);
    ui->statusBar->addPermanentWidget(ui->clockLabel);
    ui->statusBar->addPermanentWidget(ui->spaceLabel);

    connect(&NumOfPoints, SIGNAL(timeout()), this, SLOT(SetNumOfPoints()));
    NumOfPoints.start(250);

    connect(&CurrFrame, SIGNAL(timeout()), this, SLOT(SetCurrFrame()));
    CurrFrame.start(100);

    connect(&clusterTimer, SIGNAL(timeout()), this, SLOT(SetCluster()));
    clusterTimer.start(100);

}

MainWindow::~MainWindow()
{
    if(ui->GraphicWidget->System().IsOpen_C3D())
        ui->GraphicWidget->System().CleanMemory();
    delete ui;
}

//Set the clock
void MainWindow::Clock()
{
    QTime time = QTime::currentTime();
    QString time_next = time.toString("hh : mm : ss");
    ui->clockLabel->setText(time_next);
    ui->clockLabel->show();
}

//Set Number of Points
void MainWindow::SetNumOfPoints() {
    ui->numberOfPoints->setText(QString::number(pointsOnScreen));
}

//Set Current Frame
void MainWindow::SetCurrFrame() {
    ui->currentFrame->setValue(currFrame);
}

void MainWindow::SetCluster() {

    if(isOpen_Cluster) {
        QBrush brush;
        QColor color;
        std::string str;
        if(clusterSizeCheck != ui->GraphicWidget->System().C3D_File().Cloud().ClusterSize()) {
            clusterIsChanged = false;
            clusterSizeCheck = ui->GraphicWidget->System().C3D_File().Cloud().ClusterSize();

            ui->clusterList->clear();
            for(int i = 0; i != clusterSizeCheck; ++i) {
                str = ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).Name() + " ("
                        + std::to_string(ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).PointsInCluster()) + ")";
                ui->clusterList->addItem(QString::fromStdString(str));

                color.setRgb(ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).RGB_().R(),
                             ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).RGB_().G(),
                             ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).RGB_().B());
                brush.setColor(color);
                ui->clusterList->item(i)->setForeground(brush);

                ui->clusterList->item(i)->setCheckState(Qt::Checked);
                ui->clusterList->item(i)->setCheckState(ui->clusterList->item(i)->checkState());

                ui->clusterList->item(i)->setBackground(Qt::black);

            }
        } else if(clusterIsChanged){
            for(int i = 0; i != clusterSizeCheck; ++i) {
                str = ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).Name() + " ("
                        + std::to_string(ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).PointsInCluster()) + ")";
                ui->clusterList->item(i)->setText(QString::fromStdString(str));

                color.setRgb(ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).RGB_().R(),
                             ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).RGB_().G(),
                             ui->GraphicWidget->System().C3D_File().Cloud().Cluster_(i).RGB_().B());
                brush.setColor(color);
                ui->clusterList->item(i)->setForeground(brush);
                ui->clusterList->item(i)->setCheckState(ui->clusterList->item(i)->checkState());
                ui->clusterList->item(i)->setBackground(Qt::black);

            }
        }


        my_float_ x_coord = ui->GraphicWidget->System().C3D_File().Cloud().Frame_(currFrame).Cluster_(itemListSelected).Centroid_().X();
        my_float_ y_coord = ui->GraphicWidget->System().C3D_File().Cloud().Frame_(currFrame).Cluster_(itemListSelected).Centroid_().Y();
        my_float_ z_coord = ui->GraphicWidget->System().C3D_File().Cloud().Frame_(currFrame).Cluster_(itemListSelected).Centroid_().Z();

        ui->X_coord->setText(QString::fromStdString(std::to_string(x_coord)));
        ui->Y_coord->setText(QString::fromStdString(std::to_string(y_coord)));
        ui->Z_coord->setText(QString::fromStdString(std::to_string(z_coord)));
    }
}

void MainWindow::MousePosition() {

    ;
}

/*********/
/* Grids */
/*********/

/* Show/Hide XY Grid */
void MainWindow::on_actionXY_triggered()
{
    ui->GraphicWidget->setGrid_XY(ui->actionXY->isChecked());
}

/* Show/Hide XZ Grid */
void MainWindow::on_actionXZ_triggered()
{
    ui->GraphicWidget->setGrid_XZ(ui->actionXZ->isChecked());
}

/* Show/Hide YZ Grid */
void MainWindow::on_actionYZ_triggered()
{
    ui->GraphicWidget->setGrid_YZ(ui->actionYZ->isChecked());
}

/*************/
/* Movements */
/*************/

/* Move Camera Up */
void MainWindow::on_actionUp_triggered()
{
    ui->GraphicWidget->setCoordY(-0.5);
}

/* Move Camera Down */
void MainWindow::on_actionDown_triggered()
{
    ui->GraphicWidget->setCoordY(0.5);
}

/* Move Camera Left */
void MainWindow::on_actionLeft_triggered()
{
    ui->GraphicWidget->setCoordX(0.5);
}

/* Move Camera Right */
void MainWindow::on_actionRight_triggered()
{
    ui->GraphicWidget->setCoordX(-0.5);
}

/* Move Camera Front */
void MainWindow::on_actionFront_triggered()
{
    ui->GraphicWidget->setCoordZ(0.5);
}

/* Move Camera Back */
void MainWindow::on_actionBack_triggered()
{
    ui->GraphicWidget->setCoordZ(-0.5);
}

/*************/
/* Rotations */
/*************/

/* Rotate Camera X+ */
void MainWindow::on_actionXp_triggered()
{
    ui->GraphicWidget->setRotX(5.0);
}

/* Rotate Camera X- */
void MainWindow::on_actionXm_triggered()
{
    ui->GraphicWidget->setRotX(-5.0);
}

/* Rotate Camera Y+ */
void MainWindow::on_actionYp_triggered()
{
    ui->GraphicWidget->setRotY(5.0);
}

/* Rotate Camera Y- */
void MainWindow::on_actionYm_triggered()
{
    ui->GraphicWidget->setRotY(-5.0);
}

/* Rotate Camera Z+ */
void MainWindow::on_actionZp_triggered()
{
    ui->GraphicWidget->setRotZ(5.0);
}

/* Rotate Camera Z- */
void MainWindow::on_actionZm_triggered()
{
    ui->GraphicWidget->setRotZ(-5.0);
}

/************/
/* Set View */
/************/

/* Set Pilot View */
void MainWindow::on_actionPilot_View_triggered()
{
    ui->actionPolar_View->setChecked(false);
    ui->actionPilot_View->setChecked(true);
    ui->GraphicWidget->resetTransform();
    ui->GraphicWidget->setView(PILOT_VIEW);
}

/* Set Polar View */
void MainWindow::on_actionPolar_View_triggered()
{
    ui->actionPilot_View->setChecked(false);
    ui->actionPolar_View->setChecked(true);
    ui->GraphicWidget->resetTransform();
    ui->GraphicWidget->setView(POLAR_VIEW);
}

/* Reset View Values */
void MainWindow::on_actionResetView_triggered()
{
    ui->GraphicWidget->resetTransform();
}

/****************/
/* File Actions */
/****************/

/* Exit the Application */
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit(); //Quit the application
}

/* Open a C3D File */
void MainWindow::on_actionC3D_triggered()
{
    QString filter = C3D_FORMAT;

    //set the file path Options = QFileDialog::DontUseNativeDialog
    QString openFilePath = QFileDialog::getOpenFileName(this, "Open", QDir::homePath(), filter, 0, DIALOG_OPTION);

    //Check if the file path isn't null
    if(openFilePath != nullptr) {
        //Check if the file has been read correctly
        if(ui->GraphicWidget->System().IsOpen_C3D())
            ui->GraphicWidget->System().CleanMemory();
       if( !ui->GraphicWidget->Open_C3D(openFilePath.toUtf8().constData() ) ) {
           ;
       }
       isOpen_Cluster = true;
    }
}

void MainWindow::on_actionHeader_triggered()
{
    QString filter = TXT_FORMAT;

    //set the file path
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), filter, 0, DIALOG_OPTION);
    //Check if the file path isn't null
    if(saveFilePath != nullptr) {
        //Check if the file has been saved correctly
       if( !ui->GraphicWidget->System().C3D_File().Header().PrintToFile( saveFilePath.toUtf8().constData() ) ) {
           ;
       }
    }
}

void MainWindow::on_actionParameter_triggered()
{
    QString filter = TXT_FORMAT;

    //set the file path Options = QFileDialog::DontUseNativeDialog
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), filter, 0, DIALOG_OPTION);
    //Check if the file path isn't null
    if(saveFilePath != nullptr) {
        //Check if the file has been saved correctly
       if( !ui->GraphicWidget->System().C3D_File().Parameter().PrintToFile( saveFilePath.toUtf8().constData() ) ) {
           ;
       }
    }
}

void MainWindow::on_actionPoint_csv_triggered()
{
    QString filter = CSV_FORMAT;

    //set the file path Options = QFileDialog::DontUseNativeDialog
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), filter, 0, DIALOG_OPTION);
    //Check if the file path isn't null
    if(saveFilePath != nullptr) {
        //Check if the file has been saved correctly
       if( !ui->GraphicWidget->System().C3D_File().Data().PrintPointsToFile_CSV( saveFilePath.toUtf8().constData() )) {
           ;
       }
    }
}

void MainWindow::on_actionAnalog_csv_triggered()
{
    QString filter = CSV_FORMAT;

    //set the file path Options = QFileDialog::DontUseNativeDialog
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), filter, 0, DIALOG_OPTION);
    //Check if the file path isn't null
    if(saveFilePath != nullptr) {
        //Check if the file has been saved correctly
       if( !ui->GraphicWidget->System().C3D_File().Data().PrintAnalogToFile_CSV( saveFilePath.toUtf8().constData() )) {
           ;
       }
    }
}

void MainWindow::on_actionCloudSet_Parameters_triggered()
{
    ui->GraphicWidget->SetValuesForCloudPrint(cloudCSV);
    cloudCSV.show();
}

void MainWindow::on_actionCloudCSV_triggered()
{

    QString filter = CSV_FORMAT;
    //set the file path Options = QFileDialog::DontUseNativeDialog
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), filter, 0, DIALOG_OPTION);
    //Check if the file path isn't null
    if(saveFilePath != nullptr) {
       //Check if the file has been saved correctly
       if( !ui->GraphicWidget->PrintCloud( saveFilePath.toUtf8().constData() )) {
           ;
       }
    }

}

void MainWindow::on_actionPlay_Pause_triggered()
{
    ui->GraphicWidget->SetPlay(!ui->GraphicWidget->System().Play());
}

void MainWindow::on_actionAbout_Crabs3D_triggered()
{
    QString text;

    text = "<p>Copyright (C) May 2018 <a href=\"https://github.com/JohnCrabs\">John Crabs</a> <br><b>johncrabs1995@gmail.com</b></p>";
    text += "<p>This program is free software: you can redistribute it and/or modify ";
    text += "it under the terms of the GNU General Public License as published by ";
    text += "the Free Software Foundation, either version 3 of the License, or ";
    text += "(at your option) any later version.</p>";
    text += "<p>This program is distributed in the hope that it will be useful, ";
    text += "but WITHOUT ANY WARRANTY; without even the implied warranty of ";
    text += "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the ";
    text += "GNU General Public License for more details.</p>";
    text += "<p>You should have received a copy of the GNU General Public License ";
    text += "along with this program.  If not, see <br><a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.";

    QMessageBox::about(this, "About Crabs3D", text);
}

void MainWindow::on_Velocity_valueChanged(int value)
{
    ui->GraphicWidget->SetFrameStep(value);
}

void MainWindow::on_actionAccumulated_Error_Remover_triggered()
{
    ui->GraphicWidget->RemoveAccumulatedNoise(accumNoiseRemover);
    accumNoiseRemover.exec();

}

void MainWindow::on_actionResetPointCloud_triggered()
{
    ui->GraphicWidget->ResetVisibility();
}

void MainWindow::on_actionMin_Nearest_Neighbour_Remover_triggered()
{
    ui->GraphicWidget->RemoveNearestNeighborMethod(nearNeigh);
    nearNeigh.show();
}

void MainWindow::on_actionUnits_triggered()
{
    ui->GraphicWidget->SetUnits(unitsDialog);
    unitsDialog.show();
}

void MainWindow::on_actionFix_Cloud_Irreversible_triggered()
{
    ui->GraphicWidget->FixCloudLinear();
}

void MainWindow::on_actionFix_Cloud_Weighted_Linear_Irreversible_triggered()
{
    ui->GraphicWidget->FixCloudLinearWithWeights();
}

void MainWindow::on_currentFrame_valueChanged(int arg1)
{
    ui->GraphicWidget->SetCurrFrame(arg1);
}

void MainWindow::on_actionClusters_triggered()
{
    ui->GraphicWidget->SetClusters(clusterWin, &clusterIsChanged, 0);
    clusterWin.show();
}

void MainWindow::on_actionKMeansRun_triggered()
{
    ui->GraphicWidget->K_Means();
}

void MainWindow::on_actionKMeansSet_Parameters_triggered()
{
    ui->GraphicWidget->SetK_Means(kmeansPar);
    kmeansPar.show();
}

void MainWindow::on_clusterList_itemDoubleClicked(QListWidgetItem *item)
{
    int index;
    for(int i = 0; i != clusterSizeCheck; ++i) {
        if(ui->clusterList->item(i)->text() == item->text()) {
            index = i;
            break;
        }
    }

    ui->GraphicWidget->SetClusters(clusterWin, &clusterIsChanged, index);
    clusterWin.show();
}

void MainWindow::on_clusterList_itemClicked(QListWidgetItem *item)
{
    int index;
    for(int i = 0; i != clusterSizeCheck; ++i) {
        if(ui->clusterList->item(i)->text() == item->text()) {
            index = i;
            break;
        }
    }
    itemListSelected = index;
    if(item->checkState() == Qt::Checked) {
        ui->GraphicWidget->SetClusterVisibility(index, true);
    } else {
        ui->GraphicWidget->SetClusterVisibility(index, false);
    }
}

void MainWindow::on_actionCrabs_Editor_triggered()
{
    crabsEditor.show();
}

void MainWindow::on_actionPrintClusters_triggered()
{
    QString filter = CSV_FORMAT;
    //set the file path Options = QFileDialog::DontUseNativeDialog
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save", QDir::homePath(), filter, 0, DIALOG_OPTION);
    //Check if the file path isn't null
    if(saveFilePath != nullptr) {
       //Check if the file has been saved correctly
       if( !ui->GraphicWidget->PrintClusters( saveFilePath.toUtf8().constData() )) {
           ;
       }
    }
}

void MainWindow::on_actionPoint_List_triggered()
{
    ui->GraphicWidget->SetPointList(pointList);

    pointList.show();
}
