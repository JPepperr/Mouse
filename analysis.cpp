#include "analysis.h"

CompressionsData::Stat::Stat(const std::string& was, const std::string& became, uint32_t len) : was_(was), became_(became), best_len_(len) {}

CompressionsData::CompressionsData(Seria& s) {
    std::shared_ptr<Graph> g = std::make_shared<Graph>();
    g->BuildGraph(s);
    d_ = DistMatrix(*g);
    for (const auto& m : s.GetMice()) {
        const auto& curname = m.first;
        for (const auto& session : m.second->GetSessions()) {
            const auto& trials = session->GetTrials();
            for (size_t i = 1; i < trials.size(); i++) {
                PathAnalysis(trials[i - 1], trials[i], data_[curname]);
            }
        }
    }
}

void CompressionsData::PathAnalysis(const std::string& from, const std::string& to, std::vector<Stat>& stat) {
    bool visited[2] = {false, false};
    size_t pos[2] = {from.size(), from.size()};
    size_t id = 0;
    for (size_t i = 0; i < from.size(); i++) {
        for (size_t num = 0; num < CNT_FEEDERS; num++) {
            if (from[i] == FEEDERS[num] && !visited[num]) {
                visited[num] = true;
                pos[id++] = i;
            }
        }
    }    
    for (size_t i = 0; i < pos[0]; i++) {
        for (size_t j = i + MIN_COMPRESSION_SIZE - 1; j < std::min(pos[0], MAX_COMPRESSION_SIZE + i - 1); j++) {
            TryFindCompression(i, j, from, to, stat);
        }
    }
    for (size_t i = pos[0]; i < pos[1]; i++) {
        for (size_t j = i + MIN_COMPRESSION_SIZE - 1; j < std::min(pos[1], MAX_COMPRESSION_SIZE + i - 1); j++) {
            TryFindCompression(i, j, from, to, stat);
        }
    }
    for (size_t i = pos[1]; i < from.size(); i++) {
        for (size_t j = i + MIN_COMPRESSION_SIZE - 1; j < std::min(from.size(), MAX_COMPRESSION_SIZE + i - 1); j++) {
            TryFindCompression(i, j, from, to, stat);
        }
    }
}

void CompressionsData::TryFindCompression(size_t fr, size_t to, const std::string& b, const std::string& s, std::vector<Stat>& stat) {
    size_t len = to - fr + 1;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == b[fr]) {
            for (size_t j = i; j < std::min(s.size(), i + len - 1); j++) {
                if (s[j] == b[to]) {
                    stat.emplace_back(Stat{ b.substr(fr, len), s.substr(i, j - i + 1), d_.GetDist(std::string(1, s[i]), std::string(1, s[j]))});
                    i = j;
                    break;
                }   
            }
        }
    }
}

void CompressionsData::Stat::Print() const {
    std::cout << was_ << " | " << became_ << " | " << best_len_ << '\n';
}

void CompressionsData::Print() const {
    for (const auto& p : data_) {
        std::cout << p.first << '\n';
        for (const auto& d : p.second) {
            d.Print();
        }
    }
}

LenStat::Path::Path() : cnt_(0) {
    std::fill(len_, len_ + CNT_OF_PARTS, 0);
}

LenStat::Path::Path(const std::string& p) {
    std::fill(len_, len_ + CNT_OF_PARTS, 0);
    cnt_ = 0;
    bool visited[2] = {false, false};
    size_t last = 0;
    size_t id = 0;
    for (size_t i = 0; i < p.size(); i++) {
        for (size_t num = 0; num < CNT_FEEDERS; num++) {
            if (p[i] == FEEDERS[num] && !visited[num]) {
                visited[num] = true;
                len_[id++] = i - last + 1;
                last = i;
            }
        }
    }
    if (id == CNT_OF_PARTS - 1) len_[id] = p.size() - last;
    cnt_ = id;
}

void LenStat::AddMouse(std::shared_ptr<Mouse> m) {
    const auto& curname = m->GetName();
    for (const auto& s : m->GetSessions()) {
        auto trs = s->GetTrials();
        if (trs.size() > trials_data_.size()) {
            trials_data_.resize(trs.size());
        }
        for (size_t t = 0; t < trs.size(); t++) {
            auto curp = Path(trs[t]);
            trials_data_[t].push_back(curp);
            data_[curname].emplace_back(curp);
        }
    }
}

LenStat::LenStat(Seria& s) {
    for (auto& m : s.GetMice()) {
        AddMouse(m.second);
    }
}

void BuildPlotData(Seria& f, Seria& h) {
    LenStat f_stat(f);
    LenStat h_stat(h);
    std::ofstream file;
    file.open("plot_data.txt");
    for (size_t part = 0; part < CNT_OF_PARTS; part++) {
        file << "Average_path_length_on_part_" << part + 1 << ' ';
        file << "Trial Length ";
        file << f.GetName() << ' ' << h.GetName() << ' ';
        auto f_coord = GetAvgOnPart(f_stat, part);
        auto h_coord = GetAvgOnPart(h_stat, part);
        file << f_coord.size() << ' ' << h_coord.size() << '\n';
        for (const auto& pr : f_coord) {
            file << pr.first << ' ' << pr.second << '\n';
        }
        for (const auto& pr : h_coord) {
            file << pr.first << ' ' << pr.second << '\n';
        }
    }
    for (size_t part = 0; part < CNT_OF_PARTS; part++) {
        file << "Median_path_length_on_part_" << part + 1 << ' ';
        file << "Trial Length ";
        file << f.GetName() << ' ' << h.GetName() << ' ';
        auto f_coord = GetMedOnPart(f_stat, part);
        auto h_coord = GetMedOnPart(h_stat, part);
        file << f_coord.size() << ' ' << h_coord.size() << '\n';
        for (const auto& pr : f_coord) {
            file << pr.first << ' ' << pr.second << '\n';
        }
        for (const auto& pr : h_coord) {
            file << pr.first << ' ' << pr.second << '\n';
        }
    }
    {
        file << "Average_path_length ";
        file << "Trial Length ";
        file << f.GetName() << ' ' << h.GetName() << ' ';
        auto f_coord = GetAvgOnTrial(f_stat);
        auto h_coord = GetAvgOnTrial(h_stat);
        file << f_coord.size() << ' ' << h_coord.size() << '\n';
        for (const auto& pr : f_coord) {
            file << pr.first << ' ' << pr.second << '\n';
        }
        for (const auto& pr : h_coord) {
            file << pr.first << ' ' << pr.second << '\n';
        }
    }
    {
        file << "Median_path_length ";
        file << "Trial Length ";
        file << f.GetName() << ' ' << h.GetName() << ' ';
        auto f_coord = GetMedOnTrial(f_stat);
        auto h_coord = GetMedOnTrial(h_stat);
        file << f_coord.size() << ' ' << h_coord.size() << '\n';
        for (const auto& pr : f_coord) {
            file << pr.first << ' ' << pr.second << '\n';
        }
        for (const auto& pr : h_coord) {
            file << pr.first << ' ' << pr.second << '\n';
        }
        file.close();
    }
}

const std::vector<std::vector<LenStat::Path>>& LenStat::GetTrialsData() const {
    return trials_data_;
}

std::vector<std::pair<uint32_t, double>> GetAvgOnPart(LenStat& stat, size_t part) {
    const auto& data = stat.GetTrialsData();
    std::vector<std::pair<uint32_t, double>> res(data.size());
    for (size_t i = 0; i < res.size(); i++) {
        res[i].first = i + 1;
        uint32_t sum = 0, cnt = 0;
        for (size_t j = 0; j < data[i].size(); j++) {
            if (data[i][j].cnt_ > part || (data[i][j].cnt_ == part && part == CNT_OF_PARTS - 1)) {
                sum += data[i][j].len_[part];
                cnt += 1;
            }
        }
        res[i].second = double(sum) / double(cnt);
    }
    return res;
}

std::vector<std::pair<uint32_t, uint32_t>> GetMedOnPart(LenStat& stat, size_t part) {
    const auto& data = stat.GetTrialsData();
    std::vector<std::pair<uint32_t, uint32_t>> res(data.size());
    for (size_t i = 0; i < res.size(); i++) {
        std::vector<LenStat::Path> curd;
        for (size_t j = 0; j < data[i].size(); j++) {
            if (data[i][j].cnt_ > part || (data[i][j].cnt_ == part && part == CNT_OF_PARTS - 1)) {
                curd.push_back(data[i][j]);
            }
        }
        sort(curd.begin(), curd.end(), SorterByPart(part));
        res[i].first = i + 1;
        if (curd.size() == 0) {
            res[i].second = 0;
        } else {
            res[i].second = curd[curd.size() / 2].len_[part];
        }
    }
    return res;
}

bool SorterByPart::operator()(LenStat::Path a, LenStat::Path b) {
    return a.len_[part_] < b.len_[part_];
}

const std::map<std::string, std::vector<LenStat::Path>>& LenStat::GetData() const {
    return data_;
}

uint32_t GetCntFeedersFound(const LenStat& s, uint32_t cnt) {
    const auto& data = s.GetData();
    uint32_t res = 0;
    for (const auto& d : data) {
        for (const auto& t : d.second) {
            if (t.cnt_ == cnt) {
                res++;
            }
        }
    }
    return res;
}

void PrintTableFeedersFound(Seria& s) {
    LenStat s_stat(s);
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << "Feeders found\t|\t0\t|\t1\t|\t2\t|\n";
    std::cout << "-----------------------------------------------------------------\n";
    auto r0 = GetCntFeedersFound(s, 0);
    auto r1 = GetCntFeedersFound(s, 1);
    auto r2 = GetCntFeedersFound(s, 2);
    auto sm = r0 + r1 + r2;
    std::cout << s.GetName() << "\t\t" << "|\t" << r0 << "\t|\t" << r1 << "\t|\t" << r2 << "\t|\n";
    std::cout << "-----------------------------------------------------------------\n";
    uint32_t s0 = double(r0) / double(sm) * double(100);
    uint32_t s1 = double(r1) / double(sm) * double(100);
    uint32_t s2 = double(r2) / double(sm) * double(100);
    std::cout << s.GetName() << ",%" << "\t\t" << "|\t" << s0 << "\t|\t" << s1 << "\t|\t" << s2 << "\t|\n";
    std::cout << "-----------------------------------------------------------------\n";
}

std::vector<std::pair<uint32_t, double>> GetAvgOnTrial(LenStat& stat) {
    const auto& data = stat.GetTrialsData();
    std::vector<std::pair<uint32_t, double>> res(data.size());
    for (size_t i = 0; i < res.size(); i++) {
        res[i].first = i + 1;
        uint32_t sum = 0, cnt = 0;
        for (size_t j = 0; j < data[i].size(); j++) {
            for (size_t part = 0; part < CNT_OF_PARTS; part++) {
                sum += data[i][j].len_[part];
                if (data[i][j].len_[part] > 0 && part > 0) sum--;
            }
            cnt += 1;
        }
        res[i].second = double(sum) / double(cnt);
    }
    return res;
}

std::vector<std::pair<uint32_t, uint32_t>> GetMedOnTrial(LenStat& stat) {
    const auto& data = stat.GetTrialsData();
    std::vector<std::pair<uint32_t, uint32_t>> res(data.size());
    for (size_t i = 0; i < res.size(); i++) {
        std::vector<uint32_t> curd;
        for (size_t j = 0; j < data[i].size(); j++) {
            uint32_t sum = 0;
            for (size_t part = 0; part < CNT_OF_PARTS; part++) {
                sum += data[i][j].len_[part];
                if (data[i][j].len_[part] > 0 && part > 0) sum--;
            }
            curd.push_back(sum);
        }
        sort(curd.begin(), curd.end());
        res[i].first = i + 1;
        if (curd.size() == 0) {
            res[i].second = 0;
        } else {
            res[i].second = curd[curd.size() / 2];
        }
    }
    return res;
}