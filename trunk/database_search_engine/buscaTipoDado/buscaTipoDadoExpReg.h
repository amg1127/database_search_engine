#ifndef BUSCATIPODADOEXPREG_H
#define BUSCATIPODADOEXPREG_H

#include <QRegExp>

#include "buscaTipoDadoString.h"

class buscaTipoDadoExpReg : public buscaTipoDadoString
{
Q_OBJECT
protected:
    QRegExp _expressaoregular;
public:
    bool comparaReg (QSqlQuery&, int);
    bool entradaValida ();
    QString rotulo ();
    buscaTipoDadoExpReg();
public slots:
    void _txtCaixaEntrada_textChanged (const QString&);
};

#endif // BUSCATIPODADOEXPREG_H
