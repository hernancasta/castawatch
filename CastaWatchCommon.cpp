#include "config.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "Sounds/ohno.h"
#include <driver/i2s.h>
#include "CastaWatchCommon.h"


TTGOClass *ttgo;
lv_obj_t * mainScreen;
bool use24HS = false;
uint8_t brightness = 255;
float volume = 0.5f;
bool IsAudioOutMode = true;

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

    // workaround to allow use I2S_0 shared between mic and speaker
    InitializeSpeaker();

    file = new AudioFileSourcePROGMEM(ohno_sound, sizeof(ohno_sound));
    id3 = new AudioFileSourceID3(file);
    mp3->begin(id3, out);
    out->SetGain(volume);
}

void InitializeMic(){

    i2s_driver_uninstall(I2S_NUM_0);

    IsAudioOutMode = false;

     /*****************
     * mic
     * 
     * ****************/

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate =  44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };


    i2s_pin_config_t i2s_cfg;
    i2s_cfg.bck_io_num   = I2S_PIN_NO_CHANGE;
    i2s_cfg.ws_io_num    = MIC_CLOCK;
    i2s_cfg.data_out_num = I2S_PIN_NO_CHANGE;
    i2s_cfg.data_in_num  = MIC_DATA;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_cfg);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void InitializeSpeaker(){
      /****************************
     * Initialize Audio
     * **************************/
    IsAudioOutMode = true;
    i2s_driver_uninstall(I2S_NUM_0);

    ttgo->enableAudio();
    ttgo->enableLDO3();
    #if defined(STANDARD_BACKPLANE)
        out = new AudioOutputI2S(0, 1);
    #elif defined(EXTERNAL_DAC_BACKPLANE)
        out = new AudioOutputI2S();
        //External DAC decoding
        out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
    #endif
    mp3 = new AudioGeneratorMP3();

}