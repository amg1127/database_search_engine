#ifndef BUSCATIPODADODATAHORASEG_H
#define BUSCATIPODADODATAHORASEG_H

#include <QDate>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QTime>

#include "buscaTipoDadoDataHora.h"

class buscaTipoDadoDataHoraSeg : public buscaTipoDadoDataHora
{
Q_OBJECT
protected:
    int _segundo;
public:
    bool comparaReg (QSqlQuery&, int);
    QString rotulo ();
    QString texto ();
    bool setTexto (QString);
    buscaTipoDadoDataHoraSeg();
public slots:
    void dateTimeChanged (const QDateTime&);
};

#endif // BUSCATIPODADODATAHORASEG_H
