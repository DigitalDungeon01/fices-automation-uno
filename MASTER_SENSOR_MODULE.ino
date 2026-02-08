void moduleSensorMaster() {

  /* ===== REQUEST TEMP ===== */
  ds18b20.requestTemperatures();

  /* ===== WATER TEMP ===== */
  temp = ds18b20.getTempCByIndex(0);
  t1t  = ds18b20.getTempCByIndex(1);
  t2t  = ds18b20.getTempCByIndex(2);
  t3t  = ds18b20.getTempCByIndex(3);

  // Temperature Guard: Default to 25.0 if sensor fails (-127 or invalid)
  if (temp < -50 || temp > 100) temp = 25.0;
  if (t1t < -50  || t1t > 100)  t1t  = 25.0;
  if (t2t < -50  || t2t > 100)  t2t  = 25.0;
  if (t3t < -50  || t3t > 100)  t3t  = 25.0;

  /* ===== PH (EC Power OFF to avoid interference) ===== */
  digitalWrite(PIN_EC_PWR, LOW); 
  delay(50); // Short settle time after power cut

  ph  = readPH(ph_master, PH1_PIN,   temp, PH_OFFSET_MASTER);
  t1p = readPH(ph_t1,     T1_PH_PIN, t1t,  PH_OFFSET_T1);
  t2p = readPH(ph_t2,     T2_PH_PIN, t2t,  PH_OFFSET_T2);
  t3p = readPH(ph_t3,     T3_PH_PIN, t3t,  PH_OFFSET_T3);

  /* ===== EC & TDS (EC Power ON) ===== */
  digitalWrite(PIN_EC_PWR, HIGH);
  delay(500); // Stabilization time for EC hardware to wake up correctly

  ec  = readEC(EC1_PIN, temp);       
  tds = readTDS(TDS1_PIN, temp);     

  t1tds = readTDS(T1_TDS_PIN, t1t);
  t2tds = readTDS(T2_TDS_PIN, t2t);
  t3tds = readTDS(T3_TDS_PIN, t3t);

  /* ===== ULTRASONIC RAW (CM) ===== */
  va  = readUltrasonicCM(VA_TRIG_PIN, VA_ECHO_PIN); delay(50);
  vb  = readUltrasonicCM(VB_TRIG_PIN, VB_ECHO_PIN); delay(50);
  vc  = readUltrasonicCM(VC_TRIG_PIN, VC_ECHO_PIN); delay(50);

  mtl = readUltrasonicCM(MTL_TRIG_PIN, MTL_ECHO_PIN); delay(50);
  btl = readUltrasonicCM(BTL_TRIG_PIN, BTL_ECHO_PIN); delay(50);

  t1l = readUltrasonicCM(T1_TRIG_PIN, T1_ECHO_PIN); delay(50);
  t2l = readUltrasonicCM(T2_TRIG_PIN, T2_ECHO_PIN); delay(50);
  t3l = readUltrasonicCM(T3_TRIG_PIN, T3_ECHO_PIN); delay(50);

  /* ===== CONVERT TO % (OVERWRITE VALUE) ===== */
  mtl = levelToPercent(mtl, 10, 150);
  btl = levelToPercent(btl, 10, 120);

  va  = levelToPercent(va, 10, 80);
  vb  = levelToPercent(vb, 10, 80);
  vc  = levelToPercent(vc, 10, 80);

  t1l = levelToPercent(t1l, 10, 100);
  t2l = levelToPercent(t2l, 10, 100);
  t3l = levelToPercent(t3l, 10, 100);
}
