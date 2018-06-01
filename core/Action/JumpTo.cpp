#include "..\e2daction.h"
#include "..\e2dnode.h"

e2d::JumpTo::JumpTo(double duration, const Point & pos, double height, int jumps)
	: JumpBy(duration, Point(), height, jumps)
	, _endPos(pos)
{
}

e2d::JumpTo * e2d::JumpTo::clone() const
{
	return Create<JumpTo>(_duration, _endPos, _height, _jumps);
}

void e2d::JumpTo::_init()
{
	JumpBy::_init();
	_deltaPos = _endPos - _startPos;
}