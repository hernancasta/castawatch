#ifndef GENERAL_SETTINGS
#define GENERAL_SETTINGS

  #define MSG_BRIGHTNESS_CHANGED

  void LaunchGeneralSettings();
  static void event_handler(lv_event_t * e);
  static void event_close(lv_event_t * e);
  static void event_slider(lv_event_t * e);

#endif
