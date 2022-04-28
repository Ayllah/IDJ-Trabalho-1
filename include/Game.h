#pragma once
#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"


#include <string>

/************************************************** SPRITE **************************************************/
/* Classe Sprite: irá tomar conta de uma imagem dentro do state. */ 
class Sprite
{
    private:
        SDL_Texture* texture;
        int width;
        int height;
        SDL_Rect clipRect;

    public:
        Sprite ();
        Sprite (std::string file);
        ~Sprite ();
        void Open (std::string file);
        void SetClip (int x, int y, int w, int h);
        void Render (int x, int y);
        int GetWidth ();
        int GetHeight ();
        bool IsOpen();
};


/************************************************** MUSIC ***************************************************/
/* Classe Music: irá tomar conta de uma música no state.*/ 
class Music
{
    private:
        Mix_Music* music;

    public:
        Music ();
        Music (std::string file);
        ~Music ();
        void Play (int times = -1);
        void Stop (int msToStop = 1500);
        void Open (std::string file);
        bool IsOpen ();
};

/************************************************** STATE ***************************************************/
/* Classe State: "estado da tela de jogo". Podemos ter uma tela de apresentação, uma tela de NewGame, a tela
** do jogo em si, uma tela de fim de jogo, etc. Cada um desses seria um State. */ 
class State
{
    private:
        Sprite bg;
        Music music;
        bool quitRequested;

    public:
        State ();
        bool QuitRequested ();
        void LoadAssets ();
        void Update (float dt);
        void Render ();
};

/************************************************** GAME ***************************************************/
/* Classe Game: responsável por inicializar recursos, rodar o loop principal do jogo e liberar os recursos
** quando o jogo for encerrado. */ 
class Game
{
    private:
        Game (std::string title, int width, int heigth);   

        static Game* instance;
        SDL_Window* window;
        SDL_Renderer* renderer;
        State* state;

    public:
        ~Game ();
        void Run ();
        SDL_Renderer* GetRenderer ();
        State& GetState ();
        static Game& GetInstance ();
};