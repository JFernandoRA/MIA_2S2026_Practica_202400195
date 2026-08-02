#include "third_party/httplib.h"
#include <iostream>

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

    std::cout << "Servidor escuchando en http://localhost:8080" << std::endl;

    server.Post("/api/analyze", [](const httplib::Request& req, httplib::Response& res) {
    
    std::cout << "Recibido del frontend: " << req.body << std::endl;

    // Respuesta fija, de mentira, solo para probar el camino completo.
    std::string fakeResponse = R"({
        "results": [
            {
                "line": 1,
                "raw": "linea de prueba",
                "command": "mkdisk",
                "tokens": [],
                "status": "ok",
                "message": "Esto es una respuesta de prueba, todavia no hay analisis real"
            }
        ]
    })";

    res.set_content(fakeResponse, "application/json");
});

    server.listen("0.0.0.0", 8080);

    return 0;
}