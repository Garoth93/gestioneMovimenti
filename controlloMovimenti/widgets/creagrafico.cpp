#include "creagrafico.h"

creaGrafico::creaGrafico()
{
    m_mappaSerie=nullptr;
    series=nullptr;
    chart=nullptr;
}

void creaGrafico::init(QString titolo,QMap<QString,double> *map)
{
    m_mappaSerie=map;
    series = new QPieSeries();
    chart = new QChart();

    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTitle(titolo);
    chart->addSeries(series);

    this->setChart(chart);
    this->setRenderHint(QPainter::Antialiasing);

    this->setMinimumSize(350,350);
    this->setMaximumSize(350,350);
}

void creaGrafico::componiGrafico()
{
    if(m_mappaSerie==nullptr)return;

    series->clear();
    QMap<QString,double>::iterator itMap=m_mappaSerie->begin();
    for(;itMap!=m_mappaSerie->end();itMap++)
    {
        series->append(itMap.key(), itMap.value());
    }

    int totserie=series->count();
    for(int xx=0; xx<totserie; xx++)
    {
        dynamic_cast<QPieSlice*>(series->slices().at(xx))->setExploded();
        dynamic_cast<QPieSlice*>(series->slices().at(xx))->setLabelVisible();
        /*slice->setPen(QPen(Qt::darkGreen, 2));
            slice->setBrush(Qt::green);*/
    }
}

void creaGrafico::setTemaGrafico(QChart::ChartTheme theme)
{
    QPalette pal = window()->palette();
    if (theme == QChart::ChartThemeLight) {
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    //![8]
    } else if (theme == QChart::ChartThemeDark) {
        pal.setColor(QPalette::Window, QRgb(0x121218));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    } else if (theme == QChart::ChartThemeBlueCerulean) {
        pal.setColor(QPalette::Window, QRgb(0x40434a));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    } else if (theme == QChart::ChartThemeBrownSand) {
        pal.setColor(QPalette::Window, QRgb(0x9e8965));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else if (theme == QChart::ChartThemeBlueNcs) {
        pal.setColor(QPalette::Window, QRgb(0x018bba));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else if (theme == QChart::ChartThemeHighContrast) {
        pal.setColor(QPalette::Window, QRgb(0xffab03));
        pal.setColor(QPalette::WindowText, QRgb(0x181818));
    } else if (theme == QChart::ChartThemeBlueIcy) {
        pal.setColor(QPalette::Window, QRgb(0xcee7f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else {
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    chart->setTheme(theme);
}

void creaGrafico::setLegenda(bool toDo)
{
    if(toDo)
        chart->legend()->show();
    else
        chart->legend()->hide();
}

void creaGrafico::setDimensioneGrafici(QString direzione)
{
    int currentAlt=this->size().height();
    int currentLarg=this->size().width();
    if(direzione=="-")
    {
        currentAlt-=25;
        currentLarg-=25;
        if(currentLarg<=0)
            currentLarg=25;
        if(currentAlt<=0)
            currentAlt=25;
    }
    else
    {
        currentAlt+=25;
        currentLarg+=25;
    }
    this->setMinimumSize(currentLarg,currentAlt);
    this->setMaximumSize(currentLarg,currentAlt);
}
