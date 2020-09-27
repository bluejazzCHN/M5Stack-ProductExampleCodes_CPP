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

static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
static lv_obj_t * kb;
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
  if (data->point.x > -1 && data->point.y > -1 )
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

/*1.Demo--TextArea and KeyBoard*/
void lv_ex_textarea(void)
{
  /* Create the text area */
  lv_obj_t * ta = lv_textarea_create(lv_scr_act(), NULL);
  lv_obj_set_event_cb(ta, ta_event_cb);
  lv_textarea_set_accepted_chars(ta, "0123456789:");
  lv_textarea_set_max_length(ta, 5);
  //  lv_textarea_set_one_line(ta, true);
  lv_textarea_set_text(ta, "");
  lv_obj_set_size(ta, 316, 100);
  lv_obj_align(ta, NULL, LV_LABEL_ALIGN_LEFT, 0, -60);

  /* Create a keyboard*/
  kb = lv_keyboard_create(lv_scr_act(), NULL);
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
