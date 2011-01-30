#ifndef BUSCATIPODADONUMERO_H
#define BUSCATIPODADONUMERO_H

#include "buscaTipoDadoTxt.h"

class buscaTipoDadoNumero : public buscaTipoDadoTxt
{
Q_OBJECT
public:
    bool comparaReg (QSqlQuery&, int);
    bool entradaValida ();
    QString rotulo ();
    QString texto ();
    bool setTexto (QString);
    buscaTipoDadoNumero ();
public slots:
    void _txtCaixaEntrada_textChanged (const QString&);
};

#endif // BUSCATIPODADONUMERO_H
