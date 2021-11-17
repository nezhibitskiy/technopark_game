#ifndef FROMBOOK_OBJECT_H
#define FROMBOOK_OBJECT_H


class Object {
public:

    virtual void setPos(float x, float y) = 0;
    virtual sf::Sprite getSprite() = 0;
};


#endif //FROMBOOK_OBJECT_H
