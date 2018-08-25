#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString applySuccessful();




public slots:
    void on_apply_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
