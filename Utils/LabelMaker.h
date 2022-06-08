#pragma once
#include <vector>
#include "Utils/Math.h"

namespace LabelMaker
{
    enum Labels
    {
        T = 0,
        CT,
        T_HEAD,
        CT_HEAD
    };
    void makeLabel(std::vector<Math::Vec5> labels);
}
