// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "keypad.h"


// ******************************************************************************************* //

#define KEYPAD_C0			PORTBbits.RB2
#define KEYPAD_C1		    PORTBbits.RB3	
#define KEYPAD_C2			PORTBbits.RB5 
#define KEYPAD_R0			PORTBbits.RB11 
#define KEYPAD_R1			PORTBbits.RB10 
#define KEYPAD_R2			PORTAbits.RA0 
#define KEYPAD_R3			PORTAbits.RA1 

/************************************Variables*************************************************/

char returnChar;

/***********************************Fucntion Prototypes****************************************/
void KeypadInitialize();
void active_column(int col);
char KeypadScan();
char ScanRows();

/***************************************Initialize*********************************************/
void KeypadInitialize() {
	
// TODO: Configure IOs and Change Notificaiton interrupt for keypad scanning. This 
// configuration should ensure that if any key is pressed, a change notification interrupt 
// will be generated.

//AD1PCFGbits.PCFG4 = 1;
//AD1PCFGbits.PCFG4 = 1;

// Configure cols->inputs and rows->outputs
TRISBbits.TRISB2  = 1;
TRISBbits.TRISB3  = 1;
TRISBbits.TRISB5  = 1; 
TRISBbits.TRISB11 = 0;
TRISBbits.TRISB10 = 0;
TRISAbits.TRISA0  = 0;
TRISAbits.TRISA1  = 0;

// Enable the CNPU resistors for inputs
CNPU1bits.CN6PUE =  1; //RB2
CNPU1bits.CN7PUE =  1; //RB3
CNPU2bits.CN27PUE = 1; //RB5

//Enable input pins for CN interrupt
CNEN1bits.CN6IE =  1; //RB2
CNEN1bits.CN7IE =  1; //RB3
CNEN2bits.CN27IE = 1; //RB5

//Enable Change of Notification Interrupt for columns
IEC1bits.CNIE = 1; // Enable CN interrupt
IFS1bits.CNIF = 0; //clear flag

//Configure ODC register bits to use open drain configuration for output pins
ODCBbits.ODB11 = 1;
ODCBbits.ODB10 = 1;
ODCAbits.ODA0 =  1;
ODCAbits.ODA1 =  1;

//Initialize inputs pins to off
//LATBbits.LATB2 = 1;
//LATBbits.LATB3 = 1;
//
LATBbits.LATB5 = 0;

}

//**************************************Set Column Active**************************************//

void active_column(int col) // set one column active
  {
   if(col == 0) 
     {
      LATBbits.LATB2 = 0;
      LATBbits.LATB3 = 1;
	  LATBbits.LATB5 = 1;
     }
    
   else if(col == 1) 
     {
      LATBbits.LATB2 = 1;
      LATBbits.LATB3 = 0;
	  LATBbits.LATB5 = 1;
     }
    
   else if(col == 2) 
     {
      LATBbits.LATB2 = 1;
      LATBbits.LATB3 = 1;
	  LATBbits.LATB5 = 0; 
     }
    
   //default set all on
/*   else 
     {
      LATBbits.LATB2 = 0;
      LATBbits.LATB3 = 0;
	  LATBbits.LATB5 = 0; 
     }  */
}

// **************************************KeyPad Scan***************************************************** //

char KeypadScan() 
  { 
    char key = -1;
    key = ScanRows(); //read each row when a specific col is set active
 

    // TODO: Implement the keypad scanning procedure to detect if exactly one button of the 
	// keypad is pressed. The function should return:
	//
	//      -1         : Return -1 if no keys are pressed.
	//      '0' - '9'  : Return the ASCII character '0' to '9' if one of the 
	//                   numeric (0 - 9) keys are pressed.
	//      '#'        : Return the ASCII character '#' if the # key is pressed. 
	//      '*'        : Return the ASCII character '*' if the * key is pressed. 
	//       -1        : Return -1 if more than one key is pressed simultaneously.
	// Notes: 
	//        1. Only valid inputs should be allowed by the user such that all invalid inputs 
	//           are ignored until a valid input is detected.
	//        2. The user must release all keys of the keypad before the following key press
	//           is processed. This is to prevent invalid keypress from being processed if the 
	//           users presses multiple keys simultaneously.
	//
	return key;
 }

// ***********************************Scan Rows Function*************************************** //

char ScanRows()
 {
   int column=0;
   for(column=0; column<3; column++)
    {
     active_column(column); // determines which column is active   
   
   
     if(column == 0) 
      {
       if(KEYPAD_R0 == 0 && KEYPAD_R1 ==1 && KEYPAD_R2 == 1 && KEYPAD_R3 == 1) 
        {
         returnChar = '1';
         while(KEYPAD_R0== 0); // wait for button release
        }
       else if(KEYPAD_R1 == 0 && KEYPAD_R0 == 1 && KEYPAD_R2 == 1 && KEYPAD_R3 ==1)
        {
         returnChar = '4';
         while(KEYPAD_R1==0); // wait for button release
        }
	   else if(KEYPAD_R2 == 0 && KEYPAD_R0 == 1 && KEYPAD_R1 == 1 && KEYPAD_R3 == 1)
        {
         returnChar = '7';
         while(KEYPAD_R2==0); // wait for button release
        }
       else if(KEYPAD_R3 == 0 && KEYPAD_R0 == 1 && KEYPAD_R1 == 1 && KEYPAD_R2 == 1)
        { 
         returnChar = '*';
         while(KEYPAD_R3 ==0); // wait for button release
        }
      }

    
     else if(column == 1) 
       {
       if(KEYPAD_R0 == 0 && KEYPAD_R1 ==1 && KEYPAD_R2 == 1 && KEYPAD_R3 == 1) 
        {
         returnChar = '2';
         while(KEYPAD_R0== 0); // wait for button release
        }
       else if(KEYPAD_R1 == 0 && KEYPAD_R0 == 1 && KEYPAD_R2 == 1 && KEYPAD_R3 ==1)
        {
         returnChar = '5';
         while(KEYPAD_R1==0); // wait for button release
        }
	   else if(KEYPAD_R2 == 0 && KEYPAD_R0 == 1 && KEYPAD_R1 == 1 && KEYPAD_R3 == 1)
        {
         returnChar = '8';
         while(KEYPAD_R2==0); // wait for button release
        }
      else if(KEYPAD_R3 == 0 && KEYPAD_R0 == 1 && KEYPAD_R1 == 1 && KEYPAD_R2 == 1)
        { 
         returnChar = '0';
         while(KEYPAD_R3 ==0); // wait for button release
        }
       }

    
      else if(column == 2) 
        {
       if(KEYPAD_R0 == 0 && KEYPAD_R1 ==1 && KEYPAD_R2 == 1 && KEYPAD_R3 == 1) 
        {
         returnChar = '3';
         while(KEYPAD_R0== 0); // wait for button release
        }
       else if(KEYPAD_R1 == 0 && KEYPAD_R0 == 1 && KEYPAD_R2 == 1 && KEYPAD_R3 ==1)
        {
         returnChar = '6';
         while(KEYPAD_R1==0); // wait for button release
        }
	   else if(KEYPAD_R2 == 0 && KEYPAD_R0 == 1 && KEYPAD_R1 == 1 && KEYPAD_R3 == 1)
        {
         returnChar = '9';
         while(KEYPAD_R2==0); // wait for button release
        }
       else if(KEYPAD_R3 == 0 && KEYPAD_R0 == 1 && KEYPAD_R1 == 1 && KEYPAD_R2 == 1)
        { 
         returnChar = '#';
         while(KEYPAD_R3 ==0); // wait for button release
        }
       }

      else
        {
         returnChar = -1; // either no button pressed or buttons simulataneously pressed
         while(KEYPAD_R0 == 0 || KEYPAD_R1 == 0 || KEYPAD_R2 == 0 || KEYPAD_R3 == 0); // wait for button(s) to be released
        } 
     
     }   
return returnChar;

 }