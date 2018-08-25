#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_apply_clicked()
{


    //if the input is empty,show a warn message
    if(ui->nameInput->text().isEmpty()){

         QMessageBox::information(this, QString::fromLocal8Bit("warn"),QString::fromLocal8Bit("name can not be empty!"));

       }
    else{
        this->close();
        applySuccessful();


    }




}
  QString Dialog::applySuccessful()
  {
      QString name;
      name=ui->nameInput->text();
      return name;
  }

