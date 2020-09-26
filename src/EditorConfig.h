//
// Created by Harrison Marshall on 21/9/20.
//

#ifndef HVIMSRC_EDITORCONFIG_H
#define HVIMSRC_EDITORCONFIG_H

#include "util.h"

/**
 * The distance from the edge of the top/bottom of the screen to start scrolling
 */
const int DISTANCE_TO_SCROLL = 9;

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FORWARD,
    BACK
};

struct Vector
{
    const int x;
    const int y;
};

struct EditorConfig
{
    const Vector m_cursor;
    const Vector m_view_offset;
    const std::vector<std::string> m_content;
    const bool m_do_run;
};

/**
 * Given an EditorConfig and a navigation direction, return a new EditorConfig with the original EditorConfig's cursor
 * moved in the direction specified
 * @param e The EditorConfig
 * @param d The Direction to navigate
 * @return A new EditorConfig
 */
Vector move_cursor(const EditorConfig& e, Direction d)
{
    const Vector& c = e.m_cursor;
    switch(d)
    {
        case LEFT: {
            if (e.m_content.at(c.y).length() == 0 || c.x <= 0)
                return c;
            else
                return {c.x - 1, c.y};
        }
        case RIGHT: {
            if (e.m_content.at(c.y).length() == 0 || c.x >= e.m_content.at(c.y).length() - 1)
                return c;
            else
                return {c.x + 1, c.y};
        }
        case UP: {
            if (c.y > 0)
            {
                if (e.m_content.at(c.y - 1).length() == 0)
                    return {0, c.y - 1};
                else
                    return {static_cast<int>(c.x > e.m_content.at(c.y - 1).length() - 1 ? e.m_content.at(c.y - 1).length() - 1 : c.x),
                        c.y - 1};
            }
            return c;
        }
        case DOWN: {
            if (c.y < e.m_content.size() - 1)
            {
                if (e.m_content.at(c.y + 1).length() == 0)
                    return {0, c.y + 1};
                else
                    return {static_cast<int>(c.x > e.m_content.at(c.y + 1).length() - 1 ? e.m_content.at(c.y + 1).length() - 1 : c.x),
                            c.y + 1};
            }
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

/**
 * Given a offset value and a cursor value, returns a new offset value that ensures the cursor is within the screen
 * @param offset
 * @param cursor
 * @return
 */
Vector update_offset(const Vector& offset, const Vector& cursor)
{
    // TODO - Implement scrolling up
    if (get_window_size().height - cursor.y + offset.y < DISTANCE_TO_SCROLL)
        return {offset.x, offset.y + 1};
    return offset;
}

#endif //HVIMSRC_EDITORCONFIG_H
