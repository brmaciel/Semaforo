#include <LiquidCrystal.h>

/* Definição dos pinos usados */
int led_carroVerde = 12;
int led_carroAmarelo = 11;
int led_carroVermelho = 10;
int led_pedVerde = 9;
int led_pedVermelho = 8;
int botao = 2;
int buzzer = 4;
LiquidCrystal lcd(0, 1, 3, 5, 6, 7);

/* Definição de variáveis auxiliares */
int EstadoBotao = 0;
unsigned long tempo_espera;
const unsigned int delayAcionamentoImediato = 20000; // delay (milliseg) necessario esperar para o acionamento imadiato
const unsigned int tempo_aguardeParaAtravessar = 11;  // tempo (seg) que os pedestres devem aguardar para mudanca do semaforo no acionamento retardado
const unsigned int tempo_travessiaPedestres = 12;    // tempo que os pedestres tem para atravessar em seg

void acionaSemaforo () {
  /* Ciclo de Abertura e Fechamento do Semaforo
   * Semaforo para carros fecha -> Semaforo para pedestres abre
   * Semaforo para pedestres fecha -> Semaforo para carros abre
   */
  digitalWrite (led_carroVerde, LOW);
  digitalWrite (led_carroAmarelo, HIGH);  // sinal amarelo pra carros por 2seg
  delay(2000);
  digitalWrite (led_carroAmarelo, LOW);
  digitalWrite (led_carroVermelho, HIGH); // sinal de carros muda para vermelho
  delay(1000);                            // sinal de pedestres abrira 1seg após sinal de carros fechar
  atravessar();
  mudatempo(0);   // sinal de pedestres ira fechar
  digitalWrite (led_pedVerde, LOW);
  digitalWrite (led_pedVermelho, HIGH); //sinal de pedestres fecha
  lcd.clear();                          //retira a mensagem de atravessar
  lcd.setCursor(7,1);  lcd.print("00"); //e exibe apenas o tempo 00
  delay(1000);
  digitalWrite (led_carroVermelho, LOW);
  digitalWrite (led_carroVerde, HIGH); //sinal de carros abre
  tempo_espera = millis();
}

void atravessar() {
  /* Ciclo de Abertura e Fechamento do Semaforo de Pedestres */
  digitalWrite (led_pedVermelho, LOW);
  digitalWrite (led_pedVerde, HIGH);    // semaforo de pedestres abre
  lcd.setCursor(3,0);
  lcd.print("Atravesse");     // exibe mensagem "Atrevesse"
  
  for (int tempo = tempo_travessiaPedestres; tempo > 5; tempo--) {
    mudatempo(tempo);
    digitalWrite (buzzer, HIGH); delay(100);
    digitalWrite (buzzer, LOW);
    delay(900);
  }

  mudatempo(5);               // exibe mensagem que falta 5 seg para fechar o sinal de pedestres
  for (int y=0; y<15; y++) {  // luz verde pedestre aceso constante e buzzer apita por 3 seg
    digitalWrite (buzzer, HIGH); delay(100);
    digitalWrite (buzzer, LOW);  delay(100);
    if (y == 4)
    {    mudatempo(4);    } //falta 4seg para fechar o sinal de pedestres
    if (y == 9)
    {    mudatempo(3);    } //falta 3seg para fechar o sinal de pedestres
  }
  mudatempo(2);             // falta 2seg para fechar o sinal de pedestres
  for (int x=0; x<5; x++) {
    // pisca luz verde de pedestre e buzzer acelerado por 2 seg
    digitalWrite (led_pedVerde, LOW);
    buzzerAltaFrequencia();
    if (x == 2) {    mudatempo(1);    } // falta 1seg para fechar o sinal de pedestres
    digitalWrite (led_pedVerde, HIGH);
    buzzerAltaFrequencia();
  }
}

void buzzerAltaFrequencia() {
  // loop de 0.2seg total
  for (int z=0; z<2; z++) {
    digitalWrite (buzzer, HIGH); delay(50);
    digitalWrite (buzzer, LOW);  delay(50);
  }
}

void mudatempo(int tempo) {
  /* Funcao que exibe no display o tempo */
  if (tempo >=10) {
    lcd.setCursor(7,1);
    lcd.print(tempo);
  } else {
    lcd.setCursor(7,1);
    lcd.print("0");
    lcd.setCursor(8,1);
    lcd.print(tempo);
  }
}


/***** Funções Setup e Loop *****/
void setup() {
  pinMode (led_carroVerde, OUTPUT);
  pinMode (led_carroAmarelo, OUTPUT);
  pinMode (led_carroVermelho, OUTPUT);
  pinMode (led_pedVerde, OUTPUT);
  pinMode (led_pedVermelho, OUTPUT);
  pinMode (botao, INPUT);
  pinMode (buzzer, OUTPUT);
  lcd.begin(16, 2);
  
  digitalWrite (led_carroVerde, HIGH);
  digitalWrite (led_pedVermelho, HIGH);
}

void loop () {
  // exibe a mensagem no display "Aperte o botao para atravessa"
  lcd.setCursor(0,0);
  lcd.print("Aperte o botao");
  lcd.setCursor(0,1);
  lcd.print("para atravessar");
  
  EstadoBotao = digitalRead(botao);
  if (EstadoBotao == HIGH && millis()-tempo_espera >= delayAcionamentoImediato) {
    /* Acionamento Imediato
     * aciona o ciclo de mudança das luzes do semaforo imediatamente
     */
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Aguarde"); // exibe mensagem "Aguarde"
    acionaSemaforo();     // mudanca de luzes
  }
  else if (EstadoBotao == HIGH && millis()-tempo_espera < delayAcionamentoImediato) {
    /* Acionamento Retardado
     * ciclo do semaforo foi realizado recentemente, logo deve-se esperar um periodo para que se inicie novamente
     */
    lcd.clear();          // limpa a mensagem "aperte um botao para atravessar"
    lcd.setCursor(4,0);
    lcd.print("Aguarde"); // exibe mensagem "Aguarde"
    for (int tempo = tempo_aguardeParaAtravessar; tempo >= 0; tempo--) {
      mudatempo(tempo);
      delay(1000);
    }
    acionaSemaforo();     // mudanca de luzes
  }
}
