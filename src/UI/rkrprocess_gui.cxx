/*
  rakarrack - a guitar effects software

  rkrprocess_gui.cxx  -  RKRGUI class definitions
  Copyright (C) 2008-2010 Josep Andreu
  Author: Josep Andreu & Ryan Billing

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License
 (version2)  along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

/*
  The following function definitions are declared in the rakarrack.h header file. 
  Any changes to these function prototype declarations need to be changed in the 
  header using the fluid IDE since fluid generates the header. These definitions
  were moved to this new file on 2/17/18.
 */

#include "rakarrack.h"
#include "../icono_rakarrack_128x128.xpm"
static Fl_Widget *old;
static int last_tecla;
static int drag;
static int at;
static Pixmap p, mask;
static XWMHints *hints = NULL;
static volatile int got_sigint = 0;
static volatile int got_sigusr1 = 0;

RKRGUI::RKRGUI(int argc, char**argv, RKR *rkr_) :
    made()
{
    // Initialize Gui
    Fl::args(argc, argv);
    fl_open_display();
    XpmCreatePixmapFromData(fl_display, DefaultRootWindow(fl_display), (char **) icono_rakarrack_128x128, &p, &mask, NULL);

    Fl::visual(FL_DOUBLE | FL_RGB);
    fl_register_images();
    Fl::set_fonts(0);
    rkr = rkr_;

    mBankNameList = NULL;
    mBankNameListTail = NULL;

    back = NULL;
    old = NULL;
    make_window();

    Principal->icon((char *) p);
    BankWindow->icon((char *) p);
    Order->icon((char *) p);
    Settings->icon((char *) p);
    AboutWin->icon((char *) p);
    MIDILearn->icon((char *) p);
    Trigger->icon((char *) p);

    last_tecla = 0;
    made = 0;
    char tmp[256];
    drag = 1000;
    at = 0;
    Analy->set_analyzer_ON(false);
    Sco->set_scope_ON(false);

    Sco->init(rkr->anall, rkr->analr, rkr->period);
    Analy->init(rkr->anall, rkr->analr, rkr->period, rkr->sample_rate);

    memset(tmp, 0, sizeof (tmp));
    if (filetoload != NULL)
    {
        sprintf(tmp, "Session: %s", filetoload);
    }
    else
    {
        sprintf(tmp, "%s   v%s", rkr->jackcliname, VERSION);
    }

    Principal->copy_label(tmp);
    BankWin_Label(rkr->BankFilename);
    memset(tmp, 0, sizeof (tmp));
    sprintf(tmp, "%s   v%s - Effects Order", rkr->jackcliname, VERSION);
    Order->copy_label(tmp);
    memset(tmp, 0, sizeof (tmp));
    sprintf(tmp, "%s   v%s - Settings", rkr->jackcliname, VERSION);
    Settings->copy_label(tmp);
    sprintf(tmp, "%s   v%s - MIDI Learn", rkr->jackcliname, VERSION);
    MIDILearn->copy_label(tmp);
    sprintf(tmp, "%s   v%s - ACI", rkr->jackcliname, VERSION);
    Trigger->copy_label(tmp);
    
    load_previous_state();
    Settings->make_table_window();
    Put_Loaded();
    Principal->show(argc, argv);
    put_icon(Principal);
    ReadIntPresets();
    
    if (rkr->deachide)
    {
        HideUE->label("Show");
    }
    else
    {
        HideUE->label("Hide");
    }
    
    HideUE->redraw();

    Fl::add_timeout(.04, this->TimeoutStatic, this);
    Fl::add_handler(prevnext);
    install_signal_handlers();
}

/**
 Main FLTK gui timeout
 */
void RKRGUI::TimeoutStatic(void* ptr)
{
    // Main gui timeout
    ((RKRGUI*) ptr)->GuiTimeout();
}

void RKRGUI::GuiTimeout(void)
{
    // Main gui process on timeout
    highlight();
    drag_effect();
    check_signals(this);

    if (rkr->Tuner_Bypass)
    {

        if (rkr->efx_Tuner->note_actual != rkr->note_old)
        {
            char notas[3];
            sprintf(notas, "%-2s", rkr->efx_Tuner->notes[rkr->efx_Tuner->note_actual]);
            WNote->copy_label(notas);
            rkr->note_old = rkr->efx_Tuner->note_actual;
        }

        if (rkr->efx_Tuner->nfreq != rkr->nfreq_old)
        {
            char refreq[60];
            sprintf(refreq, "%8.3f", rkr->efx_Tuner->nfreq);
            WRfreq->copy_label(refreq);
            rkr->nfreq_old = rkr->efx_Tuner->nfreq;
        }

        //if(rkr->efx_Tuner->afreq != rkr->afreq_old)
        if (fabsf(rkr->efx_Tuner->afreq - rkr->afreq_old) > .2f)
        {
            char nofreq[60];
            sprintf(nofreq, "%8.3f", rkr->efx_Tuner->afreq);
            if (abs(rkr->efx_Tuner->cents) > 5)
                WNfreq->labelcolor(FL_RED);
            else
                WNfreq->labelcolor(label_color);
            WNfreq->copy_label(nofreq);
            rkr->afreq_old = rkr->efx_Tuner->afreq;
        }

        if (rkr->efx_Tuner->afreq == 0.0f)
        {
            char notas[3];
            sprintf(notas, "  ");
            WNote->copy_label(notas);
        }


        if (rkr->efx_Tuner->cents != rkr->cents_old)
        {
            tuner_bar->value(rkr->efx_Tuner->cents);
            rkr->cents_old = rkr->efx_Tuner->cents;
        }
    }

    if (stecla == 1)
    {
        if (rkr->Selected_Preset < 60)
        {
            Preset_Counter->value(rkr->Selected_Preset + 1);
            Preset_Counter->do_callback();
        }

        stecla = 0;
    }


    if (stecla == 2)
    {
        if (rkr->Selected_Preset > 1)
        {
            Preset_Counter->value(rkr->Selected_Preset - 1);
            Preset_Counter->do_callback();
        }

        stecla = 0;
    }

    if (stecla == 3)
    {
        if (Nivel_Salida->value()>-50)
        {
            Nivel_Salida->value(Nivel_Salida->value() - 1);
            Nivel_Salida->do_callback();
            Nivel_Salida->redraw();
        }

        stecla = 0;
    }


    if (stecla == 4)
    {
        if (Nivel_Salida->value() < 50)
        {
            Nivel_Salida->value(Nivel_Salida->value() + 1);
            Nivel_Salida->do_callback();
            Nivel_Salida->redraw();
        }

        stecla = 0;
    }


    if (stecla == 5)
    {
        update_looper();
        stecla = 0;
    }


    if (preset != 1000)
    {
        if (!rkr->midi_table)
        {
            Preset_Counter->value(preset);
            Preset_Counter->do_callback();
        }
        else
        {
            if (rkr->a_bank != rkr->M_table[preset].bank)
            {
                switch (rkr->M_table[preset].bank)
                {
                    case 0:
                        L_B1->do_callback();
                        break;

                    case 1:
                        L_B2->do_callback();
                        break;

                    case 2:
                        L_B3->do_callback();
                        break;

                    case 3:
                        L_B4->do_callback();
                        break;
                }
            }
            Preset_Counter->value(rkr->M_table[preset].preset + 1);
            Preset_Counter->do_callback();
        }

        preset = 1000;
    }


    MIDI_control_gui_refresh();
    if (rkr->ACI_Bypass)
        ActACI();

    rkr->cpufp++;
    if (rkr->cpufp == 40)
    {
        char tmp[8];
        memset(tmp, 0, sizeof (tmp));
        sprintf(tmp, "%5.2f%%", rkr->cpuload);
        CPULOAD->copy_label(tmp);
        rkr->cpufp = 0;
    }


    if (rkr->numpc)
    {
        if (rkr->numpi)
            P_IN_ST->show();
        else
            P_IN_ST->hide();
        if (rkr->numpo)
            P_OUT_ST->show();
        else
            P_OUT_ST->hide();
        if (rkr->numpa)
            P_AUX_ST->show();
        else
            P_AUX_ST->hide();
        if (rkr->numpmi)
            P_MIN_ST->show();
        else
            P_MIN_ST->hide();
        if (rkr->numpmo)
            P_MOUT_ST->show();
        else
            P_MOUT_ST->hide();
        rkr->numpc = 0;
    }

    if (error_num > 0)
        rkr->Error_Handle(error_num);

    if (rkr->Tap_Bypass)
    {
        if ((rkr->Tap_Display == 1) && (rkr->Tap_Selection > 0))
        {
            T_DIS->value(rkr->Tap_TempoSet);
            UpdateTGUI();
            rkr->Tap_Display = 0;
        }

        if (rkr->Tap_Display == 2)
        {
            rkr->Tap_Display = 0;
            Tap_activar->value(0);
            Tap_activar->do_callback();
        }
    }


    if (rkr->Bypass)
    {
        if (rkr->val_il_sum != rkr->old_il_sum)
        {
            input_vul->value(rkr->val_il_sum);
            rkr->old_il_sum = rkr->val_il_sum;
        }

        if (rkr->val_ir_sum != rkr->old_ir_sum)
        {
            input_vur->value(rkr->val_ir_sum);
            rkr->old_ir_sum = rkr->val_ir_sum;
        }

        if (rkr->val_vl_sum != rkr->old_vl_sum)
        {
            output_vul->value(rkr->val_vl_sum);
            rkr->old_vl_sum = rkr->val_vl_sum;
        }

        if (rkr->val_vr_sum != rkr->old_vr_sum)
        {
            output_vur->value(rkr->val_vr_sum);
            rkr->old_vr_sum = rkr->val_vr_sum;
        }


        if (rkr->efx_FLimiter->clipping)
        {
            CLIP_LED->color(FL_RED);
            CLIP_LED->redraw();
        }
        else
        {
            CLIP_LED->color(fl_darker(FL_BLUE));
            CLIP_LED->redraw();
        }

        if (rkr->efx_FLimiter->limit)
        {
            LMT_LED->color(leds_color);
            LMT_LED->redraw();
        }
        else
        {
            LMT_LED->color(fl_darker(leds_color));
            LMT_LED->redraw();
        }

        rkr->efx_FLimiter->clipping = 0;
        rkr->efx_FLimiter->limit = 0;


        if (rkr->checkforaux())
        {
            Vocoder *Efx_Vocoder = static_cast <Vocoder*>(rkr->Rack_Effects[EFX_VOCODER]);
            
            if (VOCODER->vu_vu->value() != Efx_Vocoder->vulevel)
            {
                VOCODER->vu_vu->value(Efx_Vocoder->vulevel);
            }
        }

        if (Sco->get_scope_ON())
        {
            Sco->redraw();
        }

        if (Analy->get_analyzer_ON())
        {
            at++;
            if (at == 3)
            {
                Analy->redraw();
                at = 0;
            }
        }
        else
        {
            at = 0;
        }

        if (rkr->EFX_Bypass[EFX_LOOPER])
        {
            Looper *Efx_Looper = static_cast <Looper*> (rkr->Rack_Effects[EFX_LOOPER]);
            if ((Efx_Looper->Pplay) && (!Efx_Looper->Pstop))
            {
                if (looper_lqua != Efx_Looper->looper_qua)
                {
                    looper_lqua = Efx_Looper->looper_qua;
                    char tmp[16];
                    memset(tmp, 0, sizeof (tmp));
                    sprintf(tmp, "%d/%d", Efx_Looper->looper_bar, Efx_Looper->looper_qua);
                    LOOPER->L_TimePos->copy_label(tmp);
                }
            }
        }
    }


    if ((rkr->Metro_Bypass) && (MetroSound->value() == 0))
    {
        if ((!rkr->M_Metronome->markctr) && ((int) Metro_Led->color() != (int) fl_lighter(FL_RED)))
        {
            Metro_Led->color(fl_lighter(FL_RED));
            Metro_Led->redraw();
        }

        if ((rkr->M_Metronome->markctr) && ((int) Metro_Led->color() != (int) fl_darker(FL_RED)))
        {
            Metro_Led->color(fl_darker(FL_RED));
            Metro_Led->redraw();
        }
    }


    if (rkr->MIDIConverter_Bypass)
    {
        if ((rkr->efx_MIDIConverter->hay) && ((int) MIDI->Mled->color() != (int) fl_lighter(FL_RED)))
        {
            MIDI->Mled->color(fl_lighter(FL_RED));
            MIDI->Mled->redraw();
        }

        if ((!rkr->efx_MIDIConverter->hay) && ((int) MIDI->Mled->color() != (int) fl_darker(FL_RED)))
        {
            MIDI->Mled->color(fl_darker(FL_RED));
            MIDI->Mled->redraw();
        }
    }


    if (rkr->EFX_Bypass[EFX_HARMONIZER])
    {
        Harmonizer *Efx_Harmonizer = static_cast <Harmonizer*> (rkr->Rack_Effects[EFX_HARMONIZER]);
        if ((Efx_Harmonizer->PSELECT) || (Efx_Harmonizer->PMIDI))
        {
            if (rkr->RC_Harm->cc)
            {
                HAR->har_chordname->copy_label(rkr->RC_Harm->NombreAcorde);
                rkr->RC_Harm->cc = 0;
                HAR->har_chordname->redraw();
                rkr->RC_Harm->Vamos(0, Efx_Harmonizer->Pinterval - 12, rkr->HarmRecNote->reconota);
                Efx_Harmonizer->r_ratio = rkr->RC_Harm->r__ratio[0]; //pass the found ratio
            }
        }
    }


    if (rkr->EFX_Bypass[EFX_STEREOHARM])
    {
        StereoHarm *Efx_StereoHarm = static_cast<StereoHarm*>(rkr->Rack_Effects[EFX_STEREOHARM]);

        if ((Efx_StereoHarm->PSELECT) || (Efx_StereoHarm->PMIDI))
        {
            if (rkr->RC_Stereo_Harm->cc)
            {
                SHAR->shar_chordname->copy_label(rkr->RC_Stereo_Harm->NombreAcorde);
                rkr->RC_Stereo_Harm->cc = 0;
                SHAR->shar_chordname->redraw();
                rkr->RC_Stereo_Harm->Vamos(1, Efx_StereoHarm->Pintervall - 12, rkr->StHarmRecNote->reconota);
                rkr->RC_Stereo_Harm->Vamos(2, Efx_StereoHarm->Pintervalr - 12, rkr->StHarmRecNote->reconota);
                Efx_StereoHarm->r_ratiol = rkr->RC_Stereo_Harm->r__ratio[1];
                Efx_StereoHarm->r_ratior = rkr->RC_Stereo_Harm->r__ratio[2];
            }
        }
    }


    if (rkr->RControl)
    {
        rkr->CountWait++;
        if (rkr->CountWait == 25)
        {
            MIDILearn->GMM->color(leds_color);
            MIDILearn->GMM->redraw();
        }

        if (rkr->CountWait == 50)
        {
            MIDILearn->GMM->color(fore_color);
            MIDILearn->GMM->redraw();
            rkr->CountWait = 0;
        }

        if (rkr->ControlGet != 0)
        {
            rkr->RControl = 0;
            MIDILearn->Disp_Control->value(rkr->ControlGet);
            MIDILearn->Disp_Control->redraw();
            MIDILearn->GMM->color(fore_color);
            MIDILearn->GMM->redraw();
            if ((rkr->comemouse) && (rkr->autoassign))
            {
                rkr->comemouse = 0;
                MIDILearn->Assign->do_callback();
            }
        }
    };
    
    if(rkr->change_scale)
    {
        rkr->change_scale = 0;  // false
        if (rkr->scalable )
        {
            Principal->size_range(384, 216, 3840, 2160, 0, 0, 1);
        }
        else
        {
            Principal->size_range(320, 240, 3200, 2400);
        }
    }

    Fl::repeat_timeout(.04, this->TimeoutStatic, this);
}

void RKRGUI::Background_Color_Change(Fl_Color bcolor)
{
    // change the gui background color if png is not used
    MenuP->color(bcolor);
    BankWindow->MenuB->color(bcolor);

    back_color = bcolor;

    Label_Color_Change(label_color);
    
    if (!rkr->EnableBackgroundImage)
    {
        back->color_average(back_color, 0.0);
    }
}

void RKRGUI::Label_Color_Change(Fl_Color bcolor)
{
    // change the label colors
    label_color = bcolor;
    chfsize(C_DONT_CHANGE_FONT_SIZE);


    for (int t = 0; t < Settings->STabs->children(); t++)
    {
        Fl_Widget *tabg = Settings->STabs->child(t);
        tabg->labelcolor(label_color);
        tabg->selection_color(back_color);
        tabg->color(fore_color);

        long long ud = (long long) tabg->user_data();

        if (ud == 1)
        {
            Fl_Group *g = (Fl_Group *) tabg;

            for (int i = 0; i < g->children(); i++)
            {
                Fl_Widget *w = g->child(i);

                w->labelcolor(label_color);
                w->selection_color(back_color);
                w->color(fore_color);
                
                long long ugd = (long long) w->user_data();
                
                if(ugd == GROUP_USER_DATA)
                {
                    Fl_Group *gg = (Fl_Group *) w;
                    for (int i = 0; i < gg->children(); i++)
                    {
                        Fl_Widget *ww = gg->child(i);

                        ww->labelcolor(label_color);
                        ww->selection_color(back_color);
                        ww->color(fore_color);
                    }
                }
                
            }
        }
    }

    for (int t = 0; t < BankWindow->children(); t++)
    {
        Fl_Widget *w = BankWindow->child(t);

        w->labelcolor(label_color);
    }


    for (int t = 0; t < BankWindow->ob->children(); t++)
    {
        Fl_Widget *w = BankWindow->ob->child(t);

        w->labelcolor(label_color);
    }


    for (int t = 0; t < Order->children(); t++)
    {
        Fl_Widget *w = Order->child(t);

        w->labelcolor(label_color);
        w->selection_color(back_color);
        w->color(fore_color);
    }

    for (int t = 0; t < Order->Filters->children(); t++)
    {
        Fl_Widget *w = Order->Filters->child(t);

        w->labelcolor(label_color);
        w->selection_color(back_color);
        w->color(fore_color);
    }


    for (int t = 0; t < MIDILearn->children(); t++)
    {
        Fl_Widget *w = MIDILearn->child(t);

        w->labelcolor(label_color);
        w->selection_color(back_color);
        w->color(fore_color);
    }

    Settings->Font_Bro->textcolor(label_color);
    Trigger->ACI_LABEL->labelcolor(fl_lighter(label_color));
    Trigger->aux_source->labelcolor(label_color);
    Trigger->aux_midi->labelcolor(label_color);

    Fl::redraw();
}

void RKRGUI::Buttons_Color_Change(Fl_Color bcolor)
{
    // change the button colors
    if (made)
    {
        for (int t = 0; t < BankWindow->ob->children(); t++)
        {
            Fl_Widget *w = BankWindow->ob->child(t);
            long long temp = (long long) w->user_data();
            
            if (temp > 0)
            {
                w->color(bcolor);
            }
        }

        BankWindow->B_B1->color(bcolor);
        BankWindow->B_B2->color(bcolor);
        BankWindow->B_B3->color(bcolor);
        BankWindow->B_B4->color(bcolor);
        BankWindow->CH_UB->color(bcolor);
    }


    fore_color = bcolor;
    Trigger->aux_source->color(bcolor);
    Label_Color_Change(label_color);
}

void RKRGUI::Leds_Color_Change(Fl_Color bcolor)
{
    // change the leds color
    leds_color = bcolor;
    chfsize(C_DONT_CHANGE_FONT_SIZE);
}

void RKRGUI::put_icon(Fl_Window* window)
{
    // put icon
    if (hints == NULL)
    {
        hints = XGetWMHints(fl_display, fl_xid(window));
    }

    hints->icon_pixmap = p;
    hints->icon_mask = mask;
    hints->flags = IconPixmapHint | IconMaskHint;
    XSetWMHints(fl_display, fl_xid(window), hints);
}

/**
 *  Loads the previous state when shutdown, window sizes, images, etc.
 *  From ~user/.fltk/github.com.Stazed.rakarrack.plus/rakarrack-plus.prefs
 */
void RKRGUI::load_previous_state()
{
    int x, y, w, h, k, b, f, l, a;
    x = y = w = h = k = b = f = l = a = 0;

    Fl_Preferences rakarrack(Fl_Preferences::USER, WEBSITE, PACKAGE);

    rakarrack.get(rkr->PrefNom("Principal X"), x, 1);
    rakarrack.get(rkr->PrefNom("Principal Y"), y, 1);
    rakarrack.get(rkr->PrefNom("Principal W"), w, 960);
    rakarrack.get(rkr->PrefNom("Principal H"), h, 540);

    char tmp[256];
    sprintf(tmp, "%s/blackbg.png", DATADIR);

    rakarrack.get(rkr->PrefNom("Enable Background Image"), rkr->EnableBackgroundImage, 1);
    rakarrack.get(rkr->PrefNom("Background Image"), rkr->BackgroundImage, tmp, 256);
    PutBackground();
    rakarrack.get(rkr->PrefNom("FontSize"), rkr->fontsize, C_DEFAULT_FONT_SIZE);
    if (rkr->fontsize != C_DEFAULT_FONT_SIZE)
        chfsize(rkr->fontsize);
    rakarrack.get(rkr->PrefNom("Font"), rkr->font, 0);

    rakarrack.get(rkr->PrefNom("Background Color"), b, 56);
    rakarrack.get(rkr->PrefNom("Foreground Color"), f, 34499072);
    rakarrack.get(rkr->PrefNom("Leds Color"), l, -13485824);
    rakarrack.get(rkr->PrefNom("Labels Color"), a, -1075849984);
    rakarrack.get(rkr->PrefNom("Schema"), k, 4);
    Settings->scheme_ch->value(k);
    Settings->scheme_ch->do_callback();
    rakarrack.get(rkr->PrefNom("Hide Effects"), rkr->deachide, 0);
    rakarrack.get(rkr->PrefNom("Scale Window"), rkr->scalable, 0);

    Principal->resize(x, y, w, h);

    rakarrack.get(rkr->PrefNom("BankWindow X"), x, 1);
    rakarrack.get(rkr->PrefNom("BankWindow Y"), y, 1);
    rakarrack.get(rkr->PrefNom("BankWindow W"), w, 680);
    rakarrack.get(rkr->PrefNom("BankWindow H"), h, 425);

    BankWindow->resize(x, y, w, h);

    rakarrack.get(rkr->PrefNom("Order X"), x, 1);
    rakarrack.get(rkr->PrefNom("Order Y"), y, 1);
    rakarrack.get(rkr->PrefNom("Order W"), w, 600);
    rakarrack.get(rkr->PrefNom("Order H"), h, 330);

    Order->resize(x, y, w, h);

    rakarrack.get(rkr->PrefNom("MIDI Learn X"), x, 1);
    rakarrack.get(rkr->PrefNom("MIDI Learn Y"), y, 1);
    rakarrack.get(rkr->PrefNom("MIDI Learn W"), w, 600);
    rakarrack.get(rkr->PrefNom("MIDI Learn H"), h, 360);

    MIDILearn->resize(x, y, w, h);

    rakarrack.get(rkr->PrefNom("Trigger X"), x, 1);
    rakarrack.get(rkr->PrefNom("Trigger Y"), y, 1);
    rakarrack.get(rkr->PrefNom("Trigger W"), w, 280);
    rakarrack.get(rkr->PrefNom("Trigger H"), h, 165);

    Trigger->resize(x, y, w, h);

    rakarrack.get(rkr->PrefNom("Settings X"), x, 1);
    rakarrack.get(rkr->PrefNom("Settings Y"), y, 1);
    rakarrack.get(rkr->PrefNom("Settings W"), w, 440);
    rakarrack.get(rkr->PrefNom("Settings H"), h, 410);

    Settings->resize(x, y, w, h);


    back_color = (Fl_Color) b;
    fore_color = (Fl_Color) f;
    leds_color = (Fl_Color) l;
    label_color = (Fl_Color) a;

    Background_Color_Change(back_color);
    Buttons_Color_Change(fore_color);
    Leds_Color_Change(leds_color);
    Label_Color_Change(label_color);

    rakarrack.get(rkr->PrefNom("Bank Selected"), rkr->a_bank, 3);

    if (!needtoloadbank)
    {
        switch (rkr->a_bank)
        {
            case 0:
                L_B1->do_callback();
                break;

            case 1:
                L_B2->do_callback();
                break;

            case 2:
                L_B3->do_callback();
                break;
        }
    }

    if (!commandline)
    {
        rakarrack.get(rkr->PrefNom("Preset Num"), k, 1);
        rkr->Selected_Preset = k;
        Preset_Counter->value(k);
        Preset_Counter->do_callback();
    }

    rakarrack.get(rkr->PrefNom("MIDI Implementation"), rkr->MIDIway, 0);
    if (!rkr->MIDIway)
        ML_Menu->deactivate();

    rakarrack.get(rkr->PrefNom("MIDI Table"), rkr->midi_table, 0);
    if (!rkr->midi_table)
        Settings->scroll->deactivate();


    rakarrack.get(rkr->PrefNom("UserName"), rkr->UserRealName, "", 127);
    rakarrack.get(rkr->PrefNom("User Directory"), rkr->UDirFilename, DATADIR, 127);
    rakarrack.get(rkr->PrefNom("Preserve Gain/Master"), rkr->actuvol, 0);
    rakarrack.get(rkr->PrefNom("Metronome Volume"), rkr->Metro_Vol, 50);
    
    Looper *Efx_Looper = static_cast <Looper*> (rkr->Rack_Effects[EFX_LOOPER]);
    Efx_Looper->setmvol(rkr->Metro_Vol);

    rakarrack.get(rkr->PrefNom("Update Tap"), rkr->Tap_Updated, 0);
    rakarrack.get(rkr->PrefNom("MIDI IN Channel"), rkr->MidiCh, 1);
    rkr->MidiCh--;

    rakarrack.get(rkr->PrefNom("MIDI IN Harmonizer"), rkr->HarCh, 1);
    rkr->HarCh--;
    rakarrack.get(rkr->PrefNom("MIDI IN Stereo Harmonizer"), rkr->StereoHarCh, 1);
    rkr->StereoHarCh--;

    rakarrack.get(rkr->PrefNom("Limiter Position"), rkr->flpos, 0);
    rakarrack.get(rkr->PrefNom("Limiter +6dB"), rkr->db6booster, 0);
    rakarrack.get(rkr->PrefNom("Booster"), rkr->booster, 1.0f);
    if (rkr->booster == 1.0)
    {
        BostBut->value(0);
    }
    else
    {
        BostBut->value(1);
    }

    rakarrack.get(rkr->PrefNom("FX_init_state"), rkr->init_state, 0);
    rakarrack.get(rkr->PrefNom("Auto Assign"), rkr->autoassign, 0);


    if (rkr->init_state)
    {
        if (!needtoloadstate)
            rkr->Bypass = 1;
        
        rkr->calculavol(1);
        rkr->calculavol(2);
    }


    rakarrack.get(rkr->PrefNom("Recognize Optimization Harmonizer"), rkr->RCOpti_Harm, 0);
    Settings->RC_Harm_Opti->value(rkr->RCOpti_Harm);
    Settings->RC_Harm_Opti->do_callback();

    rakarrack.get(rkr->PrefNom("Recognize Optimization Stereo Harm"), rkr->RCOpti_Stereo, 0);
    Settings->RC_St_Harm_Opti->value(rkr->RCOpti_Stereo);
    Settings->RC_St_Harm_Opti->do_callback();

    rakarrack.get(rkr->PrefNom("Recognize Optimization Ring"), rkr->RCOpti_Ring, 0);
    Settings->RC_Ring_Opti->value(rkr->RCOpti_Ring);
    Settings->RC_Ring_Opti->do_callback();

    //MIDIConverter
    rakarrack.get(rkr->PrefNom("Midi Out Channel"), k, 1);
    MIDI->Midi_out_Counter->value(k);
    MIDI->Midi_out_Counter->do_callback();

    rkr->efx_MIDIConverter->setmidichannel(k);

    rakarrack.get(rkr->PrefNom("Trigger Adjust"), k, 4);
    MIDI->Trig_Adj->value(k);
    rkr->efx_MIDIConverter->setTriggerAdjust(k);


    rakarrack.get(rkr->PrefNom("Velocity Adjust"), k, 50);
    MIDI->Vel_Adj->value(k);
    rkr->efx_MIDIConverter->setVelAdjust(k);

    rakarrack.get(rkr->PrefNom("Converter Octave"), k, 2);
    MIDI->MIDIOctave->value(k);
    MIDI->MIDIOctave->do_callback();

    rakarrack.get(rkr->PrefNom("MIDI Converter On/Off"), k, 0);
    MIDI->midi_activar->value(k);
    MIDI->midi_activar->do_callback();

    //Metronome
    rakarrack.get(rkr->PrefNom("Internal Metronome Time"), k, 2);
    MetroBar->value(k);
    MetroBar->do_callback();

    rakarrack.get(rkr->PrefNom("Internal Metronome Sound"), k, 0);
    MetroSound->value(k);
    MetroSound->do_callback();

    rakarrack.get(rkr->PrefNom("Internal Metronome Volume"), k, 50);
    Metro_Volume->value(k);
    Metro_Volume->do_callback();

    rakarrack.get(rkr->PrefNom("Internal Metronome Tempo"), k, 100);
    Metro_Tempo->value(k);
    Metro_Tempo->do_callback();

    rakarrack.get(rkr->PrefNom("Internal Metronome Show"), rkr->sw_stat, 0);

    rakarrack.get(rkr->PrefNom("Internal Metronome On/Off"), k, 0);
    metro_activar->value(k);
    metro_activar->do_callback();

    if (rkr->sw_stat == 1)
    {
        MIDI->hide();
        Metro->show();
    }
    else
    {
        Metro->hide();
        MIDI->show();
    }

    //Tuner
    rakarrack.get(rkr->PrefNom("Tuner On/Off"), k, 0);
    tuner_activar->value(k);
    tuner_activar->do_callback();


    //Tap Tempo
    rakarrack.get(rkr->PrefNom("TapTempo Input"), rkr->Tap_Selection, 0);
    T_SEL->value(rkr->Tap_Selection);
    rakarrack.get(rkr->PrefNom("Tap Tempo Timeout"), rkr->t_timeout, 0);
    Settings->T_TIMEOUT->value(rkr->t_timeout);
    rakarrack.get(rkr->PrefNom("TapTempo Set"), rkr->Tap_SetValue, 0);
    T_SET->value(rkr->Tap_SetValue);

    rakarrack.get(rkr->PrefNom("TapTempo On/Off"), k, 0);
    Tap_activar->value(k);
    Tap_activar->do_callback();


    rakarrack.get(rkr->PrefNom("Disable Warnings"), rkr->mess_dis, 0);
    rakarrack.get(rkr->PrefNom("Enable Tooltips"), rkr->ena_tool, 1);
    Fl_Tooltip::enable(rkr->ena_tool);

    //Trigger
    rakarrack.get(rkr->PrefNom("Aux Source"), rkr->Aux_Source, 0);
    Trigger->aux_source->value(rkr->Aux_Source);
    rakarrack.get(rkr->PrefNom("Aux Gain"), rkr->Aux_Gain, 0);
    Trigger->aux_gain->value(rkr->Aux_Gain);
    rakarrack.get(rkr->PrefNom("Aux Threshold"), rkr->Aux_Threshold, 0);
    Trigger->aux_thres->value(rkr->Aux_Threshold);
    rakarrack.get(rkr->PrefNom("Aux MIDI"), rkr->Aux_MIDI, 1);
    Trigger->aux_midi->value(rkr->Aux_MIDI);
    rakarrack.get(rkr->PrefNom("Aux Minimum"), rkr->Aux_Minimum, 0);
    Trigger->aux_min->value(rkr->Aux_Minimum);
    rakarrack.get(rkr->PrefNom("Aux Maximum"), rkr->Aux_Maximum, 127);
    Trigger->aux_max->value(rkr->Aux_Maximum);


    char temp[64];


    for (int i = 0; i < 128; i++)
    {
        if (i < 60)
            k = i;
        
        if ((i > 59)&&(i < 120))
            k = 1000 + i - 60;
        
        if (i > 119)
            k = 0;
        
        memset(temp, 0, sizeof (temp));
        sprintf(temp, "Midi Table Program %d", i);
        rakarrack.get(rkr->PrefNom(temp), f, k);


        if (f < 1000)
        {
            rkr->M_table[i].bank = 0;
            rkr->M_table[i].preset = f;
        }

        if ((f > 999) && (f < 2000))
        {
            rkr->M_table[i].bank = 1;
            rkr->M_table[i].preset = f - 1000;
        }

        if ((f > 1999) && (f < 3000))
        {
            rkr->M_table[i].bank = 2;
            rkr->M_table[i].preset = f - 2000;
        }


        if ((f > 2999) && (f < 4000))
        {
            rkr->M_table[i].bank = 3;
            rkr->M_table[i].preset = f - 3000;
        }
    }
}

/**
 *  Save the current user state, windows, images, bank selected, preset, etc.
 *  In ~/user/.fltk/github.com.Stazed.rakarrack.plus/rakarrack-plus.prefs
 * 
 * @param whati
 *      The window settings that should be saved:
 *      0 = Main window & User settings
 *      1 = Bank window
 *      2 = Order window
 *      3 = Settings window & User settings
 *      4 = Not used
 *      5 = MIDI Learn
 *      6 = Trigger window (ACI)
 */
void RKRGUI::save_current_state(int whati)
{
    char temp1[128];

    Fl_Preferences rakarrack(Fl_Preferences::USER, WEBSITE, PACKAGE);

    if (whati == 0)
    {
        rakarrack.set(rkr->PrefNom("Principal X"), Principal->x());
        rakarrack.set(rkr->PrefNom("Principal Y"), Principal->y());
        rakarrack.set(rkr->PrefNom("Principal W"), Principal->w());
        rakarrack.set(rkr->PrefNom("Principal H"), Principal->h());
        rakarrack.set(rkr->PrefNom("FontSize"), rkr->fontsize);
        rakarrack.set(rkr->PrefNom("Font"), rkr->font);

        rakarrack.set(rkr->PrefNom("Background Color"), (int) back_color);
        rakarrack.set(rkr->PrefNom("Foreground Color"), (int) fore_color);
        rakarrack.set(rkr->PrefNom("Leds Color"), (int) leds_color);
        rakarrack.set(rkr->PrefNom("Labels Color"), (int) label_color);
        rakarrack.set(rkr->PrefNom("Schema"), (int) Settings->scheme_ch->value());
        rakarrack.set(rkr->PrefNom("Hide Effects"), (int) rkr->deachide);
        rakarrack.set(rkr->PrefNom("Scale Window"), (int) rkr->scalable);

        rakarrack.set(rkr->PrefNom("Bank Selected"), rkr->a_bank);

        if ((Preset_Counter->value() > 0) && (Preset_Counter->value() < 61))
            rakarrack.set(rkr->PrefNom("Preset Num"), (int) Preset_Counter->value());

        if (rkr->help_displayed)
        {
            rakarrack.set(rkr->PrefNom("Help X"), visor->x());
            rakarrack.set(rkr->PrefNom("Help Y"), visor->y());
            rakarrack.set(rkr->PrefNom("Help W"), visor->w());
            rakarrack.set(rkr->PrefNom("Help H"), visor->h());
            rakarrack.set(rkr->PrefNom("Help TextSize"), visor->textsize());
        }

        //Tuner
        rakarrack.set(rkr->PrefNom("Tuner On/Off"), (int) rkr->Tuner_Bypass);


        //MIDIConverter
        rakarrack.set(rkr->PrefNom("MIDI Converter On/Off"), (int) rkr->MIDIConverter_Bypass);
        rakarrack.set(rkr->PrefNom("Midi Out Channel"), (int) MIDI->Midi_out_Counter->value());
        rakarrack.set(rkr->PrefNom("Trigger Adjust"), (int) MIDI->Trig_Adj->value());
        rakarrack.set(rkr->PrefNom("Velocity Adjust"), (int) MIDI->Vel_Adj->value());
        rakarrack.set(rkr->PrefNom("Converter Octave"), (int) MIDI->MIDIOctave->value());

        //Metronome
        rakarrack.set(rkr->PrefNom("Internal Metronome On/Off"), (int) rkr->Metro_Bypass);
        rakarrack.set(rkr->PrefNom("Internal Metronome Time"), (int) MetroBar->value());
        rakarrack.set(rkr->PrefNom("Internal Metronome Volume"), (int) Metro_Volume->value());
        rakarrack.set(rkr->PrefNom("Internal Metronome Tempo"), (int) Metro_Tempo->value());
        rakarrack.set(rkr->PrefNom("Internal Metronome Show"), (int) rkr->sw_stat);
        rakarrack.set(rkr->PrefNom("Internal Metronome Sound"), (int) MetroSound->value());

        //Booster
        rakarrack.set(rkr->PrefNom("Booster"), rkr->booster);


        //Tap Tempo
        rakarrack.set(rkr->PrefNom("TapTempo On/Off"), (int) rkr->Tap_Bypass);
        rakarrack.set(rkr->PrefNom("TapTempo Input"), (int) rkr->Tap_Selection);
        rakarrack.set(rkr->PrefNom("TapTempo Set"), (int) rkr->Tap_SetValue);
    }


    if (whati == 1)
    {
        rakarrack.set(rkr->PrefNom("BankWindow X"), BankWindow->x());
        rakarrack.set(rkr->PrefNom("BankWindow Y"), BankWindow->y());
        rakarrack.set(rkr->PrefNom("BankWindow W"), BankWindow->w());
        rakarrack.set(rkr->PrefNom("BankWindow H"), BankWindow->h());
    }

    if (whati == 5)
    {
        rakarrack.set(rkr->PrefNom("MIDI Learn X"), MIDILearn->x());
        rakarrack.set(rkr->PrefNom("MIDI Learn Y"), MIDILearn->y());
        rakarrack.set(rkr->PrefNom("MIDI Learn W"), MIDILearn->w());
        rakarrack.set(rkr->PrefNom("MIDI Learn H"), MIDILearn->h());
    }


    if (whati == 6)
    {
        rakarrack.set(rkr->PrefNom("Trigger X"), Trigger->x());
        rakarrack.set(rkr->PrefNom("Trigger Y"), Trigger->y());
        rakarrack.set(rkr->PrefNom("Trigger W"), Trigger->w());
        rakarrack.set(rkr->PrefNom("Trigger H"), Trigger->h());

        rakarrack.set(rkr->PrefNom("Aux Source"), rkr->Aux_Source);
        rakarrack.set(rkr->PrefNom("Aux Gain"), rkr->Aux_Gain);
        rakarrack.set(rkr->PrefNom("Aux Threshold"), rkr->Aux_Threshold);
        rakarrack.set(rkr->PrefNom("Aux MIDI"), rkr->Aux_MIDI);
        rakarrack.set(rkr->PrefNom("Aux Minimum"), rkr->Aux_Minimum);
        rakarrack.set(rkr->PrefNom("Aux Maximum"), rkr->Aux_Maximum);
    }




    if (whati == 2)
    {
        rakarrack.set(rkr->PrefNom("Order X"), Order->x());
        rakarrack.set(rkr->PrefNom("Order Y"), Order->y());
        rakarrack.set(rkr->PrefNom("Order W"), Order->w());
        rakarrack.set(rkr->PrefNom("Order H"), Order->h());
    }

    if (whati == 3)
    {
        rakarrack.set(rkr->PrefNom("Settings X"), Settings->x());
        rakarrack.set(rkr->PrefNom("Settings Y"), Settings->y());
        rakarrack.set(rkr->PrefNom("Settings W"), Settings->w());
        rakarrack.set(rkr->PrefNom("Settings H"), Settings->h());
    }

    if ((whati == 3) || (whati == 0))
    {
        rakarrack.set(rkr->PrefNom("UserName"), rkr->UserRealName);
        rakarrack.set(rkr->PrefNom("Preserve Gain/Master"), rkr->actuvol);
        rakarrack.set(rkr->PrefNom("Metronome Volume"), rkr->Metro_Vol);

        rakarrack.set(rkr->PrefNom("Filter DC Offset"), rkr->DC_Offset);

        rakarrack.set(rkr->PrefNom("Update Tap"), rkr->Tap_Updated);
        rakarrack.set(rkr->PrefNom("Limiter Position"), rkr->flpos);
        rakarrack.set(rkr->PrefNom("Limiter +6dB"), rkr->db6booster);

        rakarrack.set(rkr->PrefNom("Recognize Optimization Harmonizer"), rkr->RCOpti_Harm);
        rakarrack.set(rkr->PrefNom("Recognize Optimization Stereo Harm"), rkr->RCOpti_Stereo);
        rakarrack.set(rkr->PrefNom("Recognize Optimization Ring"), rkr->RCOpti_Ring);

        rakarrack.set(rkr->PrefNom("Harmonizer Downsample"), rkr->Har_Down);
        rakarrack.set(rkr->PrefNom("Harmonizer Up Quality"), rkr->Har_U_Q);
        rakarrack.set(rkr->PrefNom("Harmonizer Down Quality"), rkr->Har_D_Q);
        rakarrack.set(rkr->PrefNom("Reverbtron Downsample"), rkr->Rev_Down);
        rakarrack.set(rkr->PrefNom("Reverbtron Up Quality"), rkr->Rev_U_Q);
        rakarrack.set(rkr->PrefNom("Reverbtron Down Quality"), rkr->Rev_D_Q);
        rakarrack.set(rkr->PrefNom("Convolotron Downsample"), rkr->Con_Down);
        rakarrack.set(rkr->PrefNom("Convolotron Up Quality"), rkr->Con_U_Q);
        rakarrack.set(rkr->PrefNom("Convolotron Down Quality"), rkr->Con_D_Q);
        rakarrack.set(rkr->PrefNom("Sequence Downsample"), rkr->Seq_Down);
        rakarrack.set(rkr->PrefNom("Sequence Up Quality"), rkr->Seq_U_Q);
        rakarrack.set(rkr->PrefNom("Sequence Down Quality"), rkr->Seq_D_Q);
        rakarrack.set(rkr->PrefNom("Shifter Downsample"), rkr->Shi_Down);
        rakarrack.set(rkr->PrefNom("Shifter Up Quality"), rkr->Shi_U_Q);
        rakarrack.set(rkr->PrefNom("Shifter Down Quality"), rkr->Shi_D_Q);
        rakarrack.set(rkr->PrefNom("Vocoder Downsample"), rkr->Voc_Down);
        rakarrack.set(rkr->PrefNom("Vocoder Up Quality"), rkr->Voc_U_Q);
        rakarrack.set(rkr->PrefNom("Vocoder Down Quality"), rkr->Voc_D_Q);
        rakarrack.set(rkr->PrefNom("StereoHarm Downsample"), rkr->Ste_Down);
        rakarrack.set(rkr->PrefNom("StereoHarm Up Quality"), rkr->Ste_U_Q);
        rakarrack.set(rkr->PrefNom("StereoHarm Down Quality"), rkr->Ste_D_Q);

        rakarrack.set(rkr->PrefNom("Distortion Resampling"), rkr->Dist_res_amount);
        rakarrack.set(rkr->PrefNom("Distortion Up Quality"), rkr->Dist_up_q);
        rakarrack.set(rkr->PrefNom("Distortion Down Quality"), rkr->Dist_down_q);

        rakarrack.set(rkr->PrefNom("Overdrive Resampling"), rkr->Ovrd_res_amount);
        rakarrack.set(rkr->PrefNom("Overdrive Up Quality"), rkr->Ovrd_up_q);
        rakarrack.set(rkr->PrefNom("Overdrive Down Quality"), rkr->Ovrd_down_q);

        rakarrack.set(rkr->PrefNom("Derelict Resampling"), rkr->Dere_res_amount);
        rakarrack.set(rkr->PrefNom("Derelict Up Quality"), rkr->Dere_up_q);
        rakarrack.set(rkr->PrefNom("Derelict Down Quality"), rkr->Dere_down_q);
        
        rakarrack.set(rkr->PrefNom("DistBand Resampling"), rkr->DBand_res_amount);
        rakarrack.set(rkr->PrefNom("DistBand Up Quality"), rkr->DBand_up_q);
        rakarrack.set(rkr->PrefNom("DistBand Down Quality"), rkr->DBand_down_q);

        rakarrack.set(rkr->PrefNom("StompBox Resampling"), rkr->Stomp_res_amount);
        rakarrack.set(rkr->PrefNom("StompBox Up Quality"), rkr->Stomp_up_q);
        rakarrack.set(rkr->PrefNom("StompBox Down Quality"), rkr->Stomp_down_q);

        rakarrack.set(rkr->PrefNom("Calibration"), rkr->aFreq);
        rakarrack.set(rkr->PrefNom("Recognize Trigger"), rkr->rtrig);

        rakarrack.set(rkr->PrefNom("Vocoder Bands"), rkr->VocBands);

        rakarrack.set(rkr->PrefNom("FX_init_state"), rkr->init_state);
        rakarrack.set(rkr->PrefNom("Auto Assign"), rkr->autoassign);

        rakarrack.set(rkr->PrefNom("UpSampling"), rkr->upsample);
        rakarrack.set(rkr->PrefNom("UpQuality"), rkr->UpQual);
        rakarrack.set(rkr->PrefNom("DownQuality"), rkr->DownQual);

        rakarrack.set(rkr->PrefNom("UpAmount"), rkr->UpAmo);
        rakarrack.set(rkr->PrefNom("Looper Size"), rkr->looper_size);

        rakarrack.set(rkr->PrefNom("Bank Filename"), rkr->BankFilename);
        rakarrack.set(rkr->PrefNom("User Directory"), rkr->UDirFilename);

        rakarrack.set(rkr->PrefNom("Enable Background Image"), rkr->EnableBackgroundImage);
        rakarrack.set(rkr->PrefNom("Background Image"), rkr->BackgroundImage);
        rakarrack.set(rkr->PrefNom("Auto Connect MIDI IN"), rkr->aconnect_MI);
        rakarrack.set(rkr->PrefNom("Auto Connect Jack"), rkr->aconnect_JA);
        rakarrack.set(rkr->PrefNom("Auto Connect Jack In"), rkr->aconnect_JIA);

        rakarrack.set(rkr->PrefNom("MIDI Implementation"), rkr->MIDIway);
        rakarrack.set(rkr->PrefNom("MIDI Table"), rkr->midi_table);


        int i = Settings->BMidiIn->value();
        char *temp;
        temp = (char*) Settings->BMidiIn->text(i);
        
        if (temp != NULL)
        {
            rakarrack.set(rkr->PrefNom("MIDI IN Device"), temp);
        }
        
        rakarrack.set(rkr->PrefNom("MIDI IN Channel"), rkr->MidiCh + 1);
        rakarrack.set(rkr->PrefNom("MIDI IN Harmonizer"), rkr->HarCh + 1);
        rakarrack.set(rkr->PrefNom("MIDI IN Stereo Harmonizer"), rkr->StereoHarCh + 1);
        rakarrack.set(rkr->PrefNom("Harmonizer Quality"), rkr->HarQual);
        rakarrack.set(rkr->PrefNom("StereoHarm Quality"), rkr->SteQual);
        rakarrack.set(rkr->PrefNom("Sequence Quality"), rkr->SeqQual);
        rakarrack.set(rkr->PrefNom("Shifter Quality"), rkr->ShiQual);
        rakarrack.set(rkr->PrefNom("Tap Tempo Timeout"), rkr->t_timeout);
        rakarrack.set(rkr->PrefNom("Disable Warnings"), rkr->mess_dis);
        rakarrack.set(rkr->PrefNom("Enable Tooltips"), rkr->ena_tool);

        for (int i = 0; i < 128; i++)
        {
            memset(temp1, 0, sizeof (temp1));
            sprintf(temp1, "Midi Table Program %d", i);
            rakarrack.set(rkr->PrefNom(temp1), rkr->M_table[i].bank * 1000 + rkr->M_table[i].preset);
        }
    }


    if (whati == 3)
    {
        int k = 1;
        for (int i = 1; i <= Settings->JackCo->size(); i++)
        {
            if (Settings->JackCo->selected(i))
            {
                memset(temp1, 0, sizeof (temp1));
                sprintf(temp1, "Jack Port %d", k);
                rakarrack.set(rkr->PrefNom(temp1), Settings->JackCo->text(i));
                k++;
            }
        }

        rakarrack.set(rkr->PrefNom("Auto Connect Num"), k - 1);

        k = 1;
        for (int i = 1; i <= Settings->JackIn->size(); i++)
        {
            if (Settings->JackIn->selected(i))
            {
                memset(temp1, 0, sizeof (temp1));
                sprintf(temp1, "Jack Port In %d", k);
                rakarrack.set(rkr->PrefNom(temp1), Settings->JackIn->text(i));
                k++;
            }
        }

        rakarrack.set(rkr->PrefNom("Auto Connect In Num"), k - 1);
    }
}

void RKRGUI::Put_Loaded()
{
    //General loading of efx default settings

    WPreset_Name->value(rkr->Preset_Name);
    DAuthor->copy_label(rkr->Author);

    Nivel_Entrada->value((int) (rkr->Input_Gain * 100.0) - 50);
    rkr->calculavol(1);
    Nivel_Salida->value((int) (rkr->Master_Volume * 100.0) - 50);
    rkr->calculavol(2);
    Balance->value((int) (rkr->Fraction_Bypass * 100.0));


    ActivarGeneral->value(rkr->Bypass);

    if (rkr->Bypass == 0)
    {
        rkr->val_il_sum = -50;
        rkr->val_ir_sum = -50;

        rkr->val_vl_sum = -50;
        rkr->val_vr_sum = -50;

        input_vul->value(-50);
        input_vur->value(-50);

        output_vul->value(-50);
        output_vur->value(-50);
    }


    /*  Original set and cleared for only the ten current from efx_order[] which did not
        clear other efx from leftover settings. The new behavior will reset all efx to default
        if not used. The reset will occur when the New button is pressed or a new bank is selected.
        If an existing bank re-positions an effect, by replacement and then moving it to a new location
        then the settings are kept from previous.
  
        Legacy:
        for(i=0;i<10;i++) {
        switch(rkr->efx_order[i]){
     */

    for (int i = 0; i < C_NUMBER_EFFECTS; i++)
    {
        switch (i)
        {

            case EFX_EQ:
                EQ->eq_activar->value(rkr->EFX_Bypass[EFX_EQ]);
                EQ->eq_preset->do_callback(EQ->eq_preset, 1);
                break;

            case EFX_COMPRESSOR:
                COMPRESS->compress_activar->value(rkr->EFX_Bypass[EFX_COMPRESSOR]);
                COMPRESS->compress_preset->do_callback(COMPRESS->compress_preset, 1);
                break;

            case EFX_DISTORTION:
                DIST->dist_activar->value(rkr->EFX_Bypass[EFX_DISTORTION]);
                DIST->dist_preset->do_callback(DIST->dist_preset, 1);
                break;

            case EFX_OVERDRIVE:
                OVRD->ovrd_activar->value(rkr->EFX_Bypass[EFX_OVERDRIVE]);
                OVRD->ovrd_preset->do_callback(OVRD->ovrd_preset, 1);
                break;

            case EFX_ECHO:
                ECHO->echo_activar->value(rkr->EFX_Bypass[EFX_ECHO]);
                ECHO->echo_preset->do_callback(ECHO->echo_preset, 1);
                break;

            case EFX_CHORUS:
                CHORUS->chorus_activar->value(rkr->EFX_Bypass[EFX_CHORUS]);
                CHORUS->chorus_preset->do_callback(CHORUS->chorus_preset, 1);
                break;

            case EFX_PHASER:
                PHASER->phaser_activar->value(rkr->EFX_Bypass[EFX_PHASER]);
                PHASER->phaser_preset->do_callback(PHASER->phaser_preset, 1);
                break;

            case EFX_FLANGER:
                FLANGER->flanger_activar->value(rkr->EFX_Bypass[EFX_FLANGER]);
                FLANGER->flanger_preset->do_callback(FLANGER->flanger_preset, 1);
                break;

            case EFX_REVERB:
                REVERB->reverb_activar->value(rkr->EFX_Bypass[EFX_REVERB]);
                REVERB->reverb_preset->do_callback(REVERB->reverb_preset, 1);
                break;

            case EFX_PARAMETRIC:
                PEQ->eqp_activar->value(rkr->EFX_Bypass[EFX_PARAMETRIC]);
                PEQ->eqp_preset->do_callback(PEQ->eqp_preset, 1);
                break;

            case EFX_WAHWAH:
                WAHWAH->WahWah_activar->value(rkr->EFX_Bypass[EFX_WAHWAH]);
                WAHWAH->WahWah_preset->do_callback(WAHWAH->WahWah_preset, 1);
                break;

            case EFX_ALIENWAH:
                ALIENWAH->Alienwah_activar->value(rkr->EFX_Bypass[EFX_ALIENWAH]);
                ALIENWAH->Alienwah_preset->do_callback(ALIENWAH->Alienwah_preset, 1);
                break;

            case EFX_CABINET:
                CABINET->Cabinet_output->value(rkr->lv[EFX_CABINET][Cabinet_Gain] - 64);
                CABINET->Cabinet_preset->value(rkr->lv[EFX_CABINET][Cabinet_Preset_Idx]);
                CABINET->Cabinet_activar->value(rkr->EFX_Bypass[EFX_CABINET]);
                break;

            case EFX_PAN:
                PAN->pan_activar->value(rkr->EFX_Bypass[EFX_PAN]);
                PAN->pan_preset->do_callback(PAN->pan_preset, 1);
                break;

            case EFX_HARMONIZER:
                HAR->har_activar->value(rkr->EFX_Bypass[EFX_HARMONIZER]);
                HAR->har_preset->do_callback(HAR->har_preset, 1);
                break;

            case EFX_MUSICAL_DELAY:
                MUSDELAY->musdelay_activar->value(rkr->EFX_Bypass[EFX_MUSICAL_DELAY]);
                MUSDELAY->musdelay_preset->do_callback(MUSDELAY->musdelay_preset, 1);
                break;

            case EFX_NOISEGATE:
                GATE->gate_activar->value(rkr->EFX_Bypass[EFX_NOISEGATE]);
                GATE->gate_preset->do_callback(GATE->gate_preset, 1);
                break;

            case EFX_DERELICT:
                DERELICT->derelict_activar->value(rkr->EFX_Bypass[EFX_DERELICT]);
                DERELICT->derelict_preset->do_callback(DERELICT->derelict_preset, 1);
                break;

            case EFX_ANALOG_PHASER:
                APHASER->aphaser_activar->value(rkr->EFX_Bypass[EFX_ANALOG_PHASER]);
                APHASER->aphaser_preset->do_callback(APHASER->aphaser_preset, 1);
                break;

            case EFX_VALVE:
                VALVE->valve_activar->value(rkr->EFX_Bypass[EFX_VALVE]);
                VALVE->valve_preset->do_callback(VALVE->valve_preset, 1);
                break;

            case EFX_DUAL_FLANGE:
                DFLANGE->dflange_activar->value(rkr->EFX_Bypass[EFX_DUAL_FLANGE]);
                DFLANGE->dflange_preset->do_callback(DFLANGE->dflange_preset, 1);
                break;

            case EFX_RING:
                RING->ring_activar->value(rkr->EFX_Bypass[EFX_RING]);
                RING->ring_preset->do_callback(RING->ring_preset, 1);
                break;

            case EFX_EXCITER:
                EXCITER->exciter_activar->value(rkr->EFX_Bypass[EFX_EXCITER]);
                EXCITER->exciter_preset->do_callback(EXCITER->exciter_preset, 1);
                break;

            case EFX_DISTBAND:
                DISTBAND->distband_activar->value(rkr->EFX_Bypass[EFX_DISTBAND]);
                DISTBAND->distband_preset->do_callback(DISTBAND->distband_preset, 1);
                break;

            case EFX_ARPIE:
                ARPIE->arpie_activar->value(rkr->EFX_Bypass[EFX_ARPIE]);
                ARPIE->arpie_preset->do_callback(ARPIE->arpie_preset, 1);
                break;

            case EFX_EXPANDER:
                EXPANDER->expander_activar->value(rkr->EFX_Bypass[EFX_EXPANDER]);
                EXPANDER->expander_preset->do_callback(EXPANDER->expander_preset, 1);
                break;

            case EFX_SHUFFLE:
                SHUFFLE->shuffle_activar->value(rkr->EFX_Bypass[EFX_SHUFFLE]);
                SHUFFLE->shuffle_preset->do_callback(SHUFFLE->shuffle_preset, 1);
                break;

            case EFX_SYNTHFILTER:
                SYNTHFILTER->synthfilter_activar->value(rkr->EFX_Bypass[EFX_SYNTHFILTER]);
                SYNTHFILTER->synthfilter_preset->do_callback(SYNTHFILTER->synthfilter_preset, 1);
                break;

            case EFX_VARYBAND:
                VARYBAND->varyband_activar->value(rkr->EFX_Bypass[EFX_VARYBAND]);
                VARYBAND->varyband_preset->do_callback(VARYBAND->varyband_preset, 1);
                break;

            case EFX_CONVOLOTRON:
                CONVOLOTRON->convo_activar->value(rkr->EFX_Bypass[EFX_CONVOLOTRON]);
                CONVOLOTRON->convo_preset->do_callback(CONVOLOTRON->convo_preset, 1);
                break;

            case EFX_LOOPER:
                LOOPER->looper_activar->value(rkr->EFX_Bypass[EFX_LOOPER]);
                LOOPER->looper_preset->do_callback(LOOPER->looper_preset, 1);
                update_looper();
                break;

            case EFX_MUTROMOJO:
                MUTROMOJO->mutromojo_activar->value(rkr->EFX_Bypass[EFX_MUTROMOJO]);
                MUTROMOJO->mutromojo_preset->do_callback(MUTROMOJO->mutromojo_preset, 1);
                break;

            case EFX_ECHOVERSE:
                ECHOVERSE->echoverse_activar->value(rkr->EFX_Bypass[EFX_ECHOVERSE]);
                ECHOVERSE->echoverse_preset->do_callback(ECHOVERSE->echoverse_preset, 1);
                break;

            case EFX_COILCRAFTER:
                COILCRAFTER->coil_activar->value(rkr->EFX_Bypass[EFX_COILCRAFTER]);
                COILCRAFTER->coil_preset->do_callback(COILCRAFTER->coil_preset, 1);
                break;

            case EFX_SHELFBOOST:
                SHELFBOOST->shelf_activar->value(rkr->EFX_Bypass[EFX_SHELFBOOST]);
                SHELFBOOST->shelf_preset->do_callback(SHELFBOOST->shelf_preset, 1);
                break;

            case EFX_VOCODER:
                VOCODER->vo_activar->value(rkr->EFX_Bypass[EFX_VOCODER]);
                VOCODER->vo_preset->do_callback(VOCODER->vo_preset, 1);
                break;

            case EFX_SUSTAINER:
                SUSTAINER->sus_activar->value(rkr->EFX_Bypass[EFX_SUSTAINER]);
                SUSTAINER->sus_preset->do_callback(SUSTAINER->sus_preset, 1);
                break;

            case EFX_SEQUENCE:
                SEQUENCE->seq_activar->value(rkr->EFX_Bypass[EFX_SEQUENCE]);
                SEQUENCE->seq_preset->do_callback(SEQUENCE->seq_preset, 1);
                break;

            case EFX_SHIFTER:
                SHIFTER->shifter_activar->value(rkr->EFX_Bypass[EFX_SHIFTER]);
                SHIFTER->shifter_preset->do_callback(SHIFTER->shifter_preset, 1);
                break;

            case EFX_STOMPBOX:
                STOMPBOX->stomp_activar->value(rkr->EFX_Bypass[EFX_STOMPBOX]);
                STOMPBOX->stomp_preset->do_callback(STOMPBOX->stomp_preset, 1);
                break;

            case EFX_REVERBTRON:
                REVERBTRON->revtron_activar->value(rkr->EFX_Bypass[EFX_REVERBTRON]);
                REVERBTRON->revtron_preset->do_callback(REVERBTRON->revtron_preset, 1);
                break;

            case EFX_ECHOTRON:
                ECHOTRON->echotron_activar->value(rkr->EFX_Bypass[EFX_ECHOTRON]);
                ECHOTRON->echotron_preset->do_callback(ECHOTRON->echotron_preset, 1);
                break;

            case EFX_STEREOHARM:
                SHAR->shar_activar->value(rkr->EFX_Bypass[EFX_STEREOHARM]);
                SHAR->shar_preset->do_callback(SHAR->shar_preset, 1);
                break;

            case EFX_COMPBAND:
                COMPBAND->cband_activar->value(rkr->EFX_Bypass[EFX_COMPBAND]);
                COMPBAND->cband_preset->do_callback(COMPBAND->cband_preset, 1);
                break;

            case EFX_OPTICALTREM:
                OTREM->otrem_activar->value(rkr->EFX_Bypass[EFX_OPTICALTREM]);
                OTREM->otrem_preset->do_callback(OTREM->otrem_preset, 1);
                break;

            case EFX_VIBE:
                VIBE->vibe_activar->value(rkr->EFX_Bypass[EFX_VIBE]);
                VIBE->vibe_preset->do_callback(VIBE->vibe_preset, 1);
                break;

            case EFX_INFINITY:
                INFINIT->infinity_activar->value(rkr->EFX_Bypass[EFX_INFINITY]);
                INFINIT->infinity_preset->do_callback(INFINIT->infinity_preset, 1);
                break;

        }
    }

    reordena();
}

void RKRGUI::preset_click(Fl_Button* o, void* v)
{
    ((RKRGUI*) (o->parent()->parent()->user_data()))->preset_click_i(o, v);
}

inline void RKRGUI::preset_click_i(Fl_Button* o, void*)
{
    // used when selecting/moving/right click on preset from bank window

    int tecla = Fl::event_key();
    long long kk = (long long) o->user_data();
    int num = (int) kk;

    if (drag != 1000)
    {
        int en = search_but(Fl::event_x(), Fl::event_y());
        if (en != 1000)
        {
            rkr->Bank[0] = rkr->Bank[en];
            rkr->Bank[en] = rkr->Bank[num];
            rkr->Bank[num] = rkr->Bank[0];
            Put_Loaded_Bank();
            rkr->modified = 1;
            num = en;
            drag = 1000;
        }
    }

    if ((Fl::event_is_click() == 0)&&(Fl::event() == FL_DRAG))
    {
        drag = num;
        return;
    }
    else
    {
        drag = 1000;
    }

    if ((Fl::event_button() == FL_RIGHT_MOUSE) && (Fl::event() == FL_RELEASE))
    {
        Fl_Widget *w = BankWindow->ob->child(num - 1);

        // Check if user really wants to set the preset to the bank
        Fl_Widget *m = fl_message_icon();
        m->parent()->copy_label(rkr->jackcliname);
        int ok = fl_choice("Overwrite \"%s\"?", "No", "Yes", NULL, w->label());
        if (!ok)
        {
            o->value(0);
            o->redraw();
            return;
        }

        o->value(0);
        o->redraw();
        rkr->preset_to_bank(num);
        w->copy_label(rkr->Preset_Name);
        rkr->modified = 1;
    }


    if ((Fl::event_button() == FL_LEFT_MOUSE) || tecla == SPACE_BAR)
    {
        if ((num != rkr->Selected_Preset) || (rkr->new_bank_loaded))
        {
            Fl_Widget *w = BankWindow->ob->child(num - 1);
            BankWindow->unlight_preset(rkr->Selected_Preset);
            rkr->Selected_Preset = num;
            w->color(fl_darker(leds_color));
            Preset_Counter->value(num);
            Preset_Counter->do_callback();
        }
    }
}

void RKRGUI::reordena()
{
    // Reorder efx
    unsigned int x[10], y[10];
    ulong s[10];

    s[0] = 49;
    s[1] = 50;
    s[2] = 51;
    s[3] = 52;
    s[4] = 53;
    s[5] = 54;
    s[6] = 55;
    s[7] = 56;
    s[8] = 57;
    s[9] = 48;


    x[0] = E1->x();
    x[1] = E2->x();
    x[2] = E3->x();
    x[3] = E4->x();
    x[4] = E5->x();
    x[5] = E6->x();
    x[6] = E7->x();
    x[7] = E8->x();
    x[8] = E9->x();
    x[9] = E10->x();

    y[0] = E1->y();
    y[1] = E2->y();
    y[2] = E3->y();
    y[3] = E4->y();
    y[4] = E5->y();
    y[5] = E6->y();
    y[6] = E7->y();
    y[7] = E8->y();
    y[8] = E9->y();
    y[9] = E10->y();


    //Hide All
    EQ->hide();
    COMPRESS->hide();
    DIST->hide();
    OVRD->hide();
    ECHO->hide();
    CHORUS->hide();
    PHASER->hide();
    FLANGER->hide();
    REVERB->hide();
    PEQ->hide();
    WAHWAH->hide();
    ALIENWAH->hide();
    CABINET->hide();
    PAN->hide();
    HAR->hide();
    MUSDELAY->hide();
    GATE->hide();
    DERELICT->hide();
    APHASER->hide();
    VALVE->hide();
    DFLANGE->hide();
    RING->hide();
    EXCITER->hide();
    DISTBAND->hide();
    ARPIE->hide();
    EXPANDER->hide();
    SHUFFLE->hide();
    SYNTHFILTER->hide();
    VARYBAND->hide();
    CONVOLOTRON->hide();
    LOOPER->hide();
    MUTROMOJO->hide();
    ECHOVERSE->hide();
    COILCRAFTER->hide();
    SHELFBOOST->hide();
    VOCODER->hide();
    SUSTAINER->hide();
    SEQUENCE->hide();
    SHIFTER->hide();
    STOMPBOX->hide();
    REVERBTRON->hide();
    ECHOTRON->hide();
    SHAR->hide();
    COMPBAND->hide();
    OTREM->hide();
    VIBE->hide();
    INFINIT->hide();


    // Show

    for (int i = 0; i < C_NUMBER_ORDERED_EFFECTS; i++)
    {
        switch (rkr->efx_order[i])
        {

            case EFX_EQ:
                EQ->position(x[i], y[i]);
                EQ->eq_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    EQ->show();
                if (rkr->EFX_Bypass[EFX_EQ])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        EQ->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_COMPRESSOR:
                COMPRESS->position(x[i], y[i]);
                COMPRESS->compress_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    COMPRESS->show();
                if (rkr->EFX_Bypass[EFX_COMPRESSOR])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        COMPRESS->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_DISTORTION:
                DIST->position(x[i], y[i]);
                DIST->dist_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    DIST->show();
                if (rkr->EFX_Bypass[EFX_DISTORTION])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        DIST->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_OVERDRIVE:
                OVRD->position(x[i], y[i]);
                OVRD->ovrd_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    OVRD->show();
                if (rkr->EFX_Bypass[EFX_OVERDRIVE])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        OVRD->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_ECHO:
                ECHO->position(x[i], y[i]);
                ECHO->echo_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    ECHO->show();
                if (rkr->EFX_Bypass[EFX_ECHO])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        ECHO->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_CHORUS:
                CHORUS->position(x[i], y[i]);
                CHORUS->chorus_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    CHORUS->show();
                if (rkr->EFX_Bypass[EFX_CHORUS])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        CHORUS->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_PHASER:
                PHASER->position(x[i], y[i]);
                PHASER->phaser_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    PHASER->show();
                if (rkr->EFX_Bypass[EFX_PHASER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        PHASER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_FLANGER:
                FLANGER->position(x[i], y[i]);
                FLANGER->flanger_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    FLANGER->show();
                if (rkr->EFX_Bypass[EFX_FLANGER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        FLANGER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_REVERB:
                REVERB->position(x[i], y[i]);
                REVERB->reverb_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    REVERB->show();
                if (rkr->EFX_Bypass[EFX_REVERB])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        REVERB->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_PARAMETRIC:
                PEQ->position(x[i], y[i]);
                PEQ->eqp_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    PEQ->show();
                if (rkr->EFX_Bypass[EFX_PARAMETRIC])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        PEQ->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_WAHWAH:
                WAHWAH->position(x[i], y[i]);
                WAHWAH->WahWah_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    WAHWAH->show();
                if (rkr->EFX_Bypass[EFX_WAHWAH])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        WAHWAH->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_ALIENWAH:
                ALIENWAH->position(x[i], y[i]);
                ALIENWAH->Alienwah_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    ALIENWAH->show();
                if (rkr->EFX_Bypass[EFX_ALIENWAH])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        ALIENWAH->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_CABINET:
                CABINET->position(x[i], y[i]);
                CABINET->Cabinet_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    CABINET->show();
                if (rkr->EFX_Bypass[EFX_CABINET])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        CABINET->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_PAN:
                PAN->position(x[i], y[i]);
                PAN->pan_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    PAN->show();
                if (rkr->EFX_Bypass[EFX_PAN])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        PAN->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_HARMONIZER:
                HAR->position(x[i], y[i]);
                HAR->har_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    HAR->show();
                if (rkr->EFX_Bypass[EFX_HARMONIZER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        HAR->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_MUSICAL_DELAY:
                MUSDELAY->position(x[i], y[i]);
                MUSDELAY->musdelay_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    MUSDELAY->show();
                if (rkr->EFX_Bypass[EFX_MUSICAL_DELAY])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        MUSDELAY->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_NOISEGATE:
                GATE->position(x[i], y[i]);
                GATE->gate_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    GATE->show();
                if (rkr->EFX_Bypass[EFX_NOISEGATE])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        GATE->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_DERELICT:
                DERELICT->position(x[i], y[i]);
                DERELICT->derelict_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    DERELICT->show();
                if (rkr->EFX_Bypass[EFX_DERELICT])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        DERELICT->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_ANALOG_PHASER:
                APHASER->position(x[i], y[i]);
                APHASER->aphaser_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    APHASER->show();
                if (rkr->EFX_Bypass[EFX_ANALOG_PHASER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        APHASER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_VALVE:
                VALVE->position(x[i], y[i]);
                VALVE->valve_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    VALVE->show();
                if (rkr->EFX_Bypass[EFX_VALVE])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        VALVE->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_DUAL_FLANGE:
                DFLANGE->position(x[i], y[i]);
                DFLANGE->dflange_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    DFLANGE->show();
                if (rkr->EFX_Bypass[EFX_DUAL_FLANGE])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        DFLANGE->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_RING:
                RING->position(x[i], y[i]);
                RING->ring_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    RING->show();
                if (rkr->EFX_Bypass[EFX_RING])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        RING->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_EXCITER:
                EXCITER->position(x[i], y[i]);
                EXCITER->exciter_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    EXCITER->show();
                if (rkr->EFX_Bypass[EFX_EXCITER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        EXCITER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_DISTBAND:
                DISTBAND->position(x[i], y[i]);
                DISTBAND->distband_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    DISTBAND->show();
                if (rkr->EFX_Bypass[EFX_DISTBAND])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        DISTBAND->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_ARPIE:
                ARPIE->position(x[i], y[i]);
                ARPIE->arpie_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    ARPIE->show();
                if (rkr->EFX_Bypass[EFX_ARPIE])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        ARPIE->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_EXPANDER:
                EXPANDER->position(x[i], y[i]);
                EXPANDER->expander_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    EXPANDER->show();
                if (rkr->EFX_Bypass[EFX_EXPANDER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        EXPANDER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_SHUFFLE:
                SHUFFLE->position(x[i], y[i]);
                SHUFFLE->shuffle_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    SHUFFLE->show();
                if (rkr->EFX_Bypass[EFX_SHUFFLE])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        SHUFFLE->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_SYNTHFILTER:
                SYNTHFILTER->position(x[i], y[i]);
                SYNTHFILTER->synthfilter_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    SYNTHFILTER->show();
                if (rkr->EFX_Bypass[EFX_SYNTHFILTER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        SYNTHFILTER->show();
                }
                else
                    rkr->active[i] = 0;

                break;

            case EFX_VARYBAND:
                VARYBAND->position(x[i], y[i]);
                VARYBAND->varyband_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    VARYBAND->show();
                if (rkr->EFX_Bypass[EFX_VARYBAND])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        VARYBAND->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_CONVOLOTRON:
                CONVOLOTRON->position(x[i], y[i]);
                CONVOLOTRON->convo_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    CONVOLOTRON->show();
                if (rkr->EFX_Bypass[EFX_CONVOLOTRON])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        CONVOLOTRON->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_LOOPER:
                LOOPER->position(x[i], y[i]);
                LOOPER->looper_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    LOOPER->show();
                if (rkr->EFX_Bypass[EFX_LOOPER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        LOOPER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_MUTROMOJO:
                MUTROMOJO->position(x[i], y[i]);
                MUTROMOJO->mutromojo_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    MUTROMOJO->show();
                if (rkr->EFX_Bypass[EFX_MUTROMOJO])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        MUTROMOJO->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_ECHOVERSE:
                ECHOVERSE->position(x[i], y[i]);
                ECHOVERSE->echoverse_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    ECHOVERSE->show();
                if (rkr->EFX_Bypass[EFX_ECHOVERSE])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        ECHOVERSE->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_COILCRAFTER:
                COILCRAFTER->position(x[i], y[i]);
                COILCRAFTER->coil_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    COILCRAFTER->show();
                if (rkr->EFX_Bypass[EFX_COILCRAFTER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        COILCRAFTER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_SHELFBOOST:
                SHELFBOOST->position(x[i], y[i]);
                SHELFBOOST->shelf_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    SHELFBOOST->show();
                if (rkr->EFX_Bypass[EFX_SHELFBOOST])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        SHELFBOOST->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_VOCODER:
                VOCODER->position(x[i], y[i]);
                VOCODER->vo_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    VOCODER->show();
                if (rkr->EFX_Bypass[EFX_VOCODER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        VOCODER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_SUSTAINER:
                SUSTAINER->position(x[i], y[i]);
                SUSTAINER->sus_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    SUSTAINER->show();
                if (rkr->EFX_Bypass[EFX_SUSTAINER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        SUSTAINER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_SEQUENCE:
                SEQUENCE->position(x[i], y[i]);
                SEQUENCE->seq_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    SEQUENCE->show();
                if (rkr->EFX_Bypass[EFX_SEQUENCE])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        SEQUENCE->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_SHIFTER:
                SHIFTER->position(x[i], y[i]);
                SHIFTER->shifter_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    SHIFTER->show();
                if (rkr->EFX_Bypass[EFX_SHIFTER])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        SHIFTER->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_STOMPBOX:
                STOMPBOX->position(x[i], y[i]);
                STOMPBOX->stomp_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    STOMPBOX->show();
                if (rkr->EFX_Bypass[EFX_STOMPBOX])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        STOMPBOX->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_REVERBTRON:
                REVERBTRON->position(x[i], y[i]);
                REVERBTRON->revtron_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    REVERBTRON->show();
                if (rkr->EFX_Bypass[EFX_REVERBTRON])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        REVERBTRON->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_ECHOTRON:
                ECHOTRON->position(x[i], y[i]);
                ECHOTRON->echotron_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    ECHOTRON->show();
                if (rkr->EFX_Bypass[EFX_ECHOTRON])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        ECHOTRON->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_STEREOHARM:
                SHAR->position(x[i], y[i]);
                SHAR->shar_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    SHAR->show();
                if (rkr->EFX_Bypass[EFX_STEREOHARM])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        SHAR->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_COMPBAND:
                COMPBAND->position(x[i], y[i]);
                COMPBAND->cband_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    COMPBAND->show();
                if (rkr->EFX_Bypass[EFX_COMPBAND])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        COMPBAND->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_OPTICALTREM:
                OTREM->position(x[i], y[i]);
                OTREM->otrem_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    OTREM->show();
                if (rkr->EFX_Bypass[EFX_OPTICALTREM])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        OTREM->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_VIBE:
                VIBE->position(x[i], y[i]);
                VIBE->vibe_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    VIBE->show();
                if (rkr->EFX_Bypass[EFX_VIBE])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        VIBE->show();
                }
                else
                    rkr->active[i] = 0;
                break;

            case EFX_INFINITY:
                INFINIT->position(x[i], y[i]);
                INFINIT->infinity_activar->shortcut(s[i]);
                if (!rkr->deachide)
                    INFINIT->show();
                if (rkr->EFX_Bypass[EFX_INFINITY])
                {
                    rkr->active[i] = 1;
                    if (rkr->deachide)
                        INFINIT->show();
                }
                else
                    rkr->active[i] = 0;
                break;

        }


        switch (i)
        {
            case 0:
                L1->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L1->hide();
                else
                    L1->show();
                break;
            case 1:
                L2->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L2->hide();
                else
                    L2->show();
                break;
            case 2:
                L3->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L3->hide();
                else
                    L3->show();
                break;
            case 3:
                L4->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L4->hide();
                else
                    L4->show();
                break;
            case 4:
                L5->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L5->hide();
                else
                    L5->show();
                break;
            case 5:
                L6->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L6->hide();
                else
                    L6->show();
                break;
            case 6:
                L7->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L7->hide();
                else
                    L7->show();
                break;
            case 7:
                L8->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L8->hide();
                else
                    L8->show();
                break;
            case 8:
                L9->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L9->hide();
                else
                    L9->show();
                break;
            case 9:
                L10->copy_label(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
                if ((rkr->deachide) && (!rkr->active[i]))
                    L10->hide();
                else
                    L10->show();
                break;
        }


    }

    Principal->init_sizes();

    ChangeActives();

    Fl::redraw();
}

void RKRGUI::show_help()
{
    // show the help window
    char temp[128];

    Fl_Preferences rakarrack(Fl_Preferences::USER, WEBSITE, PACKAGE);

    if (rkr->help_displayed == 0)
    {
        visor = new Fl_Help_Dialog;

        int x, y, w, h, k;
        rakarrack.get(rkr->PrefNom("Help X"), x, 1);
        rakarrack.get(rkr->PrefNom("Help Y"), y, 1);
        rakarrack.get(rkr->PrefNom("Help W"), w, 640);
        rakarrack.get(rkr->PrefNom("Help H"), h, 480);
        rakarrack.get(rkr->PrefNom("Help TextSize"), k, 11);
        visor->resize(x, y, w, h);
        visor->textsize((unsigned char) k);
    }


    memset(temp, 0, sizeof (temp));
    sprintf(temp, "%s/html/help.html", HELPDIR);


    if (FILE * file = fopen(temp, "r"))
    {
        fclose(file);
        visor->load(temp);
        rkr->help_displayed = 1;
        visor->show();
    }
}

void RKRGUI::show_lic()
{
    // Show licenses
    char temp[128];

    Fl_Preferences rakarrack(Fl_Preferences::USER, WEBSITE, PACKAGE);

    if (rkr->help_displayed == 0)
    {
        visor = new Fl_Help_Dialog;

        int x, y, w, h, k;
        rakarrack.get(rkr->PrefNom("Help X"), x, 1);
        rakarrack.get(rkr->PrefNom("Help Y"), y, 1);
        rakarrack.get(rkr->PrefNom("Help W"), w, 640);
        rakarrack.get(rkr->PrefNom("Help H"), h, 480);
        rakarrack.get(rkr->PrefNom("Help TextSize"), k, 11);
        visor->resize(x, y, w, h);
        visor->textsize((unsigned char) k);
    }

    memset(temp, 0, sizeof (temp));
    sprintf(temp, "%s/html/license.html", HELPDIR);
    visor->load(temp);

    rkr->help_displayed = 1;

    visor->show();
}

void RKRGUI::MiraClientes()
{
    // Find Audio and midi ports
    char temp[128];
    char temp1[128];
    char *masque;
    char *name;
    FILE *fp;

    Settings->BMidiIn->clear();


    if ((fp = fopen("/proc/asound/seq/clients", "r")) != NULL)
    {
        memset(temp, 0, sizeof (temp));

        while (fgets(temp, sizeof temp, fp) != NULL)
        {
            if (strstr(temp, "Port") != NULL)
            {
                strcpy(temp1, temp);
                strtok(temp1, "\"");
                name = strtok(NULL, "\"");
                masque = strtok(NULL, ")");

                if ((masque[2] == 'R') && (strstr(name, "rakarrack-plus MC OUT") == 0))
                {
                    Settings->BMidiIn->add(name);
                }
            }
        }
    }

    fclose(fp);

    Settings->JackCo->clear();

    const char **ports;


    if ((ports = jack_get_ports(rkr->jackclient, NULL, JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsInput)) == NULL)
    {
        fprintf(stderr, "Cannot find any Input port\n");
    }
    else
    {
        int i = 0;

        while (ports[i] != NULL)
        {
            if ((strstr(ports[i], "rakarrack-plus:in_1") == 0) && (strstr(ports[i], "rakarrack-plus:in_2") == 0))
            {
                Settings->JackCo->add(ports[i]);
            }

            i++;
        }
    }

    free(ports);

    Settings->JackIn->clear();

    const char **iports;

    if ((iports = jack_get_ports(rkr->jackclient, NULL, JACK_DEFAULT_AUDIO_TYPE,
            JackPortIsOutput)) == NULL)
    {
        fprintf(stderr, "Cannot find any Output port\n");
    }
    else
    {
        int i = 0;

        while (iports[i] != NULL)
        {
            if ((strstr(iports[i], "rakarrack-plus:out_1") == 0) && (strstr(iports[i], "rakarrack-plus:out_2") == 0))
            {
                Settings->JackIn->add(iports[i]);
            }

            i++;
        }
    }

    free(iports);
}

void RKRGUI::MiraConfig()
{
    // Loads the settings into the settings class
    {
        int i = 1;
        while (Settings->BMidiIn->text(i) != NULL)
        {
            if (strcmp(Settings->BMidiIn->text(i), rkr->MID) == 0)
            {
                Settings->BMidiIn->select(i, 1);
            }
            i++;
        }
    }
    
    {
        int i = 1;
        while (Settings->JackCo->text(i) != NULL)
        {
            for (int k = 0; k < rkr->cuan_jack; k++)
            {
                if (strcmp(Settings->JackCo->text(i), rkr->jack_po[k].name) == 0)
                {
                    Settings->JackCo->select(i, 1);
                }
            }

            i++;
        }
    }

    {
        int i = 1;
        while (Settings->JackIn->text(i) != NULL)
        {
            for (int k = 0; k < rkr->cuan_ijack; k++)
            {
                if (strcmp(Settings->JackIn->text(i), rkr->jack_poi[k].name) == 0)
                {
                    Settings->JackIn->select(i, 1);
                }
            }

            i++;
        }
    }

    if (rkr->MIDIway)
    {
        Settings->Mw1->setonly();
    }
    else
    {
        Settings->Mw0->setonly();
    }

    rkr->m_displayed = 0;
    Settings->Enable_Back->value(rkr->EnableBackgroundImage);
    Settings->Enable_DeacHide->value(rkr->deachide);
    Settings->Enable_Scale->value(rkr->scalable);

    Settings->BFiname->value(rkr->BankFilename);
    Settings->BackFiname->value(rkr->BackgroundImage);
    Settings->Udir->value(rkr->UDirFilename);
    Settings->Username->value(rkr->UserRealName);
    Settings->Pre_Serve->value(rkr->actuvol);
    Settings->LM_Volume->value(rkr->Metro_Vol);
    Settings->Filter_DC->value(rkr->DC_Offset);
    Settings->FLPosition->value(rkr->flpos);
    Settings->DB6B->value(rkr->db6booster);
    Settings->Har_Downsample->value(rkr->Har_Down);
    Settings->Har_Down_Qua->value(rkr->Har_D_Q);
    Settings->Har_Up_Qua->value(rkr->Har_U_Q);
    Settings->Rev_Downsample->value(rkr->Rev_Down);
    Settings->Rev_Down_Qua->value(rkr->Rev_D_Q);
    Settings->Rev_Up_Qua->value(rkr->Rev_U_Q);
    Settings->Con_Downsample->value(rkr->Con_Down);
    Settings->Con_Down_Qua->value(rkr->Con_D_Q);
    Settings->Con_Up_Qua->value(rkr->Con_U_Q);
    Settings->Seq_Downsample->value(rkr->Seq_Down);
    Settings->Seq_Down_Qua->value(rkr->Seq_D_Q);
    Settings->Seq_Up_Qua->value(rkr->Seq_U_Q);
    Settings->Shi_Downsample->value(rkr->Shi_Down);
    Settings->Shi_Down_Qua->value(rkr->Shi_D_Q);
    Settings->Shi_Up_Qua->value(rkr->Shi_U_Q);
    Settings->Voc_Downsample->value(rkr->Voc_Down);
    Settings->Voc_Down_Qua->value(rkr->Voc_D_Q);
    Settings->Voc_Up_Qua->value(rkr->Voc_U_Q);
    Settings->Ste_Downsample->value(rkr->Ste_Down);
    Settings->Ste_Down_Qua->value(rkr->Ste_D_Q);
    Settings->Ste_Up_Qua->value(rkr->Ste_U_Q);
    
    Settings->Dist_Amo->value(rkr->Dist_res_amount);
    Settings->Dist_Up_Qua->value(rkr->Dist_up_q);
    Settings->Dist_Down_Qua->value(rkr->Dist_down_q);
    
    Settings->Ovrd_Amo->value(rkr->Ovrd_res_amount);
    Settings->Ovrd_Up_Qua->value(rkr->Ovrd_up_q);
    Settings->Ovrd_Down_Qua->value(rkr->Ovrd_down_q);
    
    Settings->Dere_Amo->value(rkr->Dere_res_amount);
    Settings->Dere_Up_Qua->value(rkr->Dere_up_q);
    Settings->Dere_Down_Qua->value(rkr->Dere_down_q);
    
    Settings->DBand_Amo->value(rkr->DBand_res_amount);
    Settings->DBand_Up_Qua->value(rkr->DBand_up_q);
    Settings->DBand_Down_Qua->value(rkr->DBand_down_q);
    
    Settings->Stomp_Amo->value(rkr->Stomp_res_amount);
    Settings->Stomp_Up_Qua->value(rkr->Stomp_up_q);
    Settings->Stomp_Down_Qua->value(rkr->Stomp_down_q);

    Settings->Calibration->value(rkr->aFreq);
    Settings->RTrigger->value(rkr->HarmRecNote->trigfact);
    Settings->RC_Harm_Opti->value(rkr->RCOpti_Harm);
    Settings->RC_St_Harm_Opti->value(rkr->RCOpti_Stereo);
    Settings->RC_Ring_Opti->value(rkr->RCOpti_Ring);


    switch (rkr->VocBands)
    {
        case 16:
            Settings->Voc_Bands->value(0);
            break;
        case 32:
            Settings->Voc_Bands->value(1);
            break;
        case 64:
            Settings->Voc_Bands->value(2);
            break;
        case 128:
            Settings->Voc_Bands->value(3);
            break;
        case 256:
            Settings->Voc_Bands->value(4);
            break;

    }


    Settings->MTable->value(rkr->midi_table);

    Settings->AAssign->value(rkr->autoassign);
    Settings->Update_TAP->value(rkr->Tap_Updated);
    Settings->INSTATE->value(rkr->init_state);
    Settings->UPSAMPLE_C->value(rkr->upsample);
    Settings->Upr_Qual->value(rkr->UpQual);
    Settings->Downr_Qual->value(rkr->DownQual);
    Settings->MES_DIS->value(rkr->mess_dis);
    Settings->ENA_TOOL->value(rkr->ena_tool);
    Settings->T_TIMEOUT->value(rkr->t_timeout);

    Settings->Upr_Amo->value(rkr->UpAmo);
    Settings->L_SIZE->value(rkr->looper_size);
    Settings->D_A_Connect->value(rkr->aconnect_MI);
    Settings->D_J_Connect->value(rkr->aconnect_JA);
    Settings->D_IJ_Connect->value(rkr->aconnect_JIA);

    Settings->Midi_In_Counter->value(rkr->MidiCh + 1);
    Settings->Har_In_Counter->value(rkr->HarCh + 1);
    Settings->Stereo_Har_In_Counter->value(rkr->StereoHarCh + 1);

    switch (rkr->HarQual)
    {
        case 4:
            Settings->Har_Qual->value(0);
            break;

        case 8:
            Settings->Har_Qual->value(1);
            break;

        case 16:
            Settings->Har_Qual->value(2);
            break;

        case 32:
            Settings->Har_Qual->value(3);
            break;
    }

    switch (rkr->SteQual)
    {
        case 4:
            Settings->Ste_Qual->value(0);
            break;

        case 8:
            Settings->Ste_Qual->value(1);
            break;

        case 16:
            Settings->Ste_Qual->value(2);
            break;

        case 32:
            Settings->Ste_Qual->value(3);
            break;
    }
    
    switch (rkr->SeqQual)
    {
        case 4:
            Settings->Seq_Qual->value(0);
            break;

        case 8:
            Settings->Seq_Qual->value(1);
            break;

        case 16:
            Settings->Seq_Qual->value(2);
            break;

        case 32:
            Settings->Seq_Qual->value(3);
            break;
    }

    switch (rkr->ShiQual)
    {
        case 4:
            Settings->Shi_Qual->value(0);
            break;

        case 8:
            Settings->Shi_Qual->value(1);
            break;

        case 16:
            Settings->Shi_Qual->value(2);
            break;

        case 32:
            Settings->Shi_Qual->value(3);
            break;
    }


    if (rkr->aconnect_MI)
    {
        Settings->BMidiIn->activate();
    }
    else
    {
        Settings->BMidiIn->deactivate();
    }

    if (rkr->aconnect_JA)
    {
        Settings->JackCo->activate();
    }
    else
    {
        Settings->JackCo->deactivate();
    }

    if (rkr->aconnect_JIA)
    {
        Settings->JackIn->activate();
    }
    else
    {
        Settings->JackIn->deactivate();
    }

    Fl_Menu_Item *p;
    Fl_Menu_Item *Har = Settings->get_menu_Har_Downsample();
    unsigned int SR_value = rkr->sample_rate;

    for (int j = 0; j < Har->size(); j++)
    {
        p = Har->next(j);

        switch (j)
        {
            case 0:
                SR_value = rkr->sample_rate;
                break;
            case 1:
                SR_value = 96000;
                break;
            case 2:
                SR_value = 48000;
                break;
            case 3:
                SR_value = 44100;
                break;
            case 4:
                SR_value = 32000;
                break;
            case 5:
                SR_value = 22050;
                break;
            case 6:
                SR_value = 16000;
                break;
            case 7:
                SR_value = 12000;
                break;
            case 8:
                SR_value = 8000;
                break;
            case 9:
                SR_value = 4000;
                break;
        }

        if ((j > 0) && ((unsigned int) rkr->sample_rate <= SR_value))
        {
            p->deactivate();
        }
        else
        {
            p->activate();
        }
    }


    Settings->Font_Bro->clear();
    Settings->Font_Bro->textcolor(label_color);

    int k = Fl::set_fonts(0);
    for (int i = 0; i < k; i++)
    {
        int t;
        const char *name = Fl::get_font_name((Fl_Font) i, &t);
        char buffer[128];
#if 1
        if (t)
        {
            char *p = buffer;
            if (t & FL_BOLD)
            {
                *p++ = '@';
                *p++ = 'b';
            }
            if (t & FL_ITALIC)
            {
                *p++ = '@';
                *p++ = 'i';
            }
            strcpy(p, name);
            name = buffer;
        }
#else // this is neat, but really slow on some X servers:
        sprintf(buffer, "@F%d@.%s", i, name);
        name = buffer;
#endif
        Settings->Font_Bro->add(name);
    }

    Settings->Font_Bro->value(rkr->font + 1);
}

void RKRGUI::BankWin_Label(char *filename)
{
    // add the bankwindow label
    char tmp[256];

    memset(tmp, 0, sizeof (tmp));
    sprintf(tmp, "%s   v%s - Bank Manager - %s", rkr->jackcliname, VERSION, fl_filename_name(filename));
    BankWindow->copy_label(tmp);
}

void RKRGUI::is_modified()
{
    // popup dialog for modified bank saving

    if (rkr->modified)
    {
        Fl_Widget *w = fl_message_icon();
        w->parent()->copy_label(rkr->jackcliname);

        int ok = fl_choice("Bank was modified, but not saved", "Discard", "Save", NULL);

        switch (ok)
        {
            case 0:
                break;

            case 1:
                set_save_file();
                break;
        }
    }
}

void RKRGUI::Put_Loaded_Bank()
{
    // Put loaded bank
    int k = 1;

    for (int t = 0; t < BankWindow->ob->children(); t++)
    {
        Fl_Widget *w = BankWindow->ob->child(t);
        long long temp = (long long) w->user_data();
        if (temp > 0)
        {
            w->copy_label(rkr->Bank[k].Preset_Name);
            k++;
        }
    }

    FillML();
}

void RKRGUI::Chord(int eff)
{
    // display selected chord in harmonizer and stereo harmonizer
    int tipo, undi;
    tipo = 0;
    undi = 0;

    switch (eff)
    {
        case 0:
            tipo = rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Chord);
            undi = rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Note);
            break;
        case 1:
            tipo = rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_Chord);
            undi = rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_Note);
            break;
    }


    if (eff == 0)
    {
        sprintf(rkr->RC_Harm->NombreAcorde, "%s%s", rkr->RC_Harm->NCE[undi].Nom, rkr->RC_Harm->ChN[tipo].Nom);

        rkr->RC_Harm->ctipo = tipo;
        rkr->RC_Harm->fundi = undi;
        rkr->RC_Harm->cc = 1;
    }
    else
    {
        sprintf(rkr->RC_Stereo_Harm->NombreAcorde, "%s%s", rkr->RC_Stereo_Harm->NCE[undi].Nom, rkr->RC_Stereo_Harm->ChN[tipo].Nom);

        rkr->RC_Stereo_Harm->ctipo = tipo;
        rkr->RC_Stereo_Harm->fundi = undi;
        rkr->RC_Stereo_Harm->cc = 1;
    }
}

/**
 *  Adjust Gui parameters from MIDI control action.
 */
void RKRGUI::MIDI_control_gui_refresh()
{
    // Mvalue is flag to indicate at least one parameter needs update
    if (rkr->Mvalue == 0)
        return;

    // Index starts at 1, since case 0 is unused
    for (int i = 1; i < C_MC_RANGE; i++)
    {
        // Mcontrol is flag for each individual parameter
        // 1 = the parameter needs update, 0 = no update needed
        if (rkr->Mcontrol[i] == 0)
        {
            continue;   // No gui update needed
        }

        rkr->Mcontrol[i] = 0;   // clear the parameter flag

        switch (i)
        {
            case MC_Unused_0:
                break;

// Start MIDI Implementation Chart range

            case MC_WahWah_Depth:
                WAHWAH->WahWah_dpth->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_Depth));
                WAHWAH->WahWah_dpth->redraw();
                break;
            case MC_Dere_Drive:
                DERELICT->derelict_drive->value(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_Drive));
                DERELICT->derelict_drive->redraw();
                break;
            case MC_Dere_Level:
                DERELICT->derelict_level->value(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_Level));
                DERELICT->derelict_level->redraw();
                break;
            case MC_Dere_LPF:
                DERELICT->derelict_lpf->value(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_LPF));
                DERELICT->derelict_lpf->redraw();
                break;
            case MC_Dere_HPF:
                DERELICT->derelict_hpf->value(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_HPF));
                DERELICT->derelict_hpf->redraw();
                break;
            case MC_Dere_Color:
                DERELICT->derelict_st->value(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_Color));
                DERELICT->derelict_st->redraw();
                break;
            case MC_Output_Volume:
                Nivel_Salida->value((int) (rkr->Master_Volume * 100.0) - 50);
                Nivel_Salida->redraw();
                break;
            case MC_Dere_Suboctave:
                DERELICT->derelict_oct->value(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_Suboctave));
                DERELICT->derelict_oct->redraw();
                break;
            case MC_Dist_Suboctave:
                DIST->dist_oct->value(rkr->Rack_Effects[EFX_DISTORTION]->getpar(Dist_Suboctave));
                DIST->dist_oct->redraw();
                break;

            case MC_Unused_10:
            case MC_Unused_11:
                break;

            case MC_Balance_FX:
                Balance->value(rkr->Fraction_Bypass * 100.0);
                Balance->redraw();
                break;

            case MC_Unused_13:
                break;

            case MC_Input_Volume:
                Nivel_Entrada->value((int) (rkr->Input_Gain * 100.0) - 50);
                Nivel_Entrada->redraw();
                break;

            case MC_Unused_15:
            case MC_Unused_16:
            case MC_Unused_17:
            case MC_Unused_18:
            case MC_Unused_19:
                break;

            case MC_Alien_Depth:
                ALIENWAH->Alienwah_dpth->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_Depth));
                ALIENWAH->Alienwah_dpth->redraw();
                break;
            case MC_Phaser_Depth:
                PHASER->phaser_dpth->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_Depth));
                PHASER->phaser_dpth->redraw();
                break;
            case MC_Flanger_Depth:
                FLANGER->flanger_dpth->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_Depth));
                FLANGER->flanger_dpth->redraw();
                break;
            case MC_Chorus_Depth:
                CHORUS->chorus_dpth->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_Depth));
                CHORUS->chorus_dpth->redraw();
                break;
            case MC_Music_Gain_1:
                MUSDELAY->musdelay_gain1->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Gain_1));
                MUSDELAY->musdelay_gain1->redraw();
                break;
            case MC_Music_Gain_2:
                MUSDELAY->musdelay_gain2->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Gain_2));
                MUSDELAY->musdelay_gain2->redraw();
                break;
            case MC_Harm_Filter_Freq:
                HAR->har_freq1->value(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Filter_Freq));
                HAR->har_freq1->redraw();
                break;
            case MC_Harm_Interval:
                HAR->har_int->value(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Interval) - 12);
                HAR->har_int->redraw();
                break;
            case MC_WahWah_DryWet:
                WAHWAH->WahWah_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_DryWet)));
                WAHWAH->WahWah_WD->redraw();
                break;
            case MC_Overdrive_DryWet:
                OVRD->ovrd_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_OVERDRIVE]->getpar(Overdrive_DryWet)));
                OVRD->ovrd_WD->redraw();
                break;
            case MC_Dist_DryWet:
                DIST->dist_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_DISTORTION]->getpar(Dist_DryWet)));
                DIST->dist_WD->redraw();
                break;
            case MC_Harm_DryWet:
                HAR->har_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_DryWet)));
                HAR->har_WD->redraw();
                break;

            case MC_Unused_32:
            case MC_Unused_33:
            case MC_Unused_34:
            case MC_Unused_35:
            case MC_Unused_36:
            case MC_Unused_37:
            case MC_Unused_38:
            case MC_Unused_39:
            case MC_Unused_40:
            case MC_Unused_41:
            case MC_Unused_42:
            case MC_Unused_43:
            case MC_Unused_44:
            case MC_Unused_45:
                break;

            case MC_Echo_Pan:
                ECHO->echo_pan->value(rkr->Rack_Effects[EFX_ECHO]->getpar(Echo_Pan) - 64);
                ECHO->echo_pan->redraw();
                break;
            case MC_Overdrive_Pan:
                OVRD->ovrd_pan->value(rkr->Rack_Effects[EFX_OVERDRIVE]->getpar(Overdrive_Pan) - 64);
                OVRD->ovrd_pan->redraw();
                break;
            case MC_Dist_Pan:
                DIST->dist_pan->value(rkr->Rack_Effects[EFX_DISTORTION]->getpar(Dist_Pan) - 64);
                DIST->dist_pan->redraw();
                break;
            case MC_Harm_Pan:
                HAR->har_pan->value(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Pan) - 64);
                HAR->har_pan->redraw();
                break;
            case MC_Chorus_Pan:
                CHORUS->chorus_pan->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_Pan) - 64);
                CHORUS->chorus_pan->redraw();
                break;
            case MC_Flanger_Pan:
                FLANGER->flanger_pan->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_Pan) - 64);
                FLANGER->flanger_pan->redraw();
                break;
            case MC_Chorus_DryWet:
                CHORUS->chorus_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_DryWet)));
                CHORUS->chorus_WD->redraw();
                break;
            case MC_Flanger_DryWet:
                FLANGER->flanger_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_DryWet)));
                FLANGER->flanger_WD->redraw();
                break;
            case MC_Phaser_DryWet:
                PHASER->phaser_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_DryWet)));
                PHASER->phaser_WD->redraw();
                break;
            case MC_Alien_DryWet:
                ALIENWAH->Alienwah_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_DryWet)));
                ALIENWAH->Alienwah_WD->redraw();
                break;
            case MC_Music_DryWet:
                MUSDELAY->musdelay_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_DryWet)));
                MUSDELAY->musdelay_WD->redraw();
                break;
            case MC_Reverb_DryWet:
                REVERB->reverb_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_REVERB]->getpar(Reverb_DryWet)));
                REVERB->reverb_WD->redraw();
                break;
            case MC_Pan_DryWet:
                PAN->pan_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_PAN]->getpar(Pan_DryWet)));
                PAN->pan_WD->redraw();
                break;
            case MC_Echo_DryWet:
                ECHO->echo_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_ECHO]->getpar(Echo_DryWet)));
                ECHO->echo_WD->redraw();
                break;
            case MC_Phaser_Pan:
                PHASER->phaser_pan->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_Pan) - 64);
                PHASER->phaser_pan->redraw();
                break;
            case MC_Alien_Pan:
                ALIENWAH->Alienwah_pan->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_Pan) - 64);
                ALIENWAH->Alienwah_pan->redraw();
                break;
            case MC_Music_Pan_1:
                MUSDELAY->musdelay_pan1->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Pan_1) - 64);
                MUSDELAY->musdelay_pan1->redraw();
                break;
            case MC_Reverb_Pan:
                REVERB->reverb_pan->value(rkr->Rack_Effects[EFX_REVERB]->getpar(Reverb_Pan) - 64);
                REVERB->reverb_pan->redraw();
                break;

            case MC_Unused_64:
                break;

            case MC_Music_Pan_2:
                MUSDELAY->musdelay_pan2->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Pan_2) - 64);
                MUSDELAY->musdelay_pan2->redraw();
                break;
            case MC_WahWah_Pan:
                WAHWAH->WahWah_pan->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_Pan) - 64);
                WAHWAH->WahWah_pan->redraw();
                break;
            case MC_Pan_Pan:
                PAN->pan_pan->value(rkr->Rack_Effects[EFX_PAN]->getpar(Pan_Pan) - 64);
                PAN->pan_pan->redraw();
                break;
            case MC_Overdrive_Drive:
                OVRD->ovrd_drive->value(rkr->Rack_Effects[EFX_OVERDRIVE]->getpar(Overdrive_Drive));
                OVRD->ovrd_drive->redraw();
                break;
            case MC_Dist_Drive:
                DIST->dist_drive->value(rkr->Rack_Effects[EFX_DISTORTION]->getpar(Dist_Drive));
                DIST->dist_drive->redraw();
                break;
            case MC_Overdrive_Level:
                OVRD->ovrd_level->value(rkr->Rack_Effects[EFX_OVERDRIVE]->getpar(Overdrive_Level));
                OVRD->ovrd_level->redraw();
                break;
            case MC_Dist_Level:
                DIST->dist_level->value(rkr->Rack_Effects[EFX_DISTORTION]->getpar(Dist_Level));
                DIST->dist_level->redraw();
                break;
            case MC_Chorus_LFO_Tempo:
                CHORUS->chorus_freq->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_LFO_Tempo));
                CHORUS->chorus_freq->redraw();
                break;
            case MC_Flanger_LFO_Tempo:
                FLANGER->flanger_freq->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_LFO_Tempo));
                FLANGER->flanger_freq->redraw();
                break;
            case MC_Phaser_LFO_Tempo:
                PHASER->phaser_freq->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_LFO_Tempo));
                PHASER->phaser_freq->redraw();
                break;
            case MC_WahWah_LFO_Tempo:
                WAHWAH->WahWah_freq->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_LFO_Tempo));
                WAHWAH->WahWah_freq->redraw();
                break;
            case MC_Alien_LFO_Tempo:
                ALIENWAH->Alienwah_freq->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_LFO_Tempo));
                ALIENWAH->Alienwah_freq->redraw();
                break;
            case MC_Pan_LFO_Tempo:
                PAN->pan_freq->value(rkr->Rack_Effects[EFX_PAN]->getpar(Pan_LFO_Tempo));
                PAN->pan_freq->redraw();
                break;
            case MC_Echo_Feedback:
                ECHO->echo_fb->value(rkr->Rack_Effects[EFX_ECHO]->getpar(Echo_Feedback));
                ECHO->echo_fb->redraw();
                break;
            case MC_Chorus_Feedback:
                CHORUS->chorus_fb->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_Feedback));
                CHORUS->chorus_fb->redraw();
                break;
            case MC_Flanger_Feedback:
                FLANGER->flanger_fb->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_Feedback));
                FLANGER->flanger_fb->redraw();
                break;
            case MC_Phaser_Feedback:
                PHASER->phaser_fb->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_Feedback));
                PHASER->phaser_fb->redraw();
                break;
            case MC_Alien_Feedback:
                ALIENWAH->Alienwah_fb->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_Feedback));
                ALIENWAH->Alienwah_fb->redraw();
                break;
            case MC_Music_Feedback_1:
                MUSDELAY->musdelay_fb1->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Feedback_1));
                MUSDELAY->musdelay_fb1->redraw();
                break;
            case MC_Music_Feedback_2:
                MUSDELAY->musdelay_fb2->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Feedback_2));
                MUSDELAY->musdelay_fb2->redraw();
                break;
            case MC_Overdrive_LPF:
                OVRD->ovrd_lpf->value(rkr->Rack_Effects[EFX_OVERDRIVE]->getpar(Overdrive_LPF));
                OVRD->ovrd_lpf->redraw();
                break;
            case MC_Dist_LPF:
                DIST->dist_lpf->value(rkr->Rack_Effects[EFX_DISTORTION]->getpar(Dist_LPF));
                DIST->dist_lpf->redraw();
                break;
            case MC_Reverb_LPF:
                REVERB->reverb_LPF->value(rkr->Rack_Effects[EFX_REVERB]->getpar(Reverb_LPF));
                REVERB->reverb_LPF->redraw();
                break;
            case MC_Overdrive_HPF:
                OVRD->ovrd_hpf->value(rkr->Rack_Effects[EFX_OVERDRIVE]->getpar(Overdrive_HPF));
                OVRD->ovrd_hpf->redraw();
                break;
            case MC_Dist_HPF:
                DIST->dist_hpf->value(rkr->Rack_Effects[EFX_DISTORTION]->getpar(Dist_HPF));
                DIST->dist_hpf->redraw();
                break;
            case MC_Reverb_HPF:
                REVERB->reverb_HPF->value(rkr->Rack_Effects[EFX_REVERB]->getpar(Reverb_HPF));
                REVERB->reverb_HPF->redraw();
                break;
            case MC_Chorus_LR_Cross:
                CHORUS->chorus_LR->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_LR_Cross));
                CHORUS->chorus_LR->redraw();
                break;
            case MC_Flanger_LR_Cross:
                FLANGER->flanger_LR->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_LR_Cross));
                FLANGER->flanger_LR->redraw();
                break;
            case MC_Phaser_LR_Cross:
                PHASER->phaser_LR->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_LR_Cross) - 64);
                PHASER->phaser_LR->redraw();
                break;
            case MC_Overdrive_LR_Cross:
                OVRD->ovrd_LRc->value(rkr->Rack_Effects[EFX_OVERDRIVE]->getpar(Overdrive_LR_Cross));
                OVRD->ovrd_LRc->redraw();
                break;
            case MC_Dist_LR_Cross:
                DIST->dist_LRc->value(rkr->Rack_Effects[EFX_DISTORTION]->getpar(Dist_LR_Cross));
                DIST->dist_LRc->redraw();
                break;
            case MC_Alien_LR_Cross:
                ALIENWAH->Alienwah_LR->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_LR_Cross));
                ALIENWAH->Alienwah_LR->redraw();
                break;
            case MC_Echo_LR_Cross:
                ECHO->echo_LRc->value(rkr->Rack_Effects[EFX_ECHO]->getpar(Echo_LR_Cross));
                ECHO->echo_LRc->redraw();
                break;
            case MC_Music_LR_Cross:
                MUSDELAY->musdelay_LRc->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_LR_Cross));
                MUSDELAY->musdelay_LRc->redraw();
                break;
            case MC_Chorus_LFO_Stereo:
                CHORUS->chorus_stdf->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_LFO_Stereo));
                CHORUS->chorus_stdf->redraw();
                break;
            case MC_Flanger_LFO_Stereo:
                FLANGER->flanger_stdf->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_LFO_Stereo));
                FLANGER->flanger_stdf->redraw();
                break;
            case MC_Phaser_LFO_Stereo:
                PHASER->phaser_stdf->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_LFO_Stereo));
                PHASER->phaser_stdf->redraw();
                break;
            case MC_WahWah_LFO_Stereo:
                WAHWAH->WahWah_stdf->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_LFO_Stereo));
                WAHWAH->WahWah_stdf->redraw();
                break;
            case MC_Alien_LFO_Stereo:
                ALIENWAH->Alienwah_stdf->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_LFO_Stereo));
                ALIENWAH->Alienwah_stdf->redraw();
                break;
            case MC_Pan_LFO_Stereo:
                PAN->pan_stdf->value(rkr->Rack_Effects[EFX_PAN]->getpar(Pan_LFO_Stereo));
                PAN->pan_stdf->redraw();
                break;
            case MC_Chorus_LFO_Random:
                CHORUS->chorus_rnd->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_LFO_Random));
                CHORUS->chorus_rnd->redraw();
                break;
            case MC_Flanger_LFO_Random:
                FLANGER->flanger_rnd->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_LFO_Random));
                FLANGER->flanger_rnd->redraw();
                break;
            case MC_Phaser_LFO_Random:
                PHASER->phaser_rnd->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_LFO_Random));
                PHASER->phaser_rnd->redraw();
                break;
            case MC_WahWah_LFO_Random:
                WAHWAH->WahWah_rnd->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_LFO_Random));
                WAHWAH->WahWah_rnd->redraw();
                break;
            case MC_Alien_LFO_Random:
                ALIENWAH->Alienwah_rnd->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_LFO_Random));
                ALIENWAH->Alienwah_rnd->redraw();
                break;
            case MC_Pan_LFO_Random:
                PAN->pan_rnd->value(rkr->Rack_Effects[EFX_PAN]->getpar(Pan_LFO_Random));
                PAN->pan_rnd->redraw();
                break;
            case MC_WahWah_Sense:
                WAHWAH->WahWah_ampsns->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_Sense));
                WAHWAH->WahWah_ampsns->redraw();
                break;
            case MC_WahWah_ASI:
                WAHWAH->WahWah_ampsnsinv->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_ASI));
                WAHWAH->WahWah_ampsnsinv->redraw();
                break;
            case MC_WahWah_Smooth:
                WAHWAH->WahWah_smooth->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_Smooth));
                WAHWAH->WahWah_smooth->redraw();
                break;
            case MC_Phaser_Phase:
                PHASER->phaser_phase->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_Phase));
                PHASER->phaser_phase->redraw();
                break;
            case MC_Alien_Phase:
                ALIENWAH->Alienwah_phase->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_Phase));
                ALIENWAH->Alienwah_phase->redraw();
                break;
            case MC_Multi_On_Off:
                ActOnOff();
                break;
            case MC_APhase_DryWet:
                APHASER->aphaser_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_DryWet)));
                APHASER->aphaser_WD->redraw();
                break;
            case MC_APhase_Distortion:
                APHASER->aphaser_distort->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_Distortion));
                APHASER->aphaser_distort->redraw();
                break;
            case MC_APhase_LFO_Tempo:
                APHASER->aphaser_freq->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_LFO_Tempo));
                APHASER->aphaser_freq->redraw();
                break;
            case MC_APhase_Depth:
                APHASER->aphaser_depth->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_Depth));
                APHASER->aphaser_depth->redraw();
                break;
            case MC_APhase_Width:
                APHASER->aphaser_width->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_Width));
                APHASER->aphaser_width->redraw();
                break;
            case MC_APhase_Feedback:
                APHASER->aphaser_fb->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_Feedback) - 64);
                APHASER->aphaser_fb->redraw();
                break;
            case MC_APhase_Mismatch:
                APHASER->aphaser_mismatch->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_Mismatch));
                APHASER->aphaser_mismatch->redraw();
                break;
            case MC_APhase_LFO_Stereo:
                APHASER->aphaser_stdf->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_LFO_Stereo));
                APHASER->aphaser_stdf->redraw();
                break;
            case MC_Dere_DryWet:
                DERELICT->derelict_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_DryWet)));
                DERELICT->derelict_WD->redraw();
                break;
            case MC_Dere_Pan:
                DERELICT->derelict_pan->value(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_Pan) - 64);
                DERELICT->derelict_pan->redraw();
                break;
            case MC_Dere_LR_Cross:
                DERELICT->derelict_LRc->value(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_LR_Cross));
                DERELICT->derelict_LRc->redraw();
                break;

// End MIDI Implementation Chart range
// Start of MIDI learn extras

            case MC_Unused_128:
            case MC_Unused_129:
                break;
            case MC_EQ_Gain:
                EQ->eq_Gain->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_Gain) - 64);
                EQ->eq_Gain->redraw();
                break;
            case MC_EQ_Q:
                EQ->eq_Q->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_Q) - 64);
                EQ->eq_Q->redraw();
                break;
            case MC_EQ_31_HZ:
                EQ->eq_1->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_31_HZ) - 64);
                EQ->eq_1->redraw();
                break;
            case MC_EQ_63_HZ:
                EQ->eq_2->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_63_HZ) - 64);
                EQ->eq_2->redraw();
                break;
            case MC_EQ_125_HZ:
                EQ->eq_3->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_125_HZ) - 64);
                EQ->eq_3->redraw();
                break;
            case MC_EQ_250_HZ:
                EQ->eq_4->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_250_HZ) - 64);
                EQ->eq_4->redraw();
                break;
            case MC_EQ_500_HZ:
                EQ->eq_5->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_500_HZ) - 64);
                EQ->eq_5->redraw();
                break;
            case MC_EQ_1_KHZ:
                EQ->eq_6->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_1_KHZ) - 64);
                EQ->eq_6->redraw();
                break;
            case MC_EQ_2_KHZ:
                EQ->eq_7->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_2_KHZ) - 64);
                EQ->eq_7->redraw();
                break;
            case MC_EQ_4_KHZ:
                EQ->eq_8->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_4_KHZ) - 64);
                EQ->eq_8->redraw();
                break;
            case MC_EQ_8_KHZ:
                EQ->eq_9->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_8_KHZ) - 64);
                EQ->eq_9->redraw();
                break;
            case MC_EQ_16_KHZ:
                EQ->eq_10->value(rkr->Rack_Effects[EFX_EQ]->getpar(EQ_16_KHZ) - 64);
                EQ->eq_10->redraw();
                break;
            case MC_Compress_Attack:
                COMPRESS->compress_ATime->value(rkr->Rack_Effects[EFX_COMPRESSOR]->getpar(Compress_Attack));
                COMPRESS->compress_ATime->redraw();
                break;
            case MC_Compress_Release:
                COMPRESS->compress_RTime->value(rkr->Rack_Effects[EFX_COMPRESSOR]->getpar(Compress_Release));
                COMPRESS->compress_RTime->redraw();
                break;
            case MC_Compress_Ratio:
                COMPRESS->compress_Ratio->value(rkr->Rack_Effects[EFX_COMPRESSOR]->getpar(Compress_Ratio));
                COMPRESS->compress_Ratio->redraw();
                break;
            case MC_Compress_Knee:
                COMPRESS->compress_Knee->value(rkr->Rack_Effects[EFX_COMPRESSOR]->getpar(Compress_Knee));
                COMPRESS->compress_Knee->redraw();
                break;
            case MC_Compress_Threshold:
                COMPRESS->compress_threshold->value(rkr->Rack_Effects[EFX_COMPRESSOR]->getpar(Compress_Threshold));
                COMPRESS->compress_threshold->redraw();
                break;
            case MC_Compress_Output:
                COMPRESS->compress_output->value(rkr->Rack_Effects[EFX_COMPRESSOR]->getpar(Compress_Output));
                COMPRESS->compress_output->redraw();
                break;
            case MC_Parametric_Gain:
                PEQ->eqp_Gain->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_Gain) - 64);
                PEQ->eqp_Gain->redraw();
                break;
            case MC_Parametric_Low_Freq:
                PEQ->eqp_LF->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_Low_Freq));
                PEQ->eqp_LF->redraw();
                break;
            case MC_Parametric_Low_Gain:
                PEQ->eqp_LFg->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_Low_Gain) - 64);
                PEQ->eqp_LFg->redraw();
                break;
            case MC_Parametric_Low_Q:
                PEQ->eqp_LQ->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_Low_Q) - 64);
                PEQ->eqp_LQ->redraw();
                break;
            case MC_Parametric_Mid_Freq:
                PEQ->eqp_MF->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_Mid_Freq));
                PEQ->eqp_MF->redraw();
                break;
            case MC_Parametric_Mid_Gain:
                PEQ->eqp_MFg->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_Mid_Gain) - 64);
                PEQ->eqp_MFg->redraw();
                break;
            case MC_Parametric_Mid_Q:
                PEQ->eqp_MQ->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_Mid_Q) - 64);
                PEQ->eqp_MQ->redraw();
                break;
            case MC_Parametric_High_Freq:
                PEQ->eqp_HF->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_High_Freq));
                PEQ->eqp_HF->redraw();
                break;
            case MC_Parametric_High_Gain:
                PEQ->eqp_HFg->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_High_Gain) - 64);
                PEQ->eqp_HFg->redraw();
                break;
            case MC_Parametric_High_Q:
                PEQ->eqp_HQ->value(rkr->Rack_Effects[EFX_PARAMETRIC]->getpar(Parametric_High_Q) - 64);
                PEQ->eqp_HQ->redraw();
                break;
            case MC_DFlange_DryWet:
                DFLANGE->dflange_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_DryWet)));
                DFLANGE->dflange_WD->redraw();
                break;
            case MC_DFlange_Pan:
                DFLANGE->dflange_pan->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_Pan));
                DFLANGE->dflange_pan->redraw();
                break;
            case MC_DFlange_LR_Cross:
                DFLANGE->dflange_LR->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_LR_Cross));
                DFLANGE->dflange_LR->redraw();
                break;
            case MC_DFlange_Depth:
                DFLANGE->dflange_depth->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_Depth));
                DFLANGE->dflange_depth->redraw();
                break;
            case MC_DFlange_Width:
                DFLANGE->dflange_width->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_Width));
                DFLANGE->dflange_width->redraw();
                break;
            case MC_DFlange_Offset:
                DFLANGE->dflange_offset->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_Offset));
                DFLANGE->dflange_offset->redraw();
                break;
            case MC_DFlange_Feedback:
                DFLANGE->dflange_fb->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_Feedback));
                DFLANGE->dflange_fb->redraw();
                break;
            case MC_DFlange_LPF:
                DFLANGE->dflange_lpf->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_LPF));
                DFLANGE->dflange_lpf->redraw();
                break;
            case MC_DFlange_LFO_Tempo:
                DFLANGE->dflange_freq->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_LFO_Tempo));
                DFLANGE->dflange_freq->redraw();
                break;
            case MC_DFlange_LFO_Stereo:
                DFLANGE->dflange_stdf->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_LFO_Stereo));
                DFLANGE->dflange_stdf->redraw();
                break;
            case MC_DFlange_LFO_Random:
                DFLANGE->dflange_rnd->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_LFO_Random));
                DFLANGE->dflange_rnd->redraw();
                break;
            case MC_Valve_DryWet:
                VALVE->valve_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_VALVE]->getpar(Valve_DryWet)));
                VALVE->valve_WD->redraw();
                break;
            case MC_Valve_LR_Cross:
                VALVE->valve_LRc->value(rkr->Rack_Effects[EFX_VALVE]->getpar(Valve_LR_Cross));
                VALVE->valve_LRc->redraw();
                break;
            case MC_Valve_Pan:
                VALVE->valve_pan->value(rkr->Rack_Effects[EFX_VALVE]->getpar(Valve_Pan) - 64);
                VALVE->valve_pan->redraw();
                break;
            case MC_Valve_Level:
                VALVE->valve_level->value(rkr->Rack_Effects[EFX_VALVE]->getpar(Valve_Level));
                VALVE->valve_level->redraw();
                break;
            case MC_Valve_Drive:
                VALVE->valve_drive->value(rkr->Rack_Effects[EFX_VALVE]->getpar(Valve_Drive));
                VALVE->valve_drive->redraw();
                break;
            case MC_Valve_Distortion:
                VALVE->valve_Q->value(rkr->Rack_Effects[EFX_VALVE]->getpar(Valve_Distortion));
                VALVE->valve_Q->redraw();
                break;
            case MC_Valve_Presence:
                VALVE->valve_Pre->value(rkr->Rack_Effects[EFX_VALVE]->getpar(Valve_Presence));
                VALVE->valve_Pre->redraw();
                break;
            case MC_Valve_LPF:
                VALVE->valve_lpf->value(rkr->Rack_Effects[EFX_VALVE]->getpar(Valve_LPF));
                VALVE->valve_lpf->redraw();
                break;
            case MC_Valve_HPF:
                VALVE->valve_hpf->value(rkr->Rack_Effects[EFX_VALVE]->getpar(Valve_HPF));
                VALVE->valve_hpf->redraw();
                break;
            case MC_Ring_DryWet:
                RING->ring_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_RING]->getpar(Ring_DryWet)));
                RING->ring_WD->redraw();
                break;
            case MC_Ring_LR_Cross:
                RING->ring_LRc->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_LR_Cross));
                RING->ring_LRc->redraw();
                break;
            case MC_Ring_Input:
                RING->ring_input->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_Input));
                RING->ring_input->redraw();
                break;
            case MC_Ring_Level:
                RING->ring_level->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_Level));
                RING->ring_level->redraw();
                break;
            case MC_Ring_Pan:
                RING->ring_pan->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_Pan));
                RING->ring_pan->redraw();
                break;
            case MC_Ring_Depth:
                RING->ring_depth->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_Depth));
                RING->ring_depth->redraw();
                break;
            case MC_Ring_Freq:
                RING->ring_freq->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_Freq));
                RING->ring_freq->redraw();
                break;
            case MC_Ring_Sine:
                RING->ring_sin->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_Sine));
                RING->ring_sin->redraw();
                break;
            case MC_Ring_Triangle:
                RING->ring_tri->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_Triangle));
                RING->ring_tri->redraw();
                break;
            case MC_Ring_Saw:
                RING->ring_saw->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_Saw));
                RING->ring_saw->redraw();
                break;
            case MC_Ring_Square:
                RING->ring_squ->value(rkr->Rack_Effects[EFX_RING]->getpar(Ring_Square));
                RING->ring_squ->redraw();
                break;
            case MC_Exciter_Gain:
                EXCITER->ex_Gain->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Gain));
                EXCITER->ex_Gain->redraw();
                break;
            case MC_Exciter_LPF:
                EXCITER->ex_lfreq->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_LPF));
                EXCITER->ex_lfreq->redraw();
                break;
            case MC_Exciter_HPF:
                EXCITER->ex_hfreq->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_HPF));
                EXCITER->ex_hfreq->redraw();
                break;
            case MC_Exciter_Harm_1:
                EXCITER->ex_1->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_1));
                EXCITER->ex_1->redraw();
                break;
            case MC_Exciter_Harm_2:
                EXCITER->ex_2->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_2));
                EXCITER->ex_2->redraw();
                break;
            case MC_Exciter_Harm_3:
                EXCITER->ex_3->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_3));
                EXCITER->ex_3->redraw();
                break;
            case MC_Exciter_Harm_4:
                EXCITER->ex_4->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_4));
                EXCITER->ex_4->redraw();
                break;
            case MC_Exciter_Harm_5:
                EXCITER->ex_5->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_5));
                EXCITER->ex_5->redraw();
                break;
            case MC_Exciter_Harm_6:
                EXCITER->ex_6->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_6));
                EXCITER->ex_6->redraw();
                break;
            case MC_Exciter_Harm_7:
                EXCITER->ex_7->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_7));
                EXCITER->ex_7->redraw();
                break;
            case MC_Exciter_Harm_8:
                EXCITER->ex_8->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_8));
                EXCITER->ex_8->redraw();
                break;
            case MC_Exciter_Harm_9:
                EXCITER->ex_9->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_9));
                EXCITER->ex_9->redraw();
                break;
            case MC_Exciter_Harm_10:
                EXCITER->ex_10->value(rkr->Rack_Effects[EFX_EXCITER]->getpar(Exciter_Harm_10));
                EXCITER->ex_10->redraw();
                break;
            case MC_DistBand_DryWet:
                DISTBAND->distband_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_DryWet)));
                DISTBAND->distband_WD->redraw();
                break;
            case MC_DistBand_LR_Cross:
                DISTBAND->distband_LRc->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_LR_Cross));
                DISTBAND->distband_LRc->redraw();
                break;
            case MC_DistBand_Drive:
                DISTBAND->distband_drive->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Drive));
                DISTBAND->distband_drive->redraw();
                break;
            case MC_DistBand_Level:
                DISTBAND->distband_level->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Level));
                DISTBAND->distband_level->redraw();
                break;
            case MC_DistBand_Gain_Low:
                DISTBAND->distband_volL->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Gain_Low));
                DISTBAND->distband_volL->redraw();
                break;
            case MC_DistBand_Gain_Mid:
                DISTBAND->distband_volM->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Gain_Mid));
                DISTBAND->distband_volM->redraw();
                break;
            case MC_DistBand_Gain_Hi:
                DISTBAND->distband_volH->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Gain_Hi));
                DISTBAND->distband_volH->redraw();
                break;
            case MC_DistBand_Cross_1:
                DISTBAND->distband_cross1->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Cross_1));
                DISTBAND->distband_cross1->redraw();
                break;
            case MC_DistBand_Cross_2:
                DISTBAND->distband_cross2->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Cross_2));
                DISTBAND->distband_cross2->redraw();
                break;
            case MC_DistBand_Pan:
                DISTBAND->distband_pan->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Pan) - 64);
                DISTBAND->distband_pan->redraw();
                break;
            case MC_Arpie_DryWet:
                ARPIE->arpie_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_ARPIE]->getpar(Arpie_DryWet)));
                ARPIE->arpie_WD->redraw();
                break;
            case MC_Arpie_ArpeWD:
                ARPIE->arpie_arpe->value(rkr->Rack_Effects[EFX_ARPIE]->getpar(Arpie_ArpeWD));
                ARPIE->arpie_arpe->redraw();
                break;
            case MC_Arpie_Pan:
                ARPIE->arpie_pan->value(rkr->Rack_Effects[EFX_ARPIE]->getpar(Arpie_Pan) - 64);
                ARPIE->arpie_pan->redraw();
                break;
            case MC_Arpie_Tempo:
                ARPIE->arpie_delay->value(rkr->Rack_Effects[EFX_ARPIE]->getpar(Arpie_Tempo));
                ARPIE->arpie_delay->redraw();
                break;
            case MC_Arpie_LR_Delay:
                ARPIE->arpie_LRdl->value(rkr->Rack_Effects[EFX_ARPIE]->getpar(Arpie_LR_Delay));
                ARPIE->arpie_LRdl->redraw();
                break;
            case MC_Arpie_LR_Cross:
                ARPIE->arpie_LRc->value(rkr->Rack_Effects[EFX_ARPIE]->getpar(Arpie_LR_Cross));
                ARPIE->arpie_LRc->redraw();
                break;
            case MC_Arpie_Feedback:
                ARPIE->arpie_fb->value(rkr->Rack_Effects[EFX_ARPIE]->getpar(Arpie_Feedback));
                ARPIE->arpie_fb->redraw();
                break;
            case MC_Arpie_Damp:
                ARPIE->arpie_damp->value(rkr->Rack_Effects[EFX_ARPIE]->getpar(Arpie_Damp));
                ARPIE->arpie_damp->redraw();
                break;
            case MC_Expander_Attack:
                EXPANDER->expander_ATime->value(rkr->Rack_Effects[EFX_EXPANDER]->getpar(Expander_Attack));
                EXPANDER->expander_ATime->redraw();
                break;
            case MC_Expander_Release:
                EXPANDER->expander_RTime->value(rkr->Rack_Effects[EFX_EXPANDER]->getpar(Expander_Release));
                EXPANDER->expander_RTime->redraw();
                break;
            case MC_Expander_Shape:
                EXPANDER->expander_shape->value(rkr->Rack_Effects[EFX_EXPANDER]->getpar(Expander_Shape));
                EXPANDER->expander_shape->redraw();
                break;
            case MC_Expander_Threshold:
                EXPANDER->expander_threshold->value(rkr->Rack_Effects[EFX_EXPANDER]->getpar(Expander_Threshold));
                EXPANDER->expander_threshold->redraw();
                break;
            case MC_Expander_Gain:
                EXPANDER->expander_level->value(rkr->Rack_Effects[EFX_EXPANDER]->getpar(Expander_Gain));
                EXPANDER->expander_level->redraw();
                break;
            case MC_Expander_LPF:
                EXPANDER->expander_LPF->value(rkr->Rack_Effects[EFX_EXPANDER]->getpar(Expander_LPF));
                EXPANDER->expander_LPF->redraw();
                break;
            case MC_Expander_HPF:
                EXPANDER->expander_HPF->value(rkr->Rack_Effects[EFX_EXPANDER]->getpar(Expander_HPF));
                EXPANDER->expander_HPF->redraw();
                break;
            case MC_Shuffle_DryWet:
                SHUFFLE->shuffle_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_DryWet)));
                SHUFFLE->shuffle_WD->redraw();
                break;
            case MC_Shuffle_Freq_L:
                SHUFFLE->shuffle_cross1->value(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_Freq_L));
                SHUFFLE->shuffle_cross1->redraw();
                break;
            case MC_Shuffle_Gain_L:
                SHUFFLE->shuffle_volL->value(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_Gain_L));
                SHUFFLE->shuffle_volL->redraw();
                break;
            case MC_Shuffle_Freq_ML:
                SHUFFLE->shuffle_cross2->value(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_Freq_ML));
                SHUFFLE->shuffle_cross2->redraw();
                break;
            case MC_Shuffle_Gain_ML:
                SHUFFLE->shuffle_volML->value(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_Gain_ML));
                SHUFFLE->shuffle_volML->redraw();
                break;
            case MC_Shuffle_Freq_MH:
                SHUFFLE->shuffle_cross3->value(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_Freq_MH));
                SHUFFLE->shuffle_cross3->redraw();
                break;
            case MC_Shuffle_Gain_MH:
                SHUFFLE->shuffle_volMH->value(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_Gain_MH));
                SHUFFLE->shuffle_volMH->redraw();
                break;
            case MC_Shuffle_Freq_H:
                SHUFFLE->shuffle_cross4->value(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_Freq_H));
                SHUFFLE->shuffle_cross4->redraw();
                break;
            case MC_Shuffle_Gain_H:
                SHUFFLE->shuffle_volH->value(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_Gain_H));
                SHUFFLE->shuffle_volH->redraw();
                break;
            case MC_Shuffle_Width:
                SHUFFLE->shuffle_Q->value(rkr->Rack_Effects[EFX_SHUFFLE]->getpar(Shuffle_Width));
                SHUFFLE->shuffle_Q->redraw();
                break;
            case MC_Synthfilter_DryWet:
                SYNTHFILTER->synthfilter_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_DryWet)));
                SYNTHFILTER->synthfilter_WD->redraw();
                break;
            case MC_Synthfilter_Distort:
                SYNTHFILTER->synthfilter_Distort->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_Distort));
                SYNTHFILTER->synthfilter_Distort->redraw();
                break;
            case MC_Synthfilter_LFO_Tempo:
                SYNTHFILTER->synthfilter_freq->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_LFO_Tempo));
                SYNTHFILTER->synthfilter_freq->redraw();
                break;
            case MC_Synthfilter_LFO_Stereo:
                SYNTHFILTER->synthfilter_stdf->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_LFO_Stereo));
                SYNTHFILTER->synthfilter_stdf->redraw();
                break;
            case MC_Synthfilter_Width:
                SYNTHFILTER->synthfilter_width->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_Width));
                SYNTHFILTER->synthfilter_width->redraw();
                break;
            case MC_Synthfilter_Feedback:
                SYNTHFILTER->synthfilter_fb->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_Feedback));
                SYNTHFILTER->synthfilter_fb->redraw();
                break;
            case MC_Synthfilter_Depth:
                SYNTHFILTER->synthfilter_dpth->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_Depth));
                SYNTHFILTER->synthfilter_dpth->redraw();
                break;
            case MC_Synthfilter_Env_Sens:
                SYNTHFILTER->synthfilter_EnvSens->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_Env_Sens));
                SYNTHFILTER->synthfilter_EnvSens->redraw();
                break;
            case MC_Synthfilter_Attack:
                SYNTHFILTER->synthfilter_ATime->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_Attack));
                SYNTHFILTER->synthfilter_ATime->redraw();
                break;
            case MC_Synthfilter_Release:
                SYNTHFILTER->synthfilter_RTime->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_Release));
                SYNTHFILTER->synthfilter_RTime->redraw();
                break;
            case MC_Synthfilter_Offset:
                SYNTHFILTER->synthfilter_Offset->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_Offset));
                SYNTHFILTER->synthfilter_Offset->redraw();
                break;
            case MC_VaryBand_DryWet:
                VARYBAND->varyband_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_DryWet)));
                VARYBAND->varyband_WD->redraw();
                break;
            case MC_VaryBand_LFO_Tempo_1:
                VARYBAND->varyband_freq1->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_LFO_Tempo_1));
                VARYBAND->varyband_freq1->redraw();
                break;
            case MC_VaryBand_LFO_Stereo_1:
                VARYBAND->varyband_stdf1->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_LFO_Stereo_1));
                VARYBAND->varyband_stdf1->redraw();
                break;
            case MC_VaryBand_LFO_Tempo_2:
                VARYBAND->varyband_freq2->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_LFO_Tempo_2));
                VARYBAND->varyband_freq2->redraw();
                break;
            case MC_VaryBand_LFO_Stereo_2:
                VARYBAND->varyband_stdf2->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_LFO_Stereo_2));
                VARYBAND->varyband_stdf2->redraw();
                break;
            case MC_VaryBand_Cross_1:
                VARYBAND->varyband_cross1->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_Cross_1));
                VARYBAND->varyband_cross1->redraw();
                break;
            case MC_VaryBand_Cross_2:
                VARYBAND->varyband_cross2->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_Cross_2));
                VARYBAND->varyband_cross2->redraw();
                break;
            case MC_VaryBand_Cross_3:
                VARYBAND->varyband_cross3->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_Cross_3));
                VARYBAND->varyband_cross3->redraw();
                break;
            case MC_MuTro_DryWet:
                MUTROMOJO->mutromojo_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_DryWet)));
                MUTROMOJO->mutromojo_WD->redraw();
                break;
            case MC_MuTro_LowPass:
                MUTROMOJO->mutromojo_lp->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_LowPass));
                MUTROMOJO->mutromojo_lp->redraw();
                break;
            case MC_MuTro_BandPass:
                MUTROMOJO->mutromojo_bp->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_BandPass));
                MUTROMOJO->mutromojo_bp->redraw();
                break;
            case MC_MuTro_HighPass:
                MUTROMOJO->mutromojo_hp->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_HighPass));
                MUTROMOJO->mutromojo_hp->redraw();
                break;
            case MC_MuTro_Depth:
                MUTROMOJO->mutromojo_dpth->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_Depth));
                MUTROMOJO->mutromojo_dpth->redraw();
                break;
            case MC_MuTro_LFO_Tempo:
                MUTROMOJO->mutromojo_freq->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_LFO_Tempo));
                MUTROMOJO->mutromojo_freq->redraw();
                break;
            case MC_MuTro_Resonance:
                MUTROMOJO->mutromojo_q->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_Resonance));
                MUTROMOJO->mutromojo_q->redraw();
                break;
            case MC_MuTro_Range:
                MUTROMOJO->mutromojo_rng->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_Range));
                MUTROMOJO->mutromojo_rng->redraw();
                break;
            case MC_MuTro_Wah:
                MUTROMOJO->mutromojo_ampsnsinv->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_Wah));
                MUTROMOJO->mutromojo_ampsnsinv->redraw();
                break;
            case MC_MuTro_Env_Sens:
                MUTROMOJO->mutromojo_ampsns->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_Env_Sens));
                MUTROMOJO->mutromojo_ampsns->redraw();
                break;
            case MC_MuTro_Env_Smooth:
                MUTROMOJO->mutromojo_smooth->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_Env_Smooth));
                MUTROMOJO->mutromojo_smooth->redraw();
                break;
            case MC_Looper_DryWet:
                LOOPER->looper_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_DryWet)));
                LOOPER->looper_WD->redraw();
                break;
            case MC_Looper_Level_1:
                LOOPER->looper_level1->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Level_1));
                LOOPER->looper_level1->redraw();
                break;
            case MC_Looper_Level_2:
                LOOPER->looper_level2->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Level_2));
                LOOPER->looper_level2->redraw();
                break;
            case MC_Looper_Reverse:
                LOOPER->looper_rv->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Reverse));
                LOOPER->looper_rv->redraw();
                break;
            case MC_Looper_AutoPlay:
                LOOPER->looper_ap->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_AutoPlay));
                LOOPER->looper_ap->redraw();
                break;
            case MC_Looper_Play:
                LOOPER->looper_play->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Play));
                update_looper();
                break;
            case MC_Looper_Stop:
                LOOPER->looper_stop->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Stop));
                update_looper();
                break;
            case MC_Looper_Record:
                LOOPER->looper_record->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Record));
                update_looper();
                break;
            case MC_Looper_Rec_1:
                LOOPER->looper_r1->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Rec_1));
                LOOPER->looper_r1->redraw();
                break;
            case MC_Looper_Rec_2:
                LOOPER->looper_r2->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Rec_2));
                LOOPER->looper_r2->redraw();
                break;
            case MC_Looper_Track_1:
                LOOPER->looper_t1->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Track_1));
                update_looper();
                break;
            case MC_Looper_Track_2:
                LOOPER->looper_t2->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Track_2));
                update_looper();
                break;
            case MC_Looper_Clear:
                /* Setting the looper_clear button after MIDI control really does not work. 
                   The button for the gui is just press and release, i.e. not a toggle button.
                   Since the clear flag Pclear is set and remains set until another button such
                   as play or record gets pushed, the Pclear variable returned by getpar(4) is
                   going to show pressed (value = 1) and set the button as pressed here. And it
                   will remain in that state until restart. If pressed manually, it just does
                   unpress and release back to press.  Would be nice to show the press and release
                   with MIDI control, but it would probably have to be a timer issue and additional
                   complications for very little value. MIDI control does work, but we should not show
                   it on the gui button here with this method */
            //    LOOPER->looper_clear->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Clear));
                update_looper();
                break;
            case MC_Convo_DryWet:
                CONVOLOTRON->convo_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_CONVOLOTRON]->getpar(Convo_DryWet)));
                CONVOLOTRON->convo_WD->redraw();
                break;
            case MC_Convo_Pan:
                CONVOLOTRON->convo_pan->value(rkr->Rack_Effects[EFX_CONVOLOTRON]->getpar(Convo_Pan) - 64);
                CONVOLOTRON->convo_pan->redraw();
                break;
            case MC_Convo_Level:
                CONVOLOTRON->convo_level->value(rkr->Rack_Effects[EFX_CONVOLOTRON]->getpar(Convo_Level));
                CONVOLOTRON->convo_level->redraw();
                break;
            case MC_Convo_Damp:
                CONVOLOTRON->convo_damp->value(rkr->Rack_Effects[EFX_CONVOLOTRON]->getpar(Convo_Damp));
                CONVOLOTRON->convo_damp->redraw();
                break;
            case MC_Convo_Feedback:
                CONVOLOTRON->convo_fb->value(rkr->Rack_Effects[EFX_CONVOLOTRON]->getpar(Convo_Feedback));
                CONVOLOTRON->convo_fb->redraw();
                break;
            case MC_Convo_Length:
                CONVOLOTRON->convo_length->value(rkr->Rack_Effects[EFX_CONVOLOTRON]->getpar(Convo_Length));
                CONVOLOTRON->convo_length->redraw();
                break;
            case MC_Coil_Gain:
                COILCRAFTER->coil_WD->value(rkr->Rack_Effects[EFX_COILCRAFTER]->getpar(Coil_Gain));
                COILCRAFTER->coil_WD->redraw();
                break;
            case MC_Coil_Tone:
                COILCRAFTER->coil_tone->value(rkr->Rack_Effects[EFX_COILCRAFTER]->getpar(Coil_Tone));
                COILCRAFTER->coil_tone->redraw();
                break;
            case MC_Coil_Freq_1:
                COILCRAFTER->coil_freq1->value(rkr->Rack_Effects[EFX_COILCRAFTER]->getpar(Coil_Freq_1));
                COILCRAFTER->coil_freq1->redraw();
                break;
            case MC_Coil_Q_1:
                COILCRAFTER->coil_q1->value(rkr->Rack_Effects[EFX_COILCRAFTER]->getpar(Coil_Q_1));
                COILCRAFTER->coil_q1->redraw();
                break;
            case MC_Coil_Freq_2:
                COILCRAFTER->coil_freq2->value(rkr->Rack_Effects[EFX_COILCRAFTER]->getpar(Coil_Freq_2));
                COILCRAFTER->coil_freq2->redraw();
                break;
            case MC_Coil_Q_2:
                COILCRAFTER->coil_q2->value(rkr->Rack_Effects[EFX_COILCRAFTER]->getpar(Coil_Q_2));
                COILCRAFTER->coil_q2->redraw();
                break;
            case MC_Shelf_Gain:
                SHELFBOOST->shelf_gain->value(rkr->Rack_Effects[EFX_SHELFBOOST]->getpar(Shelf_Gain));
                SHELFBOOST->shelf_gain->redraw();
                break;
            case MC_Shelf_Level:
                SHELFBOOST->shelf_level->value(rkr->Rack_Effects[EFX_SHELFBOOST]->getpar(Shelf_Level));
                SHELFBOOST->shelf_level->redraw();
                break;
            case MC_Shelf_Tone:
                SHELFBOOST->shelf_freq1->value(rkr->Rack_Effects[EFX_SHELFBOOST]->getpar(Shelf_Tone));
                SHELFBOOST->shelf_freq1->redraw();
                break;
            case MC_Shelf_Presence:
                SHELFBOOST->shelf_q1->value(rkr->Rack_Effects[EFX_SHELFBOOST]->getpar(Shelf_Presence));
                SHELFBOOST->shelf_q1->redraw();
                break;
            case MC_Vocoder_DryWet:
                VOCODER->vo_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_VOCODER]->getpar(Vocoder_DryWet)));
                VOCODER->vo_WD->redraw();
                break;
            case MC_Vocoder_Pan:
                VOCODER->vo_pan->value(rkr->Rack_Effects[EFX_VOCODER]->getpar(Vocoder_Pan) - 64);
                VOCODER->vo_pan->redraw();
                break;
            case MC_Vocoder_Input:
                VOCODER->vo_input->value(rkr->Rack_Effects[EFX_VOCODER]->getpar(Vocoder_Input));
                VOCODER->vo_input->redraw();
                break;
            case MC_Vocoder_Smear:
                VOCODER->vo_mu->value(rkr->Rack_Effects[EFX_VOCODER]->getpar(Vocoder_Smear));
                VOCODER->vo_mu->redraw();
                break;
            case MC_Vocoder_Q:
                VOCODER->vo_q->value(rkr->Rack_Effects[EFX_VOCODER]->getpar(Vocoder_Q));
                VOCODER->vo_q->redraw();
                break;
            case MC_Vocoder_Ring:
                VOCODER->vo_ring->value(rkr->Rack_Effects[EFX_VOCODER]->getpar(Vocoder_Ring));
                VOCODER->vo_ring->redraw();
                break;
            case MC_Vocoder_Level:
                VOCODER->vo_level->value(rkr->Rack_Effects[EFX_VOCODER]->getpar(Vocoder_Level));
                VOCODER->vo_level->redraw();
                break;
            case MC_Echoverse_DryWet:
                ECHOVERSE->echoverse_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_DryWet)));
                ECHOVERSE->echoverse_WD->redraw();
                break;
            case MC_Echoverse_Reverse:
                ECHOVERSE->echoverse_RV->value(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_Reverse));
                ECHOVERSE->echoverse_RV->redraw();
                break;
            case MC_Echoverse_Pan:
                ECHOVERSE->echoverse_pan->value(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_Pan) - 64);
                ECHOVERSE->echoverse_pan->redraw();
                break;
            case MC_Echoverse_Tempo:
                ECHOVERSE->echoverse_delay->value(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_Tempo));
                ECHOVERSE->echoverse_delay->redraw();
                break;
            case MC_Echoverse_LR_Delay:
                ECHOVERSE->echoverse_LRdl->value(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_LR_Delay));
                ECHOVERSE->echoverse_LRdl->redraw();
                break;
            case MC_Echoverse_Feedback:
                ECHOVERSE->echoverse_fb->value(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_Feedback));
                ECHOVERSE->echoverse_fb->redraw();
                break;
            case MC_Echoverse_Damp:
                ECHOVERSE->echoverse_damp->value(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_Damp));
                ECHOVERSE->echoverse_damp->redraw();
                break;
            case MC_Echoverse_Ext_Stereo:
                ECHOVERSE->echoverse_es->value(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_Ext_Stereo));
                ECHOVERSE->echoverse_es->redraw();
                break;
            case MC_Echoverse_Angle:
                ECHOVERSE->echoverse_angle->value(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_Angle) - 64);
                ECHOVERSE->echoverse_angle->redraw();
                break;
            case MC_Sustain_Gain:
                SUSTAINER->sus_gain->value(rkr->Rack_Effects[EFX_SUSTAINER]->getpar(Sustain_Gain));
                SUSTAINER->sus_gain->redraw();
                break;
            case MC_Sustain_Sustain:
                SUSTAINER->sus_sus->value(rkr->Rack_Effects[EFX_SUSTAINER]->getpar(Sustain_Sustain));
                SUSTAINER->sus_sus->redraw();
                break;
            case MC_Sequence_DryWet:
                SEQUENCE->seq_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_DryWet)));
                SEQUENCE->seq_WD->redraw();
                break;
            case MC_Sequence_Step_1:
                SEQUENCE->seq_1->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Step_1));
                SEQUENCE->seq_1->redraw();
                break;
            case MC_Sequence_Step_2:
                SEQUENCE->seq_2->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Step_2));
                SEQUENCE->seq_2->redraw();
                break;
            case MC_Sequence_Step_3:
                SEQUENCE->seq_3->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Step_3));
                SEQUENCE->seq_3->redraw();
                break;
            case MC_Sequence_Step_4:
                SEQUENCE->seq_4->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Step_4));
                SEQUENCE->seq_4->redraw();
                break;
            case MC_Sequence_Step_5:
                SEQUENCE->seq_5->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Step_5));
                SEQUENCE->seq_5->redraw();
                break;
            case MC_Sequence_Step_6:
                SEQUENCE->seq_6->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Step_6));
                SEQUENCE->seq_6->redraw();
                break;
            case MC_Sequence_Step_7:
                SEQUENCE->seq_7->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Step_7));
                SEQUENCE->seq_7->redraw();
                break;
            case MC_Sequence_Step_8:
                SEQUENCE->seq_8->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Step_8));
                SEQUENCE->seq_8->redraw();
                break;
            case MC_Sequence_Tempo:
                SEQUENCE->seq_tempo->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Tempo));
                SEQUENCE->seq_tempo->redraw();
                break;
            case MC_Sequence_Resonance:
                SEQUENCE->seq_q->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Resonance) - 64);
                SEQUENCE->seq_q->redraw();
                break;
            case MC_Sequence_Stdf:
                SEQUENCE->seq_stdf->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Stdf));
                SEQUENCE->seq_stdf->redraw();
                break;
            case MC_Shifter_DryWet:
                SHIFTER->shifter_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_SHIFTER]->getpar(Shifter_DryWet)));
                SHIFTER->shifter_WD->redraw();
                break;
            case MC_Shifter_Interval:
                SHIFTER->shifter_int->value(rkr->Rack_Effects[EFX_SHIFTER]->getpar(Shifter_Interval));
                SHIFTER->shifter_int->redraw();
                break;
            case MC_Shifter_Gain:
                SHIFTER->shifter_gain->value(rkr->Rack_Effects[EFX_SHIFTER]->getpar(Shifter_Gain) - 64);
                SHIFTER->shifter_gain->redraw();
                break;
            case MC_Shifter_Pan:
                SHIFTER->shifter_pan->value(rkr->Rack_Effects[EFX_SHIFTER]->getpar(Shifter_Pan) - 64);
                SHIFTER->shifter_pan->redraw();
                break;
            case MC_Shifter_Attack:
                SHIFTER->shifter_attack->value(rkr->Rack_Effects[EFX_SHIFTER]->getpar(Shifter_Attack));
                SHIFTER->shifter_attack->redraw();
                break;
            case MC_Shifter_Decay:
                SHIFTER->shifter_decay->value(rkr->Rack_Effects[EFX_SHIFTER]->getpar(Shifter_Decay));
                SHIFTER->shifter_decay->redraw();
                break;
            case MC_Shifter_Threshold:
                SHIFTER->shifter_thre->value(rkr->Rack_Effects[EFX_SHIFTER]->getpar(Shifter_Threshold));
                SHIFTER->shifter_thre->redraw();
                break;
            case MC_Shifter_Whammy:
                SHIFTER->shifter_whammy->value(rkr->Rack_Effects[EFX_SHIFTER]->getpar(Shifter_Whammy));
                SHIFTER->shifter_whammy->redraw();
                break;
            case MC_Stomp_Level:
                STOMPBOX->stomp_WD->value(rkr->Rack_Effects[EFX_STOMPBOX]->getpar(Stomp_Level));
                STOMPBOX->stomp_WD->redraw();
                break;
            case MC_Stomp_Gain:
                STOMPBOX->stomp_gain->value(rkr->Rack_Effects[EFX_STOMPBOX]->getpar(Stomp_Gain));
                STOMPBOX->stomp_gain->redraw();
                break;
            case MC_Stomp_Bias:
                STOMPBOX->stomp_low->value(rkr->Rack_Effects[EFX_STOMPBOX]->getpar(Stomp_Bias));
                STOMPBOX->stomp_low->redraw();
                break;
            case MC_Stomp_Mid:
                STOMPBOX->stomp_mid->value(rkr->Rack_Effects[EFX_STOMPBOX]->getpar(Stomp_Mid));
                STOMPBOX->stomp_mid->redraw();
                break;
            case MC_Stomp_Tone:
                STOMPBOX->stomp_high->value(rkr->Rack_Effects[EFX_STOMPBOX]->getpar(Stomp_Tone));
                STOMPBOX->stomp_high->redraw();
                break;
            case MC_Revtron_DryWet:
                REVERBTRON->revtron_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_DryWet)));
                REVERBTRON->revtron_WD->redraw();
                break;
            case MC_Revtron_Pan:
                REVERBTRON->revtron_pan->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_Pan) - 64);
                REVERBTRON->revtron_pan->redraw();
                break;
            case MC_Revtron_Level:
                REVERBTRON->revtron_level->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_Level));
                REVERBTRON->revtron_level->redraw();
                break;
            case MC_Revtron_Damp:
                REVERBTRON->revtron_damp->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_Damp));
                REVERBTRON->revtron_damp->redraw();
                break;
            case MC_Revtron_Feedback:
                REVERBTRON->revtron_fb->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_Feedback));
                REVERBTRON->revtron_fb->redraw();
                break;
            case MC_Revtron_Length:
                REVERBTRON->revtron_length->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_Length));
                REVERBTRON->revtron_length->redraw();
                break;
            case MC_Revtron_Stretch:
                REVERBTRON->revtron_strech->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_Stretch));
                REVERBTRON->revtron_strech->redraw();
                break;
            case MC_Revtron_I_Delay:
                REVERBTRON->revtron_idelay->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_I_Delay));
                REVERBTRON->revtron_idelay->redraw();
                break;
            case MC_Revtron_Fade:
                REVERBTRON->revtron_fade->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_Fade));
                REVERBTRON->revtron_fade->redraw();
                break;
            case MC_Echotron_DryWet:
                ECHOTRON->echotron_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_DryWet)));
                ECHOTRON->echotron_WD->redraw();
                break;
            case MC_Echotron_Pan:
                ECHOTRON->echotron_pan->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_Pan) - 64);
                ECHOTRON->echotron_pan->redraw();
                break;
            case MC_Echotron_Tempo:
                ECHOTRON->echotron_tempo->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_Tempo));
                ECHOTRON->echotron_tempo->redraw();
                break;
            case MC_Echotron_Damp:
                ECHOTRON->echotron_damp->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_Damp));
                ECHOTRON->echotron_damp->redraw();
                break;
            case MC_Echotron_Feedback:
                ECHOTRON->echotron_fb->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_Feedback));
                ECHOTRON->echotron_fb->redraw();
                break;
            case MC_Echotron_LR_Cross:
                ECHOTRON->echotron_lrcross->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_LR_Cross) - 64);
                ECHOTRON->echotron_lrcross->redraw();
                break;
            case MC_Echotron_LFO_Width:
                ECHOTRON->echotron_width->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_LFO_Width));
                ECHOTRON->echotron_width->redraw();
                break;
            case MC_Echotron_Depth:
                ECHOTRON->echotron_deep->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_Depth) - 64);
                ECHOTRON->echotron_deep->redraw();
                break;
            case MC_Echotron_LFO_Stereo:
                ECHOTRON->echotron_stdf->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_LFO_Stereo));
                ECHOTRON->echotron_stdf->redraw();
                break;
            case MC_Echotron_Taps:
                ECHOTRON->echotron_length->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_Taps));
                ECHOTRON->echotron_length->redraw();
                break;
            case MC_Sharm_DryWet:
                SHAR->shar_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_DryWet)));
                SHAR->shar_WD->redraw();
                break;
            case MC_Sharm_L_Interval:
                SHAR->shar_intl->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_L_Interval) - 12);
                SHAR->shar_intl->redraw();
                break;
            case MC_Sharm_L_Chroma:
                SHAR->shar_chl->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_L_Chroma));
                SHAR->shar_chl->redraw();
                break;
            case MC_Sharm_L_Gain:
                SHAR->shar_ganl->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_L_Gain) - 64);
                SHAR->shar_ganl->redraw();
                break;
            case MC_Sharm_R_Interval:
                SHAR->shar_intr->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_R_Interval) - 12);
                SHAR->shar_intr->redraw();
                break;
            case MC_Sharm_R_Chroma:
                SHAR->shar_chr->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_R_Chroma));
                SHAR->shar_chr->redraw();
                break;
            case MC_Sharm_R_Gain:
                SHAR->shar_ganr->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_R_Gain) - 64);
                SHAR->shar_ganr->redraw();
                break;
            case MC_Sharm_LR_Cross:
                SHAR->shar_lrc->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_LR_Cross));
                SHAR->shar_lrc->redraw();
                break;
            case MC_Sharm_Note:
                SHAR->shar_note->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_Note));
                SHAR->shar_note->redraw();
                Chord(1);   // update gui
                break;
            case MC_Sharm_Chord:
                SHAR->shar_type->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_Chord));
                SHAR->shar_type->redraw();
                Chord(1);   // update gui
                break;
            case MC_CompBand_DryWet:
                COMPBAND->cband_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_DryWet)));
                COMPBAND->cband_WD->redraw();
                break;
            case MC_CompBand_Gain:
                COMPBAND->cband_gain->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Gain));
                COMPBAND->cband_gain->redraw();
                break;
            case MC_CompBand_Low_Ratio:
                COMPBAND->cband_Lratio->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Low_Ratio));
                COMPBAND->cband_Lratio->redraw();
                break;
            case MC_CompBand_Mid_1_Ratio:
                COMPBAND->cband_MLratio->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Mid_1_Ratio));
                COMPBAND->cband_MLratio->redraw();
                break;
            case MC_CompBand_Mid_2_Ratio:
                COMPBAND->cband_MHratio->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Mid_2_Ratio));
                COMPBAND->cband_MHratio->redraw();
                break;
            case MC_CompBand_High_Ratio:
                COMPBAND->cband_Hratio->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_High_Ratio));
                COMPBAND->cband_Hratio->redraw();
                break;
            case MC_CompBand_Low_Thresh:
                COMPBAND->cband_Lthres->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Low_Thresh));
                COMPBAND->cband_Lthres->redraw();
                break;
            case MC_CompBand_Mid_1_Thresh:
                COMPBAND->cband_MLthres->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Mid_1_Thresh));
                COMPBAND->cband_MLthres->redraw();
                break;
            case MC_CompBand_Mid_2_Thresh:
                COMPBAND->cband_MHthres->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Mid_2_Thresh));
                COMPBAND->cband_MHthres->redraw();
                break;
            case MC_CompBand_High_Thresh:
                COMPBAND->cband_Hthres->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_High_Thresh));
                COMPBAND->cband_Hthres->redraw();
                break;
            case MC_CompBand_Cross_1:
                COMPBAND->cband_cross1->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Cross_1));
                COMPBAND->cband_cross1->redraw();
                break;
            case MC_CompBand_Cross_2:
                COMPBAND->cband_cross2->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Cross_2));
                COMPBAND->cband_cross2->redraw();
                break;
            case MC_CompBand_Cross_3:
                COMPBAND->cband_cross3->value(rkr->Rack_Effects[EFX_COMPBAND]->getpar(CompBand_Cross_3));
                COMPBAND->cband_cross3->redraw();
                break;
            case MC_Optical_Depth:
                OTREM->otrem_dpth->value(rkr->Rack_Effects[EFX_OPTICALTREM]->getpar(Optical_Depth));
                OTREM->otrem_dpth->redraw();
                break;
            case MC_Optical_LFO_Tempo:
                OTREM->otrem_freq->value(rkr->Rack_Effects[EFX_OPTICALTREM]->getpar(Optical_LFO_Tempo));
                OTREM->otrem_freq->redraw();
                break;
            case MC_Optical_LFO_Random:
                OTREM->otrem_rnd->value(rkr->Rack_Effects[EFX_OPTICALTREM]->getpar(Optical_LFO_Random));
                OTREM->otrem_rnd->redraw();
                break;
            case MC_Optical_LFO_Stereo:
                OTREM->otrem_stdf->value(rkr->Rack_Effects[EFX_OPTICALTREM]->getpar(Optical_LFO_Stereo));
                OTREM->otrem_stdf->redraw();
                break;
            case MC_Optical_Pan:
                OTREM->otrem_pan->value(rkr->Rack_Effects[EFX_OPTICALTREM]->getpar(Optical_Pan) - 64);
                OTREM->otrem_pan->redraw();
                break;
            case MC_Vibe_DryWet:
                VIBE->vibe_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_DryWet)));
                VIBE->vibe_WD->redraw();
                break;
            case MC_Vibe_Width:
                VIBE->vibe_width->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_Width));
                VIBE->vibe_width->redraw();
                break;
            case MC_Vibe_Depth:
                VIBE->vibe_dpth->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_Depth));
                VIBE->vibe_dpth->redraw();
                break;
            case MC_Vibe_LFO_Tempo:
                VIBE->vibe_freq->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LFO_Tempo));
                VIBE->vibe_freq->redraw();
                break;
            case MC_Vibe_LFO_Random:
                VIBE->vibe_rnd->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LFO_Random));
                VIBE->vibe_rnd->redraw();
                break;
            case MC_Vibe_LFO_Stereo:
                VIBE->vibe_stdf->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LFO_Stereo));
                VIBE->vibe_stdf->redraw();
                break;
            case MC_Vibe_Feedback:
                VIBE->vibe_fb->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_Feedback) - 64);
                VIBE->vibe_fb->redraw();
                break;
            case MC_Vibe_LR_Cross:
                VIBE->vibe_LR->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LR_Cross));
                VIBE->vibe_LR->redraw();
                break;
            case MC_Vibe_Pan:
                VIBE->vibe_pan->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_Pan) - 64);
                VIBE->vibe_pan->redraw();
                break;
            case MC_Infinity_DryWet:
                INFINIT->infinity_WD->value(Dry_Wet(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_DryWet)));
                INFINIT->infinity_WD->redraw();
                break;
            case MC_Infinity_Resonance:
                INFINIT->infinity_Q->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Resonance));
                INFINIT->infinity_Q->redraw();
                break;
            case MC_Infinity_AutoPan:
                INFINIT->infinity_pan->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_AutoPan));
                INFINIT->infinity_pan->redraw();
                break;
            case MC_Infinity_LR_Delay:
                INFINIT->infinity_stdf->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_LR_Delay));
                INFINIT->infinity_stdf->redraw();
                break;
            case MC_Infinity_Start:
                INFINIT->infinity_start->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Start));
                INFINIT->infinity_start->redraw();
                break;
            case MC_Infinity_End:
                INFINIT->infinity_end->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_End));
                INFINIT->infinity_end->redraw();
                break;
            case MC_Infinity_Tempo:
                INFINIT->infinity_rate->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Tempo));
                INFINIT->infinity_rate->redraw();
                break;
            case MC_Infinity_Subdivision:
                INFINIT->infinity_subdiv->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Subdivision));
                INFINIT->infinity_subdiv->redraw();
                break;
            case MC_Alien_Delay:
                ALIENWAH->Alienwah_delay->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_Delay));
                ALIENWAH->Alienwah_delay->redraw();
                break;
            case MC_APhase_LFO_Random:
                APHASER->aphaser_rnd->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_LFO_Random));
                APHASER->aphaser_rnd->redraw();
                break;
            case MC_Cabinet_Gain:
                CABINET->Cabinet_output->value(rkr->Rack_Effects[EFX_CABINET]->getpar(Cabinet_Gain)-64);
                CABINET->Cabinet_output->redraw();
                break;
            case MC_Chorus_Delay:
                CHORUS->chorus_delay->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_Delay));
                CHORUS->chorus_delay->redraw();
                break;
            case MC_Echo_Reverse:
                ECHO->echo_RV->value(rkr->Rack_Effects[EFX_ECHO]->getpar(Echo_Reverse));
                ECHO->echo_RV->redraw();
                break;
            case MC_Echo_Delay:
                ECHO->echo_delay->value(rkr->Rack_Effects[EFX_ECHO]->getpar(Echo_Delay));
                ECHO->echo_delay->redraw();
                break;
            case MC_Echo_LR_Delay:
                ECHO->echo_LRdl->value(rkr->Rack_Effects[EFX_ECHO]->getpar(Echo_LR_Delay));
                ECHO->echo_LRdl->redraw();
                break;
            case MC_Echo_Damp:
                ECHO->echo_damp->value(rkr->Rack_Effects[EFX_ECHO]->getpar(Echo_Damp));
                ECHO->echo_damp->redraw();
                break;
            case MC_Flanger_Delay:
                FLANGER->flanger_delay->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_Delay));
                FLANGER->flanger_delay->redraw();
                break;
            case MC_Harm_Gain:
                HAR->har_gan->value(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Gain) - 64);
                HAR->har_gan->redraw();
                break;
            case MC_Harm_Filter_Gain:
                HAR->har_gan1->value(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Filter_Gain) - 64);
                HAR->har_gan1->redraw();
                break;
            case MC_Harm_Filter_Q:
                HAR->har_q1->value(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Filter_Q) - 64);
                HAR->har_q1->redraw();
                break;
            case MC_Infinity_Band_1:
                INFINIT->infinity_1->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Band_1));
                INFINIT->infinity_1->redraw();
                break;
            case MC_Infinity_Band_2:
                INFINIT->infinity_2->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Band_2));
                INFINIT->infinity_2->redraw();
                break;
            case MC_Infinity_Band_3:
                INFINIT->infinity_3->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Band_3));
                INFINIT->infinity_3->redraw();
                break;
            case MC_Infinity_Band_4:
                INFINIT->infinity_4->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Band_4));
                INFINIT->infinity_4->redraw();
                break;
            case MC_Infinity_Band_5:
                INFINIT->infinity_5->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Band_5));
                INFINIT->infinity_5->redraw();
                break;
            case MC_Infinity_Band_6:
                INFINIT->infinity_6->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Band_6));
                INFINIT->infinity_6->redraw();
                break;
            case MC_Infinity_Band_7:
                INFINIT->infinity_7->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Band_7));
                INFINIT->infinity_7->redraw();
                break;
            case MC_Infinity_Band_8:
                INFINIT->infinity_8->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Band_8));
                INFINIT->infinity_8->redraw();
                break;
            case MC_Looper_Tempo:
                LOOPER->looper_Tempo->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Tempo));
                LOOPER->looper_Tempo->redraw();
                break;
            case MC_Music_Tempo:
                MUSDELAY->musdelay_tempo->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Tempo));
                MUSDELAY->musdelay_tempo->redraw();
                break;
            case MC_Music_Damp:
                MUSDELAY->musdelay_damp->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Damp));
                MUSDELAY->musdelay_damp->redraw();
                break;
            case MC_MuTro_LFO_Random:
                MUTROMOJO->mutromojo_Rnd->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_LFO_Random));
                MUTROMOJO->mutromojo_Rnd->redraw();
                break;
            case MC_MuTro_LFO_Stereo:
                MUTROMOJO->mutromojo_St->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_LFO_Stereo) - 64);
                MUTROMOJO->mutromojo_St->redraw();
                break;
            case MC_MuTro_St_Freq:
                MUTROMOJO->mutromojo_minfreq->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_St_Freq));
                MUTROMOJO->mutromojo_minfreq->redraw();
                break;
            case MC_Gate_Attack:
                GATE->gate_ATime->value(rkr->Rack_Effects[EFX_NOISEGATE]->getpar(Gate_Attack));
                GATE->gate_ATime->redraw();
                break;
            case MC_Gate_Release:
                GATE->gate_RTime->value(rkr->Rack_Effects[EFX_NOISEGATE]->getpar(Gate_Release));
                GATE->gate_RTime->redraw();
                break;
            case MC_Gate_Range:
                GATE->gate_range->value(rkr->Rack_Effects[EFX_NOISEGATE]->getpar(Gate_Range));
                GATE->gate_range->redraw();
                break;
            case MC_Gate_Threshold:
                GATE->gate_threshold->value(rkr->Rack_Effects[EFX_NOISEGATE]->getpar(Gate_Threshold));
                GATE->gate_threshold->redraw();
                break;
            case MC_Gate_Hold:
                GATE->gate_hold->value(rkr->Rack_Effects[EFX_NOISEGATE]->getpar(Gate_Hold));
                GATE->gate_hold->redraw();
                break;
            case MC_Gate_LPF:
                GATE->gate_LPF->value(rkr->Rack_Effects[EFX_NOISEGATE]->getpar(Gate_LPF));
                GATE->gate_LPF->redraw();
                break;
            case MC_Gate_HPF:
                GATE->gate_HPF->value(rkr->Rack_Effects[EFX_NOISEGATE]->getpar(Gate_HPF));
                GATE->gate_HPF->redraw();
                break;
            case MC_Pan_Ex_St_Amt:
                PAN->pan_extra->value(rkr->Rack_Effects[EFX_PAN]->getpar(Pan_Ex_St_Amt));
                PAN->pan_extra->redraw();
                break;
            case MC_Reverb_Time:
                REVERB->reverb_time->value(rkr->Rack_Effects[EFX_REVERB]->getpar(Reverb_Time));
                REVERB->reverb_time->redraw();
                break;
            case MC_Reverb_I_Delay:
                REVERB->reverb_ldel->value(rkr->Rack_Effects[EFX_REVERB]->getpar(Reverb_I_Delay));
                REVERB->reverb_ldel->redraw();
                break;
            case MC_Reverb_Delay_FB:
                REVERB->reverb_ldelft->value(rkr->Rack_Effects[EFX_REVERB]->getpar(Reverb_Delay_FB));
                REVERB->reverb_ldelft->redraw();
                break;
            case MC_Reverb_Room:
                REVERB->reverb_RS->value(rkr->Rack_Effects[EFX_REVERB]->getpar(Reverb_Room));
                REVERB->reverb_RS->redraw();
                break;
            case MC_Reverb_Damp:
                REVERB->reverb_damp->value(rkr->Rack_Effects[EFX_REVERB]->getpar(Reverb_Damp));
                REVERB->reverb_damp->redraw();
                break;
            case MC_Revtron_Diffusion:
                REVERBTRON->revtron_diff->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_Diffusion));
                REVERBTRON->revtron_diff->redraw();
                break;
            case MC_Revtron_LPF:
                REVERBTRON->revtron_LPF->value(rkr->Rack_Effects[EFX_REVERBTRON]->getpar(Revtron_LPF));
                REVERBTRON->revtron_LPF->redraw();
                break;
            case MC_Harm_Note:
                HAR->har_note->value(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Note));
                HAR->har_note->redraw();
                Chord(0);   // update gui
                break;
            case MC_Harm_Chord:
                HAR->har_type->value(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Chord));
                HAR->har_type->redraw();
                Chord(0);   // update gui
                break;
            case MC_Synthfilter_LFO_Random:
                SYNTHFILTER->synthfilter_rand->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_LFO_Random));
                SYNTHFILTER->synthfilter_rand->redraw();
                break;
            case MC_Harm_Select:
                HAR->har_SELECT->value(rkr->Rack_Effects[EFX_HARMONIZER]->getpar(Harm_Select));
                HAR->har_SELECT->redraw();
                break;
            case MC_Sharm_Select:
                SHAR->shar_SELECT->value(rkr->Rack_Effects[EFX_STEREOHARM]->getpar(Sharm_Select));
                SHAR->shar_SELECT->redraw();
                break;
            case MC_Dist_Type:
                DIST->dist_tipo->value(rkr->Rack_Effects[EFX_DISTORTION]->getpar(Dist_Type));
                DIST->dist_tipo->redraw();
                break;
            case MC_Overdrive_Type:
                OVRD->ovrd_tipo->value(rkr->Rack_Effects[EFX_OVERDRIVE]->getpar(Overdrive_Type));
                OVRD->ovrd_tipo->redraw();
                break;
            case MC_Dere_Type:
                DERELICT->derelict_tipo->value(rkr->Rack_Effects[EFX_DERELICT]->getpar(Dere_Type));
                DERELICT->derelict_tipo->redraw();
                break;
            case MC_DistBand_Type_Low:
                DISTBAND->distband_tipoL->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Type_Low));
                DISTBAND->distband_tipoL->redraw();
                break;
            case MC_DistBand_Type_Mid:
                DISTBAND->distband_tipoM->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Type_Mid));
                DISTBAND->distband_tipoM->redraw();
                break;
            case MC_DistBand_Type_Hi:
                DISTBAND->distband_tipoH->value(rkr->Rack_Effects[EFX_DISTBAND]->getpar(DistBand_Type_Hi));
                DISTBAND->distband_tipoH->redraw();
                break;
            case MC_Stomp_Mode:
                STOMPBOX->stomp_mode->value(rkr->Rack_Effects[EFX_STOMPBOX]->getpar(Stomp_Mode));
                STOMPBOX->stomp_mode->redraw();
                break;
            case MC_Alien_LFO_Type:
                ALIENWAH->Alienwah_lfotype->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_LFO_Type));
                ALIENWAH->Alienwah_lfotype->redraw();
                break;
            case MC_APhase_LFO_Type:
                APHASER->aphaser_lfotype->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_LFO_Type));
                APHASER->aphaser_lfotype->redraw();
                break;
            case MC_Chorus_LFO_Type:
                CHORUS->chorus_lfotype->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_LFO_Type));
                CHORUS->chorus_lfotype->redraw();
                break;
            case MC_Flanger_LFO_Type:
                FLANGER->flanger_lfotype->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_LFO_Type));
                FLANGER->flanger_lfotype->redraw();
                break;
            case MC_DFlange_LFO_Type:
                DFLANGE->dflange_lfotype->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_LFO_Type));
                DFLANGE->dflange_lfotype->redraw();
                break;
            case MC_Echotron_LFO_Type:
                ECHOTRON->echotron_lfotype->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_LFO_Type));
                ECHOTRON->echotron_lfotype->redraw();
                break;
            case MC_MuTro_LFO_Type:
                MUTROMOJO->mutromojo_lfotype->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_LFO_Type));
                MUTROMOJO->mutromojo_lfotype->redraw();
                break;
            case MC_Optical_LFO_Type:
                OTREM->otrem_lfotype->value(rkr->Rack_Effects[EFX_OPTICALTREM]->getpar(Optical_LFO_Type));
                OTREM->otrem_lfotype->redraw();
                break;
            case MC_Pan_LFO_Type:
                PAN->pan_lfotype->value(rkr->Rack_Effects[EFX_PAN]->getpar(Pan_LFO_Type));
                PAN->pan_lfotype->redraw();
                break;
            case MC_Phaser_LFO_Type:
                PHASER->phaser_lfotype->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_LFO_Type));
                PHASER->phaser_lfotype->redraw();
                break;
            case MC_Synthfilter_LFO_Type:
                SYNTHFILTER->synthfilter_lfotype->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_LFO_Type));
                SYNTHFILTER->synthfilter_lfotype->redraw();
                break;
            case MC_VaryBand_LFO_Type_1:
                VARYBAND->varyband_lfotype1->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_LFO_Type_1));
                VARYBAND->varyband_lfotype1->redraw();
                break;
            case MC_VaryBand_LFO_Type_2:
                VARYBAND->varyband_lfotype2->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_LFO_Type_2));
                VARYBAND->varyband_lfotype2->redraw();
                break;
            case MC_Vibe_LFO_Type:
                VIBE->vibe_lfotype->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LFO_Type));
                VIBE->vibe_lfotype->redraw();
                break;
            case MC_WahWah_LFO_Type:
                WAHWAH->WahWah_lfotype->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_LFO_Type));
                WAHWAH->WahWah_lfotype->redraw();
                break;
        }
    }


    rkr->Mvalue = 0;
}

void RKRGUI::ActOnOff()
{
    // turn efx on or off
    int miralo = 0;

    while (rkr->OnOffC > 0)
    {
        if (rkr->Mnumeff[rkr->OnOffC] > 2000)
        {
            miralo = rkr->Mnumeff[rkr->OnOffC] - 2000;
        }
        else if (rkr->Mnumeff[rkr->OnOffC] >= 1000)
        {
            miralo = rkr->Mnumeff[rkr->OnOffC] - 1000;
        }
        else    // efx_order[] is limited by C_NUMBER_ORDERED_EFFECTS
        {
            miralo = rkr->efx_order[rkr->Mnumeff[rkr->OnOffC]];
        }

        switch (miralo)
        {
            case EFX_EQ:
                EQ->eq_activar->value(rkr->EFX_Bypass[EFX_EQ]);
                EQ->eq_activar->do_callback();
                break;
            case EFX_COMPRESSOR:
                COMPRESS->compress_activar->value(rkr->EFX_Bypass[EFX_COMPRESSOR]);
                COMPRESS->compress_activar->do_callback();
                break;
            case EFX_DISTORTION:
                DIST->dist_activar->value(rkr->EFX_Bypass[EFX_DISTORTION]);
                DIST->dist_activar->do_callback();
                break;
            case EFX_OVERDRIVE:
                OVRD->ovrd_activar->value(rkr->EFX_Bypass[EFX_OVERDRIVE]);
                OVRD->ovrd_activar->do_callback();
                break;
            case EFX_ECHO:
                ECHO->echo_activar->value(rkr->EFX_Bypass[EFX_ECHO]);
                ECHO->echo_activar->do_callback();
                break;
            case EFX_CHORUS:
                CHORUS->chorus_activar->value(rkr->EFX_Bypass[EFX_CHORUS]);
                CHORUS->chorus_activar->do_callback();
                break;
            case EFX_PHASER:
                PHASER->phaser_activar->value(rkr->EFX_Bypass[EFX_PHASER]);
                PHASER->phaser_activar->do_callback();
                break;
            case EFX_FLANGER:
                FLANGER->flanger_activar->value(rkr->EFX_Bypass[EFX_FLANGER]);
                FLANGER->flanger_activar->do_callback();
                break;
            case EFX_REVERB:
                REVERB->reverb_activar->value(rkr->EFX_Bypass[EFX_REVERB]);
                REVERB->reverb_activar->do_callback();
                break;
            case EFX_PARAMETRIC:
                PEQ->eqp_activar->value(rkr->EFX_Bypass[EFX_PARAMETRIC]);
                PEQ->eqp_activar->do_callback();
                break;
            case EFX_WAHWAH:
                WAHWAH->WahWah_activar->value(rkr->EFX_Bypass[EFX_WAHWAH]);
                WAHWAH->WahWah_activar->do_callback();
                break;
            case EFX_ALIENWAH:
                ALIENWAH->Alienwah_activar->value(rkr->EFX_Bypass[EFX_ALIENWAH]);
                ALIENWAH->Alienwah_activar->do_callback();
                break;
            case EFX_CABINET:
                CABINET->Cabinet_activar->value(rkr->EFX_Bypass[EFX_CABINET]);
                CABINET->Cabinet_activar->do_callback();
                break;
            case EFX_PAN:
                PAN->pan_activar->value(rkr->EFX_Bypass[EFX_PAN]);
                PAN->pan_activar->do_callback();
                break;
            case EFX_HARMONIZER:
                HAR->har_activar->value(rkr->EFX_Bypass[EFX_HARMONIZER]);
                HAR->har_activar->do_callback();
                break;
            case EFX_MUSICAL_DELAY:
                MUSDELAY->musdelay_activar->value(rkr->EFX_Bypass[EFX_MUSICAL_DELAY]);
                MUSDELAY->musdelay_activar->do_callback();
                break;
            case EFX_NOISEGATE:
                GATE->gate_activar->value(rkr->EFX_Bypass[EFX_NOISEGATE]);
                GATE->gate_activar->do_callback();
                break;
            case EFX_DERELICT:
                DERELICT->derelict_activar->value(rkr->EFX_Bypass[EFX_DERELICT]);
                DERELICT->derelict_activar->do_callback();
                break;
            case EFX_ANALOG_PHASER:
                APHASER->aphaser_activar->value(rkr->EFX_Bypass[EFX_ANALOG_PHASER]);
                APHASER->aphaser_activar->do_callback();
                break;
            case EFX_VALVE:
                VALVE->valve_activar->value(rkr->EFX_Bypass[EFX_VALVE]);
                VALVE->valve_activar->do_callback();
                break;
            case EFX_DUAL_FLANGE:
                DFLANGE->dflange_activar->value(rkr->EFX_Bypass[EFX_DUAL_FLANGE]);
                DFLANGE->dflange_activar->do_callback();
                break;
            case EFX_RING:
                RING->ring_activar->value(rkr->EFX_Bypass[EFX_RING]);
                RING->ring_activar->do_callback();
                break;
            case EFX_EXCITER:
                EXCITER->exciter_activar->value(rkr->EFX_Bypass[EFX_EXCITER]);
                EXCITER->exciter_activar->do_callback();
                break;
            case EFX_DISTBAND:
                DISTBAND->distband_activar->value(rkr->EFX_Bypass[EFX_DISTBAND]);
                DISTBAND->distband_activar->do_callback();
                break;
            case EFX_ARPIE:
                ARPIE->arpie_activar->value(rkr->EFX_Bypass[EFX_ARPIE]);
                ARPIE->arpie_activar->do_callback();
                break;
            case EFX_EXPANDER:
                EXPANDER->expander_activar->value(rkr->EFX_Bypass[EFX_EXPANDER]);
                EXPANDER->expander_activar->do_callback();
                break;
            case EFX_SHUFFLE:
                SHUFFLE->shuffle_activar->value(rkr->EFX_Bypass[EFX_SHUFFLE]);
                SHUFFLE->shuffle_activar->do_callback();
                break;
            case EFX_SYNTHFILTER:
                SYNTHFILTER->synthfilter_activar->value(rkr->EFX_Bypass[EFX_SYNTHFILTER]);
                SYNTHFILTER->synthfilter_activar->do_callback();
                break;
            case EFX_VARYBAND:
                VARYBAND->varyband_activar->value(rkr->EFX_Bypass[EFX_VARYBAND]);
                VARYBAND->varyband_activar->do_callback();
                break;
            case EFX_CONVOLOTRON:
                CONVOLOTRON->convo_activar->value(rkr->EFX_Bypass[EFX_CONVOLOTRON]);
                CONVOLOTRON->convo_activar->do_callback();
                break;
            case EFX_LOOPER:
                LOOPER->looper_activar->value(rkr->EFX_Bypass[EFX_LOOPER]);
                LOOPER->looper_activar->do_callback();
                break;
            case EFX_MUTROMOJO:
                MUTROMOJO->mutromojo_activar->value(rkr->EFX_Bypass[EFX_MUTROMOJO]);
                MUTROMOJO->mutromojo_activar->do_callback();
                break;
            case EFX_ECHOVERSE:
                ECHOVERSE->echoverse_activar->value(rkr->EFX_Bypass[EFX_ECHOVERSE]);
                ECHOVERSE->echoverse_activar->do_callback();
                break;
            case EFX_COILCRAFTER:
                COILCRAFTER->coil_activar->value(rkr->EFX_Bypass[EFX_COILCRAFTER]);
                COILCRAFTER->coil_activar->do_callback();
                break;
            case EFX_SHELFBOOST:
                SHELFBOOST->shelf_activar->value(rkr->EFX_Bypass[EFX_SHELFBOOST]);
                SHELFBOOST->shelf_activar->do_callback();
                break;
            case EFX_VOCODER:
                VOCODER->vo_activar->value(rkr->EFX_Bypass[EFX_VOCODER]);
                VOCODER->vo_activar->do_callback();
                break;
            case EFX_SUSTAINER:
                SUSTAINER->sus_activar->value(rkr->EFX_Bypass[EFX_SUSTAINER]);
                SUSTAINER->sus_activar->do_callback();
                break;
            case EFX_SEQUENCE:
                SEQUENCE->seq_activar->value(rkr->EFX_Bypass[EFX_SEQUENCE]);
                SEQUENCE->seq_activar->do_callback();
                break;
            case EFX_SHIFTER:
                SHIFTER->shifter_activar->value(rkr->EFX_Bypass[EFX_SHIFTER]);
                SHIFTER->shifter_activar->do_callback();
                break;
            case EFX_STOMPBOX:
                STOMPBOX->stomp_activar->value(rkr->EFX_Bypass[EFX_STOMPBOX]);
                STOMPBOX->stomp_activar->do_callback();
                break;
            case EFX_REVERBTRON:
                REVERBTRON->revtron_activar->value(rkr->EFX_Bypass[EFX_REVERBTRON]);
                REVERBTRON->revtron_activar->do_callback();
                break;
            case EFX_ECHOTRON:
                ECHOTRON->echotron_activar->value(rkr->EFX_Bypass[EFX_ECHOTRON]);
                ECHOTRON->echotron_activar->do_callback();
                break;
            case EFX_STEREOHARM:
                SHAR->shar_activar->value(rkr->EFX_Bypass[EFX_STEREOHARM]);
                SHAR->shar_activar->do_callback();
                break;
            case EFX_COMPBAND:
                COMPBAND->cband_activar->value(rkr->EFX_Bypass[EFX_COMPBAND]);
                COMPBAND->cband_activar->do_callback();
                break;
            case EFX_OPTICALTREM:
                OTREM->otrem_activar->value(rkr->EFX_Bypass[EFX_OPTICALTREM]);
                OTREM->otrem_activar->do_callback();
                break;
            case EFX_VIBE:
                VIBE->vibe_activar->value(rkr->EFX_Bypass[EFX_VIBE]);
                VIBE->vibe_activar->do_callback();
                break;
            case EFX_INFINITY:
                INFINIT->infinity_activar->value(rkr->EFX_Bypass[EFX_INFINITY]);
                INFINIT->infinity_activar->do_callback();
                break;

            case EFX_TAP_TEMPO_ON_OFF:
                Tap_activar->value(rkr->Tap_Bypass);
                Tap_activar->do_callback();
                break;
            case EFX_MIDI_CONVERTER_ON_OFF:
                MIDI->midi_activar->value(rkr->MIDIConverter_Bypass);
                MIDI->midi_activar->do_callback();
                break;
            case EFX_TUNER_ON_OFF:
                tuner_activar->value(rkr->Tuner_Bypass);
                tuner_activar->do_callback();
                break;
            case EFX_MASTER_ON_OFF:
                ActivarGeneral->value(rkr->Bypass);
                ActivarGeneral->do_callback();
                break;
        }

        rkr->OnOffC--;
    }
}

void RKRGUI::PutBackground()
{
    // Put selected .png background image on everything
    delete back;
    back = new Fl_Tiled_Image(new Fl_PNG_Image(rkr->BackgroundImage), 3200, 2400);
    
    if (!rkr->EnableBackgroundImage)
    {
        back->color_average(back_color, 0.0);
    }

    InOut->image(back);
    EQ->image(InOut->image());
    COMPRESS->image(InOut->image());
    DIST->image(InOut->image());
    OVRD->image(InOut->image());
    ECHO->image(InOut->image());
    CHORUS->image(InOut->image());
    FLANGER->image(InOut->image());
    WAHWAH->image(InOut->image());
    ALIENWAH->image(InOut->image());
    PHASER->image(InOut->image());
    REVERB->image(InOut->image());
    PEQ->image(InOut->image());
    CABINET->image(InOut->image());
    PAN->image(InOut->image());
    HAR->image(InOut->image());
    MUSDELAY->image(InOut->image());
    GATE->image(InOut->image());
    DERELICT->image(InOut->image());
    APHASER->image(InOut->image());
    VALVE->image(InOut->image());
    DFLANGE->image(InOut->image());
    RING->image(InOut->image());
    EXCITER->image(InOut->image());
    DISTBAND->image(InOut->image());
    ARPIE->image(InOut->image());
    EXPANDER->image(InOut->image());
    SHUFFLE->image(InOut->image());
    SYNTHFILTER->image(InOut->image());
    VARYBAND->image(InOut->image());
    CONVOLOTRON->image(InOut->image());
    LOOPER->image(InOut->image());
    MUTROMOJO->image(InOut->image());
    ECHOVERSE->image(InOut->image());
    COILCRAFTER->image(InOut->image());
    SHELFBOOST->image(InOut->image());
    VOCODER->image(InOut->image());
    SUSTAINER->image(InOut->image());
    SEQUENCE->image(InOut->image());
    SHIFTER->image(InOut->image());
    STOMPBOX->image(InOut->image());
    REVERBTRON->image(InOut->image());
    ECHOTRON->image(InOut->image());
    SHAR->image(InOut->image());
    COMPBAND->image(InOut->image());
    OTREM->image(InOut->image());
    VIBE->image(InOut->image());
    INFINIT->image(InOut->image());

    Tap->image(InOut->image());
    Presets->image(InOut->image());
    Tuner->image(InOut->image());
    MIDI->image(InOut->image());
    Metro->image(InOut->image());
    fondo->image(InOut->image());
    TITTLE_L->image(InOut->image());
    Order->Fondo1->image(InOut->image());
    Settings->Fondo2->image(InOut->image());
    BankWindow->Fondo3->image(InOut->image());
    MIDILearn->Fondo4->image(InOut->image());
    Trigger->Fondo5->image(InOut->image());
    Settings->Fondo6->image(InOut->image());
    Settings->Fondo7->image(InOut->image());
    Settings->Fondo8->image(InOut->image());
    Settings->Fondo9->image(InOut->image());
    Settings->Fondo10->image(InOut->image());
    Settings->Fondo11->image(InOut->image());
    Settings->Fondo12->image(InOut->image());


    Etit->image(InOut->image());
    MIDILearn->Ares->image(InOut->image());

    MenuP->image(InOut->image());
    BankWindow->MenuB->image(InOut->image());


    Fl::redraw();
}

/**
 *  Changes the font size, colors and font type.
 * 
 * @param font_size
 *      The amount to change the font size. When != 0, it comes from the
 *      user adjusting the font up or down from the Settings/Preferences/Look
 *      + or - buttons for font size.
 */
void RKRGUI::chfsize(int font_size)
{
    /* global_font_size is used by all RKR widget overrides to adjust font size in draw().
     * This variable should only be adjusted here. */
    if(font_size)
    {
        global_font_size = rkr->fontsize = font_size;
    }

    /* Sort through widgets and adjust font colors and type */
    for (int t = 0; t < Principal->children(); t++)
    {
        Fl_Widget *w = Principal->child(t);
        long long ud = (long long) w->user_data();

        if (ud != BOX_LED_DATA)
        {
            w->labelcolor(label_color);     /* Lmt, Clip, Resample */
        }
        else
        {
            w->labelcolor(leds_color);      /* Aux, In, Out, 0.0% */
        }
        
        if (ud != 2)
        {
            w->selection_color(back_color); /* ????? */
        }
        else
        {
            w->selection_color(leds_color); /* ???? */
        }


        /* EFX, tuner, tap, volume control, etc */
        if (ud == 1)
        {
            Fl_Group *g = (Fl_Group *) w;

            for (int i = 0; i < g->children(); i++)
            {
                Fl_Widget *c = g->child(i);
                long long uh = (long long) c->user_data();

                if (uh != BOX_LED_DATA)
                {
                    c->labelcolor(label_color);     /* Labels on (almost) everything except sliders */
                }
                else
                {
                    c->labelcolor(leds_color);      /* ????? */
                }

                if (uh != 7)
                {
                    c->selection_color(back_color); /* ????? */
                }

                /* All efx On buttons, +10db, selections boxes, and all efx other buttons */
                if ((uh == 2) || (uh == 7) || (uh == 77) || (uh == 78))
                {
                    c->selection_color(leds_color);
                }

                c->color(fore_color);
                c->labelfont(rkr->font);
            }
        }
    }

    CLIP_LED->selection_color(FL_RED);
    Etit->labelcolor(leds_color);
    Trigger->aux_midi->color(fore_color);
    Trigger->aux_midi->textcolor(FL_BACKGROUND2_COLOR);

    ChangeActives();

    Fl::redraw();
}

void RKRGUI::ChangeActives()
{
    //Change label colors based on active or inactive
    Fl_Color on = fl_lighter(fl_lighter(label_color));
    Fl_Color off = fl_darker(label_color);


    if (rkr->active[0])
        L1->labelcolor(on);
    else
        L1->labelcolor(off);
    if (rkr->active[1])
        L2->labelcolor(on);
    else
        L2->labelcolor(off);
    if (rkr->active[2])
        L3->labelcolor(on);
    else
        L3->labelcolor(off);
    if (rkr->active[3])
        L4->labelcolor(on);
    else
        L4->labelcolor(off);
    if (rkr->active[4])
        L5->labelcolor(on);
    else
        L5->labelcolor(off);
    if (rkr->active[5])
        L6->labelcolor(on);
    else
        L6->labelcolor(off);
    if (rkr->active[6])
        L7->labelcolor(on);
    else
        L7->labelcolor(off);
    if (rkr->active[7])
        L8->labelcolor(on);
    else
        L8->labelcolor(off);
    if (rkr->active[8])
        L9->labelcolor(on);
    else
        L9->labelcolor(off);
    if (rkr->active[9])
        L10->labelcolor(on);
    else
        L10->labelcolor(off);



    if (rkr->MIDIConverter_Bypass)
        MIDI->MIDI_LABEL->labelcolor(on);
    else
        MIDI->MIDI_LABEL->labelcolor(off);
    if (rkr->Metro_Bypass)
        Metro_Label->labelcolor(on);
    else
        Metro_Label->labelcolor(off);
    if (rkr->Tap_Bypass)
        TAP_LABEL->labelcolor(on);
    else
        TAP_LABEL->labelcolor(off);
    if (rkr->Tuner_Bypass)
        TUNER_LABEL->labelcolor(on);
    else
        TUNER_LABEL->labelcolor(off);
    if (rkr->Bypass)
        LABEL_IO->labelcolor(on);
    else
        LABEL_IO->labelcolor(off);

    if ((rkr->upsample) && (rkr->Bypass))
    {
        UPS_LED->color(leds_color);
        UPS_LED->redraw();
    }
    else
    {
        UPS_LED->color(fl_darker(leds_color));
        UPS_LED->redraw();
    }

    CLIP_LED->color(fl_darker(FL_RED));
    LMT_LED->color(fl_darker(leds_color));
}

void RKRGUI::findpos(int num, int value, Fl_Widget*)
{
    // adjust efx title label colors based on active of inactive
    int i = 0;
    Fl_Color on = fl_lighter(fl_lighter(label_color));
    Fl_Color off = fl_darker(label_color);

    for (i = 0; i < C_NUMBER_ORDERED_EFFECTS; i++)
    {
        if (rkr->efx_order[i] == num)
        {
            rkr->active[i] = value;
            break;
        }
    }


    switch (i)
    {

        case 0:
            if (value)
                L1->labelcolor(on);
            else
                L1->labelcolor(off);
            L1->redraw_label();
            break;

        case 1:
            if (value)
                L2->labelcolor(on);
            else
                L2->labelcolor(off);
            L2->redraw_label();
            break;

        case 2:
            if (value)
                L3->labelcolor(on);
            else
                L3->labelcolor(off);
            L3->redraw_label();
            break;

        case 3:
            if (value)
                L4->labelcolor(on);
            else
                L4->labelcolor(off);
            L4->redraw_label();
            break;

        case 4:
            if (value)
                L5->labelcolor(on);
            else
                L5->labelcolor(off);
            L5->redraw_label();
            break;

        case 5:
            if (value)
                L6->labelcolor(on);
            else
                L6->labelcolor(off);
            L6->redraw_label();
            break;

        case 6:
            if (value)
                L7->labelcolor(on);
            else
                L7->labelcolor(off);
            L7->redraw_label();
            break;

        case 7:
            if (value)
                L8->labelcolor(on);
            else
                L8->labelcolor(off);
            L8->redraw_label();
            break;

        case 8:
            if (value)
                L9->labelcolor(on);
            else
                L9->labelcolor(off);
            L9->redraw_label();
            break;

        case 9:
            if (value)
                L10->labelcolor(on);
            else
                L10->labelcolor(off);
            L10->redraw_label();
            break;
    }
}

void RKRGUI::Put_Skin()
{
    // adjust based on theme selected in settings-look
    Principal->resize(Principal->x(), Principal->y(), rkr->swidth, rkr->sheight);

    back_color = (Fl_Color) rkr->sback_color;
    fore_color = (Fl_Color) rkr->sfore_color;
    leds_color = (Fl_Color) rkr->sleds_color;
    label_color = (Fl_Color) rkr->slabel_color;

    Settings->scheme_ch->value(rkr->sschema);
    Settings->scheme_ch->do_callback();
    PutBackground();
    chfsize(rkr->fontsize);
    Leds_Color_Change(leds_color);
    Buttons_Color_Change(fore_color);


    save_current_state(3);
    Fl::redraw();
}

void RKRGUI::FillML(/*int type*/)
{
    // Midi learn
    char tmp[256];
    memset(tmp, 0, sizeof (tmp));


    sprintf(tmp, "%s   v%s - MIDI Learn - Preset : %s", rkr->jackcliname, VERSION, rkr->Bank[rkr->Selected_Preset].Preset_Name);
    MIDILearn->copy_label(tmp);

    memset(rkr->ML_clist, 0, sizeof (rkr->ML_clist));
    MIDILearn->Epar->clear();
    
    int k = 0;

    switch (rkr->ML_filter)
    {
        case 0:
            for (int i = 0; i < C_MC_PARAMETER_SIZE; i++)
                MIDILearn->Epar->add(rkr->efx_params[i].Nom);
            break;

        case 1:
            for (int i = 0; i < C_MC_PARAMETER_SIZE; i++)
            {
                if (rkr->efx_params[i].Effect == EFX_MASTER)
                {
                    MIDILearn->Epar->add(rkr->efx_params[i].Nom);
                    rkr->ML_clist[k] = rkr->efx_params[i].Ato;
                    k++;
                }
            }

            for (int j = 0; j < C_NUMBER_ORDERED_EFFECTS; j++)
            {
                for (int i = 0; i < C_MC_PARAMETER_SIZE; i++)
                {
                    if (rkr->efx_params[i].Effect == rkr->efx_order[j])
                    {
                        MIDILearn->Epar->add(rkr->efx_params[i].Nom);
                        rkr->ML_clist[k] = rkr->efx_params[i].Ato;
                        k++;
                    }
                }
            }
            break;
    }

    MIDILearn->Epar->redraw();


    MIDILearn->TPresets->clear();
    for (int i = 1; i <= 60; i++)
    {
        MIDILearn->TPresets->add(rkr->Bank[i].Preset_Name);
    }

    MIDILearn->TPresets->select(rkr->Selected_Preset, 1);
    MIDILearn->TPresets->redraw();

    DisAssigns();
}

void RKRGUI::DisAssigns()
{
    //DisAssigns
    char tmp[8];

    int the_one = 0;

    if ((int) MIDILearn->Epar->value())
    {
        if (rkr->ML_filter == 0)
        {
            the_one = rkr->efx_params[(int) MIDILearn->Epar->value() - 1].Ato;
        }
        else
        {
            the_one = rkr->ML_clist[(int) MIDILearn->Epar->value() - 1];
        }
    }

    int k = 0;

    for (int i = 0; i < 128; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (rkr->XUserMIDI[i][j] == the_one)
            {
                k++;
                memset(tmp, 0, sizeof (tmp));
                sprintf(tmp, "%d", i);

                switch (k)
                {
                    case 1:
                        MIDILearn->Ar1->copy_label(tmp);
                        MIDILearn->Ar1->redraw();
                        break;

                    case 2:
                        MIDILearn->Ar2->copy_label(tmp);
                        MIDILearn->Ar2->redraw();
                        break;

                    case 3:
                        MIDILearn->Ar3->copy_label(tmp);
                        MIDILearn->Ar3->redraw();
                        break;

                    case 4:
                        MIDILearn->Ar4->copy_label(tmp);
                        MIDILearn->Ar4->redraw();
                        break;

                    case 5:
                        MIDILearn->Ar5->copy_label(tmp);
                        MIDILearn->Ar5->redraw();
                        break;

                    case 6:
                        MIDILearn->Ar6->copy_label(tmp);
                        MIDILearn->Ar6->redraw();
                        break;

                    case 7:
                        MIDILearn->Ar7->copy_label(tmp);
                        MIDILearn->Ar7->redraw();
                        break;

                    case 8:
                        MIDILearn->Ar8->copy_label(tmp);
                        MIDILearn->Ar8->redraw();
                        break;

                    case 9:
                        MIDILearn->Ar9->copy_label(tmp);
                        MIDILearn->Ar9->redraw();
                        break;

                    case 10:
                        MIDILearn->Ar10->copy_label(tmp);
                        MIDILearn->Ar10->redraw();
                        break;

                    case 11:
                        MIDILearn->Ar11->copy_label(tmp);
                        MIDILearn->Ar11->redraw();
                        break;

                    case 12:
                        MIDILearn->Ar12->copy_label(tmp);
                        MIDILearn->Ar12->redraw();
                        break;

                    case 13:
                        MIDILearn->Ar13->copy_label(tmp);
                        MIDILearn->Ar13->redraw();
                        break;

                    case 14:
                        MIDILearn->Ar14->copy_label(tmp);
                        MIDILearn->Ar14->redraw();
                        break;

                    case 15:
                        MIDILearn->Ar15->copy_label(tmp);
                        MIDILearn->Ar15->redraw();
                        break;

                    case 16:
                        MIDILearn->Ar16->copy_label(tmp);
                        MIDILearn->Ar16->redraw();
                        break;

                    case 17:
                        MIDILearn->Ar17->copy_label(tmp);
                        MIDILearn->Ar17->redraw();
                        break;

                    case 18:
                        MIDILearn->Ar18->copy_label(tmp);
                        MIDILearn->Ar18->redraw();
                        break;

                    case 19:
                        MIDILearn->Ar19->copy_label(tmp);
                        MIDILearn->Ar19->redraw();
                        break;

                    case 20:
                        MIDILearn->Ar20->copy_label(tmp);
                        MIDILearn->Ar20->redraw();
                        break;
                }
            }
        }
    }


    k++;

    for (int i = k; i <= 20; i++)
    {
        memset(tmp, 0, sizeof (tmp));

        switch (i)
        {
            case 1:
                MIDILearn->Ar1->copy_label(tmp);
                MIDILearn->Ar1->redraw();
                break;

            case 2:
                MIDILearn->Ar2->copy_label(tmp);
                MIDILearn->Ar2->redraw();
                break;

            case 3:
                MIDILearn->Ar3->copy_label(tmp);
                MIDILearn->Ar3->redraw();
                break;

            case 4:
                MIDILearn->Ar4->copy_label(tmp);
                MIDILearn->Ar4->redraw();
                break;

            case 5:
                MIDILearn->Ar5->copy_label(tmp);
                MIDILearn->Ar5->redraw();
                break;

            case 6:
                MIDILearn->Ar6->copy_label(tmp);
                MIDILearn->Ar6->redraw();
                break;

            case 7:
                MIDILearn->Ar7->copy_label(tmp);
                MIDILearn->Ar7->redraw();
                break;

            case 8:
                MIDILearn->Ar8->copy_label(tmp);
                MIDILearn->Ar8->redraw();
                break;

            case 9:
                MIDILearn->Ar9->copy_label(tmp);
                MIDILearn->Ar9->redraw();
                break;

            case 10:
                MIDILearn->Ar10->copy_label(tmp);
                MIDILearn->Ar10->redraw();
                break;

            case 11:
                MIDILearn->Ar11->copy_label(tmp);
                MIDILearn->Ar11->redraw();
                break;

            case 12:
                MIDILearn->Ar12->copy_label(tmp);
                MIDILearn->Ar12->redraw();
                break;

            case 13:
                MIDILearn->Ar13->copy_label(tmp);
                MIDILearn->Ar13->redraw();
                break;

            case 14:
                MIDILearn->Ar14->copy_label(tmp);
                MIDILearn->Ar14->redraw();
                break;

            case 15:
                MIDILearn->Ar15->copy_label(tmp);
                MIDILearn->Ar15->redraw();
                break;

            case 16:
                MIDILearn->Ar16->copy_label(tmp);
                MIDILearn->Ar16->redraw();
                break;

            case 17:
                MIDILearn->Ar17->copy_label(tmp);
                MIDILearn->Ar17->redraw();
                break;

            case 18:
                MIDILearn->Ar18->copy_label(tmp);
                MIDILearn->Ar18->redraw();
                break;

            case 19:
                MIDILearn->Ar19->copy_label(tmp);
                MIDILearn->Ar19->redraw();
                break;

            case 20:
                MIDILearn->Ar20->copy_label(tmp);
                MIDILearn->Ar20->redraw();
                break;
        }
    }
}

void RKRGUI::Prepare_Order()
{
    // prepare order
    Order->Order_Bro->clear();

    for (int i = 0; i < C_NUMBER_ORDERED_EFFECTS; i++)
    {
        rkr->new_order[i] = rkr->efx_order[i];
        rkr->saved_order[i] = rkr->efx_order[i];
        Order->Order_Bro->add(rkr->efx_names[Busca_Eff(rkr->efx_order[i])].Nom);
    }

    Fill_Avail(rkr->eff_filter);

    Order->Order_Bro->select(1);
}

void RKRGUI::Show_Next_Time()
{
    // popup for settings changes that will not take effect until restart
    if (rkr->mess_dis)
        return;

    Fl_Widget *w = fl_message_icon();
    w->parent()->copy_label(rkr->jackcliname);
    fl_message("This setting will be changed the next time you run rakarrack-plus");
}

void RKRGUI::update_looper()
{
    // update looper
    Looper *Efx_Looper = static_cast <Looper*> (rkr->Rack_Effects[EFX_LOOPER]);
    Efx_Looper->getstate();

    LOOPER->looper_play->value(Efx_Looper->progstate[0]);
    LOOPER->looper_play->redraw();
    LOOPER->looper_record->value(Efx_Looper->progstate[2]);
    LOOPER->looper_record->redraw();
    LOOPER->looper_stop->value(Efx_Looper->progstate[1]);
    LOOPER->looper_stop->redraw();

    if (Efx_Looper->progstate[2])
    {
        LOOPER->Box_P->copy_label("Stop");
        LOOPER->looper_stop->copy_label("@square");
    }
    else
    {
        LOOPER->Box_P->copy_label("Pause");
        LOOPER->looper_stop->copy_label("@||");
    }

    LOOPER->looper_t1->value(Efx_Looper->progstate[4]);
    LOOPER->looper_t1->redraw();
    LOOPER->looper_t2->value(Efx_Looper->progstate[5]);
    LOOPER->looper_t2->redraw();
}

void RKRGUI::UpdateTGUI()
{
    // updates the efx based on global tempo settings
    if (rkr->EFX_Bypass[EFX_LOOPER])
    {
        LOOPER->looper_Tempo->value(rkr->Rack_Effects[EFX_LOOPER]->getpar(Looper_Tempo));
        LOOPER->looper_Tempo->redraw();
    }

    if (rkr->EFX_Bypass[EFX_CHORUS])
    {
        CHORUS->chorus_freq->value(rkr->Rack_Effects[EFX_CHORUS]->getpar(Chorus_LFO_Tempo));
        CHORUS->chorus_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_FLANGER])
    {
        FLANGER->flanger_freq->value(rkr->Rack_Effects[EFX_FLANGER]->getpar(Flanger_LFO_Tempo));
        FLANGER->flanger_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_PHASER])
    {
        PHASER->phaser_freq->value(rkr->Rack_Effects[EFX_PHASER]->getpar(Phaser_LFO_Tempo));
        PHASER->phaser_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_PAN])
    {
        PAN->pan_freq->value(rkr->Rack_Effects[EFX_PAN]->getpar(Pan_LFO_Tempo));
        PAN->pan_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_WAHWAH])
    {
        WAHWAH->WahWah_freq->value(rkr->Rack_Effects[EFX_WAHWAH]->getpar(WahWah_LFO_Tempo));
        WAHWAH->WahWah_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_ALIENWAH])
    {
        ALIENWAH->Alienwah_freq->value(rkr->Rack_Effects[EFX_ALIENWAH]->getpar(Alien_LFO_Tempo));
        ALIENWAH->Alienwah_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_MUSICAL_DELAY])
    {
        MUSDELAY->musdelay_tempo->value(rkr->Rack_Effects[EFX_MUSICAL_DELAY]->getpar(Music_Tempo));
        MUSDELAY->musdelay_tempo->redraw();
    }

    if (rkr->EFX_Bypass[EFX_ANALOG_PHASER])
    {
        APHASER->aphaser_freq->value(rkr->Rack_Effects[EFX_ANALOG_PHASER]->getpar(APhase_LFO_Tempo));
        APHASER->aphaser_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_DUAL_FLANGE])
    {
        DFLANGE->dflange_freq->value(rkr->Rack_Effects[EFX_DUAL_FLANGE]->getpar(DFlange_LFO_Tempo));
        DFLANGE->dflange_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_SYNTHFILTER])
    {
        SYNTHFILTER->synthfilter_freq->value(rkr->Rack_Effects[EFX_SYNTHFILTER]->getpar(Synthfilter_LFO_Tempo));
        SYNTHFILTER->synthfilter_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_MUTROMOJO])
    {
        MUTROMOJO->mutromojo_freq->value(rkr->Rack_Effects[EFX_MUTROMOJO]->getpar(MuTro_LFO_Tempo));
        MUTROMOJO->mutromojo_freq->redraw();
    }

    if (rkr->EFX_Bypass[EFX_VARYBAND])
    {
        VARYBAND->varyband_freq1->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_LFO_Tempo_1));
        VARYBAND->varyband_freq1->redraw();
    }

    if (rkr->EFX_Bypass[EFX_VARYBAND])
    {
        VARYBAND->varyband_freq2->value(rkr->Rack_Effects[EFX_VARYBAND]->getpar(VaryBand_LFO_Tempo_2));
        VARYBAND->varyband_freq2->redraw();
    }

    if (rkr->EFX_Bypass[EFX_ARPIE])
    {
        ARPIE->arpie_delay->value(rkr->Rack_Effects[EFX_ARPIE]->getpar(Arpie_Tempo));
        ARPIE->arpie_delay->redraw();
    }

    if (rkr->EFX_Bypass[EFX_ECHOVERSE])
    {
        ECHOVERSE->echoverse_delay->value(rkr->Rack_Effects[EFX_ECHOVERSE]->getpar(Echoverse_Tempo));
        ECHOVERSE->echoverse_delay->redraw();
    }
    if (rkr->EFX_Bypass[EFX_SEQUENCE])
    {
        SEQUENCE->seq_tempo->value(rkr->Rack_Effects[EFX_SEQUENCE]->getpar(Sequence_Tempo));
        SEQUENCE->seq_tempo->redraw();
    }
    if (rkr->EFX_Bypass[EFX_ECHO])
    {
        ECHO->echo_delay->value(rkr->Rack_Effects[EFX_ECHO]->getpar(Echo_Delay));
        ECHO->echo_delay->redraw();
    }
    if (rkr->EFX_Bypass[EFX_ECHOTRON])
    {
        ECHOTRON->echotron_tempo->value(rkr->Rack_Effects[EFX_ECHOTRON]->getpar(Echotron_Tempo));
        ECHOTRON->echotron_tempo->redraw();
    }
    if (rkr->EFX_Bypass[EFX_OPTICALTREM])
    {
        OTREM->otrem_freq->value(rkr->Rack_Effects[EFX_OPTICALTREM]->getpar(Optical_LFO_Tempo));
        OTREM->otrem_freq->redraw();
    }
    if (rkr->EFX_Bypass[EFX_VIBE])
    {
        VIBE->vibe_freq->value(rkr->Rack_Effects[EFX_VIBE]->getpar(Vibe_LFO_Tempo));
        VIBE->vibe_freq->redraw();
    }
    if (rkr->EFX_Bypass[EFX_INFINITY])
    {
        INFINIT->infinity_rate->value(rkr->Rack_Effects[EFX_INFINITY]->getpar(Infinity_Tempo));
        INFINIT->infinity_rate->redraw();
    }
}

void RKRGUI::ActACI()
{
    //ActACI
    float gain = 0.0;
    float tmp = 0.0;

    switch (rkr->Aux_Source)
    {
        case 0:
            gain = dB2rap(75.0f * (float) rkr->Aux_Gain / 127.0f);
            tmp = rkr->val_a_sum * gain;
            break;
        case 1:
            gain = (float) rkr->Aux_Gain / 127.0f;
            tmp = dB2rap(rkr->val_il_sum) * 12.0f * gain;
            break;
        case 2:
            gain = (float) rkr->Aux_Gain / 127.0f;
            tmp = dB2rap(rkr->val_ir_sum) * 12.0f * gain;
            break;
    }

    float aux_vulevel = (float) CLAMP(rap2dB(tmp), -48.0, 15.0);

    float threshold = (float) rkr->Aux_Threshold;
    Trigger->aux_vu->value(aux_vulevel);

    if (aux_vulevel < threshold)
        tmp = 0.0;

    if (tmp > 1.0)
        tmp = 1.0f;

    int amax = rkr->Aux_Maximum;
    int amin = rkr->Aux_Minimum;

    int value = amin + lrintf((float) (amax - amin) * tmp);

    if (value != rkr->last_auxvalue)
    {
        if (rkr->MIDIway)
        {
            for (int i = 0; i < 20; i++)
            {
                if (rkr->XUserMIDI[rkr->Aux_MIDI][i])
                {
                    rkr->process_midi_controller_events(rkr->XUserMIDI[rkr->Aux_MIDI][i], value);
                }
                else
                    break;
            }
        }
        else
        {
            rkr->process_midi_controller_events(rkr->Aux_MIDI, value);
        }

        rkr->last_auxvalue = value;
    }
}

int RKRGUI::Busca_Eff(int num)
{
    // Busca_Eff
    int i = 0;

    for (i = 0; i < C_NUMBER_EFFECTS; i++)
    {
        if (rkr->efx_names[i].Pos == num)
        {
            break;
        }
    }

    return (i);
}

void RKRGUI::Fill_Avail(int filter)
{
    //Fill_Avail

    Order->Avail_Bro->clear();

    int t = 1;

    for (int i = 0; i < C_NUMBER_EFFECTS; i++)
    {
        int k = 0;
        for (int j = 0; j < C_NUMBER_ORDERED_EFFECTS; j++)
        {
            if (rkr->new_order[j] == rkr->efx_names[i].Pos)
            {
                k = 1;
            }
        }

        if (!k)
        {
            if ((rkr->efx_names[i].Type & rkr->eff_filter) == filter)
            {
                Order->Avail_Bro->add(rkr->efx_names[i].Nom);
                rkr->availables[t] = rkr->efx_names[i].Pos;
                t++;
            }
        }
    }

    Order->Avail_Bro->select(1);
}

void RKRGUI::highlight()
{
    // highlight
    int tipo = 0;
    long long k = 0;

    Fl_Widget *w;

    if (Fl::focus() == TITTLE_L)
        Fl::focus(Open_Order);

    if (Fl::focus() == Etit)
        Fl::focus(Open_Order);

    w = Fl::belowmouse();

    if (w != NULL)
    {
        tipo = (int) w->type();
        k = (long long) w->user_data();
    }
    else
    {
        if (old != NULL)
        {
            old->color(fore_color);
            old->redraw();
        }
    }


    if ((tipo == 1) || (k == 77))
    {
        if (old != NULL)
        {
            old->color(fore_color);
            old->redraw();
        }
        w->color(fl_color_average(fore_color, fl_lighter(fore_color), .8));
        w->redraw();
        old = w;
    }

    else
    {
        if (old != NULL)
        {
            old->color(fore_color);
            old->redraw();
            old->redraw_label();
        }
    }

    if (k == 99)
    {
        Fl_Browser *b;
        b = (Fl_Browser *) w;
        int tecla = Fl::event_key();
        if (tecla != last_tecla)
        {
            last_tecla = tecla;
            for (int i = 1; i <= b->size(); i++)
            {
                if (b->text(i)[0] >= tecla - 32)
                {
                    b->select(i, 1);
                    break;
                }
            }
        }
    }
}

int RKRGUI::search_but(int x, int y)
{
    // search_but
    for (int t = 0; t < BankWindow->ob->children(); t++)
    {
        Fl_Widget *w = BankWindow->ob->child(t);

        if ((x >= w->x()) && (x <= (w->x() + w->w())) && (y >= w->y()) && (y <= (w->y() + w->h())))
        {
            long long kn = (long long) w->user_data();
            return ((int) kn);
        }
    }

    return (1000);
}

/** 
 Scan the default (DATADIR) and user directories for bank files (.rkrb).
 The user directory is set in Settings/Preferences/Bank/User Directory.
 Any directory files found are passed to the BankWindow class and listed on the
 Bank Manager "User Banks" drop down menu.
 */
void RKRGUI::Scan_Bank_Dir()
{
    ClearBankNames(); /* This will free all memory allocated for names */
    BankWindow->CH_UB->clear();
    BankWindow->clear_menu();
    
    /* Scan Default Directory for Bank files */
    Set_Bank(DATADIR);
    
    /* Scan User Directory for Bank files */
    Set_Bank(rkr->UDirFilename);
    
    BankWindow->CH_UB->value(0);
}

/**
 *  Scan a directory for Bank files and if valid, send it to the BankWindow class.
 * 
 * @param directory
 *      The directory to be scanned.
 */
void RKRGUI::Set_Bank(std::string directory)
{
    char nombank[FILENAME_MAX];
    DIR *dir;
    struct dirent *fs;
    
    dir = opendir(directory.c_str());
    if (dir == NULL)
        return;

    while ((fs = readdir(dir)))
    {
        if (strstr(fs->d_name, ".rkrb") != NULL)
        {
            /* Construct full bank file name with path */
            sprintf(nombank, "%s/%s", directory.c_str(), fs->d_name);
            
            AddBankName(nombank);
            
            if (rkr->CheckOldBank(nombank) == 0)
            {
                /* Get the name to be listed in the drop down "User Banks" menu */
                std::string s_nombre = fs->d_name;
                s_nombre = s_nombre.substr(0, s_nombre.size() - c_rkrb_ext_size);   // remove extension
                
                if(!s_nombre.empty())
                {
                    BankWindow->set_bank_CH_UB(&s_nombre[0], nombank);
                } 
            }
        }
    }

    closedir(dir);
}

int RKRGUI::prevnext(int e)
{
    // find various keybinding keys for processing
    if (e == 5)
    {
        Fl_Widget *w = Fl::belowmouse();
        long long k = (long long) w->user_data();
        if ((k < 770) || (k > 779))
            return 0;
        drag = k - 770;
        return 1;
    }


    if (e != 12)
    {
        return 0;
    }
    else
    {
        if ((Fl::event_key(43)) || (Fl::event_key(FL_KP + 43))) // +(plus) key
        {
            stecla = 1;
            return 1;
        }

        if ((Fl::event_key(45)) || (Fl::event_key(FL_KP + 45))) // -(minus) key
        {
            stecla = 2;
            return 1;
        }

        if (Fl::event_key(FL_F + 2)) // F2 key - decrease output volume
        {
            stecla = 3;
            return 1;
        }

        if (Fl::event_key(FL_F + 3)) // F3 key - increase output volume
        {
            stecla = 4;
            return 1;
        }

        if (Fl::event_key(FL_Insert)) // Insert key - add user preset
        {
            Fl_Widget *w = Fl::belowmouse();
            
            if (w == NULL)
            {
                return 0;
            }
            
            long long k = (long long) w->user_data();
            
            if ((k > 11999) && (k < 12100))
            {
                ((RKRGUI*) (w->parent()->parent()->user_data()))->addpreset(w, k - 12000);
            }
            
            return 1;
        }

        if (Fl::event_key(FL_Delete)) // Delete key - delete user preset
        {
            Fl_Widget *w = Fl::belowmouse();
            
            if (w == NULL)
            {
                return 0;
            }
            long long k = (long long) w->user_data();
            
            if ((k > 11999) && (k < 12100))
            {
                ((RKRGUI*) (w->parent()->parent()->user_data()))->delpreset(w, k - 12000);
            }
            
            return 1;
        }
    }

    return 0;
}

bool RKRGUI::install_signal_handlers()
{
    /*install signal handlers*/
    struct sigaction action;
    memset(&action, 0, sizeof (action));
    action.sa_handler = sigterm_handler;

    if (sigaction(SIGUSR1, &action, NULL) == -1)
    {
        printf("sigaction() failed: \n");
        return false;
    }

    if (sigaction(SIGINT, &action, NULL) == -1)
    {
        printf("sigaction() failed: \n");
        return false;
    }

    return true;
}

void RKRGUI::sigterm_handler(int sig)
{
    // handle signal type
    if (sig == SIGUSR1)
    {
        got_sigusr1 = sig;
    }

    if (sig == SIGINT)
    {
        got_sigint = sig;
    }
}

void RKRGUI::check_signals(void *usrPtr)
{
    // process signals
    RKRGUI *gui = NULL;
    gui = (RKRGUI*) usrPtr;

    if (!gui)
        return;

    if (got_sigusr1 == SIGUSR1)
    {
        if (filetoload != NULL) // individual preset
        {
            printf("Saving file: %s\n", filetoload);
            got_sigusr1 = 0;
            rkr->save_preset(filetoload);
        }
        return;
    }

    if (got_sigint == SIGINT)
    {
        printf("Got SIGTERM, quitting...\n");
        got_sigint = 0;
        Pexitprogram = 1;
    }
}

/**
 *  This pops up the midi learn window and sets the item (num) to be set with 
 *  midi learn. It is triggered by right mouse click on any efx parameter (slider, etc)
 *  that has midi learn capability.
 * 
 *  Don't pop up the midi learn window unless the user checked the box
 *  in Settings/Preferences/MIDI/Midi Learn - Really confusing when
 *  it pops up but nothing happens when set. The menu item for Midi Learn
 *  is set inactive unless the box is checked as well. For consistency here,
 *  if (!rkr->MIDIway)
 *      return;
 * 
 * @param num
 *  The efx parameter number that is to be used by midi learn.
 */
void RKRGUI::getMIDIControl(int num)
{
    // Don't pop up the midi learn window unless the user checked the box.
    if (!rkr->MIDIway)
        return;

    int i = 0;
    rkr->comemouse = 1;
    PrepareML();
    while (i < MIDILearn->Epar->size())
    {
        if ((rkr->ML_filter == 0) && (rkr->efx_params[i].Ato == num))
        {
            MIDILearn->Epar->select(i + 1);
            break;
        }

        if ((rkr->ML_filter == 1) && (rkr->ML_clist[i] == num))
        {
            MIDILearn->Epar->select(i + 1);
            break;
        }

        i++;
    }

    DisAssigns();
    MIDILearn->GMM->do_callback();
}

void RKRGUI::PrepareML()
{
    // PrepareML
    FillML();
    if (rkr->comemouse == 0)
    {
        MIDILearn->show();
        put_icon(MIDILearn);
        return;
    }

    if (rkr->comemouse)
    {
        if (rkr->autoassign == 0)
        {
            MIDILearn->show();
            put_icon(MIDILearn);
            return;
        }
    }
}

inline void RKRGUI::addpreset(Fl_Widget *w, int num)
{
    // Add preset
    if (num == 12)
        return;

    const char *name = fl_input("Preset Name?", "");
    if (name == NULL)
        return;

    char NewName[64];
    memset(NewName, 0, sizeof (NewName));
    sprintf(NewName, "*%s", name);
    add_name(w, NewName);
    rkr->save_insert_preset(num, NewName);
}

void RKRGUI::add_name(Fl_Widget *w, char *name)
{
    // add name
    Fl_Choice *s = (Fl_Choice *) w;
    s->add(name);

    Fl_Menu_*n = (Fl_Menu_*) s->menu();
    Fl_Menu_Item *m = (Fl_Menu_Item*) n;
    Fl_Menu_Item *p;
    
    int k = 10; // default label size

    for (int i = 0; i < m->size(); i++)
    {
        p = m->next(i);
        
        if (i == 0)
        {
            k = p->labelsize();
        }
        
        p->labelsize(k);
    }
}

Fl_Widget * RKRGUI::FindWidget(int num)
{
    // FindWidget
    for (int t = 0; t < Principal->children(); t++)
    {
        Fl_Widget *w = Principal->child(t);
        long long ud = (long long) w->user_data();

        if (ud == 1)
        {
            Fl_Group *g = (Fl_Group *) w;

            for (int i = 0; i < g->children(); i++)
            {
                Fl_Widget *c = g->child(i);
                long long uh = (long long) c->user_data();
                if (uh == (num + 12000))
                    return c;
            }
        }
    }

    return NULL;
}

void RKRGUI::ReadIntPresets()
{
    // Read in user presets
    FILE *fn;
    char tempfile[256];
    char buf[256];
    char *name;
    char *sbuf;
    int num = 0;
    memset(tempfile, 0, sizeof (tempfile));

    sprintf(tempfile, "%s%s", getenv("HOME"), "/.rkrintpreset");

    if ((fn = fopen(tempfile, "r")) != NULL)
    {
        while (fgets(buf, sizeof buf, fn) != NULL)
        {
            sbuf = buf;
            sscanf(buf, "%d", &num);
            name = strsep(&sbuf, ",");
            name = strsep(&sbuf, ",");
            add_name(FindWidget(num), name);
        }

        fclose(fn);
    }
}

inline void RKRGUI::delpreset(Fl_Widget *w, int num)
{
    // delete user preset
    if (num == 12)
        return;

    char Rname[128];
    Fl_Choice *s = (Fl_Choice *) w;

    if (strncmp(s->text(), "*", 1) != 0)
    {
        fl_message("Internal Presets can not be deleted ");
        return;
    }

    int ok = fl_choice("Delete \"%s\"?", "No", "Yes", NULL, s->text());

    if (!ok)
    {
        return;
    }

    memset(Rname, 0, sizeof (Rname));
    sprintf(Rname, "%s", s->text());
    s->remove(s->value());
    s->value(0);
    s->redraw();
    rkr->delete_insert_preset(num, Rname);
}

void RKRGUI::Prep_Reorden(int source, int dest)
{
    // Prepare Reorden
    int x = rkr->efx_order[source];
    int y = rkr->efx_order[dest];

    rkr->efx_order[source] = y;
    rkr->efx_order[dest] = x;

    reordena();

    if (Order->visible())
        Prepare_Order();
}

void RKRGUI::bank_click(Fl_Choice* o, void* v)
{
    // Bank click
    ((RKRGUI*) (o->parent()->parent()->parent()->parent()->user_data()))->bank_click_i(o, v);
}

inline void RKRGUI::bank_click_i(Fl_Choice* o, void*)
{
    // bank_click_i
    long long kk = (long long) o->user_data();
    int num = (int) kk;

    rkr->M_table[num - c_bank_used].bank = (int) o->value();
    Settings->fill_mptable(num + c_bank_used, rkr->M_table[num - c_bank_used].bank);
}

void RKRGUI::p_click(Fl_Choice* o, void* v)
{
    // p_click
    ((RKRGUI*) (o->parent()->parent()->parent()->parent()->user_data()))->p_click_i(o, v);
}

inline void RKRGUI::p_click_i(Fl_Choice* o, void*)
{
    // p_click_i
    long long kk = (long long) o->user_data();
    int num = (int) kk;

    rkr->M_table[num - c_preset_used].preset = o->value();
}

void RKRGUI::RandomPreset()
{
    // Random select button
    int SelEff[10];
    int numEff = (int) (RND * 6) + 1;
 
    S_new->do_callback();

    SelEff[0] = (int) (RND * C_NUMBER_EFFECTS);

    for (int i = 1; i < 10; i++)
    {
        int l = 0;
        while (l == 0)
        {
            SelEff[i] = (int) (RND * C_NUMBER_EFFECTS);
            for (int j = 0; j < i; j++)
            {
                if (SelEff[j] == SelEff[i])
                {
                    l = 0;
                    break;
                }
                else
                    l = 1;
            }
        }
    }


    for (int i = 0; i < C_NUMBER_ORDERED_EFFECTS; i++)
    {
        rkr->efx_order[i] = SelEff[i];
    }


    for (int i = 0; i < C_NUMBER_ORDERED_EFFECTS; i++)
    {
        switch (rkr->efx_order[i])
        {

            case EFX_EQ:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_EQ] = 1;
                else
                    rkr->EFX_Bypass[EFX_EQ] = 0;
                EQ->eq_activar->value(rkr->EFX_Bypass[EFX_EQ]);
                break;

            case EFX_COMPRESSOR:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_COMPRESSOR] = 1;
                else
                    rkr->EFX_Bypass[EFX_COMPRESSOR] = 0;
                COMPRESS->compress_activar->value(rkr->EFX_Bypass[EFX_COMPRESSOR]);
                break;

            case EFX_DISTORTION:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_DISTORTION] = 1;
                else
                    rkr->EFX_Bypass[EFX_DISTORTION] = 0;
                DIST->dist_activar->value(rkr->EFX_Bypass[EFX_DISTORTION]);
                break;

            case EFX_OVERDRIVE:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_OVERDRIVE] = 1;
                else
                    rkr->EFX_Bypass[EFX_OVERDRIVE] = 0;
                OVRD->ovrd_activar->value(rkr->EFX_Bypass[EFX_OVERDRIVE]);
                break;

            case EFX_ECHO:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_ECHO] = 1;
                else
                    rkr->EFX_Bypass[EFX_ECHO] = 0;
                ECHO->echo_activar->value(rkr->EFX_Bypass[EFX_ECHO]);
                break;

            case EFX_CHORUS:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_CHORUS] = 1;
                else
                    rkr->EFX_Bypass[EFX_CHORUS] = 0;
                CHORUS->chorus_activar->value(rkr->EFX_Bypass[EFX_CHORUS]);
                break;

            case EFX_PHASER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_PHASER] = 1;
                else
                    rkr->EFX_Bypass[EFX_PHASER] = 0;
                PHASER->phaser_activar->value(rkr->EFX_Bypass[EFX_PHASER]);
                break;

            case EFX_FLANGER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_FLANGER] = 1;
                else
                    rkr->EFX_Bypass[EFX_FLANGER] = 0;
                FLANGER->flanger_activar->value(rkr->EFX_Bypass[EFX_FLANGER]);
                break;

            case EFX_REVERB:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_REVERB] = 1;
                else
                    rkr->EFX_Bypass[EFX_REVERB] = 0;
                REVERB->reverb_activar->value(rkr->EFX_Bypass[EFX_REVERB]);
                break;

            case EFX_PARAMETRIC:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_PARAMETRIC] = 1;
                else
                    rkr->EFX_Bypass[EFX_PARAMETRIC] = 0;
                PEQ->eqp_activar->value(rkr->EFX_Bypass[EFX_PARAMETRIC]);
                break;

            case EFX_WAHWAH:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_WAHWAH] = 1;
                else
                    rkr->EFX_Bypass[EFX_WAHWAH] = 0;
                WAHWAH->WahWah_activar->value(rkr->EFX_Bypass[EFX_WAHWAH]);
                break;

            case EFX_ALIENWAH:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_ALIENWAH] = 1;
                else
                    rkr->EFX_Bypass[EFX_ALIENWAH] = 0;
                ALIENWAH->Alienwah_activar->value(rkr->EFX_Bypass[EFX_ALIENWAH]);
                break;

            case EFX_CABINET:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_CABINET] = 1;
                else
                    rkr->EFX_Bypass[EFX_CABINET] = 0;
                CABINET->Cabinet_activar->value(rkr->EFX_Bypass[EFX_CABINET]);
                break;

            case EFX_PAN:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_PAN] = 1;
                else
                    rkr->EFX_Bypass[EFX_PAN] = 0;
                PAN->pan_activar->value(rkr->EFX_Bypass[EFX_PAN]);
                break;

            case EFX_HARMONIZER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_HARMONIZER] = 1;
                else
                    rkr->EFX_Bypass[EFX_HARMONIZER] = 0;
                HAR->har_activar->value(rkr->EFX_Bypass[EFX_HARMONIZER]);
                break;

            case EFX_MUSICAL_DELAY:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_MUSICAL_DELAY] = 1;
                else
                    rkr->EFX_Bypass[EFX_MUSICAL_DELAY] = 0;
                MUSDELAY->musdelay_activar->value(rkr->EFX_Bypass[EFX_MUSICAL_DELAY]);
                break;

            case EFX_NOISEGATE:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_NOISEGATE] = 1;
                else
                    rkr->EFX_Bypass[EFX_NOISEGATE] = 0;
                GATE->gate_activar->value(rkr->EFX_Bypass[EFX_NOISEGATE]);
                break;

            case EFX_DERELICT:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_DERELICT] = 1;
                else
                    rkr->EFX_Bypass[EFX_DERELICT] = 0;
                DERELICT->derelict_activar->value(rkr->EFX_Bypass[EFX_DERELICT]);
                break;

            case EFX_ANALOG_PHASER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_ANALOG_PHASER] = 1;
                else
                    rkr->EFX_Bypass[EFX_ANALOG_PHASER] = 0;
                APHASER->aphaser_activar->value(rkr->EFX_Bypass[EFX_ANALOG_PHASER]);
                break;

            case EFX_VALVE:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_VALVE] = 1;
                else
                    rkr->EFX_Bypass[EFX_VALVE] = 0;
                VALVE->valve_activar->value(rkr->EFX_Bypass[EFX_VALVE]);
                break;

            case EFX_DUAL_FLANGE:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_DUAL_FLANGE] = 1;
                else
                    rkr->EFX_Bypass[EFX_DUAL_FLANGE] = 0;
                DFLANGE->dflange_activar->value(rkr->EFX_Bypass[EFX_DUAL_FLANGE]);
                break;

            case EFX_RING:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_RING] = 1;
                else
                    rkr->EFX_Bypass[EFX_RING] = 0;
                RING->ring_activar->value(rkr->EFX_Bypass[EFX_RING]);
                break;

            case EFX_EXCITER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_EXCITER] = 1;
                else
                    rkr->EFX_Bypass[EFX_EXCITER] = 0;
                EXCITER->exciter_activar->value(rkr->EFX_Bypass[EFX_EXCITER]);
                break;

            case EFX_DISTBAND:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_DISTBAND] = 1;
                else
                    rkr->EFX_Bypass[EFX_DISTBAND] = 0;
                DISTBAND->distband_activar->value(rkr->EFX_Bypass[EFX_DISTBAND]);
                break;

            case EFX_ARPIE:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_ARPIE] = 1;
                else
                    rkr->EFX_Bypass[EFX_ARPIE] = 0;
                ARPIE->arpie_activar->value(rkr->EFX_Bypass[EFX_ARPIE]);
                break;

            case EFX_EXPANDER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_EXPANDER] = 1;
                else
                    rkr->EFX_Bypass[EFX_EXPANDER] = 0;
                EXPANDER->expander_activar->value(rkr->EFX_Bypass[EFX_EXPANDER]);
                break;

            case EFX_SHUFFLE:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_SHUFFLE] = 1;
                else
                    rkr->EFX_Bypass[EFX_SHUFFLE] = 0;
                SHUFFLE->shuffle_activar->value(rkr->EFX_Bypass[EFX_SHUFFLE]);
                break;

            case EFX_SYNTHFILTER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_SYNTHFILTER] = 1;
                else
                    rkr->EFX_Bypass[EFX_SYNTHFILTER] = 0;
                SYNTHFILTER->synthfilter_activar->value(rkr->EFX_Bypass[EFX_SYNTHFILTER]);
                break;

            case EFX_VARYBAND:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_VARYBAND] = 1;
                else
                    rkr->EFX_Bypass[EFX_VARYBAND] = 0;
                VARYBAND->varyband_activar->value(rkr->EFX_Bypass[EFX_VARYBAND]);
                break;

            case EFX_CONVOLOTRON:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_CONVOLOTRON] = 1;
                else
                    rkr->EFX_Bypass[EFX_CONVOLOTRON] = 0;
                CONVOLOTRON->convo_activar->value(rkr->EFX_Bypass[EFX_CONVOLOTRON]);
                break;

            case EFX_LOOPER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_LOOPER] = 1;
                else
                    rkr->EFX_Bypass[EFX_LOOPER] = 0;
                LOOPER->looper_activar->value(rkr->EFX_Bypass[EFX_LOOPER]);
                update_looper();
                break;

            case EFX_MUTROMOJO:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_MUTROMOJO] = 1;
                else
                    rkr->EFX_Bypass[EFX_MUTROMOJO] = 0;
                MUTROMOJO->mutromojo_activar->value(rkr->EFX_Bypass[EFX_MUTROMOJO]);
                break;

            case EFX_ECHOVERSE:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_ECHOVERSE] = 1;
                else
                    rkr->EFX_Bypass[EFX_ECHOVERSE] = 0;
                ECHOVERSE->echoverse_activar->value(rkr->EFX_Bypass[EFX_ECHOVERSE]);
                break;

            case EFX_COILCRAFTER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_COILCRAFTER] = 1;
                else
                    rkr->EFX_Bypass[EFX_COILCRAFTER] = 0;
                COILCRAFTER->coil_activar->value(rkr->EFX_Bypass[EFX_COILCRAFTER]);
                break;

            case EFX_SHELFBOOST:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_SHELFBOOST] = 1;
                else
                    rkr->EFX_Bypass[EFX_SHELFBOOST] = 0;
                SHELFBOOST->shelf_activar->value(rkr->EFX_Bypass[EFX_SHELFBOOST]);
                break;

            case EFX_VOCODER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_VOCODER] = 1;
                else
                    rkr->EFX_Bypass[EFX_VOCODER] = 0;
                VOCODER->vo_activar->value(rkr->EFX_Bypass[EFX_VOCODER]);
                break;

            case EFX_SUSTAINER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_SUSTAINER] = 1;
                else
                    rkr->EFX_Bypass[EFX_SUSTAINER] = 0;
                SUSTAINER->sus_activar->value(rkr->EFX_Bypass[EFX_SUSTAINER]);
                break;

            case EFX_SEQUENCE:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_SEQUENCE] = 1;
                else
                    rkr->EFX_Bypass[EFX_SEQUENCE] = 0;
                SEQUENCE->seq_activar->value(rkr->EFX_Bypass[EFX_SEQUENCE]);
                break;

            case EFX_SHIFTER:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_SHIFTER] = 1;
                else
                    rkr->EFX_Bypass[EFX_SHIFTER] = 0;
                SHIFTER->shifter_activar->value(rkr->EFX_Bypass[EFX_SHIFTER]);
                break;

            case EFX_STOMPBOX:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_STOMPBOX] = 1;
                else
                    rkr->EFX_Bypass[EFX_STOMPBOX] = 0;
                STOMPBOX->stomp_activar->value(rkr->EFX_Bypass[EFX_STOMPBOX]);
                break;

            case EFX_REVERBTRON:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_REVERBTRON] = 1;
                else
                    rkr->EFX_Bypass[EFX_REVERBTRON] = 0;
                REVERBTRON->revtron_activar->value(rkr->EFX_Bypass[EFX_REVERBTRON]);
                break;

            case EFX_ECHOTRON:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_ECHOTRON] = 1;
                else
                    rkr->EFX_Bypass[EFX_ECHOTRON] = 0;
                ECHOTRON->echotron_activar->value(rkr->EFX_Bypass[EFX_ECHOTRON]);
                break;

            case EFX_STEREOHARM:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_STEREOHARM] = 1;
                else
                    rkr->EFX_Bypass[EFX_STEREOHARM] = 0;
                SHAR->shar_activar->value(rkr->EFX_Bypass[EFX_STEREOHARM]);
                break;

            case EFX_COMPBAND:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_COMPBAND] = 1;
                else
                    rkr->EFX_Bypass[EFX_COMPBAND] = 0;
                COMPBAND->cband_activar->value(rkr->EFX_Bypass[EFX_COMPBAND]);
                break;

            case EFX_OPTICALTREM:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_OPTICALTREM] = 1;
                else
                    rkr->EFX_Bypass[EFX_OPTICALTREM] = 0;
                OTREM->otrem_activar->value(rkr->EFX_Bypass[EFX_OPTICALTREM]);
                break;

            case EFX_VIBE:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_VIBE] = 1;
                else
                    rkr->EFX_Bypass[EFX_VIBE] = 0;
                VIBE->vibe_activar->value(rkr->EFX_Bypass[EFX_VIBE]);
                break;

            case EFX_INFINITY:
                if (i < numEff)
                    rkr->EFX_Bypass[EFX_INFINITY] = 1;
                else
                    rkr->EFX_Bypass[EFX_INFINITY] = 0;
                INFINIT->infinity_activar->value(rkr->EFX_Bypass[EFX_INFINITY]);
                break;
        }

        Fl_Widget *w = FindWidget(SelEff[i]);
        Fl_Choice *s = (Fl_Choice *) w;
        long long k = (long long) s->user_data();
        int Esel = (int) (RND * s->size());
        s->value(Esel);
        s->do_callback(w, k);
    }

    FillML();
    Prepare_Order();
    Put_Loaded();

    ActivarGeneral->value(1);
    ActivarGeneral->do_callback();
}

void RKRGUI::drag_effect()
{
    // drag effect
    if ((drag != 1000) && (Fl::event_button1() == 0))
    {
        Fl_Widget *w = Fl::belowmouse();
        if (w != NULL)
        {
            long long k = (long long) w->user_data();

            if ((k < 770) || (k > 779))
            {
                return;
            }

            // w->user_data() is Label in range 770 to 779
            // which is checked above, so this is valid range (10) for
            // rkr->efx_order[C_NUMBER_ORDERED_EFFECTS] used by Prep_Reorden(source, dest)
            if (drag != (int) k - 770)
            {
                Prep_Reorden(drag, (int) k - 770);
            }
        }
        drag = 1000;
    }
}

int RKRGUI::AddBankName(const char *name)
{
    //AddBankName
    struct list_element *elem;

    elem = (struct list_element *) calloc(1, sizeof (struct list_element));

    if (elem == NULL)
    {
        return 0;
    }

    elem->name = (char *) calloc(1, 256);

    if (elem->name == NULL)
    {
        free(elem);
        return 0;
    }

    strncpy(elem->name, name, 255);

    if (mBankNameList == NULL)
    {
        mBankNameList = elem;
        insque(elem, NULL);
    }
    else
    {
        insque(elem, mBankNameListTail);
    }

    mBankNameListTail = elem;
    return 1;
}

void RKRGUI::ClearBankNames()
{
    // clearbanknames
    struct list_element *elem;
    while (mBankNameList != NULL)
    {
        elem = mBankNameList;
        mBankNameList = elem->forward;
        free(elem->name);
        remque(elem);
        free(elem);
    }
}

char* RKRGUI::get_bank_file()
{
    // get bank file
    char *filename;
    is_modified();
    filename = fl_file_chooser("Load Bank File:", "(*.rkrb)", NULL, 0);

    if (filename == NULL)
        return 0;

    filename = fl_filename_setext(filename, ".rkrb");
    int ok = rkr->load_bank(filename);

    if (ok)
    {
        BankWin_Label(filename);
        Put_Loaded_Bank();
        AddBankName(filename);
    }

    return filename;
}

void RKRGUI::set_save_file()
{
    // set bank file save name
    char *filename;

#define EXT ".rkrb"
    filename = fl_file_chooser("Save Bank File:", "(*" EXT")", rkr->Bank_Saved, 0);

    if (filename == NULL)
        return;

    filename = fl_filename_setext(filename, EXT);
#undef EXT
    int ok = rkr->save_bank(filename);
    if (ok)
    {
        strcpy(rkr->Bank_Saved, filename);
        BankWin_Label(filename);
    }
}

void RKRGUI::set_bank_made(int a_made)
{
    // set bank made
    made = a_made;
}

int RKRGUI::get_bank_made()
{
    // get bank made
    return made;
}

void RKRGUI::Load_Midi_Program_Change_Table()
{
    // load table
    char *filename;
    filename = fl_file_chooser("Load MIDI Table:", "(*.rmt)", NULL, 0);

    if (filename == NULL)
        return;

    filename = fl_filename_setext(filename, ".rmt");
    rkr->load_MIDI_table(filename);
    Settings->Put_MidiTable();
}

void RKRGUI::Save_Midi_Program_Change_Table()
{
    // save program change table
    char *filename;
#define EXT ".rmt"
    filename = fl_file_chooser("Save MIDI Table:", "(*" EXT")", "", 0);

    if (filename == NULL)
        return;

    filename = fl_filename_setext(filename, EXT);
#undef EXT
    rkr->save_MIDI_table(filename);
}
