#ifndef PROJECT_TP_OBJECTS_H
#define PROJECT_TP_OBJECTS_H


class Object {
public:
    Object() = default;
    virtual bool Damagable() {
        return false;
    }
    virtual bool Damage(const char damageValue) {
        return false;                               // Если умер - возвращает true
    };
    virtual bool Respawn() {
        return false;
    }
    virtual bool CanBeStandOn() {
        true;
    }
    virtual ~Object() = default;
};


#endif //PROJECT_TP_OBJECTS_H
