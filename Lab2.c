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

// ************************************Delay fuction***************************************** //

void delay(void) 
	 {
	  T1CONbits.TON = 1; // start timer 
	  while(IFS0bits.T1IF == 0);  // wait (1sec)
	  T1CONbits.TON = 0; // turn off timer1
	  TMR1 = 0; // Reset Timer1 counter
	  IFS0bits.T1IF = 0; // reset flag
     }

//********************************************************************************************//

// Varible used to indicate that the current configuration of the keypad has been changed,
// and the KeypadScan() function needs to be called.

volatile char scanKeypad;
// ******************************************************************************************* //

int main(void)
{
 char key;
 int i=0;
 int check_password=0, password_set=0;
 int cnt1=0, cnt2=0, cnt3=0, cnt4=0, cnt5=0;
 int valid_cnt=0;
 int reset=1;
 int mode=0; // 1 = user mode; 2 = program mode
 
//***************************************Password Database*************************************//
char pass_code1[4] = {'1','2','3','4'};
char pass_code2[4] = {'X','X','X','X'};
char pass_code3[4] = {'X','X','X','X'};
char pass_code4[4] = {'X','X','X','X'};
char pass_code5[4] = {'X','X','X','X'};
 
char input_code[4] = {'X', 'X','X','X'};
char set_code[4] = {'Z', 'Z', 'Z', 'Z'};

//********************************************Timer1******************************************//

// Timer 1 used for delay
T1CONbits.TCKPS1 = 1;
T1CONbits.TCKPS0 = 1;
IEC0bits.T1IE = 1; // enable interrupt for Timer1
PR1 = 57600; // Timer 1's period value regsiter to value for 1 sec.
T1CONbits.TON = 0;	// Set Timer 3 to be initially off.
TMR1 = 0; // Clear Timer 1 value      
IFS0bits.T1IF = 0; // reset interrupt flag for Timer1
    
//******************************************Initialize****************************************//
	
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
     
//**********************************Check if Program Mode Initiated*************************//     
     
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
          input_code[i] = 'X';  // re-initialize 
        delay(); // 1 sec
        LCDClear();
        LCDPrintString("Set Mode");
       }
      
     else if((key != '*') && (input_code[0] == '*')) 
       {
        LCDClear();
        LCDPrintString("Invalid"); 
        delay(); // 1 sec
        delay(); // 1 sec
        reset=1;
        for(i=0; i<4; i++)
          input_code[i] = 'X';
      }

//***************************************In Program Mode*************************************//           
        
     if(mode==2) 
       {
        
		if(password_set==0)
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
             password_set=1; // 4 characters have been entered	
		    }
          }
        
		else if((password_set == 1) && (key == '#')) // user input 4 characters followed by #
           {
            for(i=0; i<4; i++) // check if characters entered make a valid passcode
             {
              if((set_code[i] != '*') && (set_code[i] != '#'))
                valid_cnt++;
             }  
            if (valid_cnt == 4)
             {
              LCDClear();
              LCDPrintString("Valid");
              delay(); // 1 sec
              delay(); // 1 sec
              mode=1; // reset back to user mode
              valid_cnt=0; // reset counter
              reset=1; //reset system
              password_set=0;
          
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
          }
        
		else if ((password_set == 1) && (key != '#'))  // user input 4 characters but failed to follow-up with #     
           {
            LCDClear();
            LCDPrintString("Invalid");
            delay(); // 1 sec
            delay(); // 1 sec
            mode=1; // reset to user mode
            valid_cnt=0;
            reset=1;
            password_set=0;
            for(i=0; i<4; i++)
              set_code[i]= 'Z';
           }
       }

 //**************************************User Mode********************************************//

     if((key != -1) && (key!='*') && (key != '#')) // and mode=1
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
               delay(); // 1 sec delay	
			  }
           }
 /*    else if((key == '*') || (key == '#'))   ////////////////////need to ask if a pressing two at the same is considered "bad" or just ignore
        {
         LCDClear();
         LCDPrintString("Bad");
         delay(); // 1 sec
         delay(); // 1 sec
         reset = 1;
         for(i=0; i<4; i++)
          input_code[i] = 'X'; // re-initialize user input
        }    */
        
//********************************Compare User Input with Database************************//          
     if(check_password == 1)
             {
              check_password = 0; // set flag low
              for(i=0; i<4; i++)
                {
                 if(input_code[i] == pass_code1[i]) // compare with password1
                   cnt1++;

                 if(input_code[i] == pass_code2[i]) // compare with password2
                   cnt2++;

                 if(input_code[i] == pass_code3[i]) // compare with password3
                   cnt3++;

                 if(input_code[i] == pass_code4[i]) // compare with password4
                   cnt4++;

                 if(input_code[i] == pass_code5[i]) // compare with password5
                   cnt5++;
				}
              
              if((cnt1==4) || (cnt2==4) || (cnt3==4) || (cnt4==4) || (cnt5==4)) // correct password
                {
                 LCDClear();
                 LCDPrintString("Good");
                 delay(); // 1 sec
                 delay(); // 1 sec
              	}
              else  // incorrect password entered  
                {
                 LCDClear();
                 LCDPrintString("Bad"); 
                 delay(); // 1 sec
                 delay(); // 1 sec
                }
               
               reset=1; 
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


