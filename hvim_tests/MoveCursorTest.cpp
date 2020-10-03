//
// Created by Harrison Marshall on 21/9/20.
//
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "EditorConfig.h"

EditorConfig make_editor(const Vector& cursor, const std::vector<std::string>& content)
{
    return {cursor, {0, 0}, content, true, Mode::NAVIGATE, ""};
}

EditorConfig make_editor(const Vector&& cursor, const std::vector<std::string>&& content) { return make_editor(cursor, content); }

TEST(MoveCursorTest, MoveRight)
{
    EditorConfig e = make_editor({2,0}, {"Hello", "World"});
    EXPECT_EQ(e.content.at(0), "Hello");
    EXPECT_EQ(e.cursor.x, 2);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, RIGHT), e.content);
    EXPECT_EQ(e2.cursor.x, 3);
    EXPECT_EQ(e2.cursor.y, 0);
    EXPECT_EQ(e.cursor.x, 2);
    EXPECT_EQ(e.cursor.y, 0);
}

TEST(MoveCursorTest, MoveRightEndOfLine)
{
    EditorConfig e = make_editor({2,0}, {"omg", "wow"});
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 2);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, RIGHT), e.content);
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 2);
    EXPECT_EQ(e.cursor.y, 0);
    EXPECT_EQ(e2.content.at(0), "omg");
    EXPECT_EQ(e2.cursor.x, 2);
    EXPECT_EQ(e2.cursor.y, 0);
}

TEST(MoveCursorTest, MoveLeft)
{
    EditorConfig e = make_editor({2,0}, {"Hello", "World"});
    EXPECT_EQ(e.content.at(0), "Hello");
    EXPECT_EQ(e.cursor.x, 2);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, LEFT), e.content);
    EXPECT_EQ(e2.cursor.x, 1);
    EXPECT_EQ(e2.cursor.y, 0);
    EXPECT_EQ(e.cursor.x, 2);
    EXPECT_EQ(e.cursor.y, 0);
}

TEST(MoveCursorTest, MoveLeftStartOfLine)
{
    EditorConfig e = make_editor({0,0}, {"omg", "wow"});
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, LEFT), e.content);
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 0);
    EXPECT_EQ(e2.content.at(0), "omg");
    EXPECT_EQ(e2.cursor.x, 0);
    EXPECT_EQ(e2.cursor.y, 0);
}

TEST(MoveCursorTest, MoveDown)
{
    EditorConfig e = make_editor({0,0}, {"omg", "wow"});
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, DOWN), e.content);
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 0);
    EXPECT_EQ(e2.content.at(0), "omg");
    EXPECT_EQ(e2.cursor.x, 0);
    EXPECT_EQ(e2.cursor.y, 1);
}

TEST(MoveCursorTest, MoveDownBottomOfContent)
{
    EditorConfig e = make_editor({0,1}, {"omg", "wow"});
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 1);
    EditorConfig e2 = make_editor(move_cursor(e, DOWN), e.content);
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 1);
    EXPECT_EQ(e2.content.at(0), "omg");
    EXPECT_EQ(e2.cursor.x, 0);
    EXPECT_EQ(e2.cursor.y, 1);
}

TEST(MoveCursorTest, MoveUp)
{
    EditorConfig e = make_editor({0,1}, {"omg", "wow"});
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 1);
    EditorConfig e2 = make_editor(move_cursor(e, UP), e.content);
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 1);
    EXPECT_EQ(e2.content.at(0), "omg");
    EXPECT_EQ(e2.cursor.x, 0);
    EXPECT_EQ(e2.cursor.y, 0);
}

TEST(MoveCursorTest, MoveUpTopOfContent)
{
    EditorConfig e = make_editor({0,0}, {"omg", "wow"});
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, UP), e.content);
    EXPECT_EQ(e.content.at(0), "omg");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 0);
    EXPECT_EQ(e2.content.at(0), "omg");
    EXPECT_EQ(e2.cursor.x, 0);
    EXPECT_EQ(e2.cursor.y, 0);
}

TEST(MoveCursorTest, MoveForward)
{
    EditorConfig e = make_editor({0, 0}, {"Hello", "World"});
    EXPECT_EQ(e.content.at(0), "Hello");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, FORWARD), e.content);
    EXPECT_EQ(e2.cursor.x, 1);
    EXPECT_EQ(e2.cursor.y, 0);
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 0);
}

TEST(MoveCursorTest, MoveForwardEndOfLine)
{
    EditorConfig e = make_editor({4,0}, {"Hello", "World"});
    EXPECT_EQ(e.content.at(0), "Hello");
    EXPECT_EQ(e.cursor.x, 4);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, FORWARD), e.content);
    EXPECT_EQ(e2.cursor.x, 0);
    EXPECT_EQ(e2.cursor.y, 1);
    EXPECT_EQ(e.cursor.x, 4);
    EXPECT_EQ(e.cursor.y, 0);
}

TEST(MoveCursorTest, MoveBack)
{
    EditorConfig e = make_editor({1,0}, {"Hello", "World"});
    EXPECT_EQ(e.content.at(0), "Hello");
    EXPECT_EQ(e.cursor.x, 1);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, BACK), e.content);
    EXPECT_EQ(e2.cursor.x, 0);
    EXPECT_EQ(e2.cursor.y, 0);
    EXPECT_EQ(e.cursor.x, 1);
    EXPECT_EQ(e.cursor.y, 0);
}

TEST(MoveCursorTest, MoveBackAtStartOfLine)
{
    EditorConfig e = make_editor({0,1}, {"Hello", "World"});
    EXPECT_EQ(e.content.at(0), "Hello");
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 1);
    EditorConfig e2 = make_editor(move_cursor(e, BACK), e.content);
    EXPECT_EQ(e2.cursor.x, e2.content.at(e2.cursor.y).length() - 1);
    EXPECT_EQ(e2.cursor.y, 0);
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 1);
}

TEST(MoveCursorTest, MoveUpToLineShorterThanX)
{
    EditorConfig e = make_editor({3, 1}, {"ab", "abcd"});
    EXPECT_EQ(e.cursor.x, 3);
    EXPECT_EQ(e.cursor.y, 1);
    EditorConfig e2 = make_editor(move_cursor(e, UP), e.content);
    EXPECT_EQ(e.cursor.x, 3);
    EXPECT_EQ(e.cursor.y, 1);
    EXPECT_EQ(e2.cursor.x, 1);
    EXPECT_EQ(e2.cursor.y, 0);
}

TEST(MoveCursorTest, MoveDownToLineShorterThanX)
{
    EditorConfig e = make_editor({3, 0}, {"abcd", "ab"});
    EXPECT_EQ(e.cursor.x, 3);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, DOWN), e.content);
    EXPECT_EQ(e.cursor.x, 3);
    EXPECT_EQ(e.cursor.y, 0);
    EXPECT_EQ(e2.cursor.x, 1);
    EXPECT_EQ(e2.cursor.y, 1);
}

TEST(MoveCursorTest, MoveDownToEmptyLine)
{
    EditorConfig e = make_editor({3, 0}, {"abcd", ""});
    EXPECT_EQ(e.cursor.x, 3);
    EXPECT_EQ(e.cursor.y, 0);
    EditorConfig e2 = make_editor(move_cursor(e, DOWN), e.content);
    EXPECT_EQ(e.cursor.x, 3);
    EXPECT_EQ(e.cursor.y, 0);
    EXPECT_EQ(e2.cursor.x, 0);
    EXPECT_EQ(e2.cursor.y, 1);
}


TEST(MoveCursorTest, MoveUpToEmptyLine)
{
    EditorConfig e = make_editor({0, 1}, {"", "a"});
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 1);
    EditorConfig e2 = make_editor(move_cursor(e, UP), e.content);
    EXPECT_EQ(e.cursor.x, 0);
    EXPECT_EQ(e.cursor.y, 1);
    EXPECT_EQ(e2.cursor.x, 0);
    EXPECT_EQ(e2.cursor.y, 0);
}