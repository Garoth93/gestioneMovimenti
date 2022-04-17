#include "dialoggestionecategoria.h"
#include "ui_dialoggestionecategoria.h"

#include <QDebug>
#include <QMessageBox>

dialoggestionecategoria::dialoggestionecategoria(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialoggestionecategoria)
{
    ui->setupUi(this);
}

dialoggestionecategoria::~dialoggestionecategoria()
{
    delete ui;
}

void dialoggestionecategoria::init(dbConnection *db, QString stat, bool *resulScreen)
{
    m_conn=db;
    m_stato=stat;
    m_resultScreen=resulScreen;

    ui->rb_ins->setChecked(true);

    this->setWindowTitle("GESTIONE CATEGORIE");

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

    /*radio*/
    connect(ui->rb_ins, &QRadioButton::toggled, this, [=]()
    {
        m_stato="I";
        sistemaViewStato();
    });

    /*radio*/
    connect(ui->rb_del, &QRadioButton::toggled, this, [=]()
    {
        m_stato="D";
        sistemaViewStato();
    });

    sistemaViewStato();
    popolaComboCategoria();
}

void dialoggestionecategoria::sistemaViewStato()
{
    if(m_stato=="D")
    {
        ui->l_elimina->setVisible(true);
        ui->cb_elimina->setVisible(true);
        ui->l_inserimento->setVisible(false);
        ui->le_descCat->setVisible(false);
    }
    else if(m_stato=="I")
    {
        ui->l_elimina->setVisible(false);
        ui->cb_elimina->setVisible(false);
        ui->l_inserimento->setVisible(true);
        ui->le_descCat->setVisible(true);
    }
}

bool dialoggestionecategoria::controlloSeOccupataCategoria(QString cate)
{
    QString qryControllo=QString("select count(*) from movimenti where catem='%1'").arg(cate);
    QSqlQuery qr(qryControllo,*(m_conn->db()));
    qDebug() << qr.lastError();
    if(qr.next())
    {
        int valorr=qr.value(0).toInt();
        if(valorr>0)return true;
        return false;
    }
    return false;
}

void dialoggestionecategoria::popolaComboCategoria()
{
    QStringList listaToIns;
    QString stringSel;
    stringSel=QString("select cdtab from tabellaGenerica where tptab='categorie'");
    QSqlQuery query(stringSel,*(m_conn->db()));
    while(query.next())
    {
        qDebug() << query.value("cdtab").toString();
        listaToIns << query.value("cdtab").toString();
    }
    ui->cb_elimina->clear();
    ui->cb_elimina->addItems(listaToIns);
}

void dialoggestionecategoria::pressioneConferma()
{
    if(m_stato=="I")
    {
        QString valDesc=ui->le_descCat->text();
        if(valDesc=="")
        {
            QMessageBox msgBox;
            msgBox.setText("Inserire valore per la categoria.");
            msgBox.exec();
            return;
        }
        QString qryControllo=QString("select count(*) from tabellaGenerica where cdtab='%1' and tptab='categorie'").arg(valDesc);
        QSqlQuery qr(qryControllo,*(m_conn->db()));
        if(qr.next())
        {
            int valorr=qr.value(0).toInt();
            if(valorr>0)
            {
                QMessageBox msgBox;
                msgBox.setText("La categoria risulta presente.");
                msgBox.exec();
                return;
            }
        }
        QString qryIns=QString("insert into tabellaGenerica "
                               "(tptab,cdtab) "
                               "values ('categorie','%1')").arg(valDesc);
        QSqlQuery qr2(qryIns,*(m_conn->db()));
        *m_resultScreen=true;
        this->close();
    }
    if(m_stato=="D")
    {
        QString valDesc=ui->cb_elimina->currentText();
        if(valDesc=="")return;
        if(controlloSeOccupataCategoria(valDesc))
        {
            QMessageBox msgBox;
            msgBox.setText("La categoria risulta usata in alcuni documenti.");
            msgBox.exec();
            return;
        }
        QString qryIns=QString("delete from tabellaGenerica "
                               "where tptab='categorie' and cdtab='%1'").arg(valDesc);
        QSqlQuery qr(qryIns,*(m_conn->db()));
        qDebug() << qr.lastError();
        *m_resultScreen=true;
        this->close();
    }
}
