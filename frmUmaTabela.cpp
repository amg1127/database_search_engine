#include "frmUmaTabela.h"
#include "ui_frmUmaTabela.h"

#warning Isso eh temporario
#include <QDebug>

void frmUmaTabela::setNomeTabela (QString nome) {
    this->ui->lblNomeTabela->setText (nome);
}


void frmUmaTabela::criaTabela (int ncols) {
    this->limpaTabela ();
    bool visib = this->ui->frmDadosTabela->isVisible ();
    this->ui->frmDadosTabela->hide ();
    this->numeroColunas = ncols;
    // Cabeçalho horizontal (colunas)
    this->cabecHorizontal = new QSplitter (Qt::Horizontal, this->ui->frmDadosTabela);
    this->gridExterno.addWidget (this->cabecHorizontal, 0, 1);
    // Cabeçalho vertical (linhas)
    this->cabecVertical = new QSplitter (Qt::Vertical, this->ui->frmDadosTabela);
    this->gridExterno.addWidget (this->cabecVertical, 1, 0);
    this->ui->frmDadosTabela->setVisible (visib);
}

void frmUmaTabela::adicionaLinha () {
    bool visib = this->ui->frmDadosTabela->isVisible ();
    this->ui->frmDadosTabela->hide ();
    QLabel* lblNovo;
    QLabel* lblVelho;
    if (this->celulasDados.isEmpty ()) {
        lblVelho = NULL;
    } else {
        lblVelho = this->celulasDados.last();
    }
    button_spacer* btnNovo;
    button_spacer* btnVelho;
    if (this->btnsVertical.isEmpty ()) {
        btnVelho = NULL;
    } else {
        btnVelho = this->btnsVertical.last ();
    }
    // Espaçador da linha
    QSpacerItem* spcNovo = new QSpacerItem (0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->gridInterno.addItem (spcNovo, this->numeroLinhas, this->numeroColunas);
    this->spcsVertical << spcNovo;
    // Botão do cabeçalho vertical da linha
    btnNovo = new button_spacer (QString("%1").arg(this->numeroLinhas + 1), spcNovo, Qt::Vertical);
    this->ajustaCabecalho (btnNovo, btnVelho);
    this->cabecVertical->addWidget (btnNovo);
    this->btnsVertical << btnNovo;
    btnVelho = btnNovo;
    for (int i = 0; i < this->numeroColunas; i++) {
        // Células de dados
        lblNovo = new QLabel (QString("l%1 c%2").arg(this->numeroLinhas + 1).arg(i + 1));
        this->ajustaCelula (lblNovo, lblVelho);
        this->gridInterno.addWidget (lblNovo, this->numeroLinhas, i);
        this->celulasDados << lblNovo;
        lblVelho = lblNovo;
        if (this->numeroLinhas > 0) {
            // Espaçadores das colunas (reposicionar, somente)
            this->gridInterno.addItem (this->spcsHorizontal[i], this->numeroLinhas + 1, i);
        } else {
            // Espaçadores das colunas (criar)
            spcNovo = new QSpacerItem (0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
            this->gridInterno.addItem (spcNovo, 1, i);
            this->spcsHorizontal << spcNovo;
            // Botões do cabeçalho das colunas (criar)
            btnNovo = new button_spacer (QString("C%1").arg(i + 1), spcNovo, Qt::Horizontal);
            this->ajustaCabecalho (btnNovo);
            this->cabecHorizontal->addWidget (btnNovo);
            this->btnsHorizontal << btnNovo;
            btnVelho = btnNovo;
        }
    }
    this->numeroLinhas++;
    this->ui->frmDadosTabela->setVisible (visib);
}

QLabel* frmUmaTabela::celula (int l, int c) {
    if (l >= 0 && c >= 0 && l < this->numeroLinhas && c < this->numeroColunas) {
        return (this->celulasDados[l * this->numeroColunas + c]);
    } else {
        return (NULL);
    }
}

QLabel* frmUmaTabela::coluna (int c) {
    if (c >= 0 && c < this->numeroColunas && this->numeroLinhas > 0) {
        return (this->btnsHorizontal[c]);
    } else {
        return (NULL);
    }
}

void frmUmaTabela::ajustaCabecalho (QLabel* lbl, QLabel* o_lbl) {
    lbl->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Preferred);
    lbl->setAutoFillBackground (true);
    lbl->setAlignment (Qt::AlignCenter);
    if (o_lbl != NULL) {
        lbl->setPalette (o_lbl->palette ());
    } else {
        QPalette lbl_p (QApplication::palette());
        lbl_p.setColor (QPalette::Active, QPalette::Window, lbl_p.color (QPalette::Active, QPalette::Button));
        lbl_p.setColor (QPalette::Inactive, QPalette::Window, lbl_p.color (QPalette::Inactive, QPalette::Button));
        lbl_p.setColor (QPalette::Disabled, QPalette::Window, lbl_p.color (QPalette::Disabled, QPalette::Button));
        lbl->setPalette (lbl_p);
    }
}

void frmUmaTabela::ajustaCelula (QLabel* lbl, QLabel* o_lbl) {
    lbl->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Preferred);
    lbl->setFrameStyle (QFrame::Box | QFrame::Plain);
    lbl->setTextInteractionFlags (Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    lbl->setAutoFillBackground (true);
    if (o_lbl != NULL) {
        lbl->setPalette (o_lbl->palette ());
    } else {
        QPalette lbl_p (QApplication::palette());
        lbl_p.setColor (QPalette::Active, QPalette::Window, lbl_p.color (QPalette::Active, QPalette::Base));
        lbl_p.setColor (QPalette::Inactive, QPalette::Window, lbl_p.color (QPalette::Inactive, QPalette::Base));
        lbl_p.setColor (QPalette::Disabled, QPalette::Window, lbl_p.color (QPalette::Disabled, QPalette::Base));
        lbl->setPalette (lbl_p);
    }
}

void frmUmaTabela::limpaTabela () {
    bool visib = this->ui->frmDadosTabela->isVisible ();
    this->ui->frmDadosTabela->hide ();
    QSpacerItem* spcit;
    QLabel* lblit;
    while (! this->celulasDados.isEmpty()) {
        lblit = this->celulasDados.takeLast ();
        this->gridInterno.removeWidget (lblit);
        delete (lblit);
    }
    if (this->cabecVertical != NULL) {
        this->gridExterno.removeWidget (this->cabecVertical);
        delete (this->cabecVertical);
        this->cabecVertical = NULL;
    }
    if (this->cabecHorizontal != NULL) {
        this->gridExterno.removeWidget (this->cabecHorizontal);
        delete (this->cabecHorizontal);
        this->cabecHorizontal = NULL;
    }
    this->btnsVertical.clear ();
    this->btnsHorizontal.clear ();
    while (! this->spcsVertical.isEmpty()) {
        spcit = this->spcsVertical.takeLast ();
        this->gridInterno.removeItem (spcit);
        delete (spcit);
    }
    while (! this->spcsHorizontal.isEmpty()) {
        spcit = this->spcsHorizontal.takeLast ();
        this->gridInterno.removeItem (spcit);
        delete (spcit);
    }
    this->numeroLinhas = 0;
    this->numeroColunas = 0;
    this->gridInterno.invalidate ();
    this->gridExterno.invalidate ();
    this->ui->frmDadosTabela->setVisible (visib);
}

frmUmaTabela::frmUmaTabela(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmUmaTabela)
{
    ui->setupUi(this);
    // Grid externo
    this->gridExterno.setSpacing (0);
    this->ui->frmDadosTabela->setLayout (&(this->gridExterno));
    // Grid interno
    this->gridInterno.setSpacing (0);
    this->gridExterno.addLayout (&(this->gridInterno), 1, 1);

    this->cabecHorizontal = NULL;
    this->cabecVertical = NULL;
    this->numeroLinhas = 0;
    this->numeroColunas = 0;
    this->vredim = false;
    this->hredim = false;
    this->ui->lnDireita->installEventFilter (this);
    this->ui->lnDireitaInf->installEventFilter (this);
    this->ui->lnInferiorDir->installEventFilter (this);
    this->ui->lnInferior->installEventFilter (this);

    this->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    QLayout* lay = this->layout ();
    if (lay != NULL) {
        this->_sizeHint = lay->sizeHint ();
    } else {
        this->_sizeHint = this->size ();
    }
    this->updateGeometry ();
}

frmUmaTabela::~frmUmaTabela()
{
    this->limpaTabela ();
    delete ui;
}

QSize frmUmaTabela::sizeHint () const {
    return (this->_sizeHint);
}

bool frmUmaTabela::eventFilter(QObject *obj, QEvent *event) {
    QEvent::Type et = event->type();
    QMouseEvent* m_ev;
    switch (et) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseMove:
        case QEvent::MouseButtonRelease:
            m_ev = dynamic_cast<QMouseEvent*> (event);
            if (m_ev != NULL) {
                if (et == QEvent::MouseMove || m_ev->button() == Qt::LeftButton) {
                    if (obj == this->ui->lnDireita || obj == this->ui->lnDireitaInf || obj == this->ui->lnInferiorDir || obj == this->ui->lnInferior) {
                        QWidget* widget = qobject_cast<QWidget*> (obj);
                        if (widget != NULL) {
                            if (et == QEvent::MouseButtonPress) {
                                widget->grabMouse (widget->cursor ());
                                this->startTopLeft = this->mapToGlobal (QPoint (0, 0));
                                this->startDiff.setTopLeft (m_ev->globalPos ());
                                this->startDiff.setBottomRight (this->startTopLeft + QPoint (this->width(), this->height()));
                                if (obj == this->ui->lnDireitaInf || obj == this->ui->lnInferiorDir) {
                                    this->hredim = true;
                                    this->vredim = true;
                                } else if (obj == this->ui->lnInferior) {
                                    this->hredim = true;
                                } else if (obj == this->ui->lnDireita) {
                                    this->vredim = true;
                                }
                            }
                            QRect rect (this->startTopLeft, this->size ());
                            if (this->hredim) {
                                rect.setBottom (m_ev->globalY() + this->startDiff.height());
                            }
                            if (this->vredim) {
                                rect.setRight (m_ev->globalX() + this->startDiff.width());
                            }
                            this->_sizeHint = rect.size ();
                            this->updateGeometry ();
                            if (et == QEvent::MouseButtonRelease) {
                                widget->releaseMouse ();
                                this->hredim = false;
                                this->vredim = false;
                            }
                            event->accept ();
                            return (true);
                        }
                    }
                }
            }
        default:
            ;
    }
    return (this->QWidget::eventFilter (obj, event));
}

///////////////////////

button_spacer::button_spacer (QString text, QSpacerItem* spacer, Qt::Orientation orientation) : QLabel (text) {
    this->spc = spacer;
    this->orient = orientation;
}

void button_spacer::resizeEvent (QResizeEvent* ev) {
    static bool naoentrou = true;
    if (naoentrou) {
        naoentrou = false;
        if (this->orient == Qt::Horizontal) {
            this->spc->changeSize (ev->size().width(), 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
            ev->accept ();
        } else if (this->orient == Qt::Vertical) {
            this->spc->changeSize (1, ev->size().height(), QSizePolicy::Fixed, QSizePolicy::Fixed);
            ev->accept ();
        }
        #warning Isso nao da certo!
        if (ev->isAccepted ()) {
            QLayout* l = this->spc->layout ();
            if (l != NULL) {
                QWidget* w = l->parentWidget ();
                if (w != NULL) {
                    w->updateGeometry ();
                }
            }
        }
        naoentrou = true;
    } else {
        qDebug() << "Aiiii...";
    }
}
