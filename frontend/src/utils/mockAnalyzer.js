const COMMAND_SPECS = {
  mkdisk: {
    required: ["size", "path"],
    optional: {
      fit: ["bf", "ff", "wf"],
      unit: ["k", "m"],
    },
  },
  rmdisk: {
    required: ["path"],
    optional: {},
  },
  fdisk: {
    required: ["size", "path", "name"],
    optional: {
      unit: ["b", "k", "m"],
      type: ["p", "e", "l"],
      fit: ["bf", "ff", "wf"],
    },
  },
  mount: {
    required: ["path", "name"],
    optional: {},
  },
  mkfs: {
    required: ["id"],
    optional: {
      type: ["full"],
    },
  },
  mkusr: {
    required: ["user", "pass", "grp"],
    optional: {},
  },
  rmusr: {
    required: ["user"],
    optional: {},
  },
  mkfile: {
    required: ["path"],
    optional: {
      r: null,
      size: null,
      cont: null,
    },
  },
};

// Separa una linea en tokens tipo -key=value, respetando comillas.
function tokenize(line) {
  const tokens = [];
  const regex = /-(\w+)=(?:"([^"]*)"|(\S+))/g;
  let match;
  let consumed = "";
  while ((match = regex.exec(line)) !== null) {
    const key = match[1].toLowerCase();
    const value = match[2] !== undefined ? match[2] : match[3];
    tokens.push({ key, value, raw: match[0] });
    consumed += match[0] + " ";
  }
  return tokens;
}

function analyzeLine(rawLine, lineNumber) {
  const trimmed = rawLine.trim();

  if (trimmed === "" || trimmed.startsWith("#")) {
    return {
      line: lineNumber,
      raw: rawLine,
      command: null,
      tokens: [],
      status: "empty",
      message: trimmed.startsWith("#") ? "Comentario" : "Linea vacia",
    };
  }

  const firstSpace = trimmed.indexOf(" ");
  const commandRaw = firstSpace === -1 ? trimmed : trimmed.slice(0, firstSpace);
  const command = commandRaw.toLowerCase();
  const rest = firstSpace === -1 ? "" : trimmed.slice(firstSpace + 1);

  const spec = COMMAND_SPECS[command];
  if (!spec) {
    return {
      line: lineNumber,
      raw: rawLine,
      command: commandRaw,
      tokens: [],
      status: "lex_error",
      message: `Comando no reconocido: "${commandRaw}"`,
    };
  }

  const tokens = tokenize(rest);
  const seen = new Map();
  for (const t of tokens) {
    seen.set(t.key, t.value);
  }

  const errors = [];

  for (const req of spec.required) {
    if (!seen.has(req)) {
      errors.push(`falta el parametro obligatorio -${req}`);
    }
  }

  for (const [key, value] of seen.entries()) {
    const isRequired = spec.required.includes(key);
    const isOptional = Object.prototype.hasOwnProperty.call(spec.optional, key);
    if (!isRequired && !isOptional) {
      errors.push(`parametro desconocido -${key}`);
      continue;
    }
    const allowedValues = isRequired ? null : spec.optional[key];
    if (allowedValues && !allowedValues.includes(value.toLowerCase())) {
      errors.push(`valor invalido "${value}" para -${key}`);
    }
    if (key === "size") {
      const n = Number(value);
      if (Number.isNaN(n) || n <= 0) {
        errors.push(`-size debe ser un numero positivo mayor que cero`);
      }
    }
    if (["user", "pass", "grp"].includes(key) && value.length > 10) {
      errors.push(`-${key} excede el maximo de 10 caracteres`);
    }
  }

  if (errors.length > 0) {
    return {
      line: lineNumber,
      raw: rawLine,
      command: commandRaw,
      tokens: tokens.map((t) => t.raw),
      status: "syntax_error",
      message: errors.join("; "),
    };
  }

  return {
    line: lineNumber,
    raw: rawLine,
    command: commandRaw,
    tokens: tokens.map((t) => t.raw),
    status: "ok",
    message: `Comando "${command}" valido`,
  };
}

export function mockAnalyze(script) {
  const lines = script.split("\n");
  return lines.map((line, idx) => analyzeLine(line, idx + 1));
}