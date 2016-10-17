/*******************************************************
audioProcessing.h
This is my library for digitized audio processing.

Written by Ronald B Taylor - illuminatics
*******************************************************/


#ifndef  AUDIOPROCESSING_H
#define AUDIOPROCESSING_H

#include <Arduino.h>

class audioProcessing 
{
	public :
		audioProcessing(byte*,int);
		~audioProcessing();
		void noiseReduction(byte sample[],int bufferSize);
		void popRemoval(byte sample[],int bufferSize);
		void expansion(byte sample[],int bufferSize);
		
};

#endif
		