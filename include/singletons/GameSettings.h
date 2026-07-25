#pragma once

#include "singletons/Singleton.h"
#include <fstream>
#include <string>

enum class Difficulty {
    EASY = 0,
    MEDIUM = 1,
    HARD = 2
};


class GameSettings : public Singleton<GameSettings> {
friend class Singleton<GameSettings>;

public:
    int getHighScore() const { return high_scores[static_cast<int>(difficulty)]; }
    void setHighScore(int new_score) {
        int diff_index = static_cast<int>(difficulty);
        if (new_score > high_scores[diff_index]) {
            high_scores[diff_index] = new_score;
        }
        save();
    }

    int getLastScore() const { return last_score; }
    void setLastScore(int score) {
        last_score = score; 
        save();
    }

    float getVolume() const { return volume; }
    void setVolume(float new_volume) { volume = new_volume; }

    Difficulty getDifficulty() const { return difficulty; }
    void setDifficulty(Difficulty new_difficulty) { difficulty = new_difficulty; }

    void save() const {
        std::ofstream out(SETTINGS_PATH, std::ios::trunc);
        out << high_scores[0] << '\n'
            << high_scores[1] << '\n'
            << high_scores[2] << '\n'
            << last_score << '\n'
            << volume << '\n'
            << static_cast<int>(difficulty);
    }
private:
    const std::string SETTINGS_PATH = "settings.txt";
    
    int high_scores[3] = {0, 0, 0};
    int last_score = 0;
    float volume = 1.f;
    Difficulty difficulty = Difficulty::EASY;

    GameSettings() { load(); }

    void load() {
        std::ifstream in(SETTINGS_PATH);
        if (!in) {
            setDefaults();
            return;
        }
        
        int diff_index;
        in >> high_scores[0] >> high_scores[1] >> high_scores[2]
           >> last_score >> volume >> diff_index;

        difficulty = static_cast<Difficulty>(diff_index);
        
        if (in.fail()) {
            setDefaults();
        }
    }



    void setDefaults() {
        high_scores[0] = 0;
        high_scores[1] = 0;
        high_scores[2] = 0;
        last_score = 0;
        volume = 1.f;
        difficulty = Difficulty::MEDIUM;

    }

};