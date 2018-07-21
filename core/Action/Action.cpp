#include "..\e2daction.h"
#include "..\e2dmanager.h"

e2d::Action::Action() 
	: _running(false)
	, _done(false)
	, _initialized(false)
	, _target(nullptr)
	, _last(0)
{
	ActionManager::getInstance()->__add(this);
}

e2d::Action::~Action()
{
	ActionManager::getInstance()->__remove(this);
}

bool e2d::Action::isRunning()
{
	return _running;
}

void e2d::Action::resume()
{
	_running = true;
	_last = Game::getInstance()->getTotalTime();
}

void e2d::Action::pause()
{
	_running = false;
}

void e2d::Action::stop()
{
	_done = true;
}

e2d::String e2d::Action::getName() const
{
	return _name;
}

void e2d::Action::setName(const String& name)
{
	_name = name;
}

e2d::Node * e2d::Action::getTarget()
{
	return _target;
}

void e2d::Action::reset()
{
	_initialized = false;
	_done = false;
	_last = Game::getInstance()->getTotalTime();
}

bool e2d::Action::_isDone()
{
	return _done;
}

void e2d::Action::_startWithTarget(Node* target)
{
	_target = target;
	_running = true;
	this->reset();
}

void e2d::Action::_init()
{
	_initialized = true;
	_last = Game::getInstance()->getTotalTime();
}

void e2d::Action::_update()
{
	if (!_initialized)
	{
		_init();
	}
}

void e2d::Action::_resetTime()
{
}
