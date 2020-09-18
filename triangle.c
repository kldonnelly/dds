/*
 * triangle.c
 *
 *  Created on: 12/09/2020
 *      Author: kevin
 */
#include "idds.h"


/* Math definitions */

#define SAMPLE_RATE DDS_SAMPLE_RATE
/* Q16.16 */
#define FRAC 16
#define Q16(X) (X * (float)(1 << FRAC))


void Generate_Triangle(IWaveForm *this)
{

    float length = TABLE_SIZE;
    float amplitude = 2;
    float delta = amplitude * 2 / length;
    for (int i = 0; i < TABLE_SIZE / 2; i++)
    {
    	this->table[i] = -(amplitude / 2) + delta * (float)i;
    }
    for (int i = TABLE_SIZE / 2; i < TABLE_SIZE; i++)
    {
    	this->table[i] = (amplitude / 2) - delta * ((float)i - length / 2);
    }
    /* set the last byte equal to first for interpolation */
    this->table[TABLE_SIZE] = this->table[0];
    this->phaseAccumulator_L = 0;
    this->phaseAccumulator_R = 0;
}
