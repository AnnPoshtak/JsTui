#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <nlohmann/json.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>       
#include <ftxui/component/screen_interactive.hpp> 
#include <ftxui/component/component.hpp>

#include "JsEngine.h"
#include "UIFactory.h"

using namespace ftxui;
using json = nlohmann::json;

ftxui::Color parseColor(const json& color_data) {
    if (color_data.is_object()) {
        if (color_data.contains("r") && color_data.contains("g") && color_data.contains("b")) {
            return ftxui::Color::RGB(color_data["r"], color_data["g"], color_data["b"]);
        }
    }
    return ftxui::Color::White;
}

int main() {
    auto screen = ScreenInteractive::TerminalOutput();
    auto logo_lines = ftxui::vbox({
        ftxui::text("       ##   ######   ########  ##     ##  #### "),
        ftxui::text("       ##  ##    ##     ##     ##     ##   ##  "),
        ftxui::text("       ##  ##           ##     ##     ##   ##  "),
        ftxui::text("       ##   ######      ##     ##     ##   ##  "),
        ftxui::text(" ##    ##        ##     ##     ##     ##   ##  "),
        ftxui::text(" ##    ##  ##    ##     ##     ##     ##   ##  "),
        ftxui::text("  ######    ######      ##      #######   #### ")
    }) | ftxui::color(ftxui::Color::YellowLight) | ftxui::bold;

    auto beautiful_logo = ftxui::vbox({
        ftxui::text("/// JSTUI FRAMEWORK INITIALIZATION ///") | ftxui::bold | ftxui::center | ftxui::color(ftxui::Color::CyanLight),
        ftxui::separatorDouble() | ftxui::color(ftxui::Color::Cyan),
        ftxui::emptyElement(),
        logo_lines | ftxui::center,
        ftxui::emptyElement(),
        ftxui::separator() | ftxui::color(ftxui::Color::Cyan),
        ftxui::hbox({
            ftxui::text(" STATUS: ") | ftxui::dim, 
            ftxui::text("AWAITING INPUT") | ftxui::bold | ftxui::color(ftxui::Color::YellowLight),
            ftxui::filler(), 
            ftxui::text("v1.0.0 ") | ftxui::dim
        })
    }) | ftxui::borderRounded | ftxui::color(ftxui::Color::Cyan) | ftxui::center;

    std::string file_path = "";
    ftxui::InputOption input_opt;
    input_opt.on_enter = screen.ExitLoopClosure(); 
    
    auto path_input = ftxui::Input(&file_path, "Enter path...", input_opt);

    auto init_renderer = ftxui::Renderer(path_input, [&] {
        return ftxui::vbox({
            beautiful_logo,
            ftxui::emptyElement(),
            ftxui::hbox({
                ftxui::text(" FILE PATH > ") | ftxui::bold | ftxui::color(ftxui::Color::YellowLight),
                path_input->Render() | ftxui::color(ftxui::Color::White)
            }) | ftxui::border | ftxui::color(ftxui::Color::Cyan) | ftxui::center,
            ftxui::emptyElement(),
            ftxui::text(" Press ENTER to launch, ESC to quit ") | ftxui::dim | ftxui::center
        }) | ftxui::center;
    });

    bool quit_early = false;
    auto init_layout = ftxui::CatchEvent(init_renderer, [&](Event e) {
        if (e == Event::Escape) {
            quit_early = true;
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(init_layout);

    if (quit_early || file_path.empty()) {
        std::cout << "Initialization aborted. Goodbye!" << std::endl;
        return 0;
    }

    if (!file_path.empty()) {
        file_path.erase(file_path.find_last_not_of(" \n\r\t") + 1);
        file_path.erase(0, file_path.find_first_not_of(" \n\r\t"));
    }

    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << file_path << "'" << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf(); 

    JsEngine engine(buffer.str());
    UIFactory factory;
    
    auto container = Container::Vertical({});
    json current_data;

    std::function<void()> refresh_ui = [&]() {
        std::string json_str = engine.callRender();
        try {
            current_data = json::parse(json_str);
            factory.buildUI(current_data, container, engine, refresh_ui);
        } catch (...) { std::cerr << "JSON parse error" << std::endl; }
    };

    refresh_ui();

    auto layout = Renderer(container, [&] {
        Element ui_element = container->Render(); 

        if (current_data.contains("IsBorder") && current_data["IsBorder"] == true) {
            ui_element = ui_element | border | color(parseColor(current_data["BorderColor"]));
        }

        return ui_element;
    });

    auto final_layout = CatchEvent(layout, [&](Event e) {
        if (e == Event::Escape) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(final_layout);
    
    std::cout << "Exiting JsTui. Goodbye!" << std::endl;
    return 0;
}