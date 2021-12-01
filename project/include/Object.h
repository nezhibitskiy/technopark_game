#ifndef PROJECT_OBJECT_H
#define PROJECT_OBJECT_H

class Object {
public:
    Object() = default;
    virtual bool Damagable() const = 0;
    virtual bool Damage(const char damageValue) {
        return false;                               // Если умер - возвращает true
    };
    virtual bool Respawn() {
        return false;
    }
    virtual bool CanBeStandOn() const { return true; };
    virtual void ToDo(Object *player) {}
    virtual char getObjectSymbol() {
        return '0';
    };
    ~Object() = default;
};

#endif // PROJECT_OBJECT_H
