#ifndef DIALOGSCEGLICONTO_H
#define DIALOGSCEGLICONTO_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QStandardItemModel>
#include <QTableView>

namespace Ui {
class dialogScegliConto;
}

class dialogScegliConto : public QDialog
{
    Q_OBJECT

public:
    explicit dialogScegliConto(QWidget *parent = nullptr);
    ~dialogScegliConto();

    virtual void init(QString pathJ,QString nomeJ,QString *nomeContoToOpen);
    virtual void loadDataFromJson();
    virtual void saveDataInJson();

    virtual void popolaModello();
    virtual void daModelloALista();
    virtual void resetTabelView();

public slots:
    virtual void pressionNuovaRiga();
    virtual void pressioneSalva();
    virtual void pressioneEliminaRiga();
    virtual void pressioneApriConto();
private:
    Ui::dialogScegliConto *ui;

    QString m_pathJson;
    QString m_nomeJson;
    QString * nomeDelContoDaAprire;
    QList<QStringList> m_listaVociJson;
    QStandardItemModel * m_model;
    QTableView * m_tableView;
};

#endif // DIALOGSCEGLICONTO_H
