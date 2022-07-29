#ifndef GRID_H
#define GRID_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "model.h"


class QPaintEvent;

class Grid : public QWidget
{
    Q_OBJECT
public:
    Grid(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;
    void paintPawn(int row, int column, int p);
    void paintCount(bool isRow, int index, int black, int white);
    void mousePressEvent(QMouseEvent* event) override;
    void setRows(int rows);


signals:
    void alertGridClicked(int x,int y);

public slots:
    void count(int i, int j, int Brow, int Bcol, int Wrow, int Wcol);
    void initialPawn(int i, int j, int p);
    void newPawn(int i, int j, int p);
    void threeAdjacentPawns(int index, bool isVertical, bool isOk);
    void commonPatterns(int first, int second, bool isVertical, bool isOK);

private:
    int _rows = 0;
    int _widthRect;
    int _margin;
    QPen* _pen;
    QPainter* _painter;
    QRect** _rects;
    QRect* _rowCountArea;
    QRect* _columnCountArea;
    int* _rowCounts;
    int* _colCounts;
    bool* _initPawns;
    int* _pawns;
    bool* _invalidVertical;
    bool* _invalidHorizontal;
    bool* _commonRows;
    bool* _commonColumns;
};

#endif // GRID_H
