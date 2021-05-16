#include"stdafx.h"
#include"ExplosionFrames.h"

ExplosionFrames::ExplosionFrames()
{
	frame_width = 0;
	frame_height = 0;
	frames_ = 0;
}

ExplosionFrames::~ExplosionFrames()
{

}

bool ExplosionFrames::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool success = BaseObject::LoadImg(path, screen);
	if (success)
	{
		frame_width = rect_.w / MAX_FRAMES;
		frame_height = rect_.h;
	}
	
	return success;
}


void ExplosionFrames::set_clips()
{
	if (frame_width > 0 && frame_height > 0)
	{
		for (int i = 0; i < MAX_FRAMES; i++)
		{
			frame_clip_[i].x = i * frame_width;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = frame_width;
			frame_clip_[i].h = frame_height;
		}
	}
}

void ExplosionFrames::Display(SDL_Renderer* des)
{
	SDL_Rect* currentClip = &frame_clip_[frames_];
	SDL_Rect renderQuad = { rect_.x , rect_.y , frame_width , frame_height };
	if (currentClip != NULL)
	{
		renderQuad.w = currentClip->w;
		renderQuad.h = currentClip->h;
	}
	SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);

}