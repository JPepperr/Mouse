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
    len_[id] = p.size() - last;
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