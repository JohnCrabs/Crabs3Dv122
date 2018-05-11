#ifndef POINT_LIST_H
#define POINT_LIST_H

#include <QDialog>
#include <QListWidgetItem>
#include "pointcloud.h"

namespace Ui {
class PointList;
}

class PointList : public QDialog
{
    Q_OBJECT

public:
    explicit PointList(QWidget *parent = 0);
    ~PointList();

    void SetList(PointList &list, PointCloud *cloud_);

private slots:
    void on_pointList_itemClicked(QListWidgetItem *item);

    void on_ok_clicked();

private:
    Ui::PointList *ui;

    PointCloud *cloud;
};

#endif // POINT_LIST_H
