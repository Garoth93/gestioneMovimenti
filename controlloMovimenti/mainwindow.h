#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./widgets/creagrafico.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class screeMovimenti;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void cambioRadio();
    virtual void popolaComboTemi();
    virtual void comboTemiCambiata();
    virtual void CheckLegendaCambiata();
    virtual void setZoomGrafici(QString oper);
    virtual void impostaLeDateAlDettaglioSingolo();

private:
    Ui::MainWindow *ui;

    screeMovimenti * mainScreen;

    QList<creaGrafico*> l_listaGrafici;
};
#endif // MAINWINDOW_H
