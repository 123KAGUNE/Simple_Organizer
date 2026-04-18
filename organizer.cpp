#include "organizer.h"
#include <fstream>
#include <algorithm>

Organizer::Organizer() {
    loadPaths();
}

void Organizer::loadPaths() {
    std::ifstream input(pathFile);
    std::string ext, p;
    while (std::getline(input, ext) && std::getline(input, p)) {
        if (!ext.empty() && !p.empty()) {
            data.emplace_back(p, ext);
        }
    }
}

void Organizer::savePaths() {
    std::ofstream output(pathFile);
    for (const auto& d : data) {
        output << d.ext << "\n" << d.path.string() << "\n";
    }
}

bool Organizer::addRule(const std::string& p, const std::string& e) {
    if (e.empty()) return false;
    std::string ext = (e[0] == '.') ? e : "." + e;

    fs::path dest(p);
    if (!fs::exists(dest) || !fs::is_directory(dest)) return false;

    // evita duplicata
    for (const auto& rule : data) {
        if (rule.ext == ext) return false;
    }

    data.emplace_back(p, ext);
    savePaths();
    return true;
}

bool Organizer::removeRule(const std::string& e) {
    if (e.empty()) return false;
    std::string ext = (e[0] == '.') ? e : "." + e;

    auto it = std::remove_if(data.begin(), data.end(),
                             [&ext](const Storage& s){ return s.ext == ext; });
    if (it != data.end()) {
        data.erase(it, data.end());
        savePaths();
        return true;
    }
    return false;
}

int Organizer::organize(const std::string& srcStr) {
    fs::path src(srcStr);
    if (!fs::exists(src) || !fs::is_directory(src)) return -1;

    int count = 0;
    for (const auto& entry : fs::directory_iterator(src)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext.empty()) continue;

        for (const auto& rule : data) {
            if (ext == rule.ext) {
                try {
                    if (!fs::exists(rule.path)) fs::create_directories(rule.path);
                    fs::rename(entry.path(), rule.path / entry.path().filename());
                    count++;
                } catch (...) {}
                break;
            }
        }
    }
    return count;
}