#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>

class dbConnection
{
public:
    dbConnection();

    virtual void init(QString pathdb,QString nomedb);

    virtual bool openDb();
    virtual bool initTabelle();
    QSqlDatabase *db() const;

    const QString &pathDB() const;

    const QString &nomePrincpDB() const;

    const QString &pathConDb() const;

private:
    QString m_pathDB;
    QString m_nomePrincpDB;
    QString m_pathConDb;

    QSqlDatabase * m_db;
};

#endif // DBCONNECTION_H
