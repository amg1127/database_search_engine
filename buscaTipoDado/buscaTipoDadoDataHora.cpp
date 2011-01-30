#include "buscaTipoDadoDataHora.h"

buscaTipoDadoDataHora::buscaTipoDadoDataHora()
{
    QObject::connect (&(this->_campoDataHora), SIGNAL(dateTimeChanged(const QDateTime&)), this, SLOT(dateTimeChanged(const QDateTime&)));
    this->_campoDataHora.setDisplayFormat ("dd/MM/yyyy hh:mm");
    QDateTime dt (this->_campoDataHora.dateTime ());
    this->dateTimeChanged (dt);
}

bool buscaTipoDadoDataHora::comparaReg (QSqlQuery&, int) {
    return (false);
}

bool buscaTipoDadoDataHora::entradaValida () {
    return (true);
}

QWidget& buscaTipoDadoDataHora::campoEntrada() {
    return (this->_campoDataHora);
}

QString buscaTipoDadoDataHora::rotulo () {
    return ("Data/Hora");
}

void buscaTipoDadoDataHora::dateTimeChanged (const QDateTime& dt) {
    this->_datahorario = dt;
    this->_data = dt.date();
    this->_horario = dt.time();
    this->_ano4 = this->_data.year();
    this->_seculo = this->_ano4 / 100;
    this->_ano = this->_ano4 % 100;
    this->_mes = this->_data.month();
    this->_dia = this->_data.day();
    this->_hora = this->_horario.hour();
    this->_minuto = this->_horario.minute();
}

QString buscaTipoDadoDataHora::texto () {
    return (this->_datahorario.toString("yyyy-MM-dd hh:mm"));
}

bool buscaTipoDadoDataHora::setTexto (QString texto) {
    QDateTime dt (QDateTime::fromString (texto, "yyyy-MM-dd hh:mm"));
    if (dt.isValid ()) {
        this->_campoDataHora.setDateTime (dt);
        return (true);
    } else {
        return (false);
    }
}
