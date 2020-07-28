#include "header.h"

TTF_Font* font;

void select_font(TTF_Font* new_font)
{
    font = new_font;
}

Image::Image(SDL_Renderer* renderer, const char* path) noexcept
    : rect(make_rect(0, 0, 0, 0)), renderer(renderer), texture(nullptr),
    angle(0), flip(SDL_RendererFlip::SDL_FLIP_NONE)
{
    this->load_file(path);
}

Image::Image(SDL_Renderer* renderer, SDL_Surface* surface) noexcept
    : rect(make_rect(0, 0, 0, 0)), renderer(renderer), texture(nullptr),
    angle(0), flip(SDL_RendererFlip::SDL_FLIP_NONE)
{
    this->load_surface(surface);
}

Image::Image(SDL_Renderer* renderer) noexcept
    : rect(make_rect(0, 0, 0, 0)), renderer(renderer), texture(nullptr),
    angle(0), center(make_point(0, 0)), flip(SDL_RendererFlip::SDL_FLIP_NONE)
{ }

void Image::load_file(const char* path) noexcept
{
    auto surface = IMG_Load(path);
    if_nullptr(surface, critical_img());
    this->load_surface(surface);
    SDL_FreeSurface(surface);
}

void Image::load_surface(SDL_Surface* surface) noexcept
{
    this->rect.w = surface->w;
    this->rect.h = surface->h;
    this->center.x = this->rect.w / 2;
    this->center.y = this->rect.h / 2;
    this->texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    if_nullptr(this->texture, critical_sdl());
    if_negative(SDL_SetTextureBlendMode(
        this->texture, SDL_BlendMode::SDL_BLENDMODE_BLEND
    ), critical_sdl());
}

void Image::set_alpha(uint8_t a)
{
    if_negative(SDL_SetTextureAlphaMod(this->texture, a), critical_sdl());
}

void Image::set_color(uint8_t r, uint8_t g, uint8_t b)
{
    if_negative(SDL_SetTextureColorMod(this->texture, r, g, b), critical_sdl());
}

void Image::show() const noexcept
{
    if_negative(SDL_RenderCopyEx(
        this->renderer, this->texture, nullptr, &this->rect, this->angle, &this->center, this->flip
    ), critical_sdl());
}

void Image::set_position(SDL_Point positon) noexcept
{
    this->rect.x = positon.x;
    this->rect.y = positon.y;
}

void Image::set_position(int x, int y) noexcept
{
    this->rect.x = x;
    this->rect.y = y;
}

Image::~Image() noexcept
{
    SDL_DestroyTexture(this->texture);
}

Text::Text(SDL_Renderer* renderer, const char* text, SDL_Color color) noexcept
    : Image(renderer)
{
    this->load_text(text, color);
}

Text::Text(SDL_Renderer* renderer, const wchar_t* text, SDL_Color color) noexcept
    : Image(renderer)
{
    this->load_text(text, color);
}

Text::Text(SDL_Renderer* renderer) noexcept
    : Image(renderer)
{ }

void Text::load_text(const char* text, SDL_Color color) noexcept
{
    auto surface = TTF_RenderUTF8_Blended(font, text, color);
    if_nullptr(surface, critical_ttf());
    this->load_surface(surface);
    SDL_FreeSurface(surface);
}

void Text::load_text(const wchar_t* text, SDL_Color color) noexcept
{
    auto surface = TTF_RenderUNICODE_Blended(font, (const uint16_t*)text, color);
    if_nullptr(surface, critical_ttf());
    this->load_surface(surface);
    SDL_FreeSurface(surface);
}

Input::Input(SDL_Renderer* renderer, SDL_Color color) noexcept
    :Text(renderer)
{
    this->set_text_color(color);
    this->set_text(L"");
}

Input::Input(SDL_Renderer* renderer) noexcept
    :Text(renderer)
{
    this->set_text_color(C_TEXT);
    this->set_text(L"");
}

void Input::set_text_color(SDL_Color color) noexcept
{
    this->color = color;
}

void Input::set_text(wstring text) noexcept
{
    this->text = text;
    this->refresh();
}

void Input::append_text(wstring text) noexcept
{
    this->text.append(text);
    this->refresh();
}

wstring Input::get_text() noexcept
{
    return this->text;
}

void Input::refresh() noexcept
{
    this->load_text((this->text + L"|").c_str(), this->color);
}

Paragraph::Paragraph(SDL_Renderer* renderer, const char* text, SDL_Color color, int gap) noexcept
    : renderer(renderer)
{
    this->load_text(text, color);
    this->set_position(0, 0, gap);
}

Paragraph::Paragraph(SDL_Renderer* renderer, const wchar_t* text, SDL_Color color, int gap) noexcept
    : renderer(renderer)
{
    this->load_text(text, color);
    this->set_position(0, 0, gap);
}

Paragraph::Paragraph(SDL_Renderer* renderer) noexcept
    : renderer(renderer), rect(make_rect(0,0,0,0))
{ }

void Paragraph::load_text(const char* text, SDL_Color color) noexcept
{
    string s(text), t;
    for (const auto c : s)
    {
        if (c != '\n')
        {
            t.append(1, c);
        }
        else
        {
            if (t.empty())
            {
                t = " ";
            }
            this->texts.push_back(new Text(this->renderer, t.c_str(), color));
            t.clear();
        }
    }
    if (!t.empty())
    {
        this->texts.push_back(new Text(this->renderer, t.c_str(), color));
        t.clear();
    }
}

void Paragraph::load_text(const wchar_t* text, SDL_Color color) noexcept
{
    wstring s(text), t;
    for (const auto c : s)
    {
        if (c != L'\n')
        {
            t.append(1, c);
        }
        else
        {
            if (t.empty())
            {
                t = L" ";
            }
            this->texts.push_back(new Text(this->renderer, t.c_str(), color));
            t.clear();
        }
    }
    if (!t.empty())
    {
        this->texts.push_back(new Text(this->renderer, t.c_str(), color));
        t.clear();
    }
}

void Paragraph::show() const noexcept
{
    for (auto it = this->texts.cbegin(); it != this->texts.cend(); it++)
    {
        (*it)->show();
    }
}

void Paragraph::set_position(SDL_Point positon, int gap) noexcept
{
    this->set_position(positon.x, positon.y, gap);
}

void Paragraph::set_position(int x, int y, int gap) noexcept
{
    this->rect.x = x;
    this->rect.y = y;
    this->rect.w = 0;
    int next_y = y;
    for (const auto text : this->texts)
    {
        text->set_position(x, next_y);
        if (text->rect.w > this->rect.w)
        {
            this->rect.w = text->rect.w;
        }
        next_y += text->rect.h;
        next_y += gap;
    }
    this->rect.h = next_y - gap - y;
}

Paragraph::~Paragraph() noexcept
{
    for (auto it = this->texts.cbegin(); it != this->texts.cend(); it++)
    {
        delete *it;
    }
}

Button::Button(Image* image0, Image* image1, Image* image2, SDL_Rect rect) noexcept
    : status(false)
{
    this->load_button(image0, image1, image2, rect);
}

Button::Button() noexcept
    : status(false), image0(nullptr), image1(nullptr), image2(nullptr), rect(make_rect(0, 0, 0, 0))
{ }

void Button::load_button(Image* image0, Image* image1, Image* image2, SDL_Rect rect) noexcept
{
    this->image0 = image0;
    this->image1 = image1;
    this->image2 = image2;
    this->rect = rect;
}

bool Button::show(SDL_Point mouse, bool mouse_down)noexcept
{
    return this->show(mouse.x, mouse.y, mouse_down);
}

bool Button::show(int mouse_x, int mouse_y, bool mouse_down)noexcept
{
    if (this->is_mouse_in(mouse_x, mouse_y))
    {
        if (mouse_down)
        {
            this->image2->show();
            this->status = true;
        }
        else
        {
            this->image1->show();
            if (this->status)
            {
                this->status = false;
                return true;
            }
        }
    }
    else
    {
        this->image0->show();
        this->status = false;
    }
    return false;
}

bool Button::is_mouse_in(SDL_Point mouse) const noexcept
{
    return is_point_in_rect(mouse, this->rect);
}

bool Button::is_mouse_in(int mouse_x, int mouse_y) const noexcept
{
    return is_point_in_rect(mouse_x, mouse_y, this->rect);
}

TextButton::TextButton(SDL_Renderer* renderer, const char* text0, SDL_Color color0,
    const char* text1, SDL_Color color1,
    const char* text2, SDL_Color color2) noexcept
    : renderer(renderer)
{
    this->load_text(text0, color0, text1, color1, text2, color2);
}

TextButton::TextButton(SDL_Renderer* renderer, const wchar_t* text0, SDL_Color color0,
    const wchar_t* text1, SDL_Color color1,
    const wchar_t* text2, SDL_Color color2) noexcept
    : renderer(renderer)
{
    this->load_text(text0, color0, text1, color1, text2, color2);
}

TextButton::TextButton(SDL_Renderer* renderer) noexcept
    : renderer(renderer)
{ }

void TextButton::load_text(const char* text0, SDL_Color color0,
    const char* text1, SDL_Color color1,
    const char* text2, SDL_Color color2) noexcept
{
    this->image0 = new Text(this->renderer, text0, color0);
    this->image1 = new Text(this->renderer, text1, color1);
    this->image2 = new Text(this->renderer, text2, color2);
    this->rect = this->image0->rect;
}

void TextButton::load_text(const wchar_t* text0, SDL_Color color0,
    const wchar_t* text1, SDL_Color color1,
    const wchar_t* text2, SDL_Color color2) noexcept
{
    this->image0 = new Text(this->renderer, text0, color0);
    this->image1 = new Text(this->renderer, text1, color1);
    this->image2 = new Text(this->renderer, text2, color2);
    this->rect = this->image0->rect;
}

void TextButton::set_position(SDL_Point positon) noexcept
{
    this->set_position(positon.x, positon.y);
}

void TextButton::set_position(int x, int y) noexcept
{
    this->rect.x = this->image0->rect.x
        = this->image1->rect.x = this->image2->rect.x = x;
    this->rect.y = this->image0->rect.y
        = this->image1->rect.y = this->image2->rect.y = y;
}

TextButton::~TextButton() noexcept
{
    delete this->image0;
    delete this->image1;
    delete this->image2;
}

TextColorButton::TextColorButton(SDL_Renderer* renderer, const char* text, SDL_Color color0,
    SDL_Color color1, SDL_Color color2) noexcept
    : TextButton(renderer, text, color0, text, color1, text, color2)
{ }

TextColorButton::TextColorButton(SDL_Renderer* renderer, const wchar_t* text, SDL_Color color0,
    SDL_Color color1, SDL_Color color2) noexcept
    : TextButton(renderer, text, color0, text, color1, text, color2)
{ }

TextColorButton::TextColorButton(SDL_Renderer* renderer) noexcept
    :TextButton(renderer)
{ }

void TextColorButton::load_color(const char* text, SDL_Color color0,
    SDL_Color color1, SDL_Color color2) noexcept
{
    this->load_text(text, color0, text, color1, text, color2);
}

void TextColorButton::load_color(const wchar_t* text, SDL_Color color0,
    SDL_Color color1, SDL_Color color2) noexcept
{
    this->load_text(text, color0, text, color1, text, color2);
}
