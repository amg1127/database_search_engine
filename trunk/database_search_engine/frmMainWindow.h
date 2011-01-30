#ifndef FRMMAINWINDOW_H
#define FRMMAINWINDOW_H

#include <QDateEdit>
#include <QDateTimeEdit>
#include <QFileDialog>
#include <QLineEdit>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSpacerItem>
#include <QSpinBox>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QTableView>
#include <QTemporaryFile>
#include <QTimeEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "buscaTipoDado/buscaTipoDado.h"
#include "frmEscolheBancoDeDados.h"
#include "frmUmaTabela.h"

namespace Ui {
    class frmMainWindow;
}

class frmMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit frmMainWindow(QWidget *parent = 0);
    ~frmMainWindow();
private:
    Ui::frmMainWindow *ui;
    QList<buscaTipoDado*> tipoDadoEntrada;
    QList<frmUmaTabela*> tabelas_resultados;
    QVBoxLayout frmDadoEntradaLayout;
    QVBoxLayout frmResultadosLayout;
    QSpacerItem* frmResultadosEspacoFinal;
    static QString local_db_path;
    QMenu mnuPesquisasEmCache;
    QSqlDatabase obtemBaseDeDadosLocal ();
    bool imprimeErroSql (QSqlDatabase& teste, QString = QString("Falha durante a transação com o banco de dados!"));
    bool imprimeErroSql (QSqlQuery&, QString = QString("Falha durante a consulta no banco de dados!"));
    bool imprimeErroSql (QSqlError&, QString = QString("Erro na operação"));
    QString QVariant2SQLite (QVariant::Type);
    bool QVariant2SQLite (QSqlQuery&, QVariant, QString = QString());
    inline QString QVariant2Screen (QVariant v) { return (this->QVariant2Screen (v, v.type())); }
    QString QVariant2Screen (QVariant, QVariant::Type);
    bool QByteArrayCanBeQString (QByteArray);
    void exibirResultados (int);
    void limpaTabelasResultados ();
public slots:
    void cmbTipoDadoEntrada_currentIndexChanged (int);
    void notImplemented ();
    void btnDefinirArquivoTrab_clicked ();
    void btnImportarBaseDeDados_clicked ();
    void btnBuscar_clicked ();
    void mnuPesquisasEmCache_actiontriggered (bool);
    void listaResultadosEmCache ();
};

#endif // FRMMAINWINDOW_H
