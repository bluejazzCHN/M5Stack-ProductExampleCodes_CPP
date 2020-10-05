
/**********************
    STATIC VARIABLES
 **********************/
static lv_obj_t * tv;
static lv_obj_t * t1;
static lv_obj_t * t2;
static lv_obj_t * t3;
//static lv_obj_t * kb;
static lv_style_t style_box;

void btn_check() {
  if (M5.BtnA.wasReleased())
  {
    Serial.print("BtnA \n ");
      lv_tabview_set_tab_act( tv,0, LV_ANIM_OFF);
  }
  if (M5.BtnB.wasPressed())
  {
    Serial.print("BtnB \n ");
    lv_page_scroll_ver( lv_tabview_get_tab(tv,lv_tabview_get_tab_act(tv)), 50);
  }
   if (M5.BtnC.wasPressed())
  {
    Serial.print("BtnC \n ");
    lv_page_scroll_ver( lv_tabview_get_tab(tv,lv_tabview_get_tab_act(tv)), -50);
  }
}

void lv_demo_widgets(void)
{
  tv = lv_tabview_create(lv_scr_act(), NULL);
  lv_tabview_set_anim_time(tv,0);
  if (LV_THEME_DEFAULT_INIT == lv_theme_material_init) {
    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
    if (disp_size >= LV_DISP_SIZE_MEDIUM) {
      lv_obj_set_style_local_pad_left(tv, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES / 2);
      lv_obj_t * sw = lv_switch_create(lv_scr_act(), NULL);
      if (lv_theme_get_flags() & LV_THEME_MATERIAL_FLAG_DARK)
        lv_switch_on(sw, LV_ANIM_OFF);
      lv_obj_set_event_cb(sw, color_chg_event_cb);
      lv_obj_set_pos(sw, LV_DPX(10), LV_DPX(10));
      lv_obj_set_style_local_value_str(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, "Dark");
      lv_obj_set_style_local_value_align(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_RIGHT_MID);
      lv_obj_set_style_local_value_ofs_x(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_DPI / 35);
    }
  }

  t1 = lv_tabview_add_tab(tv, "Controls");
  t2 = lv_tabview_add_tab(tv, "Visuals");
  //    t3 = lv_tabview_add_tab(tv, "Selectors");


  lv_style_init(&style_box);
  lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_LEFT);
  lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, - LV_DPX(10));
  lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(30));

  controls_create(t1);
  visuals_create(t2);
  //    selectors_create(t3);

#if LV_DEMO_WIDGETS_SLIDESHOW
  lv_task_create(tab_changer_task_cb, 1000, LV_TASK_PRIO_MID, NULL);
#endif
}


static void controls_create(lv_obj_t * parent)
{
  lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);

  lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
  lv_coord_t grid_w = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1);

#if LV_DEMO_WIDGETS_SLIDESHOW == 0
  static const char * btns[] = {"Cancel", "Ok", ""};

  lv_obj_t * m = lv_msgbox_create(lv_scr_act(), NULL);
  lv_msgbox_add_btns(m, btns);
  lv_obj_t * btnm = lv_msgbox_get_btnmatrix(m);
  lv_btnmatrix_set_btn_ctrl(btnm, 1, LV_BTNMATRIX_CTRL_CHECK_STATE);
#endif

  lv_obj_t * h = lv_cont_create(parent, NULL);
  lv_cont_set_layout(h, LV_LAYOUT_PRETTY_MID);
  lv_obj_add_style(h, LV_CONT_PART_MAIN, &style_box);
  lv_obj_set_drag_parent(h, true);

  lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Basics");

  lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
  lv_obj_set_width(h, grid_w);
  lv_obj_t * btn = lv_btn_create(h, NULL);
  lv_btn_set_fit2(btn, LV_FIT_NONE, LV_FIT_TIGHT);
  lv_obj_set_width(btn, lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
  lv_obj_t * label = lv_label_create(btn, NULL);
  lv_label_set_text(label , "Button");

  btn = lv_btn_create(h, btn);
  lv_btn_toggle(btn);
  label = lv_label_create(btn, NULL);
  lv_label_set_text(label , "Button");

  lv_switch_create(h, NULL);

  lv_checkbox_create(h, NULL);

  lv_coord_t fit_w = lv_obj_get_width_fit(h);

  lv_obj_t * slider = lv_slider_create(h, NULL);
  lv_slider_set_value(slider, 40, LV_ANIM_OFF);
  //    lv_obj_set_event_cb(slider, slider_event_cb);
  lv_obj_set_width_margin(slider, fit_w);

  /*Use the knobs style value the display the current value in focused state*/
  lv_obj_set_style_local_margin_top(slider, LV_SLIDER_PART_BG, LV_STATE_DEFAULT, LV_DPX(25));
  lv_obj_set_style_local_value_font(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, lv_theme_get_font_small());
  lv_obj_set_style_local_value_ofs_y(slider, LV_SLIDER_PART_KNOB, LV_STATE_FOCUSED, - LV_DPX(25));
  lv_obj_set_style_local_value_opa(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_value_opa(slider, LV_SLIDER_PART_KNOB, LV_STATE_FOCUSED, LV_OPA_COVER);
  lv_obj_set_style_local_transition_time(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, 300);
  lv_obj_set_style_local_transition_prop_5(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_STYLE_VALUE_OFS_Y);
  lv_obj_set_style_local_transition_prop_6(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, LV_STYLE_VALUE_OPA);

  slider = lv_slider_create(h, slider);
  lv_slider_set_type(slider, LV_SLIDER_TYPE_RANGE);
  lv_slider_set_value(slider, 70, LV_ANIM_OFF);
  lv_slider_set_left_value(slider, 30, LV_ANIM_OFF);
  lv_obj_set_style_local_value_ofs_y(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, - LV_DPX(25));
  lv_obj_set_style_local_value_font(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, lv_theme_get_font_small());
  lv_obj_set_style_local_value_opa(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, LV_OPA_COVER);
  //    lv_obj_set_event_cb(slider, slider_event_cb);
  lv_event_send(slider, LV_EVENT_VALUE_CHANGED, NULL);      /*To refresh the text*/
  if (lv_obj_get_width(slider) > fit_w) lv_obj_set_width(slider, fit_w);

  h = lv_cont_create(parent, h);
  lv_cont_set_fit(h, LV_FIT_NONE);
  lv_obj_set_style_local_value_str(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Text input");

  lv_obj_t * ta = lv_textarea_create(h, NULL);
  lv_cont_set_fit2(ta, LV_FIT_PARENT, LV_FIT_NONE);
  lv_textarea_set_text(ta, "");
  lv_textarea_set_placeholder_text(ta, "E-mail address");
  lv_textarea_set_one_line(ta, true);
  lv_textarea_set_cursor_hidden(ta, true);
  lv_obj_set_event_cb(ta, ta_event_cb);

  ta = lv_textarea_create(h, ta);
  lv_textarea_set_pwd_mode(ta, true);
  lv_textarea_set_placeholder_text(ta, "Password");

  ta = lv_textarea_create(h, NULL);
  lv_cont_set_fit2(ta, LV_FIT_PARENT, LV_FIT_NONE);
  lv_textarea_set_text(ta, "");
  lv_textarea_set_placeholder_text(ta, "Message");
  lv_textarea_set_cursor_hidden(ta, true);
  lv_obj_set_event_cb(ta, ta_event_cb);
  lv_cont_set_fit4(ta, LV_FIT_PARENT, LV_FIT_PARENT, LV_FIT_NONE, LV_FIT_PARENT);

#if LV_DEMO_WIDGETS_SLIDESHOW
  tab_content_anim_create(parent);
#endif
}
static void visuals_create(lv_obj_t * parent)
{
  lv_page_set_scrl_layout(parent, LV_LAYOUT_PRETTY_TOP);

  lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);

  lv_coord_t grid_h_chart = lv_page_get_height_grid(parent, 1, 1);
  lv_coord_t grid_w_chart = lv_page_get_width_grid(parent, disp_size <= LV_DISP_SIZE_LARGE ? 1 : 2, 1);

  lv_obj_t * chart = lv_chart_create(parent, NULL);
  lv_obj_add_style(chart, LV_CHART_PART_BG, &style_box);
  if (disp_size <= LV_DISP_SIZE_SMALL) {
    lv_obj_set_style_local_text_font(chart, LV_CHART_PART_SERIES_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());

  }
  lv_obj_set_drag_parent(chart, true);
  lv_obj_set_style_local_value_str(chart, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Line chart");
  lv_obj_set_width_margin(chart, grid_w_chart);
  lv_obj_set_height_margin(chart, grid_h_chart);
  lv_chart_set_div_line_count(chart, 3, 0);
  lv_chart_set_point_count(chart, 8);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
  if (disp_size > LV_DISP_SIZE_SMALL) {
    lv_obj_set_style_local_pad_left(chart,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 4 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_bottom(chart,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 3 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_right(chart,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 2 * (LV_DPI / 10));
    lv_obj_set_style_local_pad_top(chart,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 2 * (LV_DPI / 10));
    lv_chart_set_y_tick_length(chart, 0, 0);
    lv_chart_set_x_tick_length(chart, 0, 0);
    lv_chart_set_y_tick_texts(chart, "600\n500\n400\n300\n200", 0, LV_CHART_AXIS_DRAW_LAST_TICK);
    lv_chart_set_x_tick_texts(chart, "Jan\nFeb\nMar\nApr\nMay\nJun\nJul\nAug", 0, LV_CHART_AXIS_DRAW_LAST_TICK);
  }
  lv_chart_series_t * s1 = lv_chart_add_series(chart, LV_THEME_DEFAULT_COLOR_PRIMARY);
  lv_chart_series_t * s2 = lv_chart_add_series(chart, LV_THEME_DEFAULT_COLOR_SECONDARY);

  lv_chart_set_next(chart, s1, 10);
  lv_chart_set_next(chart, s1, 90);
  lv_chart_set_next(chart, s1, 30);
  lv_chart_set_next(chart, s1, 60);
  lv_chart_set_next(chart, s1, 10);
  lv_chart_set_next(chart, s1, 90);
  lv_chart_set_next(chart, s1, 30);
  lv_chart_set_next(chart, s1, 60);
  lv_chart_set_next(chart, s1, 10);
  lv_chart_set_next(chart, s1, 90);

  lv_chart_set_next(chart, s2, 32);
  lv_chart_set_next(chart, s2, 66);
  lv_chart_set_next(chart, s2, 5);
  lv_chart_set_next(chart, s2, 47);
  lv_chart_set_next(chart, s2, 32);
  lv_chart_set_next(chart, s2, 32);
  lv_chart_set_next(chart, s2, 66);
  lv_chart_set_next(chart, s2, 5);
  lv_chart_set_next(chart, s2, 47);
  lv_chart_set_next(chart, s2, 66);
  lv_chart_set_next(chart, s2, 5);
  lv_chart_set_next(chart, s2, 47);

  lv_obj_t * chart2 = lv_chart_create(parent, chart);
  lv_chart_set_type(chart2, LV_CHART_TYPE_COLUMN);
  lv_obj_set_style_local_value_str(chart2, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Column chart");

  s1 = lv_chart_add_series(chart2, LV_THEME_DEFAULT_COLOR_PRIMARY);
  s2 = lv_chart_add_series(chart2, LV_THEME_DEFAULT_COLOR_SECONDARY);

  lv_chart_set_next(chart2, s1, 10);
  lv_chart_set_next(chart2, s1, 90);
  lv_chart_set_next(chart2, s1, 30);
  lv_chart_set_next(chart2, s1, 60);
  lv_chart_set_next(chart2, s1, 10);
  lv_chart_set_next(chart2, s1, 90);
  lv_chart_set_next(chart2, s1, 30);
  lv_chart_set_next(chart2, s1, 60);
  lv_chart_set_next(chart2, s1, 10);
  lv_chart_set_next(chart2, s1, 90);

  lv_chart_set_next(chart2, s2, 32);
  lv_chart_set_next(chart2, s2, 66);
  lv_chart_set_next(chart2, s2, 5);
  lv_chart_set_next(chart2, s2, 47);
  lv_chart_set_next(chart2, s2, 32);
  lv_chart_set_next(chart2, s2, 32);
  lv_chart_set_next(chart2, s2, 66);
  lv_chart_set_next(chart2, s2, 5);
  lv_chart_set_next(chart2, s2, 47);
  lv_chart_set_next(chart2, s2, 66);
  lv_chart_set_next(chart2, s2, 5);
  lv_chart_set_next(chart2, s2, 47);

  lv_coord_t grid_w_meter;
  if (disp_size <= LV_DISP_SIZE_SMALL) grid_w_meter = lv_page_get_width_grid(parent, 1, 1);
  else if (disp_size <= LV_DISP_SIZE_MEDIUM) grid_w_meter = lv_page_get_width_grid(parent, 2, 1);
  else grid_w_meter = lv_page_get_width_grid(parent, 3, 1);

  lv_coord_t meter_h = lv_page_get_height_fit(parent);
  lv_coord_t meter_size = LV_MATH_MIN(grid_w_meter, meter_h);

  lv_obj_t * lmeter = lv_linemeter_create(parent, NULL);
  lv_obj_set_drag_parent(lmeter, true);
  lv_linemeter_set_value(lmeter, 50);
  lv_obj_set_size(lmeter, meter_size, meter_size);
  lv_obj_add_style(lmeter, LV_LINEMETER_PART_MAIN, &style_box);
  lv_obj_set_style_local_value_str(lmeter, LV_LINEMETER_PART_MAIN, LV_STATE_DEFAULT, "Line meter");

  lv_obj_t * label = lv_label_create(lmeter, NULL);
  lv_obj_align(label, lmeter, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_theme_get_font_title());

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, lmeter);
  //    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)linemeter_anim);
  lv_anim_set_values(&a, 0, 100);
  lv_anim_set_time(&a, 4000);
  lv_anim_set_playback_time(&a, 1000);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_start(&a);


  lv_obj_t * gauge = lv_gauge_create(parent, NULL);
  lv_obj_set_drag_parent(gauge, true);
  lv_obj_set_size(gauge, meter_size, meter_size);
  lv_obj_add_style(gauge, LV_GAUGE_PART_MAIN, &style_box);
  lv_obj_set_style_local_value_str(gauge, LV_GAUGE_PART_MAIN, LV_STATE_DEFAULT, "Gauge");

  label = lv_label_create(gauge, label);
  lv_obj_align(label, gauge, LV_ALIGN_CENTER, 0, grid_w_meter / 3);

  lv_anim_set_var(&a, gauge);
  //    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)gauge_anim);
  lv_anim_start(&a);

  lv_obj_t * arc = lv_arc_create(parent, NULL);
  lv_obj_set_drag_parent(arc, true);
  lv_arc_set_bg_angles(arc, 0, 360);
  lv_arc_set_rotation(arc, 270);
  lv_arc_set_angles(arc, 0, 0);
  lv_obj_set_size(arc,  meter_size, meter_size);
  lv_obj_add_style(arc, LV_ARC_PART_BG, &style_box);
  lv_obj_set_style_local_value_str(arc, LV_ARC_PART_BG, LV_STATE_DEFAULT, "Arc");

  label = lv_label_create(arc, label);
  lv_obj_align(label, arc, LV_ALIGN_CENTER, 0, 0);

  lv_anim_set_var(&a, arc);
  //    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)arc_anim);
  lv_anim_set_values(&a, 0, 360);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_start(&a);

  /*Create a bar and use the backgrounds value style property to display the current value*/
  lv_obj_t * bar_h = lv_cont_create(parent, NULL);
  lv_cont_set_fit2(bar_h, LV_FIT_NONE, LV_FIT_TIGHT);
  lv_obj_add_style(bar_h, LV_CONT_PART_MAIN, &style_box);
  lv_obj_set_style_local_value_str(bar_h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Bar");

  if (disp_size <= LV_DISP_SIZE_SMALL) lv_obj_set_width(bar_h, lv_page_get_width_grid(parent, 1, 1));
  else if (disp_size <= LV_DISP_SIZE_MEDIUM) lv_obj_set_width(bar_h, lv_page_get_width_grid(parent, 2, 1));
  else lv_obj_set_width(bar_h, lv_page_get_width_grid(parent, 3, 2));

  lv_obj_t * bar = lv_bar_create(bar_h, NULL);
  lv_obj_set_width(bar, lv_obj_get_width_fit(bar_h));
  lv_obj_set_style_local_value_font(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
  lv_obj_set_style_local_value_align(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_MID);
  lv_obj_set_style_local_value_ofs_y(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 20);
  lv_obj_set_style_local_margin_bottom(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_DPI / 7);
  lv_obj_align(bar, NULL, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * led_h = lv_cont_create(parent, NULL);
  lv_cont_set_layout(led_h, LV_LAYOUT_PRETTY_MID);
  if (disp_size <= LV_DISP_SIZE_SMALL) lv_obj_set_width(led_h, lv_page_get_width_grid(parent, 1, 1));
  else if (disp_size <= LV_DISP_SIZE_MEDIUM) lv_obj_set_width(led_h, lv_page_get_width_grid(parent, 2, 1));
  else lv_obj_set_width(led_h, lv_page_get_width_grid(parent, 3, 1));

  lv_obj_set_height(led_h, lv_obj_get_height(bar_h));
  lv_obj_add_style(led_h, LV_CONT_PART_MAIN, &style_box);
  lv_obj_set_drag_parent(led_h, true);
  lv_obj_set_style_local_value_str(led_h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "LEDs");

  lv_obj_t * led = lv_led_create(led_h, NULL);
  lv_coord_t led_size = lv_obj_get_height_fit(led_h);
  lv_obj_set_size(led, led_size, led_size);
  lv_led_off(led);

  led = lv_led_create(led_h, led);
  lv_led_set_bright(led, (LV_LED_BRIGHT_MAX - LV_LED_BRIGHT_MIN) / 2 + LV_LED_BRIGHT_MIN);

  led = lv_led_create(led_h, led);
  lv_led_on(led);

  if (disp_size == LV_DISP_SIZE_MEDIUM) {
    lv_obj_add_protect(led_h, LV_PROTECT_POS);
    lv_obj_align(led_h, bar_h, LV_ALIGN_OUT_BOTTOM_MID, 0, lv_obj_get_style_margin_top(led_h, LV_OBJ_PART_MAIN) + lv_obj_get_style_pad_inner(parent, LV_PAGE_PART_SCROLLABLE));
  }

  //    lv_task_create(bar_anim, 100, LV_TASK_PRIO_LOW, bar);
}

static void color_chg_event_cb(lv_obj_t * sw, lv_event_t e)
{
  if (LV_THEME_DEFAULT_INIT != lv_theme_material_init) return;
  if (e == LV_EVENT_VALUE_CHANGED) {
    uint32_t flag = LV_THEME_MATERIAL_FLAG_LIGHT;
    if (lv_switch_get_state(sw)) flag = LV_THEME_MATERIAL_FLAG_DARK;

    LV_THEME_DEFAULT_INIT(lv_theme_get_color_primary(), lv_theme_get_color_secondary(),
                          flag,
                          lv_theme_get_font_small(), lv_theme_get_font_normal(), lv_theme_get_font_subtitle(), lv_theme_get_font_title());
  }
}
