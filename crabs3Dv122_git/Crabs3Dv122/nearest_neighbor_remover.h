#ifndef NEAREST_NEIGHBOR_REMOVER_H
#define NEAREST_NEIGHBOR_REMOVER_H

#include <QDialog>

namespace Ui {
class NearestNeighborRemover;
}

class NearestNeighborRemover : public QDialog
{
    Q_OBJECT

public:
    explicit NearestNeighborRemover(QWidget *parent = 0);
    ~NearestNeighborRemover();

    void SetValues(float *thresh, int *tol, bool *state);

private slots:
    void on_runButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::NearestNeighborRemover *ui;

    float* threshold;
    int* tolerance;
    bool* runState;

};

#endif // NEAREST_NEIGHBOR_REMOVER_H
