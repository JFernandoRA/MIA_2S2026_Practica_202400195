const STATUS_LABEL = {
  ok: "OK",
  lex_error: "LEX",
  syntax_error: "SYN",
  empty: "--",
};

export default function OutputConsole({ results }) {
  const hasResults = results && results.length > 0;

  return (
    <section className="panel console-panel">
      <div className="panel__header">
        <span>Resultado del analisis</span>
        <span className="panel__meta">
          {hasResults ? `${results.length} lineas procesadas` : "Sin analizar"}
        </span>
      </div>

      <div className="console-panel__body">
        {!hasResults && (
          <div className="console-empty">
            Escribe comandos EXT2 en el panel izquierdo y presiona "Analizar".
            <br />
            Ningun comando se ejecuta realmente: solo se valida su estructura.
          </div>
        )}

        {hasResults && (
          <ol className="sector-list">
            {results.map((r) => (
              <li key={r.line} className={`sector-row sector-row--${r.status}`}>
                <span className="sector-tick" aria-hidden="true" />
                <span className="sector-row__line">{r.line}</span>
                <span className="sector-row__badge">{STATUS_LABEL[r.status] ?? "?"}</span>
                <div className="sector-row__content">
                  <code className="sector-row__raw">{r.raw || "(vacio)"}</code>
                  <p className="sector-row__message">{r.message}</p>
                </div>
              </li>
            ))}
          </ol>
        )}
      </div>
    </section>
  );
}