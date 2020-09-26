//
// Created by Harrison Marshall on 21/9/20.
//
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "EditorConfig.h"

TEST(MoveCursorTest, MoveRight)
{
    EditorConfig e = {{2,0}, 0, 0, {"Hello", "World"}, true};
    EXPECT_EQ(e.m_content.at(0), "Hello");
    EXPECT_EQ(e.m_cursor.x, 2);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2 = {move_cursor(e, RIGHT), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e2.m_cursor.x, 3);
    EXPECT_EQ(e2.m_cursor.y, 0);
    EXPECT_EQ(e.m_cursor.x, 2);
    EXPECT_EQ(e.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveRightEndOfLine)
{
    EditorConfig e = {{2,0}, 0, 0, {"omg", "wow"}, true};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 2);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2 = {move_cursor(e, RIGHT), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 2);
    EXPECT_EQ(e.m_cursor.y, 0);
    EXPECT_EQ(e2.m_content.at(0), "omg");
    EXPECT_EQ(e2.m_cursor.x, 2);
    EXPECT_EQ(e2.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveLeft)
{
EditorConfig e = {{2,0}, 0, 0, {"Hello", "World"}, true};
EXPECT_EQ(e.m_content.at(0), "Hello");
EXPECT_EQ(e.m_cursor.x, 2);
EXPECT_EQ(e.m_cursor.y, 0);
EditorConfig e2 = {move_cursor(e, LEFT), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
EXPECT_EQ(e2.m_cursor.x, 1);
EXPECT_EQ(e2.m_cursor.y, 0);
EXPECT_EQ(e.m_cursor.x, 2);
EXPECT_EQ(e.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveLeftStartOfLine)
{
EditorConfig e = {{0,0}, 0, 0, {"omg", "wow"}, true};
EXPECT_EQ(e.m_content.at(0), "omg");
EXPECT_EQ(e.m_cursor.x, 0);
EXPECT_EQ(e.m_cursor.y, 0);
EditorConfig e2 = {move_cursor(e, LEFT), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
EXPECT_EQ(e.m_content.at(0), "omg");
EXPECT_EQ(e.m_cursor.x, 0);
EXPECT_EQ(e.m_cursor.y, 0);
EXPECT_EQ(e2.m_content.at(0), "omg");
EXPECT_EQ(e2.m_cursor.x, 0);
EXPECT_EQ(e2.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveDown)
{
    EditorConfig e = {{0,0}, 0, 0, {"omg", "wow"}, true};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2 = {move_cursor(e, DOWN), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 0);
    EXPECT_EQ(e2.m_content.at(0), "omg");
    EXPECT_EQ(e2.m_cursor.x, 0);
    EXPECT_EQ(e2.m_cursor.y, 1);
}

TEST(MoveCursorTest, MoveDownBottomOfContent)
{
    EditorConfig e = {{0,1}, 0, 0, {"omg", "wow"}, true};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 1);
    EditorConfig e2 = {move_cursor(e, DOWN), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 1);
    EXPECT_EQ(e2.m_content.at(0), "omg");
    EXPECT_EQ(e2.m_cursor.x, 0);
    EXPECT_EQ(e2.m_cursor.y, 1);
}

TEST(MoveCursorTest, MoveUp)
{
    EditorConfig e = {{0,1}, 0, 0, {"omg", "wow"}, true};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 1);
    EditorConfig e2 = {move_cursor(e, UP), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 1);
    EXPECT_EQ(e2.m_content.at(0), "omg");
    EXPECT_EQ(e2.m_cursor.x, 0);
    EXPECT_EQ(e2.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveUpTopOfContent)
{
    EditorConfig e = {{0,0}, 0, 0, {"omg", "wow"}, true};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2 = {move_cursor(e, UP), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e.m_content.at(0), "omg");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 0);
    EXPECT_EQ(e2.m_content.at(0), "omg");
    EXPECT_EQ(e2.m_cursor.x, 0);
    EXPECT_EQ(e2.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveForward)
{
    EditorConfig e = {{0,0}, 0, 0, {"Hello", "World"}, true};
    EXPECT_EQ(e.m_content.at(0), "Hello");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2 = {move_cursor(e, FORWARD), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e2.m_cursor.x, 1);
    EXPECT_EQ(e2.m_cursor.y, 0);
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveForwardEndOfLine)
{
    EditorConfig e = {{4,0}, 0, 0, {"Hello", "World"}, true};
    EXPECT_EQ(e.m_content.at(0), "Hello");
    EXPECT_EQ(e.m_cursor.x, 4);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2 = {move_cursor(e, FORWARD), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e2.m_cursor.x, 0);
    EXPECT_EQ(e2.m_cursor.y, 1);
    EXPECT_EQ(e.m_cursor.x, 4);
    EXPECT_EQ(e.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveBack)
{
    EditorConfig e = {{1,0}, 0, 0, {"Hello", "World"}, true};
    EXPECT_EQ(e.m_content.at(0), "Hello");
    EXPECT_EQ(e.m_cursor.x, 1);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2 = {move_cursor(e, BACK), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e2.m_cursor.x, 0);
    EXPECT_EQ(e2.m_cursor.y, 0);
    EXPECT_EQ(e.m_cursor.x, 1);
    EXPECT_EQ(e.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveBackAtStartOfLine)
{
    EditorConfig e = {{0,1}, 0, 0, {"Hello", "World"}, true};
    EXPECT_EQ(e.m_content.at(0), "Hello");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 1);
    EditorConfig e2 = {move_cursor(e, BACK), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run};
    EXPECT_EQ(e2.m_cursor.x, e2.m_content.at(e2.m_cursor.y).length() - 1);
    EXPECT_EQ(e2.m_cursor.y, 0);
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 1);
}

TEST(MoveCursorTest, MoveUpToLineShorterThanX)
{
    EditorConfig e = {{3, 1}, 0, 0, {"ab", "abcd"}, true};
    EXPECT_EQ(e.m_cursor.x, 3);
    EXPECT_EQ(e.m_cursor.y, 1);
    EditorConfig e2 = {move_cursor(e, UP), 0, 0, e.m_content, true};
    EXPECT_EQ(e.m_cursor.x, 3);
    EXPECT_EQ(e.m_cursor.y, 1);
    EXPECT_EQ(e2.m_cursor.x, 1);
    EXPECT_EQ(e2.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveDownToLineShorterThanX)
{
    EditorConfig e = {{3, 0}, 0, 0, {"abcd", "ab"}, true};
    EXPECT_EQ(e.m_cursor.x, 3);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2 = {move_cursor(e, DOWN), 0, 0, e.m_content, true};
    EXPECT_EQ(e.m_cursor.x, 3);
    EXPECT_EQ(e.m_cursor.y, 0);
    EXPECT_EQ(e2.m_cursor.x, 1);
    EXPECT_EQ(e2.m_cursor.y, 1);
}

TEST(MoveCursorTest, MoveDownToEmptyLine)
{
    EditorConfig e = {{3, 0}, 0, 0, {"abcd", ""}, true};
    EXPECT_EQ(e.m_cursor.x, 3);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2 = {move_cursor(e, DOWN), 0, 0, e.m_content, true};
    EXPECT_EQ(e.m_cursor.x, 3);
    EXPECT_EQ(e.m_cursor.y, 0);
    EXPECT_EQ(e2.m_cursor.x, 0);
    EXPECT_EQ(e2.m_cursor.y, 1);
}


TEST(MoveCursorTest, MoveUpToEmptyLine)
{
    EditorConfig e = {{0, 1}, 0, 0, {"", "a"}, true};
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 1);
    EditorConfig e2 = {move_cursor(e, UP), 0, 0, e.m_content, true};
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 1);
    EXPECT_EQ(e2.m_cursor.x, 0);
    EXPECT_EQ(e2.m_cursor.y, 0);
}