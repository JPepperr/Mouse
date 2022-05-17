#include "analysis.h"

Result::Stat::Stat(std::shared_ptr<Mouse> mouse, std::string path, bool best): mouse_(mouse), path_(std::move(path)), best_(best) {}

std::optional<std::string> TryToFindPath(const std::string& s, const std::string& b, size_t pos) {
    size_t ans = 200;
    std::pair<size_t, size_t> ans_pos;
    for (size_t i = 1; i < s.size(); i++) {
        if (s[i] == b[pos - 1] && s[i - 1] == b[pos - 2]) {
            for (size_t j = i + 1; j < s.size() - 1; j++) {
                if (s[j] == b[pos] && s[j + 1] == b[pos + 1]) {
                    size_t del = abs((int32_t)pos - (int32_t)j);
                    if (del < ans && j - i < 10 && j > i + 2) {
                        ans = del;
                        ans_pos = {i, j};
                    }
                    i = j;
                    break;
                }
            }
        }
    }
    if (ans == 200) return std::nullopt;
    return s.substr(ans_pos.first, ans_pos.second - ans_pos.first + 1);
}

void Result::AddMouseAnalysis(std::shared_ptr<Mouse> m) {
    for (const auto& s : m->GetSessions()) {
        const auto& t = s->GetTrials();
        for (size_t i = 1; i < t.size(); i++) {
            for (size_t j = 2; j < t[i].size() - 1; j++) {
                auto opt = TryToFindPath(t[i - 1], t[i], j);
                if (opt.has_value()) {
                    res_.push_back({m, opt.value(), false});
                }
            }
        }
    }
}

Result GetCompressions(Seria& s) {
    Result res;
    for (const auto& m : s.GetMice()) {
        res.AddMouseAnalysis(m.second);
    }
    return res;
}

LenStat::Path::Path(const std::string& p) {
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
    if (id == CNT_OF_PARTS - 1) len_[id++] = p.size() - last;
    cnt_ = id;
    //std::cout << cnt_ << ' ' << len_[0] << ' ' << len_[1] << ' ' << len_[2] << '\n';
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
    file.close();
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
            if (data[i][j].cnt_ > part) {
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
            if (data[i][j].cnt_ > part) {
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