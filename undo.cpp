#include "undo.h"

Undo::Undo(ModelTakuzu *m, int i, int j, int p)
{
    _model = m;
    _row = i;
    _col = j;
    _pawn = p;
}

Undo::~Undo()
{}

void Undo::undo()
{
    int oldPawn = ModelTakuzu::undoCase(_pawn);
    _model->At(_row, _col, oldPawn);
}
