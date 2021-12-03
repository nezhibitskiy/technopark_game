#ifndef FROMBOOK_OBJECT_H
#define FROMBOOK_OBJECT_H


class DrawObject {
public:

    virtual void setPos(unsigned int x, unsigned int y) = 0;

    virtual int getX() = 0;

    virtual int getY() = 0;

    virtual void setID(unsigned short _id) = 0;

    virtual void draw(sf::RenderWindow &l_window) = 0;
};


#endif //FROMBOOK_OBJECT_H
