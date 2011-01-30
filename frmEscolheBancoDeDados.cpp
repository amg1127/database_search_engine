#include "frmEscolheBancoDeDados.h"
#include "ui_frmEscolheBancoDeDados.h"

QString frmEscolheBancoDeDados::remote_db_driver = "";
QString frmEscolheBancoDeDados::remote_db_hostname = "";
int frmEscolheBancoDeDados::remote_db_port = 0;
QString frmEscolheBancoDeDados::remote_db_user = "";
QString frmEscolheBancoDeDados::remote_db_database = "";
QString frmEscolheBancoDeDados::remote_db_options = "";

frmEscolheBancoDeDados::frmEscolheBancoDeDados(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEscolheBancoDeDados)
{
    this->_database = NULL;
    ui->setupUi(this);
    QStringList availableDrivers = QSqlDatabase::drivers();
    this->ui->cmbDriver->addItem ("(escolha...)");
    for (QStringList::Iterator i = availableDrivers.begin(); i != availableDrivers.end(); i++) {
        this->ui->cmbDriver->addItem (*i);
        if (this->remote_db_driver == (*i)) {
            this->ui->cmbDriver->setCurrentIndex (this->ui->cmbDriver->count() - 1);
        }
    }
    this->ui->txtHostName->setText (this->remote_db_hostname);
    this->ui->spnPort->setValue (this->remote_db_port);
    this->ui->txtUser->setText (this->remote_db_user);
    this->ui->txtDatabase->setText (this->remote_db_database);
    this->ui->txtOptions->setText (this->remote_db_options);
    QObject::connect (this->ui->btnTestConnection, SIGNAL(clicked()), this, SLOT(btnTestConnection_clicked()));
    QObject::connect (this->ui->btnOk, SIGNAL(clicked()), this, SLOT(btnOk_clicked()));
    QObject::connect (this->ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    
}

frmEscolheBancoDeDados::~frmEscolheBancoDeDados()
{
    if (this->_database != NULL) {
        QString sqldb_name (this->_database->connectionName ());
        if (this->_database->isOpen ()) {
            this->_database->close ();
        }
        delete (this->_database);
        this->_database = NULL;
        QSqlDatabase::removeDatabase (sqldb_name);
    }
    delete ui;
}

void frmEscolheBancoDeDados::btnOk_clicked () {
    this->configuraBancoDeDados ();
    if (this->_database != NULL) {
        this->remote_db_driver = this->ui->cmbDriver->currentText();
        this->remote_db_hostname = this->ui->txtHostName->text();
        this->remote_db_port = this->ui->spnPort->value();
        this->remote_db_user = this->ui->txtUser->text();
        this->remote_db_database = this->ui->txtDatabase->text();
        this->remote_db_options = this->ui->txtOptions->text();
        this->accept();
    }
}

void frmEscolheBancoDeDados::btnTestConnection_clicked () {
    this->configuraBancoDeDados ();
    if (this->_database != NULL) {
        QMessageBox::information (this, "Informação", "Base de dados conectada com sucesso!");
        this->_database->close ();
        QString sqldb_name (this->_database->connectionName ());
        delete (this->_database);
        this->_database = NULL;
        QSqlDatabase::removeDatabase (sqldb_name);
    }
}

void frmEscolheBancoDeDados::configuraBancoDeDados () {
    if (this->ui->cmbDriver->currentIndex() > 0) {
        QString sqldb_name ("connection_test");
        QSqlDatabase* sqldb = new QSqlDatabase (QSqlDatabase::addDatabase (this->ui->cmbDriver->currentText(), sqldb_name));
        if (sqldb->isValid()) {
            sqldb->setHostName (this->ui->txtHostName->text());
            sqldb->setPort (this->ui->spnPort->value());
            sqldb->setUserName (this->ui->txtUser->text());
            sqldb->setPassword (this->ui->txtPassword->text());
            sqldb->setDatabaseName (this->ui->txtDatabase->text());
            sqldb->setConnectOptions (this->ui->txtOptions->text());
            if (sqldb->open()) {
                this->_database = sqldb;
                return;
            } else {
                QSqlError e (sqldb->lastError());
                QMessageBox::warning (this, "Aviso", QString ("Falha ao abrir a conexão com o banco de dados\n\n") + e.driverText() + "\n" + QString::number (e.number()) + ": " + e.databaseText());
            }
        } else {
            QMessageBox::warning (this, "Aviso", "Driver de banco de dados é inválido ou não pôde ser carregado!");
        }
        delete (sqldb);
        QSqlDatabase::removeDatabase (sqldb_name);
    } else {
        QMessageBox::warning (this, "Aviso", "Escolha um driver de banco de dados!");
    }
}
