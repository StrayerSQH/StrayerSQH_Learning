#include "MusicBGM.h"

MusicBGM::MusicBGM(const std::string& file) : musicBGMFile(file) {}

bool MusicBGM::loadMusic() {
    if (music.openFromFile(musicBGMFile)) {
        music.setLoop(true);  // …Ë÷√—≠ª∑≤•∑≈
        return true;
    }
    return false;
}

void MusicBGM::play() {
    music.play();
}

void MusicBGM::stop() {
    music.stop();
}

void MusicBGM::setLoop(bool loop) {
    music.setLoop(loop);
}