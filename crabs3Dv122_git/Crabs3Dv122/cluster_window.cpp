#include "cluster_window.h"
#include "ui_cluster_window.h"

#include<QComboBox>
#include<QString>
#include<QSpinBox>
#include<QLineEdit>

ClusterWindow::ClusterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClusterWindow)
{
    ui->setupUi(this);
}

ClusterWindow::~ClusterWindow()
{
    delete ui;
}

void ClusterWindow::SetClusters(ClusterWindow &winClust, PointCloud *cloud_, bool *state, int index) {
    winClust.cloud = cloud_;
    okClusterState = state;

    ui->clusterBox->clear();
    for(int i = 0; i != winClust.cloud->ClusterSize(); ++i) {
        ui->clusterBox->addItem(QString::fromStdString(winClust.cloud->Cluster_(i).Name()));
    }
    ui->clusterBox->setCurrentIndex(index);
    ui->redSpinBox->setValue(cloud->Cluster_(index).RGB_().R());
    ui->greenSpinBox->setValue(cloud->Cluster_(index).RGB_().G());
    ui->blueSpinBox->setValue(cloud->Cluster_(index).RGB_().B());

    red_val = cloud->Cluster_(index).RGB_().R();
    green_val = cloud->Cluster_(index).RGB_().G();
    blue_val = cloud->Cluster_(index).RGB_().B();

    ui->pointSizeSpinBox->setValue(cloud->Cluster_(index).PointSize_());

    name = cloud->Cluster_(index).Name();

    ui->clusterName->setText(QString::fromStdString(name));
}

void ClusterWindow::on_clusterBox_currentIndexChanged(int index)
{
    currIndex = index;
    ui->redSpinBox->setValue(cloud->Cluster_(index).RGB_().R());
    ui->greenSpinBox->setValue(cloud->Cluster_(index).RGB_().G());
    ui->blueSpinBox->setValue(cloud->Cluster_(index).RGB_().B());
}

void ClusterWindow::on_redSpinBox_valueChanged(int arg1)
{
    red_val = arg1;
}

void ClusterWindow::on_greenSpinBox_valueChanged(int arg1)
{
    green_val = arg1;
}

void ClusterWindow::on_blueSpinBox_valueChanged(int arg1)
{
    blue_val = arg1;
}

void ClusterWindow::on_pointSizeSpinBox_valueChanged(double arg1)
{
    pointSize = arg1;
}

void ClusterWindow::on_clusterName_textChanged(const QString &arg1)
{
    name = arg1.toStdString();
}

void ClusterWindow::on_Ok_clicked()
{
    *okClusterState = true;
    cloud->SetClusterColor(*cloud, currIndex, red_val, green_val, blue_val);
    cloud->SetPointSize(*cloud, currIndex, pointSize);
    cloud->SetName(*cloud, currIndex, name);
    QDialog::close();
}

void ClusterWindow::on_Cancel_clicked()
{
    *okClusterState = false;
    QDialog::close();
}
