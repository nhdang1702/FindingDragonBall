#include"stdafx.h"
#include"ThreatObj.h"

ThreatsObject :: ThreatsObject()
{
	width_frame_ = 0;
	height_frame_ = 0;
	
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 0.0;
	y_pos_ = 0.0;
	
	on_ground_ = 0;
	come_back_time = 0;
	
	frame_ = 0;

	animation_a_ = 0;
	animation_b_ = 0;

	input_type_.left_ = 0;

	type_movement = STATIC_THREAT;
}

ThreatsObject :: ~ThreatsObject()
{
	
}

bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / THREAT_FRAME;
		height_frame_ = rect_.h;
	}
	return ret;
}

SDL_Rect ThreatsObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;
	return rect;
}



void ThreatsObject::SetCLips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		for (int i = 0; i < THREAT_FRAME ; i++)
		{
			frame_clip_[i].x = i * width_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
		}

	}
}

void ThreatsObject::Display(SDL_Renderer* des)
{
	if (come_back_time == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;
		frame_++;
		if (frame_ >= 8)
		{
			frame_ = 0;
		}

		SDL_Rect* currentClip = &frame_clip_[frame_];
		SDL_Rect rendrQuad = { rect_.x, rect_.y ,width_frame_,height_frame_ };

		SDL_RenderCopy(des, p_object_, currentClip, &rendrQuad);
	}

}

void ThreatsObject::DoThreats(Map& g_map)
{
	if (come_back_time == 0)
	{
		x_val_ = 0;
		y_val_ += 0.8;
		if (y_val_ > THREAT_MAX_FALL_SPEED)
		{
			y_val_ = THREAT_MAX_FALL_SPEED;
		}
		if (input_type_.left_ == 1)
		{
			x_val_ -= THREAT_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += THREAT_SPEED;
		}
		CheckToMap(g_map);
	}
	else if (come_back_time > 0)
	{
		come_back_time--;
		if (come_back_time == 0)
		{
			InitThreat();
		}
	}
}

void ThreatsObject::InitThreat()
{
	x_val_ = 0;
	y_val_ = 0;
	if (x_pos_ > 256) // = 4 TILES
	{
		x_pos_ -= 256;
		animation_a_ -= 256;
		animation_b_ -= 256;
	}
	else
	{
		x_pos_ = 15;
	}
	y_pos_ = 300;
	come_back_time = 0;
	input_type_.left_ = 1;
}

bool ThreatsObject::contains(int tiles_value)
{
	if ((tiles_value > 161 || tiles_value < 145)
		&& (tiles_value < 64 || tiles_value > 67) && (tiles_value < 93 || tiles_value > 96)
		&& tiles_value != 37 && tiles_value != 124 && (tiles_value < 35 || tiles_value > 38)
		&& tiles_value != 146 && tiles_value != 175 && tiles_value != 60
		&& tiles_value != 162 && tiles_value != 191 && tiles_value != 59
		&& tiles_value != 1 && tiles_value != 2 && tiles_value != 3
		&& tiles_value != 4 && tiles_value != 30 && tiles_value != 33
		&& tiles_value != 59 && tiles_value != 62 && tiles_value != 151
		&& tiles_value != 204 && tiles_value != 233 && tiles_value != 290
		&& tiles_value != 220 && tiles_value != 249 && tiles_value != 0 )
	{
		return true;
	}
	else return false;
}

void ThreatsObject::CheckToMap(Map& g_map)
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
		if (x_val_ > 0) //move right
		{
			if (contains(g_map.tile[y1][x2]) || contains(g_map.tile[y2][x2]))
			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_val_ = 0;
			}
		}
		else if (x_val_ < 0)
		{
			if (contains(g_map.tile[y1][x1]) || contains(g_map.tile[y2][x1]))
			{
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
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
			if (contains(g_map.tile[y2][x1]) || contains(g_map.tile[y2][x2]))//g_map.tile[y2][x1] != BLANK_TILE || g_map.tile[y2][x2] != BLANK_TILE)
			{
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (height_frame_ + 1);
				y_val_ = 0;
				on_ground_ = true;
			}
		}
		else if (y_val_ < 0)
		{
			if (contains(g_map.tile[y1][x1]) || contains(g_map.tile[y1][x2]))//g_map.tile[y1][x1] != BLANK_TILE || g_map.tile[y1][x2] != BLANK_TILE)
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}
		}
	}
	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > g_map.max_x_)
	{
		x_pos_ = g_map.max_x_ - width_frame_ - 1;
	}
	if (y_pos_ > g_map.max_y_)
	{
		come_back_time = 60;
	}
}


// MOVING THREAT

void ThreatsObject::ImpMovement(SDL_Renderer* screen)
{
	if (type_movement == STATIC_THREAT) 
	{
		;
	}
	else 
	{
		if (on_ground_ == true)
		{
			if (x_pos_ > animation_b_) // quai vat di qua diem b thi quay lai ben trai
			{
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				LoadImg("img//threat_left1.png", screen);
			}
			else if (x_pos_ < animation_a_)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 1;
				LoadImg("img//threat_right1.png", screen);
			}
		}
		else
		{
			if (input_type_.left_ == 1)
			{
				LoadImg("img//threat_left1.png", screen);
			}
		}
	}
}

void ThreatsObject::InitBullet(BulletObj* t_bullet, SDL_Renderer* screen)
{
	if(t_bullet != NULL)
	{
		t_bullet->LoadImg("img//laser_bullet.png", screen);
		t_bullet->set_is_moving(true);
		t_bullet->set_bullet_dir(BulletObj::DIR_LEFT);
		t_bullet->SetRect(x_pos_ + 25, y_pos_ + 20);
		t_bullet->set_x_val(15);
		t_bullet_list_.push_back(t_bullet);
	}
}

void ThreatsObject::MakeBullet(SDL_Renderer* screen, const int& t_x_border, const int& t_y_border)
{
	for (int i = 0; i < t_bullet_list_.size(); ++i)
	{
		BulletObj* t_bullet = t_bullet_list_.at(i);
		if (t_bullet != NULL)
		{
			if (t_bullet->get_is_moving())
			{
				int bullet_distance = rect_.x - t_bullet->GetRect().x + width_frame_;
				if (bullet_distance < THREAT_BULLET_DISTANCE && bullet_distance > 0)
				{
					t_bullet->HandleMove(t_x_border, t_y_border);
					t_bullet->Render(screen);
				}
				else 
				{
					t_bullet->set_is_moving(false);
				}
			}
			else
			{
				t_bullet->set_is_moving(true);
				t_bullet->SetRect(rect_.x + 25, rect_.y + 20);
			}
		}
	}
}

