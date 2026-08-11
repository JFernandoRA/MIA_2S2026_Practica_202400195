#include "CommandSpec.h"

const std::map<std::string, CommandSpec>& getCommandSpecs() {
    static const std::map<std::string, CommandSpec> specs = {

        {"mkdisk", {
            {"size", "path"},                          // requerido
            {                                            // opcional
                {"fit", {"bf", "ff", "wf"}},
                {"unit", {"k", "m"}}
            }
        }},

        {"rmdisk", {
            {"path"},
            {}
        }},

        {"fdisk", {
            {"size", "path", "name"},
            {
                {"unit", {"b", "k", "m"}},
                {"type", {"p", "e", "l"}},
                {"fit", {"bf", "ff", "wf"}}
            }
        }},

        {"mount", {
            {"path", "name"},
            {}
        }},

        {"mkfs", {
            {"id"},
            {
                {"type", {"full"}}
            }
        }},

        {"mkusr", {
            {"user", "pass", "grp"},
            {}
        }},

        {"rmusr", {
            {"user"},
            {}
        }},

        {"mkfile", {
            {"path"},
            {
                {"r", {}},      // lista vacia = acepta cualquier valor (o ninguno)
                {"size", {}},
                {"cont", {}}
            }
        }}
    };

    return specs;
}