#include"Engine.h"

bool Engine::init()
    {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        SDL_Log(
            "Init failed:%s",SDL_GetError());
        return false;
    }
    //create window
    win = SDL_CreateWindow(
        "demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        W,
        H,
        SDL_WINDOW_RESIZABLE);
    if(NULL == win)
    {
        SDL_Log(
            "SDL_CreateWindow failed:%s",SDL_GetError());
        deinit();
        return false;
    }
    //create renderer
    rdr = SDL_CreateRenderer(win,-1,0);
    if(NULL == rdr)
    {
        SDL_Log(
            "SDL_CreateRenderer failed:%s",SDL_GetError());
        deinit();
        return false;
    }
 
    running = true;
    return true;
}

void Engine::deinit()
{
     if (rdr)
    {
        SDL_DestroyRenderer(rdr);
        rdr = nullptr;
    }
    if (win)
    {
        SDL_DestroyWindow(win);
        win = nullptr;
    }
    SDL_Quit();
}
       
void Engine::render()
{
    SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
    SDL_RenderClear(rdr);

    player.Render(rdr);

    // 整帧绘制完成后，只提交一次
    SDL_RenderPresent(rdr);
}

void Engine::handle_events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
            break;
        
        case SDL_KEYUP:
            break;

        case SDL_MOUSEBUTTONDOWN:
            SDL_Log(
                "Mouse: x=%d, y=%d",
                event.button.x,
                event.button.y
            );
            break;

        default:
            break;
        }

        player.Handle_event(event);
    }
}

void Engine::update(float delta_time)
{
    player.Updata(delta_time);
}

void Engine::run()
{
    m_lastTick = SDL_GetTicks64();

    while (running)
    {
        beginFrame();
        
        handle_events();
        update(m_delta_time);
        render();

        endFrame();             
    }
}

void Engine::beginFrame()
{
    m_frame_start = SDL_GetTicks64();
    m_delta_time = static_cast<float>(m_frame_start - m_lastTick) / 1000.0f;
    m_lastTick = m_frame_start;
}

void Engine::endFrame()
{
    //fps = 60
    constexpr Uint64 target_frame_time = 1000 / 60;

    Uint64 frame_time = SDL_GetTicks64() - m_frame_start;

    if (frame_time < target_frame_time)
    {
        SDL_Delay(
            static_cast<Uint32>(target_frame_time - frame_time)
        );
    }
}

Engine::~Engine()
{
    deinit();
}


