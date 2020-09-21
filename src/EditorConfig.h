//
// Created by Harrison Marshall on 21/9/20.
//

#ifndef HVIMSRC_EDITORCONFIG_H
#define HVIMSRC_EDITORCONFIG_H

struct WindowSize
{
    int width;
    int height;
};

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORWARD,
    BACK
};

struct CursorPosition
{
    const int x;
    const int y;
    CursorPosition(const int& x_in, const int& y_in) : x(x_in), y(y_in) {}
};

struct EditorConfig
{
    const CursorPosition m_cursor;
    const int m_view_offset_x;
    const int m_view_offset_y;
    const std::vector<std::string> m_content;
    const bool m_do_run;
    EditorConfig(const CursorPosition& cursor,
                 const int& view_offset_x,
                 const int& view_offset_y,
                 const std::vector<std::string>& content,
                 const bool& do_run)
                 : m_cursor(CursorPosition(cursor.x, cursor.y)),
                   m_view_offset_x(view_offset_x),
                   m_view_offset_y(view_offset_y),
                   m_content(content),
                   m_do_run(do_run) {}
};

CursorPosition move_cursor(const EditorConfig& e, Direction d)
{
    const CursorPosition& c = e.m_cursor;
    switch(d)
    {
        case LEFT: {
            if (c.x > 0)
                return {c.x - 1, c.y};
            return c;
        }
        case RIGHT: {
            if (c.x < e.m_content.at(c.y).length() - 1)
                return {c.x + 1, c.y};
            return c;
        }
        case UP: {
            if (c.y > 0)
                return {c.x, c.y - 1};
            return c;
        }
        case DOWN: {
            if (c.y < e.m_content.size() - 1)
                return {c.x, c.y + 1};
            return c;
        }
        case FORWARD: {
            if (c.x < e.m_content.at(c.y).length() - 1)
                return {c.x + 1, c.y};
            return {0, c.y + 1};
        }
        case BACK: {
            if (c.x > 0)
                return {c.x - 1, c.y};
            if (c.y > 0 && e.m_content.at(c.y - 1).length() > 0)
                return {static_cast<int>(e.m_content.at(c.y - 1).size() - 1), c.y - 1};
            if (c.y > 0)
                return {0, c.y - 1};
            return {1, 0};
        }
        default:
            return c;
    }
}

#endif //HVIMSRC_EDITORCONFIG_H