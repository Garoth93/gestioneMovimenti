#include "oggmov.h"
#include <QDebug>
#include <QVariant>

oggMov::oggMov()
{

}

void oggMov::init(dbConnection *db)
{
    m_conn=db;

    dateFormat="yyyyMMdd";
    dateFormatVideo="dd-MM-yyyy";
}

void oggMov::caricaDato()
{
    QString stringaQry=QString("select * from movimenti where idmov=%1").arg(m_idMov);
    QSqlQuery qr(stringaQry,*(m_conn->db()));
    qDebug() << stringaQry;
    while(qr.next())
    {
        m_tipoMov=qr.value("tipom").toString();
        m_categoria=qr.value("catem").toString();
        m_descrizione=qr.value("descr").toString();
        m_dataMov=convertDate(qr.value("datam").toString());
        m_valorew=qr.value("valor").toDouble();
    }
}

void oggMov::insertDato()
{
    QString stringaQry=QString("insert into movimenti "
                               "(datam,descr,valor,tipom,catem) "
                               "values ('%1','%2',%3,'%4','%5')")
            .arg(m_dataMov.toString(dateFormat))
            .arg(m_descrizione)
            .arg(m_valorew)
            .arg(m_tipoMov)
            .arg(m_categoria);
    qDebug() << stringaQry;
    QSqlQuery qr(stringaQry,*(m_conn->db()));
    qDebug() << qr.lastError();
}

void oggMov::updateDato()
{
    QString stringaQry=QString("update movimenti "
                               "set "
                               "datam='%1',"
                               "descr='%2',"
                               "valor=%3,"
                               "tipom='%4',"
                               "catem='%5' "
                               "where idmov=%6")
            .arg(convertDate(m_dataMov))
            .arg(m_descrizione)
            .arg(m_valorew)
            .arg(m_tipoMov)
            .arg(m_categoria)
            .arg(m_idMov);
    qDebug() << stringaQry;
    QSqlQuery qr(stringaQry,*(m_conn->db()));
}

void oggMov::deleteDato()
{
    QString stringaQry=QString("delete from movimenti where idmov=%1").arg(m_idMov);
    QSqlQuery qr(stringaQry,*(m_conn->db()));
}

QDate oggMov::convertDate(QString stdata)
{
    int y=stdata.left(4).toInt();
    int m=(stdata.left(6)).right(2).toInt();
    int d=stdata.right(2).toInt();
    QDate dat(y,m,d);
    return dat;
}

QString oggMov::convertDate(QDate qdate)
{
    return qdate.toString(dateFormat);
}

QString oggMov::convertDateVideo(QDate qdate)
{
    return qdate.toString(dateFormatVideo);
}

QDate oggMov::convertDateVideo(QString stdata)
{
    int y=stdata.right(4).toInt();
    int m=(stdata.left(5)).right(2).toInt();
    int d=stdata.left(2).toInt();
    QDate dat(y,m,d);
    return dat;
}

const QString &oggMov::tipoMov() const
{
    return m_tipoMov;
}

void oggMov::setTipoMov(const QString &newTipoMov)
{
    m_tipoMov = newTipoMov;
}

const QString &oggMov::categoria() const
{
    return m_categoria;
}

void oggMov::setCategoria(const QString &newCategoria)
{
    m_categoria = newCategoria;
}

const QString &oggMov::descrizione() const
{
    return m_descrizione;
}

void oggMov::setDescrizione(const QString &newDescrizione)
{
    m_descrizione = newDescrizione;
}

const QDate &oggMov::dataMov() const
{
    return m_dataMov;
}

void oggMov::setDataMov(const QDate &newDataMov)
{
    m_dataMov = newDataMov;
}

double oggMov::valorew() const
{
    return m_valorew;
}

void oggMov::setValorew(double newValorew)
{
    m_valorew = newValorew;
}

int oggMov::idMov() const
{
    return m_idMov;
}

void oggMov::setIdMov(int newIdMov)
{
    m_idMov = newIdMov;
}

const QString &oggMov::getDateFormat() const
{
    return dateFormat;
}

const QString &oggMov::getDateFormatVideo() const
{
    return dateFormatVideo;
}
