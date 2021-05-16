#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "CommonFunc.h"
#include "stdafx.h"

class BaseObject
{
public:
	BaseObject();
	~BaseObject();
	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);


	void Free();
	void SetRect(const int& x, const int& y);
	SDL_Rect GetRect() const { return rect_; } // lay ra rect cua tam anh
	SDL_Texture* GetObject() { return p_object_; }

protected:
	SDL_Texture* p_object_; //luu tru cac hinh anh
	SDL_Rect rect_; // luu kich thuoc anh
};


#endif