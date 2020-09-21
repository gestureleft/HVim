//
// Created by Harrison Marshall on 21/9/20.
//
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "EditorConfig.h"

TEST(MoveCursorTest, MoveForward)
{
    EditorConfig e({0,0}, 0, 0, {"Hello", "World"}, true);
    EXPECT_EQ(e.m_content.at(0), "Hello");
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2(move_cursor(e, FORWARD), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run);
    EXPECT_EQ(e2.m_cursor.x, 1);
    EXPECT_EQ(e2.m_cursor.y, 0);
    EXPECT_EQ(e.m_cursor.x, 0);
    EXPECT_EQ(e.m_cursor.y, 0);
}

TEST(MoveCursorTest, MoveForwardEndOfLine)
{
    EditorConfig e({4,0}, 0, 0, {"Hello", "World"}, true);
    EXPECT_EQ(e.m_content.at(0), "Hello");
    EXPECT_EQ(e.m_cursor.x, 4);
    EXPECT_EQ(e.m_cursor.y, 0);
    EditorConfig e2(move_cursor(e, FORWARD), e.m_view_offset_x, e.m_view_offset_y, e.m_content, e.m_do_run);
    EXPECT_EQ(e2.m_cursor.x, 0);
    EXPECT_EQ(e2.m_cursor.y, 1);
    EXPECT_EQ(e.m_cursor.x, 4);
    EXPECT_EQ(e.m_cursor.y, 0);
}