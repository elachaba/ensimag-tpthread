#include "ensitheora.h"
#include "synchro.h"
#include <pthread.h>

/* les variables pour la synchro, ici */
pthread_mutex_t mutex_stream;
pthread_mutex_t mutex_window;
pthread_mutex_t mutex_texture;


pthread_cond_t cond_window;
pthread_cond_t cond_texture;
pthread_cond_t cond_prod_cons;

bool texture_sent = false;
int tex_count = 0;

/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&mutex_window);
    windowsx = buffer[0].width;
    windowsy = buffer[0].height;
    pthread_cond_signal(&cond_window);
    pthread_mutex_unlock(&mutex_window);
}

void attendreTailleFenetre() {
    pthread_mutex_lock(&mutex_window);
    // Wait for the windowsx and windowsy to be sent
    while (windowsx == 0 || windowsy == 0)
        pthread_cond_wait(&cond_window, &mutex_window);
    pthread_mutex_unlock(&mutex_window);
}

void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&mutex_texture);
    texture_sent = true;
    pthread_cond_signal(&cond_texture);
    pthread_mutex_unlock(&mutex_texture);
}

void attendreFenetreTexture() {
    pthread_mutex_lock(&mutex_texture);
    while (!texture_sent)
        pthread_cond_wait(&cond_texture, &mutex_texture);
    pthread_mutex_unlock(&mutex_texture);
}

void debutConsommerTexture() {
    pthread_mutex_lock(&mutex_texture);
    while (tex_count == 0)
        pthread_cond_wait(&cond_prod_cons, &mutex_texture);
    pthread_mutex_unlock(&mutex_texture);
}

void finConsommerTexture() {
    pthread_mutex_lock(&mutex_texture);
    tex_count--;
    pthread_cond_signal(&cond_prod_cons);
    pthread_mutex_unlock(&mutex_texture);
}

void debutDeposerTexture() {
    pthread_mutex_lock(&mutex_texture);
    while (tex_count == NBTEX) {
        pthread_cond_wait(&cond_prod_cons, &mutex_texture);
    }
    pthread_mutex_unlock(&mutex_texture);
}

void finDeposerTexture() {
    pthread_mutex_lock(&mutex_texture);
    tex_count++;
    pthread_cond_signal(&cond_prod_cons);
    pthread_mutex_unlock(&mutex_texture);
}
