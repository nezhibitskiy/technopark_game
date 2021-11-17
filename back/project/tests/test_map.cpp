#include <iostream>
#include "gtest/gtest.h"

#include "map_class.h"
#include "block.h"
#include "player.h"

#define BLOCK_ERROR 144


TEST(MAP_TEST, block1) {
    auto block = new Block(1, 1, true);
    auto map = new Map("map.txt");
    map->set_map_object(1, 1, block);
    EXPECT_EQ(map->get_map_object(1, 1), block);
    delete block;
    delete map;
}

TEST(MAP_TEST, block2) {
    auto block = new Block(-1, -1, true);
    auto map = new Map("map.txt");
    EXPECT_EQ(map->set_map_object(1, 1, block), BLOCK_ERROR);
    delete block;
    delete map;
}

TEST(INPUT_TEST, block3) {
    auto block = new Block(1, 1, true);
    auto map = new Map("map.txt");
    map->set_map_object(1, 1, block);
    EXPECT_EQ(map->get_map_object(0, 0), nullptr);
}

TEST(INPUT_TEST, block4) {
    auto block = new Block(10, 10, true);
    auto map = new Map("map.txt");
    map->set_map_object(10, 10, block);
    EXPECT_EQ(map->get_map_object(10, 10), block);
}

TEST(INPUT_TEST, block5) {
    auto block1 = new Block(10000, 10000, true);
    auto map = new Map("map.txt");
    map->set_map_object(10000, 10000, block1);
    EXPECT_EQ(map->get_map_object(10000, 10000), block1);
}

TEST(INPUT_TEST, block_damage) {
    auto block1 = new Block(10, 10, true);
    auto map = new Map("map.txt");
    map->set_map_object(10, 10, block1);
    Object* obj = map->get_map_object(10, 10);
    obj->Damage(1);
    EXPECT_EQ(map->get_map_object(10, 10), nullptr);
}

TEST(INPUT_TEST, end_of_map_damage) {
    auto block1 = new Block(10, 10, false);
    auto map = new Map("map.txt");
    map->set_map_object(10, 10, block1);
    EXPECT_EQ(map->get_map_object(10000, 10000)->Damagable(), false);
}

TEST(INPUT_TEST, stand_on) {
    auto block1 = new Block(10, 10, false);
    EXPECT_EQ(block1->CanBeStandOn(), false);
}

TEST(INPUT_TEST, stand_on1) {
    auto player1 = new Player(0, 5, 5);
    auto map = new Map("map.txt");
    map->set_map_object(5, 5, player1);
    map->move_player(player1, 6, 6);
    EXPECT_EQ(map->get_map_object(6, 6), player1);
}

TEST(INPUT_TEST, stand_on2) {
    auto map = new Map("map.txt");
    EXPECT_EQ(map->get_map_object(10, 10)->CanBeStandOn(), true);
}



TEST(INPUT_TEST, stand_on3) {
    auto block1 = new Block(10, 10, false);
    auto map = new Map("map.txt");
    map->set_map_object(10, 10, block1);
    EXPECT_EQ(map->get_map_object(5, 5)->CanBeStandOn(), false);
}

TEST(INPUT_TEST, player1) {
    auto player1 = new Player(0, 5, 5);
    auto map = new Map("map.txt");
    map->set_map_object(5, 5, player1);
    EXPECT_EQ(map->get_map_object(5, 5), player1);
}

TEST(INPUT_TEST, player2) {
    auto player1 = new Player(0, 5, 5);
    EXPECT_EQ(player1->CanBeStandOn(), false);
}

TEST(INPUT_TEST, player3) {
    auto player1 = new Player(0, 5, 5);
    EXPECT_EQ(player1->Damagable(), true);
}

TEST(INPUT_TEST, player4) {
    auto player1 = new Player(0, 5, 5);
    EXPECT_EQ(player1->Damage(1), false);
}

TEST(INPUT_TEST, player5) {
    auto player1 = new Player(0, 5, 5);
    EXPECT_EQ(player1->Damage(3), true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}