#ifndef FILTER_H
#define FILTER_H

//#include "stm32f10x.h"
#include "structconfig.h"

float Low_Filter(float value);
void SortAver_Filter(float value,float *filter,uint8_t N);
void  SortAver_Filter1(float value,float *filter,uint8_t n);
void  SortAver_FilterXYZ(INT16_XYZ *acc,FLOAT_XYZ *Acc_filt,uint8_t N);
void Aver_FilterXYZ6(INT16_XYZ *acc,INT16_XYZ *gry,FLOAT_XYZ *Acc_filt,FLOAT_XYZ *Gry_filt,uint8_t N);
void Aver_FilterXYZ(INT16_XYZ *acc,FLOAT_XYZ *Acc_filt,uint8_t N);
void Aver_Filter(float data,float *filt_data,uint8_t n);
void Aver_Filter1(float data,float *filt_data,uint8_t n);
void presssureFilter(float* in, float* out);

void LPF2pSetCutoffFreq_1(float sample_freq, float cutoff_freq);
float LPF2pApply_1(float sample);
#endif
