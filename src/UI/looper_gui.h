// generated by Fast Light User Interface Designer (fluid) version 1.0304

#ifndef looper_gui_h
#define looper_gui_h
#include <FL/Fl.H>
#include "RKR_Light_Button.h"
#include "RKR_Box.h"
#include <FL/Fl_Group.H>
#include "sliderW.h"
#include "../process.h"
#include "rakarrack.h"
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>

class LooperGui : public Fl_Group {
public:
  LooperGui(int X, int Y, int W, int H, const char *L = 0);
  RKR_Light_Button *looper_activar;
private:
  inline void cb_looper_activar_i(RKR_Light_Button*, void*);
  static void cb_looper_activar(RKR_Light_Button*, void*);
public:
  Fl_Choice *looper_preset;
private:
  inline void cb_looper_preset_i(Fl_Choice*, void*);
  static void cb_looper_preset(Fl_Choice*, void*);
  static Fl_Menu_Item menu_looper_preset[];
public:
  SliderW *looper_WD;
private:
  inline void cb_looper_WD_i(SliderW*, void*);
  static void cb_looper_WD(SliderW*, void*);
public:
  SliderW *looper_level1;
private:
  inline void cb_looper_level1_i(SliderW*, void*);
  static void cb_looper_level1(SliderW*, void*);
public:
  SliderW *looper_level2;
private:
  inline void cb_looper_level2_i(SliderW*, void*);
  static void cb_looper_level2(SliderW*, void*);
public:
  SliderW *looper_Tempo;
private:
  inline void cb_looper_Tempo_i(SliderW*, void*);
  static void cb_looper_Tempo(SliderW*, void*);
public:
  Fl_Choice *looper_bar;
private:
  inline void cb_looper_bar_i(Fl_Choice*, void*);
  static void cb_looper_bar(Fl_Choice*, void*);
public:
  Fl_Choice *looper_ms;
private:
  inline void cb_looper_ms_i(Fl_Choice*, void*);
  static void cb_looper_ms(Fl_Choice*, void*);
public:
  Fl_Check_Button *looper_rv;
private:
  inline void cb_looper_rv_i(Fl_Check_Button*, void*);
  static void cb_looper_rv(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *looper_ap;
private:
  inline void cb_looper_ap_i(Fl_Check_Button*, void*);
  static void cb_looper_ap(Fl_Check_Button*, void*);
public:
  RKR_Box *Box_Play;
  RKR_Box *Box_P;
  RKR_Button *looper_play;
private:
  inline void cb_looper_play_i(RKR_Button*, void*);
  static void cb_looper_play(RKR_Button*, void*);
public:
  RKR_Button *looper_stop;
private:
  inline void cb_looper_stop_i(RKR_Button*, void*);
  static void cb_looper_stop(RKR_Button*, void*);
public:
  RKR_Button *looper_record;
private:
  inline void cb_looper_record_i(RKR_Button*, void*);
  static void cb_looper_record(RKR_Button*, void*);
public:
  Fl_Check_Button *looper_r1;
private:
  inline void cb_looper_r1_i(Fl_Check_Button*, void*);
  static void cb_looper_r1(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *looper_r2;
private:
  inline void cb_looper_r2_i(Fl_Check_Button*, void*);
  static void cb_looper_r2(Fl_Check_Button*, void*);
public:
  RKR_Button *looper_t1;
private:
  inline void cb_looper_t1_i(RKR_Button*, void*);
  static void cb_looper_t1(RKR_Button*, void*);
public:
  RKR_Button *looper_t2;
private:
  inline void cb_looper_t2_i(RKR_Button*, void*);
  static void cb_looper_t2(RKR_Button*, void*);
public:
  Fl_Check_Button *looper_lnk;
private:
  inline void cb_looper_lnk_i(Fl_Check_Button*, void*);
  static void cb_looper_lnk(Fl_Check_Button*, void*);
public:
  RKR_Button *looper_clear;
private:
  inline void cb_looper_clear_i(RKR_Button*, void*);
  static void cb_looper_clear(RKR_Button*, void*);
public:
  Fl_Check_Button *looper_mt;
private:
  inline void cb_looper_mt_i(Fl_Check_Button*, void*);
  static void cb_looper_mt(Fl_Check_Button*, void*);
public:
  RKR_Box *L_TimePos;
private:
  CommonGuiMenu *m_looper_bar; 
  CommonGuiMenu *m_looper_ms; 
};
#endif
