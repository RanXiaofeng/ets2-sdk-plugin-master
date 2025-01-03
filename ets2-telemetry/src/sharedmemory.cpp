#include "sharedmemory.hpp"
#include "ets2-telemetry-common.hpp"
#include <stdio.h>

SharedMemory::SharedMemory() : hMapFile(NULL), pBuffer(NULL), hooked(false) {
}

SharedMemory::~SharedMemory() {
    Close();
}

bool SharedMemory::Create(LPCWSTR name, unsigned int size) {
    hMapFile = CreateFileMappingW(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        size,
        name);

    if (hMapFile == NULL) {
        return false;
    }

    pBuffer = MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        size);

    if (pBuffer == NULL) {
        CloseHandle(hMapFile);
        hMapFile = NULL;
        return false;
    }

    ZeroMemory(pBuffer, size);
    hooked = true;
    return true;
}

bool SharedMemory::Open(LPCWSTR name, unsigned int size) {
    hMapFile = OpenFileMappingW(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        name);

    if (hMapFile == NULL) {
        return false;
    }

    pBuffer = MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        size);

    if (pBuffer == NULL) {
        CloseHandle(hMapFile);
        hMapFile = NULL;
        return false;
    }

    hooked = true;
    return true;
}

void SharedMemory::Close() {
    if (pBuffer != NULL) {
        UnmapViewOfFile(pBuffer);
        pBuffer = NULL;
    }

    if (hMapFile != NULL) {
        CloseHandle(hMapFile);
        hMapFile = NULL;
    }

    hooked = false;
}

bool SharedMemory::Hooked() {
    return hooked;
}

void* SharedMemory::GetBuffer() {
    return pBuffer;
}