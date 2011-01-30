#ifndef FRMESCOLHEBANCODEDADOS_H
#define FRMESCOLHEBANCODEDADOS_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>

namespace Ui {
    class frmEscolheBancoDeDados;
}

class frmEscolheBancoDeDados : public QDialog
{
    Q_OBJECT

public:
    explicit frmEscolheBancoDeDados(QWidget *parent = 0);
    ~frmEscolheBancoDeDados();
    inline QSqlDatabase& database () { return (*(this->_database)); }

private:
    QSqlDatabase* _database;
    Ui::frmEscolheBancoDeDados *ui;
    void configuraBancoDeDados ();
    static QString remote_db_driver;
    static QString remote_db_hostname;
    static int remote_db_port;
    static QString remote_db_user;
    static QString remote_db_database;
    static QString remote_db_options;

public slots:
    void btnOk_clicked ();
    void btnTestConnection_clicked ();
};

#endif // FRMESCOLHEBANCODEDADOS_H
