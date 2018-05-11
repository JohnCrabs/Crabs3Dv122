#include "point_list.h"
#include "ui_point_list.h"
#include <QWidgetList>
#include <QString>

PointList::PointList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointList)
{
    ui->setupUi(this);
}

void PointList::SetList(PointList &list, PointCloud *cloud_) {
    list.cloud = cloud_;

    ui->pointList->clear();

    for(int point = 0; point != list.cloud->PointSize(); ++point) {
        ui->pointList->addItem(QString::fromStdString(cloud_->Frame_(0).Point_(point).Name()));
        if(cloud_->Frame_(0).Point_(point).Visible())
            ui->pointList->item(point)->setCheckState(Qt::Checked);
        else
            ui->pointList->item(point)->setCheckState(Qt::Unchecked);
    }
}

PointList::~PointList()
{
    delete ui;
}

void PointList::on_pointList_itemClicked(QListWidgetItem *item)
{
    int index;
    for(int i = 0; i != cloud->PointSize(); ++i) {
        if(ui->pointList->item(i)->text() == item->text()) {
            index = i;
            break;
        }
    }

    for(int i = 0; i < cloud->FrameSize(); ++i)
        cloud->SetPointVisibility(*cloud, i, index, item->checkState());
}

void PointList::on_ok_clicked()
{
    QDialog::close();
}
