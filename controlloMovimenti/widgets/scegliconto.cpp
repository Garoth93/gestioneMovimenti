#include "scegliconto.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

scegliConto::scegliConto()
{

}

void scegliConto::init(QString pathJ,QString nomeJ)
{
    m_pathJson=pathJ;
    m_nomeJson=nomeJ;
}

void scegliConto::loadDataFromJson()
{
    QFile m_fileJ;
    m_fileJ.setFileName(QString("%1/%2").arg(m_pathJson).arg(m_nomeJson));
    if( !m_fileJ.open( QIODevice::ReadWrite ) ) //read json content
        qDebug() << "Errore apertura json";
    QJsonDocument jsonOrg = QJsonDocument::fromJson(m_fileJ.readAll());
    m_fileJ.close();
    QJsonArray jsonArray = jsonOrg.array();
    for (int i=0; i < jsonArray.size(); i++)
    {
        QJsonValue temp    = jsonArray.at(i).toObject().value("contto");
        QString nomeConto  = temp.toObject().value("nomeConto").toString();
        QString descConto  = temp.toObject().value("descConto").toString();
        m_listaVociJson.push_back(QStringList() << nomeConto << descConto);
    }
}
