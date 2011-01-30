#ifndef BUSCATIPODADODATA_H
#define BUSCATIPODADODATA_H

#include <QDate>
#include <QDateEdit>

#include "buscaTipoDado.h"

class buscaTipoDadoData : public buscaTipoDado
{
Q_OBJECT
protected:
    QDateEdit _campoData;
    QDate _data;
    int _seculo;
    int _ano;
    int _ano4;
    int _mes;
    int _dia;
public:
    bool comparaReg (QSqlQuery&, int);
    bool entradaValida ();
    QWidget& campoEntrada();
    QString rotulo ();
    QString texto ();
    bool setTexto (QString);
    buscaTipoDadoData ();
public slots:
    void dateChanged (const QDate&);
};

#endif // BUSCATIPODADODATA_H
