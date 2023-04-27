#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <process.h>
#pragma comment(lib, "winmm.lib")


void play_sound(const char* filename) {
    PlaySound(filename, NULL, SND_FILENAME);
}

void play_sound_from_memory(void* sound_data, DWORD sound_size, DWORD flags) {
    PlaySound(sound_data, NULL, SND_MEMORY | SND_ASYNC | flags);
}

void play_correct_sound() {
    const char* filename = "correct_sound.wav";
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    DWORD sound_size = GetFileSize(hFile, NULL);
    if (sound_size == INVALID_FILE_SIZE) {
        printf("Failed to get file size: %s\n", filename);
        CloseHandle(hFile);
        return;
    }

    void* sound_data = malloc(sound_size);
    if (sound_data == NULL) {
        printf("Failed to allocate memory for sound data.\n");
        CloseHandle(hFile);
        return;
    }

    DWORD bytes_read;
    if (!ReadFile(hFile, sound_data, sound_size, &bytes_read, NULL)) {
        printf("Failed to read file: %s\n", filename);
        free(sound_data);
        CloseHandle(hFile);
        return;
    }

    CloseHandle(hFile);

    play_sound_from_memory(sound_data, sound_size, SND_ASYNC);

    free(sound_data);
}

void play_wrong_sound() {
    const char* filename = "wrong_sound.wav";
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    DWORD sound_size = GetFileSize(hFile, NULL);
    if (sound_size == INVALID_FILE_SIZE) {
        printf("Failed to get file size: %s\n", filename);
        CloseHandle(hFile);
        return;
    }

    void* sound_data = malloc(sound_size);
    if (sound_data == NULL) {
        printf("Failed to allocate memory for sound data.\n");
        CloseHandle(hFile);
        return;
    }

    DWORD bytes_read;
    if (!ReadFile(hFile, sound_data, sound_size, &bytes_read, NULL)) {
        printf("Failed to read file: %s\n", filename);
        free(sound_data);
        CloseHandle(hFile);
        return;
    }

    CloseHandle(hFile);

    play_sound_from_memory(sound_data, sound_size, SND_ASYNC);

    free(sound_data);
}


void play_background_music(void* arg) {
    const char* filename = "background_music.wav";
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    DWORD sound_size = GetFileSize(hFile, NULL);
    if (sound_size == INVALID_FILE_SIZE) {
        printf("Failed to get file size: %s\n", filename);
        CloseHandle(hFile);
        return;
    }

    void* sound_data = malloc(sound_size);
    if (sound_data == NULL) {
        printf("Failed to allocate memory for sound data.\n");
        CloseHandle(hFile);
        return;
    }

    DWORD bytes_read;
    if (!ReadFile(hFile, sound_data, sound_size, &bytes_read, NULL)) {
        printf("Failed to read file: %s\n", filename);
        free(sound_data);
        CloseHandle(hFile);
        return;
    }

    CloseHandle(hFile);

    PlaySound(filename, NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

    free(sound_data);
}
int main() {
HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)play_background_music, NULL, 0, NULL);
    if (hThread == NULL) {
        printf("Error creating thread.\n");
        return 1;
    }
while (1) {
    // Get user input and update the game state

    // Check for user input
    if (_kbhit()) {
        int input = getchar();
        switch (input) {
        case 'c':
            play_correct_sound();
            break;
        case 'w':
            play_wrong_sound();
            break;
        }

        // Clear input buffer
        while (getchar() != '\n') {
            // do nothing
        }
    }
}

    // Wait for the background music thread to exit
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}
