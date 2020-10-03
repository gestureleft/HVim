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

enum Mode {
    NAVIGATE,
    VISUAL,
    INSERT,
    COMMAND,
    SEARCH
};

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
    const Vector cursor;
    const Vector view_offset;
    const std::vector<std::string> content;
    const bool do_run;
    const Mode mode;
    const std::string command;
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
    const Vector& c = e.cursor;
    switch(d)
    {
        case LEFT: {
            if (e.content.at(c.y).length() == 0 || c.x <= 0)
                return c;
            else
                return {c.x - 1, c.y};
        }
        case RIGHT: {
            if (e.content.at(c.y).length() == 0 || c.x >= e.content.at(c.y).length() - 1)
                return c;
            else
                return {c.x + 1, c.y};
        }
        case UP: {
            if (c.y > 0)
            {
                if (e.content.at(c.y - 1).length() == 0)
                    return {0, c.y - 1};
                else
                    return {static_cast<int>(c.x > e.content.at(c.y - 1).length() - 1 ? e.content.at(c.y - 1).length() - 1 : c.x),
                        c.y - 1};
            }
            return c;
        }
        case DOWN: {
            if (c.y < e.content.size() - 1)
            {
                if (e.content.at(c.y + 1).length() == 0)
                    return {0, c.y + 1};
                else
                    return {static_cast<int>(c.x > e.content.at(c.y + 1).length() - 1 ? e.content.at(c.y + 1).length() - 1 : c.x),
                            c.y + 1};
            }
            return c;
        }
        case FORWARD: {
            if (e.content.at(c.y).length() != 0 && c.x < e.content.at(c.y).length() - 1)
                return {c.x + 1, c.y};
            return {0, c.y + 1};
        }
        case BACK: {
            if (c.x > 0)
                return {c.x - 1, c.y};
            if (c.y > 0 && e.content.at(c.y - 1).length() > 0)
                return {static_cast<int>(e.content.at(c.y - 1).size() - 1), c.y - 1};
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
 * @param offset The current offset
 * @param cursor The cursor position
 * @return A new offset
 */
Vector update_offset(const Vector& offset, const Vector& cursor)
{
    // TODO - Implement scrolling sideways
    if (get_window_size().height - cursor.y + offset.y < DISTANCE_TO_SCROLL)
        return {offset.x, offset.y + 1};
    if (cursor.y > DISTANCE_TO_SCROLL - 1 && cursor.y - offset.y < DISTANCE_TO_SCROLL)
        return {offset.x, offset.y - 1};
    return offset;
}

#endif //HVIMSRC_EDITORCONFIG_H
