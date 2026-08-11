#include "third_party/httplib.h"
#include "third_party/json.hpp"
#include "src/parser/Parser.h"
#include <iostream>
#include <sstream>

using json = nlohmann::json;

// Convierte un AnalysisResult a un objeto JSON, para enviarlo al frontend.
json resultToJson(const AnalysisResult& r) {
    return json{
        {"line", r.line},
        {"raw", r.raw},
        {"command", r.command},
        {"tokens", r.tokens},
        {"status", r.status},
        {"message", r.message}
    };
}

// Parte un texto largo en lineas, usando el salto de linea como separador.
std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

int main() {
    httplib::Server server;

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    server.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("", "text/plain");
    });

    server.Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("{\"status\":\"ok\"}", "application/json");
    });

    server.Post("/api/analyze", [](const httplib::Request& req, httplib::Response& res) {
        try {
            // Paso 1: leer el JSON que mando el frontend y sacar "script".
            json body = json::parse(req.body);
            std::string script = body.value("script", "");

            // Paso 2: partir ese texto completo en lineas individuales.
            std::vector<std::string> lines = splitLines(script);

            // Paso 3: analizar cada linea con el parser real.
            json resultsArray = json::array();
            int lineNumber = 1;
            for (const auto& line : lines) {
                AnalysisResult r = analyzeLine(line, lineNumber);
                resultsArray.push_back(resultToJson(r));
                lineNumber++;
            }

            // Paso 4: armar la respuesta final, misma forma que espera api.js.
            json response = {{"results", resultsArray}};
            res.set_content(response.dump(), "application/json");

        } catch (const std::exception& e) {
            // Si hubo un error al parsear el JSON, devolvemos un error 400 con mensaje.
            res.status = 400;
            json errorResponse = {{"error", std::string("JSON invalido: ") + e.what()}};
            res.set_content(errorResponse.dump(), "application/json");
        }
    });

    std::cout << "Servidor escuchando en http://localhost:8080" << std::endl;
    server.listen("0.0.0.0", 8080);

    return 0;
}