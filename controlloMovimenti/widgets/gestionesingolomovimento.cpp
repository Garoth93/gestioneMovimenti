#include "gestionesingolomovimento.h"
#include "ui_gestionesingolomovimento.h"

#include <QPushButton>
#include <QCalendarWidget>
#include <QDebug>
#include <QMessageBox>

gestioneSingoloMovimento::gestioneSingoloMovimento(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gestioneSingoloMovimento)
{
    ui->setupUi(this);
}

gestioneSingoloMovimento::~gestioneSingoloMovimento()
{
    delete ui;
}

void gestioneSingoloMovimento::init(dbConnection *db,QString stat,bool * resulScreen,oggMov * toMod)
{
    m_conn=db;
    m_stato=stat;
    m_resultScreen=resulScreen;

    popolaComboCategoria();

    QDoubleValidator* validFirstX = new QDoubleValidator(-100000.9999,+100000.9999,4, this);
    validFirstX->setNotation(QDoubleValidator::StandardNotation);
    ui->le_valor->setValidator(validFirstX);

    if(m_stato=="I")
    {
        m_mainObj=new oggMov();
        m_mainObj->init(m_conn);
        pressioneCalendario();
        this->setWindowTitle("INSERIMENTO MOVIMENTO");
    }
    else if(m_stato=="M")
    {
        m_mainObj=toMod;
        popolaViewDaOgg(m_mainObj);
        this->setWindowTitle(QString("MODIFICA MOVIMENTO %1").arg(m_mainObj->idMov()));
    }

    /*btn conferma*/
    connect(ui->pb_conferma, &QPushButton::clicked, this, [=]()
    {
        pressioneConferma();
    });

    /*btn annulla*/
    connect(ui->pb_annulla, &QPushButton::clicked, this, [=]()
    {
        *m_resultScreen=false;
        this->close();
    });

    /*pressione data calendario*/
    connect(ui->cw_main, &QCalendarWidget::clicked, this, [=]()
    {
        pressioneCalendario();
    });

}

void gestioneSingoloMovimento::daUiInObg(oggMov * obj)
{
    QString cateToIns=ui->cb_categoria->currentText();
    QString descToIns=ui->le_desc->toPlainText();
    QString tipoMov=ui->cb_tipoMov->currentText();
    double valoreToIns=ui->le_valor->text().replace(",", ".").toDouble();
    qDebug() << ui->le_valor->text().replace(",", ".").toDouble() << valoreToIns;
    if(tipoMov=="Uscita")
    {
        if(valoreToIns>0)
            valoreToIns=valoreToIns*-1;
    }
    else
    {
        if(valoreToIns<0)
            valoreToIns=valoreToIns*-1;
    }

    obj->setCategoria(cateToIns);
    obj->setDataMov(ui->cw_main->selectedDate());
    obj->setDescrizione(descToIns);
    obj->setValorew(valoreToIns);
    obj->setTipoMov(tipoMov);
}

void gestioneSingoloMovimento::popolaViewDaOgg(oggMov *obj)
{
    ui->cb_categoria->setCurrentText(obj->categoria());
    ui->le_desc->setText(obj->descrizione());
    ui->cb_tipoMov->setCurrentText(obj->tipoMov());
    QString valorStringToIns=QString("%1").arg(obj->valorew());
    valorStringToIns.replace(".",",");
    ui->le_valor->setText(valorStringToIns);
    QDate daInsView=obj->dataMov();
    ui->cw_main->setSelectedDate(daInsView);
    pressioneCalendario();
}

bool gestioneSingoloMovimento::controlloValiditaObg(oggMov *obj)
{
    QMessageBox msWar;
    if(obj->categoria()=="")
    {
        msWar.setText("Categoria vuota.");
        msWar.exec();
        return false;
    }
    if(obj->descrizione()=="")
    {
        msWar.setText("Descrizione vuota.");
        msWar.exec();
        return false;
    }
    if(obj->tipoMov()=="")
    {
        msWar.setText("Tipo movimento vuota.");
        msWar.exec();
        return false;
    }
    return true;
}

void gestioneSingoloMovimento::popolaComboCategoria()
{
    QStringList listaToIns;
    QString stringSel;
    stringSel=QString("select cdtab from tabellaGenerica where tptab='categorie'");
    QSqlQuery query(stringSel,*(m_conn->db()));
    while(query.next())
    {
        listaToIns << query.value("cdtab").toString();
    }
    ui->cb_categoria->clear();
    ui->cb_categoria->addItems(listaToIns);
}

void gestioneSingoloMovimento::pressioneConferma()
{
    daUiInObg(m_mainObj);
    if(!controlloValiditaObg(m_mainObj))return;
    if(m_stato=="I")
    {
        m_mainObj->insertDato();
        *m_resultScreen=true;
    }
    if(m_stato=="M")
    {
        m_mainObj->updateDato();
        *m_resultScreen=true;
    }
    this->close();
}

void gestioneSingoloMovimento::pressioneCalendario()
{
    QDate press=ui->cw_main->selectedDate();
    ui->le_data->setText(m_mainObj->convertDateVideo(press));
}

void gestioneSingoloMovimento::setDataCalenario(QDate dataTo)
{
    ui->cw_main->setSelectedDate(dataTo);
    pressioneCalendario();
}
