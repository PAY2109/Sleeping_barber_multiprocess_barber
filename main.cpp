#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <random>

int NUMBEROFSEATSWR = 2;

HANDLE barberReady;
HANDLE accessWRSeats;
HANDLE custReady;
HANDLE seatBelt;


DWORD WINAPI barber(LPVOID);


int main(void) {
    HANDLE barberThread;
    DWORD ThreadID;

    barberReady = CreateSemaphore(
            NULL,           // default security attributes
            0,  // initial count
            1,  // maximum count
            "barberReady");          // barberReady semaphore
    accessWRSeats = CreateSemaphore(
            NULL,           // default security attributes
            NUMBEROFSEATSWR,  // initial count
            NUMBEROFSEATSWR,  // maximum count
            "accessWRSeats");          // accessWRSeats semaphore

    custReady = CreateSemaphore(
            NULL,           // default security attributes
            0,  // initial count
            1,  // maximum count
            "custReady");          // custReady semaphore

    seatBelt = CreateSemaphore(
            NULL,           // default security attributes
            0,  // initial count
            1,  // maximum count
            "seatBelt");          // seatBelt semaphore



    // Create worker threads


    barberThread = CreateThread(
            NULL,       // default security attributes
            0,          // default stack size
            (LPTHREAD_START_ROUTINE) barber,
            NULL,       // no thread function arguments
            0,          // default creation flags
            &ThreadID); // receive thread identifier

    if (barberThread == NULL) {
        printf("CreateThread error: %d\n", GetLastError());
        return 1;
    }


    // Wait for all threads to terminate

    WaitForSingleObject(barberThread, INFINITE);

    // Close thread and semaphore handles


    CloseHandle(barberThread);

    CloseHandle(barberReady);
    CloseHandle(accessWRSeats);
    CloseHandle(custReady);
    CloseHandle(seatBelt);

    return 0;
}


DWORD WINAPI barber(LPVOID lpParam) {
// lpParam not used
    UNREFERENCED_PARAMETER(lpParam);

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(1000, 10000); // define the range

    int i = 1;
    while (true) {

        std::cout << "Barber is sleeping" << std::endl;
        WaitForSingleObject(
                custReady,   // handle to semaphore
                INFINITE);           // infinite time-out interval
        std::cout << "Barber is awaken" << std::endl;

        if (!ReleaseSemaphore(
                barberReady,  // handle to semaphore
                1,            // increase count by one
                NULL))       // not interested in previous count
        {
            printf("ReleaseSemaphore error: %d\n", GetLastError());
        }

        if (!ReleaseSemaphore(
                accessWRSeats,  // handle to semaphore
                1,            // increase count by one
                NULL))       // not interested in previous count
        {
            printf("ReleaseSemaphore error: %d\n", GetLastError());
        }


        std::cout << "Barber is cutting client number " << i << std::endl;
        Sleep(distr(eng)); // simulate cutting
        if (!ReleaseSemaphore(
                seatBelt,  // handle to semaphore
                1,            // increase count by one
                NULL))       // not interested in previous count
        {
            printf("ReleaseSemaphore error: %d\n", GetLastError());
        }

        i++;
    }
    return 0;
}