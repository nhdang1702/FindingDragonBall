#ifndef _EXP_FR_
#define _EXP_FR_

#include"stdafx.h"
#include"CommonFunc.h"
#include"BaseObject.h"

#define MAX_FRAMES 8

class ExplosionFrames : public BaseObject
{
public:
	ExplosionFrames();
	~ExplosionFrames();

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void set_clips();
	void set_frames(const int& fr) { frames_ = fr; }
	int get_frame_width() const { return frame_width; }
	int get_frame_height() const { return frame_height; }

	void Display(SDL_Renderer* des);

private:
	int frame_width;
	int frame_height;
	unsigned int frames_;

	SDL_Rect frame_clip_[8];
};


#endif // !_EXP_FR_

