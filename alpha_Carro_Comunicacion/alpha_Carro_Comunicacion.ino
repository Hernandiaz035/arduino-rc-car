#include <Servo.h>
#include <string.h>

#define sA 5
#define sB 6
#define pD 3

//----------------------------------------------------------------
//----------------------Variables de Tiempo-----------------------
//----------------------------------------------------------------
#define deltaTM 5   //Actualizacion de datos, acondicionar con la comunicacion***
unsigned long lastM;

#define deltaTD 10  //Direccion
unsigned long lastD;

volatile float t=0;// temporal  para fir de S(senos)
//----------------------------------------------------------------
//----------------Variables de Manipulacion de String-------------
//----------------------------------------------------------------
char inChar = 0;
char sz[] = "                                                                                          ";//"Here; is some; sample;100;data;1.414;1020;200";//"$;a=20;b=46;*";
String inputString = "";
boolean stringComplete = false;  // whether the string is complet
//boolean READDATA = false;  // whether the string is complet
//char floatbuf[32]; // make this at least big enough for the whole string

//----------------------------------------------------------------
//----------------------Variables FIR 2D--------------------------
//----------------------------------------------------------------
const float a[]={0.020979020979021 ,  0.020979020979021},
            b[]={1 , -0.958041958041958};

const byte orden=(sizeof(a)/sizeof(a[0])); //******Cambié char por byte******

volatile float p[2][orden],
        x,
        y;

//----------------------------------------------------------------
//----------------------Variables Carro---------------------------
//----------------------------------------------------------------
#define initAng 94
#define minAng -35
#define maxAng 35
#define thrAcc 10

volatile int acc = 0,
              accf = 0;
volatile int dirAng = 0;

Servo sD;


//================================================================
void setup() {
  Serial.begin(57600);
  inputString.reserve(200);
  Serial.attachInterrupt(SerialInterrupt);
  
  sD.attach(pD);
  DDRD|=B11111100;//pinMode(sA,OUTPUT)
  DDRB|=B11111111;
  PORTD&=B11111100;
  PORTB&=B00100000;
  lastM=0;
  lastD=0;
}

void loop() {
  if((millis()-lastM)>=deltaTM){//-----------ACTUALIZACION ACCELERACION FILTRADA------------
    accf=FIR(acc);
    lastM=millis();
  }
  //------------------------------------------*---------------------------------------------
  //---------------------ACTUALIZACION DE VARIABLES DE FUNCIONAMIENTO-----------------------
  if((millis()-lastD)>=deltaTD){
    sD.write(dirAng);

    if (accf<=0){
      analogWrite(sA,(int)(accf>>2));
      analogWrite(sB,0);
    }else{
      accf=abs(accf);
      analogWrite(sB,(int)(accf));
      analogWrite(sA,0);
    }
    
    lastD=millis();
  }
  Serial.print("accf:\t");
  Serial.print(accf);
  Serial.print("\t;dir:\t");
  Serial.print(dirAng);
  Serial.print("\r\n");
  
}

//============================================================================================
//********************************************************************************************
//============================================================================================
//========================================SUBRUTINAS==========================================
//============================================================================================
int FIR(int x){//--------------------------------FIR------------------------------------------
    y=a[0]*x+p[0][0];                                               //yk(i)=a(1)*yt(i)+p(1,k-1);
    p[1][orden-1]=a[orden]*x-b[orden]*y;                            //p(orden,k)=a(orden+1)*yt(i)-b(orden+1)*yk(i);
    for(int iorden=0;iorden<orden-1;iorden++) {
      p[1][iorden]=p[0][iorden+1]+a[iorden+1]*x-b[iorden+1]*y;      //p(iorden,k)=p(iorden+1,k-1)+a(iorden+1)*yt(i)-b(iorden+1)*yk(i);
    }
    for(int iorden=0;iorden<orden;iorden++) {
      p[0][iorden]=p[1][iorden];
    }
    
    return y;
}
//----------------------------------------------*---------------------------------------------

void SerialInterrupt(char inChar){//-----------------EVENTO DE INTERRUPCION POR SERIAL-------------------
    //inChar=Serial.read();
    inputString+=inChar;
    if (inChar == '*'){//-------------CONCATENA MIENTRAS NO HAYA LLEGADO EL FIN DEL MENSAJE-------------------
      stringComplete=true;
    }
    if (stringComplete) {//---------------------PROCESA EL MENSAJE------------------------
      inputString.toCharArray(sz, inputString.length()) ;
      char *p = sz;
      char *str;
      while ((str = strtok_r(p, ";", &p)) != NULL){//---------RECORRE EL STRING CON UN PUNTERO-------------
        inputString =str;
        if(inputString.startsWith("$")){//------------------QUITA LA CABEZA DEL MENSAJE--------------------
          inputString.replace("$","");     
        }
        if(inputString.startsWith("A=")){//----------------LEE LA PRIMER VARIABLE DEL MENSAJE--------------
          inputString.replace("A=","");
          acc=inputString.toInt();
          if (acc>=0){  //AVANZAR
            acc=map(acc,thrAcc,1023,0,255);
            acc=constrain(acc,0,255);
          }
        }
        if(inputString.startsWith("B=")){//---------------LEE LA SEGUNDA VARIABLE DEL MENSAJE--------------
          inputString.replace("B=","");
          dirAng = inputString.toInt();
          dirAng = map(dirAng , 0 , 1023 , initAng + minAng , initAng + maxAng);
        }
      }
      inputString = "";//-----------------APAGA BANDERA PROPIA---------------
      stringComplete = false;
    }
}
