#include "presenter.h"
#include "mainwindow.h"
#include "undo.h"

PresenterTakuzu::PresenterTakuzu(QObject *parent) : QObject(parent)
{
    model = new ModelTakuzu();
    view = new MainWindow();
    view->show();
    connect(view, SIGNAL(notifyMapChosen(QString,int)), model, SLOT(registerChooseMapPool(QString,int)));
    connect(view->getGrid(), SIGNAL(alertGridClicked(int,int)), model, SLOT(registerPlayAt(int,int)));
    connect(model, SIGNAL(notifyCount(int,int,int,int,int,int)), view->getGrid(), SLOT(count(int,int,int,int,int,int)));
    connect(model, SIGNAL(notifyInitialPawn(int,int,int)), view->getGrid(), SLOT(initialPawn(int,int,int)));
    connect(model, SIGNAL(notifyNewToken(int,int,int)), view->getGrid(), SLOT(newPawn(int,int,int)));
    _undoStack = new QUndoStack(this);
    view->getUndoButton()->setDefaultAction(_undoStack->createUndoAction(this, tr("&Undo")));
    connect(view->getGrid(), SIGNAL(alertGridClicked(int, int)), this, SLOT(registerCommand(int, int)));
    connect(model, SIGNAL(notifyOverThreeAdjacentPawns(int,bool,bool)), view->getGrid(), SLOT(threeAdjacentPawns(int,bool,bool)));
    connect(model, SIGNAL(notifyCommonPatterns(int,int,bool,bool)), view->getGrid(), SLOT(commonPatterns(int,int,bool,bool)));
    connect(view->getUndoButton(), SIGNAL(triggered(QAction*)), this, SLOT(registerUndoRedoTriggered()));
    connect(view->getSubmitButton(), SIGNAL(clicked()), model, SLOT(registerAttemptToEndGame()));
    connect(model, SIGNAL(notifyEndGame(bool)), view, SLOT(endGame(bool)));


}

PresenterTakuzu::~PresenterTakuzu()
{
    delete model;
}

void PresenterTakuzu::registerCommand(int i, int j)
{
    _undoStack->push(new Undo(model, i, j, model->getPosition(i, j)));
    view->getLabelUndo()->setText("UNDO : "+QString::number(0));
}

void PresenterTakuzu::registerUndoRedoTriggered()
{
    view->getLabelUndo()->setText("UNDO : "+QString::number(_undoStack->count()-_undoStack->index()));
}
