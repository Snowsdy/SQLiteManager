#ifndef SQLSERVICE_H
#define SQLSERVICE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlError>

class SqlService
{
/**
  * Auteur: Snowsdy
  */
public:
    bool flag;          // etat connexion à la base
    bool r;             // valeur de retour des fonctions bool
    QSqlDatabase db;	// référence sur base
    QSqlQuery *q;		// objet de requete sql

    QString msgInfo;
    QString table;
    QString bdd;
public:
    SqlService();
    ~SqlService();

    bool connexionBase();
    bool creerTable();
    bool supprimerTable();

    QString getMsgInfo(void);
    QString getTable(void);
    QStringList getListeTable(void);
    bool getFlag(void);
    void listDriverDispo(void);
};

#endif // SQLSERVICE_H
