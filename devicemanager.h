#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#ifdef __linux__
    #include <stdio.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/ioctl.h>
    #include <linux/videodev2.h>
    #include <string.h>
    #include <dirent.h>
#elif _WIN32
    #include <Windows.h>
    #include <dshow.h>
    #pragma comment(lib, "ole32.lib")
    #pragma comment(lib, "oleaut32.lib")
    #pragma comment(lib, "strmiids.lib")
#endif


#include <vector>
#include <string>

class DeviceManager
{
public:
    DeviceManager();
    static std::vector<std::string> GetWebcamList();

private:
#ifdef _WIN32
    static HRESULT EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum);
    static void DisplayDeviceInformation(IEnumMoniker *pEnum, std::vector<std::string> *deviceList);
#endif
};

#endif // DEVICEMANAGER_H
