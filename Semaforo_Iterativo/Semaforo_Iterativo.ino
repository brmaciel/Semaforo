#include <LiquidCrystal.h>

int carro_verde = 12;
int carro_amarelo = 11;
int carro_vermelho = 10;
int ped_verde = 9;
int ped_vermelho = 8;
int botao = 2;
int buzzer = 4;
int EstadoBotao = 0;
unsigned long tempo_espera;
LiquidCrystal lcd(0, 1, 3, 5, 6, 7);

void setup()
{
  pinMode (carro_verde, OUTPUT);
  pinMode (carro_amarelo, OUTPUT);
  pinMode (carro_vermelho, OUTPUT);
  pinMode (ped_verde, OUTPUT);
  pinMode (ped_vermelho, OUTPUT);
  pinMode (botao, INPUT);
  pinMode (buzzer, OUTPUT);
  lcd.begin(16, 2);
  
  digitalWrite (carro_verde, HIGH);
  digitalWrite (ped_vermelho, HIGH);
}

void loop ()
{
  lcd.setCursor(0,0);
  lcd.print("Aperte o botao"); //mostra no display a mensagem
  lcd.setCursor(0,1);
  lcd.print("para atravessar"); //"Aperte o botao para atravessa"
  EstadoBotao = digitalRead(botao);
  if (EstadoBotao == HIGH && millis()-tempo_espera>=20000) //condicao para acionamento imediato
  {
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Aguarde"); //exibe mensagem "aguarde"
    aciona(); //mudanca de luzes
  }
  else if (EstadoBotao == HIGH && millis()-tempo_espera<20000) //condicao de espera pra acionamento
  {
    lcd.clear(); //limpa a mensagem "aperte um botao para atravessar
    lcd.setCursor(4,0);
    lcd.print("Aguarde"); //exibe mensagem "aguarde"
    lcd.setCursor(7,1);
    lcd.print("0");
    mudatempo(7); delay(1000); //contagem de tempo para abrir o sinal
    mudatempo(6); delay(1000);
    mudatempo(5); delay(1000);
    mudatempo(4); delay(1000);
    mudatempo(3); delay(1000);
    mudatempo(2); delay(1000);
    mudatempo(1); delay(1000);
    mudatempo(0);
    aciona(); //mudanca de luzes
  }
}

void aciona ()
{
  digitalWrite (carro_verde, LOW);
  digitalWrite (carro_amarelo, HIGH); //sinal amarelo pra carros por 2seg
  delay(2000);
  digitalWrite (carro_amarelo, LOW);
  digitalWrite (carro_vermelho, HIGH); //sinal de carros muda para vermelho
  delay(1000);
  digitalWrite (ped_vermelho, LOW);
  atravessar();
  mudatempo(0); //sinal de pedestres ira fechar
  digitalWrite (ped_verde, LOW);
  digitalWrite (ped_vermelho, HIGH); //sinal de pedestres fecha
  lcd.clear();                          //retira a mensagem de atravessar
  lcd.setCursor(7,1);  lcd.print("00"); //e exibe apenas o tempo 00
  delay(1000);
  digitalWrite (carro_vermelho, LOW);
  digitalWrite (carro_verde, HIGH); //sinal de carros abre
  tempo_espera = millis();
}

void atravessar()
{
  digitalWrite (ped_verde, HIGH); //sinal de pedestres abre
  lcd.setCursor(3,0);
  lcd.print("Atravesse"); //exibe mensagem "Atrevesse"
  lcd.setCursor(7,1);
  lcd.print(0);
  mudatempo(5); //exibe mensagem que falta 5 seg para fechar o sinal de pedestres
  for (int y=0; y<15; y++) //luz verde pedestre aceso constante e buzzer apita por 3 seg
  {
    digitalWrite (buzzer, HIGH);
    delay(100);
    digitalWrite (buzzer, LOW);
    delay(100);
    if (y == 4)
    {    mudatempo(4);    } //falta 4seg para fechar o sinal de pedestres
    if (y == 9)
    {    mudatempo(3);    } //falta 3seg para fechar o sinal de pedestres
  }
  mudatempo(2); //falta 2seg para fechar o sinal de pedestres
  for (int x=0; x<5; x++) //pisca luz verde de pedestre e buzzer acelerado por 2 seg
  {
    digitalWrite (ped_verde, LOW);
    for (int z=0; z<2;z++)
    { 
      digitalWrite (buzzer, HIGH);
      delay(50);
      digitalWrite (buzzer, LOW);
      delay(50);
    }
    if (x == 2)
    {    mudatempo(1);    } //falta 1seg para fechar o sinal de pedestres
    digitalWrite (ped_verde, HIGH);
    for (int z=0; z<2;z++)
    { 
      digitalWrite (buzzer, HIGH);
      delay(50);
      digitalWrite (buzzer, LOW);
      delay(50);
    }
  }
}

void mudatempo(int a) //funcao para exibir no display o tempo
{
  lcd.setCursor(8,1);
  lcd.print(a);
}

