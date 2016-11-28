#ifndef DRAW_MANAGER_HPP_INCLUDED
#define DRAW_MANAGER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <vec/vec.hpp>

struct entity_manager;
struct drag_manager;
struct shop;

struct draw_manager
{
    sf::RenderWindow window;
    sf::View view;

    sf::Clock clk;

    float dt_ms = 0.f;

    std::string* font_memory = new std::string;
    ImFontConfig* cfg = new ImFontConfig;
    ImGuiStyle style;
    sf::Font font;

    /*struct ImFontConfig
    {
        void*           FontData;                   //          // TTF data
        int             FontDataSize;               //          // TTF data size
        bool            FontDataOwnedByAtlas;       // true     // TTF data ownership taken by the container ImFontAtlas (will delete memory itself). Set to true
        int             FontNo;                     // 0        // Index of font within TTF file
        float           SizePixels;                 //          // Size in pixels for rasterizer
        int             OversampleH, OversampleV;   // 3, 1     // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
        bool            PixelSnapH;                 // false    // Align every character to pixel boundary (if enabled, set OversampleH/V to 1)
        ImVec2          GlyphExtraSpacing;          // 0, 0     // Extra spacing (in pixels) between glyphs
        const ImWchar*  GlyphRanges;                //          // Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list). THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE.
        bool            MergeMode;                  // false    // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs).
        bool            MergeGlyphCenterV;          // false    // When merging (multiple ImFontInput for one ImFont), vertically center new glyphs instead of aligning their baseline

        // [Internal]
        char            Name[32];                               // Name (strictly for debugging)
        ImFont*         DstFont;

        IMGUI_API ImFontConfig();
    };*/

    std::string read_file(const std::string& fname)
    {
        FILE *f = fopen(fname.c_str(), "rb");

        fseek(f, 0, SEEK_END);

        long fsize = ftell(f);

        fseek(f, 0, SEEK_SET);

        std::string str;
        str.resize(fsize+1);

        fread(&str[0], fsize, 1, f);

        fclose(f);

        return str;
    }

    void init(int width, int height)
    {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;

        window.create(sf::VideoMode(width, height), "Title", sf::Style::Default, settings);

        view = window.getDefaultView();

        ImGuiIO io = ImGui::GetIO();

        *font_memory = read_file("ProggyClean.ttf");
        //*font_memory = read_file("VeraMono.ttf");

        if(font_memory->size() == 0)
        {
            std::cout << "No veramono.ttf or read error" << std::endl;
            return;
        }

        cfg->FontData = &(*font_memory)[0];
        cfg->FontDataSize = font_memory->size();
        cfg->FontDataOwnedByAtlas = true;
        cfg->FontNo = 0;
        cfg->SizePixels = 13;
        cfg->OversampleH = 3;
        cfg->OversampleV = 1;
        cfg->PixelSnapH = false;
        cfg->GlyphExtraSpacing = ImVec2(0,0);
        cfg->GlyphRanges = nullptr;
        cfg->MergeMode = false;
        cfg->MergeGlyphCenterV = false;

        io.Fonts->AddFont(cfg);

        //io.MouseDrawCursor = false;

        //io.Fonts->AddFontFromFileTTF("VeraMono.ttf", 13.0f);

        style = ImGui::GetStyle();

        font.loadFromFile("VeraMono.ttf");

        //printf("%f %f style\n", style.FramePadding.x, style.FramePadding.y);
    }

    void tick()
    {
        window.setView(view);

        sf::Event Event;

        while(window.pollEvent(Event))
        {
            ImGui::SFML::ProcessEvent(Event);

            if(Event.type == sf::Event::Closed)
            {
                window.close();
            }

            /*if(Event.type == sf::Event::Resized)
            {
                do_resize = true;

                r_x = Event.size.width;
                r_y = Event.size.height;

                s.width = r_x;
                s.height = r_y;

                s.save("./res/settings.txt");
            }

            if(Event.type == sf::Event::MouseWheelScrolled)
            {
                window.update_scrollwheel_delta(Event);f
            }

            if(Event.type == sf::Event::GainedFocus)
            {
                window.set_focus(true);
            }

            if(Event.type == sf::Event::LostFocus)
            {
                window.set_focus(false);
            }*/
        }

        dt_ms = clk.getElapsedTime().asMicroseconds() / 1000.f;
        clk.restart();

        sf::Time t = sf::microseconds(dt_ms * 1000.f);

        ImGui::SFML::Update(t);

        sf::Keyboard key;

        if(key.isKeyPressed(sf::Keyboard::F10))
            window.close();
    }

    void draw_entity_ui(entity_manager& entity_manage, drag_manager& drag_manage);
    void draw_entity_shop_ui(entity_manager& entity_manage, drag_manager& drag_manage, shop& s);

    void render_ui()
    {
        ImGui::Render();
    }

    vec2f get_mouse_pos()
    {
        sf::Mouse mouse;

        return {(float)mouse.getPosition(window).x, (float)mouse.getPosition(window).y};
    }

    ///ie from
    vec2f screen_to_world(vec2f in)
    {
        /*sf::Transform trans = view.getTransform();

        auto vec = trans.transformPoint({in.v[0], in.v[1]});

        float rat = view.getSize().x / (float)view.getSize().y;

        vec.x = vec.x;// * 1.f / rat;

        return (vec2f){vec.x, vec.y} * (vec2f){view.getSize().x, view.getSize().y};*/

        auto vec = window.mapPixelToCoords({in.v[0], in.v[1]});

        return {vec.x, vec.y};
    }

    void flip()
    {
        window.display();
        window.clear();
    }

    float get_frametime_s()
    {
        return dt_ms / 1000.f;
    }
};

#endif // DRAW_MANAGER_HPP_INCLUDED
