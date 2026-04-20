#include "UIFactory.h"
#include "ApplyStyles.h"

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

        return ftxui::Renderer([item, text_val] {
            ftxui::Element el = ftxui::text(text_val);
            if (item.contains("styles")) {
                el = applyStyles(el, item["styles"]);
            }
            return el;
        });
    };

    builders_["separator"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        return ftxui::Renderer([] { return ftxui::separator(); });
    };

    builders_["button"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        std::string text_val = item.value("content", "Button");
        std::string action = item.value("action", "");

        auto btn = ftxui::Button(text_val, [&js, action, on_refresh] {
            if (!action.empty()) {
                js.callAction(action);
                on_refresh();
            }
        });

        if (item.contains("styles")) {
            btn = ftxui::Renderer(btn, [btn, item] {
                return applyStyles(btn->Render(), item["styles"]);
            });
        }
        return btn;
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
        
        auto checkbox = ftxui::Checkbox(text_val, state.get(), option);
        if (item.contains("styles")) {
            checkbox = ftxui::Renderer(checkbox, [checkbox, item] {
                return applyStyles(checkbox->Render(), item["styles"]);
            });
        }
        return checkbox;
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

        auto radiobox = ftxui::Radiobox(entries_ptr.get(), selected_ptr.get(), option);
        if (item.contains("styles")) {
            radiobox = ftxui::Renderer(radiobox, [radiobox, item] {
                return applyStyles(radiobox->Render(), item["styles"]);
            });
        }
        return radiobox;
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

        auto menu = ftxui::Menu(entries_ptr.get(), selected_ptr.get(), option);
        if (item.contains("styles")) {
            menu = ftxui::Renderer(menu, [menu, item] {
                return applyStyles(menu->Render(), item["styles"]);
            });
        }
        return menu;
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
        
        auto toggle = ftxui::Menu(entries_ptr.get(), selected_ptr.get(), option);
        if (item.contains("styles")) {
            toggle = ftxui::Renderer(toggle, [toggle, item] {
                return applyStyles(toggle->Render(), item["styles"]);
            });
        }
        return toggle;
    };

    builders_["input"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        std::string placeholder = item.value("placeholder", "");
        std::string action = item.value("action", "");

        auto text_state = std::make_shared<std::string>();
        ftxui::InputOption option;
        
        option.on_enter = [&js, action, on_refresh, text_state]() {
            if (!action.empty()) {
                js.callActionWithString(action, *text_state); 
            }
            on_refresh();
        };

        auto input = ftxui::Input(text_state.get(), placeholder, option);
        if (item.contains("styles")) {
            input = ftxui::Renderer(input, [input, item] {
                return applyStyles(input->Render(), item["styles"]);
            });
        }
        return input;
    };

    builders_["hbox"] = [this](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        ftxui::Components children;

        if (item.contains("content") && item["content"].is_array()) {
            for (const auto& child : item["content"]) {
                std::string child_type = child.value("type", "unknown");
                if (builders_.count(child_type)) {
                    children.push_back(builders_[child_type](child, js, on_refresh));
                } else {
                    children.push_back(ftxui::Renderer([child_type] { return ftxui::text("Unknown: " + child_type); }));
                }
            }
        }

        auto container = ftxui::Container::Horizontal(children);
        if (item.contains("styles")) {
            container = ftxui::Renderer(container, [container, item] {
                return applyStyles(container->Render(), item["styles"]);
            });
        }
        return container;
    };
    
    builders_["vbox"] = [this](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        ftxui::Components children;

        if (item.contains("content") && item["content"].is_array()) {
            for (const auto& child : item["content"]) {
                std::string child_type = child.value("type", "unknown");
                if (builders_.count(child_type)) {
                    children.push_back(builders_[child_type](child, js, on_refresh));
                } else {
                    children.push_back(ftxui::Renderer([child_type] { return ftxui::text("Unknown: " + child_type); }));
                }
            }
        }

        auto container = ftxui::Container::Vertical(children);
        if (item.contains("styles")) {
            container = ftxui::Renderer(container, [container, item] {
                return applyStyles(container->Render(), item["styles"]);
            });
        }
        return container;
    };

    builders_["slider"] = [](const nlohmann::json& item, JsEngine& js, std::function<void()> on_refresh) {
        auto value = std::make_shared<int>(item.value("value", 0));
        std::string content = item.value("content", "");
        std::string action = item.value("action", "");

        auto base_slider = ftxui::Slider(content, value.get(), 0, 100, 1);
        auto slider = ftxui::CatchEvent(base_slider, [&js, action, on_refresh, value](ftxui::Event e) {
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

        if (item.contains("styles")) {
            slider = ftxui::Renderer(slider, [slider, item] {
                return applyStyles(slider->Render(), item["styles"]);
            });
        }
        return slider;
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
                    children.push_back(ftxui::Renderer([child_type] { return ftxui::text("Unknown: " + child_type); }));
                }
            }
        }
        
        auto inner_container = ftxui::Container::Vertical(children);
        auto win_component = ftxui::Renderer(inner_container, [inner_container, title] {
            return ftxui::window(ftxui::text(title), inner_container->Render());
        });

        if (item.contains("styles")) {
            win_component = ftxui::Renderer(win_component, [win_component, item] {
                return applyStyles(win_component->Render(), item["styles"]);
            });
        }
        return win_component;
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