#ifndef ADDUSERDLG_H
#define ADDUSERDLG_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include "user.h"
#include "admin.h"

namespace Ui {
class AddUserDlg;
}

class AddUserDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDlg(QWidget *parent = nullptr);
    ~AddUserDlg();
    User *u;
    Admin *a;
    User* getUser();
    Admin* getAdmin();
    void setUser(User *);
    void setAdmin(Admin *);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_cB_Admin_stateChanged(int arg1);
    void on_pB_Cancel_clicked();
    void on_pB_Accept_clicked();

signals:
    void userDetected();
    void adminDetected();

private:
    Ui::AddUserDlg *ui;
    void clear();
    bool isValid();
    bool isSetted;
};

#endif // ADDUSERDLG_H
