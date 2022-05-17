#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <optional>
#include <utility>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include "mouse.h"

static const int CNT_FEEDERS = 2;
static const char FEEDERS[CNT_FEEDERS] = {'P', 'D'};
static const size_t CNT_OF_PARTS = 3;

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

class LenStat{
public:
    class Path{
    public:
        Path();
        Path(const std::string& p);
        uint32_t len_[CNT_OF_PARTS];
        size_t cnt_;
    };
    LenStat() = default;
    LenStat(Seria& s);
    void AddMouse(std::shared_ptr<Mouse> m);
    const std::vector<std::vector<Path>>& GetTrialsData() const;
    const std::map<std::string, std::vector<Path>>& GetData() const;
private:
    std::map<std::string, std::vector<Path>> data_;
    std::vector<std::vector<Path>> trials_data_;
};

void BuildPlotData(Seria& f, Seria& h);

std::vector<std::pair<uint32_t, double>> GetAvgOnPart(LenStat& stat, size_t part);

std::vector<std::pair<uint32_t, uint32_t>> GetMedOnPart(LenStat& stat, size_t part);

struct SorterByPart{
    SorterByPart(size_t part):part_(part){}
    size_t part_ = 0;
    bool operator()(LenStat::Path a, LenStat::Path b);
};

uint32_t GetCntFeedersFound(const LenStat& s, uint32_t cnt);

void PrintTableFeedersFound(Seria& s);

std::vector<std::pair<uint32_t, double>> GetAvgOnTrial(LenStat& stat);

std::vector<std::pair<uint32_t, uint32_t>> GetMedOnTrial(LenStat& stat);

#endif