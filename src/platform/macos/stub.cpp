#include "platform/stub.h"
#include "drivers/display.h"

namespace juye{

int plt_create_window(DisplayDriver* driver){
  driver = new DisplayOSX;
  return driver->init();
}

};
