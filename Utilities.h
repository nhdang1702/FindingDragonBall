#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class IMG_DISPLAY : public BaseObject
{
public:
	IMG_DISPLAY();
	~IMG_DISPLAY();


	void setNum(static int& NUM) { num = NUM; }
	//void addPos(const int& Pos);
	void Show(SDL_Renderer* screen);
	void Init(SDL_Renderer* screen);

	//void Increase();
	//void Decrease();

private:
	int pos[5];
	int num;

};

class TextObj
{
public:
	TextObj();
	~TextObj();

	bool LoadToRender(TTF_Font* font, SDL_Renderer* screen);
	void Free();

	void SetColor(Uint8 r, Uint8 g, Uint8 b);

	void RenderText(SDL_Renderer* screen, int xp, int yp, SDL_Rect* clip = NULL,
		double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void SetText(const std::string text) { str_ = text; }

private:
	std::string str_;
	SDL_Color text_color;
	SDL_Texture* texture_;
	int width;
	int height;

};


#endif // !1

