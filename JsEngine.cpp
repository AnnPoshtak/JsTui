#include "JsEngine.h"
#include <iostream>
JsEngine::JsEngine(const std::string& js_code) {
    rt_ = JS_NewRuntime();
    ctx_ = JS_NewContext(rt_);
    
    JSValue eval_ret = JS_Eval(ctx_, js_code.c_str(), js_code.length(), "index.js", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(eval_ret)) {
        JSValue exc = JS_GetException(ctx_);
        const char *err = JS_ToCString(ctx_, exc);
        std::cerr << "JS init error: " << (err ? err : "unknown") << std::endl;
        JS_FreeCString(ctx_, err);
        JS_FreeValue(ctx_, exc);
    }
    JS_FreeValue(ctx_, eval_ret);
}

JsEngine::~JsEngine() {
    JS_FreeContext(ctx_);
    JS_FreeRuntime(rt_);
}

std::string JsEngine::callRender() {
    JSValue global_obj = JS_GetGlobalObject(ctx_);
    JSValue func = JS_GetPropertyStr(ctx_, global_obj, "render");
    std::string result = "";

    if (JS_IsFunction(ctx_, func)) {
        JSValue ret = JS_Call(ctx_, func, global_obj, 0, nullptr);
        
        if (JS_IsException(ret)) {
            JSValue exc = JS_GetException(ctx_);
            const char *err = JS_ToCString(ctx_, exc);
            std::cerr << "JS render error: " << (err ? err : "unknown") << std::endl;
            JS_FreeCString(ctx_, err);
            JS_FreeValue(ctx_, exc);
        } else {
            const char *str = JS_ToCString(ctx_, ret);
            if (str) {
                result = str;
                JS_FreeCString(ctx_, str);
            }
        }
        JS_FreeValue(ctx_, ret);
    } else {
        std::cerr << "JS error: 'render' is not a function" << std::endl;
    }
    
    JS_FreeValue(ctx_, func);
    JS_FreeValue(ctx_, global_obj);
    return result;
}

void JsEngine::callAction(const std::string& action_name) {
    JSValue global_obj = JS_GetGlobalObject(ctx_);
    JSValue func = JS_GetPropertyStr(ctx_, global_obj, action_name.c_str());

    if (JS_IsFunction(ctx_, func)) {
        JSValue ret = JS_Call(ctx_, func, global_obj, 0, nullptr);
        
        if (JS_IsException(ret)) {
            JSValue exc = JS_GetException(ctx_);
            const char *err = JS_ToCString(ctx_, exc);
            std::cerr << "JS action error: " << (err ? err : "unknown") << std::endl;
            JS_FreeCString(ctx_, err);
            JS_FreeValue(ctx_, exc);
        }
        JS_FreeValue(ctx_, ret);
    } else {
        std::cerr << "JS warning: Action '" << action_name << "' not found." << std::endl;
    }
    
    JS_FreeValue(ctx_, func);
    JS_FreeValue(ctx_, global_obj);
}

void JsEngine::callActionWithInt(const std::string& actionName, int value) {
    JSValue global_obj = JS_GetGlobalObject(ctx_); 
    
    JSValue func = JS_GetPropertyStr(ctx_, global_obj, actionName.c_str());
    
    if (JS_IsFunction(ctx_, func)) {
        JSValue arg = JS_NewInt32(ctx_, value); 
        
        JS_Call(ctx_, func, global_obj, 1, &arg); 

        JS_FreeValue(ctx_, arg);
    } else {
        std::cerr << "Function " << actionName << " not found in JS!" << std::endl;
    }
    
    JS_FreeValue(ctx_, func);
    JS_FreeValue(ctx_, global_obj);
}

void JsEngine::callActionWithString(const std::string& actionName, std::string& value) {
    JSValue global_obj = JS_GetGlobalObject(ctx_); 
    
    JSValue func = JS_GetPropertyStr(ctx_, global_obj, actionName.c_str());
    
    if (JS_IsFunction(ctx_, func)) {
        JSValue arg = JS_NewString(ctx_, value.c_str());
        
        JS_Call(ctx_, func, global_obj, 1, &arg); 

        JS_FreeValue(ctx_, arg);
    } else {
        std::cerr << "Function " << actionName << " not found in JS!" << std::endl;
    }
    
    JS_FreeValue(ctx_, func);
    JS_FreeValue(ctx_, global_obj);
}