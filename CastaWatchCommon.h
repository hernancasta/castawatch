#include "config.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

#ifndef CASTA_WATCH_COMMON
#define CASTA_WATCH_COMMON

#define MSG_BRIGHTNESS_CHANGED


extern TTGOClass *ttgo;
extern lv_obj_t * mainScreen;
extern bool use24HS;
extern uint8_t brightness;
extern AudioGeneratorMP3 *mp3;
extern AudioOutputI2S *out;

void GoSleep();
void PlaySound(int index);

#endif