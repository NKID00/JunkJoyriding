#include "header.h"

constexpr SDL_Color Junk::get_color(Junk::Color color)
{
    constexpr auto l = 255;  // light
    constexpr auto d = 220;  // dark
    switch (color)
    {
    case Junk::Color::White:
        return make_color(l, l, l);
    case Junk::Color::Red:
        return make_color(l, d, d);
    case Junk::Color::Yellow:
        return make_color(l, l, d);
    case Junk::Color::Green:
        return make_color(d, l, d);
    case Junk::Color::Cyan:
        return make_color(d, l, l);
    case Junk::Color::Blue:
        return make_color(d, d, l);
    case Junk::Color::Magenta:
    default:
        return make_color(l, d, l);
    }
}

constexpr SDL_Color Junk::get_color(uint8_t color)
{
    return Junk::get_color(static_cast<Junk::Color>(color));
}

void Junk::load(SDL_Renderer *renderer) noexcept
{
    Junk::junk = new Image(renderer, "assets/junk.png");
    Junk::junk_ = new Image(renderer, "assets/junk_.png");
    Junk::junkl = new Image(renderer, "assets/junkl.png");
    Junk::junkr = new Image(renderer, "assets/junkr.png");
    Junk::junk1 = new Image(renderer, "assets/junk1.png");
    Junk::junk2 = new Image(renderer, "assets/junk2.png");
    Junk::junk3 = new Image(renderer, "assets/junk3.png");
    Junk::junk4 = new Image(renderer, "assets/junk4.png");
}

void Junk::quit() noexcept
{
    delete_s(Junk::junk);
    delete_s(Junk::junk_);
    delete_s(Junk::junkl);
    delete_s(Junk::junkr);
    delete_s(Junk::junk1);
    delete_s(Junk::junk2);
    delete_s(Junk::junk3);
    delete_s(Junk::junk4);
}

Junk::Junk() noexcept
    : x(0), y(0), rotation(0), color(Junk::Color::White),
    flame(Junk::Flame::None), rcs(Junk::RCS::None)
{ }

void Junk::show() noexcept
{
    Junk::show(this->x, this->y, this->rotation, this->color,
        this->flame, this->rcs);
}

void Junk::show(int x, int y, double rotation, Junk::Color color,
    Junk::Flame flame, Junk::RCS rcs) noexcept
{
    auto rcs_img(Junk::junk_);  // Junk::RCS::None
    switch (rcs)
    {
    case Junk::RCS::Left:
        rcs_img = Junk::junkl;
        break;
    case Junk::RCS::Right:
        rcs_img = Junk::junkr;
        break;
    }
    rcs_img->set_position(x, y);
    rcs_img->angle = rotation;
    rcs_img->show();

    Junk::junk->set_position(x, y);
    Junk::junk->angle = rotation;
    auto c(Junk::get_color(color));
    Junk::junk->set_color(c.r, c.g, c.b);
    Junk::junk->show();

    if (flame != Junk::Flame::None)
    {
        auto flame_img(Junk::junk4);  // Junk::Flame::Flame100
        switch (flame)
        {
        case Junk::Flame::Flame25:
            flame_img = Junk::junk1;
            break;
        case Junk::Flame::Flame50:
            flame_img = Junk::junk2;
            break;
        case Junk::Flame::Flame75:
            flame_img = Junk::junk3;
            break;
        }
        flame_img->set_position(x, y);
        flame_img->angle = rotation;
        flame_img->show();
    }
}
