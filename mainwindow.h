#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QAbstractButton>
#include <QSqlTableModel>
#include <QSqlRecord>

#include "adduserdlg.h"
#include "sqlservice.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pB_Add_clicked();
    void on_pB_Edit_clicked();

    void on_pB_Erase_clicked();

    void on_pB_Delete_clicked();

private:
    Ui::MainWindow *ui;
    AddUserDlg addUserDlg;
    AddUserDlg editUserDlg;
    SqlService *sql;
    QSqlTableModel *model;
    int id; // Va servir au update !
    /// CREATE
    void addUser(User *u);
    void addAdmin(Admin *a);
    /// UPDATE
    void updateUser(User *u);
    void updateAdmin(Admin *a);
    /// REMOVE

public slots:
    /// AddUserDlg
    void detectUser(void);
    void detectAdmin(void);
    /// EditUserDlg
    void editUser(void);
    void editAdmin(void);

protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
