#include "accumulation_noise_remover.h"
#include "ui_accumulation_noise_remover.h"

#include <QDoubleSpinBox>

AccumulationNoiseRemover::AccumulationNoiseRemover(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccumulationNoiseRemover)
{
    ui->setupUi(this);
}

AccumulationNoiseRemover::~AccumulationNoiseRemover()
{
    delete ui;
}

void AccumulationNoiseRemover::SetValues(float* thresh, float* tol, bool* runState) {

    threshold = thresh;
    tolerance = tol;
    run_state = runState;

    ui->minimumThreshold->setValue(*thresh);
    ui->tolerance->setValue(*tol);
}


void AccumulationNoiseRemover::on_run_clicked()
{
    *threshold = ui->minimumThreshold->value();
    *tolerance = ui->tolerance->value();
    *run_state = true;
    QDialog::close();
}

void AccumulationNoiseRemover::on_cancel_clicked()
{
    QDialog::close();
}
