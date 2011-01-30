#include "buscaTipoDadoString.h"

buscaTipoDadoString::buscaTipoDadoString()
{
    this->_layoutTextoBoxes.addWidget (&(this->_chkMatchCase));
    this->_chkMatchCase.setText ("Coincidir maiúsculas e minúsculas");
    this->_chkMatchCase.show ();
    QObject::connect (&(this->_chkMatchCase), SIGNAL(toggled(bool)), this, SLOT(_chkMatchCase_toggled(bool)));
    this->_chkMatchCase_toggled (this->_chkMatchCase.isChecked ());
}

bool buscaTipoDadoString::comparaReg (QSqlQuery& q, int nfields) {
    int i;
    QVariant val;
    for (val = q.value (i = 0); i < nfields && val.isValid(); val = q.value (++i)) {
        if (this->_matchWholeText) {
            if (val.toString().compare (this->_string, (this->_matchCase) ? Qt::CaseSensitive : Qt::CaseInsensitive) == 0) {
                return (true);
            }
        } else {
            if (val.toString().contains (this->_string, (this->_matchCase) ? Qt::CaseSensitive : Qt::CaseInsensitive)) {
                return (true);
            }
        }
    }
    return (false);
}

QString buscaTipoDadoString::rotulo () {
    return ("String");
}

QString buscaTipoDadoString::texto () {
    return (QString("{%1} {%2} %3").arg((this->_matchCase) ? "Case" : "NoCase").arg((this->_matchWholeText) ? "AllText" : "Substr").arg(this->_string));
}

bool buscaTipoDadoString::setTexto (QString texto) {
    bool chkCase, chkAllText;
    if (texto.startsWith ("{Case} ", Qt::CaseInsensitive)) {
        texto = texto.mid (7);
        chkCase = true;
    } else if (texto.startsWith ("{NoCase} ", Qt::CaseInsensitive)) {
        texto = texto.mid (9);
        chkCase = false;
    } else {
        return (false);
    }
    if (texto.startsWith ("{AllText} ", Qt::CaseInsensitive)) {
        texto = texto.mid (10);
        chkAllText = true;
    } else if (texto.startsWith ("{Substr} ", Qt::CaseInsensitive)) {
        texto = texto.mid (9);
        chkAllText = false;
    } else {
        return (false);
    }
    this->_chkMatchCase.setChecked (chkCase);
    this->_chkMatchWholeText.setChecked (chkAllText);
    this->_txtCaixaEntrada.setText (texto);
    return (true);
}

void buscaTipoDadoString::_chkMatchCase_toggled (bool state) {
    this->_matchCase = state;
}
