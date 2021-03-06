// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "vibe_gui.h"

void VibeGui::cb_vibe_activar_i(RKR_Light_Button* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Multi_On_Off);
 o->value(rkr->EFX_Bypass[EFX_VIBE]);
 return;
}
rkr->EFX_Bypass[EFX_VIBE]=(int)o->value();
if((int) o->value()==0)
rkr->Rack_Effects[EFX_VIBE]->cleanup();
rgui->findpos(EFX_VIBE,(int)o->value(),o);
}
void VibeGui::cb_vibe_activar(RKR_Light_Button* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_activar_i(o,v);
}

void VibeGui::cb_vibe_preset_i(RKR_Choice* o, void* v) {
  long long ud= (long long) v;
if((ud==0)||(ud==12045))rkr->Rack_Effects[EFX_VIBE]->setpreset((int)o->value());
vibe_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_DryWet)));
vibe_width->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_Width));
vibe_dpth->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_Depth));
vibe_freq->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LFO_Tempo));
vibe_rnd->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LFO_Random));
vibe_lfotype->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LFO_Type));
vibe_stdf->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LFO_Stereo));
vibe_pan->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_Pan)-64);
vibe_fb->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_Feedback)-64);
vibe_LR->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LR_Cross));
vibe_stereo->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_Stereo));
}
void VibeGui::cb_vibe_preset(RKR_Choice* o, void* v) {
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

void VibeGui::cb_vibe_WD_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_DryWet);
 return;
}
rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_DryWet,Dry_Wet((int)(o->value())));
}
void VibeGui::cb_vibe_WD(RKR_Slider* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_WD_i(o,v);
}

void VibeGui::cb_vibe_width_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_Width);
 return;
}
rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_Width,(int)o->value());
}
void VibeGui::cb_vibe_width(RKR_Slider* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_width_i(o,v);
}

void VibeGui::cb_vibe_dpth_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_Depth);
 return;
}
rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_Depth,(int)o->value());
}
void VibeGui::cb_vibe_dpth(RKR_Slider* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_dpth_i(o,v);
}

void VibeGui::cb_vibe_freq_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_LFO_Tempo);
 return;
}
rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_LFO_Tempo,(int)o->value());
}
void VibeGui::cb_vibe_freq(RKR_Slider* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_freq_i(o,v);
}

void VibeGui::cb_vibe_rnd_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_LFO_Random);
 return;
}
rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_LFO_Random,(int)o->value());
}
void VibeGui::cb_vibe_rnd(RKR_Slider* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_rnd_i(o,v);
}

void VibeGui::cb_vibe_lfotype_i(RKR_Choice* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_LFO_Type);
 return;
}

rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_LFO_Type,(int)o->value());
}
void VibeGui::cb_vibe_lfotype(RKR_Choice* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_lfotype_i(o,v);
}

void VibeGui::cb_vibe_stdf_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_LFO_Stereo);
 return;
}
rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_LFO_Stereo,(int)o->value());
}
void VibeGui::cb_vibe_stdf(RKR_Slider* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_stdf_i(o,v);
}

void VibeGui::cb_vibe_fb_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_Feedback);
 return;
}
rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_Feedback,(int)(o->value()+64));
}
void VibeGui::cb_vibe_fb(RKR_Slider* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_fb_i(o,v);
}

void VibeGui::cb_vibe_LR_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_LR_Cross);
 return;
}
rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_LR_Cross,(int)(o->value()));
}
void VibeGui::cb_vibe_LR(RKR_Slider* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_LR_i(o,v);
}

void VibeGui::cb_vibe_pan_i(RKR_Slider* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(MC_Vibe_Pan);
 return;
}
rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_Pan,(int)(o->value()+64));
}
void VibeGui::cb_vibe_pan(RKR_Slider* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_pan_i(o,v);
}

void VibeGui::cb_vibe_stereo_i(RKR_Check_Button* o, void*) {
  rkr->Rack_Effects[EFX_VIBE]->changepar(Vibe_Stereo,(int)o->value());
}
void VibeGui::cb_vibe_stereo(RKR_Check_Button* o, void* v) {
  ((VibeGui*)(o->parent()))->cb_vibe_stereo_i(o,v);
}
VibeGui::VibeGui(int X, int Y, int W, int H, const char *L)
  : Fl_Group(0, 0, W, H, L) {
this->box(FL_UP_BOX);
this->color(FL_FOREGROUND_COLOR);
this->selection_color(FL_FOREGROUND_COLOR);
this->user_data((void*)(1));
this->align(Fl_Align(96|FL_ALIGN_INSIDE));
{ vibe_activar = new RKR_Light_Button(5, 4, 34, 18, "On");
  vibe_activar->box(FL_UP_BOX);
  vibe_activar->shortcut(0x38);
  vibe_activar->color((Fl_Color)62);
  vibe_activar->selection_color((Fl_Color)1);
  vibe_activar->labeltype(FL_NORMAL_LABEL);
  vibe_activar->labelfont(0);
  vibe_activar->labelsize(10);
  vibe_activar->labelcolor(FL_FOREGROUND_COLOR);
  vibe_activar->callback((Fl_Callback*)cb_vibe_activar, (void*)(2));
  vibe_activar->align(Fl_Align(68|FL_ALIGN_INSIDE));
  vibe_activar->when(FL_WHEN_CHANGED);
} // RKR_Light_Button* vibe_activar
{ vibe_preset = new RKR_Choice(77, 4, 76, 18, "Preset");
  vibe_preset->box(FL_FLAT_BOX);
  vibe_preset->down_box(FL_BORDER_BOX);
  vibe_preset->color(FL_BACKGROUND_COLOR);
  vibe_preset->selection_color(FL_FOREGROUND_COLOR);
  vibe_preset->labeltype(FL_NORMAL_LABEL);
  vibe_preset->labelfont(0);
  vibe_preset->labelsize(10);
  vibe_preset->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_preset->textsize(10);
  vibe_preset->textcolor(FL_BACKGROUND2_COLOR);
  vibe_preset->callback((Fl_Callback*)cb_vibe_preset, (void*)(12045));
  vibe_preset->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_preset->when(FL_WHEN_RELEASE_ALWAYS);
  vibe_preset->menu(menu_vibe_preset);
} // RKR_Choice* vibe_preset
{ vibe_WD = new RKR_Slider(56, 29, 100, 10, "Dry/Wet");
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
} // RKR_Slider* vibe_WD
{ vibe_width = new RKR_Slider(56, 42, 100, 10, "Width");
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
} // RKR_Slider* vibe_width
{ vibe_dpth = new RKR_Slider(56, 55, 100, 10, "Depth");
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
} // RKR_Slider* vibe_dpth
{ vibe_freq = new RKR_Slider(56, 68, 100, 10, "Tempo");
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
} // RKR_Slider* vibe_freq
{ vibe_rnd = new RKR_Slider(56, 81, 100, 10, "Random");
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
} // RKR_Slider* vibe_rnd
{ RKR_Choice* o = vibe_lfotype = new RKR_Choice(78, 96, 72, 16, "LFO Type");
  vibe_lfotype->box(FL_FLAT_BOX);
  vibe_lfotype->down_box(FL_BORDER_BOX);
  vibe_lfotype->color(FL_BACKGROUND_COLOR);
  vibe_lfotype->selection_color(FL_FOREGROUND_COLOR);
  vibe_lfotype->labeltype(FL_NORMAL_LABEL);
  vibe_lfotype->labelfont(0);
  vibe_lfotype->labelsize(10);
  vibe_lfotype->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_lfotype->textsize(10);
  vibe_lfotype->textcolor(FL_BACKGROUND2_COLOR);
  vibe_lfotype->callback((Fl_Callback*)cb_vibe_lfotype);
  vibe_lfotype->align(Fl_Align(FL_ALIGN_LEFT));
  vibe_lfotype->when(FL_WHEN_RELEASE);
  o->menu(m_lfo_menu->get_lfo_type());
} // RKR_Choice* vibe_lfotype
{ vibe_stdf = new RKR_Slider(56, 118, 100, 10, "Stereo Df");
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
} // RKR_Slider* vibe_stdf
{ vibe_fb = new RKR_Slider(56, 131, 100, 10, "Feedback");
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
} // RKR_Slider* vibe_fb
{ vibe_LR = new RKR_Slider(56, 144, 100, 10, "L/R Cross");
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
} // RKR_Slider* vibe_LR
{ vibe_pan = new RKR_Slider(56, 157, 100, 10, "Pan");
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
} // RKR_Slider* vibe_pan
{ vibe_stereo = new RKR_Check_Button(103, 169, 43, 14, "Stereo");
  vibe_stereo->box(FL_NO_BOX);
  vibe_stereo->down_box(FL_BORDER_BOX);
  vibe_stereo->color(FL_BACKGROUND_COLOR);
  vibe_stereo->selection_color(FL_FOREGROUND_COLOR);
  vibe_stereo->labeltype(FL_NORMAL_LABEL);
  vibe_stereo->labelfont(0);
  vibe_stereo->labelsize(10);
  vibe_stereo->labelcolor(FL_BACKGROUND2_COLOR);
  vibe_stereo->callback((Fl_Callback*)cb_vibe_stereo, (void*)(2));
  vibe_stereo->align(Fl_Align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE));
  vibe_stereo->when(FL_WHEN_RELEASE);
} // RKR_Check_Button* vibe_stereo
position(X, Y);
end();
}
