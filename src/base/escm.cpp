#include "global.h"
#include "cmdline.h"
#include "client.h"
#include "drivers/display.h"
#include "graphick/renderer/renderer.h"
#include "input/actions.h"


//NOTE: This is the main subsytem glue module for now file, until
// we figure out how we want to structure the engine.

juye:: DisplayDriver* win = nullptr;

void* query_main_display(){
  return win->handle();
}


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

static void main_loop(){
  
  while(win->is_running()){
    win->update();
    render_fe_tick();

  };
}


int engine_entry(cmdline_config config){
  client_load_dyn(config.client_dll.c_str());
  client_init();
  
  win = create_window();
  render_fe_begin();
  main_loop();
  render_fe_end();
  printf("Engine exited successfully!\n");
  return 0;
}
