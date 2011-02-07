#include "frmMainWindow.h"
#include "ui_frmMainWindow.h"

#include "buscaTipoDado/buscaTipoDadoSeleciona.h"
#include "buscaTipoDado/buscaTipoDadoData.h"
#include "buscaTipoDado/buscaTipoDadoDataHora.h"
#include "buscaTipoDado/buscaTipoDadoDataHoraSeg.h"
#include "buscaTipoDado/buscaTipoDadoExpReg.h"
#include "buscaTipoDado/buscaTipoDadoHora.h"
#include "buscaTipoDado/buscaTipoDadoHoraSeg.h"
#include "buscaTipoDado/buscaTipoDadoNumero.h"
#include "buscaTipoDado/buscaTipoDadoString.h"

QString frmMainWindow::local_db_path = "";

frmMainWindow::frmMainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmMainWindow)
{
    ui->setupUi(this);
    this->ui->frmDadoEntrada->setLayout (&(this->frmDadoEntradaLayout));
    this->ui->frmResultados->widget()->setLayout (&(this->frmResultadosLayout));
    this->frmResultadosEspacoFinal = new QSpacerItem (0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->tipoDadoEntrada
        << (new buscaTipoDadoSeleciona ())
        << (new buscaTipoDadoData ())
        << (new buscaTipoDadoDataHora ())
        << (new buscaTipoDadoDataHoraSeg ())
        << (new buscaTipoDadoHora ())
        << (new buscaTipoDadoHoraSeg ())
        << (new buscaTipoDadoExpReg ())
        << (new buscaTipoDadoNumero ())
        << (new buscaTipoDadoString ())
    ;
    for (QList<buscaTipoDado*>::iterator i = this->tipoDadoEntrada.begin(); i != this->tipoDadoEntrada.end(); i++) {
        static QStringList rotulos;
        if (rotulos.contains ((*i)->rotulo ())) {
            qFatal ("Tipo de dado repetido: '%s'!", (*i)->rotulo().toLocal8Bit().constData());
        } else {
            rotulos << (*i)->rotulo ();
        }
        this->ui->cmbTipoDadoEntrada->addItem ((*i)->rotulo());
        this->frmDadoEntradaLayout.addWidget (&((*i)->campoEntrada()));
        (*i)->campoEntrada().setVisible (false);
    }
    this->ui->cmbTipoDadoEntrada->setCurrentIndex (0);
    this->tipoDadoEntrada[0]->campoEntrada().setVisible (true);
    this->ui->btnPesquisas->setMenu (&(this->mnuPesquisasEmCache));
    QObject::connect (this->ui->cmbTipoDadoEntrada, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbTipoDadoEntrada_currentIndexChanged(int)));
    QObject::connect (this->ui->btnBuscar, SIGNAL(clicked()), this, SLOT(btnBuscar_clicked()));
    QObject::connect (this->ui->btnExportarResu, SIGNAL(clicked()), this, SLOT(notImplemented()));
    QObject::connect (this->ui->btnImportarBancoDeDados, SIGNAL(clicked()), this, SLOT(btnImportarBaseDeDados_clicked()));
    QObject::connect (this->ui->btnDefinirArquivoTrab, SIGNAL(clicked()), this, SLOT(btnDefinirArquivoTrab_clicked()));
    QObject::connect (&(this->mnuPesquisasEmCache), SIGNAL(aboutToShow()), this, SLOT(listaResultadosEmCache()));
}

frmMainWindow::~frmMainWindow()
{
    this->limpaTabelasResultados ();
    delete (this->frmResultadosEspacoFinal);
    QSqlDatabase dbsql = QSqlDatabase::database();
    if (dbsql.isValid ()) {
        dbsql.close ();
    }
    for (QList<buscaTipoDado*>::iterator i = this->tipoDadoEntrada.begin(); i != this->tipoDadoEntrada.end(); i++) {
        delete (*i);
    }
    delete ui;
}

void frmMainWindow::limpaTabelasResultados () {
    if (! this->tabelas_resultados.isEmpty()) {
        this->frmResultadosLayout.removeItem (this->frmResultadosEspacoFinal);
        do {
            delete (this->tabelas_resultados.takeLast ());
        } while (! this->tabelas_resultados.isEmpty());
    }
}

void frmMainWindow::cmbTipoDadoEntrada_currentIndexChanged (int index) {
    int pos;
    QList<buscaTipoDado*>::iterator i;
    for (pos = 0, i = this->tipoDadoEntrada.begin(); i != this->tipoDadoEntrada.end(); i++, pos++) {
        if (pos == index) {
            (*i)->campoEntrada().setVisible (true);
        } else {
            (*i)->campoEntrada().setVisible (false);
        }
    }
}

void frmMainWindow::notImplemented () {
    QMessageBox::critical (this, "Erro", "Comando não-implementado pelo programa!");
}

void frmMainWindow::btnImportarBaseDeDados_clicked () {
    frmEscolheBancoDeDados f;
    if (f.exec() == QDialog::Accepted) {
        this->limpaTabelasResultados ();
        QSqlDatabase local_db = this->obtemBaseDeDadosLocal ();
        if (local_db.isValid()) {
            if (local_db.open ()) {
                { QSqlQuery q("PRAGMA auto_vacuum = full;", local_db); }
                { QSqlQuery q("PRAGMA foreign_keys = false;", local_db); }
                QDateTime d_inicio (QDateTime::currentDateTime());
                // Limpar a base de dados local
                bool transaction_started = local_db.transaction ();
                if (! transaction_started) {
                    this->imprimeErroSql (local_db, "Erro iniciando mecanismo transacional do banco de dados local!");
                }
                QProgressDialog progress_d (this);
                QStringList tbls_loc = local_db.tables();
                QStringList tbls_rem = f.database().tables();
                tbls_loc.removeDuplicates ();
                tbls_rem.removeDuplicates ();
                progress_d.setMaximum (tbls_loc.count() + 4);
                progress_d.setAutoReset (false);
                progress_d.setAutoClose (false);
                progress_d.setMinimumDuration (0);
                progress_d.setCancelButtonText ("Cancelar");
                progress_d.open ();
                progress_d.setCursor (Qt::BusyCursor);
                int progress_value;
                int qts_tabelas_mesmo = 0;
                progress_d.setValue (progress_value = 0);
                for (QStringList::iterator i = tbls_loc.begin(); i != tbls_loc.end(); i++) {
                    progress_d.setValue (++progress_value);
                    progress_d.setLabelText (QString("Apagando tabela local '%1'...").arg(*i));
                    QSqlQuery q (QString("DROP TABLE %1;").arg(local_db.driver()->escapeIdentifier (*i, QSqlDriver::TableName)), local_db);
                    if (this->imprimeErroSql (q, QString("Erro apagando tabela local '%1'!").arg(*i))) {
                        progress_d.cancel ();
                        break;
                    }
                }
                if (! progress_d.wasCanceled ()) {
                    progress_d.setValue (++progress_value);
                    progress_d.setLabelText ("Criando tabela 'db_tables'...");
                    // Tabela que serve para guardar os nomes anteriores da tabela no banco original
                    QSqlQuery q (
                        "CREATE TABLE db_tables ("
                        "local_table_number INTEGER NOT NULL,"
                        "remote_table_name TEXT NOT NULL,"
                        "number_of_records INTEGER NOT NULL,"
                        "number_of_fields INTEGER NOT NULL,"
                        "PRIMARY KEY (local_table_number),"
                        "UNIQUE (remote_table_name));",
                        local_db);
                    if (this->imprimeErroSql (q, "Erro criando tabela local 'db_tables'!")) {
                        progress_d.cancel ();
                    } else {
                        q.clear ();
                        progress_d.setValue (++progress_value);
                        progress_d.setLabelText ("Criando tabela 'db_fields'...");
                        // Tabela que serve para guardar os nomes anteriores dos campos no banco original
                        QSqlQuery q (
                            "CREATE TABLE db_fields ("
                            "local_field_number INTEGER NOT NULL,"
                            "local_field_table INTEGER NOT NULL,"
                            "remote_field_name TEXT NOT NULL,"
                            "field_type INTEGER NOT NULL,"
                            "PRIMARY KEY (local_field_number, local_field_table),"
                            "FOREIGN KEY (local_field_table) REFERENCES db_tables (local_table_number) ON UPDATE RESTRICT ON DELETE CASCADE);",
                            local_db);
                        if (this->imprimeErroSql (q, "Erro criando tabela local 'db_fields'!")) {
                            progress_d.cancel ();
                        } else {
                            q.clear ();
                            progress_d.setValue (++progress_value);
                            progress_d.setLabelText ("Criando tabela 'searches'...");
                            // Tabela que serve para guardar o que ja foi buscado
                            QSqlQuery q (
                                "CREATE TABLE searches ("
                                "search_id INTEGER NOT NULL,"
                                "search_label TEXT NOT NULL,"
                                "search_text TEXT NOT NULL,"
                                "PRIMARY KEY (search_id),"
                                "UNIQUE (search_label, search_text));",
                                local_db);
                            if (this->imprimeErroSql (q, "Erro criando tabela local 'searches'!")) {
                                progress_d.cancel ();
                            } else {
                                q.clear ();
                                progress_d.setValue (++progress_value);
                                progress_d.setLabelText ("Criando tabela 'search_results'...");
                                // Tabela que serve para associar resultados de busca as tabelas originais
                                QSqlQuery q (
                                    "CREATE TABLE search_results ("
                                    "search_id INTEGER NOT NULL,"
                                    "search_table INTEGER NOT NULL,"
                                    "PRIMARY KEY (search_id, search_table),"
                                    "FOREIGN KEY (search_id) REFERENCES searches (search_id) ON UPDATE RESTRICT ON DELETE CASCADE,"
                                    "FOREIGN KEY (search_table) REFERENCES db_tables (local_table_number) ON UPDATE RESTRICT ON DELETE CASCADE);",
                                    local_db);
                                if (this->imprimeErroSql (q, "Erro criando tabela local 'search_results'!")) {
                                    progress_d.cancel ();
                                } else {
                                    q.clear ();
                                    int qts_tabelas = 0;
                                    int nregs_modu = 0;
                                    for (QStringList::iterator i = tbls_rem.begin(); i != tbls_rem.end(); i++, qts_tabelas++) {
                                        progress_d.setValue (0);
                                        progress_d.setMaximum (0);
                                        progress_d.setLabelText (QString("Analisando dados da tabela '%1'... [%2 de %3]").arg(*i).arg(qts_tabelas + 1).arg(tbls_rem.count()));
                                        QSqlRecord fields_rem (f.database().record (*i));
                                        if (! fields_rem.isEmpty()) {
                                            qts_tabelas_mesmo++;
                                            QStringList q_parcial;
                                            QStringList q_parcial_nomes;
                                            QStringList q_parcial_interrogs;
                                            QStringList q_parcial_fields;
                                            QStringList q_parcial_transform;
                                            // Alguns drivers transformam o tipo TEXT em BLOB.
                                            // Tentar detectar se a transformacao ocorreu e revertê-la
                                            QList<QVariant::Type> fields_vtype;
                                            QList<bool> blob_can_be_text;
                                            QString fld_def;
                                            int fi_l = fields_rem.count ();
                                            int fi = 0;
                                            for (fi = 0; fi < fi_l; fi++) {
                                                QSqlField fld (fields_rem.field(fi));
                                                QVariant::Type fld_vtype = fld.type();
                                                QString tipodado = this->QVariant2SQLite (fld_vtype);
                                                if (tipodado.isEmpty()) {
                                                    progress_d.cancel ();
                                                    break;
                                                }
                                                q_parcial_nomes.append (fld.name());
                                                fld_def = QString("field_%1 %2 NULL").arg(fi).arg(tipodado);
                                                q_parcial.append (fld_def);
                                                q_parcial_interrogs.append ("?");
                                                q_parcial_fields.append (QString("field_%1").arg(fi));
                                                fields_vtype << fld_vtype;
                                                if (fld_vtype == QVariant::ByteArray) {
                                                    blob_can_be_text.append (true);
                                                    q_parcial_transform.append (QString("field_%1 TEXT NULL").arg(fi));
                                                } else {
                                                    blob_can_be_text.append (false);
                                                    q_parcial_transform.append (fld_def);
                                                }
                                            }
                                            QCoreApplication::processEvents ();
                                            if (progress_d.wasCanceled ()) {
                                                break;
                                            }
                                            QSqlQuery q (QString("CREATE TABLE table_%1 (%2);").arg(qts_tabelas).arg(q_parcial.join(",")), local_db);
                                            if (this->imprimeErroSql (q, QString("Erro criando tabela local 'table_%1'!").arg(qts_tabelas))) {
                                                progress_d.cancel ();
                                            } else {
                                                q.clear ();
                                                QSqlTableModel dados_tabela (this, f.database());
                                                dados_tabela.setEditStrategy (QSqlTableModel::OnManualSubmit);
                                                dados_tabela.setTable (*i);
                                                if (dados_tabela.select ()) {
                                                    while (dados_tabela.canFetchMore ()) {
                                                        dados_tabela.fetchMore ();
                                                        QCoreApplication::processEvents ();
                                                        if (progress_d.wasCanceled ()) {
                                                            break;
                                                        }
                                                    }
                                                    progress_d.setLabelText (QString("Importando dados da tabela '%1'... [%2 de %3]").arg(*i).arg(qts_tabelas + 1).arg(tbls_rem.count()));
                                                    QCoreApplication::processEvents ();
                                                    if (progress_d.wasCanceled ()) {
                                                        break;
                                                    }
                                                    QString q_prep_stmt (QString("INSERT INTO table_%1 (%2) VALUES (%3);").arg(qts_tabelas).arg(q_parcial_fields.join(",")).arg(q_parcial_interrogs.join(", ")));
                                                    int nregs = dados_tabela.rowCount ();
                                                    if (nregs >= 0) {
                                                        progress_d.setMaximum (nregs);
                                                    }
                                                    for (int o_reg = 0; o_reg < nregs; o_reg++) {
                                                        QSqlRecord dado_tabela (dados_tabela.record (o_reg));
                                                        QSqlQuery qid (local_db);
                                                        if (qid.prepare (q_prep_stmt)) {
                                                            for (fi = 0; fi < fi_l; fi++) {
                                                                QVariant valu (dado_tabela.field(q_parcial_nomes[fi]).value());
                                                                if (! this->QVariant2SQLite (qid, valu)) {
                                                                    progress_d.cancel ();
                                                                    break;
                                                                }
                                                                if (blob_can_be_text[fi]) {
                                                                    if (! this->QByteArrayCanBeQString (valu.toByteArray ())) {
                                                                        blob_can_be_text[fi] = false;
                                                                        q_parcial_transform[fi] = q_parcial[fi];
                                                                    }
                                                                }
                                                            }
                                                            if (qid.exec ()) {
                                                                nregs_modu++;
                                                                if (nregs_modu >= 300) {
                                                                    nregs_modu = 0;
                                                                    if (nregs >= 0) {
                                                                        progress_d.setValue (o_reg);
                                                                        QCoreApplication::processEvents ();
                                                                        if (progress_d.wasCanceled ()) {
                                                                            break;
                                                                        }
                                                                    }
                                                                }
                                                            } else {
                                                                this->imprimeErroSql (qid, QString("Erro inserindo informações da tabela '%1' na tabela 'table_%2'!").arg(*i).arg(qts_tabelas));
                                                                progress_d.cancel ();
                                                                break;
                                                            }
                                                        } else {
                                                            this->imprimeErroSql (qid, QString("Erro preparando-se para inserir informações da tabela '%1' na tabela 'table_%2'!").arg(*i).arg(qts_tabelas));
                                                            progress_d.cancel ();
                                                            break;
                                                        }
                                                    }
                                                    QCoreApplication::processEvents ();
                                                    if (progress_d.wasCanceled ()) {
                                                        break;
                                                    }
                                                    if (nregs >= 0) {
                                                        progress_d.setValue (nregs);
                                                    }
                                                    // Putz... O SQLite nao suporta a instrucao SQL "ALTER TABLE ... ALTER COLUMN ..." !
                                                    // Para alterar o tipo de dado de "BLOB" para "TEXT", preciso criar uma nova tabela, copiar os dados e apagar a tabela antiga... Argh!
                                                    if (q_parcial != q_parcial_transform) {
                                                        QSqlQuery q (QString("CREATE TABLE table_%1_new (%2);").arg(qts_tabelas).arg(q_parcial_transform.join(",")), local_db);
                                                        if (this->imprimeErroSql (q, QString("Erro criando tabela local 'table_%1_new'!").arg(qts_tabelas))) {
                                                            progress_d.cancel ();
                                                        } else {
                                                            q.clear ();
                                                            QSqlQuery q (QString("INSERT INTO table_%1_new (%2) SELECT %2 FROM table_%1;").arg(qts_tabelas).arg(q_parcial_fields.join(",")), local_db);
                                                            if (this->imprimeErroSql (q, QString("Erro transferindo os dados da tabela local 'table_%1' para 'table_%1_new'!").arg(qts_tabelas))) {
                                                                progress_d.cancel ();
                                                            } else {
                                                                q.clear ();
                                                                QSqlQuery q (QString("DROP TABLE table_%1;").arg(qts_tabelas), local_db);
                                                                if (this->imprimeErroSql (q, QString("Erro removendo tabela local 'table_%1'!").arg(qts_tabelas))) {
                                                                    progress_d.cancel ();
                                                                } else {
                                                                    q.clear ();
                                                                    QSqlQuery q (QString("ALTER TABLE table_%1_new RENAME TO table_%1;").arg(qts_tabelas), local_db);
                                                                    if (this->imprimeErroSql (q, QString("Erro renomeando tabela local de 'table_%1_new' para 'table_%1'!").arg(qts_tabelas))) {
                                                                        progress_d.cancel ();
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    QCoreApplication::processEvents ();
                                                    if (progress_d.wasCanceled ()) {
                                                        break;
                                                    }
                                                    QSqlQuery q (local_db);
                                                    if (q.prepare ("INSERT INTO db_tables (local_table_number, remote_table_name, number_of_records, number_of_fields) VALUES (?, ?, ?, ?);")) {
                                                        q.addBindValue (qts_tabelas);
                                                        q.addBindValue (*i);
                                                        q.addBindValue (nregs);
                                                        q.addBindValue (fi_l);
                                                        if (q.exec ()) {
                                                            q.clear ();
                                                            for (fi = 0; fi < fi_l; fi++) {
                                                                QSqlField campo (fields_rem.field(fi));
                                                                QSqlQuery q (local_db);
                                                                QVariant::Type fld_vtype = campo.type();                                                                                                
                                                                if (q.prepare ("INSERT INTO db_fields (local_field_number, local_field_table, remote_field_name, field_type) VALUES (?, ?, ?, ?);")) {
                                                                    q.addBindValue (fi);
                                                                    q.addBindValue (qts_tabelas);
                                                                    q.addBindValue (campo.name());
                                                                    if (fld_vtype == QVariant::ByteArray && blob_can_be_text[fi]) {
                                                                        q.addBindValue ((int) QVariant::String);
                                                                    } else {
                                                                        q.addBindValue ((int) fld_vtype);
                                                                    }
                                                                    if (q.exec()) {
                                                                        q.clear ();
                                                                    } else {
                                                                        this->imprimeErroSql (q, QString("Erro inserindo informações do campo '%1' (%2) da tabela '%3' na tabela 'db_fields'!").arg(campo.name()).arg(campo.type()).arg(*i));
                                                                        progress_d.cancel ();
                                                                        break;
                                                                    }
                                                                } else {
                                                                    this->imprimeErroSql (q, QString("Erro preparando-se para inserir informações do campo '%1' (%2) da tabela '%3' na tabela 'db_fields'!").arg(campo.name()).arg(campo.type()).arg(*i));
                                                                    progress_d.cancel ();
                                                                    break;
                                                                }
                                                            }
                                                            QCoreApplication::processEvents ();
                                                            if (progress_d.wasCanceled ()) {
                                                                break;
                                                            }
                                                        } else {
                                                            this->imprimeErroSql (q, QString("Erro inserindo informações da tabela '%1' na tabela 'db_tables'!").arg(*i));
                                                            progress_d.cancel ();
                                                            break;
                                                        }
                                                    } else {
                                                        this->imprimeErroSql (q, QString("Erro preparando-se para inserir informações da tabela '%1' na tabela 'db_tables'!").arg(*i));
                                                        progress_d.cancel ();
                                                        break;
                                                    }
                                                } else {
                                                    QSqlError eroooo (dados_tabela.lastError());
                                                    this->imprimeErroSql (eroooo, QString("Erro obtendo dados da tabela '%1'!").arg(*i));
                                                    progress_d.cancel ();
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (progress_d.wasCanceled()) {
                    if (transaction_started) {
                        if (! local_db.rollback ()) {
                            this->imprimeErroSql (local_db, "Erro ao tentar reverter as operações que foram executadas na base de dados local!");
                        }
                    }
                    QMessageBox::warning (this, "Aviso", "A importação da base de dados foi cancelada!");
                } else {
                    progress_d.accept ();
                    bool ok = false;
                    if (transaction_started) {
                        if (local_db.commit ()) {
                            ok = true;
                        } else {
                            this->imprimeErroSql (local_db, "Erro ao salvar permanentemente o resultado das operações que foram executadas na base de dados local!");
                        }
                    } else {
                        ok = true;
                    }
                    if (ok) {
                        int ddiff = d_inicio.secsTo (QDateTime::currentDateTime());
                        int ddiff_m = (int) (ddiff / 60);
                        int ddiff_s = ddiff % 60;
                        QMessageBox::information (this, "Informação", QString("A importação da base de dados foi completada com sucesso!\n\nA operação demorou %1 minutos e %2 segundos e %3 tabelas foram importadas.").arg(ddiff_m).arg(ddiff_s).arg(qts_tabelas_mesmo));
                    }
                }
            } else {
                this->imprimeErroSql (local_db, "Falha ao abrir o banco de dados SQLite local!");
            }
        }
    }
}

void frmMainWindow::btnDefinirArquivoTrab_clicked () {
    QString selearq = QFileDialog::getSaveFileName (this, "Escolha um arquivo de trabalho", this->local_db_path, "Arquivo de dados SQLite (*.sqlite)");
    if (! selearq.isEmpty()) {
        this->limpaTabelasResultados ();
        this->local_db_path = selearq;
    }
}

QSqlDatabase frmMainWindow::obtemBaseDeDadosLocal () {
    QSqlDatabase dbsql = QSqlDatabase::database();
    if (! dbsql.isValid ()) {
        dbsql = QSqlDatabase::addDatabase ("QSQLITE");
    }
    if (dbsql.isValid ()) {
        if (this->local_db_path.isEmpty()) {
            QTemporaryFile tmpf (QDir::tempPath() + "/database-search-engine_XXXXXXX.sqlite");
            tmpf.setAutoRemove (false);
            if (tmpf.open()) {
                this->local_db_path = tmpf.fileName();
                tmpf.close ();
            } else if (QMessageBox::warning (this, "Aviso", "Impossível criar arquivo temporário no sistema para utilizá-lo como de trabalho!\n\nEscolha um arquivo de trabalho alternativo!", QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok) {
                this->btnDefinirArquivoTrab_clicked ();
                return (this->obtemBaseDeDadosLocal ());
            }
        }
        dbsql.setDatabaseName (this->local_db_path);
    } else {
        QMessageBox::critical (this, "Erro", "Impossível carregar drive de banco de dados SQLITE!");
    }
    return (dbsql);
}

bool frmMainWindow::imprimeErroSql (QSqlDatabase& sql_db, QString cabec) {
    QSqlError e (sql_db.lastError());
    return (this->imprimeErroSql (e, cabec));
}

bool frmMainWindow::imprimeErroSql (QSqlQuery& sql_q, QString cabec) {
    QSqlError e (sql_q.lastError());
    return (this->imprimeErroSql (e, cabec));
}

bool frmMainWindow::imprimeErroSql (QSqlError& sql_err, QString cabec) {
    if (sql_err.isValid ()) {
        QMessageBox::critical (this, "Erro", cabec + "\n\n" + sql_err.driverText() + "\n" + QString::number (sql_err.number()) + ": " + sql_err.databaseText());
        return (true);
    } else {
        return (false);
    }
}

QString frmMainWindow::QVariant2SQLite (QVariant::Type vtype) {
    switch (vtype) {
        case QVariant::Bool:
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
            return ("INTEGER");
        case QVariant::Double:
            return ("REAL");
        case QVariant::Date:
        case QVariant::DateTime:
        case QVariant::Time:
            return ("TEXT");
        case QVariant::String:
            return ("TEXT");
        case QVariant::ByteArray:
            return ("BLOB");
        default:
            QMessageBox::critical (this, "Erro", QString("QVariant::Type ") + QString::number (vtype) + " não pode ser armazenado no SQLite!");
            return (QString());
    }
}

bool frmMainWindow::QVariant2SQLite (QSqlQuery& query, QVariant value, QString placeholder) {
    QVariant realValue;
    switch (value.type()) {
        case QVariant::Bool:
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QVariant::Double:
        case QVariant::String:
        case QVariant::ByteArray:
            realValue = value; break;
        case QVariant::Date:
            realValue = value.toDate().toString("yyyy-MM-dd"); break;
        case QVariant::DateTime:
            realValue = value.toDateTime().toString("yyyy-MM-dd hh:mm:ss"); break;
        case QVariant::Time:
            realValue = value.toTime().toString("hh:mm:ss"); break;
        default:
            QMessageBox::critical (this, "Erro", QString("QVariant::Type ") + QString::number (value.type()) + " não pode ser convertido para o SQLite!");
            return (false);
    }
    if (placeholder.isEmpty()) {
        query.addBindValue (realValue);
    } else {
        query.bindValue (placeholder, realValue);
    }
    return (true);
}

QString frmMainWindow::QVariant2Screen (QVariant value, QVariant::Type type) {
    switch (type) {
        case QVariant::Bool:
            if (value.toBool()) {
                return ("true");
            } else {
                return ("false");
            }
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
        case QVariant::Double:
        case QVariant::String:
            return (value.toString());
        case QVariant::ByteArray:
            return ("(BLOB)");
        case QVariant::Date:
            return (value.toDate().toString("dd/MM/yyyy"));
        case QVariant::DateTime:
            {
                QDateTime d (value.toDateTime());
                if (d.time().second() > 0) {
                    return (d.toString("dd/MM/yyyy hh:mm:ss"));
                } else {
                    return (d.toString("dd/MM/yyyy hh:mm"));
                }
            }
        case QVariant::Time:
            {
                QTime t (value.toTime());
                if (t.second() > 0) {
                    return (t.toString("hh:mm:ss"));
                } else {
                    return (t.toString("hh:mm"));
                }
            }
        default:
            QMessageBox::critical (this, "Erro", QString("QVariant::Type ") + QString::number (type) + " não pode ser convertido para a tela!");
            return (QString());
    }
}

void frmMainWindow::exibirResultados (int search_id) {
    this->limpaTabelasResultados ();
    QSqlDatabase local_db = this->obtemBaseDeDadosLocal ();
    if (local_db.isValid ()) {
        if (local_db.open ()) {
                /*
            db_tables (local_table_number INTEGER, remote_table_name TEXT, number_of_records INTEGER, number_of_fields INTEGER)
            db_fields (local_field_number INTEGER, local_field_table INTEGER, remote_field_name TEXT, field_type INTEGER)
            searches (search_id INTEGER, search_label TEXT, search_text TEXT)
            search_results (search_id INTEGER, search_table INTEGER)
                */
            int i, l;
            QSqlQuery q (local_db);
            q.setForwardOnly (true);
            if (q.prepare ("SELECT DBT.local_table_number, DBT.number_of_fields, DBT.remote_table_name FROM search_results AS SR INNER JOIN db_tables AS DBT ON (SR.search_table=DBT.local_table_number) WHERE SR.search_id=?;")) {
                q.addBindValue (search_id);
                if (q.exec ()) {
                    frmUmaTabela* ntab;
                    while (q.next ()) {
                        int table_id = q.value(0).toInt();
                        int number_of_fields = q.value(1).toInt();
                        QString table_name = q.value(2).toString();
                        ntab = new frmUmaTabela (this->ui->frmResultados->widget());
                        this->frmResultadosLayout.addWidget (ntab);
                        ntab->show ();
                        this->tabelas_resultados << ntab;
                        ntab->setNomeTabela (table_name);
                        QMap<int,QVariant::Type> tiposCamps;
                        QMap<int,QString> nomesCamps;
                        QSqlQuery buscaNomesETipos (local_db);
                        buscaNomesETipos.setForwardOnly (true);
                        if (buscaNomesETipos.exec (QString("SELECT local_field_number, field_type, remote_field_name FROM db_fields WHERE local_field_table=%1;").arg(table_id))) {
                            while (buscaNomesETipos.next ()) {
                                i = buscaNomesETipos.value(0).toInt();
                                tiposCamps[i] = (QVariant::Type) buscaNomesETipos.value(1).toInt();
                                nomesCamps[i] = buscaNomesETipos.value(2).toString();
                            }
                            QSqlQuery buscaDados (local_db);
                            buscaDados.setForwardOnly (true);
                            QString pedeFields("");
                            for (i = 0; i < number_of_fields; i++) {
                                if (! pedeFields.isEmpty()) {
                                    pedeFields += ", ";
                                }
                                pedeFields += QString("field_%1").arg(i);
                                if (! tiposCamps.contains (i)) {
                                    // Isso realmente não deveria acontecer!
                                    QMessageBox::critical (this, "Erro", QString("As informações sobre o campo 'field_%1' da tabela 'table_%2' nao foram localizadas na base de dados local!").arg(i).arg(table_id));
                                    break;
                                }
                            }
                            if (i != number_of_fields) {
                                // Eu repito: isso nao deveria acontecer!
                                break;
                            }
                            if (buscaDados.exec (QString("SELECT ") + pedeFields + QString(" FROM table_%1_search_%2;").arg(table_id).arg(search_id))) {
                                l = 0;
                                ntab->criaTabela (number_of_fields);
                                while (buscaDados.next()) {
                                    ntab->adicionaLinha ();
                                    for (i = 0; i < number_of_fields; i++) {
                                        ntab->celula(l, i)->setText (this->QVariant2Screen (buscaDados.value(i), tiposCamps[i]));
                                    }
                                    l++;
                                }
                                if (l > 0) {
                                    for (i = 0; i < number_of_fields; i++) {
                                        ntab->coluna(i)->setText (nomesCamps[i]);
                                    }
                                }
                            } else {
                                this->imprimeErroSql (buscaDados, QString("Falha ao buscar os dados de resultados de busca da tabela '%1'!").arg(table_name));
                                break;
                            }
                        } else {
                            this->imprimeErroSql (buscaNomesETipos, QString("Falha ao buscar informações sobre os campos dos dados de resultados de busca da tabela '%1'!").arg(table_name));
                            break;
                        }
                    }
                    if (! this->tabelas_resultados.isEmpty ()) {
                        this->frmResultadosLayout.addSpacerItem (this->frmResultadosEspacoFinal);
                    }
                } else {
                    this->imprimeErroSql (q, "Falha ao executar a consulta de enumeração de tabelas de resultados de busca em cache!");
                }
            } else {
                this->imprimeErroSql (q, "Falha ao preparar a consulta de enumeração de tabelas de resultados de busca em cache!");
            }
        } else {
            this->imprimeErroSql (local_db, "Falha ao abrir o banco de dados SQLite local!");
        }
    }    
}

void frmMainWindow::mnuPesquisasEmCache_actiontriggered (bool) {
    QAction* acao = qobject_cast<QAction*> (QObject::sender ());
    if (acao != NULL) {
        QString search_label (acao->data().toString());
        int i, l = this->ui->cmbTipoDadoEntrada->count ();
        for (i = 0; i < l; i++) {
            if (this->ui->cmbTipoDadoEntrada->itemText(i) == search_label) {
                break;
            }
        }
        if (i < l) {
            this->ui->cmbTipoDadoEntrada->setCurrentIndex (i);
            if (this->tipoDadoEntrada[i]->setTexto (acao->text())) {
                this->btnBuscar_clicked ();
            } else {
                QMessageBox::critical (this, "Erro", QString("Impossível definir o dado para buscar!\n\nEsse é um erro que não deveria acontecer. Por favor, reimporte a base de dados original com um clique no botão '%1'.").arg(this->ui->btnImportarBancoDeDados->text()));
            }
        } else {
            QMessageBox::critical (this, "Erro", QString("Impossível selecionar tipo de dado de busca!\n\nEsse é um erro que não deveria acontecer. Por favor, redefina o caminho do arquivo de trabalho com um clique no botão '%1'.").arg(this->ui->btnDefinirArquivoTrab->text()));
        }
    }
}

void frmMainWindow::listaResultadosEmCache () {
    QSqlDatabase local_db = this->obtemBaseDeDadosLocal ();
    if (local_db.isValid ()) {
        if (local_db.open ()) {
            this->mnuPesquisasEmCache.clear ();
            bool cleared = true;
            for (QList<buscaTipoDado*>::iterator i = this->tipoDadoEntrada.begin(); i != this->tipoDadoEntrada.end(); i++) {
                QSqlQuery q (local_db);
                q.setForwardOnly (true);
                if (q.prepare ("SELECT search_text FROM searches WHERE search_label=? ORDER BY search_text;")) {
                    q.addBindValue ((*i)->rotulo());
                    if (q.exec ()) {
                        if (q.next ()) {
                            QMenu* submenu = this->mnuPesquisasEmCache.addMenu ((*i)->rotulo());
                            cleared = false;
                            do {
                                QAction* acao = submenu->addAction (q.value(0).toString(), this, SLOT(mnuPesquisasEmCache_actiontriggered(bool)));
                                acao->setData ((*i)->rotulo());
                            } while (q.next ());
                        }
                    } else {
                        this->imprimeErroSql (q, "Falha ao executar a consulta de enumeração de resultados de busca em cache!");
                        break;
                    }
                } else {
                    this->imprimeErroSql (q, "Falha ao preparar a consulta de enumeração de resultados de busca em cache!");
                    break;
                }
            }
            if (cleared) {
                QAction* nothing = this->mnuPesquisasEmCache.addAction ("Nenhum resultado em cache...");
                nothing->setEnabled (false);
            }
        } else {
            this->imprimeErroSql (local_db, "Falha ao abrir o banco de dados SQLite local!");
        }
    }
}

void frmMainWindow::btnBuscar_clicked () {
    buscaTipoDado* criterio_alvo = this->tipoDadoEntrada[this->ui->cmbTipoDadoEntrada->currentIndex()];
    if (criterio_alvo->entradaValida()) {
        QSqlDatabase local_db = this->obtemBaseDeDadosLocal ();
        if (local_db.isValid()) {
            if (local_db.open ()) {
                /*
        db_tables (local_table_number INTEGER, remote_table_name TEXT, number_of_records INTEGER, number_of_fields INTEGER)
        db_fields (local_field_number INTEGER, local_field_table INTEGER, remote_field_name TEXT, field_type INTEGER)
        searches (search_id INTEGER, search_label TEXT, search_text TEXT)
        search_results (search_id INTEGER, search_table INTEGER)
                */
                // Verificar se o critério de busca já foi buscado antes. Em caso afirmativo, não é necessário buscar tudo de novo...
                QSqlQuery q (local_db);
                q.setForwardOnly (true);
                if (q.prepare ("SELECT search_id FROM searches WHERE search_label=? AND search_text=?;")) {
                    q.addBindValue (criterio_alvo->rotulo());
                    q.addBindValue (criterio_alvo->texto());
                    if (q.exec ()) {
                        if (q.next ()) {
                            int search_id = q.value(0).toInt();
                            q.clear ();
                            this->exibirResultados (search_id);
                        } else {
                            // O resultado não existe no cache. É preciso montar...
                            q.clear ();
                            int new_search_id;
                            bool transaction_started = local_db.transaction ();
                            if (! transaction_started) {
                                this->imprimeErroSql (local_db, "Erro iniciando mecanismo transacional do banco de dados local!");
                            }
                            bool can_commit = false;
                            QProgressDialog progress_d (this);
                            progress_d.setAutoReset (false);
                            progress_d.setAutoClose (false);
                            progress_d.setMinimumDuration (0);
                            progress_d.setCancelButtonText ("Cancelar");
                            progress_d.open ();
                            progress_d.setCursor (Qt::BusyCursor);                           
                            QSqlQuery q_new_search_id (local_db);
                            // Boa dica sobre campos auto-incrementáveis do SQLite: http://www.sqlite.org/autoinc.html
                            if (q_new_search_id.prepare ("INSERT INTO searches (search_label, search_text) values (?, ?);")) {
                                q_new_search_id.addBindValue (criterio_alvo->rotulo());
                                q_new_search_id.addBindValue (criterio_alvo->texto());
                                if (q_new_search_id.exec ()) {
                                    QVariant last_insert_id (q_new_search_id.lastInsertId ());
                                    if (last_insert_id.isValid ()) {
                                        new_search_id = last_insert_id.toInt ();
                                        can_commit = true;
                                    } else {
                                        QMessageBox::critical (this, "Erro", "Erro interno durante a transação com o banco de dados local: impossível determinar código do último valor inserido na tabela 'searches'!");
                                        progress_d.cancel ();
                                    }
                                } else {
                                    this->imprimeErroSql (q_new_search_id, "Erro ao executar o armazenamento dos parametros de busca em cache!");
                                    progress_d.cancel ();
                                }
                            } else {
                                this->imprimeErroSql (q_new_search_id, "Erro ao preparar o armazenamento dos parametros de busca em cache!");
                                progress_d.cancel ();
                            }
                            if (can_commit) {
                                QSqlQuery lista_tabelas (local_db);
                                lista_tabelas.setForwardOnly (true);                            
                                if (lista_tabelas.exec ("SELECT local_table_number, number_of_records, remote_table_name FROM db_tables;")) {
                                    QList<int> numeros_tabelas;
                                    QList<int> quants_registros;
                                    QStringList table_names;
                                    int numero_tabela;
                                    int quant_registros;
                                    QString table_name;
                                    int i;
                                    int fld_num, fld_type;
                                    while (lista_tabelas.next ()) {
                                        numeros_tabelas << lista_tabelas.value(0).toInt();
                                        quants_registros << lista_tabelas.value(1).toInt();
                                        table_names << lista_tabelas.value(2).toString();
                                    }
                                    int cnt_tables = numeros_tabelas.count();
                                    int qts_tabelas = 0;
                                    int bus_modu = 0;
                                    int bus_regs;
                                    lista_tabelas.clear ();
                                    while (can_commit && (! numeros_tabelas.isEmpty ())) {
                                        numero_tabela = numeros_tabelas.takeFirst ();
                                        quant_registros = quants_registros.takeFirst ();
                                        table_name = table_names.takeFirst ();
                                        progress_d.setMaximum (quant_registros);
                                        progress_d.setValue (0);
                                        progress_d.setLabelText (QString("Pesquisando a tabela '%1'... [%2 de %3]").arg(table_name).arg(++qts_tabelas).arg(cnt_tables));
                                        QSqlQuery lista_campos (local_db);
                                        lista_campos.setForwardOnly (true);
                                        if (lista_campos.exec (QString("SELECT local_field_number, field_type FROM db_fields WHERE local_field_table = %1 ORDER BY local_field_number ASC;").arg(numero_tabela))) {
                                            QStringList def_campos;
                                            QStringList non_blob_fields, blob_fields, fields_placeholders;
                                            QString def_campo;
                                            while (can_commit && lista_campos.next ()) {
                                                fld_num = lista_campos.value(0).toInt();
                                                fld_type = lista_campos.value(1).toInt();
                                                def_campo = this->QVariant2SQLite ((QVariant::Type) fld_type);
                                                if (def_campo.isEmpty ()) {
                                                    // Isso realmente nao deveria acontecer, mas...
                                                    can_commit = false;
                                                    progress_d.cancel ();
                                                    break;
                                                } else {
                                                    def_campos << QString("field_%1 %2 NULL").arg(fld_num).arg(def_campo);
                                                    fields_placeholders << "?";
                                                    if (def_campo.compare ("BLOB", Qt::CaseInsensitive)) {
                                                        non_blob_fields << QString("field_%1").arg(fld_num);
                                                    } else {
                                                        blob_fields << QString("field_%1").arg(fld_num);
                                                    }
                                                }
                                            }
                                            if (! can_commit) {
                                                progress_d.cancel ();
                                                break;
                                            }
                                            int number_of_non_blob_fields = non_blob_fields.count ();
                                            if (number_of_non_blob_fields < 1) {
                                                continue;
                                            }
                                            int number_of_fields = number_of_non_blob_fields + blob_fields.count ();
                                            QString q_str((non_blob_fields + blob_fields).join(", "));
                                            QString q_str_placeholders (fields_placeholders.join(", "));
                                            QSqlQuery uma_tabela (local_db);
                                            uma_tabela.setForwardOnly (true);
                                            if (uma_tabela.exec (QString("SELECT ") + q_str + QString(" FROM table_%1;").arg(numero_tabela))) {
                                                bool must_create_new_table = true;
                                                bus_regs = 0;
                                                while (can_commit && uma_tabela.next ()) {
                                                    bus_regs++;
                                                    bus_modu++;
                                                    if (bus_modu >= 1000) {
                                                        bus_modu = 0;
                                                        progress_d.setValue (bus_regs);
                                                        QCoreApplication::processEvents ();
                                                        if (progress_d.wasCanceled ()) {
                                                            can_commit = false;
                                                            break;
                                                        }
                                                    }
                                                    if (criterio_alvo->comparaReg (uma_tabela, number_of_non_blob_fields)) {
                                                        // O registro coincide com o criterio. Se a tabela de resultados nao foi criada ainda, criar agora.
                                                        if (must_create_new_table) {
                                                            QSqlQuery nova_tabela (QString("CREATE TABLE table_%1_search_%2 (%3);").arg(numero_tabela).arg(new_search_id).arg(def_campos.join(", ")), local_db);
                                                            if (this->imprimeErroSql (nova_tabela, QString("Falha ao criar tabela 'table_%1_search_%2'!").arg(numero_tabela).arg(new_search_id))) {
                                                                can_commit = false;
                                                                progress_d.cancel ();
                                                                break;
                                                            } else {
                                                                nova_tabela.clear ();
                                                                QSqlQuery grav_nova_tabela (local_db);
                                                                if (grav_nova_tabela.prepare("INSERT INTO search_results (search_id, search_table) VALUES (?, ?);")) {
                                                                    grav_nova_tabela.addBindValue (new_search_id);
                                                                    grav_nova_tabela.addBindValue (numero_tabela);
                                                                    if (grav_nova_tabela.exec ()) {
                                                                        must_create_new_table = false;
                                                                    } else {
                                                                        this->imprimeErroSql (grav_nova_tabela, "Falha ao executar o armazenamento do relacionamento busca <=> tabela!");
                                                                        can_commit = false;
                                                                        progress_d.cancel ();
                                                                        break;
                                                                    }
                                                                } else {
                                                                    this->imprimeErroSql (grav_nova_tabela, "Falha ao preparar o armazenamento do relacionamento busca <=> tabela!");
                                                                    can_commit = false;
                                                                    progress_d.cancel ();
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        if (must_create_new_table) {
                                                            can_commit = false;
                                                            progress_d.cancel ();
                                                            break;
                                                        }
                                                        #warning Questionar o usuario quando ha muitos resultados...
                                                        QSqlQuery novo_registro (local_db);
                                                        if (novo_registro.prepare (QString("INSERT INTO table_%1_search_%2 (%3) VALUES (%4);").arg(numero_tabela).arg(new_search_id).arg(q_str).arg(q_str_placeholders))) {
                                                            for (i = 0; i < number_of_fields; i++) {
                                                                novo_registro.addBindValue (uma_tabela.value (i));
                                                            }
                                                            if (! novo_registro.exec ()) {
                                                                this->imprimeErroSql (novo_registro, "Falha ao executar o armazenamento do resultado de busca!");
                                                                can_commit = false;
                                                                progress_d.cancel ();
                                                                break;
                                                            }
                                                        } else {
                                                            this->imprimeErroSql (novo_registro, "Falha ao preparar o armazenamento do resultado de busca!");
                                                            can_commit = false;
                                                            progress_d.cancel ();
                                                            break;
                                                        }
                                                    }
                                                }
                                                QCoreApplication::processEvents ();
                                                if (progress_d.wasCanceled ()) {
                                                    can_commit = false;
                                                    break;
                                                }
                                                if (! can_commit) {
                                                    progress_d.cancel ();
                                                    break;
                                                }
                                                progress_d.setValue (bus_regs);
                                                uma_tabela.clear ();
                                            } else {
                                                this->imprimeErroSql (uma_tabela, QString("Falha ao buscar os dados da tabela 'table_%1'!").arg(numero_tabela));
                                                can_commit = false;
                                                progress_d.cancel ();
                                                break;
                                            }
                                        } else {
                                            this->imprimeErroSql (lista_campos, QString("Falha ao buscar informações sobre os campos da tabela 'table_%1'!").arg(numero_tabela));
                                            can_commit = false;
                                            progress_d.cancel ();
                                            break;
                                        }
                                    }
                                } else {
                                    this->imprimeErroSql (lista_tabelas, "Falha ao identificar as tabelas importadas!");
                                    progress_d.cancel ();
                                    can_commit = false;
                                }
                            }
                            QCoreApplication::processEvents ();
                            if (progress_d.wasCanceled ()) {
                                can_commit = false;
                            }
                            if (can_commit) {                            
                                progress_d.accept ();
                                bool ok = false;
                                if (transaction_started) {
                                    if (local_db.commit ()) {
                                        ok = true;
                                    } else {
                                        this->imprimeErroSql (local_db, "Erro ao salvar permanentemente o resultado das operações que foram executadas na base de dados local!");
                                    }
                                } else {
                                    ok = true;
                                }
                                if (ok) {
                                    this->exibirResultados (new_search_id);
                                }
                            } else {
                                progress_d.cancel ();
                                if (transaction_started) {
                                    if (! local_db.rollback ()) {
                                        this->imprimeErroSql (local_db, "Erro ao tentar reverter as operações que foram executadas na base de dados local!");
                                    }
                                }
                                QMessageBox::warning (this, "Aviso", "A pesquisa na base de dados não foi bem sucedida ou foi cancelada!");
                            }
                        }
                    } else {
                        this->imprimeErroSql (q, "Falha ao executar a consulta de localizacao de resultados de busca em cache!");
                    }
                } else {
                    this->imprimeErroSql (q, "Falha ao preparar a consulta de localizacao de resultados de busca em cache!");
                }
            } else {
                this->imprimeErroSql (local_db, "Falha ao abrir o banco de dados SQLite local!");
            }
        }
    } else {
         QMessageBox::warning (this, "Aviso", "O parâmetro de busca é inválido!");
    }
}

bool frmMainWindow::QByteArrayCanBeQString (QByteArray v) {
    int i, l;
    l = v.size ();
    for (i = 0; i < 64 && i < l; i++) {
        QChar ch(v[i]);
        if (ch != '\n' && ch != '\r' && ch != '\t') {
            switch (ch.category()) {
                case QChar::Letter_Lowercase:
                case QChar::Letter_Modifier:
                case QChar::Letter_Other:
                case QChar::Letter_Titlecase:
                case QChar::Letter_Uppercase:
                case QChar::Mark_Enclosing:
                case QChar::Mark_NonSpacing:
                case QChar::Mark_SpacingCombining:
                case QChar::Number_DecimalDigit:
                case QChar::Number_Letter:
                case QChar::Number_Other:
                case QChar::Punctuation_Close:
                case QChar::Punctuation_Connector:
                case QChar::Punctuation_Dash:
                case QChar::Punctuation_FinalQuote:
                case QChar::Punctuation_InitialQuote:
                case QChar::Punctuation_Open:
                case QChar::Punctuation_Other:
                case QChar::Separator_Line:
                case QChar::Separator_Paragraph:
                case QChar::Separator_Space:
                case QChar::Symbol_Currency:
                case QChar::Symbol_Math:
                case QChar::Symbol_Modifier:
                case QChar::Symbol_Other:
                    break;
                default:
                    return (false);
            }
        }
    }
    return (true);
}
