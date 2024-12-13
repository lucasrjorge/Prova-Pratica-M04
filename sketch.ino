#include <WiFi.h>
#include <HTTPClient.h>


#define led_verde 2 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

//Definição das especificações da rede
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6

const int buttonPin = 18;  // numero do pino do botão
int buttonState = 0;  // variável que lê o estado do botão

const int ldrPin = 4;  // número do pino do LDR
int threshold=600;

long lastDebounceTime = 0;  // tempo do último debounce
long debounceDelay = 50;    // o timer do buffer no debounce

int buttonPressCount = 0;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(buttonPin, INPUT); // Inicializando o botão como um input

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

   // Define o modo Wi-Fi como Station, ou seja, o ESP32 se conecta a um roteador
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST e senha 1234

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }

  // Verifica estado do botão
  if ( (millis() - lastDebounceTime) > debounceDelay) {
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      Serial.println("Botão pressionado!");
      buttonPressCount++;
    } else {
      Serial.println("Botão não pressionado!");
    }
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)
    
    //Imprime o ip da conexão
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str()); //Inicia a conexão http

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    //Se o código de resposta http for maior que 0
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload); //Imprimir código de resposta
      }
    else {
      Serial.print("Error code: "); //Se não, imprimir código de erro
      Serial.println(httpResponseCode);
      }
      http.end(); //e finalizar a conexão http
    }

//Se não estiver, imprime mensagem de desconexão 
  else {
    Serial.println("WiFi Disconnected"); 
  }
}

//Função para gerenciar o loop do led amarelo (quando está escuro)
void PiscarAmarelo()
{
    digitalWrite(led_amarelo, HIGH);
    delay(1000);
    digitalWrite(led_amarelo, LOW);
}

//Função para gerenciar os loops dos leds quando estiver claro
void ModoClaro()
{
  digitalWrite(led_verde, HIGH);
  delay(3000);
  digitalWrite(led_verde, LOW);

  digitalWrite(led_amarelo, HIGH);
  delay(2000);
  digitalWrite(led_amarelo, LOW);

  digitalWrite(led_vermelho, HIGH);
  delay(5000);
  digitalWrite(led_vermelho, LOW);

//Com debounce, se o led vermelho estiver ligado e o botão for pressionado, led vermelho desliga e verde liga
if ((millis() - lastDebounceTime) > debounceDelay) { 
  if(led_vermelho == HIGH && buttonState == HIGH)
      {
        delay(1000);
        digitalWrite(led_vermelho, LOW);
        digitalWrite(led_verde, HIGH);
      }
    
    //Se o led vermelho estiver acesso e o botão for pressionado 3 vezes faz um GET
    if(led_vermelho == HIGH && buttonPressCount == 3)
    {
      HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str()); //Inicia a conexão http

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP
      //Se o código de resposta http for maior que 0
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload); //Imprimir código de resposta
        }
      else {
        Serial.print("Error code: "); //Se não, imprimir código de erro
        Serial.println(httpResponseCode);
        }
        http.end(); //e finalizar a conexão http
    }
  }
}

//Loop que verifica o status do LDR e imprime diferenges mensagem de acordo com ele
void loop() {
  int ldrstatus=analogRead(ldrPin);

  //Se estiver escuro imprime mensagem de ligar led
  if(ldrstatus<=threshold){
    Serial.print("its dark turn on led");
    PiscarAmarelo();
    Serial.println(ldrstatus);

  //Se estiver claro imprime mensagem de desligar led
  }else{
    Serial.print("its bright turn off light");
    ModoClaro();
    Serial.println(ldrstatus);
  }
}