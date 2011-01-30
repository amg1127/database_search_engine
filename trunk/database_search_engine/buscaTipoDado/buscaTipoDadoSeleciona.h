#ifndef BUSCATIPODADOSELECIONA_H
#define BUSCATIPODADOSELECIONA_H

#include <QLineEdit>

#include "buscaTipoDado.h"

class buscaTipoDadoSeleciona : public buscaTipoDado
{
Q_OBJECT
private:
    QLineEdit _campoEntrada;
public:
    bool comparaReg (QSqlQuery&, int);
    bool entradaValida ();
    QWidget& campoEntrada();
    QString rotulo ();
    QString texto ();
    bool setTexto (QString);
    buscaTipoDadoSeleciona();
};

#endif // BUSCATIPODADOSELECIONA_H
