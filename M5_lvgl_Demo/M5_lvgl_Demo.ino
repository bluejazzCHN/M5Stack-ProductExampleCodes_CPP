/*
    Author: bluejazzchn
    lvgl flow : lv_init() --> lv_disp_buf_init() --> lv_disp_drv_init()-->lv_indev_drv_init() --> widgets init()
                                                                                                                                   other dev init()

    Comments:
    1. my_input_read is schedule task， poll touchpad , touch screen,mouse device.
    2. myDisplayFlush is screen flush function
*/

#include <M5Core2.h>
#include <lvgl.h>

/************lvgl global variables definition***************/
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
static lv_obj_t * kb;
lv_obj_t * ta;
lv_obj_t * page1;
lv_obj_t * page2;

lv_obj_t *tabview;
lv_obj_t *tab1;
lv_obj_t *tab2;


/************lvgl Init Collections***************/

void lvgl_display_init()
{
  /*initialize the display*/
  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = myDisplayFlush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);
}
void lvgl_input_device_init()
{
  /*initialize the input*/
  lv_indev_drv_t indev_drv;                  /*Descriptor of a input device driver*/
  lv_indev_drv_init(&indev_drv);             /*Basic initialization*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_input_read;      /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/
}
/************lvgl Init Collections end***************/


/************lvgl Callback Collections***************/
/*Display flushing*/
void myDisplayFlush(lv_disp_drv_t * disp, const lv_area_t *area, lv_color_t *color_p)
{
  //  Serial.println("Screan flushed");
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  M5.Lcd.startWrite();
  M5.Lcd.setAddrWindow(area->x1, area->y1, w, h);
  M5.Lcd.pushColors(&color_p->full, w * h, true);
  M5.Lcd.endWrite();
  lv_disp_flush_ready(disp);
}

/*input device*/
bool my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
  /*M5Core2 get touch point*/
  TouchPoint_t pos = M5.Touch.getPressPoint();
  if (pos.x > -1 && pos.y > -1)
  {
    Serial.println("indev");
    data->state = LV_INDEV_STATE_PR ;
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
  data->point.x = pos.x;
  data->point.y = pos.y;
  return false;
}
/************lvgl Callback Collections end***************/

/*Demo*/
/*Create TabView*/
void lv_tabview()
{
  /*Create a Tab view object*/
  tabview = lv_tabview_create(lv_scr_act(), NULL);
}

/*1.Demo--TextArea and KeyBoard*/
void lv_ex_textarea(void)
{
  /*Create a tab for textarea*/
  tab1 = lv_tabview_add_tab(tabview, "Main");
  lv_obj_set_size(tab1, 320, 240);
  lv_obj_align(tab1, NULL, LV_ALIGN_CENTER, 0, 0);
  /* Create the text area */
  ta = lv_textarea_create(tab1, NULL);
  lv_obj_set_event_cb(ta, ta_event_cb);
  lv_textarea_set_accepted_chars(ta, "0123456789:");
  lv_textarea_set_max_length(ta, 5);
  lv_textarea_set_text_sel(ta, true);
  lv_textarea_set_text(ta, "");
  lv_obj_set_size(ta, 316, 100);
  lv_obj_align(ta, NULL, LV_LABEL_ALIGN_LEFT, 0, -60);

  /* Create a keyboard*/
  kb = lv_keyboard_create(tab1, NULL);
  lv_obj_set_size(kb,  LV_HOR_RES, LV_VER_RES / 2);
  lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUM);
  lv_keyboard_set_textarea(kb, ta);
}

static void ta_event_cb(lv_obj_t * ta, lv_event_t event)
{
  if (event == LV_EVENT_VALUE_CHANGED) {
    const char * txt = lv_textarea_get_text(ta);
    if (txt[0] >= '0' && txt[0] <= '9' &&
        txt[1] >= '0' && txt[1] <= '9' &&
        txt[2] != ':')
    {
      lv_textarea_set_cursor_pos(ta, 2);
      lv_textarea_add_char(ta, ':');
    }
  }
}

/*Create no 2 page*/
void lv_another_page()
{
  /*Create a page for textarea*/
  tab2 = lv_tabview_add_tab(tabview, "Tab 2");
  lv_obj_set_size(tab2, 320, 200);
  lv_obj_align(tab2, NULL, LV_ALIGN_CENTER, 0, 0);

  /*Create a label on the page*/
  lv_obj_t * label = lv_label_create(tab2, NULL);
  lv_label_set_long_mode(label, LV_LABEL_LONG_BREAK);            /*Automatically break long lines*/
  lv_obj_set_width(label, lv_page_get_width_fit(tab2));          /*Set the label width to max value to not show hor. scroll bars*/
  lv_label_set_text(label, "This the first tab\n\n"
                    "If the content\n"
                    "of a tab\n"
                    "become too long\n"
                    "the it \n"
                    "automatically\n"
                    "become\n"
                    "scrollable.");
}

void lv_ex_win_1(void)
{
  /*Create a window*/
  lv_obj_t * win = lv_win_create(lv_scr_act(), NULL);
  lv_win_set_title(win, "Weather Station");                        /*Set the title*/


  /*Add control button to the header*/
  lv_obj_t * close_btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE);           /*Add close button and use built-in close action*/
  lv_obj_set_event_cb(close_btn, lv_win_close_event_cb);
  lv_win_add_btn(win, LV_SYMBOL_SETTINGS);        /*Add a setup button*/

  /*Add some dummy content*/
  lv_obj_t * txt = lv_label_create(win, NULL);
  lv_label_set_text(txt, "This is the content of the window\n\n"
                    "You can add control buttons to\n"
                    "the window header\n\n"
                    "The content area becomes\n"
                    "automatically scrollable is it's \n"
                    "large enough.\n\n"
                    " You can scroll the content\n"
                    "See the scroll bar on the right!");
}
