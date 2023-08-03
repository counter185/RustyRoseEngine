#pragma once
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
class Voice
{
public:
	Voice(std::string path);

	void setChannel(int channel);
	int getChannel();
	void load();

	void play();
	void stop();
	void free();
	std::string getPath();

private:
	std::string _path;
	Mix_Chunk* _soundEffect;
	int _channel;
};

