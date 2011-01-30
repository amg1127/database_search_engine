#include "buscaTipoDadoHoraSeg.h"

buscaTipoDadoHoraSeg::buscaTipoDadoHoraSeg()
{
    QObject::connect (&(this->_campoHora), SIGNAL(timeChanged(const QTime&)), this, SLOT(timeChanged(const QTime&)));
    this->_campoHora.setDisplayFormat ("hh:mm:ss");
    QTime t (this->_campoHora.time ());
    this->timeChanged (t);
}

bool buscaTipoDadoHoraSeg::comparaReg (QSqlQuery&, int) {
    return (false);
}

QString buscaTipoDadoHoraSeg::rotulo () {
    return ("Hora (+segundos)");
}

void buscaTipoDadoHoraSeg::timeChanged (const QTime&) {
    this->_segundo = this->_horario.second();
}

QString buscaTipoDadoHoraSeg::texto () {
    return (this->_horario.toString("hh:mm:ss"));
}

bool buscaTipoDadoHoraSeg::setTexto (QString texto) {
    QTime t (QTime::fromString (texto, "hh:mm:ss"));
    if (t.isValid ()) {
        this->_campoHora.setTime (t);
        return (true);
    } else {
        return (false);
    }
}
