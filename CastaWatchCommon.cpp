#include "config.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "Sounds/ohno.h"

TTGOClass *ttgo;
lv_obj_t * mainScreen;
bool use24HS = false;
uint8_t brightness = 255;

AudioGeneratorMP3 *mp3;
AudioFileSourcePROGMEM *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

void GoSleep(){

    ttgo->displaySleep();
    ttgo->power->setPowerOutPut(AXP202_LDO3, false);
    ttgo->power->setPowerOutPut(AXP202_LDO4, false);
    ttgo->power->setPowerOutPut(AXP202_LDO2, false);
    // The following power channels are not used
    ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
    ttgo->power->setPowerOutPut(AXP202_DCDC2, false);

    // PEK Button WakeUp    
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
    // TOUCH SCREEN  Wakeup source
//     pinMode(TOUCH_INT, INPUT);
//    esp_sleep_enable_ext1_wakeup(GPIO_SEL_38 , ESP_EXT1_WAKEUP_ALL_LOW);

    esp_deep_sleep_start();


}

void PlaySound(int index){

    file = new AudioFileSourcePROGMEM(ohno_sound, sizeof(ohno_sound));
    id3 = new AudioFileSourceID3(file);
    mp3->begin(id3, out);
    out->SetGain(0.5f);
}