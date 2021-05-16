#include "stdafx.h"
#include "MainObj.h"

MainObj::MainObj()
{
	frame_ = 0;
	
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = 100;
	y_pos_ = 200;
	
	width_frame_ = 0;
	height_frame_ = 0;
	
	status_ = STAND;
	
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.down_ = 0;
	//input_type_.atk_ = 0;

	on_ground_ = false;
	
	map_x_ = 0;
	map_y_ = 0;

	come_back_time_ = 0;
    money_count = 0;
	fall_count = 1;
	balls_count = 0;
}

MainObj::~MainObj()
{

}

bool MainObj::LoadImg(std::string path, SDL_Renderer* screen) 
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true)
	{
		width_frame_ = rect_.w / MAX_FRAME_CLIP;
		height_frame_ = rect_.h;
	}

	return ret;
}

SDL_Rect MainObj::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;
	return rect;
}

void MainObj::Set_Clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		for (int i = 0; i < MAX_FRAME_CLIP; i++)
		{
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}

	}
}

void MainObj::Display(SDL_Renderer* des)
{
	if(input_type_.left_ == 1 || input_type_.right_ == 1)// || input_type_.atk_ == 1)
	{
		frame_++;
	}
	else
	{
		frame_ = 0;
	}

	if (frame_ >= MAX_FRAME_CLIP)
	{
		frame_ = 0;
	}
	if (come_back_time_ == 0) 
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect* current_clip = &frame_clip_[frame_];

		SDL_Rect renderQuad = { rect_.x , rect_.y , width_frame_ , height_frame_ };

		SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
	}
}

void MainObj::HandleInputAction(SDL_Event events, SDL_Renderer* screen, 
								Mix_Chunk* bullet_sound)
{

	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = MOVE_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			UpdatePlayerImg(screen);
		}
		break;
		case SDLK_LEFT:
		{
			status_ = MOVE_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			UpdatePlayerImg(screen);
		}
		break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			input_type_.right_ = 0;
		}
		break;
		case SDLK_LEFT:
		{
			input_type_.left_ = 0;
		}
		break;
		}
	}

	if (events.type == SDL_MOUSEBUTTONDOWN)
	{
		if (events.button.button == SDL_BUTTON_RIGHT)
		{
			input_type_.jump_ = 1;
		}
		else if (events.button.button == SDL_BUTTON_LEFT) 
		{

            BulletObj* p_bullet = new BulletObj();

            p_bullet->LoadImg("img//spr_bullet_strip.png",screen);

            if(status_ == MOVE_LEFT)
            {
                p_bullet->set_bullet_dir(BulletObj::DIR_LEFT);
                p_bullet->SetRect(this->rect_.x , rect_.y + height_frame_ * 0.25);
				Mix_PlayChannel(-1, bullet_sound, 0);
            }
            else if(status_== MOVE_RIGHT)
            {            
                p_bullet->set_bullet_dir(BulletObj::DIR_RIGHT);
                p_bullet->SetRect(this->rect_.x + width_frame_ - 20 , rect_.y + height_frame_ * 0.25);
				Mix_PlayChannel(-1, bullet_sound, 0);

            }
            p_bullet->set_x_val(20);
            p_bullet->set_is_moving(true);

            p_bullet_list_.push_back(p_bullet);

		}
	}
}

void MainObj::HandleBullet(SDL_Renderer* des) // xu li dan ban
{
    for(int i = 0 ; i < p_bullet_list_.size() ; i++)
    {
        BulletObj* p_bullet = p_bullet_list_.at(i);
        if(p_bullet != NULL)
        {
            if(p_bullet->get_is_moving() == true)
            {
                p_bullet->HandleMove(1280 ,640);
                p_bullet->Render(des);
            }
            else 
            {
                p_bullet_list_.erase(p_bullet_list_.begin() + i );
                if(p_bullet != NULL)
                {
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }

    }
}

void MainObj::RemoveBulletHit(const int& idx)
{
	int size = p_bullet_list_.size();
	if (size > 0 && idx < size)
	{
		BulletObj* p_bullet = p_bullet_list_.at(idx);
		p_bullet_list_.erase(p_bullet_list_.begin() + idx);

		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

void MainObj::DoPlayer(Map& map_data ,Mix_Chunk* fall_sound, Mix_Chunk* coin_sound )
{
	if (come_back_time_ == 0) {
		x_val_ = 0;
		y_val_ += 0.8;

		if (y_val_ >= MAX_FALL_SPEED)
		{
			y_val_ = MAX_FALL_SPEED;
		}

		if (input_type_.left_ == 1)
		{
			x_val_ -= PLAYER_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += PLAYER_SPEED;
		}
		if (input_type_.jump_ == 1)
		{
			if (on_ground_ == true)
			{
				y_val_ = -PLAYER_JUMP_VALUE;
				on_ground_ = false;
			}
			input_type_.jump_ = 0;
		}
		CheckToMap(map_data, fall_sound , coin_sound);
		CenterEntityOnMap(map_data);
	}
	if (come_back_time_ > 0)
	{
		come_back_time_--;
		if(come_back_time_ == 0)
		{
			if (x_pos_ > 256)
			{
				x_pos_ -= 256;
			}
			else 
			{
				x_pos_ = 15;
			}
			y_pos_ = 300;
			x_val_ = 0;
			y_val_ = 0;

		}
	}
}

void MainObj::CenterEntityOnMap(Map& map_data)
{
	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);
	if (map_data.start_x_ < 0)
	{
		map_data.start_x_ = 0;
	}
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
	{
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
	}

	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map_data.start_y_ < 0)
	{
		map_data.start_y_ = 0;
	}
	else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
	{
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}
}
bool MainObj :: contains(int tiles_value)
{
	if ((tiles_value > 161 || tiles_value < 145 )
		&& (tiles_value < 64 || tiles_value > 67) && (tiles_value < 93 || tiles_value > 96)
		&& tiles_value != 37 && tiles_value != 124 && (tiles_value < 35 || tiles_value > 38)
		&& tiles_value != 146 && tiles_value != 175 && tiles_value != 60
		&& tiles_value != 162 && tiles_value != 191 && tiles_value != 59
		&& tiles_value != 1 && tiles_value != 2 && tiles_value != 3 && tiles_value != 243
		&& tiles_value != 4 && tiles_value != 30 && tiles_value != 33
		&& tiles_value != 59 && tiles_value != 62 && tiles_value != 151
		&& tiles_value != 204 && tiles_value != 233 && tiles_value != 127 && tiles_value != 128
		&& tiles_value != 129 && tiles_value != 130 && tiles_value != 239
		&& tiles_value != 220 && tiles_value != 249  && tiles_value != 0)
	{
		return true;
	}
	else return false;
}

void MainObj::IncreaseMoney()
{
    money_count++;
}

void MainObj::IncreaseLife()
{
	if (fall_count >= 1)
	{
		fall_count--;
		if (fall_count < 1)
		{
			fall_count = 1;
		}
	}
}

void MainObj::IncreaseFallCount()
{
	fall_count++;
}

void MainObj::IncreaseBalls()
{
	balls_count++;
}


void MainObj::CheckToMap(Map& map_data , Mix_Chunk* fall_sound , Mix_Chunk* coin_sound)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	//Check horizontal
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
	
	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (x_val_ > 0) //player move right
		{
            if(map_data.tile[y1][x2] == MONEY_TILE || map_data.tile[y2][x2] == MONEY_TILE )
            {
                 map_data.tile[y1][x2] = 0;
                 map_data.tile[y2][x2] = 0;
                 IncreaseMoney();
				 Mix_PlayChannel(-1, coin_sound, 0);
            }

			else if (map_data.tile[y1][x2] == LIFE_TILE || map_data.tile[y2][x2] == LIFE_TILE)
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseLife();
				Mix_PlayChannel(-1, coin_sound, 0);
			}
			else if (map_data.tile[y1][x2] == BALLS_TILE || map_data.tile[y2][x2] == BALLS_TILE)
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseBalls();
				Mix_PlayChannel(-1, coin_sound, 0);
			}

            else 
            {
			    if (contains(map_data.tile[y1][x2]) || contains(map_data.tile[y2][x2]))
			    {
				    x_pos_ = x2 * TILE_SIZE;
				    x_pos_ -= width_frame_ + 1;
				    x_val_ = 0;
			    }
            }
		}
		else if (x_val_ < 0) 
		{
            if(map_data.tile[y1][x1] == MONEY_TILE || map_data.tile[y2][x1] == MONEY_TILE )
            {
                 map_data.tile[y1][x1] = 0;
                 map_data.tile[y2][x1] = 0;
                 IncreaseMoney();
				 Mix_PlayChannel(-1, coin_sound, 0);
            }
			else if (map_data.tile[y1][x1] == LIFE_TILE || map_data.tile[y2][x1] == LIFE_TILE)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				IncreaseLife();
				Mix_PlayChannel(-1, coin_sound, 0);
			}
			else if (map_data.tile[y1][x1] == BALLS_TILE || map_data.tile[y2][x1] == BALLS_TILE)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				IncreaseBalls();
				Mix_PlayChannel(-1, coin_sound, 0);
			}
            else 
            {
			    if (contains(map_data.tile[y1][x1]) || contains(map_data.tile[y2][x1]))
			    {
				    x_pos_ = (x1 + 1) * TILE_SIZE;
				    x_val_ = 0;
			    }
            }
		}
	}


	//CHECK VERTICAL
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val_ > 0) 
		{
            if(map_data.tile[y2][x1] == MONEY_TILE || map_data.tile[y2][x2] == MONEY_TILE )
            {
                 map_data.tile[y2][x2] = 0;
                 map_data.tile[y2][x2] = 0;
                 IncreaseMoney();
				 Mix_PlayChannel(-1, coin_sound, 0);
            }
			else if (map_data.tile[y2][x1] == LIFE_TILE || map_data.tile[y2][x2] == LIFE_TILE)
			{
				map_data.tile[y2][x2] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseLife();
				Mix_PlayChannel(-1, coin_sound, 0);
			}
			else if (map_data.tile[y2][x1] == BALLS_TILE || map_data.tile[y2][x2] == BALLS_TILE)
			{
				map_data.tile[y2][x2] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseBalls();
				Mix_PlayChannel(-1, coin_sound, 0);
			}

            else 
            {
			    if (contains(map_data.tile[y2][x1]) || contains(map_data.tile[y2][x2]))
			    {
				    y_pos_ = y2 * TILE_SIZE;
				    y_pos_ -= (height_frame_ + 1);
				    y_val_ = 0;
				    on_ground_ = true;
			    }
            }
		}
		else if (y_val_ < 0)
		{
            if(map_data.tile[y1][x1] == MONEY_TILE || map_data.tile[y1][x2] == MONEY_TILE )
            { 
                 map_data.tile[y1][x1] = 0;
                 map_data.tile[y1][x2] = 0;
                 IncreaseMoney();
				 Mix_PlayChannel(-1, coin_sound, 0);
            }
			
			else if (map_data.tile[y1][x1] == LIFE_TILE || map_data.tile[y1][x2] == LIFE_TILE)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				IncreaseLife();
				Mix_PlayChannel(-1, coin_sound, 0);
			}
			else if (map_data.tile[y1][x1] == BALLS_TILE || map_data.tile[y1][x2] == BALLS_TILE)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				IncreaseBalls();
				Mix_PlayChannel(-1, coin_sound, 0);
			}
            else
            {
			    if (contains(map_data.tile[y1][x1]) || contains(map_data.tile[y1][x2]))
			    {
				    y_pos_ = (y1 + 1) * TILE_SIZE;
				    y_val_ = 0;
			    }
            }
		}
	}
	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0) 
	{
		x_pos_ = 0;
	}
	else if(x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}
	if (y_pos_ > map_data.max_y_)
	{
		IncreaseFallCount();
		come_back_time_ = 60;
		Mix_PlayChannel(-1, fall_sound, 0);
	}
}

void MainObj::UpdatePlayerImg(SDL_Renderer* des)
{
	if (on_ground_ == true)
	{
		if (status_ == MOVE_LEFT)
		{
			LoadImg("img//player_left.png", des);
		}
		else if (status_ == MOVE_RIGHT)
		{
			LoadImg("img//player_right.png", des);
		}
	}

	else if(on_ground_ == false)
	{
		if (status_ == MOVE_LEFT)
		{
			LoadImg("img//jum_left.png", des);
		}
		else if (status_ == MOVE_RIGHT)
		{
			LoadImg("img//jum_right.png", des);
		
	}
}

}