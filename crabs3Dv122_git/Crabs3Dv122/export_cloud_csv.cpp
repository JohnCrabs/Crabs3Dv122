#include "export_cloud_csv.h"
#include "ui_export_cloud_csv.h"

#include <QSpinBox>
#include <QRadioButton>

ExportCloudCSV::ExportCloudCSV(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportCloudCSV)
{
    ui->setupUi(this);
}

ExportCloudCSV::~ExportCloudCSV()
{
    delete ui;
}

void ExportCloudCSV::SetValue(int* fromFrame_, int* toFrame_, char* seperator_) {
    fromFrame = fromFrame_;
    toFrame = toFrame_;
    seperator = seperator_;

    ui->fromFrame->setValue(*fromFrame_);
    ui->toFrame->setValue(*toFrame_);

    if(*seperator_ == ',') {
        ui->comma->setChecked(true);
    } else if(*seperator_ == ' ') {
        ui->space->setChecked(true);
    } else if(*seperator_ == ';') {
        ui->semicolon->setChecked(true);
    }

}

void ExportCloudCSV::on_Ok_clicked()
{
    *fromFrame = ui->fromFrame->value();
    *toFrame = ui->toFrame->value();

    if(ui->comma->isChecked()) {
        *seperator = ',';
    } else if(ui->space->isChecked()) {
        *seperator = ' ';
    } else if(ui->semicolon->isChecked()) {
        *seperator = ';';
    }

    QDialog::close();
}

void ExportCloudCSV::on_Cancel_clicked()
{
    QDialog::close();
}


