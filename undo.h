#ifndef UNDOREDO_H
#define UNDOREDO_H
#include <QUndoCommand>
#include "model.h"


class Undo : public QUndoCommand
{
public:
    Undo(ModelTakuzu *m, int i, int j, int p);
    ~Undo();
    void undo() override;


private:
    ModelTakuzu *_model;
    int _row;
    int _col;
    int _pawn;
    int _nbUndo;

};

#endif // UNDOREDO_H

