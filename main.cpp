#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "duktape.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

    std::cout << "JavaScript context created successfully!" << std::endl;

    std::stringstream buffer;
    buffer << file.rdbuf(); 
    std::string js_code = buffer.str(); 

    std::cout << "File " << file_path << " loaded." << std::endl;

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
    if (!output.empty()) {
        std::cout << "Processed Output:\n" << output << std::endl;
    } else {
        std::cout << "No output to display." << std::endl;
    }  


    std::cout << "Exiting JsTui. Goodbye!" << std::endl;
    
    return 0;
}