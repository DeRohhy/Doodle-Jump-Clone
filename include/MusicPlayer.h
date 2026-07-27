#pragma once

#include "singletons/ResourceManager.h"
#include <string>
#include <SFML/Audio.hpp>

class MusicPlayer {
public:
    MusicPlayer() : music(ResourceManager<sf::Music>::getInstance().get(MUSIC_PATH)) {
        music.setLooping(true);
    }

    void play() { music.play(); }
    void stop() { music.stop(); }
    void pause() { music.pause(); }
    bool isPlaying() { return music.getStatus() == sf::SoundSource::Status::Playing; }

private:
    const std::string MUSIC_PATH = "sounds/main_menu_song.flac";
    sf::Music& music;
};