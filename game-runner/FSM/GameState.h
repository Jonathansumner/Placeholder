#pragma once
#include "led-matrix.h"
#include "../graphics/gfx.h"

class MarpleTiltMachine;
class Marple;
class GameState {
    public:
        GameState(rgb_matrix::Canvas *c, Marple *m) {canvas = c; marple=m;};
        virtual ~GameState() {};

        virtual void OnEntry() {};
        void OnExit() {Object::clearStage(canvas); std::cout << "Clearing!\n";};
        virtual void Update() {};
        virtual std::string Name() {return "";};

        void setMarple(Marple* m) {marple=m;};
        Marple* getMarple() {return marple;};

        rgb_matrix::Canvas *canvas;
        static MarpleTiltMachine runner;
        Marple *marple;
};  