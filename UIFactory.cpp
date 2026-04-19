#include "UIFactory.h"

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