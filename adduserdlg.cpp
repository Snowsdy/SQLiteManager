#include "adduserdlg.h"
#include "ui_adduserdlg.h"

AddUserDlg::AddUserDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUserDlg)
{
    ui->setupUi(this);
    u = new User();
    a = new Admin();
    isSetted = false;
}

AddUserDlg::~AddUserDlg()
{
    delete ui;
}

User *AddUserDlg::getUser(){return u;}

Admin *AddUserDlg::getAdmin(){return a;}

void AddUserDlg::setUser(User *user)
{
    isSetted = true;
    ui->lE_Nom->setText(user->getNom());
    ui->lE_Prenom->setText(user->getPrenom());
    ui->cB_Admin->setCheckState(Qt::CheckState::Unchecked);
    ui->lE_Email->setText(user->getEmail());
    ui->lE_Mdp->setText(user->getMdp());
}

void AddUserDlg::setAdmin(Admin *ad)
{
    isSetted = true;
    ui->lE_Nom->setText(ad->getNom());
    ui->lE_Prenom->setText(ad->getPrenom());
    ui->cB_Admin->setCheckState(Qt::CheckState::Checked);
    ui->lE_Email->setText(ad->getEmail());
    ui->lE_Pseudo->setText(ad->getPseudo());
    ui->lE_Mdp->setText(ad->getMdp());
}

void AddUserDlg::closeEvent(QCloseEvent *event)
{
    event->ignore();
    QMessageBox::critical(this, "Restriction", "Navré mais pour quitter, appuyer sur le bouton Annuler.");
}

void AddUserDlg::on_cB_Admin_stateChanged(int arg1)
{
    switch (arg1) {
    case 0:
        // Donc n'est pas checked
        ui->label_Pseudo->setEnabled(false);
        ui->lE_Pseudo->setEnabled(false);
        break;
    default:
        // Est checked
        ui->label_Pseudo->setEnabled(true);
        ui->lE_Pseudo->setEnabled(true);
        break;
    }
}

void AddUserDlg::on_pB_Cancel_clicked()
{
    this->reject();
    clear();
}

void AddUserDlg::on_pB_Accept_clicked()
{
    if (!isSetted) {
        // Ajout :
        if (isValid()){
            if (ui->cB_Admin->isChecked()){
                // Admin confirmed
                a->setNom(ui->lE_Nom->text());
                a->setPrenom(ui->lE_Prenom->text());
                a->setEmail(ui->lE_Email->text());
                a->setMdp(ui->lE_Mdp->text());
                a->setPseudo(ui->lE_Pseudo->text());

                emit adminDetected();
            } else {
                // User confirmed
                u->setNom(ui->lE_Nom->text());
                u->setPrenom(ui->lE_Prenom->text());
                u->setEmail(ui->lE_Email->text());
                u->setMdp(ui->lE_Mdp->text());

                emit userDetected();
            }
            this->accept();
            clear();
        } else {
            ui->label->setText("La confirmation ne correspond pas au mot de passe !");
        }
    } else {
        // Modification :
        if (isValid()) {
            if (ui->cB_Admin->isChecked()){
                // Admin confirmed
                a->setNom(ui->lE_Nom->text());
                a->setPrenom(ui->lE_Prenom->text());
                a->setEmail(ui->lE_Email->text());
                a->setEncodedMdp(ui->lE_Mdp->text());
                a->setPseudo(ui->lE_Pseudo->text());

                emit adminDetected();
            } else {
                // User confirmed
                u->setNom(ui->lE_Nom->text());
                u->setPrenom(ui->lE_Prenom->text());
                u->setEmail(ui->lE_Email->text());
                u->setEncodedMdp(ui->lE_Mdp->text());

                emit userDetected();
            }
            this->accept();
            clear();
            isSetted = false;
        } else {
            ui->label->setText("La confirmation ne correspond pas au mot de passe !");
        }
    }
}

void AddUserDlg::clear()
{
    /**
      Sert à vider tout contenu dans les composant du dialogue.
    */
    ui->lE_Nom->clear();
    ui->lE_Prenom->clear();
    ui->lE_Email->clear();
    ui->lE_Pseudo->clear();
    ui->lE_Mdp->clear();
    ui->lE_ConfirmMdp->clear();
    ui->label->clear();
    ui->cB_Admin->setCheckState(Qt::CheckState::Unchecked);
}

bool AddUserDlg::isValid()
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QCryptographicHash hash2(QCryptographicHash::Md5);
    QByteArray input, input2;

    // 2 cas possibles :
    // Cas 1 : ajout d'un utilisateur, donc on vérifie la correspondance :

    input.append(ui->lE_Mdp->text().toLatin1());
    hash.addData(input);

    input2.append(ui->lE_ConfirmMdp->text().toLatin1());
    hash2.addData(input2);

    if (hash.result().toHex() == hash2.result().toHex()) {
        // Correspondance
        return true;
    } else if (ui->lE_Mdp->text().toLatin1() == hash2.result().toHex()) {
        // Cas n°2 => Correspondance :
        return true;
    } else {
        return false;
    }
}
