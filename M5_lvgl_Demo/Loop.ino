void loop() {
  lv_tick_inc(5);
  lv_task_handler();
  delay(50);
}
