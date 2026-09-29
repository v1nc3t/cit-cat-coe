#include "BoardDrawDirector.h"

void BoardDrawDirector::draw(BoardDrawBuilder &builder) const
{
    builder.background();
    builder.grid();
    builder.marks();
    builder.winLine();
}
