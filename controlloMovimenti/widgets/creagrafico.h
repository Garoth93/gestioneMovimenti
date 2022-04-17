#ifndef CREAGRAFICO_H
#define CREAGRAFICO_H

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QMap>

class creaGrafico: public QChartView
{
public:
    creaGrafico();

    virtual void init(QString titolo,QMap<QString,double> *map);
    virtual void componiGrafico();

    virtual void setTemaGrafico(QChart::ChartTheme theme);
    virtual void setLegenda(bool toDo);

    /** direzione==+ aumenta direzione==- diminuisce*/
    virtual void setDimensioneGrafici(QString direzione);
private:
    QPieSeries *series;
    QChart *chart;
    QMap<QString,double> *m_mappaSerie;
};

#endif // CREAGRAFICO_H
