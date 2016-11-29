#ifndef LOAD_AUDIO_EFFECTS_H
#define LOAD_AUDIO_EFFECTS_H
#include "library.h"

extern Mix_Chunk *DUEL_MODE_hit[3];
extern Mix_Chunk *DUEL_MODE_START;
extern bool DUEL_MODE_EFFECTS_LOADED;

void Load_Duel_Mode_effects();

void Clear_Duel_Mode_effects();

#endif //LOAD_AUDIO_EFFECTS_H
