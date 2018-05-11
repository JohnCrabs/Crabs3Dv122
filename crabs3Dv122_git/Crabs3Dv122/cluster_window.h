#ifndef CLUSTER_WINDOW_H
#define CLUSTER_WINDOW_H

#include <string>

#include <QDialog>
#include "pointcloud.h"

namespace Ui {
class ClusterWindow;
}

class ClusterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ClusterWindow(QWidget *parent = 0);
    ~ClusterWindow();

    void SetClusters(ClusterWindow &winClust, PointCloud *cloud_, bool *state, int index);

private slots:
    void on_clusterBox_currentIndexChanged(int index);

    void on_redSpinBox_valueChanged(int arg1);

    void on_greenSpinBox_valueChanged(int arg1);

    void on_blueSpinBox_valueChanged(int arg1);

    void on_Ok_clicked();

    void on_Cancel_clicked();


    void on_pointSizeSpinBox_valueChanged(double arg1);

    void on_clusterName_textChanged(const QString &arg1);

private:
    Ui::ClusterWindow *ui;
    PointCloud *cloud;
    bool *okClusterState;

    int currIndex;
    int red_val;
    int green_val;
    int blue_val;

    my_float_ pointSize;

    std::string name;

};

#endif // CLUSTER_WINDOW_H
