// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "midilearnwindow_gui.h"

void MidiLearnWindowGui::cb_Epar_i(RKR_Browser*, void*) {
  m_rgui->DisAssigns();
}
void MidiLearnWindowGui::cb_Epar(RKR_Browser* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_Epar_i(o,v);
}

void MidiLearnWindowGui::cb_GMM_i(RKR_Button*, void*) {
  m_rkr->ControlGet=0;
m_rkr->CountWait=0;
m_rkr->RControl=1;
}
void MidiLearnWindowGui::cb_GMM(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_GMM_i(o,v);
}

void MidiLearnWindowGui::cb_CopyF_i(RKR_Button*, void*) {
  memcpy(m_rkr->XUserMIDI,m_rkr->Bank[TPresets->value()].XUserMIDI,sizeof(m_rkr->XUserMIDI));
  m_rgui->DisAssigns();
}
void MidiLearnWindowGui::cb_CopyF(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_CopyF_i(o,v);
}

void MidiLearnWindowGui::cb_CopyT_i(RKR_Button*, void*) {
  memcpy(m_rkr->Bank[TPresets->value()].XUserMIDI,m_rkr->XUserMIDI, sizeof(m_rkr->XUserMIDI));
}
void MidiLearnWindowGui::cb_CopyT(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_CopyT_i(o,v);
}

void MidiLearnWindowGui::cb_ClearA_i(RKR_Button*, void*) {
  int i, j,k;
int the_one;

if (m_rkr->ML_filter==0)
 the_one = m_rkr->efx_params[(int)Epar->value()-1].Ato;
 else
 the_one = m_rkr->ML_clist[(int)Epar->value()-1];


for(i=0; i<128; i++)
  {
    for(j=0;j<20;j++)
      {
         if (m_rkr->XUserMIDI[i][j] == the_one)
           {
             for(k=j+1;k<20;k++) m_rkr->XUserMIDI[i][k-1]=m_rkr->XUserMIDI[i][k];
             m_rkr->XUserMIDI[i][19]=0;             
           }
       
       }  
   }
   
    m_rgui->DisAssigns();
}
void MidiLearnWindowGui::cb_ClearA(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_ClearA_i(o,v);
}

void MidiLearnWindowGui::cb_ClearP_i(RKR_Button*, void*) {
  memset(m_rkr->XUserMIDI, 0, sizeof(m_rkr->XUserMIDI));
    m_rgui->DisAssigns();
}
void MidiLearnWindowGui::cb_ClearP(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_ClearP_i(o,v);
}

void MidiLearnWindowGui::cb_Assign_i(RKR_Button*, void*) {
  int i;

int the_one = 0;

if((int)Epar->value()-1 >= 0)
{
    if (m_rkr->ML_filter==0)
        the_one = m_rkr->efx_params[(int)Epar->value()-1].Ato;
    else
        the_one = m_rkr->ML_clist[(int)Epar->value()-1];
}

for(i=0;i<20;i++)

 {
    if(m_rkr->XUserMIDI[(int)Disp_Control->value()][i] == the_one) return;

    if(m_rkr->XUserMIDI[(int)Disp_Control->value()][i] ==0)
       {
         m_rkr->XUserMIDI[(int)Disp_Control->value()][i]=the_one;
         break;
        }
 }
 
 
    m_rgui->DisAssigns();
}
void MidiLearnWindowGui::cb_Assign(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_Assign_i(o,v);
}

void MidiLearnWindowGui::cb_AssignA_i(RKR_Button*, void*) {
  int i,j;
int the_one = 0;

if((int)Epar->value()-1 >= 0)
{
    if (m_rkr->ML_filter==0)
        the_one = m_rkr->efx_params[(int)Epar->value()-1].Ato;
    else
        the_one = m_rkr->ML_clist[(int)Epar->value()-1];
}

for(j=1;j<61;j++)

{
for(i=0;i<20;i++)

 {
    if(m_rkr->Bank[j].XUserMIDI[(int)Disp_Control->value()][i] == the_one) break;

    if(m_rkr->Bank[j].XUserMIDI[(int)Disp_Control->value()][i] ==0)
       {
         m_rkr->Bank[j].XUserMIDI[(int)Disp_Control->value()][i]=the_one;
         break;
        }
 }
 
} 

Assign->do_callback();
}
void MidiLearnWindowGui::cb_AssignA(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_AssignA_i(o,v);
}

void MidiLearnWindowGui::cb_CancelRec_i(RKR_Button*, void*) {
  m_rkr->RControl = 0;
GMM->color(fore_color);
GMM->redraw();
}
void MidiLearnWindowGui::cb_CancelRec(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_CancelRec_i(o,v);
}

void MidiLearnWindowGui::cb_Disp_Control_i(RKR_Value_Input* o, void*) {
  if(o->value()> 127) o->value(127);
if(o->value()< 1) o->value(1);
}
void MidiLearnWindowGui::cb_Disp_Control(RKR_Value_Input* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_Disp_Control_i(o,v);
}

void MidiLearnWindowGui::cb_CopyTAll_i(RKR_Button*, void*) {
  int i;
for(i=1;i<61;i++) memcpy(m_rkr->Bank[i].XUserMIDI,m_rkr->XUserMIDI, sizeof(m_rkr->XUserMIDI));
}
void MidiLearnWindowGui::cb_CopyTAll(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_CopyTAll_i(o,v);
}

void MidiLearnWindowGui::cb_CloseML_i(RKR_Button*, void*) {
  do_callback();
}
void MidiLearnWindowGui::cb_CloseML(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()))->cb_CloseML_i(o,v);
}

void MidiLearnWindowGui::cb_M_fil_all_i(RKR_Button*, void*) {
  m_rkr->ML_filter=0;
      m_rgui->FillML();
}
void MidiLearnWindowGui::cb_M_fil_all(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()->parent()))->cb_M_fil_all_i(o,v);
}

void MidiLearnWindowGui::cb_M_fil_current_i(RKR_Button*, void*) {
  m_rkr->ML_filter=1;
      m_rgui->FillML();
}
void MidiLearnWindowGui::cb_M_fil_current(RKR_Button* o, void* v) {
  ((MidiLearnWindowGui*)(o->parent()->parent()))->cb_M_fil_current_i(o,v);
}
MidiLearnWindowGui::MidiLearnWindowGui(int X, int Y, int W, int H, const char *L)
  : Fl_Double_Window(X, Y, W, H, L) {
  _MidiLearnWindowGui();
}

MidiLearnWindowGui::MidiLearnWindowGui(int W, int H, const char *L)
  : Fl_Double_Window(0, 0, W, H, L) {
  clear_flag(16);
  _MidiLearnWindowGui();
}

MidiLearnWindowGui::MidiLearnWindowGui()
  : Fl_Double_Window(0, 0, 640, 480, 0) {
  clear_flag(16);
  _MidiLearnWindowGui();
}

void MidiLearnWindowGui::_MidiLearnWindowGui() {
this->box(FL_FLAT_BOX);
this->color((Fl_Color)22);
this->selection_color(FL_BACKGROUND2_COLOR);
this->labeltype(FL_NO_LABEL);
this->labelfont(0);
this->labelsize(14);
this->labelcolor(FL_FOREGROUND_COLOR);
this->align(Fl_Align(FL_ALIGN_TOP));
this->when(FL_WHEN_RELEASE);
{ Fondo4 = new Fl_Box(1, 1, 640, 480);
} // Fl_Box* Fondo4
{ RKR_Browser* o = Epar = new RKR_Browser(10, 58, 201, 348);
  Epar->type(2);
  Epar->box(FL_NO_BOX);
  Epar->color(FL_BACKGROUND2_COLOR);
  Epar->selection_color(FL_SELECTION_COLOR);
  Epar->labeltype(FL_NORMAL_LABEL);
  Epar->labelfont(0);
  Epar->labelsize(14);
  Epar->labelcolor(FL_FOREGROUND_COLOR);
  Epar->textcolor(FL_BACKGROUND2_COLOR);
  Epar->callback((Fl_Callback*)cb_Epar, (void*)(99));
  Epar->align(Fl_Align(FL_ALIGN_BOTTOM));
  Epar->when(FL_WHEN_RELEASE_ALWAYS);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Browser* Epar
{ RKR_Button* o = GMM = new RKR_Button(10, 445, 135, 30, "Get MIDI Message");
  GMM->box(FL_UP_BOX);
  GMM->color(FL_BACKGROUND_COLOR);
  GMM->selection_color(FL_BACKGROUND_COLOR);
  GMM->labeltype(FL_NORMAL_LABEL);
  GMM->labelfont(0);
  GMM->labelsize(14);
  GMM->labelcolor(FL_FOREGROUND_COLOR);
  GMM->callback((Fl_Callback*)cb_GMM, (void*)(77));
  GMM->align(Fl_Align(FL_ALIGN_CENTER));
  GMM->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* GMM
{ RKR_Browser* o = TPresets = new RKR_Browser(430, 61, 201, 348);
  TPresets->type(2);
  TPresets->box(FL_NO_BOX);
  TPresets->color(FL_BACKGROUND2_COLOR);
  TPresets->selection_color(FL_SELECTION_COLOR);
  TPresets->labeltype(FL_NORMAL_LABEL);
  TPresets->labelfont(0);
  TPresets->labelsize(14);
  TPresets->labelcolor(FL_FOREGROUND_COLOR);
  TPresets->textcolor(FL_BACKGROUND2_COLOR);
  TPresets->align(Fl_Align(FL_ALIGN_BOTTOM));
  TPresets->when(3);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Browser* TPresets
{ RKR_Button* o = CopyF = new RKR_Button(430, 20, 95, 30, "Copy from: ");
  CopyF->box(FL_UP_BOX);
  CopyF->color(FL_BACKGROUND_COLOR);
  CopyF->selection_color(FL_BACKGROUND_COLOR);
  CopyF->labeltype(FL_NORMAL_LABEL);
  CopyF->labelfont(0);
  CopyF->labelsize(14);
  CopyF->labelcolor(FL_FOREGROUND_COLOR);
  CopyF->callback((Fl_Callback*)cb_CopyF, (void*)(77));
  CopyF->align(Fl_Align(FL_ALIGN_CENTER));
  CopyF->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* CopyF
{ RKR_Button* o = CopyT = new RKR_Button(535, 20, 95, 30, "Copy to: ");
  CopyT->box(FL_UP_BOX);
  CopyT->color(FL_BACKGROUND_COLOR);
  CopyT->selection_color(FL_BACKGROUND_COLOR);
  CopyT->labeltype(FL_NORMAL_LABEL);
  CopyT->labelfont(0);
  CopyT->labelsize(14);
  CopyT->labelcolor(FL_FOREGROUND_COLOR);
  CopyT->callback((Fl_Callback*)cb_CopyT, (void*)(77));
  CopyT->align(Fl_Align(FL_ALIGN_CENTER));
  CopyT->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* CopyT
{ Ares = new Fl_Group(215, 60, 205, 180);
  Ares->box(FL_THIN_DOWN_BOX);
  Ares->align(Fl_Align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE));
  { RKR_Box* o = Ar1 = new RKR_Box(220, 65, 45, 30);
    Ar1->box(FL_DOWN_BOX);
    Ar1->color(FL_BACKGROUND_COLOR);
    Ar1->selection_color(FL_BACKGROUND_COLOR);
    Ar1->labeltype(FL_NORMAL_LABEL);
    Ar1->labelfont(0);
    Ar1->labelsize(14);
    Ar1->labelcolor(FL_FOREGROUND_COLOR);
    Ar1->align(Fl_Align(FL_ALIGN_CENTER));
    Ar1->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar1
  { RKR_Box* o = Ar2 = new RKR_Box(270, 65, 45, 30);
    Ar2->box(FL_DOWN_BOX);
    Ar2->color(FL_BACKGROUND_COLOR);
    Ar2->selection_color(FL_BACKGROUND_COLOR);
    Ar2->labeltype(FL_NORMAL_LABEL);
    Ar2->labelfont(0);
    Ar2->labelsize(14);
    Ar2->labelcolor(FL_FOREGROUND_COLOR);
    Ar2->align(Fl_Align(FL_ALIGN_CENTER));
    Ar2->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar2
  { RKR_Box* o = Ar3 = new RKR_Box(320, 65, 45, 30);
    Ar3->box(FL_DOWN_BOX);
    Ar3->color(FL_BACKGROUND_COLOR);
    Ar3->selection_color(FL_BACKGROUND_COLOR);
    Ar3->labeltype(FL_NORMAL_LABEL);
    Ar3->labelfont(0);
    Ar3->labelsize(14);
    Ar3->labelcolor(FL_FOREGROUND_COLOR);
    Ar3->align(Fl_Align(FL_ALIGN_CENTER));
    Ar3->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar3
  { RKR_Box* o = Ar4 = new RKR_Box(370, 65, 45, 30);
    Ar4->box(FL_DOWN_BOX);
    Ar4->color(FL_BACKGROUND_COLOR);
    Ar4->selection_color(FL_BACKGROUND_COLOR);
    Ar4->labeltype(FL_NORMAL_LABEL);
    Ar4->labelfont(0);
    Ar4->labelsize(14);
    Ar4->labelcolor(FL_FOREGROUND_COLOR);
    Ar4->align(Fl_Align(FL_ALIGN_CENTER));
    Ar4->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar4
  { RKR_Box* o = Ar5 = new RKR_Box(220, 100, 45, 30);
    Ar5->box(FL_DOWN_BOX);
    Ar5->color(FL_BACKGROUND_COLOR);
    Ar5->selection_color(FL_BACKGROUND_COLOR);
    Ar5->labeltype(FL_NORMAL_LABEL);
    Ar5->labelfont(0);
    Ar5->labelsize(14);
    Ar5->labelcolor(FL_FOREGROUND_COLOR);
    Ar5->align(Fl_Align(FL_ALIGN_CENTER));
    Ar5->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar5
  { RKR_Box* o = Ar6 = new RKR_Box(270, 100, 45, 30);
    Ar6->box(FL_DOWN_BOX);
    Ar6->color(FL_BACKGROUND_COLOR);
    Ar6->selection_color(FL_BACKGROUND_COLOR);
    Ar6->labeltype(FL_NORMAL_LABEL);
    Ar6->labelfont(0);
    Ar6->labelsize(14);
    Ar6->labelcolor(FL_FOREGROUND_COLOR);
    Ar6->align(Fl_Align(FL_ALIGN_CENTER));
    Ar6->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar6
  { RKR_Box* o = Ar7 = new RKR_Box(320, 100, 45, 30);
    Ar7->box(FL_DOWN_BOX);
    Ar7->color(FL_BACKGROUND_COLOR);
    Ar7->selection_color(FL_BACKGROUND_COLOR);
    Ar7->labeltype(FL_NORMAL_LABEL);
    Ar7->labelfont(0);
    Ar7->labelsize(14);
    Ar7->labelcolor(FL_FOREGROUND_COLOR);
    Ar7->align(Fl_Align(FL_ALIGN_CENTER));
    Ar7->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar7
  { RKR_Box* o = Ar8 = new RKR_Box(370, 100, 45, 30);
    Ar8->box(FL_DOWN_BOX);
    Ar8->color(FL_BACKGROUND_COLOR);
    Ar8->selection_color(FL_BACKGROUND_COLOR);
    Ar8->labeltype(FL_NORMAL_LABEL);
    Ar8->labelfont(0);
    Ar8->labelsize(14);
    Ar8->labelcolor(FL_FOREGROUND_COLOR);
    Ar8->align(Fl_Align(FL_ALIGN_CENTER));
    Ar8->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar8
  { RKR_Box* o = Ar9 = new RKR_Box(220, 135, 45, 30);
    Ar9->box(FL_DOWN_BOX);
    Ar9->color(FL_BACKGROUND_COLOR);
    Ar9->selection_color(FL_BACKGROUND_COLOR);
    Ar9->labeltype(FL_NORMAL_LABEL);
    Ar9->labelfont(0);
    Ar9->labelsize(14);
    Ar9->labelcolor(FL_FOREGROUND_COLOR);
    Ar9->align(Fl_Align(FL_ALIGN_CENTER));
    Ar9->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar9
  { RKR_Box* o = Ar10 = new RKR_Box(270, 135, 45, 30);
    Ar10->box(FL_DOWN_BOX);
    Ar10->color(FL_BACKGROUND_COLOR);
    Ar10->selection_color(FL_BACKGROUND_COLOR);
    Ar10->labeltype(FL_NORMAL_LABEL);
    Ar10->labelfont(0);
    Ar10->labelsize(14);
    Ar10->labelcolor(FL_FOREGROUND_COLOR);
    Ar10->align(Fl_Align(FL_ALIGN_CENTER));
    Ar10->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar10
  { RKR_Box* o = Ar11 = new RKR_Box(320, 135, 45, 30);
    Ar11->box(FL_DOWN_BOX);
    Ar11->color(FL_BACKGROUND_COLOR);
    Ar11->selection_color(FL_BACKGROUND_COLOR);
    Ar11->labeltype(FL_NORMAL_LABEL);
    Ar11->labelfont(0);
    Ar11->labelsize(14);
    Ar11->labelcolor(FL_FOREGROUND_COLOR);
    Ar11->align(Fl_Align(FL_ALIGN_CENTER));
    Ar11->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar11
  { RKR_Box* o = Ar12 = new RKR_Box(370, 135, 45, 30);
    Ar12->box(FL_DOWN_BOX);
    Ar12->color(FL_BACKGROUND_COLOR);
    Ar12->selection_color(FL_BACKGROUND_COLOR);
    Ar12->labeltype(FL_NORMAL_LABEL);
    Ar12->labelfont(0);
    Ar12->labelsize(14);
    Ar12->labelcolor(FL_FOREGROUND_COLOR);
    Ar12->align(Fl_Align(FL_ALIGN_CENTER));
    Ar12->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar12
  { RKR_Box* o = Ar13 = new RKR_Box(220, 170, 45, 30);
    Ar13->box(FL_DOWN_BOX);
    Ar13->color(FL_BACKGROUND_COLOR);
    Ar13->selection_color(FL_BACKGROUND_COLOR);
    Ar13->labeltype(FL_NORMAL_LABEL);
    Ar13->labelfont(0);
    Ar13->labelsize(14);
    Ar13->labelcolor(FL_FOREGROUND_COLOR);
    Ar13->align(Fl_Align(FL_ALIGN_CENTER));
    Ar13->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar13
  { RKR_Box* o = Ar14 = new RKR_Box(270, 170, 45, 30);
    Ar14->box(FL_DOWN_BOX);
    Ar14->color(FL_BACKGROUND_COLOR);
    Ar14->selection_color(FL_BACKGROUND_COLOR);
    Ar14->labeltype(FL_NORMAL_LABEL);
    Ar14->labelfont(0);
    Ar14->labelsize(14);
    Ar14->labelcolor(FL_FOREGROUND_COLOR);
    Ar14->align(Fl_Align(FL_ALIGN_CENTER));
    Ar14->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar14
  { RKR_Box* o = Ar15 = new RKR_Box(320, 170, 45, 30);
    Ar15->box(FL_DOWN_BOX);
    Ar15->color(FL_BACKGROUND_COLOR);
    Ar15->selection_color(FL_BACKGROUND_COLOR);
    Ar15->labeltype(FL_NORMAL_LABEL);
    Ar15->labelfont(0);
    Ar15->labelsize(14);
    Ar15->labelcolor(FL_FOREGROUND_COLOR);
    Ar15->align(Fl_Align(FL_ALIGN_CENTER));
    Ar15->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar15
  { RKR_Box* o = Ar16 = new RKR_Box(370, 170, 45, 30);
    Ar16->box(FL_DOWN_BOX);
    Ar16->color(FL_BACKGROUND_COLOR);
    Ar16->selection_color(FL_BACKGROUND_COLOR);
    Ar16->labeltype(FL_NORMAL_LABEL);
    Ar16->labelfont(0);
    Ar16->labelsize(14);
    Ar16->labelcolor(FL_FOREGROUND_COLOR);
    Ar16->align(Fl_Align(FL_ALIGN_CENTER));
    Ar16->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar16
  { RKR_Box* o = Ar17 = new RKR_Box(220, 205, 45, 30);
    Ar17->box(FL_DOWN_BOX);
    Ar17->color(FL_BACKGROUND_COLOR);
    Ar17->selection_color(FL_BACKGROUND_COLOR);
    Ar17->labeltype(FL_NORMAL_LABEL);
    Ar17->labelfont(0);
    Ar17->labelsize(14);
    Ar17->labelcolor(FL_FOREGROUND_COLOR);
    Ar17->align(Fl_Align(FL_ALIGN_CENTER));
    Ar17->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar17
  { RKR_Box* o = Ar18 = new RKR_Box(270, 205, 45, 30);
    Ar18->box(FL_DOWN_BOX);
    Ar18->color(FL_BACKGROUND_COLOR);
    Ar18->selection_color(FL_BACKGROUND_COLOR);
    Ar18->labeltype(FL_NORMAL_LABEL);
    Ar18->labelfont(0);
    Ar18->labelsize(14);
    Ar18->labelcolor(FL_FOREGROUND_COLOR);
    Ar18->align(Fl_Align(FL_ALIGN_CENTER));
    Ar18->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar18
  { RKR_Box* o = Ar19 = new RKR_Box(320, 205, 45, 30);
    Ar19->box(FL_DOWN_BOX);
    Ar19->color(FL_BACKGROUND_COLOR);
    Ar19->selection_color(FL_BACKGROUND_COLOR);
    Ar19->labeltype(FL_NORMAL_LABEL);
    Ar19->labelfont(0);
    Ar19->labelsize(14);
    Ar19->labelcolor(FL_FOREGROUND_COLOR);
    Ar19->align(Fl_Align(FL_ALIGN_CENTER));
    Ar19->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar19
  { RKR_Box* o = Ar20 = new RKR_Box(370, 205, 45, 30);
    Ar20->box(FL_DOWN_BOX);
    Ar20->color(FL_BACKGROUND_COLOR);
    Ar20->selection_color(FL_BACKGROUND_COLOR);
    Ar20->labeltype(FL_NORMAL_LABEL);
    Ar20->labelfont(0);
    Ar20->labelsize(14);
    Ar20->labelcolor(FL_FOREGROUND_COLOR);
    Ar20->align(Fl_Align(FL_ALIGN_CENTER));
    Ar20->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Box* Ar20
  Ares->end();
} // Fl_Group* Ares
{ RKR_Button* o = ClearA = new RKR_Button(245, 250, 150, 30, "Clear Assignements");
  ClearA->box(FL_UP_BOX);
  ClearA->color(FL_BACKGROUND_COLOR);
  ClearA->selection_color(FL_BACKGROUND_COLOR);
  ClearA->labeltype(FL_NORMAL_LABEL);
  ClearA->labelfont(0);
  ClearA->labelsize(14);
  ClearA->labelcolor(FL_FOREGROUND_COLOR);
  ClearA->callback((Fl_Callback*)cb_ClearA, (void*)(77));
  ClearA->align(Fl_Align(FL_ALIGN_CENTER));
  ClearA->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* ClearA
{ RKR_Button* o = ClearP = new RKR_Button(245, 285, 150, 30, "Clear Preset");
  ClearP->box(FL_UP_BOX);
  ClearP->color(FL_BACKGROUND_COLOR);
  ClearP->selection_color(FL_BACKGROUND_COLOR);
  ClearP->labeltype(FL_NORMAL_LABEL);
  ClearP->labelfont(0);
  ClearP->labelsize(14);
  ClearP->labelcolor(FL_FOREGROUND_COLOR);
  ClearP->callback((Fl_Callback*)cb_ClearP, (void*)(77));
  ClearP->align(Fl_Align(FL_ALIGN_CENTER));
  ClearP->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* ClearP
{ RKR_Button* o = Assign = new RKR_Button(112, 20, 98, 30, "Assign");
  Assign->box(FL_UP_BOX);
  Assign->color(FL_BACKGROUND_COLOR);
  Assign->selection_color(FL_BACKGROUND_COLOR);
  Assign->labeltype(FL_NORMAL_LABEL);
  Assign->labelfont(0);
  Assign->labelsize(14);
  Assign->labelcolor(FL_FOREGROUND_COLOR);
  Assign->callback((Fl_Callback*)cb_Assign, (void*)(77));
  Assign->align(Fl_Align(FL_ALIGN_CENTER));
  Assign->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* Assign
{ RKR_Button* o = AssignA = new RKR_Button(10, 20, 98, 30, "Assign to All");
  AssignA->box(FL_UP_BOX);
  AssignA->color(FL_BACKGROUND_COLOR);
  AssignA->selection_color(FL_BACKGROUND_COLOR);
  AssignA->labeltype(FL_NORMAL_LABEL);
  AssignA->labelfont(0);
  AssignA->labelsize(14);
  AssignA->labelcolor(FL_FOREGROUND_COLOR);
  AssignA->callback((Fl_Callback*)cb_AssignA, (void*)(77));
  AssignA->align(Fl_Align(FL_ALIGN_CENTER));
  AssignA->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* AssignA
{ RKR_Button* o = CancelRec = new RKR_Button(200, 450, 60, 20, "Cancel");
  CancelRec->box(FL_UP_BOX);
  CancelRec->color(FL_BACKGROUND_COLOR);
  CancelRec->selection_color(FL_BACKGROUND_COLOR);
  CancelRec->labeltype(FL_NORMAL_LABEL);
  CancelRec->labelfont(0);
  CancelRec->labelsize(14);
  CancelRec->labelcolor(FL_FOREGROUND_COLOR);
  CancelRec->callback((Fl_Callback*)cb_CancelRec, (void*)(77));
  CancelRec->align(Fl_Align(FL_ALIGN_CENTER));
  CancelRec->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* CancelRec
{ RKR_Value_Input* o = Disp_Control = new RKR_Value_Input(155, 445, 40, 30);
  Disp_Control->box(FL_DOWN_BOX);
  Disp_Control->color(FL_BACKGROUND2_COLOR);
  Disp_Control->selection_color(FL_SELECTION_COLOR);
  Disp_Control->labeltype(FL_NORMAL_LABEL);
  Disp_Control->labelfont(0);
  Disp_Control->labelsize(14);
  Disp_Control->labelcolor(FL_FOREGROUND_COLOR);
  Disp_Control->minimum(1);
  Disp_Control->maximum(127);
  Disp_Control->step(1);
  Disp_Control->value(1);
  Disp_Control->textcolor(FL_BACKGROUND2_COLOR);
  Disp_Control->callback((Fl_Callback*)cb_Disp_Control);
  Disp_Control->align(Fl_Align(FL_ALIGN_LEFT));
  Disp_Control->when(FL_WHEN_CHANGED);
  o->m_start_text_offset = 4; // 10 - 14
} // RKR_Value_Input* Disp_Control
{ RKR_Button* o = CopyTAll = new RKR_Button(480, 412, 150, 30, "Copy to All");
  CopyTAll->box(FL_UP_BOX);
  CopyTAll->color(FL_BACKGROUND_COLOR);
  CopyTAll->selection_color(FL_BACKGROUND_COLOR);
  CopyTAll->labeltype(FL_NORMAL_LABEL);
  CopyTAll->labelfont(0);
  CopyTAll->labelsize(14);
  CopyTAll->labelcolor(FL_FOREGROUND_COLOR);
  CopyTAll->callback((Fl_Callback*)cb_CopyTAll, (void*)(77));
  CopyTAll->align(Fl_Align(FL_ALIGN_CENTER));
  CopyTAll->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* CopyTAll
{ RKR_Button* o = CloseML = new RKR_Button(480, 445, 150, 30, "Close");
  CloseML->box(FL_UP_BOX);
  CloseML->color(FL_BACKGROUND_COLOR);
  CloseML->selection_color(FL_BACKGROUND_COLOR);
  CloseML->labeltype(FL_NORMAL_LABEL);
  CloseML->labelfont(0);
  CloseML->labelsize(14);
  CloseML->labelcolor(FL_FOREGROUND_COLOR);
  CloseML->callback((Fl_Callback*)cb_CloseML, (void*)(77));
  CloseML->align(Fl_Align(FL_ALIGN_CENTER));
  CloseML->when(FL_WHEN_RELEASE);
  o->m_start_font_offset = 4; // 10 - 14
} // RKR_Button* CloseML
{ Filters_ML = new Fl_Group(10, 410, 200, 31);
  Filters_ML->box(FL_DOWN_BOX);
  { RKR_Button* o = M_fil_all = new RKR_Button(16, 415, 89, 20, "All");
    M_fil_all->type(102);
    M_fil_all->box(FL_UP_BOX);
    M_fil_all->color(FL_BACKGROUND_COLOR);
    M_fil_all->selection_color(FL_BACKGROUND_COLOR);
    M_fil_all->labeltype(FL_NORMAL_LABEL);
    M_fil_all->labelfont(0);
    M_fil_all->labelsize(14);
    M_fil_all->labelcolor(FL_FOREGROUND_COLOR);
    M_fil_all->callback((Fl_Callback*)cb_M_fil_all);
    M_fil_all->align(Fl_Align(FL_ALIGN_CENTER));
    M_fil_all->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Button* M_fil_all
  { RKR_Button* o = M_fil_current = new RKR_Button(113, 415, 89, 20, "Current");
    M_fil_current->type(102);
    M_fil_current->box(FL_UP_BOX);
    M_fil_current->color(FL_BACKGROUND_COLOR);
    M_fil_current->selection_color(FL_BACKGROUND_COLOR);
    M_fil_current->labeltype(FL_NORMAL_LABEL);
    M_fil_current->labelfont(0);
    M_fil_current->labelsize(14);
    M_fil_current->labelcolor(FL_FOREGROUND_COLOR);
    M_fil_current->callback((Fl_Callback*)cb_M_fil_current);
    M_fil_current->align(Fl_Align(FL_ALIGN_CENTER));
    M_fil_current->when(FL_WHEN_RELEASE);
    o->m_start_font_offset = 4; // 10 - 14
  } // RKR_Button* M_fil_current
  Filters_ML->end();
} // Fl_Group* Filters_ML
this->m_rkr = NULL;
this->m_rgui = NULL;
end();
resizable(this);
}

void MidiLearnWindowGui::initialize(RKR *_rkr,RKRGUI *_rgui ) {
  m_rkr = _rkr;
  m_rgui= _rgui;
}
