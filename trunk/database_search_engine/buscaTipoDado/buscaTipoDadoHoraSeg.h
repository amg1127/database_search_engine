#ifndef BUSCATIPODADOHORASEG_H
#define BUSCATIPODADOHORASEG_H

#include <QTime>
#include <QTimeEdit>

#include "buscaTipoDadoHora.h"

class buscaTipoDadoHoraSeg : public buscaTipoDadoHora
{
Q_OBJECT
protected:
    int _segundo;
public:
    bool comparaReg (QSqlQuery&, int);
    QString rotulo ();
    QString texto ();
    bool setTexto (QString);
    buscaTipoDadoHoraSeg();
public slots:
    void timeChanged (const QTime&);
};

#endif // BUSCATIPODADOHORASEG_H
