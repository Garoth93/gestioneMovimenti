#include "screemovimenti.h"
#include "ui_screemovimenti.h"

#include "./widgets/gestionesingolomovimento.h"
#include "./widgets/dialoggestionecategoria.h"
#include "./widgets/dialogseparatore.h"


#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QPageSize>
#include <QPrinter>
#include <QTextDocument>
#include <QPrintDialog>
#include <QFileDialog>

screeMovimenti::screeMovimenti(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::screeMovimenti)
{
    ui->setupUi(this);
}

screeMovimenti::~screeMovimenti()
{
    delete ui;
}

void screeMovimenti::init(dbConnection * oggDb)
{
    m_modello=NULL;
    m_tableView=NULL;
    m_sortModello=NULL;
    m_tableView=ui->tv_movimenti;

    m_mapTotCatE=new QMap<QString,double>;
    m_mapTotCatU=new QMap<QString,double>;
    m_mapTotEU=new QMap<QString,double>;

    m_mapPerCatE=new QMap<QString,double>;
    m_mapPerCatU=new QMap<QString,double>;
    m_mapPerEU=new QMap<QString,double>;

    //gg->init("NEW GRAFO",m_mappaSerie);
    m_graTotCatE=new creaGrafico();
    m_graTotCatE->init("Totale entrate",m_mapTotCatE);

    m_graTotCatU=new creaGrafico();
    m_graTotCatU->init("Totale uscite",m_mapTotCatU);

    m_graTotEU=new creaGrafico();
    m_graTotEU->init("Totale entrare/uscite",m_mapTotEU);

    m_graPerCatE=new creaGrafico();
    m_graPerCatE->init("Totale entrate del periodo",m_mapPerCatE);

    m_graPerCatU=new creaGrafico();
    m_graPerCatU->init("Totale uscite del periodo",m_mapPerCatU);

    m_graPerEU=new creaGrafico();
    m_graPerEU->init("Totale entrare/uscite del periodo",m_mapPerEU);

    m_conn=oggDb;

    ui->le_valor1->setValidator( new QDoubleValidator(-10000,+10000, 2, this) );
    ui->le_valor2->setValidator( new QDoubleValidator(-10000,+10000, 2, this) );

    ui->frameFiltriDett->setVisible(false);
    ui->frameFiltroCal->setVisible(true);
    statoFiltri=0;

    /*btn cambio filtro*/
    ui->tb_filtriAgg->setIcon(QIcon((":/right.png")));
    ui->tb_filtriAgg->setToolTip("Filtri più dettagliati");
    connect(ui->tb_filtriAgg, &QToolButton::clicked, this, [=]()
    {
        btnFiltriAgg();
    });

    /*btn mod riga sel*/
    ui->tb_mod->setIcon(QIcon((":/opzioni.png")));
    ui->tb_mod->setToolTip("Modifica voce selezionata");
    connect(ui->tb_mod, &QToolButton::clicked, this, [=]()
    {
        modRigaSel();
    });

    /*btn cambio agg riga*/
    ui->tb_aggMov->setIcon(QIcon((":/aggiungi.png")));
    ui->tb_aggMov->setToolTip("Aggiungi nuova riga");
    connect(ui->tb_aggMov, &QToolButton::clicked, this, [=]()
    {
        aggNewMovimento();
    });

    /*pressione data calendario*/
    connect(ui->cw_main, &QCalendarWidget::clicked, this, [=]()
    {
        ricaricaModello();
    });

    /*btn reset filtri*/
    ui->tb_resetF->setIcon(QIcon((":/undo.png")));
    ui->tb_resetF->setToolTip("Reset filtri più dettagliati");
    connect(ui->tb_resetF, &QToolButton::clicked, this, [=]()
    {
        btnResetFiltri();
    });

    /*btn reset data oggi*/
    ui->tb_goToday->setIcon(QIcon((":/undo.png")));
    ui->tb_goToday->setToolTip("Data di oggi");
    connect(ui->tb_goToday, &QToolButton::clicked, this, [=]()
    {
        ui->cw_main->setSelectedDate(QDate::currentDate());
        ricaricaModello();
    });

    /*btn cerca filtri*/
    ui->tb_cercaF->setIcon(QIcon((":/cerca.png")));
    ui->tb_cercaF->setToolTip("Cerca");
    connect(ui->tb_cercaF, &QToolButton::clicked, this, [=]()
    {
        ricaricaModello();
    });

    /*btn elimina*/
    ui->tb_delRow->setIcon(QIcon((":/elimina.png")));
    ui->tb_delRow->setToolTip("Elimina riga selezionata");
    connect(ui->tb_delRow, &QToolButton::clicked, this, [=]()
    {
        eliminaRigaSel();
    });

    /*btn gestione categoria*/
    ui->tb_gestCate->setIcon(QIcon((":/opzioni.png")));
    ui->tb_gestCate->setToolTip("Gestione categoria");
    connect(ui->tb_gestCate, &QToolButton::clicked, this, [=]()
    {
        dialoggestionecategoria dialGest;
        bool resDial=false;
        dialGest.init(m_conn,"I",&resDial);
        dialGest.exec();
        if(resDial)
            popolaComboCategoria();
    });

    /*btn resize table view*/
    ui->tb_dettCompleti->setIcon(QIcon((":/up.png")));
    ui->tb_dettCompleti->setToolTip("Più dettagli");
    connect(ui->tb_dettCompleti, &QToolButton::clicked, this, [=]()
    {
        m_tableView->resizeColumnsToContents();
        m_tableView->resizeRowsToContents();
    });

    /*btn stampa*/
    ui->tb_stampaModello->setIcon(QIcon((":/printer.png")));
    ui->tb_stampaModello->setToolTip("Stampa");
    connect(ui->tb_stampaModello, &QToolButton::clicked, this, [=]()
    {
        pressioneStampa();
    });

    /*btn setta periodo mese corrente*/
    ui->tb_dataMese->setIcon(QIcon((":/calendario.ico")));
    connect(ui->tb_dataMese, &QToolButton::clicked, this, [=]()
    {
        QDate today=QDate::currentDate();
        ui->de_s1->setDate(QDate(today.year(),today.month(),1));
        ui->de_s2->setDate(QDate(today.month()==12?today.year()+1:today.year(),today.month()==12?1:today.month()+1,1));
    });


    /* btn add settimana filri*/
    ui->tb_piuSett->setToolTip("Più una settimana");
    ui->tb_piuSett->setIcon(QIcon((":/zoomPlus.png")));
    connect(ui->tb_piuSett, &QToolButton::clicked, this, [=]()
    {
        ui->de_s1->setDate((ui->de_s1->date().addDays(7)));
        ui->de_s2->setDate((ui->de_s2->date().addDays(7)));
        ricaricaModello();
    });

    /* btn add settimana filri*/
    ui->tb_menoSett->setToolTip("Meno una settimana");
    ui->tb_menoSett->setIcon(QIcon((":/zoomMinus.png")));
    connect(ui->tb_menoSett, &QToolButton::clicked, this, [=]()
    {
        ui->de_s1->setDate((ui->de_s1->date().addDays(-7)));
        ui->de_s2->setDate((ui->de_s2->date().addDays(-7)));
        ricaricaModello();
    });

    popolaComboCategoria();
    btnResetFiltri();
    ricaricaModello();
}

void screeMovimenti::creaModello()
{
    if(m_modello!=NULL) m_modello->deleteLater();
    if(m_sortModello!=NULL) m_sortModello->deleteLater();

    m_modello = new QStandardItemModel();
    m_sortModello=new QSortFilterProxyModel(this);
    m_sortModello->setSourceModel(m_modello);
    m_tableView->setSortingEnabled(true);
    m_sortModello->setFilterCaseSensitivity(Qt::CaseSensitive);
    m_tableView->sortByColumn(0,Qt::AscendingOrder);

    /*prima riempio lista con stringa vuota*/
    QStringList ll;
    for (int xx =0; xx<=MAXCOLUMMOV;xx++)
        ll.insert(xx,QString(""));

    ll[cmm_idmov]=             "IDDOC";
    ll[cmm_datam]=             "DATA";
    ll[cmm_descr]=             "DESCRIZIONE";
    ll[cmm_tipom]=             "TIPO MOVIMENTO";
    ll[cmm_valor]=             "VALORE";
    ll[cmm_catem]=             "CATEGORIA";

    /*setto intestazione colonne*/
    m_modello->setHorizontalHeaderLabels(ll);

    /*setto model alla tableview*/
    m_tableView->setModel(m_modello);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    /*nascondo colonne in caso ci siano colonne da noscondere*/
    foreach(int cc,colonneModelloMovimenti)
        m_tableView->hideColumn(cc);
}

QString screeMovimenti::creaQrySelect(QString perSommaQr)
{
    QString toRet="";
    if(statoFiltri==0)
    {
        QDate dataInCorso=ui->cw_main->selectedDate();
        oggMov toModUtil;
        toModUtil.init(m_conn);
        QString dataSrc=toModUtil.convertDate(dataInCorso);
        toRet=QString("where datam='%1'").arg(dataSrc);
    }
    else
    {
        QStringList lll;
        if(ui->cb_catF->isChecked())
        {
            lll << QString("catem='%1'").arg(ui->cb_categoria->currentText());
        }
        if(ui->cb_tipoF->isChecked())
        {
            if(perSommaQr=="")
                lll << QString("tipom='%1'").arg(ui->cb_tipoMov->currentText());
            else if(perSommaQr=="E")
                lll << QString("tipom='%1'").arg("Entrata");
            else if(perSommaQr=="U")
                lll << QString("tipom='%1'").arg("Uscita");
        }
        if(ui->cb_valorF->isChecked())
        {
            lll << QString("valor>=%1 and valor <=%2").arg(ui->le_valor1->text()).arg(ui->le_valor2->text());
        }
        if(ui->cb_dataF->isChecked())
        {
            QDate d1=ui->de_s1->date();
            QDate d2=ui->de_s2->date();
            QString sd1,sd2;
            oggMov oggUti;
            oggUti.init(m_conn);
            sd1=oggUti.convertDate(d1);
            sd2=oggUti.convertDate(d2);
            lll << QString("datam>='%1' and datam <='%2'").arg(sd1).arg(sd2);
        }
        if(lll.count()>0)
            toRet=QString("where %1").arg(lll.join(" and "));
    }
    return toRet;
}

void screeMovimenti::popolaModello()
{
    if(statoFiltri==1)
    {
        bool almenoUna=false;
        if(ui->cb_catF->isChecked() || ui->cb_valorF->isChecked() || ui->cb_tipoF->isChecked() || ui->cb_dataF->isChecked())
            almenoUna=true;
        if(!almenoUna)
        {
            QMessageBox msWar;
            msWar.setText("Seleziona almeno una casella dei filtri.");
            msWar.exec();
            return;
        }
    }
    QString stringSel=creaQrySelect();
    stringSel=QString("select * from movimenti %1 order by datam ASC").arg(stringSel);
    QSqlQuery query(stringSel,*(m_conn->db()));
    while(query.next())
    {
        QList<QStandardItem *>lItms;

        for(int xx=0; xx<=MAXCOLUMMOV;xx++)
        {
            lItms.insert(xx,new QStandardItem(QString("")));
            (lItms)[xx]->setEditable(false);
        }

        oggMov oggt;
        oggt.init(m_conn);

        lItms[cmm_idmov]->      setData(query.value("idmov").toString(),Qt::EditRole);
        lItms[cmm_datam]->      setData(oggt.convertDateVideo(oggt.convertDate(query.value("datam").toString())),Qt::EditRole);
        lItms[cmm_descr]->      setData(query.value("descr").toString(),Qt::EditRole);
        lItms[cmm_tipom]->      setData(query.value("tipom").toString(),Qt::EditRole);
        lItms[cmm_valor]->      setData(query.value("valor").toString(),Qt::EditRole);
        lItms[cmm_catem]->      setData(query.value("catem").toString(),Qt::EditRole);

        int r=230,g=103,b=103;
        if(query.value("tipom").toString()=="Entrata")
        {
            r=105;
            g=201;
            b=60;
        }
        for(int xx=0; xx<=MAXCOLUMMOV;xx++)
            lItms[xx]->setBackground(QColor(r,g,b));

        /*appendo al modello la riga*/
        m_modello->appendRow(lItms);
    }
    m_tableView->resizeColumnsToContents();
    double usc= sommaUscitePeriodo();
    double ent= sommaEntratePeriodo();
    ui->le_uscite->setText(QString("%1").arg(usc));
    ui->le_entrate->setText(QString("%1").arg(ent));
    ui->le_netto->setText(QString("%1").arg(ent+usc));
    ui->le_nettoTot->setText(QString("%1").arg(sommaNettoTotale()));
}

void screeMovimenti::ricaricaModello()
{
    creaModello();
    popolaModello();
    /*ricarico la grafici */
    resetGraficiTotale();
    resetGraficiPeriodo();
}

void screeMovimenti::popolaComboCategoria()
{
    QStringList listaToIns;
    QString stringSel;
    stringSel=QString("select cdtab from tabellaGenerica where tptab='categorie'");
    QSqlQuery query(stringSel,*(m_conn->db()));
    while(query.next())
    {
        listaToIns << query.value("cdtab").toString();
    }
    ui->cb_categoria->clear();
    ui->cb_categoria->addItems(listaToIns);
}

double screeMovimenti::sommaEntratePeriodo()
{
    QString stringSelE=creaQrySelect("E");
    stringSelE=QString("select sum(valor) from movimenti %1").arg(stringSelE);
    if(!stringSelE.contains("tipom"))
        stringSelE=QString("%1 and tipom='Entrata'").arg(stringSelE);
    QSqlQuery query(stringSelE,*(m_conn->db()));
    if(query.next())
        return query.value(0).toDouble();
    return 0.0;
}

double screeMovimenti::sommaUscitePeriodo()
{
    QString stringSelU=creaQrySelect("U");
    stringSelU=QString("select sum(valor) from movimenti %1").arg(stringSelU);
    if(!stringSelU.contains("tipom"))
        stringSelU=QString("%1 and tipom='Uscita'").arg(stringSelU);
    QSqlQuery query(stringSelU,*(m_conn->db()));
    if(query.next())
        return query.value(0).toDouble();
    return 0.0;
}

double screeMovimenti::sommaNettoTotale()
{
    QString stringSelE=QString("select sum(valor) from movimenti");
    QSqlQuery query(stringSelE,*(m_conn->db()));
    if(query.next())
        return query.value(0).toDouble();
    return 0.0;
}

QSqlQuery screeMovimenti::queryPopolaMapGraficiPeriodo(QString tipoMov, QDate in, QDate fin, bool categorie)
{
    QString stringSel;
    QString sd1,sd2;
    oggMov oggUti;
    oggUti.init(m_conn);
    sd1=oggUti.convertDate(in);
    sd2=oggUti.convertDate(fin);
    if(categorie)
    {
        stringSel=QString("select sum(valor),catem from movimenti where "
                          "datam>='%1' and datam <='%2' and tipom='%3' group by catem order by sum(valor)")
                .arg(sd1)
                .arg(sd2)
                .arg(tipoMov);
    }
    else
    {
        stringSel=QString("select sum(valor),tipom from movimenti where "
                          "datam>='%1' and datam <='%2' group by tipom order by sum(valor)")
                .arg(sd1)
                .arg(sd2);
    }
    QSqlQuery query(stringSel,*(m_conn->db()));
    return  query;
}

QSqlQuery screeMovimenti::queryPopolaMapGraficiTotale(QString tipoMov, bool categorie)
{
    QString stringSel;
    if(categorie)
    {
        stringSel=QString("select sum(valor),catem from movimenti where "
                          "tipom='%3' group by catem order by sum(valor)")
                .arg(tipoMov);
    }
    else
    {
        stringSel=QString("select sum(valor),tipom from movimenti group by tipom order by sum(valor)");
    }
    QSqlQuery query(stringSel,*(m_conn->db()));
    return  query;
}

bool screeMovimenti::popolaMappaGrafici(QMap<QString, double> *map, QSqlQuery * qr)
{
    map->clear();
    while(qr->next())
    {
        (*map)[qr->value(1).toString()]=qr->value(0).toDouble();
    }
    return true;
}

void screeMovimenti::resetGraficiTotale()
{
    QSqlQuery qtotE=queryPopolaMapGraficiTotale("Entrata",true);
    QSqlQuery qtotU=queryPopolaMapGraficiTotale("Uscita",true);
    QSqlQuery qtot=queryPopolaMapGraficiTotale("",false);

    popolaMappaGrafici(m_mapTotCatE,&qtotE);
    popolaMappaGrafici(m_mapTotCatU,&qtotU);
    popolaMappaGrafici(m_mapTotEU,&qtot);

    m_graTotCatE->componiGrafico();
    m_graTotCatU->componiGrafico();
    m_graTotEU->componiGrafico();
}

void screeMovimenti::resetGraficiPeriodo()
{
    QSqlQuery qperE=queryPopolaMapGraficiPeriodo("Entrata",dataCercaInizio,dataCercaFine,true);
    QSqlQuery qperU=queryPopolaMapGraficiPeriodo("Uscita",dataCercaInizio,dataCercaFine,true);
    QSqlQuery qper=queryPopolaMapGraficiPeriodo("",dataCercaInizio,dataCercaFine,false);

    popolaMappaGrafici(m_mapPerCatE,&qperE);
    popolaMappaGrafici(m_mapPerCatU,&qperU);
    popolaMappaGrafici(m_mapPerEU,&qper);

    m_graPerCatE->componiGrafico();
    m_graPerCatU->componiGrafico();
    m_graPerEU->componiGrafico();
}

void screeMovimenti::btnFiltriAgg()
{
    if(ui->frameFiltroCal->isVisible())
    {
        statoFiltri=1;
        ui->frameFiltroCal->setVisible(false);
        ui->frameFiltriDett->setVisible(true);
    }
    else
    {
        statoFiltri=0;
        ui->frameFiltroCal->setVisible(true);
        ui->frameFiltriDett->setVisible(false);
    }
}

void screeMovimenti::aggNewMovimento()
{
    gestioneSingoloMovimento dialGest(this);
    bool resOpe=false;
    dialGest.init(m_conn,"I",&resOpe);
    dialGest.setDataCalenario(ui->cw_main->selectedDate());
    dialGest.exec();
    if(resOpe)
    {
        ricaricaModello();
    }
}

bool screeMovimenti::modRigaSel()
{
    if(m_modello==nullptr)return false;
    if(!(m_tableView->selectionModel()->currentIndex().isValid()))return false;
    int rigaSel=m_tableView->selectionModel()->currentIndex().row();
    int idMovToMod=m_modello->index(rigaSel,cmm_idmov).data(Qt::EditRole).toInt();
    oggMov toModOgg;
    toModOgg.init(m_conn);
    toModOgg.setIdMov(idMovToMod);
    toModOgg.caricaDato();
    gestioneSingoloMovimento dialGest(this);
    bool resOpe=false;
    dialGest.init(m_conn,"M",&resOpe,&toModOgg);
    dialGest.exec();
    if(resOpe)
    {
        ricaricaModello();
        return true;
    }
    return false;
}

void screeMovimenti::btnResetFiltri()
{
    ui->le_valor1->setText("");
    ui->le_valor2->setText("");
    ui->de_s1->setDate(QDate::currentDate());
    ui->de_s2->setDate(QDate::currentDate());
    ui->cb_catF->setChecked(false);
    ui->cb_tipoF->setChecked(false);
    ui->cb_valorF->setChecked(false);
    ui->cb_dataF->setChecked(false);
}

void screeMovimenti::eliminaRigaSel()
{
    if(m_modello==nullptr)return;
    int rigaSel=m_tableView->selectionModel()->currentIndex().row();
    if(!(m_tableView->selectionModel()->currentIndex().isValid()))return;
    int idMovToMod=m_modello->index(rigaSel,cmm_idmov).data(Qt::EditRole).toInt();
    oggMov toDelOgg;
    toDelOgg.init(m_conn);
    toDelOgg.setIdMov(idMovToMod);
    toDelOgg.caricaDato();
    toDelOgg.deleteDato();
    ricaricaModello();
}

void screeMovimenti::pressioneStampa()
{
    if(m_modello==nullptr)return;
    stampaModello(m_tableView);
}

void screeMovimenti::stampaModello(QTableView * tabPassata)
{
    QStandardItemModel *modelPassato=dynamic_cast<QStandardItemModel*>(tabPassata->model());
    if(!modelPassato) return;
    QMap<int,QString> nomeColonne;
    int numeroColonne=modelPassato->columnCount();

    for(int colonna=0;colonna<numeroColonne;colonna++)
    {
        if(tabPassata->isColumnHidden(colonna))continue;
        nomeColonne[colonna]=modelPassato->headerData(colonna,Qt::Horizontal).toString();
    }

    QDialog dialogTmpChose;
    dialogTmpChose.setWindowTitle("Scegli opzioni");
    QVBoxLayout mainLayout;

    /*---> parte colonne*/
    QGroupBox widgetTitoliColonne("Scegli colonne da stampare");
    QGridLayout lgrid_colonne;
    QMap<int,QString>::iterator itMap=nomeColonne.begin();
    int rigaQG=-1;
    int colonnaQG=0;
    for(;itMap!=nomeColonne.end();itMap++)
    {
        QCheckBox * tmpCh=new QCheckBox(itMap.value());
        tmpCh->setObjectName(QString::number(itMap.key()));
        if(colonnaQG%4==0){
            rigaQG++;
            colonnaQG = 0;
        }
        lgrid_colonne.addWidget(tmpCh,rigaQG, colonnaQG);
        colonnaQG++;
    }
    widgetTitoliColonne.setLayout(&lgrid_colonne);
    QList<QCheckBox *> allCehck;
    for(auto *cb : widgetTitoliColonne.findChildren<QCheckBox *>())
    {
        cb->setChecked(true);
        allCehck << cb;
    }
    /*<--- parte colonne*/

    /*---> parte orientazione*/
    QGroupBox frameOrientazione("Orientazione");
    QHBoxLayout layoutRadioOrientazione;
    QRadioButton radioO;
    QRadioButton radioV;
    layoutRadioOrientazione.addWidget(&radioO);
    layoutRadioOrientazione.addWidget(&radioV);
    frameOrientazione.setLayout(&layoutRadioOrientazione);
    lgrid_colonne.addWidget(&frameOrientazione,++rigaQG,0);
    radioO.setText("Orizzontale");
    radioV.setText("Verticale");
    radioO.setChecked(true);
    /*<--- parte orientazione*/

    /*---> parte formato*/
    QGroupBox frameFormato("Formato pagina");
    QHBoxLayout layoutRadioFormato;
    QRadioButton radioA3;
    QRadioButton radioA4;
    layoutRadioFormato.addWidget(&radioA3);
    layoutRadioFormato.addWidget(&radioA4);
    frameFormato.setLayout(&layoutRadioFormato);
    lgrid_colonne.addWidget(&frameFormato,++rigaQG,0);
    radioA3.setText("A3");
    radioA4.setText("A4");
    radioA3.setChecked(true);
    /*<--- parte formato*/

    /*---> parte estensione*/
    QGroupBox frameEstensione("Estensione");
    QHBoxLayout layoutRadioEstensione;
    QRadioButton radiocsv;
    QRadioButton radiopdf;
    layoutRadioEstensione.addWidget(&radiocsv);
    layoutRadioEstensione.addWidget(&radiopdf);
    frameEstensione.setLayout(&layoutRadioEstensione);
    lgrid_colonne.addWidget(&frameEstensione,++rigaQG,0);
    radiocsv.setText("CSV");
    radiopdf.setText("PDF");
    radiopdf.setChecked(true);
    /*<--- parte estensione*/

    /*---> parte bottone conferma*/
    QWidget widgetBottoni;
    QPushButton bottoneConferma;
    bottoneConferma.setText("Ok");
    connect(&bottoneConferma, &QPushButton::clicked,[&dialogTmpChose](){
        dialogTmpChose.close();
    });
    QPushButton bottoneEsci;
    QHBoxLayout layoutBottoni;
    bool annullaBott=false;
    bottoneEsci.setText("Esci");
    connect(&bottoneEsci, &QPushButton::clicked,[&dialogTmpChose,&annullaBott](){
        annullaBott=true;
        dialogTmpChose.close();
    });
    layoutBottoni.addWidget(&bottoneConferma);
    layoutBottoni.addWidget(&bottoneEsci);
    widgetBottoni.setLayout(&layoutBottoni);
    /*<--- parte bottone conferma*/

    /*aggiungo al main layout i pezzi*/
    mainLayout.addWidget(&widgetTitoliColonne);
    mainLayout.addWidget(&frameOrientazione);
    mainLayout.addWidget(&frameFormato);
    mainLayout.addWidget(&frameEstensione);
    mainLayout.addWidget(&widgetBottoni);
    dialogTmpChose.setLayout(&mainLayout);

    dialogTmpChose.setWindowTitle("STAMPA DATI");

    /*eseguo il dialog*/
    dialogTmpChose.exec();

    /*se premuto esci*/
    if(annullaBott) return;

    /*prendo le colonne con ceck*/
    QList<int> listaColonneVisibili;
    foreach(QCheckBox * cb,allCehck)
    {
        if(cb->isChecked())
            listaColonneVisibili << cb->objectName().toInt();
    }

    bool pdf=true;
    if(radiocsv.isChecked())
        pdf=false;
    else
        pdf=true;

    if(pdf)
    {
        QString html;
        html = "<html><body><table border=\"1\" cellspacing=\"0\" cellpadding=\"3\">";
        html += "<tr>";
        numeroColonne=modelPassato->columnCount();
        for(int colonna=0;colonna<numeroColonne;colonna++)
        {
            if(tabPassata->isColumnHidden(colonna))continue;
            if(!listaColonneVisibili.contains(colonna)) continue;
            html += "<td>" + modelPassato->headerData(colonna,Qt::Horizontal).toString() + "</td>";
        }
        for(int row = 0; row < modelPassato->rowCount(); row++)
        {
            html += "<tr>";
            for(int column = 0; column < modelPassato->columnCount(); column++)
            {
                if(tabPassata->isColumnHidden(column))continue;
                if(!listaColonneVisibili.contains(column)) continue;
                QString data = modelPassato->data(modelPassato->index(row, column), Qt::DisplayRole).toString();
                QColor colore= modelPassato->item(row,column)->background().color();
                QWidget::palette().color(QWidget::backgroundRole());
                html += QString("<td bgcolor=\"%1\">").arg(colore.name(QColor::HexRgb)=="#000000"?"#ffffff":colore.name(QColor::HexRgb)) + data + "</td>";
            }
            html += "</tr>";
        }
        html += "<tr>";
        double usc=sommaUscitePeriodo();
        double ent=sommaEntratePeriodo();
        double net=usc+ent;
        html += QString("<td>") + QString("NETTO: %1").arg(net) + "</td>";
        html += QString("<td>") + QString("ENTRATE: %1").arg(ent) + "</td>";
        html += QString("<td>") + QString("USCITE: %1").arg(usc) + "</td>";
        html += "</tr>";
        html += "</table></body></html>";

        int orientazione=0;
        /*prendo orientazione*/
        if(radioO.isChecked())
            orientazione=1;
        else
            orientazione=0;

        /*prendo formato pagina*/
        QPageSize forPage;
        /*prendo orientazione*/
        if(radioA3.isChecked())
            forPage=QPageSize(QPageSize::A3);
        else
            forPage=QPageSize(QPageSize::A4);

        QPrinter printer;
        if(orientazione==1)
            printer.setPageOrientation(QPageLayout::Landscape);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setFullPage(true);
        printer.setPageSize(forPage);

        QFileDialog * dialogTake;
        dialogTake=new QFileDialog(this,"Salva pdf",QDir::homePath(),"*pdf");
        dialogTake->exec();
        QStringList nomeFIle;
        nomeFIle = dialogTake->selectedFiles();
        delete dialogTake;
        QString pathFile="";
        if(nomeFIle.count()!=0)
        {
            pathFile=nomeFIle[0];
        }
        else
        {
            return;
        }
        if(!(pathFile.right(3)=="pdf"))
            pathFile=QString("%1.pdf").arg(pathFile);
        printer.setOutputFileName(QString("%1").arg(pathFile));

        /*
        QPrintDialog *dialog = new QPrintDialog(&printer);
        if(dialog->exec() == QDialog::Accepted)
        {
            QTextDocument document;
            document.setHtml(html);
            document.print(&printer);
        }*/

        QMessageBox msgBox;
        msgBox.setText(QString("Pdf generato in: %1").arg(pathFile));
        msgBox.exec();

        QTextDocument document;
        document.setHtml(html);
        document.print(&printer);
    }
    else if(!pdf)
    {
        QFileDialog * dialogTake;
        dialogTake=new QFileDialog(this,"Salva csv",QDir::homePath(),"*csv");
        dialogTake->exec();
        QStringList nomeFIle;
        nomeFIle = dialogTake->selectedFiles();
        delete dialogTake;
        QString pathFile="";
        if(nomeFIle.count()!=0)
        {
            pathFile=nomeFIle[0];
        }
        else
        {
            return;
        }
        if(!(pathFile.right(3)=="csv"))
            pathFile=QString("%1.csv").arg(pathFile);
        QFile file(QString("%1").arg(pathFile));
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << file.errorString();
            return;
        }
        QString separatore=";";
        dialogSeparatore desp;
        desp.init(&separatore);
        desp.exec();
        QTextStream stream(&file);
        QStringList lhead;
        for(int colonna=0;colonna<numeroColonne;colonna++)
        {
            if(tabPassata->isColumnHidden(colonna))continue;
            if(!listaColonneVisibili.contains(colonna)) continue;
            QString dataH=modelPassato->headerData(colonna,Qt::Horizontal).toString();
            dataH=dataH.replace("\n"," ");
            lhead << dataH;
        }
        stream << lhead.join(separatore);
        stream << "\n";
        QStringList lbod;
        for(int row = 0; row < modelPassato->rowCount(); row++)
        {
            for(int column = 0; column < modelPassato->columnCount(); column++)
            {
                if(tabPassata->isColumnHidden(column))continue;
                if(!listaColonneVisibili.contains(column)) continue;
                QString data = modelPassato->data(modelPassato->index(row, column), Qt::DisplayRole).toString();
                data=data.replace("\n"," ");
                lbod << data;
            }
            stream << lbod.join(separatore);
            stream << "\n";
            lbod.clear();
        }
        QMessageBox msgBox;
        msgBox.setText(QString("Csv generato in: %1").arg(pathFile));
        msgBox.exec();
    }
}

creaGrafico *screeMovimenti::graPerEU() const
{
    return m_graPerEU;
}

creaGrafico *screeMovimenti::graPerCatU() const
{
    return m_graPerCatU;
}

creaGrafico *screeMovimenti::graPerCatE() const
{
    return m_graPerCatE;
}

const QDate &screeMovimenti::getDataCercaFine() const
{
    return dataCercaFine;
}

void screeMovimenti::setDataCercaFine(const QDate &newDataCercaFine)
{
    dataCercaFine = newDataCercaFine;
}

const QDate &screeMovimenti::getDataCercaInizio() const
{
    return dataCercaInizio;
}

void screeMovimenti::setDataCercaInizio(const QDate &newDataCercaInizio)
{
    dataCercaInizio = newDataCercaInizio;
}

creaGrafico *screeMovimenti::graTotEU() const
{
    return m_graTotEU;
}

creaGrafico *screeMovimenti::graTotCatU() const
{
    return m_graTotCatU;
}

creaGrafico *screeMovimenti::graTotCatE() const
{
    return m_graTotCatE;
}
