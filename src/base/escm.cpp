#include "global.h"
#include "drivers/display.h"
#include "gk/renderer/renderer.h"
#include "input/actions.h"


//NOTE: This is the main subsytem glue module for now file, until
// we figure out how we want to structure the engine.

juye:: DisplayDriver* win = nullptr;

void* query_main_display(){
  return win->handle();
}


namespace{
using namespace juye;

DisplayDriver* create_window(){
  #if _WIN32
  #endif
  #if __APPLE__
    DisplayDriver* ret = new DisplayOSX();
    ret->init();
    return ret;
  #endif
}

void main_loop(){
  while(win->is_running()){
    win->update();
    render_fe_tick();

  };
}

}//namespace anon

using namespace juye;

int engine_entry(){
  win = create_window();
  render_fe_begin();
  main_loop();

  render_fe_end();

  printf("Engine exited successfully!\n");
  return 0;
}
