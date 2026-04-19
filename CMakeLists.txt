cmake_minimum_required(VERSION 3.14)
project(JsTui)

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(quickjs
  GIT_REPOSITORY https://github.com/quickjs-ng/quickjs.git
  GIT_TAG v0.8.0
)
FetchContent_MakeAvailable(quickjs)

add_executable(JsTui main.cpp JsEngine.cpp UIFactory.cpp)

target_include_directories(JsTui PRIVATE ${quickjs_SOURCE_DIR})
target_link_libraries(JsTui PRIVATE 
    ftxui::screen 
    ftxui::dom 
    ftxui::component 
    nlohmann_json::nlohmann_json
    qjs
)