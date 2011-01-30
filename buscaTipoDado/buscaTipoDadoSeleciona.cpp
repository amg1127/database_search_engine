#include "buscaTipoDadoSeleciona.h"

buscaTipoDadoSeleciona::buscaTipoDadoSeleciona()
{
    this->_campoEntrada.setEnabled (false);
}

bool buscaTipoDadoSeleciona::comparaReg (QSqlQuery&, int) {
    return (false);
}

bool buscaTipoDadoSeleciona::entradaValida () {
    return (false);
}

QWidget& buscaTipoDadoSeleciona::campoEntrada() {
    return (this->_campoEntrada);
}

QString buscaTipoDadoSeleciona::rotulo () {
    return ("(selecione...)");
}

QString buscaTipoDadoSeleciona::texto () {
    return (QString());
}

bool buscaTipoDadoSeleciona::setTexto (QString) {
    return (false);
}
