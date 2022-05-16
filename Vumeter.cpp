#include "Vumeter.h"
#include "config.h"
#include "CastaWatchCommon.h"
#include <driver/i2s.h>
uint8_t buffer[BUFFER_SIZE] = {0};
#include <arduinoFFT.h>


// TWATCH 2020 V3 PDM microphone pin
#define MIC_DATA            2
#define MIC_CLOCK           0

static lv_obj_t * myScreen;

#define SAMPLES         1024          // Must be a power of 2
#define SAMPLING_FREQ   44100         // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
//#define AMPLITUDE       1000          // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define NUM_BANDS       16            
int oldBarHeights[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int bandValues[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

static void event_close(lv_event_t * e)
{
    lv_scr_load(mainScreen);
    lv_obj_del(myScreen);
    InitializeSpeaker();
}

uint32_t color[16] = {0}; 

void LaunchVumeter(){

    myScreen =  lv_obj_create(NULL);
    lv_obj_set_size(myScreen, 240, 240);
    lv_scr_load(myScreen);    


 //Close
    lv_obj_t * btn = lv_btn_create(myScreen);

    static lv_style_t btnStyle;
    lv_style_init(&btnStyle);
    lv_style_set_text_font(&btnStyle, &lv_font_montserrat_22);
    lv_style_set_radius(&btnStyle, 50);

    lv_obj_add_style(btn, &btnStyle, LV_PART_MAIN);
    lv_obj_t * lbl = lv_label_create(btn);
    lv_obj_align(lbl, LV_ALIGN_CENTER,0,0);
    lv_label_set_text(lbl, "Close");

    lv_obj_add_event_cb(btn, event_close, LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0,0);

/*
    lv_obj_t * cht = lv_chart_create(myScreen);
    lv_chart_set_type(cht, LV_CHART_TYPE_BAR);
    
    lv_obj_set_size(cht, 240,240);
    lv_obj_align(cht, LV_ALIGN_CENTER,0,0);
    lv_chart_set_range(cht, )
*/
    color[0] = ttgo->tft->color565(0x9C, 0x4F, 0X96);
    color[1] = ttgo->tft->color565(0xFF, 0x63, 0X55);
    color[2] = ttgo->tft->color565(0xFB, 0xA9, 0X49);
    color[3] = ttgo->tft->color565(0xFA, 0xE4, 0X42);
    color[4] = ttgo->tft->color565(0x8B, 0xD4, 0X48);
    color[5] = ttgo->tft->color565(0x2A, 0xA8, 0XF2);

    color[6] = ttgo->tft->color565(0x9C, 0x4F, 0X96);
    color[7] = ttgo->tft->color565(0xFF, 0x63, 0X55);
    color[8] = ttgo->tft->color565(0xFB, 0xA9, 0X49);
    color[9] = ttgo->tft->color565(0xFA, 0xE4, 0X42);
    color[10] = ttgo->tft->color565(0x8B, 0xD4, 0X48);
    color[11] = ttgo->tft->color565(0x2A, 0xA8, 0XF2);

    color[12] = ttgo->tft->color565(0x9C, 0x4F, 0X96);
    color[13] = ttgo->tft->color565(0xFF, 0x63, 0X55);
    color[14] = ttgo->tft->color565(0xFB, 0xA9, 0X49);
    color[15] = ttgo->tft->color565(0xFA, 0xE4, 0X42);

    InitializeMic();
}

uint8_t             val1, val2;
int16_t             val16 = 0;
int16_t             val_max = 0;
int16_t             val_max_1 = 0;
int32_t             all_val_zero1 = 0;
int32_t             all_val_zero2 = 0;
int32_t             all_val_zero3 = 0;
uint32_t            j = 0;
float               val_avg = 0;
float               val_avg_1 = 0;
float               all_val_avg = 0;


void UpdateVumeter(){

    for (int i = 0; i<NUM_BANDS; i++){
        bandValues[i] = 0;
    }

    size_t read_len = 0;
    j = j + 1;
    i2s_read(I2S_NUM_0, (char *) buffer, BUFFER_SIZE, &read_len, portMAX_DELAY);
    for(int i=0;i < BUFFER_SIZE/2;i++){
        val1 = buffer[i * 2];
        val2 = buffer[i * 2 + 1] ;
        val16 = val1 + val2 *  256;
        vReal[i] = ((float)val16/1000.0f);
        vImag[i] = 0;
        //Serial.printf("%d : %d %d |  %d %f\n",i,val1, val2, val16, vReal[i]);
    }

//     if (j % 2 == 0 && j > 0) {

         FFT.DCRemoval();
         FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
         FFT.Compute(FFT_FORWARD);
         FFT.ComplexToMagnitude();
        //lv_chart_set_next_value(chart, ser1, val_avg);
        //Serial.printf("Audio : %f\n", val_avg);
        for(int i=2;i<(SAMPLES/2);i++){
            if (vReal[i]>1){
                if (i<=2 )           bandValues[0]  += (int)vReal[i];
                if (i>2   && i<=3  ) bandValues[1]  += (int)vReal[i];
                if (i>3   && i<=5  ) bandValues[2]  += (int)vReal[i];
                if (i>5   && i<=7  ) bandValues[3]  += (int)vReal[i];
                if (i>7   && i<=9  ) bandValues[4]  += (int)vReal[i];
                if (i>9   && i<=13 ) bandValues[5]  += (int)vReal[i];
                if (i>13  && i<=18 ) bandValues[6]  += (int)vReal[i];
                if (i>18  && i<=25 ) bandValues[7]  += (int)vReal[i];
                if (i>25  && i<=36 ) bandValues[8]  += (int)vReal[i];
                if (i>36  && i<=50 ) bandValues[9]  += (int)vReal[i];
                if (i>50  && i<=69 ) bandValues[10] += (int)vReal[i];
                if (i>69  && i<=97 ) bandValues[11] += (int)vReal[i];
                if (i>97  && i<=135) bandValues[12] += (int)vReal[i];
                if (i>135 && i<=189) bandValues[13] += (int)vReal[i];
                if (i>189 && i<=264) bandValues[14] += (int)vReal[i];
                if (i>264          ) bandValues[15] += (int)vReal[i];
            }
        }
        /*
        for(int i=0;i<16;i++){
            Serial.printf("%d ", bandValues[i]);
        }
        Serial.printf("\n");
        */
  //  }

    for(int i=0;i<16;i++){
        int pos = i * 15;
        for (int y=0;y<16;y++){
            if (bandValues[i]/15>y ){
                ttgo->tft->fillRect(pos,240 -15- y*15, 15,15, color[i]);
            } else {
                ttgo->tft->fillRect(pos, 240 -15- y*15, 15,15,ttgo->tft->color565(0x00, 0x00, 0X00));
            }
        }
    }
}