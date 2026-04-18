#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

struct Storage {
    fs::path path;
    std::string ext;
    Storage(std::string p, std::string e) : path(std::move(p)), ext(std::move(e)) {}
};

class Organizer {
private:
    std::vector<Storage> data;
    const std::string pathFile = "paths.pth";

    void loadPaths();
    void savePaths();

public:
    Organizer();
    ~Organizer() = default;

    bool addRule(const std::string& p, const std::string& e);
    bool removeRule(const std::string& e);
    int organize(const std::string& srcStr);  // retorna quantidade de arquivos movidos (-1 em erro)
    const std::vector<Storage>& getRules() const { return data; }
};