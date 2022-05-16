#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <iostream>
#include <memory>
#include <string>
#include <optional>
#include <utility>
#include <math.h>
#include "mouse.h"

class Result {
public:
    class Stat {
    public:
        Stat() = default;
        Stat(std::shared_ptr<Mouse> mouse, std::string path, bool best);
    // private:
        std::shared_ptr<Mouse> mouse_;
        std::string path_;
        bool best_;
    };
    void AddMouseAnalysis(std::shared_ptr<Mouse> m);
    void Print() {
        for (size_t i = 0; i < res_.size(); i++) {
            std::cout << res_[i].path_.size() << '\t' << res_[i].path_ << '\n';
        }
    }
// private:
    std::vector<Stat> res_;
};

Result GetCompressions(Seria& s);

std::optional<std::string> TryToFindPath(const std::string& s, const std::string& b, size_t pos);

#endif