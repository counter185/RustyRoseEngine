#include "Game.h"

Game::Game()
{
    this->_initStatus = false;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error: initializing SDL: %s\n", SDL_GetError());
    }

    // initialization of SDL2 Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Error: cant initialize SDL: %s\n", SDL_GetError());
        return;
    }

    // initialization of TTF
    if (TTF_Init() == -1) {
        // Obs�uga b��du
        printf("Error: cant initialize SDL_ttf: %s\n", TTF_GetError());
        return;
    }

    // initialization of SDL2 Mixer
    int flags = MIX_INIT_OGG | MIX_INIT_MP3;
    int initFlags = Mix_Init(flags);
    if ((initFlags & flags) != flags) {
        printf("Error: cant initialize SDL Mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return;
    }

    // open stream audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        printf("Error: cant open audio streams: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return;
    }

    // set number of channels
    Mix_AllocateChannels(11); // <- 5 - Voice, 5 - SoundEffect, 1 - bgm

    this->_freeChannelsSoundEffect = { 1, 2, 3, 4, 5 };
    this->_freeChannelsVoice = { 6, 7, 8, 9, 10 };

    this->_window = SDL_CreateWindow("School Days: Rusty Rose Edition",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720, 0);

    this->_renderer = SDL_CreateRenderer(this->_window, -1, 0);
    if (this->_renderer == NULL) {
        printf("Error: cant create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(this->_window);
        SDL_Quit();
        return;
    }

    this->_gameContext = new GameContext(this->_renderer);
    this->_scene = new Scene(this->_renderer);
    this->_vDecoder = new VDecoder();
    this->_timer = new Timer();

    // debug string should ne in init.debugString
    this->_debugString = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\";

    // this should looks like: 
    // this->_gameContext->getSyste()->setSyste(init.linkToSys);
    std::string linkToSys = "C:\\Users\\Wiktor\\source\\repos\\RustyRoseEngine\\x64\\Debug\\data\\systemList.rose";
    this->_gameContext->getSystem()->setSystem(linkToSys);
    this->_gameContext->getSystem()->loadSystem();
    this->_scene->setFont(this->_gameContext->getSystem()->getFont());

    this->_initStatus = true;
}

void Game::play(std::string scriptPath)
{
    this->_gameContext->addScript(scriptPath);

    if (!this->_gameContext->loadContentFromScripts()) {
        printf("Error - there is no content from script\n");
        return;
    }

    bool end = false;
    std::vector<Script::Event*> todo = this->_gameContext->getScript(scriptPath)->getEvents();
    std::vector<Script::Event*> inprogres;

    this->_timer->reset();


    bool quit = false;
    SDL_Event sdl_event;

    const int TARGET_FPS = 30;
    Uint32 frameStartTime;
    Uint32 frameEndTime;
    Uint32 frameTime;

    while (!quit)
    {
        frameStartTime = SDL_GetTicks();
        while (SDL_PollEvent(&sdl_event))
        {
            if (sdl_event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        for (int i = 0; i < todo.size(); i++) {
            if (this->_timer->elapsed() >= *todo[i]->start) {
                // init action
                //printf("init action: 0x%X - data: %s\n", todo[i]->action, todo[i]->data.c_str());
                this->_findAndHandle(todo[i], 0);

                if (todo[i]->end != nullptr) {
                    inprogres.push_back(todo[i]);
                }
                this->_removeFrom(todo[i], todo);
            }
        }

        this->_scene->draw();
        frameEndTime = SDL_GetTicks();
        frameTime = frameEndTime - frameStartTime;

        for (int i = 0; i < inprogres.size(); i++) {
            if (this->_timer->elapsed() >= *inprogres[i]->end) {
                // end action
                //printf("end action: 0x%X - data: %s\n", inprogres[i]->action, inprogres[i]->data.c_str());
                this->_findAndHandle(inprogres[i], 1);

                this->_removeFrom(inprogres[i], inprogres);
            }
        }

        if (frameTime < 1000 / TARGET_FPS) {
            SDL_Delay(1000 / TARGET_FPS - frameTime);
        }

        if (todo.empty() && inprogres.empty()) {
            end = true;
        }
    }
}

SDL_Renderer* Game::getRenderer()
{
    return this->_renderer;
}

SDL_Window* Game::getWindow()
{
    return this->_window;
}

bool Game::isGameGood()
{
    return this->_initStatus;
}

int Game::_getFirstFreeChannelSoundEffect()
{
    if (this->_freeChannelsSoundEffect.size() == 0) {
        printf("There is no free channels for sound effect, return 5");
        return 1;
    }

    int freeChannel = this->_freeChannelsSoundEffect[this->_freeChannelsSoundEffect.size() - 1];
    this->_freeChannelsSoundEffect.pop_back();
    return freeChannel;
}

int Game::_getFirstFreeChannelVoice()
{
    if (this->_freeChannelsVoice.size() == 0) {
        printf("There is no free channels for voice, return 10");
        return 1;
    }

    int freeChannel = this->_freeChannelsVoice[this->_freeChannelsVoice.size() - 1];
    this->_freeChannelsVoice.pop_back();
    return freeChannel;
}

void Game::_removeFrom(Script::Event* event, std::vector<Script::Event*>& list)
{
    auto it = std::find(list.begin(), list.end(), event);
    if (it != list.end())
    {
        list.erase(it);
    }
}

void Game::_findAndHandle(Script::Event* event, int operation)
{
    // operation [0] = init, [1] = end, [other] = nothing yet?
    switch (event->action) {
    case 0xCC01:
        if (operation == 0) {
            this->_SkipFRAME_Init(event);
        }
        if (operation == 1) {
            this->_SkipFRAME_End(event);
        }
        break;

    case 0xCC02:
        if (operation == 0) {
            this->_PlayBgm_Init(event);
        }
        if (operation == 1) {
            this->_PlayBgm_End(event);
        }
        break;

    case 0xCC03:
        if (operation == 0) {
            this->_CreateBG_Init(event);
        }
        if (operation == 1) {
            this->_CreateBG_End(event);
        }
        break;

    case 0xCC04:
        if (operation == 0) {
            this->_PrintText_Init(event);
        }
        if (operation == 1) {
            this->_PrintText_End(event);
        }
        break;

    case 0xCC05:
        if (operation == 0) {
            this->_PlayVoice_Init(event);
        }
        if (operation == 1) {
            this->_PlayVoice_End(event);
        }
        break;

    case 0xCC06:
        if (operation == 0) {
            this->_PlaySe_Init(event);
        }
        if (operation == 1) {
            this->_PlaySe_End(event);
        }
        break;

    case 0xCC07:
        if (operation == 0) {
            this->_Next_Init(event);
        }
        if (operation == 1) {
            this->_Next_End(event);
        }
        break;

    case 0xCC08:
        if (operation == 0) {
            this->_PlayMovie_Init(event);
        }
        if (operation == 1) {
            this->_PlayMovie_End(event);
        }
        break;

    case 0xCC09:
        if (operation == 0) {
            this->_BlackFade_Init(event);
        }
        if (operation == 1) {
            this->_BlackFade_End(event);
        }
        break;

    case 0xCC0A:
        if (operation == 0) {
            this->_WhiteFade_Init(event);
        }
        if (operation == 1) {
            this->_WhiteFade_End(event);
        }
        break;

    case 0xCC0B:
        if (operation == 0) {
            this->_SetSELECT_Init(event);
        }
        if (operation == 1) {
            this->_SetSELECT_End(event);
        }
        break;

    case 0xCC0C:
        if (operation == 0) {
            this->_EndBGM_Init(event);
        }
        if (operation == 1) {
            this->_EndBGM_End(event);
        }
        break;

    case 0xCC0D:
        if (operation == 0) {
            this->_EndRoll_Init(event);
        }
        if (operation == 1) {
            this->_EndRoll_End(event);
        }
        break;

    case 0xCC0E:
        if (operation == 0) {
            this->_MoveSom_Init(event);
        }
        if (operation == 1) {
            this->_MoveSom_End(event);
        }
        break;

    default:
        printf("uanble to handle action: 0x%X\n", event->action);
        break;
    }
}

void Game::_SkipFRAME_Init(Script::Event* event)
{
    // todo
}

void Game::_SkipFRAME_End(Script::Event* event)
{
    // todo
}

void Game::_PlayBgm_Init(Script::Event* event)
{
    this->_gameContext->getBackGroundMusic(this->_debugString + event->data + ".OGG")->playLoop();
}

void Game::_PlayBgm_End(Script::Event* event)
{
    //this->_gameContext->getBackGroundMusic(this->_debugString + event->data + ".OGG")->stop();
}

void Game::_CreateBG_Init(Script::Event* event)
{
    this->_scene->addBackGround(this->_gameContext->getBackGround(this->_debugString + event->data), 0);
}

void Game::_CreateBG_End(Script::Event* event)
{
    // todo
    //this->_scene->removeBackGround()
}

void Game::_PrintText_Init(Script::Event* event)
{
    this->_scene->addText(event->data);
}

void Game::_PrintText_End(Script::Event* event)
{
    this->_scene->removeText(event->data);
}

void Game::_PlayVoice_Init(Script::Event* event)
{
    Voice* voice = this->_gameContext->getVoice(this->_debugString + event->data + ".OGG");
    if (voice) {
        voice->setChannel(this->_getFirstFreeChannelVoice());
        voice->play();
    }
}

void Game::_PlayVoice_End(Script::Event* event)
{
    Voice* voice = this->_gameContext->getVoice(this->_debugString + event->data + ".OGG");
    if (voice) {
        this->_freeChannelsVoice.push_back(voice->getChannel());
    }
}

void Game::_PlaySe_Init(Script::Event* event)
{
    SoundEffect* soundEffect = this->_gameContext->getSoundEffect(this->_debugString + event->data + ".OGG");
    if (soundEffect) {
        soundEffect->setChannel(this->_getFirstFreeChannelSoundEffect());
        soundEffect->play();
    }
}

void Game::_PlaySe_End(Script::Event* event)
{
    SoundEffect* soundEffect = this->_gameContext->getSoundEffect(this->_debugString + event->data + ".OGG");
    if (soundEffect) {
        this->_freeChannelsSoundEffect.push_back(soundEffect->getChannel());
    }
}

void Game::_Next_Init(Script::Event* event)
{
    // todo
}

void Game::_Next_End(Script::Event* event)
{
    // todo
}

void Game::_PlayMovie_Init(Script::Event* event)
{
    // todo
}

void Game::_PlayMovie_End(Script::Event* event)
{
    // todo
}

void Game::_BlackFade_Init(Script::Event* event)
{
    // todo
}

void Game::_BlackFade_End(Script::Event* event)
{
    // todo
}

void Game::_WhiteFade_Init(Script::Event* event)
{
    // todo
}

void Game::_WhiteFade_End(Script::Event* event)
{
    // todo
}

void Game::_SetSELECT_Init(Script::Event* event)
{
    // todo
    this->_scene->addText(event->data);
}

void Game::_SetSELECT_End(Script::Event* event)
{
    this->_scene->removeText(event->data);
}

void Game::_EndBGM_Init(Script::Event* event)
{
    // todo
}

void Game::_EndBGM_End(Script::Event* event)
{
    // todo
}

void Game::_EndRoll_Init(Script::Event* event)
{
    // todo
}

void Game::_EndRoll_End(Script::Event* event)
{
    // todo
}

void Game::_MoveSom_Init(Script::Event* event)
{
    // todo
}

void Game::_MoveSom_End(Script::Event* event)
{
    // todo
}
