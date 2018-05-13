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

}}


#endif // SELECTION_MANAGER_H_INCLUDED
