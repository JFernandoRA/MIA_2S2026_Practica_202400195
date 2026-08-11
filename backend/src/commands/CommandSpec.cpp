#include "CommandSpec.h"

const std::map<std::string, CommandSpec>& getCommandSpecs() {
    static const std::map<std::string, CommandSpec> specs = {

        {"mkdisk", {
            {"size", "path"},
            {
                {"fit", {"bf", "ff", "wf"}},
                {"unit", {"k", "m"}}
            },
            {
                {"fit", "ff"},
                {"unit", "m"}
            }
        }},

        {"rmdisk", {
            {"path"},
            {},
            {}
        }},

        {"fdisk", {
            {"size", "path", "name"},
            {
                {"unit", {"b", "k", "m"}},
                {"type", {"p", "e", "l"}},
                {"fit", {"bf", "ff", "wf"}}
            },
            {
                {"unit", "k"},
                {"type", "p"},
                {"fit", "wf"}
            }
        }},

        {"mount", {
            {"path", "name"},
            {},
            {}
        }},

        {"mkfs", {
            {"id"},
            {
                {"type", {"full"}}
            },
            {
                {"type", "full"}
            }
        }},

        {"mkusr", {
            {"user", "pass", "grp"},
            {},
            {}
        }},

        {"rmusr", {
            {"user"},
            {},
            {}
        }},

        {"mkfile", {
            {"path"},
            {
                {"r", {}},
                {"size", {}},
                {"cont", {}}
            },
            {}
        }}
    };

    return specs;
}