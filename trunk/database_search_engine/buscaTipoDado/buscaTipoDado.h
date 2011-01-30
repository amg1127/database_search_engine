#ifndef BUSCATIPODADO_H
#define BUSCATIPODADO_H

#include <QObject>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QWidget>

class buscaTipoDado : public QObject
{
Q_OBJECT
public:
    virtual bool comparaReg (QSqlQuery&, int) = 0;
    virtual bool entradaValida () = 0;
    virtual QWidget& campoEntrada() = 0;
    virtual QString rotulo () = 0;
    virtual QString texto () = 0;
    virtual bool setTexto (QString) = 0;
};

#endif // BUSCATIPODADO_H
