#include "admin.h"

Admin::Admin()
{
    this->discriminant = 'A';
}

int Admin::equals(Admin a)
{
    if (a.getNom() == this->nom)
        if (a.getPrenom() == this->prenom)
            if(a.getEmail() == this->email)
                if (a.getMdp() == this->mdp)
                    if (a.getDiscriminant() == this->discriminant)
                        if (a.getPseudo() == this->pseudo)
                            return 1;
    return -1;
}
