#include "config.h"

TTGOClass *ttgo;
lv_obj_t * mainScreen;
bool use24HS = false;
uint8_t brightness = 255;

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