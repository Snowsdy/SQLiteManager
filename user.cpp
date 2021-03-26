#include "user.h"

User::User(QObject *parent) : QObject(parent)
{
    this->discriminant = 'U';
    this->pseudo = "";
}

void User::setNom(QString val)
{
    this->nom = val;
}

void User::setPrenom(QString val)
{
    this->prenom = val;
}

void User::setEmail(QString val)
{
    this->email = val;
}

void User::setMdp(QString val)
{
    MD5(val);
}

void User::setEncodedMdp(QString val)
{
    this->mdp = val;
}

QString User::getNom(){return this->nom;}
QString User::getPrenom(){return this->prenom;}
QString User::getEmail(){return this->email;}
QString User::getMdp(){return this->mdp;}
QChar User::getDiscriminant(){return this->discriminant;}

int User::equals(User u)
{
    /// Cette méthode est défini de la façon suivante :
    /// Si il s'agit du même objet, on retourne 1 (vrai)
    /// Sinon -1
    if (u.getNom() == this->nom)
        if (u.getPrenom() == this->prenom)
            if(u.getEmail() == this->email)
                if (u.getMdp() == this->mdp)
                    if (u.getDiscriminant() == this->discriminant)
                        return 1;
    return -1;
}

void User::MD5(QString oldMdp)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QByteArray input;
    input.append(oldMdp.toLatin1());
    hash.addData(input);
    this->mdp = hash.result().toHex();
}

QString User::getPseudo(){return this->pseudo;}

void User::setPseudo(QString val)
{
    this->pseudo = val;
}

bool User::isValid(QString val)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QByteArray input;

    input.append(val.toLatin1());
    hash.addData(input);

    if (hash.result().toHex() == this->mdp) {
        return true;
    } else {
        return false;
    }
}
