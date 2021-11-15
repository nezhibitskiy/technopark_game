#ifndef PROJECT_TP_OBJECTS_H
#define PROJECT_TP_OBJECTS_H


class Object {
public:
    Object() = default;
    virtual bool Damagable() = 0;
    virtual bool Damage(const char damageValue) {
        return false;                               // Если умер - возвращает true
    };
    virtual bool Respawn() {
        return false;
    }
    virtual bool CanBeStandOn() = 0;
    virtual void ToDo(Object *player) {}
    virtual ~Object() = default;
};

class Block : public Object {
public:
    Block(int x, int y) : x_(x), y_(y) {}
    ~Block() override = default;
    bool Damagable() override { return true; };
    bool Damage(const char damageValue) override {
        if (damageValue > 0) {
            return true;
        }
        return false;
    }
    bool CanBeStandOn() override { return true; };
    void ToDo(Object *player) override {
        // перемещаем игрока на блок
    }

private:
    int x_;
    int y_;
};


#endif //PROJECT_TP_OBJECTS_H
