// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "exciter_gui.h"

void ExciterGui::cb_exciter_activar_i(Fl_Light_Button* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(116);
 o->value(rkr->Exciter_Bypass);
 return;
}
rkr->Exciter_Bypass=(int)o->value();
if((int) o->value()==0)
rkr->efx_Exciter->cleanup();
rgui->findpos(22,(int)o->value(),o);
}
void ExciterGui::cb_exciter_activar(Fl_Light_Button* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_exciter_activar_i(o,v);
}

void ExciterGui::cb_exciter_preset_i(Fl_Choice* o, void* v) {
  long long ud= (long long) v;
if((ud==0)||(ud==12022))rkr->efx_Exciter->setpreset((int) o->value());
ex_Gain->value(rkr->efx_Exciter->getpar(0));
ex_1->value(rkr->efx_Exciter->getpar(1));
ex_2->value(rkr->efx_Exciter->getpar(2));
ex_3->value(rkr->efx_Exciter->getpar(3));
ex_4->value(rkr->efx_Exciter->getpar(4));
ex_5->value(rkr->efx_Exciter->getpar(5));
ex_6->value(rkr->efx_Exciter->getpar(6));
ex_7->value(rkr->efx_Exciter->getpar(7));
ex_8->value(rkr->efx_Exciter->getpar(8));
ex_9->value(rkr->efx_Exciter->getpar(9));
ex_10->value(rkr->efx_Exciter->getpar(10));
ex_lfreq->value(rkr->efx_Exciter->getpar(11));
ex_hfreq->value(rkr->efx_Exciter->getpar(12));
}
void ExciterGui::cb_exciter_preset(Fl_Choice* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_exciter_preset_i(o,v);
}

Fl_Menu_Item ExciterGui::menu_exciter_preset[] = {
 {"Plain", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Loudness", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Exciter 1", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Exciter 2", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {"Exciter 3", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 10, 0},
 {0,0,0,0,0,0,0,0,0}
};

void ExciterGui::cb_ex_Gain_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(189);
 return;
} 
rkr->efx_Exciter->changepar(0,(int)o->value());
}
void ExciterGui::cb_ex_Gain(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_Gain_i(o,v);
}

void ExciterGui::cb_ex_lfreq_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(190);
 return;
} 
rkr->efx_Exciter->changepar(11,(int)o->value());
}
void ExciterGui::cb_ex_lfreq(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_lfreq_i(o,v);
}

void ExciterGui::cb_ex_hfreq_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(191);
 return;
} 
rkr->efx_Exciter->changepar(12,(int)o->value());
}
void ExciterGui::cb_ex_hfreq(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_hfreq_i(o,v);
}

void ExciterGui::cb_ex_1_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(192);
 return;
} 
rkr->efx_Exciter->changepar(1,(int)o->value());
}
void ExciterGui::cb_ex_1(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_1_i(o,v);
}

void ExciterGui::cb_ex_2_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(193);
 return;
} 
rkr->efx_Exciter->changepar(2,(int)o->value());
}
void ExciterGui::cb_ex_2(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_2_i(o,v);
}

void ExciterGui::cb_ex_3_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(194);
 return;
} 
rkr->efx_Exciter->changepar(3,(int)o->value());
}
void ExciterGui::cb_ex_3(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_3_i(o,v);
}

void ExciterGui::cb_ex_4_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(195);
 return;
} 
rkr->efx_Exciter->changepar(4,(int)o->value());
}
void ExciterGui::cb_ex_4(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_4_i(o,v);
}

void ExciterGui::cb_ex_5_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(196);
 return;
} 
rkr->efx_Exciter->changepar(5,(int)o->value());
}
void ExciterGui::cb_ex_5(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_5_i(o,v);
}

void ExciterGui::cb_ex_6_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(197);
 return;
} 
rkr->efx_Exciter->changepar(6,(int)o->value());
}
void ExciterGui::cb_ex_6(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_6_i(o,v);
}

void ExciterGui::cb_ex_7_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(198);
 return;
} 
rkr->efx_Exciter->changepar(7,(int)o->value());
}
void ExciterGui::cb_ex_7(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_7_i(o,v);
}

void ExciterGui::cb_ex_8_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(199);
 return;
} 
rkr->efx_Exciter->changepar(8,(int)o->value());
}
void ExciterGui::cb_ex_8(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_8_i(o,v);
}

void ExciterGui::cb_ex_9_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(200);
 return;
} 
rkr->efx_Exciter->changepar(9,(int)o->value());
}
void ExciterGui::cb_ex_9(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_9_i(o,v);
}

void ExciterGui::cb_ex_10_i(SliderW* o, void*) {
  if(Fl::event_button()==3)
{
 rgui->getMIDIControl(201);
 return;
} 
rkr->efx_Exciter->changepar(10,(int)o->value());
}
void ExciterGui::cb_ex_10(SliderW* o, void* v) {
  ((ExciterGui*)(o->parent()))->cb_ex_10_i(o,v);
}
ExciterGui::ExciterGui(int X, int Y, int W, int H, const char *L)
  : Fl_Group(0, 0, W, H, L) {
this->box(FL_UP_BOX);
this->color(FL_FOREGROUND_COLOR);
this->selection_color(FL_FOREGROUND_COLOR);
this->user_data((void*)(1));
this->align(Fl_Align(96|FL_ALIGN_INSIDE));
{ exciter_activar = new Fl_Light_Button(5, 4, 34, 18, "On");
  exciter_activar->shortcut(0x31);
  exciter_activar->color((Fl_Color)62);
  exciter_activar->selection_color((Fl_Color)1);
  exciter_activar->labelsize(10);
  exciter_activar->callback((Fl_Callback*)cb_exciter_activar, (void*)(2));
  exciter_activar->align(Fl_Align(68|FL_ALIGN_INSIDE));
  exciter_activar->when(FL_WHEN_CHANGED);
} // Fl_Light_Button* exciter_activar
{ exciter_preset = new Fl_Choice(77, 4, 76, 18, "Preset");
  exciter_preset->down_box(FL_BORDER_BOX);
  exciter_preset->selection_color(FL_FOREGROUND_COLOR);
  exciter_preset->labelsize(10);
  exciter_preset->labelcolor(FL_BACKGROUND2_COLOR);
  exciter_preset->textsize(10);
  exciter_preset->textcolor(FL_BACKGROUND2_COLOR);
  exciter_preset->callback((Fl_Callback*)cb_exciter_preset, (void*)(12022));
  exciter_preset->when(FL_WHEN_RELEASE_ALWAYS);
  exciter_preset->menu(menu_exciter_preset);
} // Fl_Choice* exciter_preset
{ ex_Gain = new SliderW(49, 25, 100, 10, "Gain");
  ex_Gain->type(5);
  ex_Gain->box(FL_FLAT_BOX);
  ex_Gain->color((Fl_Color)178);
  ex_Gain->selection_color((Fl_Color)62);
  ex_Gain->labeltype(FL_NORMAL_LABEL);
  ex_Gain->labelfont(0);
  ex_Gain->labelsize(10);
  ex_Gain->labelcolor(FL_BACKGROUND2_COLOR);
  ex_Gain->maximum(127);
  ex_Gain->step(1);
  ex_Gain->textcolor(FL_BACKGROUND2_COLOR);
  ex_Gain->callback((Fl_Callback*)cb_ex_Gain);
  ex_Gain->align(Fl_Align(FL_ALIGN_LEFT));
  ex_Gain->when(FL_WHEN_CHANGED);
} // SliderW* ex_Gain
{ ex_lfreq = new SliderW(49, 37, 100, 10, "LPF");
  ex_lfreq->type(5);
  ex_lfreq->box(FL_FLAT_BOX);
  ex_lfreq->color((Fl_Color)178);
  ex_lfreq->selection_color((Fl_Color)62);
  ex_lfreq->labeltype(FL_NORMAL_LABEL);
  ex_lfreq->labelfont(0);
  ex_lfreq->labelsize(10);
  ex_lfreq->labelcolor(FL_BACKGROUND2_COLOR);
  ex_lfreq->minimum(20);
  ex_lfreq->maximum(26000);
  ex_lfreq->step(1);
  ex_lfreq->value(20000);
  ex_lfreq->textcolor(FL_BACKGROUND2_COLOR);
  ex_lfreq->callback((Fl_Callback*)cb_ex_lfreq);
  ex_lfreq->align(Fl_Align(FL_ALIGN_LEFT));
  ex_lfreq->when(FL_WHEN_CHANGED);
} // SliderW* ex_lfreq
{ ex_hfreq = new SliderW(49, 49, 100, 10, "HPF");
  ex_hfreq->type(5);
  ex_hfreq->box(FL_FLAT_BOX);
  ex_hfreq->color((Fl_Color)178);
  ex_hfreq->selection_color((Fl_Color)62);
  ex_hfreq->labeltype(FL_NORMAL_LABEL);
  ex_hfreq->labelfont(0);
  ex_hfreq->labelsize(10);
  ex_hfreq->labelcolor(FL_BACKGROUND2_COLOR);
  ex_hfreq->minimum(20);
  ex_hfreq->maximum(20000);
  ex_hfreq->step(1);
  ex_hfreq->value(20);
  ex_hfreq->textcolor(FL_BACKGROUND2_COLOR);
  ex_hfreq->callback((Fl_Callback*)cb_ex_hfreq);
  ex_hfreq->align(Fl_Align(FL_ALIGN_LEFT));
  ex_hfreq->when(FL_WHEN_CHANGED);
} // SliderW* ex_hfreq
{ ex_1 = new SliderW(49, 61, 100, 10, "Har 1");
  ex_1->type(5);
  ex_1->box(FL_FLAT_BOX);
  ex_1->color((Fl_Color)178);
  ex_1->selection_color((Fl_Color)62);
  ex_1->labeltype(FL_NORMAL_LABEL);
  ex_1->labelfont(0);
  ex_1->labelsize(10);
  ex_1->labelcolor(FL_BACKGROUND2_COLOR);
  ex_1->minimum(-64);
  ex_1->maximum(64);
  ex_1->step(1);
  ex_1->textcolor(FL_BACKGROUND2_COLOR);
  ex_1->callback((Fl_Callback*)cb_ex_1);
  ex_1->align(Fl_Align(FL_ALIGN_LEFT));
  ex_1->when(FL_WHEN_CHANGED);
} // SliderW* ex_1
{ ex_2 = new SliderW(49, 73, 100, 10, "Har 2");
  ex_2->type(5);
  ex_2->box(FL_FLAT_BOX);
  ex_2->color((Fl_Color)178);
  ex_2->selection_color((Fl_Color)62);
  ex_2->labeltype(FL_NORMAL_LABEL);
  ex_2->labelfont(0);
  ex_2->labelsize(10);
  ex_2->labelcolor(FL_BACKGROUND2_COLOR);
  ex_2->minimum(-64);
  ex_2->maximum(64);
  ex_2->step(1);
  ex_2->textcolor(FL_BACKGROUND2_COLOR);
  ex_2->callback((Fl_Callback*)cb_ex_2);
  ex_2->align(Fl_Align(FL_ALIGN_LEFT));
  ex_2->when(FL_WHEN_CHANGED);
} // SliderW* ex_2
{ ex_3 = new SliderW(49, 85, 100, 10, "Har 3");
  ex_3->type(5);
  ex_3->box(FL_FLAT_BOX);
  ex_3->color((Fl_Color)178);
  ex_3->selection_color((Fl_Color)62);
  ex_3->labeltype(FL_NORMAL_LABEL);
  ex_3->labelfont(0);
  ex_3->labelsize(10);
  ex_3->labelcolor(FL_BACKGROUND2_COLOR);
  ex_3->minimum(-64);
  ex_3->maximum(64);
  ex_3->step(1);
  ex_3->textcolor(FL_BACKGROUND2_COLOR);
  ex_3->callback((Fl_Callback*)cb_ex_3);
  ex_3->align(Fl_Align(FL_ALIGN_LEFT));
  ex_3->when(FL_WHEN_CHANGED);
} // SliderW* ex_3
{ ex_4 = new SliderW(49, 97, 100, 10, "Har 4");
  ex_4->type(5);
  ex_4->box(FL_FLAT_BOX);
  ex_4->color((Fl_Color)178);
  ex_4->selection_color((Fl_Color)62);
  ex_4->labeltype(FL_NORMAL_LABEL);
  ex_4->labelfont(0);
  ex_4->labelsize(10);
  ex_4->labelcolor(FL_BACKGROUND2_COLOR);
  ex_4->minimum(-64);
  ex_4->maximum(64);
  ex_4->step(1);
  ex_4->textcolor(FL_BACKGROUND2_COLOR);
  ex_4->callback((Fl_Callback*)cb_ex_4);
  ex_4->align(Fl_Align(FL_ALIGN_LEFT));
  ex_4->when(FL_WHEN_CHANGED);
} // SliderW* ex_4
{ ex_5 = new SliderW(49, 109, 100, 10, "Har 5");
  ex_5->type(5);
  ex_5->box(FL_FLAT_BOX);
  ex_5->color((Fl_Color)178);
  ex_5->selection_color((Fl_Color)62);
  ex_5->labeltype(FL_NORMAL_LABEL);
  ex_5->labelfont(0);
  ex_5->labelsize(10);
  ex_5->labelcolor(FL_BACKGROUND2_COLOR);
  ex_5->minimum(-64);
  ex_5->maximum(64);
  ex_5->step(1);
  ex_5->textcolor(FL_BACKGROUND2_COLOR);
  ex_5->callback((Fl_Callback*)cb_ex_5);
  ex_5->align(Fl_Align(FL_ALIGN_LEFT));
  ex_5->when(FL_WHEN_CHANGED);
} // SliderW* ex_5
{ ex_6 = new SliderW(49, 121, 100, 10, "Har 6");
  ex_6->type(5);
  ex_6->box(FL_FLAT_BOX);
  ex_6->color((Fl_Color)178);
  ex_6->selection_color((Fl_Color)62);
  ex_6->labeltype(FL_NORMAL_LABEL);
  ex_6->labelfont(0);
  ex_6->labelsize(10);
  ex_6->labelcolor(FL_BACKGROUND2_COLOR);
  ex_6->minimum(-64);
  ex_6->maximum(64);
  ex_6->step(1);
  ex_6->textcolor(FL_BACKGROUND2_COLOR);
  ex_6->callback((Fl_Callback*)cb_ex_6);
  ex_6->align(Fl_Align(FL_ALIGN_LEFT));
  ex_6->when(FL_WHEN_CHANGED);
} // SliderW* ex_6
{ ex_7 = new SliderW(49, 134, 100, 10, "Har 7");
  ex_7->type(5);
  ex_7->box(FL_FLAT_BOX);
  ex_7->color((Fl_Color)178);
  ex_7->selection_color((Fl_Color)62);
  ex_7->labeltype(FL_NORMAL_LABEL);
  ex_7->labelfont(0);
  ex_7->labelsize(10);
  ex_7->labelcolor(FL_BACKGROUND2_COLOR);
  ex_7->minimum(-64);
  ex_7->maximum(64);
  ex_7->step(1);
  ex_7->textcolor(FL_BACKGROUND2_COLOR);
  ex_7->callback((Fl_Callback*)cb_ex_7);
  ex_7->align(Fl_Align(FL_ALIGN_LEFT));
  ex_7->when(FL_WHEN_CHANGED);
} // SliderW* ex_7
{ ex_8 = new SliderW(49, 146, 100, 10, "Har 8");
  ex_8->type(5);
  ex_8->box(FL_FLAT_BOX);
  ex_8->color((Fl_Color)178);
  ex_8->selection_color((Fl_Color)62);
  ex_8->labeltype(FL_NORMAL_LABEL);
  ex_8->labelfont(0);
  ex_8->labelsize(10);
  ex_8->labelcolor(FL_BACKGROUND2_COLOR);
  ex_8->minimum(-64);
  ex_8->maximum(64);
  ex_8->step(1);
  ex_8->textcolor(FL_BACKGROUND2_COLOR);
  ex_8->callback((Fl_Callback*)cb_ex_8);
  ex_8->align(Fl_Align(FL_ALIGN_LEFT));
  ex_8->when(FL_WHEN_CHANGED);
} // SliderW* ex_8
{ ex_9 = new SliderW(49, 158, 100, 10, "Har 9");
  ex_9->type(5);
  ex_9->box(FL_FLAT_BOX);
  ex_9->color((Fl_Color)178);
  ex_9->selection_color((Fl_Color)62);
  ex_9->labeltype(FL_NORMAL_LABEL);
  ex_9->labelfont(0);
  ex_9->labelsize(10);
  ex_9->labelcolor(FL_BACKGROUND2_COLOR);
  ex_9->minimum(-64);
  ex_9->maximum(64);
  ex_9->step(1);
  ex_9->textcolor(FL_BACKGROUND2_COLOR);
  ex_9->callback((Fl_Callback*)cb_ex_9);
  ex_9->align(Fl_Align(FL_ALIGN_LEFT));
  ex_9->when(FL_WHEN_CHANGED);
} // SliderW* ex_9
{ ex_10 = new SliderW(49, 169, 100, 10, "Har 10");
  ex_10->type(5);
  ex_10->box(FL_FLAT_BOX);
  ex_10->color((Fl_Color)178);
  ex_10->selection_color((Fl_Color)62);
  ex_10->labeltype(FL_NORMAL_LABEL);
  ex_10->labelfont(0);
  ex_10->labelsize(10);
  ex_10->labelcolor(FL_BACKGROUND2_COLOR);
  ex_10->minimum(-64);
  ex_10->maximum(64);
  ex_10->step(1);
  ex_10->textcolor(FL_BACKGROUND2_COLOR);
  ex_10->callback((Fl_Callback*)cb_ex_10);
  ex_10->align(Fl_Align(FL_ALIGN_LEFT));
  ex_10->when(FL_WHEN_CHANGED);
} // SliderW* ex_10
position(X, Y);
end();
}
