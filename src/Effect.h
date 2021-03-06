/*
  ZynAddSubFX - a software synthesizer

  Effect.h - this class is inherited by the all effects(Reverb, Echo, ..)
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

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

#ifndef EFFECT_H
#define EFFECT_H

#include "global.h"


class Effect
{
public:

    Effect (double, uint32_t) :
        outvolume(0.5f), Ppreset(0), Fpre(NULL) {};

    // Distortion, Overdrive, Derelict, DistBand, StompBox, Convolotron, Reverbtron
    Effect (int, int, int, double, uint32_t) :
        outvolume(0.5f), Ppreset(0), Fpre(NULL) {};

    // Harmonizer, Sequence, Shifter, StereoHarm
    Effect (long int, int, int, int, double, uint32_t) :
        outvolume(0.5f), Ppreset(0), Fpre(NULL) {};

    // Looper
    Effect (float, double, uint32_t) :
        outvolume(0.5f), Ppreset(0), Fpre(NULL) {};

    // Vocoder
    Effect (float *, int, int, int, int, double, uint32_t) :
        outvolume(0.5f), Ppreset(0), Fpre(NULL) {};

    virtual ~ Effect () {};
    
    virtual void setpreset (int) = 0;
    virtual void changepar (int, int) = 0;
    virtual int getpar (int) = 0;
    virtual void out (float *, float *) = 0;
    virtual void cleanup () = 0;
    virtual int get_number_efx_parameters () = 0;

#ifdef LV2_SUPPORT
    virtual void lv2_update_params(uint32_t)  = 0;
#endif // LV2

    // The volume of the effect and is public.
    float outvolume;
    int Ppreset;
    class FPreset *Fpre;

};

#endif
