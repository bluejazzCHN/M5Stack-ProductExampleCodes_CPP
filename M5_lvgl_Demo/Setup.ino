
void setup() {
  /*M5Core2 init*/
  M5.begin(true, true, true, true);
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(WHITE);

  /*lvgl init*/
  lv_init();
  lvgl_display_init();
  lvgl_input_device_init();

  /*widget init*/
  //  lv_tabview();
  //  lv_ex_textarea();
  //  lv_another_page();
  //  lv_ex_win_1();
  lv_demo_widgets();

}
