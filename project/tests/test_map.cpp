#include <iostream>
#include "gtest/gtest.h"

#include "map_class.h"
#include "endBlock.h"
#include "player_class.h"


TEST(MAP_TEST, block1) {
    auto block = new EndBlock();
    auto map = new Map("map.txt");
    map->addObject(block, 1, 1);
    EXPECT_EQ(map->getObject(1, 1), block);
    delete block;
    delete map;
}

TEST(INPUT_TEST, block3) {
    auto block = new DefaultBlock();
    auto map = new Map("map.txt");
    map->addObject(block, 1, 1);
    EXPECT_EQ(map->getObject(0, 0), nullptr);
    delete block;
    delete map;
}

TEST(INPUT_TEST, block4) {
    auto block = new DefaultBlock();
    auto map = new Map("map.txt");
    map->addObject(block, 10, 10);
    EXPECT_EQ(map->getObject(10, 10), block);
    delete block;
    delete map;
}

TEST(INPUT_TEST, block5) {
    auto block1 = new DefaultBlock();
    auto map = new Map("map.txt");
    map->addObject(block1, 10000, 10000);
    EXPECT_EQ(map->getObject(10000, 10000), nullptr);
    delete block1;
    delete map;
}

TEST(INPUT_TEST, block_damage) {
    auto block1 = new DefaultBlock();
    auto map = new Map("map.txt");
    map->addObject(block1, 10, 10);
    Object* obj = map->getObject(10, 10);
    obj->Damage(1);
    EXPECT_EQ(map->getObject(10, 10), nullptr);
    delete block1;
    delete map;
}

TEST(INPUT_TEST, end_of_map_damage) {
    auto block1 = new EndBlock();
    auto map = new Map("map.txt");
    map->addObject(block1, 10, 10);
    EXPECT_EQ(map->getObject(10000, 10000)->Damagable(), false);
    delete block1;
    delete map;
}

TEST(INPUT_TEST, default_damage) {
    auto block1 = new DefaultBlock();
    auto map = new Map("map.txt");
    map->addObject(block1, 10, 10);
    EXPECT_EQ(map->getObject(10000, 10000)->Damagable(), false);
    delete block1;
    delete map;
}

TEST(INPUT_TEST, stand_on) {
    auto block1 = new DefaultBlock();
    auto map = new Map("map.txt");
    map->addObject(block1, 10, 10);
    EXPECT_EQ(map->getObject(10, 10)->CanBeStandOn(), false);
    delete block1;
    delete map;
}

TEST(INPUT_TEST, stand_on1) {
    auto player1 = new Player(1);
    player1->setCoords(5, 5);
    auto map = new Map("map.txt");
    map->addObject(player1, 5, 5);
    map->moveObject(player1->getX(), player1->getY(), 6, 6);
    EXPECT_EQ(map->getObject(6, 6), player1);
    delete player1;
    delete map;
}

TEST(INPUT_TEST, stand_on2) {
    auto map = new Map("map.txt");
    EXPECT_EQ(map->getObject(10, 10)->CanBeStandOn(), true);
    delete map;
}

TEST(INPUT_TEST, stand_on3) {
    auto block1 = new DefaultBlock();
    auto map = new Map("map.txt");
    map->addObject(block1, 10, 10);
    EXPECT_EQ(map->getObject(5, 5)->CanBeStandOn(), false);
    delete block1;
    delete map;
}

TEST(INPUT_TEST, player1) {
    auto player1 = new Player(1);
    auto map = new Map("map.txt");
    map->addObject(player1, 5, 5);
    EXPECT_EQ(map->getObject(5, 5), player1);
    delete player1;
    delete map;
}

TEST(INPUT_TEST, player2) {
    auto player1 = new Player(1);
    EXPECT_EQ(player1->CanBeStandOn(), false);
    delete player1;
}

TEST(INPUT_TEST, player3) {
    auto player1 = new Player(1);
    EXPECT_EQ(player1->Damagable(), true);
    delete player1;
}

TEST(INPUT_TEST, player4) {
    auto player1 = new Player(1);
    EXPECT_EQ(player1->Damage(1), false);
    delete player1;
}

TEST(INPUT_TEST, player5) {
    auto player1 = new Player(1);
    EXPECT_EQ(player1->Damage(3), true);
    delete player1;
}

TEST(INPUT_TEST, test_move) {
    auto player1 = new Player(1);
    auto map = new Map("map.txt");
    map->addObject(player1, 5, 5);
    map->moveObject(player1->getX(), player1->getY(), 6, 6);
    EXPECT_EQ(player1->getX(), 6);
    EXPECT_EQ(player1->getY(), 6);
    delete player1;
    delete map;
}

TEST(INPUT_TEST, player6) {
    auto player1 = new Player(1);
    EXPECT_EQ(player1->Damage(3), true);
    delete player1;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}