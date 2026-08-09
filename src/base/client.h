#pragma once

namespace juye{

void client_load_dyn(const char* dylib);
void client_init();
void client_tick();
void client_update();
void client_shutdown();

}
