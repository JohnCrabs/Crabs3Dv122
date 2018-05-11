#ifndef SET_PARAMETERS_KMEANS_H
#define SET_PARAMETERS_KMEANS_H

#include <QDialog>

namespace Ui {
class SetParameters_KMeans;
}

class SetParameters_KMeans : public QDialog
{
    Q_OBJECT

public:
    explicit SetParameters_KMeans(QWidget *parent = 0);
    ~SetParameters_KMeans();

    void SetValues(SetParameters_KMeans &kmeans, int *frame, int *clusterSize, int frameSize);

private slots:
    void on_Ok_clicked();

    void on_Cancel_clicked();

private:
    Ui::SetParameters_KMeans *ui;

    int *selectionFrame;
    int *clusterSize;
    int maxFrameSize;
};

#endif // SET_PARAMETERS_KMEANS_H
