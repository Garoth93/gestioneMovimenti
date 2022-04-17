#ifndef SCEGLICONTO_H
#define SCEGLICONTO_H

#include <QString>
#include <QList>

class scegliConto
{
public:
    scegliConto();

    virtual void init(QString pathJ,QString nomeJ);
    virtual void loadDataFromJson();
    virtual void saveDataInJson();

private:
    QString m_pathJson;
    QString m_nomeJson;
    QList<QStringList> m_listaVociJson;
};

#endif // SCEGLICONTO_H
