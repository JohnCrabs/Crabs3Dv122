#ifndef UNITS_DIALOG_H
#define UNITS_DIALOG_H

#include <QDialog>

namespace Ui {
class UnitsDialog;
}

class UnitsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnitsDialog(QWidget *parent = 0);
    ~UnitsDialog();

    void SetUnit(float* unit);

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::UnitsDialog *ui;
    float bufUnit;
    float* unitToSet;
};

#endif // UNITS_DIALOG_H
