
#include"stdafx.h"
#include"CommonFunc.h"
#include"BaseObject.h"
#include"game_map.h"
#include"MainObj.h"
#include"ImplementTimer.h"
#include"ThreatObj.h"
#include"ExplosionFrames.h"
#include"Utilities.h"

BaseObject g_background;
TTF_Font* time_font;

bool Init()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("FIND DRAGONBALL NHD",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);

    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR
                                    , RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }

        if (TTF_Init() == -1)
        {
            success = false;
        }
        time_font = TTF_OpenFont("font//dlxfont_.ttf", 15);
        if (time_font == NULL)
        {
            success = false;
        }
    }
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        success = false;
    }

    g_music = Mix_LoadMUS("sound//background.mp3");
    g_bullet_sound[1] = Mix_LoadWAV("sound//Fire1.wav");
    g_bullet_sound[2] = Mix_LoadWAV("sound//Laser.wav");
    g_playerdie_sound = Mix_LoadWAV("sound//ha.wav");
    g_explosion_sound = Mix_LoadWAV("sound//exp.wav");
    g_coin_sound = Mix_LoadWAV("sound//coin.wav");

    if (g_music == NULL || g_bullet_sound[1] == NULL || g_bullet_sound[2] == NULL
        || g_playerdie_sound == NULL || g_explosion_sound == NULL || g_coin_sound == NULL)
    {
        success = false;
    }

    return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//background.png", g_screen);
    if (ret == false)
        return false;

    return true;
}

void Close()
{

    g_background.Free();
  
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

std::vector<ThreatsObject*> MakeThreatList()
{
    std::vector<ThreatsObject*> threat_list;
    
    //MOVING THREAT
    ThreatsObject* dynamic_threat = new ThreatsObject[30];
    for (int i = 0; i < 30; i++) 
    {
        ThreatsObject* p_threat = (dynamic_threat + i);
        if (p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_left1.png", g_screen);
            p_threat->SetCLips();
            p_threat->set_type_move(ThreatsObject::MOVING_THREAT);
            p_threat->SetXPos(500 + i * 1150);
            p_threat->SetYPos(250);

            int pos1 = p_threat->GetXPos() - 60;
            int pos2 = p_threat->GetXPos() + 60;
            
            p_threat->set_animation_pos(pos1, pos2);
            p_threat->set_input_left(1);
            threat_list.push_back(p_threat);
        }
    }

    //STATIC_THREAT_CAN_SHOOT   
    ThreatsObject* threat_objs = new ThreatsObject[20];
    for (int i = 0; i < 20; i++)
    {
        ThreatsObject* p_threat = (threat_objs + i);
        if (p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_left.png", g_screen);
            p_threat->SetCLips();
            p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
            p_threat->set_input_left(0);
            p_threat->SetXPos(700 + i * 1200);
            p_threat->SetYPos(250);

            BulletObj* t_bullet = new BulletObj;
            p_threat->InitBullet(t_bullet, g_screen);
            
            threat_list.push_back(p_threat);
            
        }
    }
    return threat_list;

}

int main(int argc, char* argv[])
{   
    if (Init() == false)
        return -1;

    Mix_PlayMusic(g_music, -1);

    g_background.LoadImg("img//bg.png", g_screen);
    g_background.Render(g_screen, NULL);
    SDL_RenderPresent(g_screen);

    if (LoadBackground() == false)
        return -1;

    bool is_quit = false;
    int start_time = 0;
    while (SDL_WaitEvent(&g_event))
    {
        if (g_event.type == SDL_QUIT)
        {
            is_quit = true;
            Close();
        }
        else if (g_event.type == SDL_KEYDOWN)
        {
            if (g_event.key.keysym.sym)
            {
                is_quit = false;
                start_time = SDL_GetTicks() / 1000;
                break;
            }
        }
        else if (g_event.type == SDL_MOUSEBUTTONDOWN)
        {
            is_quit = false;
            start_time = SDL_GetTicks() / 1000;
            break;
        }
    }
	GameMap game_map;
	bool bRet = game_map.LoadMap("map/map11.dat");
    if (bRet == false)
        return - 1;

	game_map.LoadTiles(g_screen);

    MainObj p_player;
    p_player.LoadImg("img//player_left.png", g_screen);
    p_player.Set_Clips();

    ExplosionFrames exp_threat;
    ExplosionFrames exp_main;

    bool threxp_success = exp_threat.LoadImg("img//exp1.png", g_screen); // vu no
    if (!threxp_success) return -1;
    exp_threat.set_clips();

    bool mainexp_success = exp_main.LoadImg("img//exp1.png", g_screen);
    if (!mainexp_success) return -1;
    exp_main.set_clips();

    int num_life = 3;
    int num_fall = 0;
    int num_bullet_hit = 0;
	int balls_cnt = 0;

    IMG_DISPLAY player_life;
    player_life.Init(g_screen);

    TextObj time_game;
    time_game.SetColor(255, 0, 0);
    int mark_hit = 0;
    TextObj mark_hits;
    mark_hits.SetColor(255, 0, 0);
    TextObj txt_coins;
    int coin_hit = 0;
    txt_coins.SetColor(255, 0, 0);

    std::vector<ThreatsObject*> threat_list = MakeThreatList();
   
    ImpTimer fps_timer;
    
    while(!is_quit)
    {
        fps_timer.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            p_player.HandleInputAction(g_event, g_screen, g_bullet_sound[1]);
        }

        SDL_RenderClear(g_screen);
        g_background.Render(g_screen, NULL);

        Map map_data = game_map.getMap();
        game_map.DrawMap(g_screen);

        player_life.Show(g_screen);
  
        for (int i = 0; i < threat_list.size(); i++)
        {
            ThreatsObject* p_threat = threat_list.at(i);
            if (p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_threat->ImpMovement(g_screen);
                p_threat->DoThreats(map_data);
                p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat->Display(g_screen);
                //Colide t_bullet p_player
                SDL_Rect rect_player = p_player.GetRectFrame();

                bool isCol1 = false;
                std::vector<BulletObj*> threatBulletList = p_threat->get_bullet_list();

                for (int k = 0; k < threatBulletList.size(); ++k)
                {
                    BulletObj* pt_bullet = threatBulletList.at(k);
                    if (pt_bullet != NULL)
                    {
                        isCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
                        if (isCol1 == true)
                        {
                            //p_threat->RemoveBulletHit(k);
                            //break;
                        }
                    }
                }
                //Colide player : threat
                SDL_Rect rect_threat = p_threat->GetRectFrame();
                bool isCol2 = SDLCommonFunc::CheckCollision(rect_threat, rect_player);
                
                //PLAYER DIED
                if ( isCol2 || isCol1)
                {
                    int m_frame_exp_width = exp_main.get_frame_width();
                    int m_frame_exp_height = exp_main.get_frame_height();
                    for (int ex = 0; ex < MAX_FRAMES; ex++)
                    {
                        int x_pos = p_player.GetRectFrame().x + p_player.GetRectFrame().w * 0.5 - m_frame_exp_width * 0.5;
                        int y_pos = p_player.GetRectFrame().y + p_player.GetRectFrame().h * 0.5 - m_frame_exp_height * 0.5;

                        exp_main.set_frames(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Display(g_screen);
                    }
                    num_bullet_hit++; 
                    Mix_PlayChannel(-1, g_playerdie_sound, 0);
                    p_player.SetRect(0, 0);
                    p_player.set_comeback_time(70);
                    continue;
                }
            }
        }

        p_player.HandleBullet(g_screen);
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPlayer(map_data, g_playerdie_sound , g_coin_sound);
        p_player.Display(g_screen);

        game_map.SetMap(map_data);

        
        //COLLISION P_BULLET -> THREAT
        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();

        std::vector<BulletObj*> bullet_arr = p_player.get_bullet_list();
        for (int r = 0; r < bullet_arr.size(); r++)
        {
            BulletObj* p_bullet = bullet_arr.at(r);
            if (p_bullet != NULL)
            {
                for (int t = 0; t < threat_list.size(); t++)
                {
                    ThreatsObject* obj_threat = threat_list.at(t);
                    if (obj_threat != NULL)
                    {
                        SDL_Rect threatRect;
                        threatRect.x = obj_threat->GetRect().x;
                        threatRect.y = obj_threat->GetRect().y;
                        threatRect.w = obj_threat->GetWidthFrame();
                        threatRect.h = obj_threat->GetHeightFrame();

                        SDL_Rect bulletRect = p_bullet->GetRect();
                        bool isColide = SDLCommonFunc::CheckCollision(bulletRect, threatRect);
                        if (isColide)
                        {
                            for (int ex = 0; ex < MAX_FRAMES; ex++)
                            {
                                int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
                                int y_pos = p_bullet->GetRect().y - frame_exp_width * 0.5;

                                exp_threat.set_frames(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Display(g_screen);
                            }
                            mark_hit++;
                            p_player.RemoveBulletHit(r);
                            Mix_PlayChannel(-1, g_explosion_sound, 0);
                            obj_threat->Free();
                            threat_list.erase(threat_list.begin() + t);
                        }
                    }  
                }
            }
        }

        mark_hits.SetText(std::to_string(mark_hit));
        mark_hits.LoadToRender(time_font, g_screen);
        mark_hits.RenderText(g_screen, SCREEN_WIDTH - 410, 10);

        coin_hit = p_player.get_coin_count();
        txt_coins.SetText(std::to_string(coin_hit));
        txt_coins.LoadToRender(time_font, g_screen);
        txt_coins.RenderText(g_screen, SCREEN_WIDTH/2 - 100, 10);

        std::string str_time = "Time: ";
        Uint32 time_ = SDL_GetTicks() / 1000 - start_time;
        std::string str_time_val = std::to_string(time_);
        str_time += str_time_val;
        time_game.SetText(str_time);
        time_game.LoadToRender(time_font, g_screen);
        time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 10);


        num_fall = p_player.get_fall_count();
        num_life = 4 - num_fall - num_bullet_hit;

        player_life.setNum(num_life);

		balls_cnt = p_player.get_balls();


        SDL_RenderPresent(g_screen);

		if (balls_cnt == 1)
        {
            Mix_PauseMusic();
            g_background.Free();
            SDL_RenderClear(g_screen);
            g_background.LoadImg("img//win.png", g_screen);
            g_background.Render(g_screen, NULL);
            SDL_RenderPresent(g_screen);
            SDL_Delay(5000);
            if (MessageBox(NULL, L"YOU WIN", L"INFO", MB_OK | MB_ICONSTOP) == IDOK)
            {
                Close();
                SDL_Quit();
                return 0;
            }

        }

        if (num_life == 0)
        {
            Mix_PauseMusic();
            g_background.Free();
            SDL_RenderClear(g_screen);
            g_background.LoadImg("img//overtest.png", g_screen);
            g_background.Render(g_screen, NULL);
            SDL_RenderPresent(g_screen);
            SDL_Delay(2000);

            if (MessageBox(NULL, L"GAME OVER", L"INFO", MB_OK | MB_ICONSTOP) == IDOK)
            {
                Close();
                SDL_Quit();
                return 0;
            }
        }


        int real_imp_time = fps_timer.get_ticks();
        int time_per_frame = 1000 / FRAME_PER_SECOND;
        if (real_imp_time < time_per_frame)
        {
            int delayed_time = time_per_frame - real_imp_time;
            if(delayed_time >= 0)
                SDL_Delay(delayed_time);
        }
    }
    for (int i = 0; i < threat_list.size(); ++i)
    {
        ThreatsObject* p_threat = threat_list.at(i);
        if (p_threat)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }
    threat_list.clear();

    Close();

    return 0;
}