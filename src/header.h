#pragma once

#pragma comment(lib ,"box2d.lib")
#pragma comment(lib ,"SDL2.lib")
#pragma comment(lib ,"SDL2main.lib")
#pragma comment(lib ,"SDL2_image.lib")
#pragma comment(lib ,"SDL2_mixer.lib")
#pragma comment(lib ,"SDL2_net.lib")
#pragma comment(lib ,"SDL2_ttf.lib")

#include <Windows.h>

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>
#include <list>

#include <box2d/box2d.h>

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

using namespace std;

// macros
#define INFO_NAME "JunkJoyriding"
#define INFO_VERSION "v0.1.1"
#define INFO_COPYRIGHT "版权所有 (C) NKID00 2020"

#ifdef _DEBUG
#define log(msg) _log(string("[" __FUNCTION__ ":") + to_string(__LINE__) + string("] ") + msg)
#define debug(msg) log(string("[*] ") + msg)
#else  // _DEBUG
#define log(msg) _log(msg)
#define debug(msg)
#endif  // _DEBUG

extern ostringstream _stream;

#define critical(msg) log(string("[X] ") + msg); _critical(msg)
#define error(msg) log(string("[E] ") + msg)
#define warning(msg) log(string("[!] ") + msg)
#define info(msg) log(string("[i] ") + msg)

#define critical_sdl() critical("SDL critical error: " + string(SDL_GetError()))
#define critical_img() critical("SDL_image critical error: " + string(IMG_GetError()))
#define critical_mix() critical("SDL_mixer critical error: " + string(Mix_GetError()))
#define critical_net() critical("SDL_net critical error: " + string(SDLNet_GetError()))
#define critical_ttf() critical("SDL_ttf critical error: " + string(TTF_GetError()))

#define if_negative(expr, action) if (expr < 0) { action; }
#define if_zero(expr, action) if (expr < 0) { action; }
#define if_nullptr(expr, action) if (expr == nullptr) { action; }
#define if_not_nullptr(expr, action) if (expr != nullptr) { action; }

#define delete_s(pointer) if_not_nullptr(pointer, delete pointer)

// main
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

enum class Status;

extern TTF_Font* font_42, * font_32, * font_22;
extern Status status;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

void client_run() noexcept;
void client_quit() noexcept;

// utilities
void _log(string message);
void _critical(string message);

constexpr SDL_Rect make_rect(int x, int y, int w, int h) { return SDL_Rect{ x, y, w, h }; }
constexpr SDL_Color make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = SDL_ALPHA_OPAQUE) { return SDL_Color{ r, g, b, a }; }
constexpr SDL_Point make_point(int x, int y) { return SDL_Point{ x, y }; }

void set_position(SDL_Rect& rect, int x, int y);
void set_position(SDL_Rect& rect, nullptr_t, int y);
void set_position(SDL_Rect& rect, int x, nullptr_t);
void set_position(SDL_Rect& rect, nullptr_t, nullptr_t);
void set_position(SDL_Rect& rect);

bool is_point_in_rect(SDL_Point point, SDL_Rect rect);
bool is_point_in_rect(int x, int y, SDL_Rect rect);

// graphics
constexpr auto C_TITLE = make_color(255, 200, 160);

constexpr auto C_TEXT = make_color(200, 200, 200);

constexpr auto C_BUTTON0 = C_TEXT;
constexpr auto C_BUTTON1 = make_color(255, 255, 220);
constexpr auto C_BUTTON2 = make_color(220, 220, 200);
#define C_BUTTON C_BUTTON0, C_BUTTON1, C_BUTTON2

void select_font(TTF_Font* new_font);

class Image
{
public:
    Image(SDL_Renderer* renderer, const char* path) noexcept;
    Image(SDL_Renderer* renderer, SDL_Surface* surface) noexcept;
    Image(SDL_Renderer* renderer) noexcept;
    void load_file(const char* path) noexcept;
    void load_surface(SDL_Surface* surface) noexcept;
    void set_alpha(uint8_t a = SDL_ALPHA_OPAQUE);
    void set_color(uint8_t r, uint8_t g, uint8_t b);
    void show() const noexcept;
    void set_position(SDL_Point positon) noexcept;
    void set_position(int x, int y) noexcept;
    ~Image() noexcept;
    SDL_Rect rect;
    double angle;
    SDL_Point center;
    SDL_RendererFlip flip;
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

class Text : public Image
{
public:
    Text(SDL_Renderer* renderer, const char* text, SDL_Color color) noexcept;
    Text(SDL_Renderer* renderer, const wchar_t* text, SDL_Color color) noexcept;
    Text(SDL_Renderer* renderer) noexcept;
    void load_text(const char* text, SDL_Color color) noexcept;
    void load_text(const wchar_t* text, SDL_Color color) noexcept;
};

class Input : public Text
{
public:
    Input(SDL_Renderer* renderer, SDL_Color color) noexcept;
    Input(SDL_Renderer* renderer) noexcept;
    void set_text_color(SDL_Color color) noexcept;
    void set_text(wstring text) noexcept;
    void append_text(wstring text) noexcept;
    wstring get_text() noexcept;
private:
    void refresh() noexcept;
    SDL_Color color;
    wstring text;
};

class Paragraph
{
public:
    Paragraph(SDL_Renderer* renderer, const char* text, SDL_Color color, int gap = 5) noexcept;
    Paragraph(SDL_Renderer* renderer, const wchar_t* text, SDL_Color color, int gap = 5) noexcept;
    Paragraph(SDL_Renderer* renderer) noexcept;
    void load_text(const char* text, SDL_Color color) noexcept;
    void load_text(const wchar_t* text, SDL_Color color) noexcept;
    void show() const noexcept;
    void set_position(SDL_Point positon, int gap = 5) noexcept;
    void set_position(int x, int y, int gap = 5) noexcept;
    ~Paragraph() noexcept;
    SDL_Rect rect;
private:
    SDL_Renderer* renderer;
    list<Text*> texts;
};

class Button
{
public:
    Button(Image* image0, Image* image1, Image* image2, SDL_Rect rect) noexcept;
    Button() noexcept;
    void load_button(Image* image0, Image* image1, Image* image2, SDL_Rect rect) noexcept;
    bool show(SDL_Point mouse, bool mouse_down) noexcept;
    bool show(int mouse_x, int mouse_y, bool mouse_down) noexcept;
    bool is_mouse_in(SDL_Point mouse) const noexcept;
    bool is_mouse_in(int mouse_x, int mouse_y) const noexcept;
    bool status;
    Image* image0, * image1, * image2;
    SDL_Rect rect;
};

class TextButton: public Button
{
public:
    TextButton(SDL_Renderer* renderer, const char* text0, SDL_Color color0,
        const char* text1, SDL_Color color1,
        const char* text2, SDL_Color color2) noexcept;
    TextButton(SDL_Renderer* renderer, const wchar_t* text0, SDL_Color color0,
        const wchar_t* text1, SDL_Color color1,
        const wchar_t* text2, SDL_Color color2) noexcept;
    TextButton(SDL_Renderer* renderer) noexcept;
    void load_text(const char* text0, SDL_Color color0,
        const char* text1, SDL_Color color1,
        const char* text2, SDL_Color color2) noexcept;
    void load_text(const wchar_t* text0, SDL_Color color0,
        const wchar_t* text1, SDL_Color color1,
        const wchar_t* text2, SDL_Color color2) noexcept;
    void set_position(SDL_Point positon) noexcept;
    void set_position(int x, int y) noexcept;
    ~TextButton() noexcept;
private:
    SDL_Renderer* renderer;
};

class TextColorButton : public TextButton
{
public:
    TextColorButton(SDL_Renderer* renderer, const char* text, SDL_Color color0,
        SDL_Color color1, SDL_Color color2) noexcept;
    TextColorButton(SDL_Renderer* renderer, const wchar_t* text, SDL_Color color0,
        SDL_Color color1, SDL_Color color2) noexcept;
    TextColorButton(SDL_Renderer* renderer) noexcept;
    void load_color(const char* text, SDL_Color color0,
        SDL_Color color1, SDL_Color color2) noexcept;
    void load_color(const wchar_t* text, SDL_Color color0,
        SDL_Color color1, SDL_Color color2) noexcept;
};

// junk
class Junk
{
public:
    enum class Color : uint8_t
    {
        White = 0,
        Red = 1,
        Yellow = 2,
        Green = 3,
        Cyan = 4,
        Blue = 5,
        Magenta = 6
    } color;
    static constexpr SDL_Color get_color(Junk::Color index);
    static constexpr SDL_Color get_color(uint8_t color);
    enum class Flame : uint8_t
    {
        None = 0,
        Flame25 = 1,
        Flame50 = 2,
        Flame75 = 3,
        Flame100 = 4
    } flame;
    enum class RCS : uint8_t
    {
        None = 0,
        Left = 1,
        Right = 2
    } rcs;
    static void load(SDL_Renderer* renderer) noexcept;
    static void quit() noexcept;
    Junk() noexcept;
    void show() noexcept;
    static void show(int x, int y, double rotation, Junk::Color color,
        Junk::Flame flame, Junk::RCS rcs) noexcept;
    int x, y;
    double rotation;
private:
    static inline Image* junk;
    static inline Image* junk_, * junkl, * junkr;
    static inline Image* junk1, * junk2, * junk3, * junk4;
};
