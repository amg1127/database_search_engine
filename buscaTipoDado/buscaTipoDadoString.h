#ifndef BUSCATIPODADOSTRING_H
#define BUSCATIPODADOSTRING_H

#include "buscaTipoDadoTxt.h"

class buscaTipoDadoString : public buscaTipoDadoTxt
{
Q_OBJECT
protected:
    QCheckBox _chkMatchCase;
    bool _matchCase;
public:
    bool comparaReg (QSqlQuery&, int);
    QString rotulo ();
    QString texto ();
    bool setTexto (QString);
    buscaTipoDadoString();
public slots:
    void _chkMatchCase_toggled (bool);
};

#endif // BUSCATIPODADOSTRING_H
