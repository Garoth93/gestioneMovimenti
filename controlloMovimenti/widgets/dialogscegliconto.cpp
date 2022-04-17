#include "dialogscegliconto.h"
#include "ui_dialogscegliconto.h"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QDir>
#include <QMessageBox>

dialogScegliConto::dialogScegliConto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogScegliConto)
{
    ui->setupUi(this);
}

dialogScegliConto::~dialogScegliConto()
{
    delete ui;
}

void dialogScegliConto::init(QString pathJ, QString nomeJ,QString *nomeContoToOpen)
{
    this->setWindowTitle("Scegli/Elimina/Aggiungi conto");
    m_model=new QStandardItemModel();
    m_tableView=ui->tv_main;
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setModel(m_model);

    m_pathJson=QString("%1/gestContiApp").arg(pathJ);
    m_nomeJson=nomeJ;
    nomeDelContoDaAprire=nomeContoToOpen;

    /*non esiste cartella app la creo*/
    if(!QDir(m_pathJson).exists())
    {
        QDir().mkdir(m_pathJson);
    }

    ui->tb_aggiungi->setIcon(QIcon((":/aggiungi.png")));
    ui->tb_aggiungi->setToolTip("Aggiungi nuova riga conto");
    connect(ui->tb_aggiungi, &QToolButton::clicked, this, [=](){
        pressionNuovaRiga();
    });

    ui->tb_salva->setIcon(QIcon((":/save.png")));
    ui->tb_salva->setToolTip("Salva le righe correnti");
    connect(ui->tb_salva, &QToolButton::clicked, this, [=](){
        pressioneSalva();
    });

    ui->tb_elimina->setIcon(QIcon((":/elimina.png")));
    ui->tb_elimina->setToolTip("Elimina la riga selezionata");
    connect(ui->tb_elimina, &QToolButton::clicked, this, [=](){
        pressioneEliminaRiga();
    });

    ui->pb_apriConto->setToolTip("Seleziona il conto da aprire e gestire");
    connect(ui->pb_apriConto, &QPushButton::clicked, this, [=](){
        pressioneApriConto();
    });

    resetTabelView();
}

void dialogScegliConto::loadDataFromJson()
{
    QFile m_fileJ;
    m_fileJ.setFileName(QString("%1/%2").arg(m_pathJson).arg(m_nomeJson));
    if( !m_fileJ.open( QIODevice::ReadWrite ) )
    {
        //read json content
        qDebug() << "Errore apertura json";
        return;
    }
    QJsonDocument jsonOrg = QJsonDocument::fromJson(m_fileJ.readAll());
    m_fileJ.close();
    QJsonArray jsonArray = jsonOrg.array();
    for (int i=0; i < jsonArray.size(); i++)
    {
        QJsonValue temp    = jsonArray.at(i).toObject().value("conto");
        QString nomeConto  = temp.toObject().value("nomeConto").toString();
        QString descConto  = temp.toObject().value("descConto").toString();
        m_listaVociJson.push_back(QStringList() << nomeConto << descConto);
    }
}

void dialogScegliConto::saveDataInJson()
{
    daModelloALista();
    QFile m_fileJ;
    /*creo il file da zero, nuovo*/
    m_fileJ.setFileName(QString("%1/%2").arg(m_pathJson).arg(m_nomeJson));
    m_fileJ.open( QIODevice::WriteOnly );
    m_fileJ.close();
    QList<QStringList>::iterator it=m_listaVociJson.begin();
    for(;it!=m_listaVociJson.end();++it)
    {
        if( !m_fileJ.open( QIODevice::ReadOnly ) )
        {
            //read json content
            qDebug() << "Errore apertura json";
            return;
        }
        QJsonDocument jsonOrg = QJsonDocument::fromJson( m_fileJ.readAll() );
        QByteArray jsonFileData = m_fileJ.readAll();
        m_fileJ.close();
        QJsonObject projectDetails = { {"nomeConto", (*it)[0]},
                                       {"descConto", (*it)[1]}
                                     };
        QJsonObject notificationObj =  {{ "conto", projectDetails }};
        QJsonArray arrLog = jsonOrg.array();
        arrLog.push_back( notificationObj );

        QJsonDocument doc( arrLog );

        if( !m_fileJ.open( QIODevice::WriteOnly ) ) //write json content to file.
        {
            //cannot open for write ...
            return;
        }
        m_fileJ.write(doc.toJson());
        m_fileJ.close();
    }
}

void dialogScegliConto::popolaModello()
{
    m_model->clear();
    m_model->setHorizontalHeaderLabels(QStringList() << "NOME" << "DESCRIZIONE");
    QList<QStringList>::iterator it=m_listaVociJson.begin();
    for(;it!=m_listaVociJson.end();++it)
    {
        QList<QStandardItem*> ll;
        ll.push_back(new QStandardItem((*it)[0]));
        ll.push_back(new QStandardItem((*it)[1]));
        m_model->appendRow(ll);
    }
    m_tableView->resizeColumnsToContents();
}

void dialogScegliConto::daModelloALista()
{
    int righeTotali=0;
    righeTotali=m_model->rowCount();
    m_listaVociJson.clear();
    for(int x=0;x<righeTotali;x++)
    {
        QString nomeToIns=m_model->index(x,0).data().toString();
        QString descToIns=m_model->index(x,1).data().toString();
        m_listaVociJson.push_back(QStringList() << nomeToIns << descToIns);
    }
}

void dialogScegliConto::resetTabelView()
{
    loadDataFromJson();
    popolaModello();
}

void dialogScegliConto::pressionNuovaRiga()
{
    QList<QStandardItem*> ll;
    ll.push_back(new QStandardItem(""));
    ll.push_back(new QStandardItem(""));
    m_model->appendRow(ll);
    m_tableView->resizeColumnsToContents();
}

void dialogScegliConto::pressioneSalva()
{
    saveDataInJson();
}

void dialogScegliConto::pressioneEliminaRiga()
{
    if(!m_tableView->selectionModel()->currentIndex().isValid())return;
    int rigaSel=m_tableView->selectionModel()->currentIndex().row();
    QString dettaglioDel=m_model->index(rigaSel,0).data().toString();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Richiesta conferma", QString("Vuoi eliminare il conto %1?").arg(dettaglioDel),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
           m_model->removeRow(rigaSel);
    }
}

void dialogScegliConto::pressioneApriConto()
{
    if(!m_tableView->selectionModel()->currentIndex().isValid())return;
    int rigaSel=m_tableView->selectionModel()->currentIndex().row();
    QString dettaglioDel=m_model->index(rigaSel,0).data().toString();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Richiesta conferma", QString("Vuoi aprire il conto %1?").arg(dettaglioDel),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        *nomeDelContoDaAprire=dettaglioDel;
        this->close();
    }
}
