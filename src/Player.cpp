#include"Player.h"

Player::Player()
{
    m_player.w = 50;
    m_player.h = 50;

    m_player_X = 0.0f;
    m_player_Y = 200.0f;

    m_player.x = static_cast<int>(m_player_X);
    m_player.y = static_cast<int>(m_player_Y);

    m_speed = 2.0f;   
}

void Player::SetPosition(float x, float y)
{
    m_player_X = x;
    m_player_Y = y;

    m_player.x = static_cast<int>(m_player.x);
    m_player.y = static_cast<int>(m_player.y);
}

void Player::Move(float dx, float dy, float delta_time)
{
    m_player_X += dx * m_speed * delta_time;
    m_player_Y += dy * m_speed * delta_time;

    m_player.x = static_cast<int>(m_player_X);
    m_player.y = static_cast<int>(m_player_Y);
}

void Player::Render(SDL_Renderer* renderer) const
{   
    if(NULL == img_surf)
    {
        SDL_Log("fail surf");
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,img_surf);
    if(NULL == texture)
    {
        SDL_Log("fail texture");
    }
    
    SDL_RenderCopy(renderer,texture,NULL,&m_player);

    //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    //SDL_RenderFillRect(renderer, &m_player);
}

void Player::Updata(float delta_time)
{
    Collision(800,600);

    float dx = 0.0f;
    float dy = 0.0f;

    if (m_left)  dx -= 100.0f;
    if (m_right) dx += 100.0f;
    if (m_up)    dy -= 100.0f;
    if (m_down)  dy += 100.0f;

    Move(dx,dy,delta_time);
}

void Player::Handle_event(const SDL_Event& event)
{   
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w: m_up = true;    break;
        case SDLK_s: m_down = true;  break;
        case SDLK_a: m_left = true;  break;
        case SDLK_d: m_right = true; break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w: m_up = false;    break;
        case SDLK_s: m_down = false;  break;
        case SDLK_a: m_left = false;  break;
        case SDLK_d: m_right = false; break;
        }
    }
}

void Player::Collision(int W,int H)
{
    if(m_player_Y + m_player.h> H)
    {
        m_down = false;
    }
    if(m_player_Y < 0)
    {
        m_up = false;
    }
    if(m_player_X + m_player.w > W)
    {
        m_right = false;
    }   
    if(m_player_X < 0)
    {
        m_left = false;
    }
}

Player::~Player()
{
    
}