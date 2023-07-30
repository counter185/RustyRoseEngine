#include "BackGroundMusic.h"

BackGroundMusic::BackGroundMusic(std::string path)
{
	this->_path = path;
	this->_nameInt = this->_endUppercase(path) + "_INT.OGG";
	this->_nameLoop = this->_endUppercase(path) + "_LOOP.OGG";
	this->_load();
}

void BackGroundMusic::playInt()
{
	if (this == nullptr) {
		printf("ERROR - Trying to play NULL in BGM init\n");
		return;
	}

	this->_channel = 0;
	this->_channel = Mix_PlayChannel(this->_channel, this->_musicInt, 0);
	if (this->_channel == -1) {
		printf("unable to play background music Init: %s\n", this->_nameInt.c_str());
		printf("its possible that this file just not exists\n");
	}
}

void BackGroundMusic::playLoop()
{
	if (this == nullptr) {
		printf("ERROR - Trying to play NULL in BGM loop\n");
		return;
	}

	this->_channel = 0;
	this->_channel = Mix_PlayChannel(this->_channel, this->_musicLoop, 0);
	if (this->_channel == -1) {
		printf("unable to play background music Loop: %s\n", this->_nameLoop.c_str());
	}
}

bool BackGroundMusic::isReadyForLoop()
{
	if (Mix_Playing(this->_channel) == 0) {
		return true;
	}
	return false;
}

void BackGroundMusic::stop()
{
	Mix_HaltChannel(this->_channel);
}

void BackGroundMusic::free()
{
	Mix_FreeChunk(this->_musicInt);
	Mix_FreeChunk(this->_musicLoop);
}

std::string BackGroundMusic::getPath()
{
	return this->_path;
}

void BackGroundMusic::_load()
{
	this->_musicInt = Mix_LoadWAV(this->_nameInt.c_str());
	if (this->_musicInt == NULL) {
		printf("unable to load background muisc int: %s\n", this->_nameInt.c_str());
		printf("its possible that this file just not exists\n");
	}

	this->_musicLoop = Mix_LoadWAV(this->_nameLoop.c_str());
	if (this->_musicLoop == NULL) {
		printf("unable to load background muisc loop: %s\n", this->_nameLoop.c_str());
	}
}

std::string BackGroundMusic::_endUppercase(std::string text)
{
	for (int i = text.size() - 1; i >= 0; i--) {
		if (text[i] == '/') {
			break;
		}

		if (text[i] > 96 && text[i] < 123) {
			text[i] = text[i] - 32;
		}
	}
	return text;
}
