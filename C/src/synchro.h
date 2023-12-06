#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "ensitheora.h"
#include <stdbool.h>

extern bool fini;

/* Les extern des variables pour la synchro ici */
extern pthread_mutex_t mutex_stream;
extern pthread_mutex_t mutex_window;
extern pthread_mutex_t mutex_texture;

extern pthread_cond_t cond_window;
extern pthread_cond_t cond_texture;
extern pthread_cond_t cond_prod_cons;
/* Fonctions de synchro à implanter */

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
