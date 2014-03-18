#include "devicemanager.h"
#include "QDebug"

DeviceManager::DeviceManager()
{
}

std::vector<std::string> DeviceManager::GetWebcamList()
{
    std::vector<std::string> deviceList;
#ifdef __linux__
    int fd;
    struct v4l2_capability video_cap;

    DIR *d;
    struct dirent *dir;
    d = opendir("/dev/");

    // need to the normal code!!!!!!!!!!!!
    if (d)
    {
        std::vector<std::string> sysDeviceNames;
        //returns unsorted devices list. So we need to sort it. (video1, video0 ..etc)
        while( (dir = readdir(d) ) != NULL )
        {
            // tamplate video0, video1 ... etc.
            if ( strlen(dir->d_name) < 6)
            {
                continue;
            }
            // 6 - length of "video" + "\0"
            char buf[6];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, dir->d_name, 5);
            if (strcmp(buf, "video") == 0)
            {
                sysDeviceNames.push_back(dir->d_name);
            }
        }

        //sort the system devices vector
        std::sort(sysDeviceNames.begin(), sysDeviceNames.end());

        //then getdevices human readable Names
        std::vector<std::string>::iterator sysDevicesIterator = sysDeviceNames.begin();
        while( sysDevicesIterator != sysDeviceNames.end() )
        {
            // 256 - its max string size.
            char buffer[256];

            memset(buffer, 0, sizeof(buffer));
            sprintf(buffer, "%s%s", "/dev/", (*sysDevicesIterator).c_str());
            if((fd = open(buffer, O_RDONLY)) == -1)
            {
                break;
            }

            if(ioctl(fd, VIDIOC_QUERYCAP, &video_cap) == -1)
            {
                deviceList.push_back("Unknown camera");
            }
            else
            {
                //i need to convert from addres to string
                char bufStr[256];
                memset(bufStr, 0, sizeof(bufStr));
                sprintf(bufStr, "%s", video_cap.card);
                deviceList.push_back(bufStr);
            }

            ++sysDevicesIterator;
        }
    }
#elif _WIN32
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if ( hr == RPC_E_CHANGED_MODE )
    {
        hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    }

    if ( SUCCEEDED(hr) )
    {
        IEnumMoniker *pEnum;

        hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
        if ( SUCCEEDED(hr) )
        {
            DisplayDeviceInformation(pEnum, &deviceList);
            pEnum->Release();
        }
        CoUninitialize();
    }
#endif

    return deviceList;
}



#ifdef _WIN32
    HRESULT DeviceManager::EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
    {
        // Create the System Device Enumerator.
        ICreateDevEnum *pDevEnum;
        HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
            CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));

        if (SUCCEEDED(hr))
        {
            // Create an enumerator for the category.
            hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
            if (hr == S_FALSE)
            {
                hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
            }
            pDevEnum->Release();
        }
        return hr;
    }

    void DeviceManager::DisplayDeviceInformation(IEnumMoniker *pEnum, std::vector<std::string> *deviceList)
    {
        IMoniker *pMoniker = NULL;

        while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
        {
            IPropertyBag *pPropBag;
            HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
            if (FAILED(hr))
            {
                pMoniker->Release();
                continue;
            }

            VARIANT var;
            VariantInit(&var);

            // Get description or friendly name.
            hr = pPropBag->Read(L"Description", &var, 0);
            if (FAILED(hr))
            {
                hr = pPropBag->Read(L"FriendlyName", &var, 0);
            }
            if (SUCCEEDED(hr))
            {
                // 256 - max name length
                char buf[256];
                memset(buf, 0, sizeof(buf));
                sprintf_s(buf, "%S", var.bstrVal);
                deviceList->push_back(buf);
                VariantClear(&var);
            }

            pPropBag->Release();
            pMoniker->Release();
        }
    }
#endif
