#ifndef MODELTAKUZU_H
#define MODELTAKUZU_H

#include <algorithm>
#include <set>
#include <string>
#include <vector>
#include <QObject>
#include <QString>
#define EMPTY 0
#define BLACK 1
#define WHITE 2

class ModelTakuzu : public QObject {
    Q_OBJECT

public:
    ModelTakuzu();
    ~ModelTakuzu();
    int changeColor(int pawn);
    static int undoCase(int pawn);
    void At(int i, int j, int pawn);
    bool positionIsValid(int i, int j);
    bool rowIsValid(int i);
    bool colIsValid(int j);
    void initCount();
    void updateCount(int i, int j, int oldPawn, int newToken);
    int getPosition(int i, int j) const;

    void randomGridChoice(const QString &name);
    void setMap();

signals:
    void notifyNewToken(int i, int j, int newToken);
    void notifyCount(int i, int j, int Brow, int Bcol, int Wrow, int Wcol);
    void notifyPositionIsValid(int i, int j, bool isValid);
    void notifyInitialPawn(int i, int j, int readOnlyPawn);
    void notifyOverThreeAdjacentPawns(int index, bool isVertical, bool isOK);
    void notifyCommonPatterns(int first, int second, bool isVertical, bool isOK);
    void notifyEndGame(bool win);

public slots:
    void registerPlayAt(int i, int j);
    void registerChooseMapPool(QString difficulty, int size);
    void registerAttemptToEndGame();

private:
    void loadFile(const QString &name);
    static QChar toQChar(int pawn);
    static int toPawn(QChar letter);
    int &positionAt(int i, int j);
    int positionAt(int i, int j) const;
    std::vector<int> getLine(int i, QString line) const;
    static bool threeAlignedIn(std::vector<int> vec);
    int findIdentical(int i, QString line) const;


private:
    int _nbMaps;
    int _sizeMap;
    QString _difficulty;
    std::vector<int> _currentGrid;
    struct {
        std::vector<int> _Wrow;
        std::vector<int> _Brow;
        std::vector<int> _Wcol;
        std::vector<int> _Bcol;
    } _countPawn;
};

#endif // MODELTAKUZU_H
