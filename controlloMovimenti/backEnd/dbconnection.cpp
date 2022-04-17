#include "dbconnection.h"
#include <QDebug>

dbConnection::dbConnection()
{

}

void dbConnection::init(QString pathdb, QString nomedb)
{
    m_db=nullptr;
    m_pathDB=pathdb;
    m_nomePrincpDB=nomedb;
    m_pathConDb=QString("%1/%2").arg(m_pathDB).arg(m_nomePrincpDB);
    qDebug() << m_pathConDb;
    openDb();
}

bool dbConnection::openDb()
{
    if(m_db!=nullptr)delete m_db;
    m_db=new QSqlDatabase();
    *m_db=QSqlDatabase::addDatabase("QSQLITE");
    m_db->setDatabaseName(m_pathConDb);
    if (!m_db->open())
    {
        qDebug() << "Error: connection with database failed";
        return false;
    }
    else
    {
        initTabelle();
        qDebug() << "Database: connection ok";
        return true;
    }
}

bool dbConnection::initTabelle()
{
    QString stringToIns=QString("CREATE TABLE movimenti ("
                                "idmov INTEGER PRIMARY KEY,"
                                "datam TEXT,"
                                "descr TEXT,"
                                "valor REAL,"
                                "tipom TEXT,"
                                "catem TEXT,"
                                "fill1 TEXT,"
                                "fill2 TEXT,"
                                "fill3 TEXT,"
                                "dill1 REAL,"
                                "dill2 REAL,"
                                "dill3 REAL"
                                ")");
    QSqlQuery query(stringToIns,*m_db);
    stringToIns=QString("CREATE TABLE tabellaGenerica ("
                        "tptab TEXT NOT NULL,"
                        "cdtab TEXT NOT NULL,"
                        "descr TEXT,"
                        "fill1 TEXT,"
                        "fill2 TEXT,"
                        "fill3 TEXT,"
                        "dill1 REAL,"
                        "dill2 REAL,"
                        "dill3 REAL,"
                        "PRIMARY KEY(tptab,cdtab)"
                        ")");
    QSqlQuery query2(stringToIns,*m_db);
    return true;
}

QSqlDatabase *dbConnection::db() const
{
    return m_db;
}

const QString &dbConnection::pathDB() const
{
    return m_pathDB;
}

const QString &dbConnection::nomePrincpDB() const
{
    return m_nomePrincpDB;
}

const QString &dbConnection::pathConDb() const
{
    return m_pathConDb;
}
