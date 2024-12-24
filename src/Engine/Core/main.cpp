#pragma once

#include <iostream>
//#include <stdexcept>
//#include <functional>
//#include <vector>
#include <Engine.h>
#include "log.hpp"
#include "Components/GeometryComponent.h"

const int WIDTH = 500;
const int HEIGHT = 500;

using namespace TourBillon;
int main() {
    
    LOG_DEBUG("main start");

    EngineInitInfo engine_init_info;
    TBEngine engine;
    engine_init_info.window_width = WIDTH;
    engine_init_info.window_height = HEIGHT;
    engine_init_info.window_num = 1;

	//test();

    engine.initialize(engine_init_info);
    engine.run();

    LOG_DEBUG("main finish");


    //RenderSystemInitInfo render_system_info;
    //RenderSystem rendersystem;
    //rendersystem.initialize(render_system_info);
    //rendersystem.rendLoop();
    //rendersystem.clear();

    return EXIT_SUCCESS;
}