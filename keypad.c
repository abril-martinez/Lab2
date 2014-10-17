// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "keypad.h"
#include "lcd.h"
//#include <stdio.h>

// ******************************************************************************************* //

//#define ROWA LATA
//#define ROWB LATB

#define KEY_C0			PORTBbits.RB2
#define KEY_C1		    PORTBbits.RB3	
#define KEY_C2			PORTBbits.RB5 
#define KEY_R0			LATBbits.LATB11        
#define KEY_R1			LATBbits.LATB10 
#define KEY_R2			LATAbits.LATA0
#define KEY_R3			LATAbits.LATA1

//char register[4]

void KeypadInitialize() {
	
 // Configure cols->inputs and rows->outputs
 TRISBbits.TRISB2  = 1;
 TRISBbits.TRISB3  = 1;
 TRISBbits.TRISB5  = 1; 
 TRISBbits.TRISB11 = 0;
 TRISBbits.TRISB10 = 0;
 TRISAbits.TRISA0  = 0;
 TRISAbits.TRISA1  = 0;	

 //Enable input pins for CN interrupt
 CNEN1bits.CN6IE =  1; //RB2
 CNEN1bits.CN7IE =  1; //RB3
 CNEN2bits.CN27IE = 1; //RB5

 //Enable Change of Notification Interrupt for columns
 IEC1bits.CNIE = 1; // Enable CN interrupt
 IFS1bits.CNIF = 0; //clear flag

 // Enable the CNPU resistors for inputs
 CNPU1bits.CN6PUE =  1; //RB2
 CNPU1bits.CN7PUE =  1; //RB3
 CNPU2bits.CN27PUE = 1; //RB5

 //Configure ODC register bits to use open drain configuration for output pins
 ODCBbits.ODB11 = 1;
 ODCBbits.ODB10 = 1;
 ODCAbits.ODA0 =  1;
 ODCAbits.ODA1 =  1;

 // Ground all the rows
 KEY_R0 = 0;
 KEY_R1 = 0;
 KEY_R2 = 0;
 KEY_R3 = 0;

 AD1PCFGbits.PCFG4 = 1;
 AD1PCFGbits.PCFG5 = 1;
 AD1PCFGbits.PCFG0 = 1;
 AD1PCFGbits.PCFG1 = 1;

//Initialize inputs pins to off
LATBbits.LATB2 = 0;
LATBbits.LATB3 = 0;
LATBbits.LATB5 = 0;

 // Ground all rows
 //ROWB &= 0xF3FF; // RB11 & RB10 = 0
 //ROWA &= 0xFFFC; // RA0 & RA1 = 0
}

// ******************************************************************************************* //

char KeypadScan() {
	char key = -1;
	int row=0;	
	int col0 = 0; //Column zero flag
	int col1 = 0; //Column one flag
	int col2 = 0; //Column two flag
	int rows = 0; //num of rows pressed

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

	while((KEY_C0 == 0) || (KEY_C1 == 0) || (KEY_C2 == 0)){
		
		//These three if statements keep track of how many columns have been activated
		if (KEY_C0 == 0) 	col0 = 1; //If button in column zero was pressed set flag to high
		if (KEY_C1 == 0) 	col1 = 1; //If button in column one was pressed set flag to high
		if (KEY_C2 == 0) 	col2 = 1; //If button in column two was pressed set flag to high

		//If two or more buttons have not been pressed proceed to scan
		if (rows < 2 && (col0 + col1 + col2) < 2){
			//Reset the number of rows activated flag
			rows = 0;
			//for loop to scan the rows
			for( row = 0; row < 4; row++){


         if(row == 0)
          {
           KEY_R0 = 0;
	       KEY_R1 = 1;
           KEY_R2 = 1;
           KEY_R3 = 1;
          }
         else if(row == 1)
          {
           KEY_R0 = 1;
	       KEY_R1 = 0;
           KEY_R2 = 1;
           KEY_R3 = 1;
          }
         else if(row == 2)
          {
           KEY_R0 = 1;
	       KEY_R1 = 1;
           KEY_R2 = 0;
           KEY_R3 = 1;
          }

         else if(row == 3)
          {
           KEY_R0 = 1;
	       KEY_R1 = 1;
           KEY_R2 = 1;
           KEY_R3 = 0;
          }
           

				//Use Bit masking and shifting to set each row to zero for each pass of the for loop
	//			ROWB |= 0x0C00; 	//Clear RB11->RB10
     //           ROWA |= 0x0003;     //Clear RA1->RA0
				

            //    ROWB &= 0xF7FF; 	//Set RB11->RB10 to '0111'
             //   ROWA &= 0xFFFD;     //Set RA1->RA0 to '1101
				
            //    ROWB ^= 0x0C00; 	//Invert RB11->RB10 for shifting
            //    ROWA ^= 0x0003;     //Invert RA1->RA0 for shifting
				
            //    ROWB >>= row;		//Shift by row number
            //    ROWA >>= row;
				
     //           ROWB ^= 0x0C00;	//Invert back to set correct ground
       //         ROWA ^= 0x0003;   





			
	            DelayUs(10); //Delay to allow signal response

				//Check if a button in pressed for this row iteration
				if((KEY_C0 == 0) || (KEY_C1 == 0) || (KEY_C2 == 0)){
					
					rows++; //If a pressed button is detected increase number of rows activated

					//Row 0 through 2 check which column is pressed and update the key character
					if (row < 3){
						if (KEY_C0 == 0){
							key = 0x31 + row * 3;
						}
						if (KEY_C1 == 0){ 
							key = 0x32 + row * 3;
						}	
						if (KEY_C2 == 0){ 
							key = 0x33 + row * 3;	
						}
					}
					//	If row is 3 check rows and set custom characters that don't fit above method
					else if (row == 3){
						if (KEY_C0 == 0){
							key = 0x2A; 		//'*'
						}
						if (KEY_C1 == 0){
							key = 0x30;			//'0'
						}
						if (KEY_C2 == 0){
							key = 0x23;			//'#'
						}
					}
				}
			}	
		}
                // Ground all rows
               // ROWB &= 0xF3FF; // RB11 & RB10 = 0
               // ROWA &= 0xFFFC; // RA0 & RA1 = 0

// Ground all the rows
 KEY_R0 = 0;
 KEY_R1 = 0;
 KEY_R2 = 0;
 KEY_R3 = 0;

		DelayUs(10); //Delay to allow signal response
	}

	//If the sum of rows and columns activated is more than 2, it means more than one button has been pressed.
	if ((col0 + col1 + col2 + rows) > 2) return -1; 
	else return key;
}

// ******************************************************************************************* //
