#include <httplib.h>
#include <iostream>
#include <regex>
#include <sstream>

using namespace httplib;
using namespace std;

string calcularMonomioPorMonomio(float coefficient, int exponent) {
    stringstream resultado;
    if (coefficient != 0) {
        if (coefficient > 0 && exponent != 3) {
            resultado << " + ";
        } else if (coefficient < 0) {
            resultado << " ";
        }
        resultado << (coefficient == (int)coefficient ? to_string((int)coefficient) : to_string(coefficient));
        if (exponent != 0) {
            resultado << "x";
            if (exponent != 1) {
                resultado << "^" << exponent;
            }
        }
    }
    return resultado.str();
}

string processarPolinomios(const string &polinomioP, const string &polinomioQ) {
    float soma[4] = {0};
    float p[4] = {0};
    float q[4] = {0};

    // Regex para extrair coeficientes
    sscanf(polinomioP.c_str(), "%fx^3 %fx^2 %fx %f", &p[3], &p[2], &p[1], &p[0]);
    sscanf(polinomioQ.c_str(), "%fx^3 %fx^2 %fx %f", &q[3], &q[2], &q[1], &q[0]);

    for (int i = 0; i < 4; i++) {
        soma[i] = p[i] + q[i];
    }

    string resultado;
    for (int i = 3; i >= 0; i--) {
        resultado += calcularMonomioPorMonomio(soma[i], i);
    }

    return (resultado.empty()) ? "0" : resultado;
}

int main() {
    Server server;

    // Rota para página inicial
    server.Get("/", [](const Request &, Response &res) {
        ifstream file("index.html");
        stringstream buffer;
        buffer << file.rdbuf();
        res.set_content(buffer.str(), "text/html");
    });

    // Rota para soma de polinômios
    server.Get("/soma", [](const Request &req, Response &res) {
        auto polinomioP = req.get_param_value("polinomioP");
        auto polinomioQ = req.get_param_value("polinomioQ");

        string resultado = processarPolinomios(polinomioP, polinomioQ);

        stringstream content;
        content << "<!DOCTYPE html>"
                << "<html lang=\"pt-BR\">"
                << "<head>"
                << "<meta charset=\"UTF-8\">"
                << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                << "<title>Resultado</title>"
                << "<link rel=\"stylesheet\" href=\"style.css\">"
                << "</head>"
                << "<body>"
                << "<div class=\"container\">"
                << "<h1>Resultado</h1>"
                << "<p>" << resultado << "</p>"
                << "<a href=\"/\">Voltar</a>"
                << "</div>"
                << "</body>"
                << "</html>";

        res.set_content(content.str(), "text/html");
    });

    cout << "Servidor rodando em http://localhost:8080" << endl;
    server.listen("localhost", 8080);

    return 0;
}
