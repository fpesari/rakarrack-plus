/*
  ZynAddSubFX - a software synthesizer

  Distorsion.h - Distorsion Effect
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  Modified for rakarrack by Josep Andreu & Hernan Ordiales & Ryan Billing

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

/* 
 * File:   Overdrive.C
 * Author: sspresto
 * 
 * Created on January 26, 2021, 8:16 AM
 */

#include "Overdrive.h"

Overdrive::Overdrive(int wave_res, int wave_upq, int wave_dnq, double samplerate, uint32_t intermediate_bufsize):
    Distorsion(wave_res, wave_upq, wave_dnq, samplerate, intermediate_bufsize)
{
}

void
Overdrive::setpreset (int npreset)
{
    const int PRESET_SIZE = C_OVERDRIVE_PARAMETERS;
    const int NUM_PRESETS = 2;
    int pdata[MAX_PDATA_SIZE];
    int presets[NUM_PRESETS][PRESET_SIZE] = {
        //Overdrive 1
        {84, 64, 35, 56, 40, 0, 0, 6703, 21, 0, 0, 0, 0},
        //Overdrive 2
        {85, 64, 35, 29, 45, 1, 0, 25040, 21, 0, 0, 0, 0}
        
    };

    // (npreset > NUM_PRESETS - 1) means user defined (Insert) presets for Overdrive
    if (npreset > NUM_PRESETS - 1)
    {
        Fpre->ReadPreset(EFX_OVERDRIVE, npreset - NUM_PRESETS + 1, pdata);
        
        for (int n = 0; n < PRESET_SIZE; n++)
            changepar(n, pdata[n]);
    }
    else    // No user defined presets so just set it
    {
        for (int n = 0; n < PRESET_SIZE; n++)
            changepar(n, presets[npreset][n]);
    }
    
    Ppreset = npreset;
    cleanup();
}
