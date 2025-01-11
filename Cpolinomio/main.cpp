#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

const char* ssid = "Nome da rede";
const char* password = "Senha da Rede";

ESP8266WebServer server(80);

String resultado = "";

void calcularMonomioPorMonomio(float coefficient, int exponent) {
  if (coefficient != 0) {
    if (resultado != "") {
      resultado += (coefficient > 0) ? " +" : " ";
    }
    resultado += (coefficient == static_cast<int>(coefficient)) ? String(static_cast<int>(coefficient)) : String(coefficient);

    if (exponent != 0) {
      resultado += "x";
      resultado += (exponent != 1) ? "<sup>" + String(exponent) + "</sup>" : "";
    }
  }
}

void handleRoot() {
  String content =
    "<!DOCTYPE html>"
    "<html html=\"pt-BR\">"

    "<head>"
      "<meta charset=\"utf-8\">"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
      "<title>Cálculoo de Polinômios</title>"
      "<style>"
        "body {"
          "font-family: \"Arial\", sans-serif;"
          "background-color: #F0F8FF;"
          "text-align: center;"
        "}"
        ".container {"
          "margin: 20px auto;"
          "max-width: 400px;"
          "padding: 20px;"
          "background-color: #FAFAFA;"
          "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.05);"
        "}"
        "input {"
          "width: 100%;"
          "padding: 10px;"
          "margin: 10px 0;"
          "border: 1px solid #ccc;"
          "border-radius: 5px;"
        "}"
        "input[type=\"submit\"] {"
          "background-color: #4CAF50;"
          "color: white;"
        "}"
      "</style>"
    "</head>"

    "<body>"
      "<div class=\"container\">"
        "<h1>Cálculo de Polinômios</h1>"
        "<form action=\"/soma\" method=\"get\">"
          "<input type=\"text\" name=\"polinomioP\" placeholder=\"Polinômio P(x)\">"
          "<br>"
          "<input type=\"text\" name=\"polinomioQ\" placeholder=\"Polinômio Q(x)\">"
          "<br>"
          "<input type=\"submit\" value=\"Calcular\">"
        "</form>"
      "</div>"
    "</body>"

    "</html>"
    "";
server.send(200, "text/html", content);
}

void handleSoma() {
  resultado = "";
  String polinomioP = server.arg("polinomioP");
  String polinomioQ = server.arg("polinomioQ");

  float soma[4] = { 0 };

  float p[4] = { 0 };
  float q[4] = { 0 };

  sscanf(polinomioP.c_str(), "%fx^3 %fx^2 %fx %f", &p[3], &p[2], &p[1], &p[0]);
  sscanf(polinomioQ.c_str(), "%fx^3 %fx^2 %fx %f", &q[3], &q[2], &q[1], &q[0]);

  for (int i = 0; i < 4; i++) {
    soma[i] = p[i] + q[i];
  }
 
  for (int i = 3; i >= 0; i--) {
    calcularMonomioPorMonomio(soma[i], i);
  }
  if (resultado == "") {
    resultado = "0";
  }
  Serial.println("valor de resultado: " + resultado);
 
  String content =
    "<!DOCTYPE html>"
    "<html lang=\"pt-BR\">"

    "<head>"
      "<meta charset=\"UTF-8\">"
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"

      "<title>Resultado</title>"
      "<style>"
        "body {"
          "font-family: \"Arial\", sans-serif;"
          "background-color: #F0F8FF;"
          "text-align: center;"
        "}"

        ".container {"
          "margin: 20px auto;"
          "max-width: 400px;"
          "padding: 20px;"
          "background-color: #FAFAFA;"
          "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.05);"
        "}"
      "</style>"
    "</head>"

    "<body>"
      "<div class=\"container\">"
        "<h1>Resultado</h1>"
        "<p>"+
        resultado
        + "</p>"
      "</div>"
    "</body>"

    "</html>";
server.send(200, "text/html", content);
}

void setup() {
  Serial.begin(115200);
  Serial.print("\nConectando ao WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi!");
  Serial.println("Endereço IP do dispositivo: ");
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, handleRoot);
  server.on("/soma", HTTP_GET, handleSoma);
  server.begin();
  Serial.println("Servidor web iniciado!");
}
 
void loop() {
  server.handleClient();
}
