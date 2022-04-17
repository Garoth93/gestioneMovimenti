#ifndef OGGMOV_H
#define OGGMOV_H

#include <QString>
#include <QDate>

#include "./backEnd/dbconnection.h"

class oggMov
{
public:
    oggMov();

    virtual void init(dbConnection *db);

    virtual void caricaDato();
    virtual void insertDato();
    virtual void updateDato();
    virtual void deleteDato();

    virtual QDate convertDate(QString stdata);
    virtual QString convertDate(QDate qdate);
    virtual QString convertDateVideo(QDate qdate);
    virtual QDate convertDateVideo(QString stdata);

    const QString &tipoMov() const;
    void setTipoMov(const QString &newTipoMov);

    const QString &categoria() const;
    void setCategoria(const QString &newCategoria);

    const QString &descrizione() const;
    void setDescrizione(const QString &newDescrizione);

    const QDate &dataMov() const;
    void setDataMov(const QDate &newDataMov);

    double valorew() const;
    void setValorew(double newValorew);

    int idMov() const;
    void setIdMov(int newIdMov);

    const QString &getDateFormat() const;

    const QString &getDateFormatVideo() const;

private:
    QString m_tipoMov;
    QString m_categoria;
    QString m_descrizione;
    QDate   m_dataMov;
    double  m_valorew;
    int     m_idMov;

    QString dateFormat;
    QString dateFormatVideo;

    dbConnection * m_conn;
};

#endif // OGGMOV_H
