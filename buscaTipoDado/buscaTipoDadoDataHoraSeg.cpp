#include "buscaTipoDadoDataHoraSeg.h"

buscaTipoDadoDataHoraSeg::buscaTipoDadoDataHoraSeg()
{
    QObject::connect (&(this->_campoDataHora), SIGNAL(dateTimeChanged(const QDateTime&)), this, SLOT(dateTimeChanged(const QDateTime&)));
    this->_campoDataHora.setDisplayFormat ("dd/MM/yyyy hh:mm:ss");
    QDateTime dt (this->_campoDataHora.dateTime ());
    this->dateTimeChanged (dt);
}

bool buscaTipoDadoDataHoraSeg::comparaReg (QSqlQuery&, int) {
    return (false);
}

QString buscaTipoDadoDataHoraSeg::rotulo () {
    return ("Data/Hora (+segundos)");
}

void buscaTipoDadoDataHoraSeg::dateTimeChanged (const QDateTime&) {
    this->_segundo = this->_horario.second();
}

QString buscaTipoDadoDataHoraSeg::texto () {
    return (this->_datahorario.toString("yyyy-MM-dd hh:mm:ss"));
}

bool buscaTipoDadoDataHoraSeg::setTexto (QString texto) {
    QDateTime dt (QDateTime::fromString (texto, "yyyy-MM-dd hh:mm:ss"));
    if (dt.isValid ()) {
        this->_campoDataHora.setDateTime (dt);
        return (true);
    } else {
        return (false);
    }
}
