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
        popolaComboTemi();

        ui->de_s1->setDate(QDate::currentDate());
        ui->de_s2->setDate(QDate::currentDate());
        ui->r_tutti->setChecked(true);

        dbConnection * dbog=new dbConnection();
        dbog->init(QString("%1/gestContiApp").arg(pathFileConf),QString("%1.db").arg(nomeContoDaA));
        mainScreen= new screeMovimenti(this);
        mainScreen->init(dbog);

        /*setto le date per aggiornare grafici con ora*/
        impostaLeDateAlDettaglioSingolo();

        /*carico i grafici*/
        mainScreen->resetGraficiPeriodo();
        ui->gl_tab1->addWidget(mainScreen);

        /*setto i grafici nel tab due*/
        ui->grid_grafici->addWidget(mainScreen->graTotEU(),2,0);
        ui->grid_grafici->addWidget(mainScreen->graTotCatU(),2,1);
        ui->grid_grafici->addWidget(mainScreen->graTotCatE(),2,2);

        ui->grid_grafici->addWidget(mainScreen->graPerEU(),1,0);
        ui->grid_grafici->addWidget(mainScreen->graPerCatU(),1,1);
        ui->grid_grafici->addWidget(mainScreen->graPerCatE(),1,2);

        /*metto tutti i grafici nella lista per fare operazioni poi*/
        l_listaGrafici.push_back(mainScreen->graTotEU());
        l_listaGrafici.push_back(mainScreen->graTotCatU());
        l_listaGrafici.push_back(mainScreen->graTotCatE());
        l_listaGrafici.push_back(mainScreen->graPerEU());
        l_listaGrafici.push_back(mainScreen->graPerCatU());
        l_listaGrafici.push_back(mainScreen->graPerCatE());

        ui->tb_cercaF->setIcon(QIcon((":/cerca.png")));
        ui->tb_cercaF->setToolTip("Cerca periodo");
        connect(ui->tb_cercaF, &QToolButton::clicked, this, [=]()
        {
            /*prendo date da ui per settarle ad altra ui bruttttto*/
            impostaLeDateAlDettaglioSingolo();
            mainScreen->resetGraficiPeriodo();
        });

        /*radio*/
        connect(ui->r_tutti, &QRadioButton::toggled, this, [=]()
        {
            cambioRadio();
        });
        connect(ui->r_periodo, &QRadioButton::toggled, this, [=]()
        {
            cambioRadio();
        });
        connect(ui->r_totale, &QRadioButton::toggled, this, [=]()
        {
            cambioRadio();
        });

        /*setto periodo a mese corrente*/
        ui->tb_setMese->setIcon(QIcon((":/calendario.ico")));
        ui->tb_setMese->setToolTip("Setta data periodo mese corrente");
        connect(ui->tb_setMese, &QToolButton::clicked, this, [=]()
        {
            QDate today=QDate::currentDate();
            ui->de_s1->setDate(QDate(today.year(),today.month(),1));
            ui->de_s2->setDate(QDate(today.month()==12?today.year()+1:today.year(),today.month()==12?1:today.month()+1,1));
            impostaLeDateAlDettaglioSingolo();
            mainScreen->resetGraficiPeriodo();
        });

        /*combo temi*/
        connect(ui->cb_temi, &QComboBox::currentIndexChanged, this, [=]()
        {
            comboTemiCambiata();
        });

        /*chek box legenda*/
        ui->cb_legenda->setChecked(true);
        connect(ui->cb_legenda, &QCheckBox::clicked, this, [=]()
        {
            CheckLegendaCambiata();
        });

        /*btn zoom grafici*/
        ui->tb_setDimMenGra->setIcon(QIcon((":/zoomMinus.png")));
        ui->tb_setDimMenGra->setToolTip("Zoom - grafici");
        connect(ui->tb_setDimMenGra, &QToolButton::clicked, this, [=]()
        {
            setZoomGrafici("-");
        });

        /*btn zoom grafici*/
        ui->tb_setDimPiuGra->setIcon(QIcon((":/zoomPlus.png")));
        ui->tb_setDimPiuGra->setToolTip("Zoom + grafici");
        connect(ui->tb_setDimPiuGra, &QToolButton::clicked, this, [=]()
        {
            setZoomGrafici("+");
        });

        /* btn add settimana filri*/
        ui->tb_setPiuData->setToolTip("Più una settimana");
        ui->tb_setPiuData->setIcon(QIcon((":/zoomPlus.png")));
        connect(ui->tb_setPiuData, &QToolButton::clicked, this, [=]()
        {
            ui->de_s1->setDate((ui->de_s1->date().addDays(7)));
            ui->de_s2->setDate((ui->de_s2->date().addDays(7)));
            mainScreen->resetGraficiPeriodo();
        });

        /* btn add settimana filri*/
        ui->tb_setMenoData->setToolTip("Meno una settimana");
        ui->tb_setMenoData->setIcon(QIcon((":/zoomMinus.png")));
        connect(ui->tb_setMenoData, &QToolButton::clicked, this, [=]()
        {
            ui->de_s1->setDate((ui->de_s1->date().addDays(-7)));
            ui->de_s2->setDate((ui->de_s2->date().addDays(-7)));
            impostaLeDateAlDettaglioSingolo();
            mainScreen->resetGraficiPeriodo();
        });

        comboTemiCambiata();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::cambioRadio()
{
    if(ui->r_tutti->isChecked())
    {
        mainScreen->graPerCatE()->setVisible(true);
        mainScreen->graPerCatU()->setVisible(true);
        mainScreen->graPerEU()->setVisible(true);
        mainScreen->graTotCatE()->setVisible(true);
        mainScreen->graTotCatU()->setVisible(true);
        mainScreen->graTotEU()->setVisible(true);
    }
    else if(ui->r_periodo->isChecked())
    {
        mainScreen->graPerCatE()->setVisible(true);
        mainScreen->graPerCatU()->setVisible(true);
        mainScreen->graPerEU()->setVisible(true);
        mainScreen->graTotCatE()->setVisible(false);
        mainScreen->graTotCatU()->setVisible(false);
        mainScreen->graTotEU()->setVisible(false);
    }
    else if(ui->r_totale->isChecked())
    {
        mainScreen->graPerCatE()->setVisible(false);
        mainScreen->graPerCatU()->setVisible(false);
        mainScreen->graPerEU()->setVisible(false);
        mainScreen->graTotCatE()->setVisible(true);
        mainScreen->graTotCatU()->setVisible(true);
        mainScreen->graTotEU()->setVisible(true);
    }
}

void MainWindow::popolaComboTemi()
{
    ui->cb_temi->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    ui->cb_temi->addItem("Light", QChart::ChartThemeLight);
    ui->cb_temi->addItem("Dark", QChart::ChartThemeDark);
    ui->cb_temi->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    ui->cb_temi->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    ui->cb_temi->addItem("High Contrast", QChart::ChartThemeHighContrast);
    ui->cb_temi->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    ui->cb_temi->addItem("Qt", QChart::ChartThemeQt);
}

void MainWindow::comboTemiCambiata()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                ui->cb_temi->itemData(ui->cb_temi->currentIndex()).toInt());
    foreach(creaGrafico* ggg,l_listaGrafici)
    {
        ggg->setTemaGrafico(theme);
    }
}

void MainWindow::CheckLegendaCambiata()
{
    bool toDo=false;
    if(ui->cb_legenda->isChecked())
        toDo=true;
    foreach(creaGrafico* ggg,l_listaGrafici)
    {
        ggg->setLegenda(toDo);
    }
}

void MainWindow::setZoomGrafici(QString oper)
{
    foreach(creaGrafico* ggg,l_listaGrafici)
    {
        ggg->setDimensioneGrafici(oper);
    }
}

void MainWindow::impostaLeDateAlDettaglioSingolo()
{
    QDate d1=ui->de_s1->date();
    QDate d2=ui->de_s2->date();
    mainScreen->setDataCercaInizio(d1);
    mainScreen->setDataCercaFine(d2);
}

