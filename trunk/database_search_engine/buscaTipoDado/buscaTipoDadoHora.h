#ifndef BUSCATIPODADOHORA_H
#define BUSCATIPODADOHORA_H

#include <QTime>
#include <QTimeEdit>

#include "buscaTipoDado.h"

class buscaTipoDadoHora : public buscaTipoDado
{
Q_OBJECT
protected:
    QTimeEdit _campoHora;
    QTime _horario;
    int _hora;
    int _minuto;
public:
    bool comparaReg (QSqlQuery&, int);
    bool entradaValida ();
    QWidget& campoEntrada();
    QString rotulo ();
    QString texto ();
    bool setTexto (QString);
    buscaTipoDadoHora();
public slots:
    void timeChanged (const QTime&);
};

#endif // BUSCATIPODADOHORA_H
