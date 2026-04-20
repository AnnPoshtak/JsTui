#include "UIFactory.h"

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

UIFactory::UIFactory() {
    builders_["text"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        std::string text_val = "";
        
        if (item.contains("content")) {
            if (item["content"].is_string()) {
                text_val = item["content"].get<std::string>();
            } else if (item["content"].is_number()) {
                text_val = std::to_string(item["content"].get<int>());
            }
        }
        
        return ftxui::Renderer([text_val] { return ftxui::text(text_val); });
    };

    builders_["separator"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        return ftxui::Renderer([] { return ftxui::separator(); });
    };

    builders_["button"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        std::string text_val = item.value("content", "Button");
        std::string action = item.value("action", "");
        
        return ftxui::Button(text_val, [&js, action, on_refresh] {
            if (!action.empty()) {
                js.callAction(action);
                on_refresh();
            }
        });
    };

    builders_["checkbox"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        std::string text_val = item.value("content", "Checkbox");
        bool is_checked = item.value("checked", false);
        std::string action = item.value("action", "");
        auto state = std::make_shared<bool>(is_checked);

        ftxui::CheckboxOption option;
        option.on_change = [&js, action, on_refresh, state]() {
            if (!action.empty()) {
                js.callAction(action);
                on_refresh();
            }
        };
        return ftxui::Checkbox(text_val, state.get(), option);
    };

    builders_["radiobox"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        std::vector<std::string> entries = item.value("content", std::vector<std::string>{"None"});
        int selected_idx = item.value("selected", 0);
        
        std::string action = item.value("action", ""); 

        auto entries_ptr = std::make_shared<std::vector<std::string>>(entries);
        auto selected_ptr = std::make_shared<int>(selected_idx);

        ftxui::RadioboxOption option;
        option.on_change = [&js, action, on_refresh, selected_ptr, entries_ptr]() {
            if (!action.empty()) {
                js.callActionWithInt(action, *selected_ptr); 
            }
            on_refresh();
        };

        return ftxui::Radiobox(entries_ptr.get(), selected_ptr.get(), option);
    };

    builders_["menu"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        std::vector<std::string> entries = item.value("content", std::vector<std::string>{"None"});
        int selected_idx = item.value("selected", 0);
        
        std::string action = item.value("action", ""); 

        auto entries_ptr = std::make_shared<std::vector<std::string>>(entries);
        auto selected_ptr = std::make_shared<int>(selected_idx);

        ftxui::MenuOption option;
        option.on_change = [&js, action, on_refresh, selected_ptr, entries_ptr]() {
            if (!action.empty()) {
                js.callActionWithInt(action, *selected_ptr); 
            }
            on_refresh();
        };

        return ftxui::Menu(entries_ptr.get(), selected_ptr.get(), option);
    };

    builders_["toggle"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        std::vector<std::string> entries = item.value("content", std::vector<std::string>{"None"});
        int selected_idx = item.value("selected", 0);
        
        std::string action = item.value("action", ""); 

        auto entries_ptr = std::make_shared<std::vector<std::string>>(entries);
        auto selected_ptr = std::make_shared<int>(selected_idx);
        
        ftxui::MenuOption option = ftxui::MenuOption::Toggle();
        
        option.on_change = [&js, action, on_refresh, selected_ptr, entries_ptr]() {
            if (!action.empty()) {
                js.callActionWithInt(action, *selected_ptr); 
            }
            on_refresh();
        };
        return ftxui::Menu(entries_ptr.get(), selected_ptr.get(), option);
    };

    builders_["input"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        std::string placeholder = item.value("placeholder", "");
        std::string action = item.value("action", "");
        std::string content = item.value("content", "");

        auto text_state = std::make_shared<std::string>();
        ftxui::InputOption option;
        
        option.on_enter = [&js, action, on_refresh, text_state]() {
            if (!action.empty()) {
                js.callActionWithString(action, *text_state); 
            }
            on_refresh();
        };

        return ftxui::Input(text_state.get(), placeholder, option);
    };
    
    builders_["hbox"] = [this](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        ftxui::Components children;

        if (item.contains("content") && item["content"].is_array()) {
            for (const auto& child : item["content"]) {
                std::string child_type = child.value("type", "unknown");

                if (builders_.count(child_type)) {
                    children.push_back(builders_[child_type](child, js, on_refresh));
                } else {
                    children.push_back(
                        ftxui::Renderer([child_type] {
                            return ftxui::text("Unknown type: " + child_type);
                        })
                    );
                }
            }
        }

        return ftxui::Container::Horizontal(children);
    };
    
    builders_["vbox"] = [this](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        ftxui::Components children;

        if (item.contains("content") && item["content"].is_array()) {
            for (const auto& child : item["content"]) {
                std::string child_type = child.value("type", "unknown");

                if (builders_.count(child_type)) {
                    children.push_back(builders_[child_type](child, js, on_refresh));
                } else {
                    children.push_back(
                        ftxui::Renderer([child_type] {
                            return ftxui::text("Unknown type: " + child_type);
                        })
                    );
                }
            }
        }

        return ftxui::Container::Vertical(children);
    };

    builders_["slider"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        auto value = std::make_shared<int>(item.value("value", 0));
        std::string content = item.value("content", "");
        std::string action = item.value("action", "");

        auto base_slider = ftxui::Slider(content, value.get(), 0, 100, 1);
        return ftxui::CatchEvent(base_slider, [&js, action, on_refresh, value](ftxui::Event e) {
            int old_value = *value;
            if (e == ftxui::Event::ArrowRight) {
                *value = std::min(100, *value + 1);
            } else if (e == ftxui::Event::ArrowLeft) {
                *value = std::max(0, *value - 1);
            } else {
                return false; 
            }
            if (*value != old_value && !action.empty()) {
                js.callActionWithInt(action, *value);
                on_refresh();
            }
            return true; 
        });
    };

    builders_["window"] = [this](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        ftxui::Components children;
        std::string title = item.value("title", "");

        if (item.contains("content") && item["content"].is_array()) {
            for (const auto& child : item["content"]) {
                std::string child_type = child.value("type", "unknown");

                if (builders_.count(child_type)) {
                    children.push_back(builders_[child_type](child, js, on_refresh));
                } else {
                    children.push_back(
                        ftxui::Renderer([child_type] {
                            return ftxui::text("Unknown type: " + child_type);
                        })
                    );
                }
            }
        }
        auto inner_container = ftxui::Container::Vertical(children);

        return ftxui::Renderer(inner_container, [inner_container, title] {
            return ftxui::window(ftxui::text(title), inner_container->Render());
        });
    };
}

void UIFactory::buildUI(const nlohmann::json& data, ftxui::Component& container, JsEngine& jsEngine, std::function<void()> on_refresh) {
    container->DetachAllChildren();
    
    if (data.contains("content") && data["content"].is_array()) {
        for (const auto& item : data["content"]) {
            std::string type = item.value("type", "unknown");
            
            if (builders_.count(type)) {
                auto new_component = builders_[type](item, jsEngine, on_refresh);
                container->Add(new_component);
            } else {
                container->Add(ftxui::Renderer([type] { return ftxui::text("Unknown type: " + type); }));
            }
        }
    }
}