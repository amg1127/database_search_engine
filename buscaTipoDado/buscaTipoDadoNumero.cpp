#include "buscaTipoDadoNumero.h"

buscaTipoDadoNumero::buscaTipoDadoNumero () {
    QObject::connect (&(this->_txtCaixaEntrada), SIGNAL(textChanged(const QString&)), this, SLOT(_txtCaixaEntrada_textChanged(const QString&)));
}

bool buscaTipoDadoNumero::comparaReg (QSqlQuery&, int) {
    return (false);
}

bool buscaTipoDadoNumero::entradaValida () {
    if (this->_string.isEmpty ()) {
        return (false);
    } else {
        int i, l;
        l = this->_string.length();
        for (i = 0; i < l; i++) {
            if (! this->_string[i].isDigit ()) {
                return (false);
            }
        }
        return (true);
    }
}

QString buscaTipoDadoNumero::rotulo () {
    return ("Número");
}

void buscaTipoDadoNumero::_txtCaixaEntrada_textChanged (const QString& t) {
    this->_string = t.trimmed();
}

QString buscaTipoDadoNumero::texto () {
    return (QString("{%1} %2").arg((this->_matchWholeText) ? "AllText" : "Substr").arg(this->_string));
}

bool buscaTipoDadoNumero::setTexto (QString texto) {
    if (texto.startsWith ("{AllText} ", Qt::CaseInsensitive)) {
        this->_txtCaixaEntrada.setText (texto.mid (10));
        this->_chkMatchWholeText.setChecked (true);
        return (true);
    } else if (texto.startsWith ("{Substr} ", Qt::CaseInsensitive)) {
        this->_txtCaixaEntrada.setText (texto.mid (9));
        this->_chkMatchWholeText.setChecked (false);
        return (true);
    } else {
        return (false);
    }
}
