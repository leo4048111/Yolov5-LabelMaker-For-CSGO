#pragma once
#include <vector>
#include "Utils/Math.h"

namespace LabelMaker
{
    enum Labels
    {
        CT = 0,
        CT_HEAD,
        T,
        T_HEAD
    };
    void makeLabel(std::vector<Math::Vec5> labels);
}
