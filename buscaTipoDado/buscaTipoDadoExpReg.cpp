#include "buscaTipoDadoExpReg.h"

buscaTipoDadoExpReg::buscaTipoDadoExpReg()
{
    QObject::connect (&(this->_txtCaixaEntrada), SIGNAL(textChanged(const QString&)), this, SLOT(_txtCaixaEntrada_textChanged(const QString&)));
    QString txt (this->_txtCaixaEntrada.text());
    this->_txtCaixaEntrada_textChanged (txt);
}

bool buscaTipoDadoExpReg::comparaReg (QSqlQuery&, int) {
    return (false);
}

bool buscaTipoDadoExpReg::entradaValida () {
    return (this->_expressaoregular.isValid() && (! this->_expressaoregular.isEmpty()));
}

QString buscaTipoDadoExpReg::rotulo () {
    return ("Expressão Regular");
}

void buscaTipoDadoExpReg::_txtCaixaEntrada_textChanged (const QString& t) {
    this->_expressaoregular.setPattern (this->_string = t);
}
