#pragma once

#include "head.h"

class Player
{
private:
    SDL_Rect m_player;
    float m_speed;
    float m_player_X;
    float m_player_Y;

    bool m_up = false;
    bool m_down = false;
    bool m_left = false;
    bool m_right = false;

    SDL_Surface *img_surf = SDL_LoadBMP("trollface_24.bmp");

    void SetPosition(float x, float y);

    void Move(float dx, float dy, float deltaTime);

    void Collision(int W,int H);

public:
    Player();

    const SDL_Rect& GetPlayerRect() const
    {
        return m_player;
    }

    void Updata(float delta_time);

    void Render(SDL_Renderer* renderer) const;

    void Handle_event(const SDL_Event& event);

    ~Player();
};