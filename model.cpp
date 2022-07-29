#include "model.h"
#include <iostream>
#include <QtAlgorithms>
#include <QFile>
#include <QTextStream>
#include<QDebug>

ModelTakuzu::ModelTakuzu()
{
    _nbMaps  = 0;
    _sizeMap = 0;
}

ModelTakuzu::~ModelTakuzu()
{
}

int ModelTakuzu::changeColor(int p)
{
    if (p == BLACK) {
        return WHITE;
    } else if (p == WHITE) {
        return EMPTY;
    } else  {
        return BLACK;
    }
}

int ModelTakuzu::undoCase(int p)
{
    if (p == BLACK) {
        return EMPTY;
    } else if (p == EMPTY) {
        return WHITE;
    } else  {
        return BLACK;
    }
}

void ModelTakuzu::randomGridChoice(const QString &name)
{
    QFile file(":/resources/"+name);
    if (!(file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        std::cerr << "Error while opening new file: " << name.toStdString() << " .\n";
    } else {
        QString line;
        QTextStream in(&file);

        _nbMaps = in.readLine().toInt();
        int chosenMap = (rand() % _nbMaps);
        for (int j = 0; j < chosenMap; j++) {
            line = in.readLine();
        }


        line = in.readLine();
        int letterIndex = 0;
        QChar letter;
        foreach(letter, line) {
            if (letterIndex < _sizeMap * _sizeMap) {
                _currentGrid.push_back(ModelTakuzu::toPawn(letter));
            }
            letterIndex++;
        }
    }
}

void ModelTakuzu::setMap()
{
    _countPawn = {
        std::vector<int>(_sizeMap),
        std::vector<int>(_sizeMap),
        std::vector<int>(_sizeMap),
        std::vector<int>(_sizeMap)
    };
    for(int i = 0; i < _sizeMap; ++i) {
        for (int j = 0; j < _sizeMap; ++j) {
            emit notifyInitialPawn(i, j, _currentGrid[i * _sizeMap + j]);
        }
    }
    initCount();
}

void ModelTakuzu::At(int i, int j, int pawn)
{
    int oldPawn = positionAt(i, j);
    int newPawn = pawn;
    updateCount(i, j, oldPawn, newPawn);
    positionAt(i, j) = newPawn;
    positionIsValid(i, j);
    emit notifyNewToken(i, j, pawn);
}

bool ModelTakuzu::positionIsValid(int i, int j)
{
    const bool isVertical = true;
    const bool isOK = true;

    bool repetitionInRow = threeAlignedIn(getLine(i, "row"));
    bool repetitionInCol = threeAlignedIn(getLine(j, "column"));
    if (repetitionInRow) {
        emit notifyOverThreeAdjacentPawns(i, !isVertical, !isOK);
    } else {
        emit notifyOverThreeAdjacentPawns(i, !isVertical, isOK);
    }
    if (repetitionInCol) {
        emit notifyOverThreeAdjacentPawns(j, isVertical, !isOK);
    } else {
        emit notifyOverThreeAdjacentPawns(j, isVertical, isOK);
    }

    int oneOtherIdenticalRow = findIdentical(i, "row");
    int oneOtherIdenticalCol = findIdentical(j, "column");
    static auto oneOtherIdenticalRowColIsFound = [this](int index) -> bool {
        return (index != _sizeMap);
    };
    if (oneOtherIdenticalRowColIsFound(oneOtherIdenticalRow)) {
        emit notifyCommonPatterns(i, oneOtherIdenticalRow, !isVertical, isOK);
    } else {
        emit notifyCommonPatterns(i, oneOtherIdenticalRow, !isVertical, !isOK);
    }
    if (oneOtherIdenticalRowColIsFound(oneOtherIdenticalCol)) {
        emit notifyCommonPatterns(j, oneOtherIdenticalCol, isVertical, isOK);
    } else {
        emit notifyCommonPatterns(j, oneOtherIdenticalCol, isVertical, !isOK);
    }

    return (!repetitionInRow &&
            !repetitionInCol &&
            (oneOtherIdenticalRow == _sizeMap) &&
            (oneOtherIdenticalCol == _sizeMap));
}

bool ModelTakuzu::rowIsValid(int i)
{
    static auto forAll = [](bool tab[], int length) -> bool {
        for (int i = 0; i < length; ++i) {
            if (!tab[i]) return false;
        }
        return true;
    };
    bool tab[_sizeMap];
    for (int j = 0; j < _sizeMap; ++j) {
        tab[j] = positionIsValid(i, j);
    }
    return forAll(tab, _sizeMap) &&
            (_countPawn._Brow[i] == _countPawn._Wrow[i]);
}

bool ModelTakuzu::colIsValid(int j)
{
    static auto forAll = [](bool tab[], int length) -> bool {
        for (int i = 0; i < length; ++i) {
            if (!tab[i]) return false;
        }
        return true;
    };
    bool tab[_sizeMap];
    for (int i = 0; i < _sizeMap; ++i) {
        tab[i] = positionIsValid(i, j);
    }
    return forAll(tab, _sizeMap) &&
            (_countPawn._Bcol[j] == _countPawn._Wcol[j]);
}

void ModelTakuzu::initCount()
{
    for (int i = 0; i < _sizeMap; ++i) {
        for (int j = 0; j < _sizeMap; ++j) {
            updateCount(i, j, EMPTY, _currentGrid[i * _sizeMap + j]);
        }
    }
}

void ModelTakuzu::updateCount(int i, int j, int oldPawn, int newPawn)
{
    if (oldPawn != newPawn) {
        switch (oldPawn) {
        case BLACK:
            _countPawn._Brow[i]--;
            _countPawn._Bcol[j]--;
            break;
        case WHITE:
            _countPawn._Wrow[i]--;
            _countPawn._Wcol[j]--;
            break;
        case EMPTY:
            break;
        }
        switch (newPawn) {
        case BLACK:
            _countPawn._Brow[i]++;
            _countPawn._Bcol[j]++;
            break;
        case WHITE:
            _countPawn._Wrow[i]++;
            _countPawn._Wcol[j]++;
            break;
        case EMPTY:
            break;
        }
    }
    emit notifyCount(i, j,
                     _countPawn._Brow[i],
                     _countPawn._Bcol[j],
                     _countPawn._Wrow[i],
                     _countPawn._Wcol[j]);
}

int ModelTakuzu::getPosition(int i, int j) const
{
    return _currentGrid[i * _sizeMap + j];
}

void ModelTakuzu::registerPlayAt(int i, int j)
{
    int nextPawn = changeColor(positionAt(i, j));
    At(i, j, nextPawn);
}

void ModelTakuzu::registerChooseMapPool(QString difficulty, int size)
{
    QString name = QString::number(size);
    if (difficulty == "Easy") {
        name.append("_easy.txt");
    } else {
        name.append("_hard.txt");
    }
    _difficulty = difficulty;
    _sizeMap = size;
    randomGridChoice(name);
    setMap();
}

void ModelTakuzu::registerAttemptToEndGame()
{
    std::vector<bool> isValid;
    for(int i = 0; i < _sizeMap; ++i) {
        for(int j = 0; j < _sizeMap; ++j) {
            isValid.push_back(positionIsValid(i, j));
        }
    }
    std::vector<std::vector<int>> rowsAndCols;
    for (int i = 0; i < _sizeMap; ++i) {
        rowsAndCols.push_back(getLine(i, "row"));
        rowsAndCols.push_back(getLine(i, "column"));
    }

    std::vector<int> counterOcc;
    std::for_each(rowsAndCols.begin(), rowsAndCols.end(), [&counterOcc](std::vector<int> &vec) -> void {
       counterOcc.push_back(std::count(vec.begin(), vec.end(), BLACK));
       counterOcc.push_back(std::count(vec.begin(), vec.end(), WHITE));
    });


    bool win = (std::all_of(_currentGrid.begin(), _currentGrid.end(),
                       [](int p)->bool { return (p != EMPTY);}) &&
            std::all_of(isValid.begin(), isValid.end(), [](bool b)-> bool {return b;}) &&
            std::all_of(counterOcc.begin(), counterOcc.end(), [this](int v) -> bool {return (_sizeMap == 2 * v);}));

    emit notifyEndGame(win);
}



QChar ModelTakuzu::toQChar(int pawn)
{
    switch (pawn) {
    case BLACK: return QChar('B');
    case WHITE: return QChar('W');
    case EMPTY: return QChar('.');
    default: return QChar('.');
    }
}

int ModelTakuzu::toPawn(QChar letter)
{
    switch (letter.unicode()) {
    case 'B': return BLACK;
    case 'W': return WHITE;
    case '.': return EMPTY;
    default: return EMPTY;
    }
}

int &ModelTakuzu::positionAt(int i, int j)
{
    return _currentGrid[i * _sizeMap + j];
}

int ModelTakuzu::positionAt(int i, int j) const
{
    return _currentGrid[i * _sizeMap + j];
}

std::vector<int> ModelTakuzu::getLine(int i, QString line) const
{
    if(line == "row"){
        return std::vector<int>(_currentGrid.begin() + i * _sizeMap,
                                 _currentGrid.begin() + (i + 1) * _sizeMap);
    } else if(line == "column"){
        std::vector<int> column;
        for (int row = 0; row < _sizeMap; ++row) {
            column.push_back(positionAt(row, i));
        }
        return column;
    }
}

bool ModelTakuzu::threeAlignedIn(std::vector<int> vec)
{
    std::vector<int> vecBBB = {BLACK, BLACK, BLACK};
    std::vector<int> vecWWW = {WHITE, WHITE, WHITE};
    auto itB = std::search(vec.cbegin(), vec.cend(), vecBBB.cbegin(), vecBBB.cend());
    auto itW = std::search(vec.cbegin(), vec.cend(), vecWWW.cbegin(), vecWWW.cend());
    return (itB != vec.cend() || itW != vec.cend());
}

int ModelTakuzu::findIdentical(int i, QString line) const
{
    if(line == "row"){
        std::vector<int> rowToScan = getLine(i, "row");
        for (int rowIndex = 0; rowIndex < _sizeMap;++rowIndex) {
            if (rowIndex != i) {
                if (std::equal(_currentGrid.cbegin() + rowIndex * _sizeMap,
                           _currentGrid.cbegin() + (rowIndex + 1) * _sizeMap,
                               rowToScan.cbegin())) {
                    return rowIndex;
                }
            }
        }
        return _sizeMap;
    } else{
        std::vector<int> colToScan = getLine(i, "column");
        for (int colIndex = 0; colIndex < _sizeMap; ++colIndex) {
            if (colIndex != i) {
                std::vector<int> otherCol = getLine(colIndex, "column");
                if (std::equal(colToScan.begin(), colToScan.end(),
                               otherCol.begin(), otherCol.end())) {
                    return colIndex;
                }
            }
        }
        return _sizeMap;
    }
}



