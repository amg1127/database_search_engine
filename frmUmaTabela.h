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

/*
    Infelizmente, terei que reimplementar uma estrutura de tabela, linhas, colunas e celulas dentro do frmUmaTabela::ui->frmDadosTabela.
    O widget QTableView não serve para os meus objetivos, porque ele produz uma barra de rolagem interna quando 
dados de tabelas grandes precisam ser exibidos e nao ha forma de mudar esse comportamento.
    Dentro do frmUmaTabela::ui->frmDadosTabela, sera criado um QGridLayout de 2x2 que terá:
        # (0, 1) Um QSplitter com M QLabels para os rótulos das colunas (nomes dos campos)
        # (1, 0) Um QSplitter com N QLabels para os rótulos das linhas (sequência numérica)
        # (1, 1) Um QGridLayout de (M+1)x(N+1), sendo que:
            # De (0, 0) a (M-1, N-1), serao criados QLabels para apresentar os dados
            # Na linha M, serão criados QSpacerItem orientados horizontalmente e com 0px de altura, que servirão para definir a largura das colunas
            # Na coluna N, serão criados QSpacerItem orientados verticalmente e com 0px de largura, que servirão para definir a altura das linhas
    Quando os filhos do QSplitter do rótulo das linhas forem redimensionados, as alturas deverão propagar-se aos QSpacerItems da coluna N.
    Quando os filhos do QSplitter do rótulo das colunas forem redimensionados, as larguras deverão propagar-se aos QSpacerItems da linha M.
*/

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
