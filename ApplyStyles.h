#pragma once
#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp>
#include <functional>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

ftxui::Element applyStyles(ftxui::Element el, const nlohmann::json& styles);