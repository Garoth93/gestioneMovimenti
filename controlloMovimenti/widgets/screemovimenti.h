#ifndef SCREEMOVIMENTI_H
#define SCREEMOVIMENTI_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QSortFilterProxyModel>

#include "./backEnd/dbconnection.h"
#include "./backEnd/oggmov.h"
#include "./widgets/creagrafico.h"

enum colModelloMovimenti
{
    cmm_idmov = 0,
    cmm_datam = 1,
    cmm_descr = 4,
    cmm_tipom = 2,
    cmm_valor = 5,
    cmm_catem = 3,
    MAXCOLUMMOV=cmm_valor
};

static QList<int> colonneModelloMovimenti;

namespace Ui {
class screeMovimenti;
}

class screeMovimenti : public QWidget
{
    Q_OBJECT

public:
    explicit screeMovimenti(QWidget *parent = nullptr);
    ~screeMovimenti();

    virtual void init(dbConnection * oggDb);

    virtual void creaModello();
    virtual QString creaQrySelect(QString perSommaQr="");
    virtual void popolaModello();
    virtual void ricaricaModello();

    virtual void popolaComboCategoria();

    virtual double sommaEntratePeriodo();
    virtual double sommaUscitePeriodo();
    virtual double sommaNettoTotale();

    virtual QSqlQuery queryPopolaMapGraficiPeriodo(QString tipoMov,QDate in,QDate fin,bool categorie);
    virtual QSqlQuery queryPopolaMapGraficiTotale(QString tipoMov,bool categorie);
    virtual bool popolaMappaGrafici(QMap<QString,double> * map,QSqlQuery * qr);
    virtual void resetGraficiTotale();
    virtual void resetGraficiPeriodo();
    virtual void cambioRadio();
    virtual void popolaComboTemi();
    virtual void comboTemiCambiata();
    virtual void checkLegendaCambiata();
    virtual void setZoomGrafici(QString oper);
    virtual void impostaLeDateAlDettaglioSingolo();


    creaGrafico *graTotCatE() const;

    creaGrafico *graTotCatU() const;

    creaGrafico *graTotEU() const;

    const QDate &getDataCercaInizio() const;
    void setDataCercaInizio(const QDate &newDataCercaInizio);

    const QDate &getDataCercaFine() const;
    void setDataCercaFine(const QDate &newDataCercaFine);

    creaGrafico *graPerCatE() const;

    creaGrafico *graPerCatU() const;

    creaGrafico *graPerEU() const;

public slots:
    virtual void btnFiltriAgg();
    virtual void aggNewMovimento();
    virtual bool modRigaSel();
    virtual void btnResetFiltri();
    virtual void eliminaRigaSel();
    virtual void pressioneStampa();
    virtual void stampaModello(QTableView * tabPassata);

private:
    Ui::screeMovimenti *ui;

    dbConnection * m_conn;

    QStandardItemModel * m_modello;
    QTableView * m_tableView;
    QSortFilterProxyModel * m_sortModello;

    QMap<QString,double> * m_mapTotCatE;
    QMap<QString,double> * m_mapTotCatU;
    QMap<QString,double> * m_mapTotEU;

    QMap<QString,double> * m_mapPerCatE;
    QMap<QString,double> * m_mapPerCatU;
    QMap<QString,double> * m_mapPerEU;

    QList<creaGrafico*> l_listaGrafici;

    creaGrafico * m_graTotCatE;
    creaGrafico * m_graTotCatU;
    creaGrafico * m_graTotEU;

    creaGrafico * m_graPerCatE;
    creaGrafico * m_graPerCatU;
    creaGrafico * m_graPerEU;
    QDate dataCercaInizio;
    QDate dataCercaFine;

    int statoFiltri;
};

#endif // SCREEMOVIMENTI_H
