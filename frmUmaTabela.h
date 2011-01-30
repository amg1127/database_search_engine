#ifndef FRMUMATABELA_H
#define FRMUMATABELA_H

#include <QEvent>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QList>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QObject>
#include <QPalette>
#include <QPoint>
#include <QRect>
#include <QResizeEvent>
#include <QSize>
#include <QSpacerItem>
#include <QSplitter>

namespace Ui {
    class frmUmaTabela;
}

class button_spacer;

class frmUmaTabela : public QWidget
{
    Q_OBJECT

public:
    explicit frmUmaTabela(QWidget *parent = 0);
    ~frmUmaTabela();
    void limpaTabela ();
    void criaTabela (int);
    void adicionaLinha ();
    QLabel* celula (int, int);
    QLabel* coluna (int);
    void setNomeTabela (QString);
    QSize sizeHint () const;
private:
    Ui::frmUmaTabela *ui;
    bool hredim;
    bool vredim;
    QSize _sizeHint;
    QPoint startTopLeft;
    QRect startDiff;
    QGridLayout gridExterno;
    QGridLayout gridInterno;
    QSplitter* cabecHorizontal;
    QSplitter* cabecVertical;
    QList<button_spacer*> btnsHorizontal;
    QList<button_spacer*> btnsVertical;
    QList<QSpacerItem*> spcsHorizontal;
    QList<QSpacerItem*> spcsVertical;
    QList<QLabel*> celulasDados;
    int numeroLinhas;
    int numeroColunas;
    static void ajustaCabecalho (QLabel*, QLabel* = 0);
    static void ajustaCelula (QLabel*, QLabel* = 0);
protected:
    bool eventFilter (QObject*, QEvent*);
};

///////////////////////

class button_spacer : public QLabel {
    Q_OBJECT;
private:
    QSpacerItem* spc;
    Qt::Orientation orient;
public:
    button_spacer (QString, QSpacerItem*, Qt::Orientation);
protected:
    void resizeEvent (QResizeEvent*);
};

#endif // FRMUMATABELA_H
