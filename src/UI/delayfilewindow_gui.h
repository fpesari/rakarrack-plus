// generated by Fast Light User Interface Designer (fluid) version 1.0304

#ifndef delayfilewindow_gui_h
#define delayfilewindow_gui_h
#include <FL/Fl.H>
#include <FL/Fl_File_Chooser.H>
#include <unistd.h>
#include "RKR_Button.h"
#include "RKR_Group.h"
#include "RKR_Scroll.h"
#include "RKR_Value_Input.h"
#include "../Echotron.h"
#include "rakarrack.h"
struct DelayLine
{
    float pan;
    float time;
    float level;
    float LP;
    float BP;
    float HP;
    float freq;
    float Q;
    int stages;
    
    DelayLine() :
    pan(),
    time(1.0),
    level(0.7),
    LP(1.0),
    BP(-1.0),
    HP(1.0),
    freq(800.0),
    Q(2.0),
    stages(0) {}
}; 
enum
{
    DELETE_LINE = 0,
    INSERT_BEFORE,
    MOVE_UP,
    MOVE_DOWN
}; 
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Scroll.H>

class DelayFileWindowGui : public Fl_Double_Window {
  void _DelayFileWindowGui();
public:
  DelayFileWindowGui(int X, int Y, int W, int H, const char *L = 0);
  DelayFileWindowGui(int W, int H, const char *L = 0);
  DelayFileWindowGui();
private:
  int m_file_size; 
  RKRGUI* m_rgui; 
  RKR* m_rkr; 
  DlyFile m_delay_file; 
public:
  RKR_Group *Delay_Group;
  RKR_Value_Input *dly_filter;
  RKR_Value_Input *dly_delay;
  RKR_Value_Input *dly_Q_mode;
private:
  inline void cb_Load_i(RKR_Button*, void*);
  static void cb_Load(RKR_Button*, void*);
  inline void cb_Save_i(RKR_Button*, void*);
  static void cb_Save(RKR_Button*, void*);
  inline void cb_New_i(RKR_Button*, void*);
  static void cb_New(RKR_Button*, void*);
public:
  RKR_Button *add_button;
private:
  inline void cb_add_button_i(RKR_Button*, void*);
  static void cb_add_button(RKR_Button*, void*);
public:
  RKR_Group *dly_group;
  Fl_Scroll *dly_scroll;
  void make_delay_window();
  void initialize(RKR *_rkr,RKRGUI *_rgui);
private:
  void load_delay_file(DlyFile delay_file);
public:
  void save_delay_file(char *filename);
private:
  DlyFile get_current_settings();
public:
  void update_scroll(int group, int type);
  void reorder_delay_lines(std::vector<DelayLine> &vector_delay_line, int line);
  int get_file_size();
};
#include <FL/Fl_Group.H>

class dlyFileGroup : public Fl_Group {
public:
  dlyFileGroup(int X, int Y, int W, int H, const char *L = 0);
private:
  DelayFileWindowGui *m_parent; 
public:
  RKR_Value_Input *dly_pan;
  RKR_Value_Input *dly_time;
  RKR_Value_Input *dly_level;
  RKR_Value_Input *dly_LP;
  RKR_Value_Input *dly_BP;
  RKR_Value_Input *dly_HP;
  RKR_Value_Input *dly_freq;
  RKR_Value_Input *dly_Q;
  RKR_Value_Input *dly_stages;
  RKR_Button *dly_up;
private:
  inline void cb_dly_up_i(RKR_Button*, void*);
  static void cb_dly_up(RKR_Button*, void*);
public:
  RKR_Button *dly_down;
private:
  inline void cb_dly_down_i(RKR_Button*, void*);
  static void cb_dly_down(RKR_Button*, void*);
public:
  RKR_Button *dly_delete;
private:
  inline void cb_dly_delete_i(RKR_Button*, void*);
  static void cb_dly_delete(RKR_Button*, void*);
public:
  RKR_Box *dly_occur;
  RKR_Button *dly_insert;
private:
  inline void cb_dly_insert_i(RKR_Button*, void*);
  static void cb_dly_insert(RKR_Button*, void*);
public:
  void initialize(DelayFileWindowGui *parent);
};
#endif
