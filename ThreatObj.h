#ifndef THREATS_OBJ_H_
#define THREATS_OBJ_H_

#include"stdafx.h"
#include"CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define THREAT_FRAME 8
#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 3

#define THREAT_BULLET_DISTANCE 360

class ThreatsObject : public BaseObject
{
public:
	ThreatsObject();
	~ThreatsObject();

	enum ThreatType
	{
		STATIC_THREAT = 0,
		MOVING_THREAT = 1,
  	};

	void SetXVal(const float& xVal) { x_val_ = xVal; }
	void SetYVal(const float& yVal) { y_val_ = yVal; }
	void SetXPos(const float& xPos) { x_pos_ = xPos; }
	void SetYPos(const float& yPos) { y_pos_ = yPos; }
	float GetXPos() const { return x_pos_; }
	float GetYPos() const { return y_pos_; }
	void SetMapXY(const int& mapX, const int& mapY) { map_x_ = mapX, map_y_ = mapY; }
	void SetCLips();
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Display(SDL_Renderer* des);
	int GetHeightFrame() const { return height_frame_; }
	int GetWidthFrame() const { return width_frame_; }
	SDL_Rect GetRectFrame();

	void InitThreat();
	void DoThreats(Map& g_map_);
	void CheckToMap(Map& g_map_);
	bool contains(int tiles_value);

	void set_type_move(const int& typeMove) { type_movement = typeMove; }
	void set_animation_pos(const int& pos_a, const int& pos_b) { animation_a_ = pos_a, animation_b_ = pos_b; }
	void set_input_left(const int& ip_left) { input_type_.left_ = ip_left; }
	void ImpMovement(SDL_Renderer* screen);

	//SHOOTING THREATS IMP BULLET
	std::vector<BulletObj*> get_bullet_list() const {return t_bullet_list_; }
	void set_bullet_list(const std::vector<BulletObj*>& tBlList) { t_bullet_list_ = tBlList; }
	void InitBullet(BulletObj* t_bullet, SDL_Renderer* screen);
	void MakeBullet(SDL_Renderer* screen, const int& t_x_border, const int& t_y_border);
	void RemoveBulletHit(const int& idx);
private:

	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;

	int map_x_;
	int map_y_;

	int come_back_time;

	SDL_Rect frame_clip_[THREAT_FRAME];
	int width_frame_;
	int height_frame_;
	int frame_;

	int on_ground_;
	//MOVING THREAT
	int type_movement;
	int animation_a_;
	int animation_b_;
	
	Input input_type_;

	//imp bullet
	std::vector<BulletObj*> t_bullet_list_;


};




#endif // !THREATS_OBJ_H_

