// ******************************************************************************************* //
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "keypad.h"
#include "lcd.h"

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings. 
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to 
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & 
		 BKBUG_ON & COE_ON & ICS_PGx1 & 
		 FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
		 IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //

// Varible used to indicate that the current configuration of the keypad has been changed,
// and the KeypadScan() function needs to be called.

volatile char scanKeypad;

// ******************************************************************************************* //

int main(void)
{
 char key;
 int i=0;
 int check_password=0;
 int cnt1=0, cnt2=0, cnt3=0, cnt4=0, cnt5=0;
 int valid_cnt=0;
 int reset=1;
 int mode=0; // 1 = user mode; 2 = program mode
 //char prev_key;
 //char curr_key;
 
 
//***************************************Password Database*************************************//
char pass_code1[4] = {'1','2','3','4'};
char pass_code2[4] = {'X','X','X','X'};
char pass_code3[4] = {'X','X','X','X'};
char pass_code4[4] = {'X','X','X','X'};
char pass_code5[4] = {'X','X','X','X'};
 
 char input_code[4] = {'X', 'X','X','X'};
 char set_code[4] = {'Z', 'Z', 'Z', 'Z'};
    
	
	// TODO: Initialize and configure IOs, LCD (using your code from Lab 1), 
	// UART (if desired for debugging), and any other configurations that are needed.
	
 LCDInitialize();
 KeypadInitialize();
 scanKeypad = 0; 
 
 while(1)
  {
   if(reset==1)
     { 
      LCDClear();
      LCDPrintString("Enter");
      reset=0;
     } 
           

   if(scanKeypad == 1) // wait for CN interrupt 
     {
      key = KeypadScan();
     
     
     if((key == '*') && (input_code[0] == 'X')) 
       {
        LCDMoveCursor(1,0);		
        LCDPrintChar(key);
        input_code[0] = '*';
       }
        
     if((key == '*') && (input_code[0] == '*')) // user has entered program mode
       {
        LCDMoveCursor(1,1);		
        LCDPrintChar(key);
        mode = 2; // program mode
        for(i=0; i<4; i++)
          input_code[i] = 'X';
        // delay 1 sec
        LCDClear();
        LCDPrintString("Set Mode");
       }
           
        
     if(mode==2)
       {
        if(set_code[0] == 'Z')
          {
           set_code[0] = key;
           LCDMoveCursor(1,0);
           LCDPrintChar(key);
          }
        else if((set_code[0]!= 'Z') && (set_code[1] == 'Z'))
          {
           set_code[1] = key;
           LCDMoveCursor(1,1);
           LCDPrintChar(key);
 		  }
        else if((set_code[1] != 'Z') && (set_code[2] == 'Z'))
          {
           set_code[2] = key;
           LCDMoveCursor(1,2);
           LCDPrintChar(key); 
          }
        else if((set_code[2] != 'Z') && (set_code[3] == 'Z'))
          {
           set_code[3] = key;
           LCDMoveCursor(1,3);
           LCDPrintChar(key);	
		  }
       }
  


     if((mode == 2) && (set_code[3] != 'Z') && (key == '#')) 
       {
        for(i=0; i<4; i++)
         {
          if((set_code[i] != '*') && (set_code[i] != '#'))
           valid_cnt++;
         }  
        if (valid_cnt == 4)
         {
          LCDClear();
          LCDPrintString("Valid");
          // delay 2 secs
          mode=1; // reset back to user mode
          valid_cnt=0; // reset counter
          reset=1; //reset system
          
          // update password database
          if(pass_code2[3] == 'X')
           {
            for(i=0; i<4; i++)
             pass_code2[i]=set_code[i];
           }
          else if(pass_code3[3] == 'X')
           {
            for(i=0; i<4; i++)
             pass_code3[i]=set_code[i];
           }
          else if(pass_code4[3] == 'X')
           {
            for(i=0; i<4; i++)
             pass_code4[i]=set_code[i];
           }
          else if(pass_code5[3] == 'X')
           {
            for(i=0; i<4; i++)
             pass_code5[i]=set_code[i];
           }
        // Override set_code array to reuse
        for(i=0; i<4; i++)
          set_code[i]= 'Z'; 



         }
        else                        //((set_code1[3] != 'Z') && (key != '#'))         //////////////////////what???????
         {
          LCDClear();
          LCDPrintString("Invalid");
          // delay 2 secs
          mode=1; // reset to user mode
          valid_cnt=0;
          reset=1;
          for(i=0; i<4; i++)
          set_code[i]= 'Z';
         }
       }

     else if((mode = 2) && (set_code[3] != 'Z') && (key != '#'))
       {
        LCDClear();
        LCDPrintString("Invalid");
        // delay 2 secs
        mode=1; // reset to user mode
        valid_cnt=0;
        reset=1;
        for(i=0; i<4; i++)
        set_code[i]= 'Z';
       }

//**************************************User Mode***************************************//

     if((key != -1) && (key!='*') && (key != '#'))
            {
             if(input_code[0]== 'X')
              {
               input_code[0] = key;
               LCDMoveCursor(1,0);		
			   LCDPrintChar(key);
              }
             else if((input_code[0]!= 'X') && (input_code[1] == 'X'))
              {
               input_code[1] = key;
               LCDMoveCursor(1,1);
               LCDPrintChar(key);
 			  }
             else if((input_code[1] != 'X') && (input_code[2] == 'X'))
              {
               input_code[2] = key;
               LCDMoveCursor(1,2);
               LCDPrintChar(key); 
              }
             else if((input_code[2] != 'X') && (input_code[3] == 'X'))
              {
               input_code[3] = key;
               check_password = 1;  // all 4-digits entered; raise check password flag
               reset = 1; // raise reset flag
               LCDMoveCursor(1,3);
               LCDPrintChar(key);
              // delay for 1 sec	
			  }
           }
     else if((key == '*') || (key == '#'))   ////////////////////need to ask if a pressing two at the same is considered "bad" or just ignore
        {
         LCDClear();
         LCDPrintString("Bad");
         reset = 1;
         for(i=0; i<4; i++)
          input_code[i] = 'X'; // re-initialize user input
        }
       
          
     if(check_password == 1)
             {
              check_password = 0; // set flag low
              for(i=0; i<4; i++)
                {
                 if(input_code[i] == pass_code1[i])
                   cnt1++;

                 if(input_code[i] == pass_code2[i])
                   cnt2++;

                 if(input_code[i] == pass_code3[i])
                   cnt3++;

                 if(input_code[i] == pass_code4[i])
                   cnt4++;

                 if(input_code[i] == pass_code5[i])
                   cnt5++;
				}
              
              if((cnt1==4) || (cnt2==4) || (cnt3==4) || (cnt4==4) || (cnt5==4)) // all 4-digits match **correct password**
                {
                 LCDClear();
                 LCDPrintString("Good");
                 // wait 2 secs 
              	}
              else  // incorrect password entered  
                {
                 LCDClear();
                 LCDPrintString("Bad"); 
                 // wait 2 secs
                }
               
               cnt1=0; // reset match password counter1
               cnt2=0; // reset match password counter2
               cnt3=0; // reset match password counter3
               cnt4=0; // reset match password counter4
               cnt5=0; // reset match password counter5
               for(i=0; i<4; i++)
                  input_code[i] = 'X'; // re-initialize user input in order to reuse
             }
    scanKeypad = 0;  
    }
  }
return 0;
}


void __attribute__((interrupt,auto_psv)) _CNInterrupt(void)
{	
	// TODO: Clear interrupt flag
	IFS1bits.CNIF = 0;
	
	// TODO: Detect if *any* key of the keypad is *pressed*, and update scanKeypad
	// variable to indicate keypad scanning process must be executed.
    scanKeypad = 1;
}


