#include "sqlservice.h"
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

SqlService::SqlService()
{
    bdd = "sqliteManager";
    table = "users";
    flag = false; // non connecte
}

SqlService::~SqlService(){
    qWarning("Fermeture et destruction du service SQL");
    if(flag){
        db.commit();
        db.close();
        delete q;
    }
}

bool SqlService::connexionBase() {
    if(!flag) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(bdd);
        if ( db.open() ) {
            qWarning("Connexion DataBase OK");
            q = new QSqlQuery(db);
            flag = true;
        }
        else
            msgInfo = db.lastError().text();
    }
    else
        msgInfo = "DataBase déjà ouverte !";
    return flag;
}

bool SqlService::creerTable(){
    QString req = "";
    if(!flag){
        msgInfo = "Pas de connexion en cours à la base !";
        return false;
    }
    req.sprintf("CREATE TABLE %s "
    "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "nom VARCHAR(128) NOT NULL, "
    "prenom VARCHAR(128) NOT NULL, "
    "discriminant VARCHAR(1) NOT NULL, "
    "login VARCHAR(128), "
    "mdp VARCHAR(128) NOT NULL, "
    "email VARCHAR(128) NOT NULL); ", table.toLatin1().data());
    qWarning() << "Requête : " << req;
    r = q->exec(req);
    if(r == false){
        msgInfo = q->lastError().text();
    }
    else {
        msgInfo= "Table créée avec succès !";
    }
    return r;
}

void SqlService::listDriverDispo(void){
    QSqlDatabase dbase;
    QStringList l = dbase.drivers();
    qWarning("count=%d", l.count());
    for(int i=0; i<l.count(); i++)
        qWarning("%s",l.at(i).toLocal8Bit().constData());
}

QStringList SqlService::getListeTable(void) {
  return db.tables(QSql::Tables);
}

QString SqlService::getTable(void) {
  return table;
}

bool SqlService::getFlag(void){
    return flag;
}

QString SqlService::getMsgInfo(void){
    return msgInfo;
}

bool SqlService::supprimerTable(){
    if(!flag){
        msgInfo = "Pas de connexion en cours à la base !";
        return false;
    }
    if ((r=q->exec("DROP TABLE " + table)) == false)
        msgInfo= q->lastError().databaseText();
    else{
        msgInfo = "Table supprimée avec succès  .\n";
        db.commit();
    }
    return r;
}
