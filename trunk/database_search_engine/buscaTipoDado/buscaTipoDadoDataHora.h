#ifndef BUSCATIPODADODATAHORA_H
#define BUSCATIPODADODATAHORA_H

#include <QDate>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QTime>

#include "buscaTipoDado.h"

class buscaTipoDadoDataHora : public buscaTipoDado
{
Q_OBJECT
protected:
    QDateTimeEdit _campoDataHora;
    QDateTime _datahorario;
    QDate _data;
    QTime _horario;
    int _seculo;
    int _ano;
    int _ano4;
    int _mes;
    int _dia;
    int _hora;
    int _minuto;
public:
    bool comparaReg (QSqlQuery&, int);
    bool entradaValida ();
    QWidget& campoEntrada();
    QString rotulo ();
    QString texto ();
    bool setTexto (QString);
    buscaTipoDadoDataHora();
public slots:
    void dateTimeChanged (const QDateTime&);
};

#endif // BUSCATIPODADODATAHORA_H
