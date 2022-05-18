#ifndef GESTIONESINGOLOMOVIMENTO_H
#define GESTIONESINGOLOMOVIMENTO_H

#include <QDialog>

#include "./backEnd/dbconnection.h"
#include "./backEnd/oggmov.h"

namespace Ui {
class gestioneSingoloMovimento;
}

class gestioneSingoloMovimento : public QDialog
{
    Q_OBJECT

public:
    explicit gestioneSingoloMovimento(QWidget *parent = nullptr);
    ~gestioneSingoloMovimento();

    virtual void init(dbConnection * db,QString stat,bool * resulScreen,oggMov * toMod=nullptr);

    virtual void daUiInObg(oggMov * obj);

    virtual void popolaViewDaOgg(oggMov * obj);

    virtual bool controlloValiditaObg(oggMov * obj);

    virtual void popolaComboCategoria();

    virtual void setDataCalenario(QDate dataTo);

    virtual void resetSommaUtility();

public slots:
    virtual void pressioneConferma();
    virtual void pressioneCalendario();
private:
    Ui::gestioneSingoloMovimento *ui;

    dbConnection * m_conn;

    QString m_stato;

    oggMov * m_mainObj;

    bool * m_resultScreen;

    double sommaUtility;
};

#endif // GESTIONESINGOLOMOVIMENTO_H
