#include "dialogseparatore.h"
#include "ui_dialogseparatore.h"

dialogSeparatore::dialogSeparatore(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogSeparatore)
{
    ui->setupUi(this);
}

dialogSeparatore::~dialogSeparatore()
{
    delete ui;
}

void dialogSeparatore::init(QString *separatore)
{
    this->setWindowTitle("Separatore");
    connect(ui->pushButton, &QPushButton::clicked, this, [=]()
    {
        (*separatore)=";";
        if(ui->r_virgola->isChecked())
            (*separatore)=",";
        else if(ui->r_puntoVirgola->isChecked())
            (*separatore)=";";
        this->close();
    });
}
