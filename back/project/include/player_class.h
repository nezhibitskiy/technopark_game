#ifndef PROJECT_PLAYER_CLASS_H
#define PROJECT_PLAYER_CLASS_H

#include <iostream>

struct Player {
    explicit Player();

    void set_conditions(size_t x, size_t y);

    size_t get_x() const;

    size_t get_y() const;

    void move_left();

    void move_right();

    void move_top();

    void move_bottom();

private:
    //size_t id_;
    //size_t hp_;
    size_t x_;
    size_t y_;
    //size_t armor_;
    //size_t team_;
    //size_t cursor;
};

#endif // PROJECT_PLAYER_CLASS_H