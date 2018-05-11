#include "units_dialog.h"
#include "ui_units_dialog.h"

#include <QRadioButton>

UnitsDialog::UnitsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnitsDialog)
{
    ui->setupUi(this);
}

UnitsDialog::~UnitsDialog()
{
    delete ui;
}

void UnitsDialog::SetUnit(float* unit) {
    unitToSet = unit;
}

void UnitsDialog::on_okButton_clicked()
{
    if(ui->KM->isChecked()) {
        bufUnit = 0.001;
    } else if(ui->M->isChecked()) {
        bufUnit = 1.0;
    } else if(ui->DM->isChecked()) {
        bufUnit = 10.0;
    } else if(ui->CM->isChecked()) {
        bufUnit = 100.0;
    } else if(ui->MM->isChecked()) {
        bufUnit = 1000.0;
    } else if(ui->micro->isChecked()) {
        bufUnit = 10000.0;
    }

    *unitToSet = bufUnit;
    QDialog::close();
}

void UnitsDialog::on_cancelButton_clicked()
{
    if(bufUnit == 0.001) {
        ui->KM->setChecked(true);
    } else if(bufUnit == 1.0) {
        ui->M->setChecked(true);;
    } else if(bufUnit == 10.0) {
        ui->DM->setChecked(true);;
    } else if(bufUnit == 100.0) {
        ui->CM->setChecked(true);;
    } else if(bufUnit == 1000.0) {
        ui->MM->setChecked(true);;
    } else if(bufUnit == 10000.0) {
        ui->micro->setChecked(true);;
    }

    *unitToSet = bufUnit;
    QDialog::close();
}
