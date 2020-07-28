#include "header.h"

ostringstream _stream;

void _log(string message)
{
    SYSTEMTIME t;
    GetLocalTime(&t);
    cout << t.wHour << ":" << t.wMinute << ":"
        << t.wSecond << "." << t.wMilliseconds << " "
        << message << endl;
}

void _critical(string message)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical Error!", message.c_str(), window);
    client_quit();
    exit(-1);
}

void set_position(SDL_Rect& rect, int x, int y)
{
    rect.x = x;
    rect.y = y;
}

void set_position(SDL_Rect& rect, nullptr_t, int y)
{
    set_position(rect, (WINDOW_WIDTH - rect.w) / 2, y);
}

void set_position(SDL_Rect& rect, int x, nullptr_t)
{
    set_position(rect, x, (WINDOW_HEIGHT - rect.h) / 2);
}

void set_position(SDL_Rect& rect, nullptr_t, nullptr_t)
{
    set_position(rect, (WINDOW_WIDTH - rect.w) / 2, (WINDOW_HEIGHT - rect.h) / 2);
}

void set_position(SDL_Rect& rect)
{
    set_position(rect, nullptr, nullptr);
}

bool is_point_in_rect(SDL_Point point, SDL_Rect rect)
{
    return SDL_PointInRect(&point, &rect) == SDL_TRUE;
}

bool is_point_in_rect(int x, int y, SDL_Rect rect)
{
    SDL_Point p;
    p.x = x;
    p.y = y;
    return is_point_in_rect(p, rect);
}
