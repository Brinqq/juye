#include <stdio.h>


extern "C"{

__attribute__((visibility("default"))) void client_main(){
  printf("Hello, from client main\n");
}

}
void client_update(){
  printf("Hello, from client update");
}

void client_shutdown(){
  printf("Hello, from client shutdown");
}

