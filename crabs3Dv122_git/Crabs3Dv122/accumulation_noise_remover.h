#ifndef ACCUMULATION_NOISE_REMOVER_H
#define ACCUMULATION_NOISE_REMOVER_H

#include <QDialog>

namespace Ui {
class AccumulationNoiseRemover;
}

class AccumulationNoiseRemover : public QDialog
{
    Q_OBJECT

public:
    explicit AccumulationNoiseRemover(QWidget *parent = 0);
    ~AccumulationNoiseRemover();

    void SetValues(float* thresh, float* tol, bool *runState);

private slots:
    void on_run_clicked();

    void on_cancel_clicked();

private:
    Ui::AccumulationNoiseRemover *ui;
    float* threshold;
    float* tolerance;
    bool* run_state;
};

#endif // ACCUMULATION_NOISE_REMOVER_H
