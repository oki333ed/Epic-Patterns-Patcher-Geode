#include <Geode/Geode.hpp>
#include <Geode/modify/CCLabelBMFont.hpp>
#include <Geode/modify/CCScene.hpp>
#include <fstream>
#include <vector>
#include <filesystem>

using namespace geode::prelude;

std::vector<std::pair<std::string, std::string>> replacements;

void loadReplacements() {
    std::string path = Mod::get()->getConfigDir().string() + "/patches/asd.txt";

    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(Mod::get()->getConfigDir().string() + "/patches");
        std::ofstream createFile(path);
        createFile.close();
        return;
    }

    replacements.clear();

    std::ifstream file(path);
    std::string from, to;
    while (std::getline(file, from) && std::getline(file, to)) {
        replacements.emplace_back(from, to);
    }
    file.close();
}

std::string replaceText(std::string_view original) {
    for (auto& [from, to] : replacements) {
        if (original == from) return to;
    }
    return std::string(original);
}

class $modify(MyCCLabelBMFont, CCLabelBMFont) {
    static CCLabelBMFont* create(const char* str, const char* fntFile) {
        return CCLabelBMFont::create(replaceText(str).c_str(), fntFile);
    }
};

class $modify(MyCCScene, CCScene) {
    static CCScene* create() {
        loadReplacements();
        return CCScene::create();
    }
};

$on_mod(Loaded) {
    loadReplacements();
}