#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "synchro.h"
#include "oggstream.h"
#include "stream_common.h"

int main(int argc, char *argv[]) {
    int res;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s FILE", argv[0]);
        exit(EXIT_FAILURE);
    }
    assert(argc == 2);

    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);

    // Your code HERE
    pthread_t theorathread, vorbisthread;
    // start the two stream readers (theoraStreamReader and vorbisStreamReader)
    if (pthread_create(&theorathread, NULL, &theoraStreamReader, (void *) &argv[1])  != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    // each in a thread
    if (pthread_create(&vorbisthread, NULL, &vorbisStreamReader, (void *) &argv[1]) != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    // wait for vorbis thread
    pthread_join(vorbisthread, NULL);
    // 1 seconde of sound in advance, thus wait 1 seconde
    // before leaving
    sleep(1);

    // Cancel the video threads
    pthread_cancel(theorathread);
    pthread_cancel(theora2sdlthread);

    // Wait for theora and theora2sdl threads
    if (pthread_join(theorathread, NULL) != 0) {
        perror("Error in thread join");
        exit(EXIT_FAILURE);
    }
    if (pthread_join(theora2sdlthread, NULL) != 0) {
        perror("Error in thread join");
        exit(EXIT_FAILURE);
    }

    /* liberer des choses ? */
    pthread_mutex_destroy(&mutex_texture);
    pthread_mutex_destroy(&mutex_window);
    pthread_mutex_destroy(&mutex_stream);

    pthread_cond_destroy(&cond_window);
    pthread_cond_destroy(&cond_texture);
    pthread_cond_destroy(&cond_prod_cons);

  exit(EXIT_SUCCESS);
}
