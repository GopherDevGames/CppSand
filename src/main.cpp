#include <iostream>
#include "SandRenderer.h"
#include "Globals.h"

int main(){
    double framerate;
    std::cout<<"Enter simulation Framerate (default:60): "<<std::endl;
    if (std::cin.peek() == '\n' || !(std::cin>>framerate)){
        framerate = 60.0;
    }

    SandRenderer sand_renderer(framerate);

    bool is_running = true;
    while(is_running){
        is_running = sand_renderer.tick();
        sand_renderer.render();
    }
}