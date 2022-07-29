#ifndef PRESENTERTAKUZU_H
#define PRESENTERTAKUZU_H


#include <QObject>
#include <QUndoStack>

#include "model.h"
#include "mainwindow.h"

class PresenterTakuzu : public QObject
{
    Q_OBJECT
public:
    PresenterTakuzu(QObject *parent = nullptr);
    ~PresenterTakuzu();
    void startNewGame();
public:
    ModelTakuzu *model;
    MainWindow *view;

public slots:
    void registerCommand(int i, int j);
    void registerUndoRedoTriggered();
private:
    QUndoStack *_undoStack;
};

#endif // PRESENTERTAKUZU_H

