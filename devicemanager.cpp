#include "devicemanager.h"
#include "QDebug"

DeviceManager::DeviceManager()
{
}

std::map<int, std::string> DeviceManager::GetWebcamList()
{
    std::map<int, std::string> deviceList;
#ifdef __linux__
    struct v4l2_capability video_cap;

    int fd;
    DIR *d;
    struct dirent *dir;
    d = opendir("/dev/");

    if (d)
    {
        //returns unsorted devices list(video1, video0 ..etc)
        while( (dir = readdir(d) ) != NULL )
        {
            // tamplate video0, video1 ... etc.
            // strlen("video0") = 6
            if ( strlen(dir->d_name) < 6)
            {
                continue;
            }
            // strlen("video") + "\0" = 6
            char buf[6];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, dir->d_name, 5);
            if (strcmp(buf, "video") == 0)
            {
                char camDevPath[MAX_CAM_NAME_LENGTH];
                memset(camDevPath, 0, sizeof(camDevPath));
                sprintf(camDevPath, "%s%s", "/dev/", dir->d_name);

                if((fd = open(camDevPath, O_RDONLY)) == -1)
                {
                    break;
                }

                //parse the index of the cam
                char camIndexChar[4]; // max value 999 + \0
                char * end;
                memset(camIndexChar, 0, sizeof(camIndexChar));
                strcpy(camIndexChar, dir->d_name+5);

                int camIndexInt = strtol(camIndexChar, &end, 10);
                //if no characters were converted to the int (incorrect name for example: videoAB)
                if ( end == camIndexChar )
                {
                    camIndexInt = -1;
                }

                if(ioctl(fd, VIDIOC_QUERYCAP, &video_cap) == -1)
                {
                    deviceList.insert(std::pair<int, std::string>(camIndexInt,"Unknown camera"));
                }
                else
                {
                    //i need to convert from addres to std::string
                    char camName[MAX_CAM_NAME_LENGTH];
                    memset(camName, 0, sizeof(camName));
                    sprintf(camName, "%s", video_cap.card);
                    deviceList.insert(std::pair<int, std::string>(camIndexInt, camName));
                }
            }
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

    void DeviceManager::DisplayDeviceInformation(IEnumMoniker *pEnum, std::map<int, std::string> *deviceList)
    {
        IMoniker *pMoniker = NULL;
        int deviceIndex = 0;


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
                char deviceName[MAX_CAM_NAME_LENGTH];
                memset(deviceName, 0, sizeof(deviceName));
                sprintf_s(deviceName, "%S", var.bstrVal);
                deviceList->insert(std::pair<int, std::string>(deviceIndex, deviceName));
                deviceIndex++;
                VariantClear(&var);
            }

            pPropBag->Release();
            pMoniker->Release();
        }
    }
#endif
