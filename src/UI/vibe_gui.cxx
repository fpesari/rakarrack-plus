// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "vibe_gui.h"

void VibeGui::cb_vibe_activar_i(Fl_Light_Button* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(116);
 o->value(rkr->Vibe_Bypass);
 return;
}
rkr->Vibe_Bypass=(int)o->value();
if((int) o->value()==0)
rkr->efx_Vibe->cleanup();
rgui->findpos(45,(int)o->value(),o);
}
void VibeGui::cb_vibe_activar(Fl_Light_Button* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_activar_i(o,v);
}

void VibeGui::cb_vibe_preset_i(Fl_Choice* o, void* v) {
  long long ud= (long long) v;
if((ud==0)||(ud==12045))rkr->efx_Vibe->setpreset((int)o->value());
vibe_WD->value(Dry_Wet(rkr->efx_Vibe->getpar(6)));
vibe_width->value(rkr->efx_Vibe->getpar(0));
vibe_dpth->value(rkr->efx_Vibe->getpar(8));
vibe_freq->value(rkr->efx_Vibe->getpar(1));
vibe_rnd->value(rkr->efx_Vibe->getpar(2));
vibe_lfotype->value(rkr->efx_Vibe->getpar(3));
vibe_stdf->value(rkr->efx_Vibe->getpar(4));
vibe_pan->value(rkr->efx_Vibe->getpar(5)-64);
vibe_fb->value(rkr->efx_Vibe->getpar(7)-64);
vibe_LR->value(rkr->efx_Vibe->getpar(9));
vibe_stereo->value(rkr->efx_Vibe->getpar(10));
}
void VibeGui::cb_vibe_preset(Fl_Choice* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_preset_i(o,v);
}

Fl_Menu_Item VibeGui::menu_vibe_preset[] = {
 {"Classic", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Stereo Classic", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Wide Vibe", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Classic Chorus", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Vibe Chorus", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Lush Chorus", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Sick Phaser", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Warble", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Vibe 9", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void VibeGui::cb_vibe_WD_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(386);
 return;
}
rkr->efx_Vibe->changepar(6,Dry_Wet((int)(o->value())));
}
void VibeGui::cb_vibe_WD(SliderW* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_WD_i(o,v);
}

void VibeGui::cb_vibe_width_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(387);
 return;
}
rkr->efx_Vibe->changepar(0,(int)o->value());
}
void VibeGui::cb_vibe_width(SliderW* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_width_i(o,v);
}

void VibeGui::cb_vibe_dpth_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(388);
 return;
}
rkr->efx_Vibe->changepar(8,(int)o->value());
}
void VibeGui::cb_vibe_dpth(SliderW* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_dpth_i(o,v);
}

void VibeGui::cb_vibe_freq_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(389);
 return;
}
rkr->efx_Vibe->changepar(1,(int)o->value());
}
void VibeGui::cb_vibe_freq(SliderW* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_freq_i(o,v);
}

void VibeGui::cb_vibe_rnd_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(390);
 return;
}
rkr->efx_Vibe->changepar(2,(int)o->value());
}
void VibeGui::cb_vibe_rnd(SliderW* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_rnd_i(o,v);
}

void VibeGui::cb_vibe_lfotype_i(Fl_Choice* o, void*) {
  rkr->efx_Vibe->changepar(3,(int)o->value());
}
void VibeGui::cb_vibe_lfotype(Fl_Choice* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_lfotype_i(o,v);
}

void VibeGui::cb_vibe_stdf_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(391);
 return;
}
rkr->efx_Vibe->changepar(4,(int)o->value());
}
void VibeGui::cb_vibe_stdf(SliderW* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_stdf_i(o,v);
}

void VibeGui::cb_vibe_fb_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(392);
 return;
}
rkr->efx_Vibe->changepar(7,(int)(o->value()+64));
}
void VibeGui::cb_vibe_fb(SliderW* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_fb_i(o,v);
}

void VibeGui::cb_vibe_LR_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(393);
 return;
}
rkr->efx_Vibe->changepar(9,(int)(o->value()));
}
void VibeGui::cb_vibe_LR(SliderW* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_LR_i(o,v);
}

void VibeGui::cb_vibe_pan_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(394);
 return;
}
rkr->efx_Vibe->changepar(5,(int)(o->value()+64));
}
void VibeGui::cb_vibe_pan(SliderW* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_pan_i(o,v);
}

void VibeGui::cb_vibe_stereo_i(Fl_Check_Button* o, void*) {
  rkr->efx_Vibe->changepar(10,(int)o->value());
}
void VibeGui::cb_vibe_stereo(Fl_Check_Button* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_stereo_i(o,v);
}
VibeGui::VibeGui(int X, int Y, int W, int H, const char *L)
  : Fl_Group(0, 0, W, H, L) {
this->box(FL_UP_BOX);
this->color(FL_FOREGROUND_COLOR);
this->selection_color(FL_FOREGROUND_COLOR);
this->user_data((void*)(1));
this->align(Fl_Align(96|FL_ALIGN_INSIDE));
{ vibe_activar = new Fl_Light_Button(5, 4, 34, 18, "On");
  vibe_activar->shortcut(0x38);
  vibe_activar->color((Fl_Color)62);
  vibe_activar->selection_color((Fl_Color)1);
  vibe_activar->labelsize(10);
  vibe_activar->callback((Fl_Callback*)cb_vibe_activar, (void*)(2));
  vibe_activar->align(Fl_Align(68|FL_ALIGN_INSIDE));
  vibe_activar->when(FL_WHEN_CHANGED);
} // Fl_Light_Button* vibe_activar
{ vibe_preset = new Fl_Choice(77, 4, 76, 18, "Preset");
  vibe_preset->down_box(FL_BORDER_BOX);
  vibe_preset->selection_color(FL_FOREGROUND_COLOR);
  vibe_preset->labelsize(10);
  vibe_preset->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_preset->textsize(10);
  vibe_preset->textcolor(FL_BACKGROUND2_COLOR);
  vibe_preset->callback((Fl_Callback*)cb_vibe_preset, (void*)(12045));
  vibe_preset->when(FL_WHEN_RELEASE_ALWAYS);
  vibe_preset->menu(menu_vibe_preset);
} // Fl_Choice* vibe_preset
{ vibe_WD = new SliderW(56, 29, 100, 10, "Dry/Wet");
  vibe_WD->type(5);
  vibe_WD->box(FL_FLAT_BOX);
  vibe_WD->color((Fl_Color)178);
  vibe_WD->selection_color((Fl_Color)62);
  vibe_WD->labeltype(FL_NORMAL_LABEL);
  vibe_WD->labelfont(0);
  vibe_WD->labelsize(10);
  vibe_WD->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_WD->maximum(127);
  vibe_WD->step(1);
  vibe_WD->textcolor(FL_BACKGROUND2_COLOR);
  vibe_WD->callback((Fl_Callback*)cb_vibe_WD);
  vibe_WD->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_WD->when(FL_WHEN_CHANGED);
} // SliderW* vibe_WD
{ vibe_width = new SliderW(56, 42, 100, 10, "Width");
  vibe_width->type(5);
  vibe_width->box(FL_FLAT_BOX);
  vibe_width->color((Fl_Color)178);
  vibe_width->selection_color((Fl_Color)62);
  vibe_width->labeltype(FL_NORMAL_LABEL);
  vibe_width->labelfont(0);
  vibe_width->labelsize(10);
  vibe_width->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_width->maximum(127);
  vibe_width->step(1);
  vibe_width->textcolor(FL_BACKGROUND2_COLOR);
  vibe_width->callback((Fl_Callback*)cb_vibe_width);
  vibe_width->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_width->when(FL_WHEN_CHANGED);
} // SliderW* vibe_width
{ vibe_dpth = new SliderW(56, 55, 100, 10, "Depth");
  vibe_dpth->type(5);
  vibe_dpth->box(FL_FLAT_BOX);
  vibe_dpth->color((Fl_Color)178);
  vibe_dpth->selection_color((Fl_Color)62);
  vibe_dpth->labeltype(FL_NORMAL_LABEL);
  vibe_dpth->labelfont(0);
  vibe_dpth->labelsize(10);
  vibe_dpth->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_dpth->maximum(127);
  vibe_dpth->step(1);
  vibe_dpth->textcolor(FL_BACKGROUND2_COLOR);
  vibe_dpth->callback((Fl_Callback*)cb_vibe_dpth);
  vibe_dpth->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_dpth->when(FL_WHEN_CHANGED);
} // SliderW* vibe_dpth
{ vibe_freq = new SliderW(56, 68, 100, 10, "Tempo");
  vibe_freq->type(5);
  vibe_freq->box(FL_FLAT_BOX);
  vibe_freq->color((Fl_Color)178);
  vibe_freq->selection_color((Fl_Color)62);
  vibe_freq->labeltype(FL_NORMAL_LABEL);
  vibe_freq->labelfont(0);
  vibe_freq->labelsize(10);
  vibe_freq->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_freq->minimum(1);
  vibe_freq->maximum(600);
  vibe_freq->step(1);
  vibe_freq->textcolor(FL_BACKGROUND2_COLOR);
  vibe_freq->callback((Fl_Callback*)cb_vibe_freq);
  vibe_freq->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_freq->when(FL_WHEN_CHANGED);
} // SliderW* vibe_freq
{ vibe_rnd = new SliderW(56, 81, 100, 10, "Random");
  vibe_rnd->type(5);
  vibe_rnd->box(FL_FLAT_BOX);
  vibe_rnd->color((Fl_Color)178);
  vibe_rnd->selection_color((Fl_Color)62);
  vibe_rnd->labeltype(FL_NORMAL_LABEL);
  vibe_rnd->labelfont(0);
  vibe_rnd->labelsize(10);
  vibe_rnd->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_rnd->maximum(127);
  vibe_rnd->step(1);
  vibe_rnd->textcolor(FL_BACKGROUND2_COLOR);
  vibe_rnd->callback((Fl_Callback*)cb_vibe_rnd);
  vibe_rnd->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_rnd->when(FL_WHEN_CHANGED);
} // SliderW* vibe_rnd
{ Fl_Choice* o = vibe_lfotype = new Fl_Choice(78, 96, 72, 16, "LFO Type");
  vibe_lfotype->down_box(FL_BORDER_BOX);
  vibe_lfotype->selection_color(FL_FOREGROUND_COLOR);
  vibe_lfotype->labelsize(10);
  vibe_lfotype->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_lfotype->textsize(10);
  vibe_lfotype->textcolor(FL_BACKGROUND2_COLOR);
  vibe_lfotype->callback((Fl_Callback*)cb_vibe_lfotype);
  o->menu(m_lfo_menu->get_lfo_type());
} // Fl_Choice* vibe_lfotype
{ vibe_stdf = new SliderW(56, 118, 100, 10, "Stereo Df");
  vibe_stdf->tooltip("LFO L/R Delay");
  vibe_stdf->type(5);
  vibe_stdf->box(FL_FLAT_BOX);
  vibe_stdf->color((Fl_Color)178);
  vibe_stdf->selection_color((Fl_Color)62);
  vibe_stdf->labeltype(FL_NORMAL_LABEL);
  vibe_stdf->labelfont(0);
  vibe_stdf->labelsize(10);
  vibe_stdf->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_stdf->maximum(127);
  vibe_stdf->step(1);
  vibe_stdf->textcolor(FL_BACKGROUND2_COLOR);
  vibe_stdf->callback((Fl_Callback*)cb_vibe_stdf);
  vibe_stdf->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_stdf->when(FL_WHEN_CHANGED);
} // SliderW* vibe_stdf
{ vibe_fb = new SliderW(56, 131, 100, 10, "Feedback");
  vibe_fb->type(5);
  vibe_fb->box(FL_FLAT_BOX);
  vibe_fb->color((Fl_Color)178);
  vibe_fb->selection_color((Fl_Color)62);
  vibe_fb->labeltype(FL_NORMAL_LABEL);
  vibe_fb->labelfont(0);
  vibe_fb->labelsize(10);
  vibe_fb->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_fb->minimum(-64);
  vibe_fb->maximum(64);
  vibe_fb->step(1);
  vibe_fb->textcolor(FL_BACKGROUND2_COLOR);
  vibe_fb->callback((Fl_Callback*)cb_vibe_fb);
  vibe_fb->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_fb->when(FL_WHEN_CHANGED);
} // SliderW* vibe_fb
{ vibe_LR = new SliderW(56, 144, 100, 10, "L/R Cross");
  vibe_LR->type(5);
  vibe_LR->box(FL_FLAT_BOX);
  vibe_LR->color((Fl_Color)178);
  vibe_LR->selection_color((Fl_Color)62);
  vibe_LR->labeltype(FL_NORMAL_LABEL);
  vibe_LR->labelfont(0);
  vibe_LR->labelsize(10);
  vibe_LR->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_LR->maximum(128);
  vibe_LR->step(1);
  vibe_LR->textcolor(FL_BACKGROUND2_COLOR);
  vibe_LR->callback((Fl_Callback*)cb_vibe_LR);
  vibe_LR->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_LR->when(FL_WHEN_CHANGED);
} // SliderW* vibe_LR
{ vibe_pan = new SliderW(56, 157, 100, 10, "Pan");
  vibe_pan->type(5);
  vibe_pan->box(FL_FLAT_BOX);
  vibe_pan->color((Fl_Color)178);
  vibe_pan->selection_color((Fl_Color)62);
  vibe_pan->labeltype(FL_NORMAL_LABEL);
  vibe_pan->labelfont(0);
  vibe_pan->labelsize(10);
  vibe_pan->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_pan->minimum(-64);
  vibe_pan->maximum(64);
  vibe_pan->step(1);
  vibe_pan->textcolor(FL_BACKGROUND2_COLOR);
  vibe_pan->callback((Fl_Callback*)cb_vibe_pan);
  vibe_pan->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_pan->when(FL_WHEN_CHANGED);
} // SliderW* vibe_pan
{ vibe_stereo = new Fl_Check_Button(103, 169, 43, 14, "Stereo");
  vibe_stereo->down_box(FL_BORDER_BOX);
  vibe_stereo->labelsize(10);
  vibe_stereo->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_stereo->callback((Fl_Callback*)cb_vibe_stereo, (void*)(2));
} // Fl_Check_Button* vibe_stereo
position(X, Y);
end();
}
