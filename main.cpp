#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector> 

#include "duktape.h"
#include <nlohmann/json.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>       
#include <ftxui/component/screen_interactive.hpp> 
#include <ftxui/component/component.hpp>
#include <memory>

using namespace ftxui;
using json = nlohmann::json;

ftxui::Color parseColor(const json& color_data) {
    if (color_data.is_object()) {
        if (color_data.contains("r") && color_data.contains("g") && color_data.contains("b")) {
            uint8_t r = color_data["r"];
            uint8_t g = color_data["g"];
            uint8_t b = color_data["b"];
            return ftxui::Color::RGB(r, g, b);
        }
    }
    return ftxui::Color::White;
}

std::string processFile() {
    std::string file_path;
    std::cout << "Enter file path (eg: /home/user/code/index.js): ";
    std::cin >> file_path;
    
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file at " << file_path << std::endl;
        return ""; 
    }
    
    duk_context *ctx = duk_create_heap_default();
    if (!ctx) {
        std::cerr << "Error: Could not create JavaScript context!" << std::endl;
        return ""; 
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); 
    std::string js_code = buffer.str(); 

    std::string result_str = "";

    if (duk_peval_string(ctx, js_code.c_str()) != 0) {
        std::cerr << "JS error: " << duk_safe_to_string(ctx, -1) << std::endl;
    } else {
        result_str = duk_safe_to_string(ctx, -1);
    }

    std::string final_output = "";
    if (!result_str.empty()) {
        try {
            json data = json::parse(result_str);
            final_output = data.dump();
        } catch (json::parse_error& e) {
            std::cerr << "JSON Parse error: " << e.what() << std::endl;
        }
    }

    duk_pop(ctx); 
    duk_destroy_heap(ctx);

    return final_output;
}

int main() {
    std::cout << "Welcome to JsTui!" << std::endl;
    
    std::string output = processFile();
    
    if (output.empty()) {
        std::cout << "No output to display. Exiting." << std::endl;
        return 1;
    }

    json data;
    try {
        data = json::parse(output);
    } catch (json::parse_error& e) {
        std::cerr << "Main Parse error: " << e.what() << std::endl;
        return 1;
    }
    
    auto screen = ScreenInteractive::TerminalOutput();

    auto layout = Renderer([&] {
        std::vector<Element> children;

        if (data.contains("content") && data["content"].is_array()) {
            
            for (const auto& item : data["content"]) {
                
                if (item.contains("type") && item["type"] == "text") {
                    std::string text_val = item.contains("content") ? item["content"].get<std::string>() : "";
                    children.push_back(text(text_val)); 
                } else if (item.contains("type") && item["type"] == "separator"){
                    children.push_back(separator());
                }
            }
        }

        Element ui_element;
        std::string boxType = data.contains("boxType") ? data["boxType"].get<std::string>() : "vbox";

        if (boxType == "vbox") {
            ui_element = vbox(std::move(children)); 
        } else if (boxType == "hbox") {
            ui_element = hbox(std::move(children));
        } else {
            ui_element = text("Error: Unknown boxType");
        }

        if (data.contains("IsBorder") && data["IsBorder"] == true) {
            
            bool is_double = false;
            if (data.contains("borderType") && data["borderType"].is_array()) {
                for (const auto& bt : data["borderType"]) {
                    if (bt == "double") is_double = true;
                }
            }

            if (is_double) {
                ui_element = ui_element | borderDouble;
            } else {
                ui_element = ui_element | border;
            }

            if (data.contains("BorderColor")) {
                ui_element = ui_element | color(parseColor(data["BorderColor"]));
            }
        }

        return ui_element;
    });

    screen.Loop(layout);
    std::cout << "Exiting JsTui. Goodbye!" << std::endl;
    return 0;
}