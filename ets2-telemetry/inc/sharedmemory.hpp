#pragma once
#include <windows.h>

class SharedMemory
{
public:
    static SharedMemory& getInstance() {
        static SharedMemory instance;
        return instance;
    }

    SharedMemory();
    ~SharedMemory();

    bool Create(LPWSTR name, unsigned int size);
    bool Open(LPWSTR name, unsigned int size);
    void Close();
    bool Hooked();
    void* GetBuffer();
    ets2TelemetryMap_s* getTelemetryData() {
        return static_cast<ets2TelemetryMap_s*>(GetBuffer());
    }

private:
    HANDLE hMapFile;
    void* pBuffer;
    bool hooked;

    // 禁止拷贝和赋值
    SharedMemory(const SharedMemory&) = delete;
    SharedMemory& operator=(const SharedMemory&) = delete;
};