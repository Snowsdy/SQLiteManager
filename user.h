#ifndef USER_H
#define USER_H

#include <QObject>
#include <QCryptographicHash>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
    /// Setters
    void setNom(QString val);
    void setPrenom(QString val);
    void setEmail(QString val);
    void setMdp(QString val);
    void setEncodedMdp(QString val);
    void setPseudo(QString val);
    bool isValid(QString val);
    /// Getters
    QString getNom();
    QString getPrenom();
    QString getEmail();
    QString getMdp();
    QChar getDiscriminant();
    QString getPseudo();
    int equals(User u);

protected:
    QString nom;
    QString prenom;
    QString email;
    QString mdp;
    QString pseudo;
    QChar discriminant;

    void MD5(QString oldMdp);

};

#endif // USER_H
