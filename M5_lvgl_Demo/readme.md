
### if you want to use other keyboard type, you can change keyboard type in M5_lvgl_Demo.ino
    
    void lv_ex_textarea(void)
    {
    /* Create a keyboard*/
    ......
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUM); // please find in lvgl document
    ......
    }
 
### if you want to change keyboard function, you can change below function in your code

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
 
