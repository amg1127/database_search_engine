#include "buscaTipoDadoData.h"

buscaTipoDadoData::buscaTipoDadoData()
{
    QObject::connect (&(this->_campoData), SIGNAL(dateChanged(const QDate&)), this, SLOT(dateChanged(const QDate&)));
    this->_campoData.setDisplayFormat ("dd/MM/yyyy");
    QDate d (this->_campoData.date ());
    this->dateChanged (d);
}

bool buscaTipoDadoData::comparaReg (QSqlQuery&, int) {
    return (false);
}

bool buscaTipoDadoData::entradaValida () {
    return (true);
}

QWidget& buscaTipoDadoData::campoEntrada() {
    return (this->_campoData);
}

QString buscaTipoDadoData::rotulo () {
    return ("Data");
}

void buscaTipoDadoData::dateChanged (const QDate& d) {
    this->_data = d;
    this->_ano4 = d.year();
    this->_seculo = this->_ano4 / 100;
    this->_ano = this->_ano4 % 100;
    this->_mes = d.month();
    this->_dia = d.day();
}

QString buscaTipoDadoData::texto () {
    return (this->_data.toString("yyyy-MM-dd"));
}

bool buscaTipoDadoData::setTexto (QString texto) {
    QDate d (QDate::fromString (texto, "yyyy-MM-dd"));
    if (d.isValid ()) {
        this->_campoData.setDate (d);
        return (true);
    } else {
        return (false);
    }
}
