#ifndef MOUSE_H
#define MOUSE_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <iostream>

class Session {
public:
    class Iterator {
    public:
        Iterator() = default;
        Iterator(std::vector<std::string>::iterator it);
        std::string& operator*();
        bool operator==(const Iterator& other) const;
        Iterator& operator++();
    private:
        std::vector<std::string>::iterator it_;
    };
    Iterator End();
    Iterator Begin();
    void AddTrial(const std::string& trial);
private:
    std::vector<std::string> trials_;
};

class Mouse {
public:
    class Iterator {
    public:
        Iterator(std::vector<std::shared_ptr<Session>>::iterator it);
        std::string& operator*();
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        Iterator& operator++(); // side function, better use NextIt()
        Session::Iterator GetSession();
        std::vector<std::shared_ptr<Session>>::iterator Get();
        void SetSession(Session::Iterator it);
    private:
        std::vector<std::shared_ptr<Session>>::iterator it_;
        Session::Iterator session_it_;    
    };
    Iterator NextIt(Iterator cur);
    Iterator Begin();
    Iterator End();
    Mouse(const std::string& name);
    void AddSession();
    void AddTrialToLastSession(const std::string& s);
private:
    std::string name_;
    std::vector<std::shared_ptr<Session>> sessions_;
};

class Seria {
public:
    class Iterator {
    public:
        Iterator(std::map<std::string, std::shared_ptr<Mouse>>::iterator it, Mouse::Iterator mouse_it);
        std::string& operator*();
        bool operator!=(const Seria::Iterator& other) const; 
        std::map<std::string, std::shared_ptr<Mouse>>::iterator Get();
        Mouse::Iterator GetMouse();
    private:
        std::map<std::string, std::shared_ptr<Mouse>>::iterator it_;
        Mouse::Iterator mouse_it_;   
    };
    Iterator NextIt(Seria::Iterator cur);
    Iterator Begin();
    Iterator End();
    Seria(const std::string& name);
    void AddMouse(const std::string& name);
    void AddData(const std::string& mouse, uint32_t s_num, uint32_t t_num, const std::string& trial, bool add);
private:
    std::string name_;
    std::map<std::string, std::shared_ptr<Mouse>> mice_;
};

void GetLines(Seria& f, Seria& h);

#endif