#include "selection_manager.h"

namespace fizvlad {namespace selection_manager{

        float Calculator::query(std::string request) {
            Formula f(request, actions);
            float result = f.calculate<float>(converter, handlers);
            return result;
        }

        Calculator::Calculator() {
            Formula::Action a_brackets(1, R"RAW_LITERAL(\(([^\(\)]+)\))RAW_LITERAL");
            Formula::Action a_plus(2,     R"RAW_LITERAL(([^\s+*\/\-]+)\s*\+\s*([^\s+*\/\-]+))RAW_LITERAL");
            Formula::Action a_minus(2,    R"RAW_LITERAL(([^\s+*\/\-]+)\s*\-\s*([^\s+*\/\-]+))RAW_LITERAL");
            Formula::Action a_multiply(2, R"RAW_LITERAL(([^\s+*\/\-]+)\s*\*\s*([^\s+*\/\-]+))RAW_LITERAL");
            Formula::Action a_divide(2,   R"RAW_LITERAL(([^\s+*\/\-]+)\s*\/\s*([^\s+*\/\-]+))RAW_LITERAL");

            actions = {a_brackets, a_multiply, a_divide, a_plus, a_minus};



            handlers.push_back([](std::vector<float> input){
                return input[0];
            });
            handlers.push_back([](std::vector<float> input){
                return input[0] * input[1];
            });
            handlers.push_back([](std::vector<float> input){
                return input[0] / input[1];
            });
            handlers.push_back([](std::vector<float> input){
                return input[0] + input[1];
            });
            handlers.push_back([](std::vector<float> input){
                return input[0] - input[1];
            });


            converter = [](std::string str){return stof(str);};
        }

}}
