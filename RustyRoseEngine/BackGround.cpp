#include "BackGround.h"

BackGround::BackGround(SDL_Renderer* renderer, std::string path)
{
	this->_renderer = renderer;
	this->_path = path;

	this->_mutex = SDL_CreateMutex();
	this->_animationCounter = 0;
}

void BackGround::tryLoadAnimation(std::string shortName)
{		
	std::string animPath = this->_path + shortName + ".A.PNG";
	SDL_Texture* texture = this->_tryGetAnimationTexture(animPath);
	
	if (texture == NULL) {
		//printf("unable to load animation image - file does not excist: %s\n", animPath.c_str());
		return;
	}

	TalkAnimation* talkAnimation = new TalkAnimation;
	talkAnimation->i = 0;
	talkAnimation->shortName = shortName;
	talkAnimation->sprites[0] = texture;

	animPath[animPath.size() - 1 - 4] = 'B';
	texture = this->_tryGetAnimationTexture(animPath);
	talkAnimation->sprites[1] = texture;

	animPath[animPath.size() - 1 - 4] = 'C';
	texture = this->_tryGetAnimationTexture(animPath);
	talkAnimation->sprites[2] = texture;

	SDL_LockMutex(this->_mutex);
	this->animations.push_back(talkAnimation);
	SDL_UnlockMutex(this->_mutex);
}

void BackGround::load()
{
	std::string pathWithEnd = this->_path + ".PNG";
	SDL_Surface* surface = IMG_Load(pathWithEnd.c_str());
	if (surface == NULL) {
		printf("unable to load backgroud image: %s\n", this->_path.c_str());
	}

	this->_texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
	if (this->_texture == NULL) {
		printf("unable to make texture from bg image: %s\n", this->_path.c_str());
	}

	SDL_FreeSurface(surface);
}

SDL_Texture* BackGround::getTexture()
{
	if (this == NULL) {
		printf("ERROR - TRY TO GET TEXTURE FROM NULL IN BACKGROUND\n");
		return NULL;
	}
	if (this->_texture != NULL) {
		return this->_texture;
	}
	return nullptr;
}

SDL_Texture* BackGround::getNextAnimationTexture(std::string shortName)
{
	TalkAnimation* animation = nullptr;
	for (int i = 0; i < this->animations.size(); i++) {
		if (this->animations[i]->shortName == shortName) {
			animation = this->animations[i];
			break;
		}
	}

	if (animation == nullptr) {
		//printf("animation '%s' for image '%s' not found\n", shortName.c_str(), this->_path.c_str());
		return nullptr;
	}

	SDL_Texture* texture = animation->sprites[animation->i];
	
	if (this->_animationCounter >= 2) {
		animation->i++;
		if (animation->i > 2) {
			animation->i = 0;
		}
		this->_animationCounter = 0;
	}
	else {
		this->_animationCounter++;
	}

	return texture;
}

std::string BackGround::getPath()
{
	return this->_path;
}

SDL_mutex* BackGround::getMutex()
{
	return this->_mutex;
}

void BackGround::free()
{
	SDL_LockMutex(this->_mutex);
	SDL_SetRenderTarget(this->_renderer, NULL);

	SDL_DestroyTexture(this->_texture);
	this->_texture = NULL;

	// Destroy animation textures and clear the animations vector.
	for (int i = 0; i < this->animations.size(); i++) {
		SDL_DestroyTexture(this->animations[i]->sprites[0]);
		SDL_DestroyTexture(this->animations[i]->sprites[1]);
		SDL_DestroyTexture(this->animations[i]->sprites[2]);
		delete this->animations[i];
	}

	this->animations.clear();
	SDL_UnlockMutex(this->_mutex);
}

SDL_Texture* BackGround::_tryGetAnimationTexture(std::string path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL) {
		printf("unable to load animation image: %s\n", path.c_str());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
	if (texture == NULL) {
		printf("unable to make texture from animation image: %s\n", path.c_str());
	}

	SDL_FreeSurface(surface);
	return texture;
}
