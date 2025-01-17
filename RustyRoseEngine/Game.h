#pragma once
#include "SharedUtils.h"
#include <RustyRenderWindow.h>
#include "IniFile.h"
#include "Jumps.h"
#include "SoundManager.h"
#include "BackGroundManager.h"
#include "VDecoder.h"
#include "Timer.h"
#include <RustyControl.h>
#include "Script.h"
#include "BackGround.h"

class Game
{
public:
	Game();
	void play();
	bool getStatus();

	~Game();

private:
	// engine
	bool _status;
	IniFile* _iniFile;
	RustyRenderWindow* _renderWindow;
	Jumps* _jumps;
	SoundManager* _soundManager;
	BackGroundManager* _backgroundManager;
	VDecoder* _vDecoder;
	Timer* _timer;
	RustyControl* _control;

	std::vector<std::string> _historyScript;
	Script* _currentScript;

	// gameplay
	enum TaskType {
		load,
		start,
		loop,
		end
	};

	bool _quitGame;
	bool _quitScriptLoop;
	bool _pauseStatus;
	bool _previousScript;
	bool _setScriptToStart;
	bool _skipToPlayerOption;

	Timer::Time _timeToLoad; // eg. if set to 1 sec, it will load events 1 sec before start
	Timer::Time _timeToEnd; // eg. if set to 500 ms, it will end events afert 500 ms extra time - useful with dialogs
	Script::EventsStateLists* _eventsStateLists;
	unsigned int _optionWindowId;
	unsigned short _playerOption;

	void _nextScript();
	void _playScript();

	void _handleWindows();

	void _loadEvents();
	void _startEvents();
	void _loopOrEndEvents();

	void _findAndHandle(Script::Event* event, TaskType taskType);

	void _SkipFRAME_(Script::Event* event);

	void _PlayBgm_Load(Script::Event* event);
	void _PlayBgm_Start(Script::Event* event);
	void _PlayBgm_Loop(Script::Event* event);
	void _PlayBgm_End(Script::Event* event);

	void _CreateBG_Load(Script::Event* event);
	void _CreateBG_Start(Script::Event* event);
	void _CreateBG_End(Script::Event* event);

	void _PrintText_Start(Script::Event* event);
	void _PrintText_End(Script::Event* event);

	void _PlayVoice_Load(Script::Event* event);
	void _PlayVoice_Start(Script::Event* event);
	void _PlayVoice_Loop(Script::Event* event); // nothing
	void _PlayVoice_End(Script::Event* event);

	void _PlaySe_Load(Script::Event* event);
	void _PlaySe_Start(Script::Event* event);
	void _PlaySe_End(Script::Event* event);

	void _Next_(Script::Event* event);

	void _PlayMovie_Start(Script::Event* event);
	void _PlayMovie_Loop(Script::Event* event);
	void _PlayMovie_End(Script::Event* event);

	void _BlackFade_Start(Script::Event* event);
	void _BlackFade_End(Script::Event* event);

	void _WhiteFade_Start(Script::Event* event);
	void _WhiteFade_End(Script::Event* event);

	void _SetSELECT_Start(Script::Event* event);
	void _SetSELECT_End(Script::Event* event);

	void _EndBGM_Load(Script::Event* event);
	void _EndBGM_Start(Script::Event* event);
	void _EndBGM_End(Script::Event* event);

	void _EndRoll_Start(Script::Event* event);
	void _EndRoll_Loop(Script::Event* event);
	void _EndRoll_End(Script::Event* event);

	// vibration for controler xD yeah im genius
	void _MoveSom_Start(Script::Event* event);
	void _MoveSom_End(Script::Event* event);

	void _exit();
	void _debug();
	void _pause();
	void _speedUp();
	void _speedDown();
	void _setSpeed1();
	void _setSpeed2();
	void _setSpeed4();
	void _setSpeed16();
	void _setSpeed32();
	void _next();
	void _previous();
	void _skip();
	void _setPlayerOption0();
	void _setPlayerOption1();
	void _setPlayerOption2();
	void _showHistory();

	int _exitWindow();
	int _debugWindow();
	int _pauseWindow();
	int _speedUpWindow();
	int _speedDownWindow();
	int _setSpeed1Window();
	int _setSpeed2Window();
	int _setSpeed4Window();
	int _setSpeed16Window();
	int _setSpeed32Window();
	int _nextWindow();
	int _previousWindow();
	int _skipWindow();
	int _setPlayerOption0Window();
	int _setPlayerOption1Window();
	int _setPlayerOption2Window();
	int _showHistoryWindow();

};

