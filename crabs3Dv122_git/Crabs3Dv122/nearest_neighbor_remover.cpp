#include "nearest_neighbor_remover.h"
#include "ui_nearest_neighbor_remover.h"

#include <QDoubleSpinBox>
#include <QSpinBox>

NearestNeighborRemover::NearestNeighborRemover(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NearestNeighborRemover)
{
    ui->setupUi(this);
}

NearestNeighborRemover::~NearestNeighborRemover()
{
    delete ui;
}

void NearestNeighborRemover::SetValues(float *thresh, int *tol, bool *state) {

    threshold = thresh;
    tolerance = tol;
    runState = state;

    ui->minDistSpinBox->setValue(*thresh);
    ui->toreranceSpin->setValue(*tol);
}

void NearestNeighborRemover::on_runButton_clicked()
{
    *threshold = ui->minDistSpinBox->value();
    *tolerance = ui->toreranceSpin->value();
    *runState = true;
    QDialog::close();
}

void NearestNeighborRemover::on_cancelButton_clicked()
{
    QDialog::close();
}
