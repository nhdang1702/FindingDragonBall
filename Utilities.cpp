#include"stdafx.h"
#include"Utilities.h"

IMG_DISPLAY::IMG_DISPLAY()
{
	num = 3;
}

IMG_DISPLAY::~IMG_DISPLAY()
{
	
}



void IMG_DISPLAY::Init(SDL_Renderer* screen)
{
	LoadImg("img//player_life.png", screen);
	
	for (int j = 0; j < num ; ++j)
	{
		pos[j] = 20 + j * 40;
	}
}

void IMG_DISPLAY::Show(SDL_Renderer* screen)
{
	for (int i = 0; i < num  ; i++)
	{
		rect_.x = pos[i];
		rect_.y = 0;
		Render(screen);
	}
}


TextObj::TextObj()
{
	text_color.r = 255;
	text_color.g = 255;
	text_color.b = 255;

	texture_ = NULL;
}

TextObj::~TextObj()
{
	
}

bool TextObj::LoadToRender(TTF_Font* font, SDL_Renderer* screen)
{
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_.c_str(), text_color);
	if (text_surface)
	{
		texture_ = SDL_CreateTextureFromSurface(screen, text_surface);
		width = text_surface->w;
		height = text_surface->h;

		SDL_FreeSurface(text_surface);
	}
	return texture_ != NULL;
}

void TextObj::Free()
{
	if (texture_ != NULL)
	{
		SDL_DestroyTexture(texture_);
		texture_ = NULL;
	}
}

void TextObj::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
	text_color.r = r;
	text_color.g = g;
	text_color.b = b;
}

void TextObj::RenderText(SDL_Renderer* screen, int xp, int yp, SDL_Rect* clip ,
	double angle , SDL_Point* center , SDL_RendererFlip flip )
{
	SDL_Rect RenderQuad = { xp,yp,width,height };
	if (clip != NULL)
	{
		RenderQuad.w = clip->w;
		RenderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(screen, texture_, clip, &RenderQuad, angle, center, flip);
}

