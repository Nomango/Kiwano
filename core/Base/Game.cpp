#include "..\e2dbase.h"
#include "..\e2dmanager.h"
#include "..\e2dtool.h"
#include "..\e2dcollider.h"


e2d::Game * e2d::Game::_instance = nullptr;

e2d::Game::Game()
	: _ended(false)
	, _paused(false)
	, _config(nullptr)
{
	CoInitialize(nullptr);
}

e2d::Game::~Game()
{
	GC::release(_config);

	CoUninitialize();
}

e2d::Game * e2d::Game::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) Game;
	return _instance;
}

void e2d::Game::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

void e2d::Game::start(bool cleanup)
{
	auto gc = GC::getInstance();
	auto input = Input::getInstance();
	auto window = Window::getInstance();
	auto renderer = Renderer::getInstance();
	auto actionManager = ActionManager::getInstance();

	// 初始化场景管理器
	SceneManager::__init();
	// 显示窗口
	::ShowWindow(window->getHWnd(), SW_SHOWNORMAL);
	// 刷新窗口内容
	::UpdateWindow(window->getHWnd());
	// 处理窗口消息
	window->__poll();
	// 初始化计时
	Time::__init();

	_ended = false;

	while (!_ended)
	{
		// 处理窗口消息
		window->__poll();
		// 刷新时间
		Time::__updateNow();

		// 判断是否达到了刷新状态
		if (Time::__isReady())
		{
			input->__update();			// 获取用户输入
			Timer::__update();			// 更新定时器
			actionManager->__update();	// 更新动作管理器
			SceneManager::__update();	// 更新场景内容
			renderer->__render();		// 渲染游戏画面

			Time::__updateLast();		// 刷新时间信息
		}
		else
		{
			Time::__sleep();			// 挂起线程
			gc->__update();				// 刷新内存池
		}
	}

	_ended = true;

	if (cleanup)
	{
		Game::cleanup();
	}
}

void e2d::Game::pause()
{
	_paused = true;
}

void e2d::Game::resume()
{
	if (_paused)
	{
		Game::reset();
	}
	_paused = false;
}

void e2d::Game::reset()
{
	if (!_ended)
	{
		Time::__reset();
		ActionManager::getInstance()->__resetAll();
		Timer::__resetAll();
	}
}

bool e2d::Game::isPaused()
{
	return _paused;
}

void e2d::Game::setConfig(Config * config)
{
	GC::release(_config);
	_config = config;
	GC::retain(_config);
}

e2d::Config * e2d::Game::getConfig()
{
	if (!_config)
	{
		_config = Create<Config>();
		GC::retain(_config);
	}
	return _config;
}

void e2d::Game::quit()
{
	_ended = true;	// 这个变量将控制游戏是否结束
}

void e2d::Game::cleanup()
{
	// 删除所有场景
	SceneManager::__uninit();
	// 删除输入监听器
	Input::__clearListeners();
	// 删除碰撞监听器
	Collision::__clearListeners();
	// 清空图片缓存
	Image::clearCache();
	// 清空定时器
	Timer::__uninit();
	// 删除所有对象
	GC::getInstance()->clear();
}
