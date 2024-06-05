#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <ftxui/dom/elements.hpp>

std::vector<std::string> title_normal {
    "   ____         __________       __________       ________  ",
    "  6MMMMb\\       MMMMMMMMMM       `MMMMMMMMM       `MMMMMMMb.",
    " 6M'    `       /   MM   \\        MM      \\        MM    `Mb",
    " MM                 MM            MM               MM     MM",
    " YM.                MM            MM    ,          MM     MM",
    "  YMMMMb            MM            MMMMMMM          MM    .M9",
    "      `Mb           MM            MM    `          MMMMMMM9'",
    "       MM           MM            MM               MM       ",
    "       MM           MM            MM               MM       ",
    " L    ,M9           MM            MM      /        MM       ",
    " MYMMMM9           _MM_          _MMMMMMMMM       _MM_      "
};

std::vector<std::string> title_big {
    "   ____                                 ____              ___                                  ",
    "  6MMMMb\\                               `Mb(      db      )d'        68b                       ",
    " 6M'    `  /                             YM.     ,PM.     ,P         Y89   /                   ",
    " MM       /M      ____  __ ____          `Mb     d'Mb     d' ___  __ ___  /M      ____  ___  __",
    " YM.     /MMMMM  6MMMMb `M6MMMMb          YM.   ,P YM.   ,P  `MM 6MM `MM /MMMMM  6MMMMb `MM 6MM",
    "  YMMMMb  MM    6M'  `Mb MM'  `Mb         `Mb   d' `Mb   d'   MM69    MM  MM    6M'  `Mb MM69  ",
    "      `Mb MM    MM    MM MM    MM          YM. ,P   YM. ,P    MM'     MM  MM    MM    MM MM'   ",
    "       MM MM    MMMMMMMM MM    MM          `Mb d'   `Mb d'    MM      MM  MM    MMMMMMMM MM    ",
    "       MM MM    MM       MM    MM           YM,P     YM,P     MM      MM  MM    MM       MM    ",
    " L    ,M9 YM.  ,YM    d9 MM.  ,M9           `MM'     `MM'     MM      MM  YM.  ,YM    d9 MM    ",
    " MYMMMM9   YMMM9 YMMMM9  MMYMMM9             YP       YP     _MM_    _MM_  YMMM9 YMMMM9 _MM_   ",
    "                         MM                                                                    ",
    "                         MM                                                                    ",
    "                        _MM_                                                                   "
};

std::vector<std::string> title_small {
    " ███████╗████████╗███████╗██████╗     ██╗    ██╗██████╗ ██╗████████╗███████╗██████╗ ",
    " ██╔════╝╚══██╔══╝██╔════╝██╔══██╗    ██║    ██║██╔══██╗██║╚══██╔══╝██╔════╝██╔══██╗",
    " ███████╗   ██║   █████╗  ██████╔╝    ██║ █╗ ██║██████╔╝██║   ██║   █████╗  ██████╔╝",
    " ╚════██║   ██║   ██╔══╝  ██╔═══╝     ██║███╗██║██╔══██╗██║   ██║   ██╔══╝  ██╔══██╗",
    " ███████║   ██║   ███████╗██║         ╚███╔███╔╝██║  ██║██║   ██║   ███████╗██║  ██║",
    " ╚══════╝   ╚═╝   ╚══════╝╚═╝          ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝",
};

ftxui::Component Text(const std::string& t) {
  return ftxui::Renderer([t] { return ftxui::text(t);});
}

ftxui::ButtonOption CustomButton() {
    using namespace ftxui;

    ButtonOption option;
    option.transform = [](const EntryState& s) {
        const std::string t = s.focused ? "> " + s.label + " "  //
                                        : "  " + s.label + " ";
        if (s.focused)
            return text(t) | color(Color::Blue);
        return text(t);
    };
    return option;
}


#endif