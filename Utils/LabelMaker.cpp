#include "pch.h"
#include "LabelMaker.h"
#include "Config.h"
#include "hazedumper/csgo.hpp"
#include "mem.h"
#include "reclass/Entity.h"
#include "reclass/EntityList.h"

#include <string>

extern float boxHeightModifier;

namespace LabelMaker
{
    void makeLabel(std::vector<Math::Vec5> labels) {
        char buffer[256];
        sprintf_s(buffer, "%s%s%d%s", LABEL_SAVE_DIR, SCREENSHOT_FILENAME_PREFIX, imgCnt, ".txt");
        LOG(buffer);
        FILE* fp;
        fopen_s(&fp, buffer, "w");
        if (fp == nullptr)
        {
            LOG("Unable to open file");
            return;
        }
        for (auto label : labels)
        {
            char buf[256];
            sprintf_s(buf, "%d %.5f %.5f %.5f %.5f\n", label.l, label.x, label.y, label.w, label.h);
            fputs(buf, fp);
        }

        fclose(fp);
    }
}
