#include "set_parameters_kmeans.h"
#include "ui_set_parameters_kmeans.h"

#include <QSpinBox>

SetParameters_KMeans::SetParameters_KMeans(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetParameters_KMeans)
{
    ui->setupUi(this);
}

SetParameters_KMeans::~SetParameters_KMeans()
{
    delete ui;
}

void SetParameters_KMeans::SetValues(SetParameters_KMeans &kmeans, int *frame, int *clusterSize, int frameSize) {
    kmeans.selectionFrame = frame;
    kmeans.clusterSize = clusterSize;
    kmeans.maxFrameSize = frameSize;

    ui->selectFrameSpin->setValue(*frame);
    ui->clustersSpin->setValue(*clusterSize);

}

void SetParameters_KMeans::on_Ok_clicked()
{
    *selectionFrame = ui->selectFrameSpin->value();
    *clusterSize = ui->clustersSpin->value();
    if(*selectionFrame >= maxFrameSize)
        *selectionFrame -= 1;

    QDialog::close();
}

void SetParameters_KMeans::on_Cancel_clicked()
{
    QDialog::close();
}
