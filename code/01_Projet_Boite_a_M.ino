/* ----------------------------------------------------------------------------
        [ Projet DIY ]  La Boite à M  ... ou boite à autre chose ...

         https://github.com/FredJ21/Boite_a_M
		 https://www.facebook.com/fredrobotic
                    
                                      Frederic JELMONI - mars 2024
---------------------------------------------------------------------------- */


#include "SoftwareSerial.h"       // pour les communications series avec le DFplayer
#include "DFRobotDFPlayerMini.h"  // pour le DFPlayer


// PIN ESP8266 WEMOS D1 mini 
#define BUTTON_1 D3               // capteur d'inclinaison 
#define LED D4                    // led de l'ESP8266

SoftwareSerial mySoftwareSerial(D2,D8); 

// Init Player
DFRobotDFPlayerMini myDFPlayer;


// variables
int buttonState_1 = 0;
int a = 0;
bool play_ok = false;
int current_file = 2;   // on démarre la lecture au 2eme fichier  ( le 1er est jouer durant la sequence d'init 
int nb_file      = 4;   // nombre total de fichier


// ------------------------------------
//            SETUP
// ------------------------------------
void setup() {

  pinMode(LED, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);           // optionnel pour info dans terminal
  
  Serial.println("\n\n\n--------------------[ GO ]--------------------");

   if (!myDFPlayer.begin(mySoftwareSerial)) {  //Utilisation de  softwareSerial pour communiquer
    Serial.println(F("Pb communication:"));
    Serial.println(F("1.SVP verifier connexion serie!"));
    Serial.println(F("2.SVP verifier SDcard !"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini En ligne."));
  
  myDFPlayer.setTimeOut(500); // Définit un temps de time out sur la communication série à 500 ms

  //  Controle volume
  myDFPlayer.volume(30);      // ( valeur de 0 à 30 )
  // indique d'utiliser le player de carte SD interne
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  // quelques infos 
  Serial.print("nb total de fichier : ");
  Serial.println(myDFPlayer.readFileCounts()); //Le nombre total de fichier mp3 sur la carte ( dossier inclus )
  Serial.print("index courant : ");
  Serial.println(myDFPlayer.readCurrentFileNumber()); // l'index courant 

  Serial.println("play: 1");
  myDFPlayer.play(1);



  // Séquence de démarrage ( un coup à l'envers, un coup à l'endroit )
  while ( digitalRead(BUTTON_1) ) {
    Serial.println("Wait 1 ...");
    delay(1000); 
  }
  while ( !digitalRead(BUTTON_1) ) {
    Serial.println("Wait 2 ...");
    delay(1000); 
  }

  Serial.println("go !!!");
  delay(100); 
  
}

// ------------------------------------
//            LOOP
// ------------------------------------
void loop() {

  buttonState_1 = !digitalRead(BUTTON_1);

  // quand le capteur est en position, on incrémente la variable a
  if ( buttonState_1 ) {    a++;
  } else {                  a=0;  play_ok = false;
  }


  // on joue un son quand a > 5  (  correspond à 5 bloucles -->  500ms )
  // permet de ne pas jouer le son trop rapidement
  
  if ( a>5 && play_ok == false ) {

    Serial.print("play: ");
    Serial.println(current_file);
    myDFPlayer.play(current_file);
  
    play_ok = true; 
    delay(3000);

    // Changement de son
    current_file++;
    if( current_file > nb_file )  {  current_file = 2; }
    
  }
  delay(100);      
}
// ----------------------------------------------------------------------------
