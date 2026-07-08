#pragma once

#include "singletons/Singleton.h"
#include <fstream>
#include <string>

class GameSettings : public Singleton<GameSettings> {
friend class Singleton<GameSettings>;

public:
    int getHighScore() const { return high_score; }
    void setHighScore(int new_score) {
        if (new_score > high_score) {
            high_score = new_score;
        }
        save();
    }

    int getLastScore() const { return last_score; }
    void setLastScore(int score) {
        last_score = score; 
        save();
    }

private:
    const std::string SETTINGS_PATH = "settings.txt";
    
    int high_score = 0;
    int last_score = 0;

    GameSettings() { load(); }

    void load() {
        std::ifstream in(SETTINGS_PATH);
        if (!in) {
            setDefaults();
            return;
        }
        
        in >> high_score >> last_score;
        
        if (in.fail()) {
            setDefaults();
        }
    }

    void save() const {
        std::ofstream out(SETTINGS_PATH, std::ios::trunc);
        out << high_score << '\n'
            << last_score;
    }

    void setDefaults() {
        high_score = 0;
    }

};