// generated by Fast Light User Interface Designer (fluid) version 1.0304

#ifndef midiconv_gui_h
#define midiconv_gui_h
#include <FL/Fl.H>
#include "RKR_Light_Button.h"
#include "RKR_Check_Button.h"
#include "RKR_Choice.h"
#include "RKR_Counter.h"
#include <FL/Fl_Group.H>
#include "sliderW.h"
#include "../process.h"
#include "rakarrack.h"
#include <FL/Fl_Box.H>

class MidiGui : public Fl_Group {
public:
  MidiGui(int X, int Y, int W, int H, const char *L = 0);
  RKR_Light_Button *midi_activar;
private:
  inline void cb_midi_activar_i(RKR_Light_Button*, void*);
  static void cb_midi_activar(RKR_Light_Button*, void*);
public:
  RKR_Choice *MIDIOctave;
private:
  inline void cb_MIDIOctave_i(RKR_Choice*, void*);
  static void cb_MIDIOctave(RKR_Choice*, void*);
  static Fl_Menu_Item menu_MIDIOctave[];
public:
  RKR_Counter *Midi_out_Counter;
private:
  inline void cb_Midi_out_Counter_i(RKR_Counter*, void*);
  static void cb_Midi_out_Counter(RKR_Counter*, void*);
public:
  RKR_Check_Button *Use_FFT;
private:
  inline void cb_Use_FFT_i(RKR_Check_Button*, void*);
  static void cb_Use_FFT(RKR_Check_Button*, void*);
public:
  SliderW *Trig_Adj;
private:
  inline void cb_Trig_Adj_i(SliderW*, void*);
  static void cb_Trig_Adj(SliderW*, void*);
public:
  SliderW *Vel_Adj;
private:
  inline void cb_Vel_Adj_i(SliderW*, void*);
  static void cb_Vel_Adj(SliderW*, void*);
public:
  Fl_Box *Mled;
  RKR_Box *MIDI_LABEL;
private:
  RKR  *m_rkr; 
  RKRGUI *m_rgui; 
public:
  void initialize(RKR* _rkr, RKRGUI* _rgui);
};
#endif
