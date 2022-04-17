#ifndef DIALOGGESTIONECATEGORIA_H
#define DIALOGGESTIONECATEGORIA_H

#include <QDialog>
#include "./backEnd/dbconnection.h"

namespace Ui {
class dialoggestionecategoria;
}

class dialoggestionecategoria : public QDialog
{
    Q_OBJECT

public:
    explicit dialoggestionecategoria(QWidget *parent = nullptr);
    ~dialoggestionecategoria();

    virtual void init(dbConnection * db,QString stat,bool * resulScreen);

    virtual void sistemaViewStato();

    virtual bool controlloSeOccupataCategoria(QString cate);

    virtual void popolaComboCategoria();

public slots:
    virtual void pressioneConferma();

private:
    Ui::dialoggestionecategoria *ui;

    dbConnection * m_conn;

    QString m_stato;

    bool * m_resultScreen;
};

#endif // DIALOGGESTIONECATEGORIA_H
