#include "ApplyStyles.h"

using StyleHandler = std::function<ftxui::Element(ftxui::Element, const nlohmann::json&)>;

ftxui::Element applyStyles(ftxui::Element el, const nlohmann::json& styles) {
    static const std::map<std::string, StyleHandler> style_handlers = {
        {"bold", [](ftxui::Element e, const nlohmann::json& val) {
            return val.get<bool>() ? e | ftxui::bold : e;
        }},
        {"dim", [](ftxui::Element e, const nlohmann::json& val) {
            return val.get<bool>() ? e | ftxui::dim : e;
        }},
        {"underlined", [](ftxui::Element e, const nlohmann::json& val) {
            return val.get<bool>() ? e | ftxui::underlined : e;
        }},
        {"center", [](ftxui::Element e, const nlohmann::json& val) {
            return val.get<bool>() ? e | ftxui::center : e;
        }},
        {"flex", [](ftxui::Element e, const nlohmann::json& val) {
            return val.get<bool>() ? e | ftxui::flex : e;
        }},
        {"color", [](ftxui::Element e, const nlohmann::json& val) {
            int r = val.value("r", 255);
            int g = val.value("g", 255);
            int b = val.value("b", 255);
            return e | ftxui::color(ftxui::Color::RGB(r, g, b));
        }},
        {"bgcolor", [](ftxui::Element e, const nlohmann::json& val) {
            int r = val.value("r", 0);
            int g = val.value("g", 0);
            int b = val.value("b", 0);
            return e | ftxui::bgcolor(ftxui::Color::RGB(r, g, b));
        }},
        {"border", [](ftxui::Element e, const nlohmann::json& val) {
            std::string b_type = val.get<std::string>();
            if (b_type == "rounded") return e | ftxui::borderRounded;
            if (b_type == "double") return e | ftxui::borderDouble;
            if (b_type == "normal") return e | ftxui::border;
            if (b_type == "empty") return e | ftxui::borderEmpty;
            return e;
        }}
    };

    for (auto it = styles.begin(); it != styles.end(); ++it) {
        std::string key = it.key();
        if (style_handlers.count(key)) {
            el = style_handlers.at(key)(el, it.value());
        }
    }

    return el;
}
