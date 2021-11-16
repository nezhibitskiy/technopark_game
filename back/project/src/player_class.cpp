#include "player_class.h"
#include <iostream>

Player::Player() = default;

void Player::set_conditions(size_t x, size_t y) {
    x_ = x;
    y_ = y;
}

size_t Player::get_x() const {
    return x_;
}

size_t Player::get_y() const {
    return y_;
}

void Player::move_left() {
    y_ -= 1;
}

void Player::move_right() {
    y_ += 1;
}

void Player::move_top() {
    x_ -= 1;
}

void Player::move_bottom() {
    x_ += 1;
}
