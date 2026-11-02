#pragma once

#include "head.h"
#include "Player.h"

#define W 800
#define H 600

class Engine
{
private:
    bool running = false;
    SDL_Window *win = nullptr;
    SDL_Renderer *rdr = nullptr;//win和rdr必须在一个主线程创建
    
    Player player;

    void deinit();

    void render();
    
    void handle_events();

    void update(float delta_time);

    // time 
    Uint64 m_lastTick = 0;
    Uint64 m_frame_start = 0;
    float m_delta_time = 0.0f;

    void beginFrame();

    void endFrame();
    // time end 

public:

    bool init();

    void run();
       
    ~Engine();
};
