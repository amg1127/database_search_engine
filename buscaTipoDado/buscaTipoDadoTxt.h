#ifndef BUSCATIPODADOTXT_H
#define BUSCATIPODADOTXT_H

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>

#include "buscaTipoDado.h"

class buscaTipoDadoTxt : public buscaTipoDado
{
Q_OBJECT
protected:
    QWidget _frmFilho;
    QHBoxLayout _layoutTextoBoxes;
    QLineEdit _txtCaixaEntrada;
    QCheckBox _chkMatchWholeText;
    QString _string;
    bool _matchWholeText;
public:
    QWidget& campoEntrada();
    buscaTipoDadoTxt();
    bool entradaValida ();
public slots:
    void _txtCaixaEntrada_textChanged (const QString&);
    void _chkMatchWholeText_toggled (bool);
};

#endif // BUSCATIPODADOTXT_H
