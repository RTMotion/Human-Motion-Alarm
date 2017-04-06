// Get the size of the array (used later)
#define SIZE(x) (sizeof(x) / sizeof((x)[0]))

//    Wiring
//        - Pin 9     RST
//        - Pin 10    NSS
//        - Pin 11    MOSI
//        - Pin 12    MISO
//       - Pin 13    SCK
//        - 3V3       3V3
//       - GND       GND

#define SERIAL

// Get the neccessarry libs
#include <SPI.h>
#include <MFRC522.h>

// Get the size of the array (used later)
#define SIZE(x) (sizeof(x) / sizeof((x)[0]))

//    Wiring
//        - Pin 9     RST
//        - Pin 10    NSS
//        - Pin 11    MOSI
//        - Pin 12    MISO
//       - Pin 13    SCK
//        - 3V3       3V3
//       - GND       GND

#define SERIAL

// Get the neccessarry libs
#include <SPI.h>
#include <MFRC522.h>

// Get the size of the array (used later)
#define SIZE(x) (sizeof(x) / sizeof((x)[0]))

// Define pins
#define RFID_SDA 10
#define RFID_RST 9
const int TILT = 8;
const int BUZZ = 7;
#define BLUELED 6
#define GREENLED 5
const int TILTREDLED = 4;

int ResetButton = 3;
int pb1 = 2;
int pb2 = A0;
int pb3 = A1;
int pb4 = A2;
int pb5 = A3;
int ButtonState = 0;
int val = 0;

// Name the receiver
MFRC522 RFID(RFID_SDA, RFID_RST);

byte* UID_Array = 0;
byte UID_Array_Size = 0;

// Enter your card-ID here
byte Access_UID_Array[2][7] = {{70, 8, 219, 147, 0, 0, 0}, {128, 60, 2, 1, 0, 0, 0}};

int combo[5] = {1,1,2,2,5};  //5-digit combination
int entered[5];               //combo that the user enters
// ##################################################################################################################### 
// ######################################### SETUP #####################################################################
// ##################################################################################################################### 

void setup()
{   
    digitalWrite(BLUELED, HIGH);
    digitalWrite(GREENLED, LOW);
    digitalWrite(TILTREDLED, LOW);
    pinMode(GREENLED, OUTPUT);
    pinMode(BLUELED, OUTPUT);
    pinMode(TILTREDLED, OUTPUT);
    pinMode(BUZZ, OUTPUT); 
    pinMode(TILT, INPUT);
  pinMode (pb1, INPUT);
  pinMode (pb2, INPUT);
  pinMode (pb3, INPUT);
  pinMode (pb4, INPUT);
  pinMode (pb5, INPUT);
  pinMode (ResetButton, INPUT);

    for (int i = 0; i < 5;i++)  {   //display values in the combo and entered array - all zeros as of this point
    Serial.println(combo[i]);
    Serial.println(entered[i]);
  }
    
    // Initialize the SPI BUS and the RFID-module
    SPI.begin();
    RFID.PCD_Init();

    // Only print to serial monitor if it has been defined.
    // If you don't need a serial monitor, comment out line 24.
    #ifdef SERIAL
    Serial.begin(9600);
    Serial.print("RFID receiver initialized.");
    #endif
}
// ##################################################################################################################### 
// ######################################### LOOP ######################################################################
// ##################################################################################################################### 
void loop()
{
  {
  val = digitalRead(TILT);
    if(val == LOW) //without vibration signal
  {
    digitalWrite(TILTREDLED,HIGH); //turn ON the led    
    digitalWrite(BUZZ,HIGH); // turn ON buzzer
    digitalWrite(BLUELED,LOW); //turn OFF blue LED 
  }
  }
  
{
  ButtonState = digitalRead(ResetButton);
  if(ButtonState==LOW) // check if button is pressed if yes reset LEDs and Buzzer
 {
  }
  else
  {
  for (int i = 0; i<5; i++)  {
  entered[i] = 0;
  Serial.println(entered[i]);
  }
  digitalWrite(TILTREDLED,HIGH);
  digitalWrite(BLUELED,LOW);
  digitalWrite(GREENLED,LOW);
  digitalWrite(BUZZ, HIGH);
  delay(500);
  digitalWrite(TILTREDLED,LOW);
  digitalWrite(BUZZ, LOW);
  digitalWrite(BLUELED,HIGH);
  }
  }
  
 if (digitalRead(pb1) == HIGH)   {
    checkElement1(1);
    delay(250);
  }
  else if (digitalRead(pb2) == HIGH)  {
    checkElement1(2);
    delay(250);
  }
  else if (digitalRead(pb3) == HIGH)  {
    checkElement1(3);
    delay(250);
  }
  else if (digitalRead(pb4) == HIGH)  {
    checkElement1(4);
    delay(250);
  }
  else if (digitalRead(pb5) == HIGH)  {
    checkElement1(5);
    delay(250);
  }


    /*  
        If a card is recognized, then go on and check if it is valid.
        If so, then turn on the LED for 2 seconds, otherwise flash the LED rapid.
    */
    
    if (getUID())
    {
        if (checkUID())
        {   int entered[5];
            digitalWrite(GREENLED, HIGH);
            digitalWrite(TILTREDLED,LOW);           
            digitalWrite(BUZZ, LOW);                       
            digitalWrite(BLUELED, LOW);
            delay (5000);
            digitalWrite(BLUELED,HIGH);
            digitalWrite(GREENLED, LOW);
        }
        else 
        {

            for (int i = 0; i < 75; i++)
            {
                digitalWrite(BLUELED, LOW);
                digitalWrite(BUZZ, HIGH);
                delay(60);
                digitalWrite(BUZZ, LOW);
                delay(60);
                digitalWrite(TILTREDLED, LOW);
                delay(80);
                digitalWrite(TILTREDLED, HIGH);
                delay(80);
                digitalWrite(BLUELED, HIGH);
                digitalWrite(TILTREDLED, LOW);
            }
        }
    }
}

  

// ##################################################################################################################### 
// ######################################### GET UNIQUE ID #############################################################
// ##################################################################################################################### 
boolean getUID()
{
    // Check if a card is resent and also if it can be accessed.
    if (RFID.PICC_IsNewCardPresent() && RFID.PICC_ReadCardSerial())
    {
        #ifdef SERIAL
        Serial.print("\n\nCard detected.\n  ID: ");
        #endif
        /*
            The array size shall have the size of the unique ID (4 or 7)
            Therefore we delete array and create a new one with the size of the UID.
        */
        delete [] UID_Array;
        UID_Array_Size = RFID.uid.size;
        UID_Array = new byte [UID_Array_Size];

        // Get the UID from the card
        for (byte i = 0; i < UID_Array_Size; i++)
        {
            UID_Array[i] = RFID.uid.uidByte[i];

            #ifdef SERIAL
            Serial.print(UID_Array[i]);
            Serial.print(" ");
            #endif
        }

        return true;
    }
    // If there is no card or it can't be accessed, then return false
    else 
        return false;
}
// ##################################################################################################################### 
// ######################################### CHECK UNIQUE ID ###########################################################
// #####################################################################################################################
boolean checkUID()
{
    boolean Access = false;
    /*
        Here we check, if the cards UID is stored in the array, which we have defined at the beginning.
        Therefore we need two for-loops. The first one goes trough the stored cards: card 1, card 2, ..., card n.
        The second one goes trough the 4 or 7 blocks of the UID.
        If every block of the stored card matches with ever block from the present one, then return true.
        Attention: If only one card is found with "all-true", the checkUID-function will be terminated, and the true
        statement will be returned. All other stored cards won't be checked.
        If there is no match at all, false will be returned.
    */
    for (byte i = 0; i < SIZE(Access_UID_Array); i++)
    {
        for (byte j = 0; j < UID_Array_Size; j++)
        {
            Access = false;
            if (Access_UID_Array[i][j] != UID_Array[j])
                break;
            else 
                Access = true;
        }

        if (Access)
        {
            #ifdef SERIAL
            Serial.print("\n  Access granted.");
            #endif
            return true;
        }
    }

    #ifdef SERIAL
    Serial.print("\n  Access denied.");
    #endif
    return false;
} 

/*##################################################################################################################### 
 ######################################### Check Button ###########################################################
 #####################################################################################################################
*/
void checkElement1(int button)  {
if (entered[0] != 0)   {               //first element in the entered array has a value other than zero meaning it has been assigned a button
   checkElement2(button);               //move to checkentered 2, passing it "button"
}
else if (entered[0] == 0)  {           //the element in the entered array has not been assigned a value by a button press
  entered[0] = button;                  //give this element the the "button" value
  Serial.print("1: ");Serial.println(entered[0]);  //display "1: " indicatingit is the first pressed and then the value in the current "ntered" element
}
}

void checkElement2(int button)  {
if (entered[1] != 0)  {
  checkElement3(button);
}
else if (entered[1] == 0)  {
   entered[1] = button;
   Serial.print("2: ");Serial.println(entered[1]);
}

}

void checkElement3(int button)  {
if (entered[2] != 0)  {
  checkElement4(button);
}
else if (entered[2] == 0)  {
  entered[2] = button;
  Serial.print("3: ");Serial.println(entered[2]);
}

}

void checkElement4(int button)  {
if (entered[3] != 0)  {
  checkElement5(button);
}
else if (entered[3] == 0)  {
   entered[3] = button;
   Serial.print("4: ");Serial.println(entered[3]);
}
}

void checkElement5(int button)  {
  if (entered[4] == 0)  {
 entered[4] = button;
  
 Serial.print("5: ");Serial.println(entered[4]);
     delay(100);
    compareCombo();
  }
  }

 void compareCombo()  {
if ((combo[0] == entered[0]) && (combo[1] == entered[1]) && (combo[2] == entered[2]) && (combo[3] == entered[3]) && (combo[4] == entered[4]))  {
   for (int i = 0; i<5; i++)  {    //if each element in the combo and corresponding entered array match
   Serial.println(entered[i]);     //print the values that have been entered
   }
 
      digitalWrite(TILTREDLED, LOW);     //unlock output (turn on green led)
      digitalWrite(BLUELED, LOW);
      digitalWrite(BUZZ, LOW);
      digitalWrite(GREENLED, HIGH);
      delay(5000);
      digitalWrite(GREENLED, LOW);
      digitalWrite(BLUELED, HIGH);
      
    for (int i = 0; i<5; i++)  {
    entered[i] = 0;
    Serial.println(entered[i]);
   }
   
}
else /*if ((combo[0] != entered[0]) || (combo[1] != entered[1]) || (combo[2] != entered[2]) || (combo[3] != entered[3]) || (combo[4] != entered[4]))*/  {
            for (int i = 0; i < 75; i++)
            {
                digitalWrite(BLUELED, LOW);
                digitalWrite(BUZZ, HIGH);
                delay(60);
                digitalWrite(BUZZ, LOW);
                delay(60);
                digitalWrite(TILTREDLED, LOW);
                delay(80);
                digitalWrite(TILTREDLED, HIGH);
                delay(80);
                digitalWrite(BLUELED, HIGH);
                digitalWrite(TILTREDLED, LOW);
                digitalWrite(BUZZ, LOW);
            }
   for (int i = 0; i<5; i++)  {
    entered[i] = 0;
    Serial.println(entered[i]);
   }
}
}


