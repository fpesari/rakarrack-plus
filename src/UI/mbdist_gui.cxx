// generated by Fast Light User Interface Designer (fluid) version 1.0305

#include "mbdist_gui.h"

void MbdistGui::cb_mbdist_activar_i(RKR_Light_Button* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(116);
 o->value(rkr->MBDist_Bypass);
 return;
}
rkr->MBDist_Bypass=(int)o->value();
if((int) o->value()==0)
rkr->efx_MBDist->cleanup();
rgui->findpos(23,(int)o->value(),o);
}
void MbdistGui::cb_mbdist_activar(RKR_Light_Button* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_activar_i(o,v);
}

void MbdistGui::cb_mbdist_preset_i(RKR_Choice* o, void* v) {
  long long ud= (long long) v;
if((ud==0)||(ud==12023))rkr->efx_MBDist->setpreset((int)o->value());
mbdist_WD->value(Dry_Wet(rkr->efx_MBDist->getpar(0)));
mbdist_LRc->value(rkr->efx_MBDist->getpar(2));
mbdist_drive->value(rkr->efx_MBDist->getpar(3));
mbdist_level->value(rkr->efx_MBDist->getpar(4));
mbdist_tipoL->value(rkr->efx_MBDist->getpar(5));
mbdist_tipoM->value(rkr->efx_MBDist->getpar(6));
mbdist_tipoH->value(rkr->efx_MBDist->getpar(7));
mbdist_volL->value(rkr->efx_MBDist->getpar(8));
mbdist_volM->value(rkr->efx_MBDist->getpar(9));
mbdist_volH->value(rkr->efx_MBDist->getpar(10));
mbdist_neg->value(rkr->efx_MBDist->getpar(11));
mbdist_st->value(rkr->efx_MBDist->getpar(14));
mbdist_pan->value(rkr->efx_MBDist->getpar(1)-64);
mbdist_cross1->value(rkr->efx_MBDist->getpar(12));
mbdist_cross2->value(rkr->efx_MBDist->getpar(13));
}
void MbdistGui::cb_mbdist_preset(RKR_Choice* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_preset_i(o,v);
}

Fl_Menu_Item MbdistGui::menu_mbdist_preset[] = {
 {"Saturation", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Distorsion 1", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Soft", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Modulated", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Crunch", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Distortion 2", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Distortion 3", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Distortion 4", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void MbdistGui::cb_mbdist_WD_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(202);
 return;
} 
rkr->efx_MBDist->changepar(0,Dry_Wet((int)(o->value())));
}
void MbdistGui::cb_mbdist_WD(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_WD_i(o,v);
}

void MbdistGui::cb_mbdist_LRc_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(203);
 return;
} 
rkr->efx_MBDist->changepar(2,(int)(o->value()));
}
void MbdistGui::cb_mbdist_LRc(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_LRc_i(o,v);
}

void MbdistGui::cb_mbdist_drive_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(204);
 return;
} 
rkr->efx_MBDist->changepar(3,(int)o->value());
}
void MbdistGui::cb_mbdist_drive(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_drive_i(o,v);
}

void MbdistGui::cb_mbdist_level_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(205);
 return;
} 
rkr->efx_MBDist->changepar(4,(int)o->value());
}
void MbdistGui::cb_mbdist_level(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_level_i(o,v);
}

void MbdistGui::cb_mbdist_volL_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(206);
 return;
} 
rkr->efx_MBDist->changepar(8,(int)o->value());
}
void MbdistGui::cb_mbdist_volL(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_volL_i(o,v);
}

void MbdistGui::cb_mbdist_volM_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(207);
 return;
} 
rkr->efx_MBDist->changepar(9,(int)o->value());
}
void MbdistGui::cb_mbdist_volM(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_volM_i(o,v);
}

void MbdistGui::cb_mbdist_volH_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(208);
 return;
} 
rkr->efx_MBDist->changepar(10,(int)o->value());
}
void MbdistGui::cb_mbdist_volH(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_volH_i(o,v);
}

void MbdistGui::cb_mbdist_cross1_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(209);
 return;
} 
rkr->efx_MBDist->changepar(12,(int)o->value());
}
void MbdistGui::cb_mbdist_cross1(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_cross1_i(o,v);
}

void MbdistGui::cb_mbdist_cross2_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(210);
 return;
} 
rkr->efx_MBDist->changepar(13,(int)o->value());
}
void MbdistGui::cb_mbdist_cross2(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_cross2_i(o,v);
}

void MbdistGui::cb_mbdist_tipoL_i(RKR_Choice* o, void*) {
  rkr->efx_MBDist->changepar(5,(int)o->value());
}
void MbdistGui::cb_mbdist_tipoL(RKR_Choice* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_tipoL_i(o,v);
}

void MbdistGui::cb_mbdist_tipoM_i(RKR_Choice* o, void*) {
  rkr->efx_MBDist->changepar(6,(int)o->value());
}
void MbdistGui::cb_mbdist_tipoM(RKR_Choice* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_tipoM_i(o,v);
}

void MbdistGui::cb_mbdist_tipoH_i(RKR_Choice* o, void*) {
  rkr->efx_MBDist->changepar(7,(int)o->value());
}
void MbdistGui::cb_mbdist_tipoH(RKR_Choice* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_tipoH_i(o,v);
}

void MbdistGui::cb_mbdist_pan_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(211);
 return;
} 
rkr->efx_MBDist->changepar(1,(int)(o->value()+64));
}
void MbdistGui::cb_mbdist_pan(SliderW* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_pan_i(o,v);
}

void MbdistGui::cb_mbdist_st_i(RKR_Check_Button* o, void*) {
  rkr->efx_MBDist->changepar(14,(int)o->value());
}
void MbdistGui::cb_mbdist_st(RKR_Check_Button* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_st_i(o,v);
}

void MbdistGui::cb_mbdist_neg_i(RKR_Check_Button* o, void*) {
  rkr->efx_MBDist->changepar(11,(int)o->value());
}
void MbdistGui::cb_mbdist_neg(RKR_Check_Button* o, void* v) {
  ((MbdistGui*)(o->parent()))->cb_mbdist_neg_i(o,v);
}
MbdistGui::MbdistGui(int X, int Y, int W, int H, const char *L)
  : Fl_Group(0, 0, W, H, L) {
this->box(FL_UP_BOX);
this->color(FL_FOREGROUND_COLOR);
this->selection_color(FL_FOREGROUND_COLOR);
this->user_data((void*)(1));
this->align(Fl_Align(96|FL_ALIGN_INSIDE));
{ mbdist_activar = new RKR_Light_Button(5, 4, 34, 18, "On");
  mbdist_activar->box(FL_UP_BOX);
  mbdist_activar->shortcut(0x33);
  mbdist_activar->color((Fl_Color)62);
  mbdist_activar->selection_color((Fl_Color)1);
  mbdist_activar->labeltype(FL_NORMAL_LABEL);
  mbdist_activar->labelfont(0);
  mbdist_activar->labelsize(10);
  mbdist_activar->labelcolor(FL_FOREGROUND_COLOR);
  mbdist_activar->callback((Fl_Callback*)cb_mbdist_activar, (void*)(2));
  mbdist_activar->align(Fl_Align(68|FL_ALIGN_INSIDE));
  mbdist_activar->when(FL_WHEN_CHANGED);
} // RKR_Light_Button* mbdist_activar
{ mbdist_preset = new RKR_Choice(77, 4, 76, 18, "Preset");
  mbdist_preset->box(FL_FLAT_BOX);
  mbdist_preset->down_box(FL_BORDER_BOX);
  mbdist_preset->color(FL_BACKGROUND_COLOR);
  mbdist_preset->selection_color(FL_FOREGROUND_COLOR);
  mbdist_preset->labeltype(FL_NORMAL_LABEL);
  mbdist_preset->labelfont(0);
  mbdist_preset->labelsize(10);
  mbdist_preset->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_preset->textsize(10);
  mbdist_preset->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_preset->callback((Fl_Callback*)cb_mbdist_preset, (void*)(12023));
  mbdist_preset->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_preset->when(FL_WHEN_RELEASE_ALWAYS);
  mbdist_preset->menu(menu_mbdist_preset);
} // RKR_Choice* mbdist_preset
{ mbdist_WD = new SliderW(56, 26, 100, 10, "Dry/Wet");
  mbdist_WD->type(5);
  mbdist_WD->box(FL_FLAT_BOX);
  mbdist_WD->color((Fl_Color)178);
  mbdist_WD->selection_color((Fl_Color)62);
  mbdist_WD->labeltype(FL_NORMAL_LABEL);
  mbdist_WD->labelfont(0);
  mbdist_WD->labelsize(10);
  mbdist_WD->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_WD->maximum(127);
  mbdist_WD->step(1);
  mbdist_WD->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_WD->callback((Fl_Callback*)cb_mbdist_WD);
  mbdist_WD->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_WD->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_WD
{ mbdist_LRc = new SliderW(56, 38, 100, 10, "L/R Cross");
  mbdist_LRc->type(5);
  mbdist_LRc->box(FL_FLAT_BOX);
  mbdist_LRc->color((Fl_Color)178);
  mbdist_LRc->selection_color((Fl_Color)62);
  mbdist_LRc->labeltype(FL_NORMAL_LABEL);
  mbdist_LRc->labelfont(0);
  mbdist_LRc->labelsize(10);
  mbdist_LRc->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_LRc->maximum(127);
  mbdist_LRc->step(1);
  mbdist_LRc->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_LRc->callback((Fl_Callback*)cb_mbdist_LRc);
  mbdist_LRc->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_LRc->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_LRc
{ mbdist_drive = new SliderW(56, 50, 100, 10, "Drive");
  mbdist_drive->type(5);
  mbdist_drive->box(FL_FLAT_BOX);
  mbdist_drive->color((Fl_Color)178);
  mbdist_drive->selection_color((Fl_Color)62);
  mbdist_drive->labeltype(FL_NORMAL_LABEL);
  mbdist_drive->labelfont(0);
  mbdist_drive->labelsize(10);
  mbdist_drive->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_drive->maximum(127);
  mbdist_drive->step(1);
  mbdist_drive->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_drive->callback((Fl_Callback*)cb_mbdist_drive);
  mbdist_drive->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_drive->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_drive
{ mbdist_level = new SliderW(56, 62, 100, 10, "Level");
  mbdist_level->type(5);
  mbdist_level->box(FL_FLAT_BOX);
  mbdist_level->color((Fl_Color)178);
  mbdist_level->selection_color((Fl_Color)62);
  mbdist_level->labeltype(FL_NORMAL_LABEL);
  mbdist_level->labelfont(0);
  mbdist_level->labelsize(10);
  mbdist_level->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_level->maximum(127);
  mbdist_level->step(1);
  mbdist_level->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_level->callback((Fl_Callback*)cb_mbdist_level);
  mbdist_level->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_level->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_level
{ mbdist_volL = new SliderW(56, 76, 100, 10, "L. Gain");
  mbdist_volL->tooltip("Low Gain");
  mbdist_volL->type(5);
  mbdist_volL->box(FL_FLAT_BOX);
  mbdist_volL->color((Fl_Color)178);
  mbdist_volL->selection_color((Fl_Color)62);
  mbdist_volL->labeltype(FL_NORMAL_LABEL);
  mbdist_volL->labelfont(0);
  mbdist_volL->labelsize(10);
  mbdist_volL->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_volL->maximum(100);
  mbdist_volL->step(1);
  mbdist_volL->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_volL->callback((Fl_Callback*)cb_mbdist_volL);
  mbdist_volL->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_volL->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_volL
{ mbdist_volM = new SliderW(56, 89, 100, 10, "M. Gain");
  mbdist_volM->tooltip("Mid Gain");
  mbdist_volM->type(5);
  mbdist_volM->box(FL_FLAT_BOX);
  mbdist_volM->color((Fl_Color)178);
  mbdist_volM->selection_color((Fl_Color)62);
  mbdist_volM->labeltype(FL_NORMAL_LABEL);
  mbdist_volM->labelfont(0);
  mbdist_volM->labelsize(10);
  mbdist_volM->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_volM->maximum(100);
  mbdist_volM->step(1);
  mbdist_volM->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_volM->callback((Fl_Callback*)cb_mbdist_volM);
  mbdist_volM->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_volM->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_volM
{ mbdist_volH = new SliderW(56, 102, 100, 10, "H. Gain");
  mbdist_volH->tooltip("High Gain");
  mbdist_volH->type(5);
  mbdist_volH->box(FL_FLAT_BOX);
  mbdist_volH->color((Fl_Color)178);
  mbdist_volH->selection_color((Fl_Color)62);
  mbdist_volH->labeltype(FL_NORMAL_LABEL);
  mbdist_volH->labelfont(0);
  mbdist_volH->labelsize(10);
  mbdist_volH->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_volH->maximum(100);
  mbdist_volH->step(1);
  mbdist_volH->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_volH->callback((Fl_Callback*)cb_mbdist_volH);
  mbdist_volH->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_volH->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_volH
{ mbdist_cross1 = new SliderW(56, 115, 100, 10, "Cross 1");
  mbdist_cross1->type(5);
  mbdist_cross1->box(FL_FLAT_BOX);
  mbdist_cross1->color((Fl_Color)178);
  mbdist_cross1->selection_color((Fl_Color)62);
  mbdist_cross1->labeltype(FL_NORMAL_LABEL);
  mbdist_cross1->labelfont(0);
  mbdist_cross1->labelsize(10);
  mbdist_cross1->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_cross1->minimum(20);
  mbdist_cross1->maximum(1000);
  mbdist_cross1->step(1);
  mbdist_cross1->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_cross1->callback((Fl_Callback*)cb_mbdist_cross1);
  mbdist_cross1->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_cross1->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_cross1
{ mbdist_cross2 = new SliderW(56, 127, 100, 10, "Cross 2");
  mbdist_cross2->type(5);
  mbdist_cross2->box(FL_FLAT_BOX);
  mbdist_cross2->color((Fl_Color)178);
  mbdist_cross2->selection_color((Fl_Color)62);
  mbdist_cross2->labeltype(FL_NORMAL_LABEL);
  mbdist_cross2->labelfont(0);
  mbdist_cross2->labelsize(10);
  mbdist_cross2->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_cross2->minimum(800);
  mbdist_cross2->maximum(12000);
  mbdist_cross2->step(1);
  mbdist_cross2->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_cross2->callback((Fl_Callback*)cb_mbdist_cross2);
  mbdist_cross2->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_cross2->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_cross2
{ RKR_Choice* o = mbdist_tipoL = new RKR_Choice(3, 139, 50, 16);
  mbdist_tipoL->box(FL_FLAT_BOX);
  mbdist_tipoL->down_box(FL_BORDER_BOX);
  mbdist_tipoL->color(FL_BACKGROUND_COLOR);
  mbdist_tipoL->selection_color(FL_FOREGROUND_COLOR);
  mbdist_tipoL->labeltype(FL_NORMAL_LABEL);
  mbdist_tipoL->labelfont(0);
  mbdist_tipoL->labelsize(10);
  mbdist_tipoL->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_tipoL->textsize(10);
  mbdist_tipoL->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_tipoL->callback((Fl_Callback*)cb_mbdist_tipoL);
  mbdist_tipoL->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_tipoL->when(FL_WHEN_RELEASE);
  o->menu(m_dist_menu->get_distortion_type());
} // RKR_Choice* mbdist_tipoL
{ RKR_Choice* o = mbdist_tipoM = new RKR_Choice(54, 139, 50, 16);
  mbdist_tipoM->box(FL_FLAT_BOX);
  mbdist_tipoM->down_box(FL_BORDER_BOX);
  mbdist_tipoM->color(FL_BACKGROUND_COLOR);
  mbdist_tipoM->selection_color(FL_FOREGROUND_COLOR);
  mbdist_tipoM->labeltype(FL_NORMAL_LABEL);
  mbdist_tipoM->labelfont(0);
  mbdist_tipoM->labelsize(10);
  mbdist_tipoM->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_tipoM->textsize(10);
  mbdist_tipoM->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_tipoM->callback((Fl_Callback*)cb_mbdist_tipoM);
  mbdist_tipoM->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_tipoM->when(FL_WHEN_RELEASE);
  o->menu(m_dist_menu->get_distortion_type());
} // RKR_Choice* mbdist_tipoM
{ RKR_Choice* o = mbdist_tipoH = new RKR_Choice(105, 139, 50, 16);
  mbdist_tipoH->box(FL_FLAT_BOX);
  mbdist_tipoH->down_box(FL_BORDER_BOX);
  mbdist_tipoH->color(FL_BACKGROUND_COLOR);
  mbdist_tipoH->selection_color(FL_FOREGROUND_COLOR);
  mbdist_tipoH->labeltype(FL_NORMAL_LABEL);
  mbdist_tipoH->labelfont(0);
  mbdist_tipoH->labelsize(10);
  mbdist_tipoH->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_tipoH->textsize(10);
  mbdist_tipoH->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_tipoH->callback((Fl_Callback*)cb_mbdist_tipoH);
  mbdist_tipoH->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_tipoH->when(FL_WHEN_RELEASE);
  o->menu(m_dist_menu->get_distortion_type());
} // RKR_Choice* mbdist_tipoH
{ mbdist_pan = new SliderW(56, 158, 100, 10, "Pan");
  mbdist_pan->type(5);
  mbdist_pan->box(FL_FLAT_BOX);
  mbdist_pan->color((Fl_Color)178);
  mbdist_pan->selection_color((Fl_Color)62);
  mbdist_pan->labeltype(FL_NORMAL_LABEL);
  mbdist_pan->labelfont(0);
  mbdist_pan->labelsize(10);
  mbdist_pan->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_pan->minimum(-64);
  mbdist_pan->maximum(63);
  mbdist_pan->step(1);
  mbdist_pan->textcolor(FL_BACKGROUND2_COLOR);
  mbdist_pan->callback((Fl_Callback*)cb_mbdist_pan);
  mbdist_pan->align(Fl_Align(FL_ALIGN_LEFT));
  mbdist_pan->when(FL_WHEN_CHANGED);
} // SliderW* mbdist_pan
{ mbdist_st = new RKR_Check_Button(32, 168, 54, 15, "Stereo");
  mbdist_st->box(FL_NO_BOX);
  mbdist_st->down_box(FL_BORDER_BOX);
  mbdist_st->color(FL_BACKGROUND_COLOR);
  mbdist_st->selection_color(FL_FOREGROUND_COLOR);
  mbdist_st->labeltype(FL_NORMAL_LABEL);
  mbdist_st->labelfont(0);
  mbdist_st->labelsize(10);
  mbdist_st->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_st->callback((Fl_Callback*)cb_mbdist_st, (void*)(2));
  mbdist_st->align(Fl_Align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE));
  mbdist_st->when(FL_WHEN_RELEASE);
} // RKR_Check_Button* mbdist_st
{ mbdist_neg = new RKR_Check_Button(101, 167, 44, 15, "Neg.");
  mbdist_neg->box(FL_NO_BOX);
  mbdist_neg->down_box(FL_BORDER_BOX);
  mbdist_neg->color(FL_BACKGROUND_COLOR);
  mbdist_neg->selection_color(FL_FOREGROUND_COLOR);
  mbdist_neg->labeltype(FL_NORMAL_LABEL);
  mbdist_neg->labelfont(0);
  mbdist_neg->labelsize(10);
  mbdist_neg->labelcolor(FL_BACKGROUND2_COLOR);
  mbdist_neg->callback((Fl_Callback*)cb_mbdist_neg, (void*)(2));
  mbdist_neg->align(Fl_Align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE));
  mbdist_neg->when(FL_WHEN_RELEASE);
} // RKR_Check_Button* mbdist_neg
position(X, Y);
end();
}
