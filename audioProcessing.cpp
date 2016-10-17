/************************************************************
  audioProcessing.cpp - Library of digital audio functions.
  Created by Ronald B Taylor, October 15, 2016.
  Released into the public domain.
*************************************************************/

#include <Arduino.h>
#include <audioProcessing.h>

audioProcessing::audioProcessing(byte sample[],int bufferSize){}

audioProcessing::~audioProcessing(){}

void audioProcessing::noiseReduction(byte sample[],int bufferSize)
{
		//Rectify samples	
		int sampleCounter = 0; 
		int sampleDelta[bufferSize];
		while (sampleCounter < (bufferSize))
      		{     
		
		sampleDelta[sampleCounter] = abs(sample[sampleCounter] -128);
		sampleCounter = sampleCounter + 1;
		}
		
	

		
		sampleCounter = 0;   
  
      		while (sampleCounter < (bufferSize))
      		{     
		if (sampleDelta[sampleCounter] < 10 )
		sample[sampleCounter] = 128;
		if (sampleDelta[sampleCounter] >120)
		sample[sampleCounter] = 128; 
		
		sampleCounter = sampleCounter + 1;
		}

		
}

void audioProcessing::popRemoval(byte sample[], int bufferSize)
{
/************************************** Pre Process*/      
      int sampleCounter = 0;   
      while (sampleCounter < (bufferSize))
      {
     	if (sample[sampleCounter] < 45 or sample[sampleCounter]> 210){sample[sampleCounter] = 128;}
     
     	sampleCounter = sampleCounter +1; 
     }
/****************************************************/
  
      sampleCounter = 26;   

      while (sampleCounter < (bufferSize-201))
      {     
        
        if (abs(sample[sampleCounter]-sample[sampleCounter + 1])> 40)
                     {
                       int U = sampleCounter -25;
                       int W = U + 175;
                       while (U < W)
                       {
                        sample[U] = 128;
                        U=U+1;
                     
                        sampleCounter = sampleCounter +1;
                       }
                                        
                     }
        
        
        sampleCounter = sampleCounter +1;  
     
      } 

 // Dynamic range limitor for spike limiting and autogain improvement
          sampleCounter = 0;   
  
      while (sampleCounter < bufferSize-10)
      {     
       
        if (sample[sampleCounter] < 55) {
          sample[sampleCounter]=128;
          sample[sampleCounter-1]=128;
          sample[sampleCounter-2]=128;
          sample[sampleCounter-4]=128;
          sample[sampleCounter-5]=128;
          sample[sampleCounter-6]=128;
          }
        if (sample[sampleCounter] > 220) {sample[sampleCounter] = 128;} 
        
        sampleCounter = sampleCounter +1;  
      } 

	sampleCounter = 0;  

      while (sampleCounter < bufferSize-150)
      {     
        
        if (abs(sample[sampleCounter]-sample[sampleCounter + 1])> 40)
                     {
                    
                     sample[sampleCounter+1] = sample[sampleCounter+1]+67;
                    sample[sampleCounter+2] = sample[sampleCounter+2]+61;
                   sample[sampleCounter+3] = sample[sampleCounter+3]+55;
                     sample[sampleCounter+4] = sample[sampleCounter+4]+50;
                       sample[sampleCounter+5] = sample[sampleCounter+5]+45;
                        sample[sampleCounter+6] = sample[sampleCounter+6]+39;
                       sample[sampleCounter+7] = sample[sampleCounter+7]+34;
                       sample[sampleCounter+8] = sample[sampleCounter+8]+30;
                        sample[sampleCounter+9] = sample[sampleCounter+9]+26;
                         sample[sampleCounter+10] = sample[sampleCounter+10]+23;
                          sample[sampleCounter+11] = sample[sampleCounter+11]+20;
                           sample[sampleCounter+12] = sample[sampleCounter+12]+17;
                        sample[sampleCounter+13] = sample[sampleCounter+13]+13;
                         sample[sampleCounter+14] = sample[sampleCounter+14]+11;
                          sample[sampleCounter+15] = sample[sampleCounter+15]+8;
                           sample[sampleCounter+16] = sample[sampleCounter+16]+5;
                            sample[sampleCounter+17] = sample[sampleCounter+17]+-1;
                             sample[sampleCounter+18] = sample[sampleCounter+18]-2;
                              sample[sampleCounter+19] = sample[sampleCounter+19]-3;
                               sample[sampleCounter+20] = sample[sampleCounter+20]-4;
                                sample[sampleCounter+21] = sample[sampleCounter+21]-5;
                                 sample[sampleCounter+22] = sample[sampleCounter+22]-6;
                                   sample[sampleCounter+23] = sample[sampleCounter+23]-10;
                         sample[sampleCounter+24] = sample[sampleCounter+24]-11;
                          sample[sampleCounter+25] = sample[sampleCounter+25]-12;
                           sample[sampleCounter+26] = sample[sampleCounter+26]-13;
                            sample[sampleCounter+27] = sample[sampleCounter+27]-14;
                             sample[sampleCounter+28] = sample[sampleCounter+28]-15;
                              sample[sampleCounter+29] = sample[sampleCounter+29]-16;
                               sample[sampleCounter+30] = sample[sampleCounter+30]-17;
                                sample[sampleCounter+31] = sample[sampleCounter+31]-18;
                                 sample[sampleCounter+32] = sample[sampleCounter+32]-19;
                         sample[sampleCounter+33] = sample[sampleCounter+33]-20;
                          sample[sampleCounter+34] = sample[sampleCounter+34]-21;
                           sample[sampleCounter+35] = sample[sampleCounter+35]-22;
                            sample[sampleCounter+36] = sample[sampleCounter+36]-23;
                             sample[sampleCounter+37] = sample[sampleCounter+37]-24;
                              sample[sampleCounter+38] = sample[sampleCounter+38]-25;
                               sample[sampleCounter+39] = sample[sampleCounter+39]-26;
                                sample[sampleCounter+40] = sample[sampleCounter+40]-27;
                                 sample[sampleCounter+41] = sample[sampleCounter+41]-28;
                    sample[sampleCounter+42] = sample[sampleCounter+42]-29;
                          sample[sampleCounter+43] = sample[sampleCounter+43]-30;
                           sample[sampleCounter+44] = sample[sampleCounter+44]-30;
                           sample[sampleCounter+45] = sample[sampleCounter+45]-30;
                            sample[sampleCounter+46] = sample[sampleCounter+46]-30;
                             sample[sampleCounter+47] = sample[sampleCounter+47]-30;
                              sample[sampleCounter+48] = sample[sampleCounter+48]-30;
                               sample[sampleCounter+49] = sample[sampleCounter+49]-30;
                                sample[sampleCounter+50] = sample[sampleCounter+50]-30;
                                 sample[sampleCounter+51] = sample[sampleCounter+51]-30;  

                            sample[sampleCounter+52] = sample[sampleCounter+52]-30;
                          sample[sampleCounter+53] = sample[sampleCounter+53]-30;
                           sample[sampleCounter+54] = sample[sampleCounter+54]-30;
                           sample[sampleCounter+55] = sample[sampleCounter+55]-30;
                            sample[sampleCounter+56] = sample[sampleCounter+56]-29;
                             sample[sampleCounter+57] = sample[sampleCounter+57]-29;
                              sample[sampleCounter+58] = sample[sampleCounter+58]-29;
                               sample[sampleCounter+59] = sample[sampleCounter+59]-28;
                                sample[sampleCounter+60] = sample[sampleCounter+60]-28;
                                 sample[sampleCounter+61] = sample[sampleCounter+61]-28;  

                         sample[sampleCounter+62] = sample[sampleCounter+62]-28;
                          sample[sampleCounter+63] = sample[sampleCounter+63]-27;
                           sample[sampleCounter+64] = sample[sampleCounter+64]-27;
                            sample[sampleCounter+65] = sample[sampleCounter+65]-27;
                            sample[sampleCounter+66] = sample[sampleCounter+66]-27;
                             sample[sampleCounter+67] = sample[sampleCounter+67]-26;
                              sample[sampleCounter+68] = sample[sampleCounter+68]-26;
                               sample[sampleCounter+69] = sample[sampleCounter+69]-26;
                                sample[sampleCounter+70] = sample[sampleCounter+70]-26;
                                 sample[sampleCounter+71] = sample[sampleCounter+71]-25;  


                         sample[sampleCounter+72] = sample[sampleCounter+72]-25;
                          sample[sampleCounter+73] = sample[sampleCounter+73]-25;
                           sample[sampleCounter+74] = sample[sampleCounter+74]-25;
                           sample[sampleCounter+75] = sample[sampleCounter+75]-24;
                            sample[sampleCounter+76] = sample[sampleCounter+76]-24;
                             sample[sampleCounter+77] = sample[sampleCounter+77]-24;
                              sample[sampleCounter+78] = sample[sampleCounter+78]-24;
                               sample[sampleCounter+79] = sample[sampleCounter+79]-23;
                                sample[sampleCounter+80] = sample[sampleCounter+80]-23;
                                 sample[sampleCounter+81] = sample[sampleCounter+81]-23;  
                        
                        sample[sampleCounter+82] = sample[sampleCounter+82]-22;
                          sample[sampleCounter+83] = sample[sampleCounter+83]-22;
                           sample[sampleCounter+84] = sample[sampleCounter+84]-22;
                            sample[sampleCounter+85] = sample[sampleCounter+85]-21;
                            sample[sampleCounter+86] = sample[sampleCounter+86]-21;
                             sample[sampleCounter+87] = sample[sampleCounter+87]-21;
                              sample[sampleCounter+88] = sample[sampleCounter+88]-20;
                               sample[sampleCounter+89] = sample[sampleCounter+89]-20;
                                sample[sampleCounter+90] = sample[sampleCounter+90]-20;
                                 sample[sampleCounter+91] = sample[sampleCounter+91]-19;  


                         sample[sampleCounter+92] = sample[sampleCounter+92]-19;
                          sample[sampleCounter+93] = sample[sampleCounter+93]-19;
                           sample[sampleCounter+94] = sample[sampleCounter+94]-18;
                           sample[sampleCounter+95] = sample[sampleCounter+95]-18;
                            sample[sampleCounter+96] = sample[sampleCounter+96]-17;
                             sample[sampleCounter+97] = sample[sampleCounter+97]-17;
                              sample[sampleCounter+98] = sample[sampleCounter+98]-16;
                               sample[sampleCounter+99] = sample[sampleCounter+99]-16;
                                sample[sampleCounter+100] = sample[sampleCounter+100]-15;
                                 sample[sampleCounter+101] = sample[sampleCounter+101]-15; 

                    sample[sampleCounter+102] = sample[sampleCounter+102]-14;
                          sample[sampleCounter+103] = sample[sampleCounter+103]-14;
                           sample[sampleCounter+104] = sample[sampleCounter+104]-13;
                            sample[sampleCounter+105] = sample[sampleCounter+105]-13;
                            sample[sampleCounter+106] = sample[sampleCounter+106]-12;
                             sample[sampleCounter+107] = sample[sampleCounter+107]-12;
                              sample[sampleCounter+108] = sample[sampleCounter+108]-11;
                               sample[sampleCounter+109] = sample[sampleCounter+109]-11;
                                sample[sampleCounter+110] = sample[sampleCounter+110]-12;
                                 sample[sampleCounter+111] = sample[sampleCounter+111]-12;  


                         sample[sampleCounter+112] = sample[sampleCounter+112]-11;
                          sample[sampleCounter+113] = sample[sampleCounter+113]-11;
                           sample[sampleCounter+114] = sample[sampleCounter+114]-10;
                           sample[sampleCounter+115] = sample[sampleCounter+115]-10;
                            sample[sampleCounter+116] = sample[sampleCounter+116]-9;
                             sample[sampleCounter+117] = sample[sampleCounter+117]-9;
                              sample[sampleCounter+118] = sample[sampleCounter+118]-8;
                               sample[sampleCounter+119] = sample[sampleCounter+119]-8;
                                sample[sampleCounter+120] = sample[sampleCounter+120]-7;
                                 sample[sampleCounter+121] = sample[sampleCounter+121]-7;  

                   sample[sampleCounter+122] = sample[sampleCounter+122]-14;
                          sample[sampleCounter+123] = sample[sampleCounter+123]-6;
                           sample[sampleCounter+124] = sample[sampleCounter+124]-6;
                            sample[sampleCounter+125] = sample[sampleCounter+125]-5;
                            sample[sampleCounter+126] = sample[sampleCounter+126]-5;
                             sample[sampleCounter+127] = sample[sampleCounter+127]-4;
                              sample[sampleCounter+128] = sample[sampleCounter+128]-4;
                               sample[sampleCounter+129] = sample[sampleCounter+129]-3;
                                sample[sampleCounter+130] = sample[sampleCounter+130]-3;
                                 sample[sampleCounter+131] = sample[sampleCounter+131]-2;  


                         sample[sampleCounter+132] = sample[sampleCounter+132]-2;
                          sample[sampleCounter+133] = sample[sampleCounter+133]-1;
                           sample[sampleCounter+134] = sample[sampleCounter+134]-1;
                           sample[sampleCounter+135] = sample[sampleCounter+135]-1;
                            sample[sampleCounter+136] = sample[sampleCounter+136]-1;
                             sample[sampleCounter+137] = sample[sampleCounter+137]-1;
                              sample[sampleCounter+138] = sample[sampleCounter+138]-1;
                               sample[sampleCounter+139] = sample[sampleCounter+139]-1;
                                sample[sampleCounter+140] = sample[sampleCounter+140]-1;
                                 sample[sampleCounter+141] = sample[sampleCounter+141]-1;  
                     }
        
        sampleCounter = sampleCounter +1;  
      } 

}

void audioProcessing::expansion(byte sample[], int bufferSize)
{
int sampleCounter = 0;
  byte min = 255;
  byte max = 0;
  float mfactor = 0;
  float diff = 0; 
   
  while (sampleCounter < bufferSize)
      {
        
        if(sample[sampleCounter] < min ) { min = sample[sampleCounter];}
        if(sample[sampleCounter] > min) {max = sample[sampleCounter];}
       
        sampleCounter = sampleCounter +1;  
        
      }

      mfactor = ((max - min)/255);
      
      diff = max-min;
      mfactor = (1/(diff/255))/2;
     
      min = min - (min - 1);
      max = ((diff)* mfactor);
 
// Dynamic range expansion

   sampleCounter = 0;   
  
      while (sampleCounter < bufferSize)
      {     
        if (sample[sampleCounter] > 128) {sample[sampleCounter]=((sample[sampleCounter]* mfactor));}
        if (sample[sampleCounter] < 128) {sample[sampleCounter]=((sample[sampleCounter]/mfactor));}
        sampleCounter = sampleCounter +1;  
      }
 }
