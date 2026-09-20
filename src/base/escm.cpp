#include "global.h"
#include "platform/cmdline.h"
#include "client.h"
#include "platform/display.h"
#include "graphick/renderer/renderer.h"
#include "input/actions.h"


// NOTE: This is the main subsytem glue module for now file, until
// we figure out how we want to structure the engine.

using namespace juye;

DisplayDriver* win = nullptr;

void* query_main_display(){
  return win->handle();
}



DisplayDriver* create_window(){
  #if _WIN32
    DisplayDriver* ret = new DisplayWin32();
    ret->init();
    return ret;
  #endif

  #if __APPLE__
    DisplayDriver* ret = new DisplayOSX();
    ret->init();
    return ret;
  #endif
}

static void main_loop(){
  
  while(win->is_running()){
    client_update();
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
  client_shutdown();
  printf("Engine exited successfully!\n");
  return 0;
}
