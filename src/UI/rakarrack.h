// generated by Fast Light User Interface Designer (fluid) version 1.0304

#ifndef rakarrack_h
#define rakarrack_h
#include <FL/Fl.H>
#include "../process.h"
#include "../global.h"
#include "sliderW.h"
#include "newvum.h"
#include "tunerbar.h"
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Tooltip.H>
#include <FL/x.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Tiled_Image.H>
#include <FL/Fl_Help_Dialog.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Color_Chooser.H>
#include <X11/xpm.h>
class RKR;  // forward declaration
class Scope;  // forward declaration
class NewVum;  // forward declaration
class Analyzer;  // forward declaration
class TunerBar;  // forward declaration
class EqGui; // forward declaration
class CompressGui; // forward declaration
class DistGui; // forward declaration
class OvrdGui; // forward declaration
class EchoGui; // forward declaration
class ChorusGui; // forward declaration
class PhaserGui; // forward declaration
class FlangerGui; // forward declaration
class ReverbGui; // forward declaration
class PeqGui; // forward declaration
class WhawhaGui; // forward declaration
class AlienwahGui; // forward declaration
class CabinetGui; // forward declaration
class PanGui; // forward declaration
class HarGui; // forward declaration
class MusdelayGui; // forward declaration
class GateGui; // forward declaration
class NewdistGui; // forward declaration
class AphaserGui; // forward declaration
class ValveGui; // forward declaration
class DflangeGui; // forward declaration
class RingGui; // forward declaration
class ExciterGui; // forward declaration
class MbdistGui; // forward declaration
class ArpieGui; // forward declaration
class ExpanderGui; // forward declaration
class ShuffleGui; // forward declaration
class SynthfilterGui; // forward declaration
class MbvvolGui; // forward declaration
class ConvoGui; // forward declaration
class LooperGui; // forward declaration
class RyanwahGui; // forward declaration
class RbechoGui; // forward declaration
class CoilGui; // forward declaration
class ShelfGui; // forward declaration
class VocoderGui; // forward declaration
class SustainGui; // forward declaration
class SequenceGui; // forward declaration
class ShifterGui; // forward declaration
class StompboxGui; // forward declaration
class RevtronGui; // forward declaration
class EchotronGui; // forward declaration
class SharGui; // forward declaration
class CbandGui; // forward declaration
class OtremGui; // forward declaration
class VibeGui; // forward declaration
class InfinityGui; // forward declaration
class MidiGui; // forward declaration
class BankWindowGui; // forward declaration
class OrderWindowGui; // forward declaration
class MidiLearnWindowGui; // forward declaration
class AboutWindowGui; // forward declaration
class TrigWindowGui; // forward declaration
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Button.H>
#include "scope.h"
#include "eq_gui.h"
#include "compress_gui.h"
#include "dist_gui.h"
#include "ovrd_gui.h"
#include "echo_gui.h"
#include "chorus_gui.h"
#include "phaser_gui.h"
#include "flanger_gui.h"
#include "reverb_gui.h"
#include "peq_gui.h"
#include "whawha_gui.h"
#include "alienwah_gui.h"
#include "cabinet_gui.h"
#include "pan_gui.h"
#include "har_gui.h"
#include "musdelay_gui.h"
#include "gate_gui.h"
#include "newdist_gui.h"
#include "aphaser_gui.h"
#include "valve_gui.h"
#include "dflange_gui.h"
#include "ring_gui.h"
#include "exciter_gui.h"
#include "mbdist_gui.h"
#include "arpie_gui.h"
#include "expander_gui.h"
#include "shuffle_gui.h"
#include "synthfilter_gui.h"
#include "mbvvol_gui.h"
#include "convo_gui.h"
#include "looper_gui.h"
#include "ryanwah_gui.h"
#include "rbecho_gui.h"
#include "coil_gui.h"
#include "shelf_gui.h"
#include "vocoder_gui.h"
#include "sustain_gui.h"
#include "sequence_gui.h"
#include "shifter_gui.h"
#include "stompbox_gui.h"
#include "reverbtron_gui.h"
#include "echotron_gui.h"
#include "stereoharm_gui.h"
#include "compband_gui.h"
#include "otrem_gui.h"
#include "vibe_gui.h"
#include "infinity_gui.h"
#include <FL/Fl_Group.H>
#include <FL/Fl_Light_Button.H>
#include "midiconv_gui.h"
#include <FL/Fl_Choice.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Counter.H>
#include "analyzer.h"
#include <FL/Fl_Value_Input.H>
#include "bankwindow_gui.h"
#include "orderwindow_gui.h"
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Scroll.H>
#include "midilearnwindow_gui.h"
#include "aboutwindow_gui.h"
#include "trigwindow_gui.h"

class RKRGUI {
  int made; 
  Fl_Help_Dialog *visor; 
  struct list_element *mBankNameList;
  struct list_element *mBankNameListTail;
public:
  void make_window();
  Fl_Double_Window *Principal;
private:
  inline void cb_Principal_i(Fl_Double_Window*, void*);
  static void cb_Principal(Fl_Double_Window*, void*);
public:
  Fl_Box *fondo;
  Fl_Menu_Bar *MenuP;
  static Fl_Menu_Item menu_MenuP[];
  static Fl_Menu_Item *Archivo;
  static Fl_Menu_Item *Menu_New;
private:
  inline void cb_Menu_New_i(Fl_Menu_*, void*);
  static void cb_Menu_New(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Load_Preset;
private:
  inline void cb_Load_Preset_i(Fl_Menu_*, void*);
  static void cb_Load_Preset(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Save_Preset;
private:
  inline void cb_Save_Preset_i(Fl_Menu_*, void*);
  static void cb_Save_Preset(Fl_Menu_*, void*);
  inline void cb_BankWindow_i(Fl_Menu_*, void*);
  static void cb_BankWindow(Fl_Menu_*, void*);
  inline void cb_BankWindow1_i(Fl_Menu_*, void*);
  static void cb_BankWindow1(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Load_Skin;
private:
  inline void cb_Load_Skin_i(Fl_Menu_*, void*);
  static void cb_Load_Skin(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Save_Skin;
private:
  inline void cb_Save_Skin_i(Fl_Menu_*, void*);
  static void cb_Save_Skin(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Load_MTable;
private:
  inline void cb_Load_MTable_i(Fl_Menu_*, void*);
  static void cb_Load_MTable(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Save_MTable;
private:
  inline void cb_Save_MTable_i(Fl_Menu_*, void*);
  static void cb_Save_MTable(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *ConvertReverb;
private:
  inline void cb_ConvertReverb_i(Fl_Menu_*, void*);
  static void cb_ConvertReverb(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *ImportPresets;
private:
  inline void cb_ImportPresets_i(Fl_Menu_*, void*);
  static void cb_ImportPresets(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *salir;
private:
  inline void cb_salir_i(Fl_Menu_*, void*);
  static void cb_salir(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Bank_Menu;
private:
  inline void cb_Bank_Menu_i(Fl_Menu_*, void*);
  static void cb_Bank_Menu(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Ajustes;
private:
  inline void cb_Ajustes_i(Fl_Menu_*, void*);
  static void cb_Ajustes(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *ML_Menu;
private:
  inline void cb_ML_Menu_i(Fl_Menu_*, void*);
  static void cb_ML_Menu(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *ACI_Menu;
private:
  inline void cb_ACI_Menu_i(Fl_Menu_*, void*);
  static void cb_ACI_Menu(Fl_Menu_*, void*);
  inline void cb_Fullscreen_i(Fl_Menu_*, void*);
  static void cb_Fullscreen(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Ayuda;
  static Fl_Menu_Item *Contenido;
private:
  inline void cb_Contenido_i(Fl_Menu_*, void*);
  static void cb_Contenido(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *Acerca_de;
private:
  inline void cb_Acerca_de_i(Fl_Menu_*, void*);
  static void cb_Acerca_de(Fl_Menu_*, void*);
public:
  Fl_Box *MT;
private:
  inline void cb_MT_i(Fl_Box*, void*);
  static void cb_MT(Fl_Box*, void*);
public:
  Fl_Button *TITTLE_L;
private:
  inline void cb_TITTLE_L_i(Fl_Button*, void*);
  static void cb_TITTLE_L(Fl_Button*, void*);
public:
  Fl_Box *LMT_LED;
  Fl_Box *CLIP_LED;
  Fl_Box *UPS_LED;
  Fl_Box *P_MIN_ST;
  Fl_Box *P_MOUT_ST;
  Fl_Box *P_AUX_ST;
  Fl_Box *P_IN_ST;
  Fl_Box *P_OUT_ST;
  Fl_Box *CPULOAD;
  Scope *Sco;
  Fl_Box *E1;
  Fl_Box *E2;
  Fl_Box *E3;
  Fl_Box *E4;
  Fl_Box *E5;
  Fl_Box *E6;
  Fl_Box *E7;
  Fl_Box *E8;
  Fl_Box *E9;
  Fl_Box *E10;
  Fl_Box *L1;
  Fl_Box *L2;
  Fl_Box *L3;
  Fl_Box *L4;
  Fl_Box *L5;
  Fl_Box *L6;
  Fl_Box *L7;
  Fl_Box *L8;
  Fl_Box *L9;
  Fl_Box *L10;
  EqGui *EQ;
  CompressGui *COMPRESS;
  DistGui *DIST;
  OvrdGui *OVRD;
  EchoGui *ECHO;
  ChorusGui *CHORUS;
  PhaserGui *PHASER;
  FlangerGui *FLANGER;
  ReverbGui *REVERB;
  PeqGui *PEQ;
  WhawhaGui *WHAWHA;
  AlienwahGui *ALIENWAH;
  CabinetGui *CABINET;
  PanGui *PAN;
  HarGui *HAR;
  MusdelayGui *MUSDELAY;
  GateGui *GATE;
  NewdistGui *NEWDIST;
  AphaserGui *APHASER;
  ValveGui *VALVE;
  DflangeGui *DFLANGE;
  RingGui *RING;
  ExciterGui *EXCITER;
  MbdistGui *MBDIST;
  ArpieGui *ARPIE;
  ExpanderGui *EXPANDER;
  ShuffleGui *SHUFFLE;
  SynthfilterGui *SYNTHFILTER;
  MbvvolGui *MBVVOL;
  ConvoGui *CONVOLOTRON;
  LooperGui *LOOPER;
  RyanwahGui *RYANWAH;
  RbechoGui *RBECHO;
  CoilGui *COILCRAFTER;
  ShelfGui *SHELFBOOST;
  VocoderGui *VOCODER;
  SustainGui *SUSTAINER;
  SequenceGui *SEQUENCE;
  ShifterGui *SHIFTER;
  StompboxGui *STOMPBOX;
  RevtronGui *REVERBTRON;
  EchotronGui *ECHOTRON;
  SharGui *SHAR;
  CbandGui *COMPBAND;
  OtremGui *OTREM;
  VibeGui *VIBE;
  InfinityGui *INFINIT;
  Fl_Group *Tuner;
  Fl_Light_Button *tuner_activar;
private:
  inline void cb_tuner_activar_i(Fl_Light_Button*, void*);
  static void cb_tuner_activar(Fl_Light_Button*, void*);
public:
  TunerBar *tuner_bar;
  Fl_Box *WNote;
  Fl_Box *WRfreq;
  Fl_Box *WNfreq;
  Fl_Box *PINCHO;
  Fl_Box *TUNER_LABEL;
  Fl_Group *InOut;
  Fl_Light_Button *ActivarGeneral;
private:
  inline void cb_ActivarGeneral_i(Fl_Light_Button*, void*);
  static void cb_ActivarGeneral(Fl_Light_Button*, void*);
public:
  Fl_Button *BostBut;
private:
  inline void cb_BostBut_i(Fl_Button*, void*);
  static void cb_BostBut(Fl_Button*, void*);
public:
  SliderW *Balance;
private:
  inline void cb_Balance_i(SliderW*, void*);
  static void cb_Balance(SliderW*, void*);
public:
  SliderW *Nivel_Entrada;
private:
  inline void cb_Nivel_Entrada_i(SliderW*, void*);
  static void cb_Nivel_Entrada(SliderW*, void*);
public:
  SliderW *Nivel_Salida;
private:
  inline void cb_Nivel_Salida_i(SliderW*, void*);
  static void cb_Nivel_Salida(SliderW*, void*);
public:
  NewVum *input_vul;
  NewVum *input_vur;
  NewVum *output_vul;
  NewVum *output_vur;
  Fl_Box *LABEL_IO;
  MidiGui *MIDI;
  Fl_Group *Metro;
  Fl_Light_Button *metro_activar;
private:
  inline void cb_metro_activar_i(Fl_Light_Button*, void*);
  static void cb_metro_activar(Fl_Light_Button*, void*);
public:
  Fl_Choice *MetroBar;
private:
  inline void cb_MetroBar_i(Fl_Choice*, void*);
  static void cb_MetroBar(Fl_Choice*, void*);
public:
  SliderW *Metro_Volume;
private:
  inline void cb_Metro_Volume_i(SliderW*, void*);
  static void cb_Metro_Volume(SliderW*, void*);
public:
  Fl_Choice *MetroSound;
private:
  inline void cb_MetroSound_i(Fl_Choice*, void*);
  static void cb_MetroSound(Fl_Choice*, void*);
public:
  SliderW *Metro_Tempo;
private:
  inline void cb_Metro_Tempo_i(SliderW*, void*);
  static void cb_Metro_Tempo(SliderW*, void*);
public:
  Fl_Box *Metro_Led;
  Fl_Box *Metro_Label;
  Fl_Group *Presets;
  Fl_Box *PRESETS_LABEL;
  Fl_Button *L_B1;
private:
  inline void cb_L_B1_i(Fl_Button*, void*);
  static void cb_L_B1(Fl_Button*, void*);
public:
  Fl_Button *L_B2;
private:
  inline void cb_L_B2_i(Fl_Button*, void*);
  static void cb_L_B2(Fl_Button*, void*);
public:
  Fl_Button *L_B3;
private:
  inline void cb_L_B3_i(Fl_Button*, void*);
  static void cb_L_B3(Fl_Button*, void*);
public:
  Fl_Button *L_B4;
private:
  inline void cb_L_B4_i(Fl_Button*, void*);
  static void cb_L_B4(Fl_Button*, void*);
public:
  Fl_Button *S_new;
private:
  inline void cb_S_new_i(Fl_Button*, void*);
  static void cb_S_new(Fl_Button*, void*);
public:
  Fl_Button *L_preset;
private:
  inline void cb_L_preset_i(Fl_Button*, void*);
  static void cb_L_preset(Fl_Button*, void*);
public:
  Fl_Button *S_preset;
private:
  inline void cb_S_preset_i(Fl_Button*, void*);
  static void cb_S_preset(Fl_Button*, void*);
public:
  Fl_Light_Button *Compare;
private:
  inline void cb_Compare_i(Fl_Light_Button*, void*);
  static void cb_Compare(Fl_Light_Button*, void*);
public:
  Fl_Button *B_preset;
private:
  inline void cb_B_preset_i(Fl_Button*, void*);
  static void cb_B_preset(Fl_Button*, void*);
public:
  Fl_Input *WPreset_Name;
private:
  inline void cb_WPreset_Name_i(Fl_Input*, void*);
  static void cb_WPreset_Name(Fl_Input*, void*);
public:
  Fl_Counter *Preset_Counter;
private:
  inline void cb_Preset_Counter_i(Fl_Counter*, void*);
  static void cb_Preset_Counter(Fl_Counter*, void*);
public:
  Fl_Box *DAuthor;
  Fl_Button *RandomP;
private:
  inline void cb_RandomP_i(Fl_Button*, void*);
  static void cb_RandomP(Fl_Button*, void*);
public:
  Fl_Button *Open_Order;
private:
  inline void cb_Open_Order_i(Fl_Button*, void*);
  static void cb_Open_Order(Fl_Button*, void*);
public:
  Fl_Button *Etit;
private:
  inline void cb_Etit_i(Fl_Button*, void*);
  static void cb_Etit(Fl_Button*, void*);
public:
  Analyzer *Analy;
  Fl_Button *HideUE;
private:
  inline void cb_HideUE_i(Fl_Button*, void*);
  static void cb_HideUE(Fl_Button*, void*);
public:
  Fl_Button *SwitchMod;
private:
  inline void cb_SwitchMod_i(Fl_Button*, void*);
  static void cb_SwitchMod(Fl_Button*, void*);
public:
  Fl_Group *Tap;
  Fl_Light_Button *Tap_activar;
private:
  inline void cb_Tap_activar_i(Fl_Light_Button*, void*);
  static void cb_Tap_activar(Fl_Light_Button*, void*);
public:
  Fl_Choice *T_SEL;
private:
  inline void cb_T_SEL_i(Fl_Choice*, void*);
  static void cb_T_SEL(Fl_Choice*, void*);
  static Fl_Menu_Item menu_T_SEL[];
public:
  Fl_Choice *T_SET;
private:
  inline void cb_T_SET_i(Fl_Choice*, void*);
  static void cb_T_SET(Fl_Choice*, void*);
  static Fl_Menu_Item menu_T_SET[];
public:
  Fl_Button *T_Apply;
private:
  inline void cb_T_Apply_i(Fl_Button*, void*);
  static void cb_T_Apply(Fl_Button*, void*);
public:
  Fl_Button *T_BUT;
private:
  inline void cb_T_BUT_i(Fl_Button*, void*);
  static void cb_T_BUT(Fl_Button*, void*);
public:
  Fl_Box *TAP_LABEL;
  Fl_Value_Input *T_DIS;
private:
  inline void cb_T_DIS_i(Fl_Value_Input*, void*);
  static void cb_T_DIS(Fl_Value_Input*, void*);
public:
  BankWindowGui *BankWindow;
private:
  inline void cb_BankWindow2_i(BankWindowGui*, void*);
  static void cb_BankWindow2(BankWindowGui*, void*);
public:
  OrderWindowGui *Order;
private:
  inline void cb_Order_i(OrderWindowGui*, void*);
  static void cb_Order(OrderWindowGui*, void*);
public:
  Fl_Double_Window *Settings;
private:
  inline void cb_Settings_i(Fl_Double_Window*, void*);
  static void cb_Settings(Fl_Double_Window*, void*);
public:
  Fl_Box *Fondo2;
  Fl_Tabs *STabs;
  Fl_Group *Look;
  Fl_Box *Fondo6;
  Fl_Choice *scheme_ch;
private:
  inline void cb_scheme_ch_i(Fl_Choice*, void*);
  static void cb_scheme_ch(Fl_Choice*, void*);
  static Fl_Menu_Item menu_scheme_ch[];
public:
  Fl_Browser *Font_Bro;
private:
  inline void cb_Font_Bro_i(Fl_Browser*, void*);
  static void cb_Font_Bro(Fl_Browser*, void*);
public:
  Fl_Box *FSLabel;
  Fl_Button *FSless;
private:
  inline void cb_FSless_i(Fl_Button*, void*);
  static void cb_FSless(Fl_Button*, void*);
public:
  Fl_Button *FSplus;
private:
  inline void cb_FSplus_i(Fl_Button*, void*);
  static void cb_FSplus(Fl_Button*, void*);
public:
  Fl_Button *FSReset;
private:
  inline void cb_FSReset_i(Fl_Button*, void*);
  static void cb_FSReset(Fl_Button*, void*);
public:
  Fl_Box *CLLabel;
  Fl_Button *B_C;
private:
  inline void cb_B_C_i(Fl_Button*, void*);
  static void cb_B_C(Fl_Button*, void*);
public:
  Fl_Button *A_C;
private:
  inline void cb_A_C_i(Fl_Button*, void*);
  static void cb_A_C(Fl_Button*, void*);
public:
  Fl_Button *L_C;
private:
  inline void cb_L_C_i(Fl_Button*, void*);
  static void cb_L_C(Fl_Button*, void*);
public:
  Fl_Button *K_C;
private:
  inline void cb_K_C_i(Fl_Button*, void*);
  static void cb_K_C(Fl_Button*, void*);
public:
  Fl_Check_Button *Enable_Back;
private:
  inline void cb_Enable_Back_i(Fl_Check_Button*, void*);
  static void cb_Enable_Back(Fl_Check_Button*, void*);
public:
  Fl_File_Input *BackFiname;
  Fl_Button *BI_Browser;
private:
  inline void cb_BI_Browser_i(Fl_Button*, void*);
  static void cb_BI_Browser(Fl_Button*, void*);
public:
  Fl_Check_Button *Enable_DeacHide;
private:
  inline void cb_Enable_DeacHide_i(Fl_Check_Button*, void*);
  static void cb_Enable_DeacHide(Fl_Check_Button*, void*);
public:
  Fl_Group *AUDIO_SET;
  Fl_Box *Fondo7;
  Fl_Check_Button *INSTATE;
private:
  inline void cb_INSTATE_i(Fl_Check_Button*, void*);
  static void cb_INSTATE(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *Filter_DC;
private:
  inline void cb_Filter_DC_i(Fl_Check_Button*, void*);
  static void cb_Filter_DC(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *Pre_Serve;
private:
  inline void cb_Pre_Serve_i(Fl_Check_Button*, void*);
  static void cb_Pre_Serve(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *Update_TAP;
private:
  inline void cb_Update_TAP_i(Fl_Check_Button*, void*);
  static void cb_Update_TAP(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *UPSAMPLE_C;
private:
  inline void cb_UPSAMPLE_C_i(Fl_Check_Button*, void*);
  static void cb_UPSAMPLE_C(Fl_Check_Button*, void*);
public:
  Fl_Choice *Upr_Amo;
private:
  inline void cb_Upr_Amo_i(Fl_Choice*, void*);
  static void cb_Upr_Amo(Fl_Choice*, void*);
  static Fl_Menu_Item menu_Upr_Amo[];
public:
  Fl_Choice *Upr_Qual;
private:
  inline void cb_Upr_Qual_i(Fl_Choice*, void*);
  static void cb_Upr_Qual(Fl_Choice*, void*);
  static Fl_Menu_Item menu_Upr_Qual[];
public:
  Fl_Choice *Downr_Qual;
private:
  inline void cb_Downr_Qual_i(Fl_Choice*, void*);
  static void cb_Downr_Qual(Fl_Choice*, void*);
public:
  Fl_Counter *L_SIZE;
private:
  inline void cb_L_SIZE_i(Fl_Counter*, void*);
  static void cb_L_SIZE(Fl_Counter*, void*);
public:
  Fl_Counter *LM_Volume;
private:
  inline void cb_LM_Volume_i(Fl_Counter*, void*);
  static void cb_LM_Volume(Fl_Counter*, void*);
public:
  Fl_Choice *Har_Qual;
private:
  inline void cb_Har_Qual_i(Fl_Choice*, void*);
  static void cb_Har_Qual(Fl_Choice*, void*);
  static Fl_Menu_Item menu_Har_Qual[];
public:
  Fl_Choice *Ste_Qual;
private:
  inline void cb_Ste_Qual_i(Fl_Choice*, void*);
  static void cb_Ste_Qual(Fl_Choice*, void*);
  static Fl_Menu_Item menu_Ste_Qual[];
public:
  Fl_Choice *Voc_Bands;
private:
  inline void cb_Voc_Bands_i(Fl_Choice*, void*);
  static void cb_Voc_Bands(Fl_Choice*, void*);
  static Fl_Menu_Item menu_Voc_Bands[];
public:
  Fl_Check_Button *FLPosition;
private:
  inline void cb_FLPosition_i(Fl_Check_Button*, void*);
  static void cb_FLPosition(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *DB6B;
private:
  inline void cb_DB6B_i(Fl_Check_Button*, void*);
  static void cb_DB6B(Fl_Check_Button*, void*);
public:
  Fl_Counter *Calibration;
private:
  inline void cb_Calibration_i(Fl_Counter*, void*);
  static void cb_Calibration(Fl_Counter*, void*);
public:
  Fl_Counter *RTrigger;
private:
  inline void cb_RTrigger_i(Fl_Counter*, void*);
  static void cb_RTrigger(Fl_Counter*, void*);
public:
  Fl_Choice *RC_Harm_Opti;
private:
  inline void cb_RC_Harm_Opti_i(Fl_Choice*, void*);
  static void cb_RC_Harm_Opti(Fl_Choice*, void*);
  static Fl_Menu_Item menu_RC_Harm_Opti[];
public:
  Fl_Choice *RC_St_Harm_Opti;
private:
  inline void cb_RC_St_Harm_Opti_i(Fl_Choice*, void*);
  static void cb_RC_St_Harm_Opti(Fl_Choice*, void*);
  static Fl_Menu_Item menu_RC_St_Harm_Opti[];
public:
  Fl_Choice *RC_Ring_Opti;
private:
  inline void cb_RC_Ring_Opti_i(Fl_Choice*, void*);
  static void cb_RC_Ring_Opti(Fl_Choice*, void*);
  static Fl_Menu_Item menu_RC_Ring_Opti[];
public:
  Fl_Choice *Har_Downsample;
private:
  inline void cb_Har_Downsample_i(Fl_Choice*, void*);
  static void cb_Har_Downsample(Fl_Choice*, void*);
  static Fl_Menu_Item menu_Har_Downsample[];
public:
  Fl_Choice *Har_Down_Qua;
private:
  inline void cb_Har_Down_Qua_i(Fl_Choice*, void*);
  static void cb_Har_Down_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Har_Up_Qua;
private:
  inline void cb_Har_Up_Qua_i(Fl_Choice*, void*);
  static void cb_Har_Up_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Rev_Downsample;
private:
  inline void cb_Rev_Downsample_i(Fl_Choice*, void*);
  static void cb_Rev_Downsample(Fl_Choice*, void*);
public:
  Fl_Choice *Rev_Down_Qua;
private:
  inline void cb_Rev_Down_Qua_i(Fl_Choice*, void*);
  static void cb_Rev_Down_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Rev_Up_Qua;
private:
  inline void cb_Rev_Up_Qua_i(Fl_Choice*, void*);
  static void cb_Rev_Up_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Con_Downsample;
private:
  inline void cb_Con_Downsample_i(Fl_Choice*, void*);
  static void cb_Con_Downsample(Fl_Choice*, void*);
public:
  Fl_Choice *Con_Down_Qua;
private:
  inline void cb_Con_Down_Qua_i(Fl_Choice*, void*);
  static void cb_Con_Down_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Con_Up_Qua;
private:
  inline void cb_Con_Up_Qua_i(Fl_Choice*, void*);
  static void cb_Con_Up_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Seq_Downsample;
private:
  inline void cb_Seq_Downsample_i(Fl_Choice*, void*);
  static void cb_Seq_Downsample(Fl_Choice*, void*);
public:
  Fl_Choice *Seq_Down_Qua;
private:
  inline void cb_Seq_Down_Qua_i(Fl_Choice*, void*);
  static void cb_Seq_Down_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Seq_Up_Qua;
private:
  inline void cb_Seq_Up_Qua_i(Fl_Choice*, void*);
  static void cb_Seq_Up_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Shi_Downsample;
private:
  inline void cb_Shi_Downsample_i(Fl_Choice*, void*);
  static void cb_Shi_Downsample(Fl_Choice*, void*);
public:
  Fl_Choice *Shi_Down_Qua;
private:
  inline void cb_Shi_Down_Qua_i(Fl_Choice*, void*);
  static void cb_Shi_Down_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Shi_Up_Qua;
private:
  inline void cb_Shi_Up_Qua_i(Fl_Choice*, void*);
  static void cb_Shi_Up_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Voc_Downsample;
private:
  inline void cb_Voc_Downsample_i(Fl_Choice*, void*);
  static void cb_Voc_Downsample(Fl_Choice*, void*);
public:
  Fl_Choice *Voc_Down_Qua;
private:
  inline void cb_Voc_Down_Qua_i(Fl_Choice*, void*);
  static void cb_Voc_Down_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Voc_Up_Qua;
private:
  inline void cb_Voc_Up_Qua_i(Fl_Choice*, void*);
  static void cb_Voc_Up_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Ste_Downsample;
private:
  inline void cb_Ste_Downsample_i(Fl_Choice*, void*);
  static void cb_Ste_Downsample(Fl_Choice*, void*);
public:
  Fl_Choice *Ste_Down_Qua;
private:
  inline void cb_Ste_Down_Qua_i(Fl_Choice*, void*);
  static void cb_Ste_Down_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Ste_Up_Qua;
private:
  inline void cb_Ste_Up_Qua_i(Fl_Choice*, void*);
  static void cb_Ste_Up_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Wave_Amo;
private:
  inline void cb_Wave_Amo_i(Fl_Choice*, void*);
  static void cb_Wave_Amo(Fl_Choice*, void*);
  static Fl_Menu_Item menu_Wave_Amo[];
public:
  Fl_Choice *Wave_Down_Qua;
private:
  inline void cb_Wave_Down_Qua_i(Fl_Choice*, void*);
  static void cb_Wave_Down_Qua(Fl_Choice*, void*);
public:
  Fl_Choice *Wave_Up_Qua;
private:
  inline void cb_Wave_Up_Qua_i(Fl_Choice*, void*);
  static void cb_Wave_Up_Qua(Fl_Choice*, void*);
public:
  Fl_Box *Recognize_Label;
  Fl_Group *MIDI_SET;
  Fl_Box *Fondo8;
  Fl_Check_Button *D_A_Connect;
private:
  inline void cb_D_A_Connect_i(Fl_Check_Button*, void*);
  static void cb_D_A_Connect(Fl_Check_Button*, void*);
public:
  Fl_Browser *BMidiIn;
private:
  inline void cb_BMidiIn_i(Fl_Browser*, void*);
  static void cb_BMidiIn(Fl_Browser*, void*);
public:
  Fl_Counter *Midi_In_Counter;
private:
  inline void cb_Midi_In_Counter_i(Fl_Counter*, void*);
  static void cb_Midi_In_Counter(Fl_Counter*, void*);
public:
  Fl_Counter *Har_In_Counter;
private:
  inline void cb_Har_In_Counter_i(Fl_Counter*, void*);
  static void cb_Har_In_Counter(Fl_Counter*, void*);
public:
  Fl_Counter *Stereo_Har_In_Counter;
private:
  inline void cb_Stereo_Har_In_Counter_i(Fl_Counter*, void*);
  static void cb_Stereo_Har_In_Counter(Fl_Counter*, void*);
public:
  Fl_Group *wMIDI;
  Fl_Check_Button *Mw0;
private:
  inline void cb_Mw0_i(Fl_Check_Button*, void*);
  static void cb_Mw0(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *Mw1;
private:
  inline void cb_Mw1_i(Fl_Check_Button*, void*);
  static void cb_Mw1(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *AAssign;
private:
  inline void cb_AAssign_i(Fl_Check_Button*, void*);
  static void cb_AAssign(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *MTable;
private:
  inline void cb_MTable_i(Fl_Check_Button*, void*);
  static void cb_MTable(Fl_Check_Button*, void*);
public:
  Fl_Scroll *scroll;
  Fl_Group *JACK_SET;
  Fl_Box *Fondo9;
  Fl_Check_Button *D_J_Connect;
private:
  inline void cb_D_J_Connect_i(Fl_Check_Button*, void*);
  static void cb_D_J_Connect(Fl_Check_Button*, void*);
public:
  Fl_Browser *JackCo;
  Fl_Check_Button *D_IJ_Connect;
private:
  inline void cb_D_IJ_Connect_i(Fl_Check_Button*, void*);
  static void cb_D_IJ_Connect(Fl_Check_Button*, void*);
public:
  Fl_Browser *JackIn;
  Fl_Group *MISC_SET;
  Fl_Box *Fondo10;
  Fl_Input *Username;
private:
  inline void cb_Username_i(Fl_Input*, void*);
  static void cb_Username(Fl_Input*, void*);
public:
  Fl_Check_Button *MES_DIS;
private:
  inline void cb_MES_DIS_i(Fl_Check_Button*, void*);
  static void cb_MES_DIS(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *T_TIMEOUT;
private:
  inline void cb_T_TIMEOUT_i(Fl_Check_Button*, void*);
  static void cb_T_TIMEOUT(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *ENA_TOOL;
private:
  inline void cb_ENA_TOOL_i(Fl_Check_Button*, void*);
  static void cb_ENA_TOOL(Fl_Check_Button*, void*);
public:
  Fl_Group *BANK_SET;
  Fl_Box *Fondo11;
  Fl_File_Input *BFiname;
  Fl_Button *BF_Browser;
private:
  inline void cb_BF_Browser_i(Fl_Button*, void*);
  static void cb_BF_Browser(Fl_Button*, void*);
public:
  Fl_File_Input *Udir;
  Fl_Button *UD_Browser;
private:
  inline void cb_UD_Browser_i(Fl_Button*, void*);
  static void cb_UD_Browser(Fl_Button*, void*);
public:
  MidiLearnWindowGui *MIDILearn;
private:
  inline void cb_MIDILearn_i(MidiLearnWindowGui*, void*);
  static void cb_MIDILearn(MidiLearnWindowGui*, void*);
public:
  AboutWindowGui *AboutWin;
private:
  inline void cb_AboutWin_i(AboutWindowGui*, void*);
  static void cb_AboutWin(AboutWindowGui*, void*);
public:
  TrigWindowGui *Trigger;
private:
  inline void cb_Trigger_i(TrigWindowGui*, void*);
  static void cb_Trigger(TrigWindowGui*, void*);
public:
  RKRGUI(int argc, char**argv,RKR *rkr_);
  void Background_Color_Change(Fl_Color bcolor);
  void Label_Color_Change(Fl_Color bcolor);
  void Buttons_Color_Change(Fl_Color bcolor);
  void Leds_Color_Change(Fl_Color bcolor);
  void put_icon(Fl_Window* window);
  void load_stat();
  void save_stat(int whati);
  void Put_Loaded();
  static void tick(void* v);
  static void preset_click(Fl_Button* o, void* v);
  inline void preset_click_i(Fl_Button* o, void*);
  void reordena();
  void show_help();
  void show_lic();
  void MiraClientes();
  void MiraConfig();
  void BankWin_Label(char *filename);
  void is_modified();
  void Put_Loaded_Bank();
  void Chord(int eff);
  void ActMIDI();
  void ActOnOff();
  void PutBackground();
  void chfsize(int value);
  void adjustfont();
  void ChangeActives();
  void findpos(int num, int value, Fl_Widget*);
  void Put_Skin(int last);
  void FillML(int type);
  void DisAssigns();
  void Prepare_Order();
  void Show_Next_Time();
  void update_looper();
  void UpdateTGUI();
  void ActACI();
  int Busca_Eff(int num);
  void Fill_Avail(int filter);
  void highlight();
  int search_but(int x, int y);
  void ScanDir();
  static int prevnext(int e);
  bool install_signal_handlers();
  static void sigterm_handler(int sig);
  static void check_signals(void *usrPtr);
  void getMIDIControl(int num);
  void PrepareML();
  inline void addpreset(Fl_Widget *w, int num);
  void add_name(Fl_Widget *w, char *name);
  Fl_Widget * FindWidget(int num);
  void ReadIntPresets();
  inline void delpreset(Fl_Widget *w, int num);
  void Prep_Reorden(int source, int dest);
  void make_table_window();
  static void bank_click(Fl_Choice* o, void* v);
  inline void bank_click_i(Fl_Choice* o, void*);
  static void p_click(Fl_Choice* o, void* v);
  inline void p_click_i(Fl_Choice* o, void*);
  void fill_mptable(int num,int value);
  void mtfillvalue(int num,int value);
  void Put_MidiTable();
  void RandomPreset();
  void drag_effect();
  int AddBankName(const char *name);
  void ClearBankNames();
  void set_bank_made(int a_made) {made = a_made;}; 
  int get_bank_made(){return made;}; 
};
#endif
