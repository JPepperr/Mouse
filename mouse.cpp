#include "mouse.h"

Session::Iterator::Iterator(std::vector<std::string>::iterator it):it_(it) {}

Session::Iterator Session::End() {
    return trials_.end();
}

Session::Iterator Session::Begin() {
    return trials_.begin();
}

std::string& Session::Iterator::operator*() {
    return *it_;
}

bool Session::Iterator::operator==(const Iterator& other) const {
    return other.it_ == it_;
}

Session::Iterator& Session::Iterator::operator++() { 
    it_++;
    return *this;
}

void Session::AddTrial(const std::string& trial) {
    trials_.emplace_back(trial);
}

Session::Iterator Mouse::Iterator::GetSession () {
    return session_it_;
}

std::vector<std::shared_ptr<Session>>::iterator Mouse::Iterator::Get () {
    return it_;
}

Mouse::Iterator::Iterator(std::vector<std::shared_ptr<Session>>::iterator it):it_(it) {}

std::string& Mouse::Iterator::operator*() {
    return *session_it_;
}

bool Mouse::Iterator::operator==(const Mouse::Iterator& other) const {
    return it_ == other.it_ && other.session_it_ == session_it_;
}

bool Mouse::Iterator::operator!=(const Mouse::Iterator& other) const {
    return !(*this == other);
}

Mouse::Iterator& Mouse::Iterator::operator++() {
    ++it_;
    return *this;
}

void Mouse::Iterator::SetSession(Session::Iterator it) {
    session_it_ = it;
}

Mouse::Iterator Mouse::NextIt(Mouse::Iterator cur) {
    auto s = cur.GetSession();
    ++s;
    if (s == (*cur.Get())->End()) {
        ++cur;
        if (sessions_.end() != cur.Get()) {
            cur.SetSession((*cur.Get())->Begin());
        } else {
            cur.SetSession(s);
        }
    } else {
        cur.SetSession(s);
    }
    return cur;
}

Mouse::Iterator Mouse::Begin() {
    Mouse::Iterator cur(sessions_.begin());
    cur.SetSession((*cur.Get())->Begin());
    return cur;
}

Mouse::Iterator Mouse::End() {
    auto it = sessions_.end();
    it--;
    Mouse::Iterator cur(sessions_.end());
    cur.SetSession((*it)->End());
    return cur;
}

Mouse::Mouse(const std::string& name) : name_(name) {}

void Mouse::AddSession() {
    sessions_.emplace_back(std::make_shared<Session>());
}

void Mouse::AddTrialToLastSession(const std::string& s) {
    sessions_.back()->AddTrial(s);
}

Seria::Seria(const std::string& name): name_(name) {}

void Seria::AddMouse(const std::string& name) {
    mice_[name] = std::make_shared<Mouse>(name);
}

void Seria::AddData(const std::string& mouse, uint32_t s_num, uint32_t t_num, const std::string& trial, bool add) {
    if (mice_.find(mouse) == mice_.end()) {
        AddMouse(mouse);
    }
    if (add) {
        mice_[mouse]->AddSession();
    }
    mice_[mouse]->AddTrialToLastSession(trial);
}

void GetLines(Seria& f, Seria& h) {
    std::string name, mouse, trial, prev_name = "";
    uint32_t prev_s = 0; 
    bool add = false;
    uint32_t s_num, t_num; 
    while (std::cin >> name >> mouse >> s_num >> t_num >> trial) {
        add = (prev_name != mouse || prev_s != s_num) ? true : false;
        if (name == "F1-24") { f.AddData(mouse, s_num, t_num, trial, add); } 
        else { h.AddData(mouse, s_num, t_num, trial, add); }
        prev_s = s_num;
        prev_name = mouse;
    }
}

Seria::Iterator::Iterator(std::map<std::string, std::shared_ptr<Mouse>>::iterator it, Mouse::Iterator mouse_it): it_(it), mouse_it_(mouse_it) {}

bool Seria::Iterator::operator!=(const Seria::Iterator& other) const {
    return (it_ != other.it_) || (mouse_it_ != other.mouse_it_);
}

Seria::Iterator Seria::Begin() {
    return Seria::Iterator(mice_.begin(), mice_.begin()->second->Begin());
}

Seria::Iterator Seria::End() {
    auto it = mice_.end();
    it--;
    return Seria::Iterator(mice_.end(), it->second->End());
}

Seria::Iterator Seria::NextIt(Seria::Iterator cur) {
    auto it = cur.GetMouse();
    it = cur.Get()->second->NextIt(it);
    auto it_map = cur.Get();
    if (it == it_map->second->End()) {
        it_map++;
        if (mice_.end() != it_map) {
            it = it_map->second->Begin();
        }
    }
    return {it_map, it};
}

std::string& Seria::Iterator::operator*() {
    return *mouse_it_;
}

Mouse::Iterator Seria::Iterator::GetMouse() {
    return mouse_it_;
}

std::map<std::string, std::shared_ptr<Mouse>>::iterator Seria::Iterator::Get() {
    return it_;
}

const std::map<std::string, std::shared_ptr<Mouse>>& Seria::GetMice() {
    return mice_;
}

const std::vector<std::shared_ptr<Session>>& Mouse::GetSessions() {
    return sessions_;
}

const std::vector<std::string>& Session::GetTrials() {
    return trials_;
}