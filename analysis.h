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
#include "graph.h"

static const int CNT_FEEDERS = 2;
static const char FEEDERS[CNT_FEEDERS] = {'P', 'D'};
static const size_t CNT_OF_PARTS = 3;
static const size_t MIN_COMPRESSION_SIZE = 4;
static const size_t MAX_COMPRESSION_SIZE = 14;

class CompressionsData {
public:
    class Stat {
    public:
        Stat(const std::string& was, const std::string& became, uint32_t len);
        void Print() const;
        uint32_t GetDelta() const;
    private:
        std::string was_, became_;
        uint32_t best_len_;
    };
    CompressionsData(Seria& s);
    void Print() const;
    std::vector<std::pair<uint32_t, double>> GetCompressionCoord() const;
private:
    bool TryFindCompression(size_t fr, size_t to, const std::string& b, const std::string& s, std::vector<Stat>& stat);
    void PathAnalysis(const std::string& from, const std::string& to, std::vector<Stat>& stat);
    std::map<std::string, std::vector<Stat>> data_;
    DistMatrix d_;
};

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