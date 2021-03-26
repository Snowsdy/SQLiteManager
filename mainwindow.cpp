#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    id = 0;
    sql = new SqlService();
    if(sql->connexionBase() == true){
        ui->tE_Info->append("Connexion base de données SQLITE OK !");
        QStringList sl = sql->getListeTable();
        if(sl.isEmpty()){
            ui->tE_Info->append("Pas de table(s) dans la base !");
            if(sql->creerTable()){
                ui->tE_Info->append("Table \"users\" créée !");
            }
            else {
                ui->tE_Info->append("Problème de création de table !");
                ui->tE_Info->append(sql->getMsgInfo());
            }
        }
        else
            ui->tE_Info->append("Table de données déjà existante !");
    }
    else {
        ui->tE_Info->append(sql->getMsgInfo());
    }

    if (sql->getFlag()) {
        model = new QSqlTableModel();
        model->setTable(sql->getTable());
        model->setSort(0, Qt::AscendingOrder);
        model->select();
        ui->tableView->setModel(model);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }

    connect(&addUserDlg, SIGNAL(userDetected(void)), this, SLOT(detectUser(void)));
    connect(&addUserDlg, SIGNAL(adminDetected(void)), this, SLOT(detectAdmin(void)));

    connect(&editUserDlg, SIGNAL(userDetected(void)), this, SLOT(editUser(void)));
    connect(&editUserDlg, SIGNAL(adminDetected(void)), this, SLOT(editAdmin(void)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pB_Add_clicked()
{
    addUserDlg.setModal(true);
    addUserDlg.show();
}

void MainWindow::on_pB_Edit_clicked()
{
    QSqlRecord result = model->record();

    User *u= new User();
    Admin *a = new Admin();

    if (ui->tableView->selectionModel()->currentIndex().isValid()) {
        /// Arrive à récuperer l'user ou l'admin...

        int idID = result.indexOf("id");
        int idDiscriminant = result.indexOf("discriminant");
        int idNom = result.indexOf("nom");
        int idPrenom = result.indexOf("prenom");
        int idLogin = result.indexOf("login");
        int idEmail = result.indexOf("email");
        int idMdp = result.indexOf("mdp");

        id = ui->tableView->currentIndex().model()->data(ui->tableView->selectionModel()->model()->index(ui->tableView->currentIndex().row(), idID)).toInt();
        QString nom = ui->tableView->currentIndex().model()->data(ui->tableView->selectionModel()->model()->index(ui->tableView->currentIndex().row(), idNom)).toString();
        QString prenom = ui->tableView->currentIndex().model()->data(ui->tableView->selectionModel()->model()->index(ui->tableView->currentIndex().row(), idPrenom)).toString();
        QString discriminant = ui->tableView->currentIndex().model()->data(ui->tableView->selectionModel()->model()->index(ui->tableView->currentIndex().row(), idDiscriminant)).toString();
        QString login = ui->tableView->currentIndex().model()->data(ui->tableView->selectionModel()->model()->index(ui->tableView->currentIndex().row(), idLogin)).toString();
        QString email = ui->tableView->currentIndex().model()->data(ui->tableView->selectionModel()->model()->index(ui->tableView->currentIndex().row(), idEmail)).toString();
        QString mdp = ui->tableView->currentIndex().model()->data(ui->tableView->selectionModel()->model()->index(ui->tableView->currentIndex().row(), idMdp)).toString();

        if (strcmp(discriminant.toLatin1().data(), "A") == 0) {
            // Admin confirmed
            a->setNom(nom);
            a->setPrenom(prenom);
            a->setPseudo(login);
            a->setEmail(email);
            a->setEncodedMdp(mdp);

            editUserDlg.setAdmin(a);
        } else {
            // User confirmed
            u->setNom(nom);
            u->setPrenom(prenom);
            u->setEmail(email);
            u->setEncodedMdp(mdp);

            editUserDlg.setUser(u);
        }

        editUserDlg.setModal(true);
        editUserDlg.show();
    } else {
        QMessageBox::warning(this, "Sélectionner un utilisateur !", "Veuillez sélectionner un utilisateur avant de pouvoir l'éditer !!!");
    }
}

void MainWindow::addUser(User *u)
{
    QString req;
    QSqlQuery query;
    if (!sql->connexionBase()) {
        ui->tE_Info->append("Pas de connexion en cours à la base !");
    }

    req = "INSERT INTO users"\
          "(nom, prenom, email, mdp, login, discriminant)"\
          "VALUES ( '"\
            + u->getNom() + "', '"\
            + u->getPrenom() + "', '"\
            + u->getEmail() + "', '"\
            + u->getMdp() + "', '"\
            + u->getPseudo() + + "', '"\
            + u->getDiscriminant() + "');";
    qWarning("Requete = %s",req.toLatin1().data());
    if (!query.exec(req)) {
        ui->tE_Info->append(query.lastError().databaseText());
    } else {
        ui->tE_Info->append("Ajout de l'utilisateur réussi !");
        sql->db.commit();
        model->select();
    }
}

void MainWindow::addAdmin(Admin *a)
{
    QString req;
    QSqlQuery query;

    if (!sql->connexionBase()) {
        ui->tE_Info->append("Pas de connexion en cours à la base !");
    }

    req = "INSERT INTO users"\
          "(nom, prenom, email, mdp, login, discriminant)"\
          "VALUES ( '"\
            + a->getNom() + "', '"\
            + a->getPrenom() + "', '"\
            + a->getEmail() + "', '"\
            + a->getMdp() + "', '"\
            + a->getPseudo() + + "', '"\
            + a->getDiscriminant() + "');";
    qWarning("Requete = %s",req.toLatin1().data());
    if (!query.exec(req)) {
        ui->tE_Info->append(query.lastError().databaseText());
    } else {
        ui->tE_Info->append("Ajout de l'administrateur réussi !");
        sql->db.commit();
        model->select();
    }
}

void MainWindow::updateUser(User *u)
{
    QString req;
    QSqlQuery query;

    if (!sql->connexionBase()) {
        ui->tE_Info->append("Pas de connexion en cours à la base !");
    }

    req = "UPDATE " + sql->getTable() + " SET "\
          "nom = '" + u->getNom() + "',"\
          "prenom = '" + u->getPrenom() + "',"\
          "email = '" + u->getEmail() + "',"\
          "mdp = '" + u->getMdp() + "' ,"\
          "login = '" + u->getPseudo() + "' ,"\
          "discriminant = '" + u->getDiscriminant() + "' "\
          "WHERE id = " + QString::number(id);

    qWarning("Requete = %s",req.toLatin1().data());

    if (!query.exec(req)) {
        ui->tE_Info->append(query.lastError().databaseText());
    } else {
        ui->tE_Info->append("Edit de l'utilisateur réussi !");
        sql->db.commit();
        model->select();
    }
}

void MainWindow::updateAdmin(Admin *a)
{
    QString req;
    QSqlQuery query;

    if (!sql->connexionBase()) {
        ui->tE_Info->append("Pas de connexion en cours à la base !");
    }

    req = "UPDATE " + sql->getTable() + " SET "\
          "nom = '" + a->getNom() + "',"\
          "prenom = '" + a->getPrenom() + "',"\
          "email = '" + a->getEmail() + "',"\
          "mdp = '" + a->getMdp() + "' ,"\
          "login = '" + a->getPseudo() + "' ,"\
          "discriminant = '" + a->getDiscriminant() + "' "\
          "WHERE id = " + QString::number(id);

    qWarning("Requete = %s",req.toLatin1().data());

    if (!query.exec(req)) {
        ui->tE_Info->append(query.lastError().databaseText());
    } else {
        ui->tE_Info->append("Edit de l'administrateur réussi !");
        sql->db.commit();
        model->select();
    }
}

void MainWindow::detectUser()
{
    User *u = new User();
    u = addUserDlg.getUser();

    /// Maintenant, on essaie de l'ajouter à la BdD :
    addUser(u);
}

void MainWindow::detectAdmin()
{
    Admin *a = new Admin();
    a = addUserDlg.getAdmin();

    /// Maintenant, on essaie de l'ajouter à la BdD :
    addAdmin(a);
}

void MainWindow::editUser()
{
    User *u = new User();
    u = editUserDlg.getUser();

    updateUser(u);
}

void MainWindow::editAdmin()
{
    Admin *a = new Admin();
    a = editUserDlg.getAdmin();

    updateAdmin(a);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox box;
    QPushButton *btn1 = box.addButton("Oui", QMessageBox::YesRole);
    QPushButton *btn2 = box.addButton("Non", QMessageBox::NoRole);
    box.setIcon(QMessageBox::Warning);
    box.setWindowTitle("Quitter ?");
    box.setText("Etes-vous sûr de vouloir quitter l'application ?");
    box.exec();
    if (box.clickedButton() == btn1){
        event->accept();
        if (sql->getFlag()){
            sql->~SqlService();
        }
    } else if (box.clickedButton() == btn2)
        event->setAccepted(false);
}

void MainWindow::on_pB_Erase_clicked()
{
    ui->tE_Info->clear();
}

void MainWindow::on_pB_Delete_clicked()
{
    if (ui->tableView->selectionModel()->currentIndex().isValid()) {
        model->removeRow(ui->tableView->selectionModel()->currentIndex().row());
        model->select();
        ui->tableView->setModel(model);
        ui->tE_Info->append("Utilisateur supprimé avec succès !");
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner l'utilisateur à supprimé !");
    }
}
