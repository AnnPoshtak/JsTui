#pragma once
#include <string>
#include <quickjs.h>

class JsEngine {
public:
    explicit JsEngine(const std::string& js_code);

    ~JsEngine();

    JsEngine(const JsEngine&) = delete;
    JsEngine& operator=(const JsEngine&) = delete;

    std::string callRender();
    void callAction(const std::string& action_name);
    void callActionWithInt(const std::string& actionName, int value);

private:
    JSRuntime* rt_;
    JSContext* ctx_;
};