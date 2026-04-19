#pragma once
#include <map>
#include <string>
#include <functional>
#include <ftxui/component/component.hpp>
#include <nlohmann/json.hpp>
#include "JsEngine.h"

using ComponentBuilder = std::function<ftxui::Component(const nlohmann::json&, JsEngine&, std::function<void()>)>;

class UIFactory {
public:
    UIFactory();
    void buildUI(const nlohmann::json& data, ftxui::Component& container, JsEngine& jsEngine, std::function<void()> on_refresh);

private:
    std::map<std::string, ComponentBuilder> builders_;
};