#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H


#ifdef __linux__
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <linux/videodev2.h>
    #include <dirent.h>
#elif _WIN32
    #include <Windows.h>
    #include <dshow.h>
    #pragma comment(lib, "ole32.lib")
    #pragma comment(lib, "oleaut32.lib")
    #pragma comment(lib, "strmiids.lib")
#endif


#include <map>
#include <string>


#define CAM_INDEX_ERROR     -1
#define MAX_CAM_NAME_LENGTH 256


class DeviceManager
{
public:
    DeviceManager();
    // int - openCV cam index; std::string - cam name
    static std::map<int, std::string> GetWebcamList();

private:
#ifdef _WIN32
    static HRESULT EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum);
    static void DisplayDeviceInformation(IEnumMoniker *pEnum, std::map<int, std::string> *deviceList);
#endif
};

#endif // DEVICEMANAGER_H
