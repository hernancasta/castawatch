#include "config.h"
#include "Functions.h"
#include "MainDisplay.h"
#include "CastaWatchCommon.h"
#include "AppList.h"


void setup() {
    Serial.begin(115200);

    /* ************************
        Initialize Watch
    */

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->setBrightness(128);
    ttgo->lvgl_begin();


    initializeStyle();
    mainScreen = lv_scr_act();

    lv_obj_t * panel = lv_obj_create(mainScreen);
    lv_obj_set_size(panel, 240, 240);
    lv_obj_set_scroll_snap_x(panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scroll_snap_y(panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_scroll_dir(panel, LV_DIR_HOR);
    lv_obj_set_style_pad_gap(panel, 0, 0);
    lv_obj_set_style_pad_all(panel, 0,0);

    static lv_style_t scrollbarstyle;
    lv_style_init(&scrollbarstyle);
    lv_style_set_width(&scrollbarstyle,0);
    lv_style_set_height(&scrollbarstyle,0);
    lv_obj_add_style(panel, &scrollbarstyle, LV_PART_SCROLLBAR);

    lv_obj_t * t1 = lv_obj_create(panel);
    lv_obj_set_size(t1, 240, 240);
    lv_obj_align(t1, LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_pad_gap(t1, 0, 0);
    lv_obj_set_style_pad_all(t1, 0,0);
    initializeClock(t1);
    lv_obj_t * t2 = lv_obj_create(panel);
    lv_obj_set_size(t2, 240, 240);
    lv_obj_align(t2, LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_pad_gap(t2, 0, 0);
    lv_obj_set_style_pad_all(t2, 0,0);
    lv_obj_set_scroll_dir(t2, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(t2, LV_SCROLLBAR_MODE_OFF);

    initializeAppList(t2);
    lv_obj_update_snap(panel, LV_ANIM_ON);
}

bool lowbright = false;

void loop() {
    lv_task_handler();
    updateDisplay();
    delay(5);

    ttgo->power->readIRQ();
            // these are the coordinates of x-, y- and z-axis of the accelerator indicates the reading position of the watch
    if (ttgo->power->isPEKShortPressIRQ()){
        Serial.printf("CLICK!\n");
        lowbright = !lowbright;
        if (lowbright){
            ttgo->setBrightness(1);
        } else {
            ttgo->setBrightness(255);
        }
        ttgo->power->clearIRQ();
    }
}


void initializeStyle(){
//    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, &lv_font_montserrat_14);
/*
    lv_style_init(&myStyle);
    lv_style_set_bg_color(&myStyle, lv_color_black());
    lv_style_set_text_color(&myStyle, lv_color_white());
    lv_obj_add_style(lv_scr_act(), &myStyle, LV_STATE_DEFAULT);
*/
    lv_theme_t * th = lv_theme_default_init( lv_disp_get_default(),
              lv_palette_main(LV_PALETTE_BLUE),
              lv_palette_main(LV_PALETTE_GREEN), true, &lv_font_montserrat_22);
    
    lv_disp_set_theme(lv_disp_get_default(), th);
    lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());
}


