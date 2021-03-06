// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "musdelay_gui.h"

void MusdelayGui::cb_musdelay_activar_i(RKR_Light_Button* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Multi_On_Off);
 o->value(rkr->EFX_Bypass[EFX_MUSICAL_DELAY]);
 return;
}
rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=(int)o->value();
if((int) o->value()==0)
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->cleanup();
rgui->findpos(EFX_MUSICAL_DELAY,(int)o->value(),o);
}
void MusdelayGui::cb_musdelay_activar(RKR_Light_Button* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_activar_i(o,v);
}

void MusdelayGui::cb_musdelay_preset_i(RKR_Choice* o, void* v) {
  rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=0;
long long ud= (long long) v;
if((ud==0)||(ud==12015))rkr->Rack_Effects[EFX_MUSICAL_DELAY]->setpreset(o->value());
musdelay_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_DryWet)));
musdelay_tempo->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Tempo));
musdelay_pan1->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Pan_1)-64);
musdelay_pan2->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Pan_2)-64);
musdelay_delay1->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Delay_1)-1);
musdelay_delay2->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Delay_2)-1);
musdelay_delay3->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Del_Offset)-1);
musdelay_fb1->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Feedback_1));
musdelay_fb2->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Feedback_2));
musdelay_LRc->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_LR_Cross));
musdelay_gain1->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Gain_1));
musdelay_gain2->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Gain_2));
musdelay_damp->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Damp));
if((int)musdelay_activar->value())rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=1;
}
void MusdelayGui::cb_musdelay_preset(RKR_Choice* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_preset_i(o,v);
}

Fl_Menu_Item MusdelayGui::menu_musdelay_preset[] = {
 {"Echo 1", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Echo 2", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void MusdelayGui::cb_musdelay_WD_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_DryWet);
 return;
} 
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_DryWet,Dry_Wet((int)o->value()));
}
void MusdelayGui::cb_musdelay_WD(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_WD_i(o,v);
}

void MusdelayGui::cb_musdelay_LRc_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_LR_Cross);
 return;
} 
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_LR_Cross,(int)o->value());
}
void MusdelayGui::cb_musdelay_LRc(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_LRc_i(o,v);
}

void MusdelayGui::cb_musdelay_pan1_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_Pan_1);
 return;
} 
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Pan_1,(int)o->value()+64);
}
void MusdelayGui::cb_musdelay_pan1(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_pan1_i(o,v);
}

void MusdelayGui::cb_musdelay_pan2_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_Pan_2);
 return;
} 
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Pan_2,(int)o->value()+64);
}
void MusdelayGui::cb_musdelay_pan2(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_pan2_i(o,v);
}

void MusdelayGui::cb_musdelay_delay1_i(RKR_Choice* o, void*) {
  rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=0;
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Delay_1,o->value()+1);
if((int)musdelay_activar->value())rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=1;
}
void MusdelayGui::cb_musdelay_delay1(RKR_Choice* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_delay1_i(o,v);
}

Fl_Menu_Item MusdelayGui::menu_musdelay_delay1[] = {
 {"1", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/2", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/3", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/4", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/5", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/6", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void MusdelayGui::cb_musdelay_delay3_i(RKR_Choice* o, void*) {
  rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=0;
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Del_Offset,o->value()+1);
if((int)musdelay_activar->value())rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=1;
}
void MusdelayGui::cb_musdelay_delay3(RKR_Choice* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_delay3_i(o,v);
}

Fl_Menu_Item MusdelayGui::menu_musdelay_delay3[] = {
 {"1", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/2", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/3", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/4", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/5", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"1/6", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"0", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void MusdelayGui::cb_musdelay_delay2_i(RKR_Choice* o, void*) {
  rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=0;
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Delay_2,o->value()+1);
if((int)musdelay_activar->value())rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=1;
}
void MusdelayGui::cb_musdelay_delay2(RKR_Choice* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_delay2_i(o,v);
}

void MusdelayGui::cb_musdelay_tempo_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_Tempo);
 return;
}
rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=0;
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Tempo,(int)o->value());
if((int)musdelay_activar->value())rkr->EFX_Bypass[EFX_MUSICAL_DELAY]=1;
}
void MusdelayGui::cb_musdelay_tempo(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_tempo_i(o,v);
}

void MusdelayGui::cb_musdelay_gain1_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_Gain_1);
 return;
} 
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Gain_1,(int)o->value());
}
void MusdelayGui::cb_musdelay_gain1(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_gain1_i(o,v);
}

void MusdelayGui::cb_musdelay_gain2_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_Gain_2);
 return;
} 
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Gain_2,(int)o->value());
}
void MusdelayGui::cb_musdelay_gain2(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_gain2_i(o,v);
}

void MusdelayGui::cb_musdelay_fb1_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_Feedback_1);
 return;
} 
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Feedback_1,(int)o->value());
}
void MusdelayGui::cb_musdelay_fb1(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_fb1_i(o,v);
}

void MusdelayGui::cb_musdelay_fb2_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_Feedback_2);
 return;
} 
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Feedback_2,(int)o->value());
}
void MusdelayGui::cb_musdelay_fb2(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_fb2_i(o,v);
}

void MusdelayGui::cb_musdelay_damp_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Music_Damp);
 return;
}
rkr->Rack_Effects[EFX_MUSICAL_DELAY]->changepar(Music_Damp,(int)o->value());
}
void MusdelayGui::cb_musdelay_damp(RKR_Slider* o, void* v) {
  ((MusdelayGui*)(o->parent()))->cb_musdelay_damp_i(o,v);
}
MusdelayGui::MusdelayGui(int X, int Y, int W, int H, const char *L)
  : Fl_Group(0, 0, W, H, L) {
this->box(FL_UP_BOX);
this->color(FL_FOREGROUND_COLOR);
this->selection_color(FL_FOREGROUND_COLOR);
this->user_data((void*)(1));
this->align(Fl_Align(96|FL_ALIGN_INSIDE));
{ musdelay_activar = new RKR_Light_Button(5, 4, 34, 18, "On");
  musdelay_activar->box(FL_UP_BOX);
  musdelay_activar->shortcut(0x35);
  musdelay_activar->color((Fl_Color)62);
  musdelay_activar->selection_color((Fl_Color)1);
  musdelay_activar->labeltype(FL_NORMAL_LABEL);
  musdelay_activar->labelfont(0);
  musdelay_activar->labelsize(10);
  musdelay_activar->labelcolor(FL_FOREGROUND_COLOR);
  musdelay_activar->callback((Fl_Callback*)cb_musdelay_activar, (void*)(2));
  musdelay_activar->align(Fl_Align(68|FL_ALIGN_INSIDE));
  musdelay_activar->when(FL_WHEN_CHANGED);
} // RKR_Light_Button* musdelay_activar
{ musdelay_preset = new RKR_Choice(77, 4, 76, 18, "Preset");
  musdelay_preset->box(FL_FLAT_BOX);
  musdelay_preset->down_box(FL_BORDER_BOX);
  musdelay_preset->color(FL_BACKGROUND_COLOR);
  musdelay_preset->selection_color(FL_FOREGROUND_COLOR);
  musdelay_preset->labeltype(FL_NORMAL_LABEL);
  musdelay_preset->labelfont(0);
  musdelay_preset->labelsize(10);
  musdelay_preset->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_preset->textsize(10);
  musdelay_preset->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_preset->callback((Fl_Callback*)cb_musdelay_preset, (void*)(12015));
  musdelay_preset->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_preset->when(FL_WHEN_RELEASE_ALWAYS);
  musdelay_preset->menu(menu_musdelay_preset);
} // RKR_Choice* musdelay_preset
{ musdelay_WD = new RKR_Slider(56, 26, 100, 10, "Dry/Wet");
  musdelay_WD->type(5);
  musdelay_WD->box(FL_FLAT_BOX);
  musdelay_WD->color((Fl_Color)178);
  musdelay_WD->selection_color((Fl_Color)62);
  musdelay_WD->labeltype(FL_NORMAL_LABEL);
  musdelay_WD->labelfont(0);
  musdelay_WD->labelsize(10);
  musdelay_WD->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_WD->maximum(127);
  musdelay_WD->step(1);
  musdelay_WD->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_WD->callback((Fl_Callback*)cb_musdelay_WD);
  musdelay_WD->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_WD->when(FL_WHEN_CHANGED);
} // RKR_Slider* musdelay_WD
{ musdelay_LRc = new RKR_Slider(56, 38, 100, 10, "L/R Cross");
  musdelay_LRc->type(5);
  musdelay_LRc->box(FL_FLAT_BOX);
  musdelay_LRc->color((Fl_Color)178);
  musdelay_LRc->selection_color((Fl_Color)62);
  musdelay_LRc->labeltype(FL_NORMAL_LABEL);
  musdelay_LRc->labelfont(0);
  musdelay_LRc->labelsize(10);
  musdelay_LRc->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_LRc->maximum(127);
  musdelay_LRc->step(1);
  musdelay_LRc->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_LRc->callback((Fl_Callback*)cb_musdelay_LRc);
  musdelay_LRc->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_LRc->when(FL_WHEN_CHANGED);
} // RKR_Slider* musdelay_LRc
{ musdelay_pan1 = new RKR_Slider(56, 50, 100, 10, "Pan 1");
  musdelay_pan1->type(5);
  musdelay_pan1->box(FL_FLAT_BOX);
  musdelay_pan1->color((Fl_Color)178);
  musdelay_pan1->selection_color((Fl_Color)62);
  musdelay_pan1->labeltype(FL_NORMAL_LABEL);
  musdelay_pan1->labelfont(0);
  musdelay_pan1->labelsize(10);
  musdelay_pan1->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_pan1->minimum(-64);
  musdelay_pan1->maximum(63);
  musdelay_pan1->step(1);
  musdelay_pan1->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_pan1->callback((Fl_Callback*)cb_musdelay_pan1);
  musdelay_pan1->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_pan1->when(FL_WHEN_CHANGED);
} // RKR_Slider* musdelay_pan1
{ musdelay_pan2 = new RKR_Slider(56, 62, 100, 10, "Pan 2");
  musdelay_pan2->type(5);
  musdelay_pan2->box(FL_FLAT_BOX);
  musdelay_pan2->color((Fl_Color)178);
  musdelay_pan2->selection_color((Fl_Color)62);
  musdelay_pan2->labeltype(FL_NORMAL_LABEL);
  musdelay_pan2->labelfont(0);
  musdelay_pan2->labelsize(10);
  musdelay_pan2->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_pan2->minimum(-64);
  musdelay_pan2->maximum(63);
  musdelay_pan2->step(1);
  musdelay_pan2->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_pan2->callback((Fl_Callback*)cb_musdelay_pan2);
  musdelay_pan2->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_pan2->when(FL_WHEN_CHANGED);
} // RKR_Slider* musdelay_pan2
{ musdelay_delay1 = new RKR_Choice(10, 82, 38, 14);
  musdelay_delay1->tooltip("Delay 1");
  musdelay_delay1->box(FL_FLAT_BOX);
  musdelay_delay1->down_box(FL_BORDER_BOX);
  musdelay_delay1->color(FL_BACKGROUND_COLOR);
  musdelay_delay1->selection_color(FL_SELECTION_COLOR);
  musdelay_delay1->labeltype(FL_NORMAL_LABEL);
  musdelay_delay1->labelfont(0);
  musdelay_delay1->labelsize(10);
  musdelay_delay1->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_delay1->textsize(10);
  musdelay_delay1->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_delay1->callback((Fl_Callback*)cb_musdelay_delay1, (void*)(12));
  musdelay_delay1->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_delay1->when(FL_WHEN_RELEASE);
  musdelay_delay1->menu(menu_musdelay_delay1);
} // RKR_Choice* musdelay_delay1
{ musdelay_delay3 = new RKR_Choice(62, 82, 38, 14);
  musdelay_delay3->tooltip("Delay Offset");
  musdelay_delay3->box(FL_FLAT_BOX);
  musdelay_delay3->down_box(FL_BORDER_BOX);
  musdelay_delay3->color(FL_BACKGROUND_COLOR);
  musdelay_delay3->selection_color(FL_SELECTION_COLOR);
  musdelay_delay3->labeltype(FL_NORMAL_LABEL);
  musdelay_delay3->labelfont(0);
  musdelay_delay3->labelsize(10);
  musdelay_delay3->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_delay3->textsize(10);
  musdelay_delay3->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_delay3->callback((Fl_Callback*)cb_musdelay_delay3, (void*)(12));
  musdelay_delay3->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_delay3->when(FL_WHEN_RELEASE);
  musdelay_delay3->menu(menu_musdelay_delay3);
} // RKR_Choice* musdelay_delay3
{ RKR_Choice* o = musdelay_delay2 = new RKR_Choice(114, 82, 38, 14);
  musdelay_delay2->tooltip("Delay 2");
  musdelay_delay2->box(FL_FLAT_BOX);
  musdelay_delay2->down_box(FL_BORDER_BOX);
  musdelay_delay2->color(FL_BACKGROUND_COLOR);
  musdelay_delay2->selection_color(FL_SELECTION_COLOR);
  musdelay_delay2->labeltype(FL_NORMAL_LABEL);
  musdelay_delay2->labelfont(0);
  musdelay_delay2->labelsize(10);
  musdelay_delay2->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_delay2->textsize(10);
  musdelay_delay2->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_delay2->callback((Fl_Callback*)cb_musdelay_delay2);
  musdelay_delay2->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_delay2->when(FL_WHEN_RELEASE);
  o->menu(menu_musdelay_delay1);
} // RKR_Choice* musdelay_delay2
{ musdelay_tempo = new RKR_Slider(56, 103, 100, 10, "Tempo");
  musdelay_tempo->type(5);
  musdelay_tempo->box(FL_FLAT_BOX);
  musdelay_tempo->color((Fl_Color)178);
  musdelay_tempo->selection_color((Fl_Color)62);
  musdelay_tempo->labeltype(FL_NORMAL_LABEL);
  musdelay_tempo->labelfont(0);
  musdelay_tempo->labelsize(10);
  musdelay_tempo->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_tempo->minimum(10);
  musdelay_tempo->maximum(480);
  musdelay_tempo->step(1);
  musdelay_tempo->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_tempo->callback((Fl_Callback*)cb_musdelay_tempo);
  musdelay_tempo->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_tempo->when(FL_WHEN_RELEASE);
} // RKR_Slider* musdelay_tempo
{ musdelay_gain1 = new RKR_Slider(56, 122, 100, 10, "Gain 1");
  musdelay_gain1->type(5);
  musdelay_gain1->box(FL_FLAT_BOX);
  musdelay_gain1->color((Fl_Color)178);
  musdelay_gain1->selection_color((Fl_Color)62);
  musdelay_gain1->labeltype(FL_NORMAL_LABEL);
  musdelay_gain1->labelfont(0);
  musdelay_gain1->labelsize(10);
  musdelay_gain1->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_gain1->maximum(127);
  musdelay_gain1->step(1);
  musdelay_gain1->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_gain1->callback((Fl_Callback*)cb_musdelay_gain1);
  musdelay_gain1->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_gain1->when(FL_WHEN_CHANGED);
} // RKR_Slider* musdelay_gain1
{ musdelay_gain2 = new RKR_Slider(56, 134, 100, 10, "Gain 2");
  musdelay_gain2->type(5);
  musdelay_gain2->box(FL_FLAT_BOX);
  musdelay_gain2->color((Fl_Color)178);
  musdelay_gain2->selection_color((Fl_Color)62);
  musdelay_gain2->labeltype(FL_NORMAL_LABEL);
  musdelay_gain2->labelfont(0);
  musdelay_gain2->labelsize(10);
  musdelay_gain2->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_gain2->maximum(127);
  musdelay_gain2->step(1);
  musdelay_gain2->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_gain2->callback((Fl_Callback*)cb_musdelay_gain2);
  musdelay_gain2->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_gain2->when(FL_WHEN_CHANGED);
} // RKR_Slider* musdelay_gain2
{ musdelay_fb1 = new RKR_Slider(56, 146, 100, 10, "FB 1");
  musdelay_fb1->tooltip("Feedback 1");
  musdelay_fb1->type(5);
  musdelay_fb1->box(FL_FLAT_BOX);
  musdelay_fb1->color((Fl_Color)178);
  musdelay_fb1->selection_color((Fl_Color)62);
  musdelay_fb1->labeltype(FL_NORMAL_LABEL);
  musdelay_fb1->labelfont(0);
  musdelay_fb1->labelsize(10);
  musdelay_fb1->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_fb1->maximum(127);
  musdelay_fb1->step(1);
  musdelay_fb1->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_fb1->callback((Fl_Callback*)cb_musdelay_fb1);
  musdelay_fb1->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_fb1->when(FL_WHEN_CHANGED);
} // RKR_Slider* musdelay_fb1
{ musdelay_fb2 = new RKR_Slider(56, 158, 100, 10, "FB 2");
  musdelay_fb2->tooltip("Feedback 2");
  musdelay_fb2->type(5);
  musdelay_fb2->box(FL_FLAT_BOX);
  musdelay_fb2->color((Fl_Color)178);
  musdelay_fb2->selection_color((Fl_Color)62);
  musdelay_fb2->labeltype(FL_NORMAL_LABEL);
  musdelay_fb2->labelfont(0);
  musdelay_fb2->labelsize(10);
  musdelay_fb2->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_fb2->maximum(127);
  musdelay_fb2->step(1);
  musdelay_fb2->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_fb2->callback((Fl_Callback*)cb_musdelay_fb2);
  musdelay_fb2->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_fb2->when(FL_WHEN_CHANGED);
} // RKR_Slider* musdelay_fb2
{ musdelay_damp = new RKR_Slider(56, 170, 100, 10, "Damp");
  musdelay_damp->type(5);
  musdelay_damp->box(FL_FLAT_BOX);
  musdelay_damp->color((Fl_Color)178);
  musdelay_damp->selection_color((Fl_Color)62);
  musdelay_damp->labeltype(FL_NORMAL_LABEL);
  musdelay_damp->labelfont(0);
  musdelay_damp->labelsize(10);
  musdelay_damp->labelcolor(FL_BACKGROUND2_COLOR);
  musdelay_damp->maximum(127);
  musdelay_damp->step(1);
  musdelay_damp->textcolor(FL_BACKGROUND2_COLOR);
  musdelay_damp->callback((Fl_Callback*)cb_musdelay_damp);
  musdelay_damp->align(Fl_Align(FL_ALIGN_LEFT));
  musdelay_damp->when(FL_WHEN_CHANGED);
} // RKR_Slider* musdelay_damp
position(X, Y);
end();
}
