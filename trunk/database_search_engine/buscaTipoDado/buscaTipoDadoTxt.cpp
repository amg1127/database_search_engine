#include "buscaTipoDadoTxt.h"

buscaTipoDadoTxt::buscaTipoDadoTxt()
{
    this->_frmFilho.setLayout (&(this->_layoutTextoBoxes));
    this->_layoutTextoBoxes.addWidget (&(this->_txtCaixaEntrada));
    this->_layoutTextoBoxes.addWidget (&(this->_chkMatchWholeText));
    this->_chkMatchWholeText.setText ("Coincidir expressão inteira");
    this->_txtCaixaEntrada.show ();
    this->_chkMatchWholeText.show ();
    QObject::connect (&(this->_txtCaixaEntrada), SIGNAL(textChanged(const QString&)), this, SLOT(_txtCaixaEntrada_textChanged(const QString&)));
    QObject::connect (&(this->_chkMatchWholeText), SIGNAL(toggled(bool)), this, SLOT(_chkMatchWholeText_toggled(bool)));
    QString t (this->_txtCaixaEntrada.text ());
    this->_txtCaixaEntrada_textChanged (t);
    this->_chkMatchWholeText_toggled (this->_chkMatchWholeText.isChecked ());
}

bool buscaTipoDadoTxt::entradaValida () {
    return (! this->_string.isEmpty());
}

QWidget& buscaTipoDadoTxt::campoEntrada() {
    return (this->_frmFilho);
}

void buscaTipoDadoTxt::_txtCaixaEntrada_textChanged (const QString& t) {
    this->_string = t;
}

void buscaTipoDadoTxt::_chkMatchWholeText_toggled (bool state) {
    this->_matchWholeText = state;
}
