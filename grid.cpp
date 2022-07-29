#include "grid.h"

Grid::Grid(QWidget *parent) : QWidget(parent)
{

}

void Grid::paintEvent(QPaintEvent *)
{

    _rects = new QRect*[_rows];
    _rowCountArea = new QRect[_rows*2];
    _columnCountArea = new QRect[_rows*2];

    _painter = new QPainter(this);
    _pen = new QPen(Qt::gray);
    _pen->setWidth(0);
    _painter->setPen(*_pen);

    int width = (QWidget::width()<QWidget::height())?QWidget::width():QWidget::height();
    _widthRect = width/(_rows+2);
    _margin = (QWidget::width()-width+5*_widthRect)/2;

    // Draws grid
    for (int i = 0; i < _rows; ++i) {
        _rects[i] = new QRect[_rows];
        for (int j = 0; j < _rows; ++j) {
            _rects[i][j] = QRect(i*_widthRect+_margin, j*_widthRect, _widthRect, _widthRect);
            _painter->drawRect(_rects[i][j]);
        }
        _rowCountArea[i*2] = QRect(_rows*_widthRect+_margin+5, i*_widthRect, _widthRect, _widthRect);
        _columnCountArea[i*2] = QRect(i*_widthRect+_margin, _rows*_widthRect+5, _widthRect, _widthRect);
        _rowCountArea[i*2+1] = QRect((_rows+1)*_widthRect+_margin+5, i*_widthRect, _widthRect, _widthRect);
        _columnCountArea[i*2+1] = QRect(i*_widthRect+_margin, (_rows+1)*_widthRect+5, _widthRect, _widthRect);
    }

    // Draws in red invalid rows and highlights common rows in orange
    QRect rect;
    for (int i = 0; i < _rows; i++) {
        if (_invalidHorizontal[i] == true) {
            for (int j = 0; j < _rows; j++) {
                rect = _rects[j][i];
                _painter->fillRect(rect.x()+3, rect.y()+3, rect.width()-3, rect.height()-3, QBrush(Qt::red));
            }
        }
        if (_invalidVertical[i]) {
            for (int j = 0; j < _rows; j++) {
                rect = _rects[i][j];
                _painter->fillRect(rect.x()+3, rect.y()+3, rect.width()-3, rect.height()-3, QBrush(Qt::red));
            }
        }
        QPen* penOrange = new QPen(Qt::red);
        penOrange->setWidth(4);
        _painter->setPen(*penOrange);
        for (int j = 0; j < _rows; j++) {
            if (_commonRows[i*_rows+j]) {
                rect = _rects[0][i];
                _painter->drawRect(rect.x(), rect.y(), _rows*_widthRect, _widthRect);
                rect = _rects[0][j];
                _painter->drawRect(rect.x(), rect.y(), _rows*_widthRect, _widthRect);
            }
            if (_commonColumns[i*_rows+j]) {
                rect = _rects[i][0];
                _painter->drawRect(rect.x(), rect.y(), _widthRect, _rows*_widthRect);
                rect = _rects[j][0];
                _painter->drawRect(rect.x(), rect.y(), _widthRect, _rows*_widthRect);
            }
        }
        _painter->setPen(*_pen);
    }

    // Draws pawns and counts
    for (int i = 0; i < _rows; i++) {
        paintCount(true, i, _rowCounts[i*2], _rowCounts[i*2+1]);
        paintCount(false, i, _colCounts[i*2], _colCounts[i*2+1]);
        for (int j = 0; j < _rows; j++) {
            paintPawn(i, j, _pawns[i*_rows+j]);
        }
    }

    _painter->end();
}

void Grid::paintPawn(int row, int column, int p) {
    QRect rect = _rects[row][column];
    int width = rect.width()/2-3;
    QPoint center(rect.center().x(), rect.center().y());
    switch(p) {
    case BLACK:
        _painter->setBrush(Qt::black);
        _painter->drawEllipse(center, width, width);
        break;
    case WHITE:
        _painter->setBrush(Qt::white);
        _painter->drawEllipse(center, width, width);
        break;
    case EMPTY:
        _painter->eraseRect(rect.x()+2, rect.y()+2, rect.width()-2, rect.height()-2);
        break;
    }
    // If initial pawns, the pawn is surrounded in blue
    if (_initPawns[row*_rows+column]) {
        QPen penInit(Qt::magenta);
        penInit.setWidth(4);
        _painter->setPen(penInit);
        _painter->drawEllipse(center, 1, 1);
        _painter->setPen(*_pen);
    }
}

void Grid::paintCount(bool isRow, int index, int black, int white)
{
    QRect countBlack;
    QRect countWhite;
    if (isRow) {
        countBlack = _rowCountArea[index*2];
        countWhite = _rowCountArea[index*2+1];
    } else {
        countBlack = _columnCountArea[index*2];
        countWhite = _columnCountArea[index*2+1];
    }

    if (black == _rows/2 && white == _rows/2) {
        _painter->fillRect(countBlack, Qt::green);
        _painter->fillRect(countWhite, Qt::green);
    } else {
        _painter->drawText(countBlack, Qt::AlignCenter, "B"+ QString::number(black));
        _painter->drawText(countWhite, Qt::AlignCenter, "W"+ QString::number(white));
    }
}

void Grid::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
            int x = (event->x()-_margin) / _widthRect;
            int y = event->y() / _widthRect;
            if (x < _rows && event->x()-_margin > 0) {
                if (y < _rows) {
                    if (!_initPawns[x*_rows+y]) {
                        emit alertGridClicked(y,x);
                    }
                }
            }
    }
}

void Grid::setRows(int rows)
{
    _rows = rows;
    _rowCounts = new int[_rows*2]();
    _colCounts = new int[_rows*2]();
    _initPawns = new bool[_rows*_rows];
    _pawns = new int[_rows*_rows];
    _invalidHorizontal = new bool[_rows];
    _invalidVertical = new bool[_rows];
    _commonRows = new bool[_rows*_rows];
    _commonColumns = new bool[_rows*_rows];

    for (int i = 0; i < _rows; i++) {
        _invalidHorizontal[i] = false;
        _invalidVertical[i] = false;
        for (int j = 0; j < _rows; j++) {
            _commonRows[i*_rows+j] = false;
            _commonColumns[i*_rows+j] = false;
            _initPawns[i*_rows+j] = false;
            _pawns[i*_rows+j] = EMPTY;
        }
    }

    repaint();
}

void Grid::count(int i, int j, int Brow, int Bcol, int Wrow, int Wcol)
{
    _rowCounts[i*2] = Brow;
    _rowCounts[i*2+1] = Wrow;
    _colCounts[j*2] = Bcol;
    _colCounts[j*2+1] = Wcol;
    repaint();
}

void Grid::initialPawn(int i, int j, int p)
{
    if (p != EMPTY) {
        _initPawns[j*_rows+i] = true;
    }
    _pawns[j*_rows+i] = p;
    repaint();
}

void Grid::newPawn(int i, int j, int p)
{
    _pawns[j*_rows+i] = p;
    repaint();
}

void Grid::threeAdjacentPawns(int index, bool isVertical, bool isOk)
{
    if (isVertical) {
        _invalidVertical[index] = !isOk;
    } else {
        _invalidHorizontal[index] = !isOk;
    }
}

void Grid::commonPatterns(int first, int second, bool isVertical, bool isOK)
{
    if (isVertical) {
        if (isOK) {
            _commonColumns[second*_rows+first] = isOK;
            _commonColumns[first*_rows+second] = isOK;
        } else {
            for (int i = 0; i < _rows; i++) {
                if (_commonColumns[i*_rows+first]) {
                    _commonColumns[i*_rows+first] = isOK;
                }
                _commonColumns[first*_rows+i] = isOK;
            }
        }
    } else {
        if (isOK) {
            _commonRows[second*_rows+first] = isOK;
            _commonRows[first*_rows+second] = isOK;
        } else {
            for (int i = 0; i < _rows; i++) {
                if (_commonRows[i*_rows+first]) {
                    _commonRows[i*_rows+first] = isOK;
                }
                _commonRows[first*_rows+i] = isOK;
            }
        }
    }
}

