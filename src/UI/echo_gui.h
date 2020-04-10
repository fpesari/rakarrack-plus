// generated by Fast Light User Interface Designer (fluid) version 1.0305

#ifndef echo_gui_h
#define echo_gui_h
#include <FL/Fl.H>
#include "RKR_Light_Button.h"
#include "RKR_Check_Button.h"
#include "RKR_Choice.h"
#include <FL/Fl_Group.H>
#include "sliderW.h"
#include "../process.h"
#include "rakarrack.h"

class EchoGui : public Fl_Group {
public:
  EchoGui(int X, int Y, int W, int H, const char *L = 0);
  RKR_Light_Button *echo_activar;
private:
  inline void cb_echo_activar_i(RKR_Light_Button*, void*);
  static void cb_echo_activar(RKR_Light_Button*, void*);
public:
  RKR_Choice *echo_preset;
private:
  inline void cb_echo_preset_i(RKR_Choice*, void*);
  static void cb_echo_preset(RKR_Choice*, void*);
  static Fl_Menu_Item menu_echo_preset[];
public:
  SliderW *echo_WD;
private:
  inline void cb_echo_WD_i(SliderW*, void*);
  static void cb_echo_WD(SliderW*, void*);
public:
  SliderW *echo_RV;
private:
  inline void cb_echo_RV_i(SliderW*, void*);
  static void cb_echo_RV(SliderW*, void*);
public:
  SliderW *echo_pan;
private:
  inline void cb_echo_pan_i(SliderW*, void*);
  static void cb_echo_pan(SliderW*, void*);
public:
  SliderW *echo_delay;
private:
  inline void cb_echo_delay_i(SliderW*, void*);
  static void cb_echo_delay(SliderW*, void*);
public:
  SliderW *echo_LRdl;
private:
  inline void cb_echo_LRdl_i(SliderW*, void*);
  static void cb_echo_LRdl(SliderW*, void*);
public:
  SliderW *echo_LRc;
private:
  inline void cb_echo_LRc_i(SliderW*, void*);
  static void cb_echo_LRc(SliderW*, void*);
public:
  SliderW *echo_fb;
private:
  inline void cb_echo_fb_i(SliderW*, void*);
  static void cb_echo_fb(SliderW*, void*);
public:
  RKR_Check_Button *echo_direct;
private:
  inline void cb_echo_direct_i(RKR_Check_Button*, void*);
  static void cb_echo_direct(RKR_Check_Button*, void*);
public:
  SliderW *echo_damp;
private:
  inline void cb_echo_damp_i(SliderW*, void*);
  static void cb_echo_damp(SliderW*, void*);
};
#endif
