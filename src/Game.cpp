#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"
#include <stdexcept>
#include "Game.h"


Game* Game::instance = nullptr;
/************************************************** GAME ***************************************************/
Game& Game::GetInstance (){
    if (instance == nullptr)        // Checa se já há uma instância rodando
        instance = new Game("Ayllah_170056465",1024,600);      

    return *instance;
}

//  Construtor
Game::Game (std::string title,int width,int height){
    if (instance != nullptr)        // Checa se já há uma instância rodando
        throw std::runtime_error("Jogo instanciado!");                     // Há um problema na lógica do jogo! 
    else
        instance = this;

    // Verifica se SDL_Init falhou. Em caso afirmativo, aborta o programa com uma mensagem de erro.
    // SDL_GetError é usado para saber a causa do erro.
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER) != 0) {             //SDL_Init inicializa a biblioteca SDL e suas auxiliares
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    }
    
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);           // Inicializa a biblioteca de imagens

    // Inicializa a biblioteca de sons
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 1024) != 0) {
		SDL_Log("Unable to initialize Mix_OpenAudio: %s", SDL_GetError());
	}
	if (Mix_Init(MIX_INIT_OGG) == 0) {
		SDL_Log("Unable to initialize Mix_Init: %s", SDL_GetError());
	}
	Mix_AllocateChannels(32);

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);      // Cria uma janela

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);       // Cria um renderizador para a janela

    state = new State();     // Inicializa o membro state
}

//  Destrutor
Game::~Game(){
    delete state;                     //Deleta o estado

    SDL_DestroyRenderer(renderer);    // Destrói o renderizador
    SDL_DestroyWindow(window);        // e a janela


    Mix_CloseAudio();                 // Encerra a SDL_Music
    Mix_Quit();

    IMG_Quit();                       // Encerra a SDL_IMG

    SDL_Quit();                       // Encerra a SDL
}

State& Game::GetState (){
    return *state;
}

SDL_Renderer* Game::GetRenderer (){
    return renderer;
}

/* Game Loop
** 1. Verifica, controla e carrega as telas de jogo;
** 2. Os dados de input são recebidos e processados;
** 3. Os objetos tem seus respectivos estados (posição, atualizados;
** 4. Os objetos são desenhados na tela.*/
void Game::Run (){
    while(state->QuitRequested() != true){
        state->Update(0);
        state->Render();

        SDL_RenderPresent(renderer);    // Atualiza a tela com as últimas renderizações feitas

        SDL_Delay(33);
    }
}

/************************************************** STATE ***************************************************/
State::State (){
    quitRequested = false;

    LoadAssets();
    music.Play();
}

void State::LoadAssets (){
    bg.Open("assets/img/ocean.jpg");
	music.Open("assets/audio/stageState.ogg");
}

void State::Update (float dt){
    if (SDL_QuitRequested() == true)    // Verifica se o usuário encerrou o jogo
        quitRequested = true;
}

void State::Render (){
    bg.Render(0,0);
}

bool State::QuitRequested (){
    return quitRequested;
}

/************************************************** SPRITE **************************************************/
// Construtor
Sprite::Sprite (){
    texture = nullptr;  // Imagem não carregada
}

Sprite::Sprite (std::string file){
    texture = nullptr;
    Open(file);
}

// Destrutor
Sprite::~Sprite (){
    if(texture != nullptr)
        SDL_DestroyTexture(texture);    // Desaloca imagem
}

void Sprite::Open (std::string file){
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();

    if(texture != nullptr)              // Checa se já há alguma imagem carregada em texture
        SDL_DestroyTexture(texture);    // Desaloca imagem  
    

    texture = IMG_LoadTexture(renderer, file.c_str());
    if (texture == nullptr) {             
        SDL_Log("Unable to load texture: %s", SDL_GetError());
    }

    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    SetClip(0,0,width,height);
}

void Sprite::SetClip (int x, int y, int w, int h){
    clipRect = {x,y,w,h};

}

void Sprite::Render (int x, int y){
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();

    SDL_Rect dstrect = {x,y,width,height}; 

    SDL_RenderCopy(renderer, texture, &clipRect, &dstrect);
}

int Sprite::GetWidth (){
    return width;
}

int Sprite::GetHeight (){
    return height;
}

bool Sprite::IsOpen(){
    if(texture != nullptr)
        return true;
    else
        return false;
}
/************************************************** MUSIC ***************************************************/
Music::Music (){
    music = nullptr; 
}

Music::Music (std::string file){
    music = nullptr;
    Open(file);
}

void Music::Play (int times){
    Mix_PlayMusic(music, times);
}

void Music::Stop (int msToStop){
    Mix_FadeOutMusic(msToStop);
}

void Music::Open (std::string file){
	music = Mix_LoadMUS(file.c_str());
    if (music == nullptr) {             
        SDL_Log("Unable to load music: %s", SDL_GetError());
    }
}

bool Music::IsOpen (){
    if(music != nullptr)
        return true;
    else 
        return false;
}

Music::~Music (){
    Stop();
    Mix_FreeMusic(music);
    music = nullptr;
}
