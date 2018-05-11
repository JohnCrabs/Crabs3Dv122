#ifndef EXPORT_CLOUD_CSV_H
#define EXPORT_CLOUD_CSV_H

#include <QDialog>

namespace Ui {
class ExportCloudCSV;
}

class ExportCloudCSV : public QDialog
{
    Q_OBJECT

public:
    explicit ExportCloudCSV(QWidget *parent = 0);
    ~ExportCloudCSV();

    void SetValue(int* fromFrame_, int* toFrame_, char* seperator_);

private slots:
    void on_Ok_clicked();

    void on_Cancel_clicked();

private:
    Ui::ExportCloudCSV *ui;

    int* fromFrame;
    int* toFrame;
    char* seperator;

};

#endif // EXPORT_CLOUD_CSV_H
