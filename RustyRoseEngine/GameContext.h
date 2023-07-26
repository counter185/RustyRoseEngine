#pragma once
#include <vector>
#include "Script.h"
#include "Voice.h"
#include "SoundEffect.h"
#include "BackGroundMusic.h"
#include "BackGround.h"
#include "System.h"


class GameContext
{
public:
	GameContext(SDL_Renderer* renderer);
	void addScript(std::string scriptPath);
	bool loadContentFromScripts();
	void clear();

	System* getSystem();
	Script* getScript(std::string path);
	Voice* getVoice(std::string path);
	SoundEffect* getSoundEffect(std::string path);
	BackGroundMusic* getBackGroundMusic(std::string path);
	BackGround* getBackGround(std::string path);

private:
	SDL_Renderer* _renderer;
	System* _system;

	std::vector<Script*> _scripts;
	std::vector<Voice*> _voices;
	std::vector<SoundEffect*> _soundEffects;
	std::vector<BackGroundMusic*> _backGroundMusics;
	std::vector<BackGround*> _backGrounds;

	void _loadAnimationForBackGround(Script* script, BackGround* backGround, int currentEvent);
};

