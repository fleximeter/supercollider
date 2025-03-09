#include <cassert>
#include "SC_PlugIn.h"

static InterfaceTable *ft;

struct MPhasor : public Unit {
    double mLevel;
    float m_previn;
};

void MPhasor_Ctor(MPhasor* unit);
void MPhasor_next_kk(MPhasor* unit, int inNumSamples);
void MPhasor_next_ak(MPhasor* unit, int inNumSamples);
void MPhasor_next_aa(MPhasor* unit, int inNumSamples);


void MPhasor_Ctor(MPhasor* unit) {
    if (unit->mCalcRate == calc_FullRate) {
        if (INRATE(0) == calc_FullRate) {
            if (INRATE(1) == calc_FullRate) {
                SETCALC(MPhasor_next_aa);
            } else {
                SETCALC(MPhasor_next_ak);
            }
        } else {
            SETCALC(MPhasor_next_kk);
        }
    } else {
        SETCALC(MPhasor_next_ak);
    }

    unit->m_previn = IN0(0);
    OUT0(0) = unit->mLevel = IN0(2);
}

void MPhasor_next_kk(MPhasor* unit, int inNumSamples) {
    float* out = OUT(0);

    float in = IN0(0);
    double rate = IN0(1);
    double start = IN0(2);
    double end = IN0(3);
    float resetPos = IN0(4);

    float previn = unit->m_previn;
    double level = unit->mLevel;

    if (previn <= 0.f && in > 0.f) {
        level = resetPos;
    }

    for (int xxn = 0; xxn < inNumSamples; xxn++) {
        level = sc_wrap(level, start, end);
        out[xxn] = level;
        level += rate;
    }

    unit->m_previn = in;
    unit->mLevel = level;
}

void MPhasor_next_ak(MPhasor* unit, int inNumSamples) {
    float* out = OUT(0);

    float* in = IN(0);
    double rate = IN0(1);
    double start = IN0(2);
    double end = IN0(3);
    float resetPos = IN0(4);

    float previn = unit->m_previn;
    double level = unit->mLevel;

    for (int xxn = 0; xxn < inNumSamples; xxn++) {
        float curin = in[xxn];
        if (previn <= 0.f && curin > 0.f) {
            float frac = 1.f - previn / (curin - previn);
            level = resetPos + frac * rate;
        }
        out[xxn] = level;
        level += rate;
        level = sc_wrap(level, start, end);
        previn = curin;
    }

    unit->m_previn = previn;
    unit->mLevel = level;
}

void MPhasor_next_aa(MPhasor* unit, int inNumSamples) {
    float* out = OUT(0);
    float* in = IN(0);
    float* rate = IN(1);
    double start = IN0(2);
    double end = IN0(3);
    float resetPos = IN0(4);

    float previn = unit->m_previn;
    double level = unit->mLevel;

    for (int xxn = 0; xxn < inNumSamples; xxn++) {
        float curin = in[xxn];
        double zrate = rate[xxn];
        if (previn <= 0.f && curin > 0.f) {
            float frac = 1.f - previn / (curin - previn);
            level = resetPos + frac * zrate;
        }
        out[xxn] = level;
        level += zrate;
        level = sc_wrap(level, start, end); previn = curin;                                                                                                     \
    }

    unit->m_previn = previn;
    unit->mLevel = level;
}

PluginLoad(MPhasor) {
    ft = inTable;
    DefineSimpleUnit(MPhasor);
}
