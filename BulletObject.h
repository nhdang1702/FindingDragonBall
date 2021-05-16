#ifndef BULLET_OBJ_H_

#define BULLET_OBJ_H_

#include"BaseObject.h"
#include"CommonFunc.h"

class BulletObj : public BaseObject
{
public:
    BulletObj();
    ~BulletObj();

    enum BulletDir
    {
        DIR_RIGHT = 30,
        DIR_LEFT = 31,
    };


    void set_x_val(const int &xVal) {x_val_ = xVal;}
    void set_y_val(const int &yVal) {y_val_ = yVal;}
    void set_is_moving(const bool &isMoving) { is_moving = isMoving;}
    void set_bullet_dir(const unsigned int& bulletDir){bullet_dir = bulletDir;}
    
    int get_x_val() const { return x_val_; }
    int get_y_val() const {return y_val_;}
    unsigned int get_bullet_dir() const {return bullet_dir;}
    
    bool get_is_moving() const {return is_moving;}

    void HandleMove(const int &x_border , const int &y_border);

private:
    int x_val_;
    int y_val_;
    bool is_moving;
    unsigned int bullet_dir;

    unsigned int bullet_type;

};








#endif