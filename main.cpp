#include <windows.h>
#include <stdio.h>
#include <iostream>

#define MAX_SEM_COUNT 10
#define THREADCOUNT 12

int NUMBEROFSEATSWR = 2;

HANDLE ghSemaphore;

HANDLE barberReady;
HANDLE accessWRSeats;
HANDLE custReady;


DWORD WINAPI barber(LPVOID);


int main(void) {
    //HANDLE aThread[THREADCOUNT];
    HANDLE barberThread;
    DWORD ThreadID;
    int i;

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

    return 0;
}


DWORD WINAPI barber(LPVOID lpParam) {
// lpParam not used in this example
    UNREFERENCED_PARAMETER(lpParam);
    DWORD dwWaitResult;
    int i = 0;
    while (i < 4) {
// Try to enter the semaphore gate.
        std::cout << "Barber is sleeping" << std::endl;
        WaitForSingleObject(
                custReady,   // handle to semaphore
                INFINITE);           // infinite time-out interval
        std::cout << "Barber is awaken" << std::endl;
/*
// The semaphore object was signaled.
dwWaitResult = WaitForSingleObject(
        accessWRSeats,   // handle to semaphore
        INFINITE);           // zero-second time-out interval
*/
//NUMBEROFSEATSWR++;
        if (!ReleaseSemaphore(
                barberReady,  // handle to semaphore
                1,            // increase count by one
                NULL))       // not interested in previous count
        {
            printf("ReleaseSemaphore error: %d\n", GetLastError());
        }

// Simulate thread spending time on task
//Sleep(5);


        if (!ReleaseSemaphore(
                accessWRSeats,  // handle to semaphore
                1,            // increase count by one
                NULL))       // not interested in previous count
        {
            printf("ReleaseSemaphore error: %d\n", GetLastError());
        }
        std::cout << "Barber is cutting" << std::endl;
        Sleep(1000); // simulate cutting
//i++;
    }
    return 0;
}
