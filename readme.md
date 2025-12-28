# **Build** 

## **Prerequisites**
 Building this project requires you have:
- Python 3.12 *(or greater)*
- CMake 4.25.1 *(or greater)*
- Build Generation Tool *(Windows)*
- VulkanSDK

Download the vulkan sdk at https://vulkan.lunarg.com/sdk/home

*Be sure to set the vulkan sdk in your enviroment* $ENV:VULKAN_SDK

Start by cloning the repository:
```
$ git clone https://github.com/Brinqq/juye.git
$ cd juye
$ git submodule update --init
```

## **Windows**

Run the setup script:
```
$ scripts/setup.bat
```
Run Cmake with your desired Build Generation, Here we use MinGW:
```
$ cmake . -G "MinGW Makefiles" -B build <options>
$ Mingw32-make.exe -C build
```
## **MacOS**
```
$ scripts/setup.sh
$ cmake . -B build <options>
$ make -C build
```
## **Linux**
### **Linux support is still in progress.**
