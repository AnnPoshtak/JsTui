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
    std::cout << "Welcome to JsTui\nEnter file path: ";
    std::string file_path;
    std::cin >> file_path;
    
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file" << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf(); 

    JsEngine engine(buffer.str());
    UIFactory factory;

    auto screen = ScreenInteractive::TerminalOutput();
    
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