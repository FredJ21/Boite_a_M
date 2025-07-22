// Bibliothèque à inclure au minimum
#include "SoftwareSerial.h"    // pour les communications series avec le DFplayer
#include "DFRobotDFPlayerMini.h"  // bibliotheque pour le DFPlayer


// PIN qui serviront pour la communication série sur le WEMOS
SoftwareSerial mySoftwareSerial(D2,D8); // RX, TX ( wemos D2,D8 ou 4,15 GPIO )  ou Tx,RX ( Dfplayer )
DFRobotDFPlayerMini myDFPlayer;  // init player


#define BUTTON_1 D3
#define LED D4


// variables
int buttonState_1 = 0;
int a = 0;
bool play_ok = false;

void setup() {

  pinMode(LED, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);  // optionnel pour info dans terminal
  
  Serial.println("\n\n\n--------------------[ GO ]--------------------");

   if (!myDFPlayer.begin(mySoftwareSerial)) {  //Utilisation de  softwareSerial pour communiquer
    Serial.println(F("Pb communication:"));
    Serial.println(F("1.SVP verifier connexion serie!"));
    Serial.println(F("2.SVP verifier SDcard !"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini En ligne."));
  
  myDFPlayer.setTimeOut(500); // Définit un temps de time out sur la communication série à 500 ms

  //----Controle volume----
  myDFPlayer.volume(30);  //Monte le volume à 18 ( valeur de 0 à 30 )
  // ---- indique d'utiliser le player de carte SD interne
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);



  // optionel , permet d'afficher quelques infos 
  Serial.print("nb total de fichier : ");
  Serial.println(myDFPlayer.readFileCounts()); //Le nombre total de fichier mp3 sur la carte ( dossier inclus )
  Serial.print("index courant : ");
  Serial.println(myDFPlayer.readCurrentFileNumber()); // l'index courant 
  

  Serial.println("play: 1");
  myDFPlayer.play(1);


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

int current_file = 2;
int play_counter = 0;

void loop() {

  buttonState_1 = !digitalRead(BUTTON_1);

  if ( buttonState_1 ) {
    a++;
  } else {
    a=0;
    play_ok = false;
  }
  
  if ( a>5 && play_ok == false ) {

    Serial.print("play: ");
    Serial.println(current_file);
    myDFPlayer.play(current_file);
  
    play_ok = true; 
    delay(3000);


    // Nombre de lecture
    play_counter++;

    if ( play_counter > 0 ) {
        
        play_counter = 0;

        // Changement de son
        current_file++;
        if( current_file > 5 )  {  current_file = 2; }

    }

  }


  delay(100);      

  
}
