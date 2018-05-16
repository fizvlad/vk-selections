#ifndef SELECTION_MANAGER_H_INCLUDED
#define SELECTION_MANAGER_H_INCLUDED

#include <functional>

#include "fizvlad/vk_selection/vk_selection.h"
#include "fizvlad/cpp-formula/cpp-formula.h"

namespace fizvlad {namespace selection_manager {

    class Calculator {
    public:

        float query(std::string request);

        Calculator();


    private:

        std::function<float(std::string)> converter;
        std::vector< std::function< float(std::vector<float>) > > handlers;
        Formula::Actions actions;

    };



    using Selection = fizvlad::vk_selection::Selection;

    class Selector {
    public:

        Selection query(std::string request);

        Selector(fizvlad::vk_api::Token groupToken, fizvlad::vk_api::Token serviceToken);


    private:

        std::function<Selection(std::string)> converter;
        std::vector< std::function< Selection(std::vector<Selection>) > > handlers;
        Formula::Actions actions;

        fizvlad::vk_api::Token gtoken_;
        fizvlad::vk_api::Token stoken_;

    };

}}


#endif // SELECTION_MANAGER_H_INCLUDED
