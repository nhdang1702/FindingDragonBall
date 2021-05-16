#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include "stdafx.h"'
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define GRAVITY_SPEED 0.9
#define MAX_FALL_SPEED 12
#define PLAYER_SPEED 5.5
#define PLAYER_JUMP_VALUE 19
#define MAX_FRAME_CLIP 8
#define MONEY_TILE 290
#define LIFE_TILE 292
#define BALLS_TILE 294


class MainObj : public BaseObject
{
public:
    MainObj();
    ~MainObj();

    enum MovementType
    {
        MOVE_RIGHT = 1,
        MOVE_LEFT = 2,
        STAND = 0,
        P_ATTACK = 3,
    };


    bool LoadImg(std :: string path, SDL_Renderer* screen);
    void Display(SDL_Renderer* des);
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound); // xu li su kien
    void Set_Clips(); // hieu ung
    SDL_Rect GetRectFrame(); // lay rect cua tam anh do

    void DoPlayer(Map& map_data, Mix_Chunk* fall_sound, Mix_Chunk* coin_sound);
    void CheckToMap(Map& map_data, Mix_Chunk* fall_sound , Mix_Chunk* coin_sound);
    void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }
    void CenterEntityOnMap(Map& map_data);  // tinh toan ra thong so map khi di chuyen nvat
    void UpdatePlayerImg(SDL_Renderer* des);
    
    bool contains(int tiles_value);
    
    void set_bullet_list(std :: vector <BulletObj*> bullet_list)
    {
        p_bullet_list_ = bullet_list;
    }
    std :: vector <BulletObj*> get_bullet_list() const {return p_bullet_list_;}
    void HandleBullet(SDL_Renderer* des );
    
    void RemoveBulletHit(const int& idx);

    void IncreaseMoney();
    void IncreaseLife();
    void IncreaseFallCount();
    void IncreaseBalls();


    int get_width_frame() const { return width_frame_; }
    int get_height_frame() const { return height_frame_; }

	int get_balls() const { return balls_count;}
    int get_fall_count() const {return fall_count; }
    int get_coin_count() const { return money_count; }
    void set_comeback_time(const int& cb_time) { come_back_time_ = cb_time;}

private:
    std :: vector <BulletObj*> p_bullet_list_;

    float x_val_; // 1 luong tang len bao nhieu
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;
    
    SDL_Rect frame_clip_[MAX_FRAME_CLIP];
    Input input_type_;
    int frame_;
    int status_;
    int stand_status_;
    bool on_ground_;

    int map_x_;
    int map_y_;
    
    int come_back_time_;

    int money_count;
    int fall_count;
	int balls_count;
};

#endif