#include "buscaTipoDadoHora.h"

buscaTipoDadoHora::buscaTipoDadoHora()
{
    QObject::connect (&(this->_campoHora), SIGNAL(timeChanged(const QTime&)), this, SLOT(timeChanged(const QTime&)));
    this->_campoHora.setDisplayFormat ("hh:mm");
    QTime t (this->_campoHora.time ());
    this->timeChanged (t);
}

bool buscaTipoDadoHora::comparaReg (QSqlQuery&, int) {
    return (false);
}

bool buscaTipoDadoHora::entradaValida () {
    return (true);
}

QWidget& buscaTipoDadoHora::campoEntrada() {
    return (this->_campoHora);
}

QString buscaTipoDadoHora::rotulo () {
    return ("Hora");
}

void buscaTipoDadoHora::timeChanged (const QTime& t) {
    this->_horario = t;
    this->_hora = t.hour();
    this->_minuto = t.minute();
}

QString buscaTipoDadoHora::texto () {
    return (this->_horario.toString("hh:mm"));
}

bool buscaTipoDadoHora::setTexto (QString texto) {
    QTime t (QTime::fromString (texto, "hh:mm"));
    if (t.isValid ()) {
        this->_campoHora.setTime (t);
        return (true);
    } else {
        return (false);
    }
}
