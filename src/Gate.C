// Based in gate_1410.c LADSPA Swh-plugins


/*
  rakarrack - a guitar effects software

 Gate.C  -  Noise Gate Effect
 Based on Steve Harris LADSPA gate.

  Copyright (C) 2008 Josep Andreu
  Author: Josep Andreu

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License
 (version2)  along with this program; if not, write to the Free Software
 Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

 */

#include <math.h>
#include "Gate.h"

Gate::Gate(double sample_rate, uint32_t intermediate_bufsize) :
    Effect(sample_rate, intermediate_bufsize),
    Pthreshold(),
    Pattack(),
    Ohold(),
    Pdecay(),
    Prange(),
    Plpf(),
    Phpf(),
    Phold(),
    PERIOD(intermediate_bufsize),
    fSAMPLE_RATE(sample_rate),
    hold_count(),
    state(CLOSED),
    range(),
    cut(),
    t_level(),
    a_rate(),
    d_rate(),
    env(),
    gate(),
    fs(sample_rate),
    hold(),
    interpbuf(NULL),
    lpfl(NULL),
    lpfr(NULL),
    hpfl(NULL),
    hpfr(NULL)
{
    initialize();
    setpreset(0);
}

Gate::~Gate()
{
    clear_initialize();
}

void
Gate::cleanup()
{
    lpfl->cleanup();
    hpfl->cleanup();
    lpfr->cleanup();
    hpfr->cleanup();
}

#ifdef LV2_SUPPORT
void
Gate::lv2_update_params(uint32_t period)
{
    if (period > PERIOD) // only re-initialize if period > intermediate_bufsize of declaration
    {
        PERIOD = period;
        clear_initialize();
        initialize();
        setlpf(Plpf);
        sethpf(Phpf);
    }
    else
    {
        PERIOD = period;
    }
}
#endif // LV2

void
Gate::initialize()
{
    interpbuf = new float[PERIOD];

    lpfl = new AnalogFilter(2, 22000, 1, 0, fSAMPLE_RATE, interpbuf);
    lpfr = new AnalogFilter(2, 22000, 1, 0, fSAMPLE_RATE, interpbuf);
    hpfl = new AnalogFilter(3, 20, 1, 0, fSAMPLE_RATE, interpbuf);
    hpfr = new AnalogFilter(3, 20, 1, 0, fSAMPLE_RATE, interpbuf);
}

void
Gate::clear_initialize()
{
    delete[] interpbuf;
    delete lpfl;
    delete lpfr;
    delete hpfl;
    delete hpfr;
}

void
Gate::setlpf(int value)
{
    Plpf = value;
    float fr = (float) Plpf;
    lpfl->setfreq(fr);
    lpfr->setfreq(fr);
}

void
Gate::sethpf(int value)
{
    Phpf = value;
    float fr = (float) Phpf;
    hpfl->setfreq(fr);
    hpfr->setfreq(fr);
}

void
Gate::out(float *efxoutl, float *efxoutr)
{
    lpfl->filterout(efxoutl, PERIOD);
    hpfl->filterout(efxoutl, PERIOD);
    lpfr->filterout(efxoutr, PERIOD);
    hpfr->filterout(efxoutr, PERIOD);

    for (unsigned i = 0; i < PERIOD; i++)
    {
        float sum = fabsf(efxoutl[i]) + fabsf(efxoutr[i]);

        if (sum > env)
        {
            env = sum;
        }
        else
        {
            env = sum * ENV_TR + env * (1.0f - ENV_TR);
        }

        if (state == CLOSED)
        {
            if (env >= t_level)
            {
                state = OPENING;
            }
        }
        else if (state == OPENING)
        {
            gate += a_rate;
            if (gate >= 1.0)
            {
                gate = 1.0f;
                state = OPEN;
                hold_count = lrintf(hold * fs * 0.001f);
            }
        }
        else if (state == OPEN)
        {
            if (hold_count <= 0)
            {
                if (env < t_level)
                {
                    state = CLOSING;
                }
            }
            else
                hold_count--;

        }
        else if (state == CLOSING)
        {
            gate -= d_rate;
            
            if (env >= t_level)
            {
                state = OPENING;
            }
            else if (gate <= 0.0)
            {
                gate = 0.0;
                state = CLOSED;
            }
        }

        efxoutl[i] *= (cut * (1.0f - gate) + gate);
        efxoutr[i] *= (cut * (1.0f - gate) + gate);
    }
}

void
Gate::setpreset(int npreset)
{
    const int PRESET_SIZE = C_GATE_PARAMETERS;
    const int NUM_PRESETS = 3;
    int pdata[MAX_PDATA_SIZE];
    int presets[NUM_PRESETS][PRESET_SIZE] = {
        //0
        {0, 0, 1, 2, 6703, 76, 2},
        //-10
        {0, -10, 1, 2, 6703, 76, 2},
        //-20
        {0, -20, 1, 2, 6703, 76, 2}
    };

    if (npreset > NUM_PRESETS - 1)
    {

        Fpre->ReadPreset(EFX_NOISEGATE, npreset - NUM_PRESETS + 1, pdata);
        
        for (int n = 0; n < PRESET_SIZE; n++)
            changepar(n, pdata[n]);
    }
    else
    {
        for (int n = 0; n < PRESET_SIZE; n++)
            changepar(n, presets[npreset][n]);
    }
}

void
Gate::changepar(int npar, int value)
{
    switch (npar)
    {
    case Gate_Threshold:
        Pthreshold = value;
        t_level = dB2rap((float) Pthreshold);
        break;
    case Gate_Range:
        Prange = value;
        cut = dB2rap((float) Prange);
        break;
    case Gate_Attack:
        Pattack = value;
        a_rate = 1000.0f / ((float) Pattack * fs);
        break;
    case Gate_Release:
        Pdecay = value;
        d_rate = 1000.0f / ((float) Pdecay * fs);
        break;
    case Gate_LPF:
        setlpf(value);
        break;
    case Gate_HPF:
        sethpf(value);
        break;
    case Gate_Hold:
        Phold = value;
        hold = (float) Phold;
        break;
    default:
        return;
    }
}

int
Gate::getpar(int npar)
{
    switch (npar)
    {
    case Gate_Threshold:
        return (Pthreshold);
        break;
    case Gate_Range:
        return (Prange);
        break;
    case Gate_Attack:
        return (Pattack);
        break;
    case Gate_Release:
        return (Pdecay);
        break;
    case Gate_LPF:
        return (Plpf);
        break;
    case Gate_HPF:
        return (Phpf);
        break;
    case Gate_Hold:
        return (Phold);
        break;
    }

    return (0);
}
