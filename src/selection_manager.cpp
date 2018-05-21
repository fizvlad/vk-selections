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




        Selection Selector::query(std::string request) {
            Formula f(request, actions);
            Selection result = f.calculate<Selection>(converter, handlers);
            return result;
        }

        Selector::Selector(fizvlad::vk_api::Token groupToken, fizvlad::vk_api::Token serviceToken) : gtoken_(groupToken), stoken_(serviceToken) {
            Formula::Action a_brackets(1, R"RAW_LITERAL(\(([^\(\)]+)\))RAW_LITERAL");
            Formula::Action a_not(1,      R"RAW_LITERAL(!\s*([^\s|&]+))RAW_LITERAL");
            Formula::Action a_and(2,      R"RAW_LITERAL(([^\s|&]+)\s*\&\&\s*([^\s|&]+))RAW_LITERAL");
            Formula::Action a_or(2,       R"RAW_LITERAL(([^\s|&]+)\s*\|\|\s*([^\s|&]+))RAW_LITERAL");

            actions = {a_brackets, a_not, a_and, a_or};



            handlers.push_back([](std::vector<Selection> input){
                return input[0];
            });
            handlers.push_back([](std::vector<Selection> input){
                return !input[0];
            });
            handlers.push_back([](std::vector<Selection> input){
                return input[0] && input[1];
            });
            handlers.push_back([](std::vector<Selection> input){
                return input[0] || input[1];
            });


            converter = [this](std::string str){
                size_t pointPos = str.find('.');
                std::string id = str.substr(0, pointPos);
                std::string method = str.substr(pointPos + 1);

                vk_selection::Unit unit(id, gtoken_);
                if (unit.type() == vk_selection::Undefined) {
                    throw vk_api::ApiRequestException("Undefined unit");
                }


                if (method == "auto") {
                    if (unit.type() == vk_selection::User) {
                        return unit.friends(stoken_);
                    } else {
                        return unit.members(gtoken_);
                    }
                }
                if (method == "members") {
                    return unit.members(gtoken_);
                }
                if (method == "friends") {
                    return unit.friends(stoken_);
                }
                /*if (method == "subscribers") {
                }*/
                throw vk_api::ApiRequestException("Unsupported unit method");
            };
        }

}}
