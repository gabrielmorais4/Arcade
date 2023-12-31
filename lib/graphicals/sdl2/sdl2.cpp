/*
**       _/_/_/  _/_/_/    _/          _/_/
**    _/        _/    _/  _/        _/    _/
**     _/_/    _/    _/  _/            _/
**        _/  _/    _/  _/          _/
** _/_/_/    _/_/_/    _/_/_/_/  _/_/_/_/
**
*/

#include "sdl2.hpp"

Sdl2::Sdl2()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(0);
    _font = TTF_OpenFont("font.ttf", 60);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_SOFTWARE);
}

Sdl2::~Sdl2()
{
    // for (auto texture : _renderedTextures) {
    //     SDL_DestroyTexture(texture);
    // }
    if (_font != nullptr)
        TTF_CloseFont(_font);
    if (_renderer != nullptr)
        SDL_DestroyRenderer(_renderer);
    Mix_CloseAudio();
    Mix_FreeMusic( _music );
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

void Sdl2::init()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    _music = Mix_LoadMUS("music.ogg");
    _window = SDL_CreateWindow("Arcade - SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_HIDDEN);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_SOFTWARE);
    Mix_Volume(-1, 30);
    Mix_PlayMusic(_music, -1);
    SDL_ShowWindow(_window);
}

void Sdl2::stop()
{
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;
    SDL_DestroyWindow(_window);
}

void Sdl2::draw()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderPresent(_renderer);
    SDL_Delay(1000/60);
}

void Sdl2::update(std::map<std::string, IGameModule::Entity> entities)
{
    if (Mix_PlayingMusic() == 0)
        Mix_PlayMusic(_music, -1);
    SDL_RenderClear(_renderer);
    for (auto texture : _renderedTextures) {
            SDL_DestroyTexture(texture);
    }
    _renderedTextures.clear();
    for (auto const &entity : entities) {
        IGameModule::Entity e = entity.second;
        if (e.id_file == -1) {
            if (e.bold && e.underline) {
                TTF_SetFontStyle(_font, TTF_STYLE_UNDERLINE);
            }
            _text_surface = TTF_RenderText_Solid(_font, e.text.c_str(), {
                (Uint8)e.color_fg.red,
                (Uint8)e.color_fg.green,
                (Uint8)e.color_fg.blue, 255
            });
            SDL_RenderSetScale(_renderer, 1, 1);
            SDL_Texture *_text_texture = SDL_CreateTextureFromSurface(_renderer, _text_surface);
            _text_rect = {(int)((e.x * 100)), (int)(e.y * 100), _text_surface->w, _text_surface->h};
            SDL_FreeSurface(_text_surface);
            SDL_RenderCopy(_renderer, _text_texture, NULL, &_text_rect);
            _renderedTextures.insert(_renderedTextures.end(), _text_texture);
            TTF_SetFontStyle(_font, TTF_STYLE_NORMAL);
        } else {
            float x = (e.x * 100) * 0.16;
            float y = (e.y * 100) * 0.16;
            SDL_RenderSetScale(_renderer, 2, 2);
            _rects[e.id_file].x = (int)x;
            _rects[e.id_file].y = (int)y;
            SDL_RenderCopy(_renderer, _textures[e.id_file], NULL, &_rects[e.id_file]);
        }
    }
    entities.clear();
}

std::string Sdl2::getEvent()
{
    while (SDL_PollEvent(&_event)) {
        if (_event.type == SDL_QUIT) {
            return "close";
        }
        int x, y;
        SDL_GetWindowSize(_window, &x, &y);
        if (_event.type == SDL_MOUSEBUTTONDOWN) {
            std::cout << _event.button.x / (x / 11) << " " << _event.button.y / (y / 11) << std::endl;
            return "MouseClick " +
            std::to_string(_event.button.x / (x / 11)) + " " +
            std::to_string(_event.button.y / (y / 11));
        }
        if (_event.type == SDL_KEYDOWN && _event.key.repeat == 0) {
            switch (_event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return "ESC";
                case SDLK_RETURN:
                    return "Enter";
                case SDLK_LEFT:
                    return "LeftArrow";
                case SDLK_RIGHT:
                    return "RightArrow";
                case SDLK_UP:
                    return "UpArrow";
                case SDLK_DOWN:
                    return "DownArrow";
                case SDLK_BACKSPACE:
                    return "Backspace";
                case SDLK_F1:
                    return "F1";
                case SDLK_F2:
                    return "F2";
                default:
                    return "";
            }
        }
        if (_event.type == SDL_TEXTINPUT && _event.key.repeat == 0) {
            std::string val = _event.text.text;
            return val;
        }
    }
    return "";
}

const std::string &Sdl2::getName() const
{
    return _name;
}

void Sdl2::resetDisplay(void)
{
    _textures.clear();
    _rects.clear();
    for (auto texture : _renderedTextures) {
        if (texture)
            SDL_DestroyTexture(texture);
    }
    _renderedTextures.clear();
}

extern "C" IDisplayModule *create(void)
{
    return new  Sdl2();
}

extern "C" std::string getType(void)
{
    return "Graphic";
}

extern "C" void destroy(IDisplayModule* obj) {
    delete obj;
}

void Sdl2::saveTextures(const std::vector<std::string> &texture)
{
    for (auto const &val : texture) {
        SDL_Surface *_image_surface;
        _image_surface = IMG_Load(val.c_str());
        SDL_Texture *_image_texture;
        _image_texture = SDL_CreateTextureFromSurface(_renderer, _image_surface);
        std::cout << "load texture: " << val << std::endl;
        _textures.push_back(_image_texture);
        _rects.push_back({0, 0, _image_surface->w, _image_surface->h});
        SDL_FreeSurface(_image_surface);
    }
}
