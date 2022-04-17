#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "./widgets/dialogscegliconto.h"
#include "./widgets/screemovimenti.h"

#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString pathFileConf=QDir::homePath();
    qDebug() << pathFileConf;
    dialogScegliConto scegliConto;
    QString nomeContoDaA="";
    scegliConto.init(pathFileConf,"jsonConfGestContiApp.json",&nomeContoDaA);
    scegliConto.exec();
    qDebug() << nomeContoDaA;

    this->setWindowTitle("GESTIONE MOVIMENTI");
    this->setMinimumHeight(850);

    if(nomeContoDaA=="")
    {
        exit(0);
    }
    else
    {
        dbConnection * dbog=new dbConnection();
        dbog->init(QString("%1/gestContiApp").arg(pathFileConf),QString("%1.db").arg(nomeContoDaA));
        mainScreen= new screeMovimenti(this);
        mainScreen->init(dbog);
        ui->gl_tab1->addWidget(mainScreen);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

