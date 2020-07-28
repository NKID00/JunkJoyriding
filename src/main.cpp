#include "header.h"

constexpr auto INFO_ABOUT = L"版权所有 (C) NKID00 2020\n\n鸣谢\nSarasa Gothic (更纱黑体)\nSimple DirectMedia Layer\nSDL_image  SDL_mixer\nSDL_net  SDL_ttf";

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

TTF_Font* font_42, * font_32, * font_22;

enum class Status
{
    Start,
    Menu,
    Game,
    Settings,
    About,
    Quit
} status;

SDL_Window* window;
SDL_Renderer* renderer;

int main(int argc, char* argv[])
{
    status = Status::Start;
    info(INFO_NAME " " INFO_VERSION);
    info(INFO_COPYRIGHT);
    debug("正在Debug");

    client_run();

    client_quit();
    return 0;
}

void client_run() noexcept
{
    debug("正在加载");
    debug("SDL_Init");
    if_negative(SDL_Init(SDL_INIT_EVERYTHING), critical_sdl());
    auto load_start_time = SDL_GetTicks();
    debug("IMG_Init");
    if_zero(IMG_Init(IMG_INIT_PNG), critical_img());
    debug("Mix_Init");
    if_zero(Mix_Init(MIX_INIT_FLAC), critical_mix());
    debug("SDLNet_Init");
    if_negative(SDLNet_Init(), critical_net());
    debug("TTF_Init");
    if_negative(TTF_Init(), critical_ttf());

    debug("Mix_OpenAudio");
    if_negative(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024), critical_mix());
    debug("TTF_OpenFont");
    TTF_Font* font_42 = TTF_OpenFont("assets/font.ttf", 42);
    if_nullptr(font_42, critical_ttf());
    TTF_Font* font_32 = TTF_OpenFont("assets/font.ttf", 32);
    if_nullptr(font_32, critical_ttf());
    TTF_Font* font_22 = TTF_OpenFont("assets/font.ttf", 22);
    if_nullptr(font_22, critical_ttf());

    debug("SDL_CreateWindow");
    window = SDL_CreateWindow(
        INFO_NAME " " INFO_VERSION,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if_nullptr(window, critical_sdl());

    debug("SDL_CreateRenderer");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // 加载junk
    debug("Loading Junk");
    Junk::load(renderer);

    // 加载界面
    debug("Loading loading");
    Image logo(renderer, "assets/logo.png");
    logo.set_position((WINDOW_WIDTH - logo.rect.w) / 2, 200);
    logo.show();

    select_font(font_42);
    Text loading(renderer, L"加载中", C_TEXT);
    loading.set_position((WINDOW_WIDTH - loading.rect.w) / 2, logo.rect.y + logo.rect.h + 10);
    loading.show();

    SDL_RenderPresent(renderer);

    // 主菜单
    logo.set_position(-300, 600);
    logo.angle = 45;

    debug("Loading menu");
    select_font(font_42);
    Text text_title(renderer, INFO_NAME, C_TITLE);
    text_title.set_position(WINDOW_WIDTH - 400 - text_title.rect.w / 2, 180);

    select_font(font_32);
    Text text_version(renderer, INFO_VERSION, C_TITLE);
    text_version.set_position(
        WINDOW_WIDTH - 400 - text_version.rect.w / 2,
        text_title.rect.y + text_title.rect.h + 10
    );

    select_font(font_42);
    TextColorButton button_start(renderer, L"开始", C_BUTTON);
    button_start.set_position(
        WINDOW_WIDTH - 400 - button_start.rect.w / 2,
        text_version.rect.y + text_version.rect.h + 60
    );

    TextColorButton button_settings(renderer, L"设置", C_BUTTON);
    button_settings.set_position(
        WINDOW_WIDTH - 400 - button_settings.rect.w / 2,
        button_start.rect.y + button_start.rect.h + 20
    );

    TextColorButton button_about(renderer, L"关于", C_BUTTON);
    button_about.set_position(
        WINDOW_WIDTH - 400 - button_about.rect.w / 2,
        button_settings.rect.y + button_settings.rect.h + 20
    );

    TextColorButton button_quit(renderer, L"退出", C_BUTTON);
    button_quit.set_position(
        WINDOW_WIDTH - 400 - button_quit.rect.w / 2,
        button_about.rect.y + button_about.rect.h + 20
    );

    TextColorButton button_back(renderer, L"返回", C_BUTTON);
    button_back.set_position(
        WINDOW_WIDTH - 400 - button_back.rect.w / 2,
        button_about.rect.y + button_about.rect.h + 20
    );

    // 开始界面
    select_font(font_42);
    Text text_server(renderer, L"服务器: ", C_TEXT);
    set_position(text_server.rect, 200, nullptr);
    Input input_server(renderer);
    set_position(input_server.rect, 200 + text_server.rect.w + 5, nullptr);

    // 设置界面
    select_font(font_42);
    TextColorButton button_color(renderer, L"更改颜色", C_BUTTON);
    button_color.set_position(
        WINDOW_WIDTH - 400 - button_color.rect.w / 2,
        text_version.rect.y + text_version.rect.h + 30
    );

    Junk junk_color;
    junk_color.x = button_color.rect.x - 200;
    junk_color.y = button_color.rect.y - 75;
    junk_color.flame = Junk::Flame::Flame75;

    // 关于界面
    select_font(font_22);
    Paragraph about(renderer, INFO_ABOUT, C_TEXT);
    about.set_position(
        WINDOW_WIDTH - 400 - about.rect.w / 2,
        text_version.rect.y + text_version.rect.h + 30
    );

    // 其他
    SDL_Event e;
    int mouse_x = 0, mouse_y = 0;
    bool mouse_down = false;

#ifndef _DEBUG
    // 装作在加载
    while (SDL_GetTicks() - load_start_time < 500)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_EventType::SDL_QUIT)
            {
                return;
            }
        }
    }
#endif  // DEBUG

    info("加载完毕");

    status = Status::Menu;

    while (true)  // 主循环
    {
        while (SDL_PollEvent(&e) != 0)  // 处理事件
        {
            switch (e.type)
            {
            case SDL_EventType::SDL_MOUSEMOTION:
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
                break;
            case SDL_EventType::SDL_MOUSEBUTTONDOWN:
                mouse_down = true;
                break;
            case SDL_EventType::SDL_MOUSEBUTTONUP:
                mouse_down = false;
                    break;
            case SDL_EventType::SDL_KEYDOWN:
                break;
            case SDL_EventType::SDL_QUIT:
                return;
            }
        }
        SDL_RenderClear(renderer);
        switch (status)  // 渲染
        {
        case Status::Menu:
            logo.rect.x += 5;
            logo.rect.y -= 5;
            if (logo.rect.y < -300)
            {
                logo.rect.x = -300;
                logo.rect.y = 600;
            }
            logo.show();
            text_title.show();
            text_version.show();
            if (button_start.show(mouse_x, mouse_y, mouse_down))
            {
                debug("开始");
                status = Status::Game;
            }
            if (button_settings.show(mouse_x, mouse_y, mouse_down))
            {
                debug("设置");
                status = Status::Settings;
            }
            if (button_about.show(mouse_x, mouse_y, mouse_down))
            {
                debug("关于");
                status = Status::About;
            }
            if (button_quit.show(mouse_x, mouse_y, mouse_down))
            {
                debug("退出");
                return;
            }
            break;
        case Status::Game:
            text_server.show();
            input_server.show();
            if (button_back.show(mouse_x, mouse_y, mouse_down))
            {
                debug("返回");
                status = Status::Menu;
            }
            break;
        case Status::Settings:
            junk_color.show();
            if (button_color.show(mouse_x, mouse_y, mouse_down))
            {
                debug("更改颜色");
                if (junk_color.color == Junk::Color::Magenta)
                {
                    junk_color.color = Junk::Color::White;
                }
                else
                {
                    junk_color.color = static_cast<Junk::Color>(static_cast<uint8_t>(junk_color.color) + 1);
                }
            }
            if (button_back.show(mouse_x, mouse_y, mouse_down))
            {
                debug("返回");
                status = Status::Menu;
            }
            break;
        case Status::About:
            logo.rect.x += 5;
            logo.rect.y -= 5;
            if (logo.rect.y < -300)
            {
                logo.rect.x = -300;
                logo.rect.y = 600;
            }
            logo.show();
            text_title.show();
            text_version.show();
            about.show();
            if (button_back.show(mouse_x, mouse_y, mouse_down))
            {
                debug("返回");
                status = Status::Menu;
            }
            break;
        }
        SDL_RenderPresent(renderer);
    }
}

void client_quit() noexcept
{
    status = Status::Quit;

    debug("扫尾");

    debug("Junk::quit");
    Junk::quit();

    debug("SDL_DestroyRenderer");
    if_not_nullptr(renderer, SDL_DestroyRenderer(renderer));
    debug("SDL_DestroyWindow");
    if_not_nullptr(window, SDL_DestroyWindow(window));

    debug("TTF_CloseFont");
    if_not_nullptr(font_42, TTF_CloseFont(font_42));
    if_not_nullptr(font_32, TTF_CloseFont(font_32));
    if_not_nullptr(font_22, TTF_CloseFont(font_22));

    debug("Mix_CloseAudio");
    Mix_CloseAudio();

    debug("TTF_Quit");
    TTF_Quit();
    debug("SDLNet_Quit");
    SDLNet_Quit();
    debug("Mix_Quit");
    Mix_Quit();
    debug("IMG_Quit");
    IMG_Quit();
    debug("SDL_Quit");
    SDL_Quit();

    info("退出");
}
