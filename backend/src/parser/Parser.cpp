#include "Parser.h"
#include "../lexer/Lexer.h"
#include "../commands/CommandSpec.h"
#include <algorithm>
#include <sstream>
#include <map>
#include <cctype>

static std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}

static bool isPositiveNumber(const std::string& value) {
    if (value.empty()) return false;
    try {
        double n = std::stod(value);
        return n > 0;
    } catch (...) {
        return false;
    }
}

AnalysisResult analyzeLine(const std::string& rawLine, int lineNumber) {
    AnalysisResult result;
    result.line = lineNumber;
    result.raw = rawLine;

    // Paso 1: usamos el lexer para partir la linea en piezas.
    TokenizedLine tokenized = tokenizeLine(rawLine);

    // Si no hay comando, es una linea vacia o comentario.
    if (tokenized.command.empty()) {
        result.status = "empty";
        result.message = "Linea vacia o comentario";
        return result;
    }

    result.command = tokenized.command;
    std::string command = toLower(tokenized.command);

    // Paso 2: buscamos las reglas de ese comando en el reglamento.
    const auto& specs = getCommandSpecs();
    auto specIt = specs.find(command);

    if (specIt == specs.end()) {
        result.status = "lex_error";
        result.message = "Comando no reconocido: \"" + tokenized.command + "\"";
        return result;
    }

    const CommandSpec& spec = specIt->second;

    // Guardamos los parametros que si vinieron, para consultarlos rapido.
    std::map<std::string, std::string> seen;
    for (const auto& p : tokenized.params) {
        seen[p.key] = p.value;
        result.tokens.push_back("-" + p.key + "=" + p.value);
    }

    std::vector<std::string> errors;

    
    for (const auto& piece : tokenized.unrecognized) {
        errors.push_back("texto no reconocido: \"" + piece + "\" (revisa si un valor con espacios necesita comillas)");
    }

    // Paso 3: revisar que todos los obligatorios esten presentes.
    for (const auto& req : spec.required) {
        if (seen.find(req) == seen.end()) {
            errors.push_back("falta el parametro obligatorio -" + req);
        }
    }

    // Paso 4: revisar cada parametro que si vino.
    for (const auto& entry : seen) {
        const std::string& key = entry.first;
        const std::string& value = entry.second;

        bool isRequired = std::find(spec.required.begin(), spec.required.end(), key)
                           != spec.required.end();
        auto optIt = spec.optional.find(key);
        bool isOptional = optIt != spec.optional.end();

        if (!isRequired && !isOptional) {
            errors.push_back("parametro desconocido -" + key);
            continue;
        }

        // Si es opcional y tiene una lista de valores permitidos, validar.
        if (isOptional && !optIt->second.empty()) {
            std::string lowerValue = toLower(value);
            bool valid = std::find(optIt->second.begin(), optIt->second.end(), lowerValue)
                         != optIt->second.end();
            if (!valid) {
                errors.push_back("valor invalido \"" + value + "\" para -" + key);
            }
        }

        if (key == "size" && !isPositiveNumber(value)) {
            errors.push_back("-size debe ser un numero positivo mayor que cero");
        }

        if ((key == "user" || key == "pass" || key == "grp") && value.size() > 10) {
            errors.push_back("-" + key + " excede el maximo de 10 caracteres");
        }
    }

    // Paso 5: armar la respuesta final.
    if (!errors.empty()) {
        result.status = "syntax_error";
        std::ostringstream oss;
        for (size_t i = 0; i < errors.size(); ++i) {
            if (i > 0) oss << "; ";
            oss << errors[i];
        }
        result.message = oss.str();
        return result;
    }

    result.status = "ok";

        std::vector<std::string> defaultNotes;
        for (const auto& def : spec.defaults) {
            if (seen.find(def.first) == seen.end()) {
                defaultNotes.push_back("-" + def.first + " no especificado, se asume " + def.second);
            }
        }

        std::string message = "Comando \"" + command + "\" valido";
        if (!defaultNotes.empty()) {
            std::ostringstream noteStream;
            for (size_t i = 0; i < defaultNotes.size(); ++i) {
                if (i > 0) noteStream << "; ";
                noteStream << defaultNotes[i];
            }
            message += " (" + noteStream.str() + ")";
        }
        result.message = message;
        return result;
}