#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <DHT_U.h>

// --- Configurações do Wi-Fi ---
const char* ssid = "A16"; // 👈 Coloque o nome da sua rede aqui
const char* password = "12345678";   // 👈 Coloque a senha da sua rede aqui

// --- Configurações do Sensor DHT22 ---
#define DHTPIN 2      // Pino onde o sensor DHT está conectado
#define DHTTYPE DHT22 // Tipo do sensor
DHT dht(DHTPIN, DHTTYPE);

// --- Configurações do Servidor Web ---
WebServer server(80); // Cria um servidor na porta 80

// Variáveis globais para armazenar os dados do sensor
float temperatura = 0.0;
float umidade = 0.0;

// Função que será chamada quando alguém acessar o IP do ESP32
void handleRoot() {
  // Construindo a página HTML com CSS moderno
  String html = "<!DOCTYPE html><html lang='pt-BR'><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 Web Server</title>";
  html += "<meta http-equiv='refresh' content='10'>"; // Atualiza a página a cada 10 segundos
  html += "<style>";
  html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f4f4f9; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; min-height: 100vh; text-align: center; }";
  html += ".container { max-width: 800px; padding: 20px; }";
  html += "h1 { color: #333; margin-bottom: 30px; }";
  html += ".card-container { display: flex; flex-wrap: wrap; justify-content: center; gap: 30px; }";
  html += ".card { background-color: #ffffff; border-radius: 15px; box-shadow: 0 8px 16px rgba(0,0,0,0.1); padding: 25px; width: 220px; transition: transform 0.2s; }";
  html += ".card:hover { transform: translateY(-5px); }";
  html += ".card h2 { color: #555; margin-top: 0; font-size: 1.2em; }";
  html += ".sensor-value { color: #1a73e8; font-size: 3em; font-weight: bold; margin: 10px 0; }";
  html += ".unit { font-size: 1.5em; color: #555; }";
  html += ".icon { width: 50px; height: 50px; margin-bottom: 15px; }";
  html += "</style></head><body>";
  
  html += "<div class='container'>";
  html += "<h1>Monitor de Ambiente com ESP32</h1>";
  html += "<div class='card-container'>";
  
  // Card de Temperatura
  html += "<div class='card'>";
  html += "<h2>Temperatura</h2>";
  html += "<svg class='icon' viewBox='0 0 24 24' fill='#ff5722'><path d='M15 13V5c0-1.66-1.34-3-3-3S9 3.34 9 5v8c-1.21.91-2 2.37-2 4 0 2.76 2.24 5 5 5s5-2.24 5-5c0-1.63-.79-3.09-2-4zm-4-8c0-.55.45-1 1-1s1 .45 1 1v7.27c.58-.57 1.25-1.07 2-1.4V5c0-2.21-1.79-4-4-4S7 2.79 7 5v3.87c.75.33 1.42.83 2 1.4V5z'/></svg>";
  html += "<p class='sensor-value'>" + String(temperatura, 1) + "<span class='unit'>&deg;C</span></p>";
  html += "</div>";

  // Card de Umidade
  html += "<div class='card'>";
  html += "<h2>Umidade</h2>";
  html += "<svg class='icon' viewBox='0 0 24 24' fill='#2196f3'><path d='M12 2c-5.33 4.55-8 8.48-8 11.8 0 4.98 3.8 8.2 8 8.2s8-3.22 8-8.2c0-3.32-2.67-7.25-8-11.8zm0 18c-3.31 0-6-2.69-6-6 0-2.5 1.93-5.44 6-9.14 4.07 3.7 6 6.64 6 9.14 0 3.31-2.69 6-6 6z'/></svg>";
  html += "<p class='sensor-value'>" + String(umidade, 1) + "<span class='unit'>%</span></p>";
  html += "</div>";
  
  html += "</div></div></body></html>";
  
  server.send(200, "text/html", html); // Envia a página web para o navegador
}


void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando...");

  dht.begin(); // Inicia o sensor

  // --- Conectando ao Wi-Fi --- 📶
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereco IP: ");
  Serial.println(WiFi.localIP()); // Imprime o endereço IP do ESP32

  // --- Configurando o Servidor Web ---
  server.on("/", handleRoot); // Define a função 'handleRoot' para a página principal "/"
  server.begin(); // Inicia o servidor
  Serial.println("Servidor web iniciado!");
}

void loop() {
  // Lê os dados do sensor
  umidade = dht.readHumidity();
  temperatura = dht.readTemperature();

  // Verifica se a leitura foi bem-sucedida
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha ao ler o sensor DHT!");
  } else {
    // Imprime os valores no Monitor Serial
    Serial.print("Temp: ");
    Serial.print(temperatura);
    Serial.print("°C\t");
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println("%");
  }

  server.handleClient(); // Verifica se algum cliente (navegador) fez uma requisição
  delay(2000); // Espera 2 segundos antes da próxima leitura
}