//#include <servo_pesc.h>
#include <IRremote.h>
#include <VarSpeedServo.h> // inclusão da bibliotecaa

VarSpeedServo servo_pesc;// criando objeto apartir da biblioteca (controle de pescoço horizontal)
VarSpeedServo servo_cab;// criando objeto apartir da biblioteca (controle de pescoço vertical)

VarSpeedServo Ant_bra_dir;
VarSpeedServo Ant_bra_esq;

VarSpeedServo Polegar_dir;
VarSpeedServo Indicador_dir;
VarSpeedServo Medio_dir;
VarSpeedServo Anelar_minimo_dir;

VarSpeedServo Polegar_esq;
VarSpeedServo Indicador_Medio_esq;
VarSpeedServo Anelar_minimo_esq;
 
IRsend irsend; //variável para poder enviar o código
 
#define Led_Rosto 13

#define RL1 A8            //RL1
#define RL2 A9            //RL2
#define Cint_dir A10      //RL3
#define Cint_esq A11      //RL4
#define Bra_esq_baixo A12 //RL5
#define Bra_esq_cima A13  //RL6
#define Bra_dir_baixo A14 //RL7
#define Bra_dir_cima A15  //RL8 
#define Led_R 53          //RL9
#define Led_G 49          //RL10
#define Led_B 51          //RL11
#define RL12 47           //RL12

//IR
const int RECV_PIN = 46;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
const int ledIR = 5; 



String inputString = "";         // a String to hold incoming data

bool stringComplete = false;  // whether the string is complete

int posicaoServo = 90;

int Cont_led = 0;//Controla cor leds

bool sw = false; //controla mãos Dir
bool sw1 = false; //controla mãos Dir
int sw2 = 0; //controla cintura de 90 vai a 0 ou 180;
bool liga = HIGH;//controla tomada externa



void setup() {
  
  // initialize serial:
  Serial.begin(115200);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  //IR
  irrecv.enableIRIn();
  //irrecv.blink13(true);pisca led pino 13 ao receber sinal do controle remoto
  
  pinMode(ledIR ,OUTPUT); // saída do infravermelho 
  pinMode(Led_Rosto, OUTPUT); // acende LED pino 13 quando localiza um rosto
  
  //Coloca os relés para pinos de saida
  pinMode(RL1, OUTPUT);          //RL1
  pinMode(RL2, OUTPUT);          //RL2
  pinMode(Cint_dir, OUTPUT);      //RL3
  pinMode(Cint_esq, OUTPUT);      //RL4
  pinMode(Bra_esq_baixo, OUTPUT); //RL5
  pinMode(Bra_esq_cima, OUTPUT);  //RL6
  pinMode(Bra_dir_baixo, OUTPUT);//RL7
  pinMode(Bra_dir_cima, OUTPUT); //RL8 
  pinMode(Led_R, OUTPUT);        //RL9
  pinMode(Led_G, OUTPUT);        //RL10
  pinMode(Led_B, OUTPUT);        //RL11
  pinMode(RL12, OUTPUT);         //RL12

   //Deixa todos desligados relés ao iniciar
  
 digitalWrite(RL1, HIGH);           //RL1 Tomada Ac
 digitalWrite(RL2, HIGH);           //RL2 Amplificicador
 digitalWrite(Cint_dir, HIGH);      //RL3
 digitalWrite(Cint_esq, HIGH);      //RL4
 digitalWrite(Bra_esq_baixo, HIGH); //RL5
 digitalWrite(Bra_esq_cima, HIGH);  //RL6
 digitalWrite(Bra_dir_baixo, HIGH); //RL7
 digitalWrite(Bra_dir_cima, HIGH); //RL8 
 digitalWrite(Led_R, HIGH);        //RL9
 digitalWrite(Led_G, HIGH);        //RL10
 digitalWrite(Led_B, LOW);        //RL11
 digitalWrite(RL12, HIGH);         //RL12 //LED Automático / Manual



///Reset o Robô posições iniciais

delay(3000);

digitalWrite(RL2, LOW);           //RL2 Amplificicador liga o amplificador

//servo_cab.attach(3); 
servo_pesc.attach(2);


//posicaoServo=90;

servo_cab.slowmove(70,5); //posição, velocidade

servo_pesc.slowmove(90, 5); //posição, velocidade   

Bra_Esq(0, 4000, 0 );//Desce mão esquerda

Bra_Dir(0, 4000, 0 );//Desce mão Direita


cintura(90, 1);



}

void loop() {
  

  //IR
 
 if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        switch(results.value){
          case 0xFFA25D:
          Serial.println("Comando CH-");
          break;
          case 0xFF629D:
          Serial.println("Comando CH");
          break;
          case 0xFFE21D:
          Serial.println("Comando CH+");
          break;
          case 0xFF22DD:
          Serial.println("Comando séta direita 2|<<");
          break;
          case 0xFF02FD:
          Serial.println("Comando séta esquerda 2 >>|");
          break ;  
          case 0xFFC23D:
          Serial.println("Comando séta esquerda 1 >|");
          break ;               
          case 0xFFE01F:
          Serial.println("Comando -");
          break ;  
          case 0xFFA857:
          Serial.println("Comando +");
          break ;  
          case 0xFF906F:
          Serial.println("Comando EQ");
          break ;  
          case 0xFF6897:
          Serial.println("Comando 0");
          
            demo(0, 1);

             break ; 
           
          case 0xFF9867:
         Serial.println("Comando 100+");

     apresenta(1);

           break ;
          
          case 0xFFB04F:
          Serial.println("Comando 200+");
          break ;
          
          case 0xFF30CF:
          Serial.println("Comando 1");
            
demo(1, 1);
         
               break ;
          
          case 0xFF18E7:
          Serial.println("Comando 2");

demo(2, 1);        
          
          break ;
          
          case 0xFF7A85:
          Serial.println("Comando 3");
        
demo(3, 1);
        
          break ;

          
          case 0xFF10EF:
          Serial.println("Comando 4");

demo(4, 1);         
         
          break ;
          case 0xFF38C7:
          
          Serial.println("Comando 5");

demo(5, 1);        
          
          break ;
          case 0xFF5AA5:
          Serial.println("Comando 6");

demo(6, 1);
          
          break ;
          case 0xFF42BD:
          Serial.println("Comando 7");

demo(7, 1);
                    
 //irsend.sendNEC(0xF7C03F, 32);// código que sera enviado ON FITA LED
         //  servo.slowmove(0, 10); //posição, velocidade 

           
          break ;
          case 0xFF4AB5:
          Serial.println("Comando 8");

demo(8, 0);
     
          break ;
          
          case 0xFF52AD:
         
           Serial.println("Comando 9");


 demo(9, 1);          
          break ; 


               
        }



        
        key_value = results.value;
        irrecv.resume(); 
  }




 /////////////////fim ir
  // print the string when a newline arrives:
  if (stringComplete) {
    // clear the string:

    //--------------- Controle dos Leds ---------------
    

 
    //--------------- Controle do Led Amarelo ---------------
    
    if(inputString.startsWith("ligar led azul")){
     //digitalWrite(Led_Y, !digitalRead(LED_BUILTIN));// toggle
    Serial.print("lédi azul ligado!!\n");
    digitalWrite(Led_B, LOW);}

      
    if(inputString.startsWith("desligar led azul")){
      Serial.print("lédi azul desligado!!\n");
      digitalWrite(Led_B, HIGH);}



    //--------------- Controle do Led Verde ---------------
    
    if(inputString.startsWith("ligar led verde")){
      digitalWrite(Led_G, LOW);
      Serial.print("lédi verde ligado!!\n");}

      
    if(inputString.startsWith("desligar led verde")){
      digitalWrite(Led_G, HIGH);
      Serial.print("lédi verde desligado!!\n");}

      

    //--------------- Controle do Led Vermelho ---------------
    
    if(inputString.startsWith("ligar led vermelho")){
      digitalWrite(Led_R, LOW);
      Serial.print("lédi vermelho ligado!!\n");}

      
    if(inputString.startsWith("desligar led vermelho")){
      digitalWrite(Led_R, HIGH);
      Serial.print("lédi vermelho desligado!!\n");}


    //--------------- Controle de todos os Leds ---------------
    
    if(inputString.startsWith("ligar todos os leds")){
      digitalWrite(Led_R, LOW);
      digitalWrite(Led_G, LOW);
      digitalWrite(Led_B, LOW);
      Serial.print("todos os lédis ligados!!\n");}

      
    if(inputString.startsWith("desligar todos os led")){
      
      digitalWrite(Led_R, HIGH);
      digitalWrite(Led_B, HIGH);
      digitalWrite(Led_G, HIGH);
      
      Serial.print("Todos os lédis desligados!!\n");}

      
      

if(inputString.startsWith("apresentação")){
apresenta(1);
}




     //--------------------------------------


      
    if(inputString.startsWith("servo")){

      digitalWrite(Led_Rosto, !digitalRead(Led_Rosto));//pisca Led quando rosto é indentificado

      int moveCamera = inputString.substring(5).toInt();

      if(moveCamera >= 1) moveCamera = 1;
      else if(moveCamera <= -1) moveCamera = -1;

      posicaoServo += moveCamera;
      
      if(posicaoServo > 180) posicaoServo = 180;
      else if(posicaoServo <=0) posicaoServo = 0;
  //    posicaoServo = map(posicaoServo, 0, 180, 180, 0);
      servo_pesc.write(posicaoServo);
    } else{
      Serial.print(inputString);
    }

    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  
}



void cintura(int ang, bool fala )//vai para o Ângulo determinado
{
 int  pot_cint;
 int  cont = 0;
  
 pot_cint = map(analogRead(A0), 0, 1023, 0, 180);
 
 if (pot_cint < ang)//se menor de 90 está a direita então vá a esquerda 
 {
while (pot_cint < ang || cont == 250) 
    {
      
pot_cint = map(analogRead(A0), 0, 1023, 0, 180);
//Serial.println("Cintura para esquerda");
digitalWrite(Cint_dir, HIGH);      //RL3
digitalWrite(Cint_esq, LOW);      //RL4
if (fala == 1) Serial.print( "Posição :"); 
if (fala == 1)Serial.println( pot_cint);
if (fala == 1) Serial.println( ang);
if (fala == 1) Serial.println( cont);
cont = cont + 1;
//delay(300);

    }
    }
    
 if (pot_cint > ang || cont == 250) 
{
while (pot_cint > ang) 
{
pot_cint = map(analogRead(A0), 0, 1023, 0, 180);
//Serial.println("Cintura para direita");
digitalWrite(Cint_dir, LOW);      //RL3
digitalWrite(Cint_esq, HIGH);      //RL4
if (fala == 1) Serial.print( "Posição: "); 
if (fala == 1) Serial.println( pot_cint);
if (fala == 1) Serial.println( ang);
if (fala == 1) Serial.println( cont);
cont = cont + 1;
//delay(300);
}

}    
digitalWrite(Cint_dir, HIGH);      //RL3
digitalWrite(Cint_esq, HIGH);      //RL4
cont = 0;
//Serial.println("Cintura parada");

 

  }

//Início da Função (Controle Braço Direito)

void Bra_Dir(int dir, int temp, bool fala )
//Função braço Direito dir 1=Para cima 2= Para baixo temp = tempo ligado em Milissegundo 
{
      if (dir == 1)
      {
if (fala == 1) Serial.println("Braço Direito para cima");
// Serial.print(temp);
// Serial.println(" ms"); //tempo ligado em Milissegundo   
        
digitalWrite(Bra_dir_cima, LOW);
digitalWrite(Bra_dir_baixo, HIGH);
         
delay(temp);

digitalWrite(Bra_dir_cima, HIGH);
digitalWrite(Bra_dir_baixo, HIGH);
}

 if (dir == 0)
      {
if (fala == 1) Serial.println("Braço Direito para baixo");
//Serial.print(temp);
//Serial.println(" ms"); //tempo ligado em Milissegundo   
        
digitalWrite(Bra_dir_cima, HIGH);
digitalWrite(Bra_dir_baixo, LOW);
         
delay(temp);

digitalWrite(Bra_dir_cima, HIGH);
digitalWrite(Bra_dir_baixo, HIGH);
}

}
//Fim função de braço direito


//Início da Função (Controle Braço Esquerda)

void Bra_Esq(int dir, int temp, bool fala )
//Função braço Esquerdo dir 1=Para cima 2= Para baixo temp = tempo ligado em Milissegundo 
{
      if (dir == 1)
      {
if (fala == 1) Serial.println("Braço Esquerdo para cima");
//Serial.print(temp);
//Serial.println(" ms"); //tempo ligado em Milissegundo   
        
digitalWrite(Bra_esq_cima, LOW);
digitalWrite(Bra_esq_baixo, HIGH);
         
delay(temp);

digitalWrite(Bra_esq_cima, HIGH);
digitalWrite(Bra_esq_baixo, HIGH);
}

 if (dir == 0)
      {
if (fala == 1) Serial.println("Braço Esquerdo para baixo");
//if (fala == 1) Serial.print(temp);
//if (fala == 1) Serial.println(" ms"); //tempo ligado em Milissegundo   
        
digitalWrite(Bra_esq_cima, HIGH);
digitalWrite(Bra_esq_baixo, LOW);
         
delay(temp);

digitalWrite(Bra_esq_cima, HIGH);
digitalWrite(Bra_esq_baixo, HIGH);
}

}
//Fim função de braço Esquerdo



 //Início função Demonstração



void Led_Control()
{
  int tempo = 500;

   Cont_led = Cont_led+1;
      
   //Serial.println(Cont_led);
         
switch (Cont_led) {
  
  case 1:
    // comando(s)
 digitalWrite(RL12, HIGH);  //modo manual  
 digitalWrite(Led_R, HIGH);        //RL9
 digitalWrite(Led_G, HIGH);        //RL10
 digitalWrite(Led_B, LOW);        //RL11
Serial.println("Cor Azul");
 delay(tempo);
    break;
    
  case 2:
    // comando(s)
 digitalWrite(RL12, HIGH);  //modo manual
 digitalWrite(Led_R, LOW);        //RL9
 digitalWrite(Led_G, HIGH);        //RL10
 digitalWrite(Led_B, HIGH);        //RL11
 Serial.println("Cor Vermelha");
     delay(tempo);
    break;

  case 3:
    // comando(s)
 digitalWrite(RL12, HIGH);  //modo manual
 digitalWrite(Led_R, HIGH);        //RL9
 digitalWrite(Led_G, LOW);        //RL10
 digitalWrite(Led_B, HIGH);        //RL11
 Serial.println("Cor Verde");
     delay(tempo);
    break;

  case 4:
    // comando(s)
 digitalWrite(RL12, HIGH);  //modo manual
 digitalWrite(Led_R, LOW);        //RL9
 digitalWrite(Led_G, LOW);        //RL10
 digitalWrite(Led_B, HIGH);        //RL11
 Serial.println("Cor Amarelo");
     delay(tempo);
    break;


  case 5:
    // comando(s)
 digitalWrite(RL12, HIGH);  //modo manual
 digitalWrite(Led_R, HIGH);        //RL9
 digitalWrite(Led_G, LOW);        //RL10
 digitalWrite(Led_B, LOW);        //RL11
 Serial.println("Cor Ciano");
     delay(tempo);
    break;

  case 6:
    // comando(s)
 digitalWrite(RL12, HIGH);  //modo manual
 digitalWrite(Led_R, LOW);        //RL9
 digitalWrite(Led_G, HIGH);        //RL10
 digitalWrite(Led_B, LOW);        //RL11
Serial.println("Cor Roxa");
 delay(tempo);
    break;

  case 7:
    // comando(s)
 digitalWrite(RL12, HIGH);  //modo manual
 digitalWrite(Led_R, LOW);        //RL9
 digitalWrite(Led_G, LOW);        //RL10
 digitalWrite(Led_B, LOW);        //RL11
 Serial.println("Cor Branca");
     delay(tempo);
    break;

  case 8:
    // comando(s)
 digitalWrite(RL12, HIGH);  //modo manual
 digitalWrite(Led_R, HIGH);        //RL9
 digitalWrite(Led_G, HIGH);        //RL10
 digitalWrite(Led_B, HIGH);        //RL11
 Serial.println("Léds Desligados");
     delay(tempo);
    break;


  case 9:
    // comando(s) //Comandos automáticos
    
    
 digitalWrite(Led_R, HIGH);        //RL9
 digitalWrite(Led_G, HIGH);        //RL10
 digitalWrite(Led_B, HIGH);        //RL11
 
     delay(tempo);
     
digitalWrite(RL12, LOW);           //RL12 Led auto/manu 
  Serial.println("Módo Automático use o Controle Remoto dos léds");  
    break;
     


    
  default:
    // comando(s)
    Cont_led = 0;
     delay(tempo);
      break;

   Cont_led = Cont_led+1;
}
  }
 
  
void demo(int modo, bool fala)//Modo demonstração modo 0 = todos
  {

// Demo 1 - Demo Servo cabeça Horizontal e vertical
    
if(modo == 1)//Demo Servo cabeça Horizontal e vertical
 {
 int temp = 3000;
 int veloc = 5; 
 
if (fala == 1) Serial.println("Mexendo a cabeça"); 

servo_pesc.slowmove(45, veloc); //posição, velocidade 
delay(temp);

servo_pesc.slowmove(90, veloc); //posição, velocidade 
delay(temp);

servo_pesc.slowmove(135, veloc); //posição, velocidade 
delay(3000);

servo_pesc.slowmove(90, veloc); //posição, velocidade 
delay(temp);

//servo_cab.slowmove(100, veloc); //posição, velocidade
delay(3000);

//servo_cab.slowmove(90, veloc); //posição, velocidade
delay(3000);

//servo_cab.slowmove(110, veloc); //posição, velocidade
delay(3000);


//servo_cab.slowmove(30, veloc); //posição, velocidade
delay(3000);

//Fim do teste de servo cabeça Horizontal e vertical




  }
//Final demo 1  Demo Servo cabeça Horizontal e vertical


// 2 - Controle Antebraço Direito
  
if(modo == 2) //Controle braço Direito
 {
if (fala == 1)  Serial.println("Controle do Antebraço Direito"); 

int pos = 20;   
int temp = 3000;
int vel = 30;

// define pinos do servo dedos direito

Ant_bra_dir.attach(9);

delay(2000);

Ant_bra_dir.slowmove(20, vel);//minimo seguro
 delay(temp);
 
 
Ant_bra_dir.slowmove(120, vel);//maximo seguro
delay(temp);

//desliga servos
Ant_bra_dir.detach();

  }
  //Fim Controle Antebraço Direito -2


// 3 - Controle Antebraço Esquerdo
  
if(modo == 3) //Controle Antebraço Esquerdo
 {
if (fala == 1) Serial.println("Controle do Antebraço Esquerdo"); 

int pos = 20;   
int temp = 2000;
int vel = 20;

// define pinos do servo dedos direito

Ant_bra_dir.attach(4);

delay(2000);

Ant_bra_dir.slowmove(20, vel);//mínimo
 delay(temp);
 
 
Ant_bra_dir.slowmove(120, vel); // máximo
delay(temp);



Ant_bra_dir.detach();

  }
  //Fim Controle Antebraço Esquerdo -3





  
//4 - Controle dos Dedos da mão Esquerda

if(modo == 4)//Controle dos Dedos da mão Esquerda

 {

int temp = 2000;
int vel = 50;

// define pinos do servo dedos Esquerdo

Polegar_esq.attach(8); 
Anelar_minimo_esq.attach(7);
Indicador_Medio_esq.attach(6);

delay(1000);

if (fala == 1) Serial.println("Dedos mão esquerda");
 
//fecha todos os dedos 

Indicador_Medio_esq.slowmove(0, vel);
 delay(temp);

Anelar_minimo_esq.slowmove(0, vel);
 delay(temp);
  
Polegar_esq.slowmove(0, vel);
 delay(temp);

//abre todos os dedos 

Anelar_minimo_esq.slowmove(180, vel);
 delay(temp);
  
Indicador_Medio_esq.slowmove(180, vel);
 delay(temp);
    
Polegar_esq.slowmove(180, vel);
 delay(temp);



delay(2000);
Polegar_esq.detach(); 
Anelar_minimo_esq.detach();
Indicador_Medio_esq.detach();

 }
 
//Fim controle dos dedos da mão esquerda - 4



//5 - Controle dos Dedos da mão Direita

if(modo == 5)//Controle dos Dedos da mão Direita

 {

int temp = 2000;
int vel = 50;

// define pinos do servo dedos Direita

Polegar_dir.attach(45); 
Anelar_minimo_dir.attach(12);
Medio_dir.attach(11);
Indicador_dir.attach(10);

delay(1000);

if (fala == 1) Serial.println("Controlando Dedos mão Direita");
 
//Fecha Todos os dedos

Indicador_dir.slowmove(0, vel); //0 = fecha 180 = abre vel = 50
 
delay(temp);

Medio_dir.slowmove(0, vel);//0 = fecha 180 = abre vel = 50
 delay(temp);

Anelar_minimo_dir.slowmove(0, vel); //0 = fecha 180 = abre vel = 50
 delay(temp);
  
 Polegar_dir.slowmove(0, vel);//0 = fecha 180 = abre vel = 50
 delay(temp);
  
//Abre Todos os dedos

Indicador_dir.slowmove(180, vel); //0 = fecha 180 = abre vel = 50
 
delay(temp);

Medio_dir.slowmove(180, vel);//0 = fecha 180 = abre vel = 50
 delay(temp);

Anelar_minimo_dir.slowmove(180, vel); //0 = fecha 180 = abre vel = 50
 delay(temp);
  
 Polegar_dir.slowmove(180, vel);//0 = fecha 180 = abre vel = 50
 delay(temp);



// Desliga servos 
Polegar_dir.detach(); 
Anelar_minimo_dir.detach();
Medio_dir.detach();
Indicador_dir.detach();
 
 }   
//Fim Controle dos Dedos da mão Direita -5


//Controle do braço direito

if(modo == 6)//Controle braço Direito

 {


if (fala == 1) Serial.println("Controle braço Direito");

if (sw == false) Bra_Dir(1, 5000, 0 );//Sobe mão direita sem falar

if (sw == true)Bra_Dir(0, 5000, 0 );//Desce mão direita sem falar

sw = !sw;

 
 }
//Fim Controle do braço direito -6


//7- Controle do braço Esquerda

if(modo == 7)//Controle braço Esquerdo
 {

if (fala == 1)  Serial.println("Controle braço Esquerdo");

if (sw1 == false) Bra_Esq(1, 4000, 0 );//Sobe mão esquerda sem falar

if (sw1 == true) Bra_Esq(0, 4000, 0 );//Desce mão esquerda sem falar

sw1 = !sw1;

 }


//Controle da cintura

if(modo == 8)//Demo cintura

{
if (fala == 1) Serial.println("Controle de Cintura");
int temp = 1000;


if (sw2 == 0)cintura(90, 0);
if (sw2 == 1) cintura(70, 0);
if (sw2 == 2) cintura(110, 0);


sw2 = sw2+1;

if (sw2 >=3) sw2=0;
}

//Tomada Externa 9
if(modo == 9)//Demo de dedos e ant_braço esquerdo
 {

 if (fala == 1)  Serial.println("Controle da tomada");
 
liga = !liga;

 digitalWrite(RL1, liga);           //RL1 Tomada Ac
 
delay(1000);
  }
 //fim 9 Tomada Externa 9
    
//0-Controle dos Leds e automático /manual

if (modo == 0)//sem comando

 {

 if (fala == 1)  Serial.println("Controle dos Leds");
 
Led_Control();

 }
 }


void apresenta(int ap){


if (ap ==1) {

int tempo = 3000;

Serial.println("Começando a Apresentação ");

delay(tempo);

          Serial.println("Bem vindos ao Programa 4.0 e inteligência artificial ");
delay(tempo);

Serial.println("SOU UM VERDADEIRO LABORATÓRIO DE TECNOLOGIA.");

delay(tempo);

Serial.println("TODOS OS MEUS COMPONENTES FORAM PRIMEIRAMENTE DESENHADO");

 Serial.println("ATRAVÉS DE SOFTWARE DE DESENHO 2D E 3D PELOS ALUNOS E OS PROFESSORES");

delay(tempo);

 Serial.println("  COM MINHA AJUDATDOS PODERAM APRENDER LINGUAGEMS PaTHON E C++ NA PRÁTICA");

 delay(tempo);

 Serial.println(" COM BIBLITECAS LIVRES COMO POR EXEMPLO RECONHECIMENTO PARA DE IMAGENS E DE OBJETOS.");

delay(tempo);

Serial.println(" E É CLARO,  TAMBALHANDO  COM  INTELIGÊNCIA ARTIFICIAL E INTERNET DAS COISAS COM A IMPLEMENTAÇÃO DA INTERNET 5G");

delay(tempo);

Serial.println(" TUDO IRÁ FICAR MAIS RÁPIDO");

  delay(tempo);

  Serial.println("VISANDO O MELHORAMENTO CONTÍNUO, IRÃO CADA VEZ MAIS APLIMORAR MEUS COMPONENTES MECÂNICOS,");
  
   delay(tempo);

  
 Serial.println(" ELÉTRICOS ,ELETRÔNICOS E PESQUISANDO NOVOS MATERIAS, MAIS LEVES E RESISTENTES");

  delay(tempo);
 
   Serial.println("SEMPRE USANDO OS RECURSOS DO LABORATÓRIO FAB LAB DO PROGRAMA 4.0, COMO IMPRESSORAS 3D");

  delay(tempo);

   Serial.println("MÁQUINA DE CORTE À LASER, RaUTER CNC, PLOTTER DE RECORTE,  ALÉM DE TODAS AS FERRAMENTAS.");

  delay(tempo);

   
  Serial.println(" POR EXEMPLO A MINHA CABEÇA E MÃOS FORAM IMPRESSAS EM IMPRESSORA 3D");
    
   delay(tempo);

  Serial.println("JÁ ALGUMAS PARTES FORAM DESENHADA  E MANDADAS PARA SEREM CORTADAS EM AÇO CARBONO EM MARINGA");

  delay(tempo);

  Serial.println(" EU POSSUO 3 METROS E 5 CENTÍMETROS ");

  delay(tempo);

 Serial.println(" VAMOS CONHECER MINHAS HABILIDADES");

  delay(tempo);


 Serial.println("Posso controlar as cores dos meus Léds");

 tempo= 4000;


    Led_Control();
    delay(tempo);
   
   Led_Control();
    delay(tempo);
   
   Led_Control();
    delay(tempo);
   
   Led_Control();
    delay(tempo);
   Led_Control();
    delay(tempo);
   
   Led_Control();
    delay(tempo);
   
   Led_Control();
    delay(tempo);
   
   Led_Control();
    delay(tempo);
   
   Led_Control();
    delay(tempo);
   
   Led_Control();
   
      Led_Control();//LIGANDO LED AZUL

      digitalWrite(Led_R, HIGH);
      digitalWrite(Led_B, LOW);
      digitalWrite(Led_G, HIGH);
      
   Serial.println("Led Azul");


Serial.println("Mexendo a cabeça");
   demo(1,0);
delay(tempo);

Serial.println("Antebraço Direito");
   demo(2,0);
delay(tempo);   

Serial.println("Antebraço Esquerdo");
   demo(3,0);
delay(tempo);

Serial.println("Dedos da mão Esquerdo");
   demo(4,0);
delay(tempo);

Serial.println("Dedos da mão Direita");
   demo(5,0);
delay(tempo);


Serial.println("Braço direito");
 //  demo(6,0);
delay(tempo);

Serial.println("Braço Esquerdo");
 //  demo(7,0);
delay(tempo);

Serial.println("Tronco");
   demo(8,0);
delay(tempo);

Serial.println("Ligando máquinas de fabricação digital fab lab");
   demo(9,0);
delay(tempo);



   /////////////////////////////FIM DA APRESENTAÇÃO  
}

}
