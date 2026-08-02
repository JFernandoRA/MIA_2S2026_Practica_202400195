export default function Toolbar({
  onAnalyze,
  onClear,
  onLoadExample,
  isAnalyzing,
  backendOnline,
}) {
  return (
    <header className="toolbar">
      <div className="toolbar__brand">
        <span className="toolbar__disk" aria-hidden="true" />
        <div>
          <h1 className="toolbar__title">Analizador EXT2</h1>
          <p className="toolbar__subtitle">Analisis lexico y sintactico de comandos</p>
        </div>
      </div>

      <div className="toolbar__actions">
        <span className={`status-pill ${backendOnline ? "status-pill--online" : "status-pill--offline"}`}>
          <span className="status-pill__dot" />
          {backendOnline ? "Backend conectado" : "Modo simulacion local"}
        </span>
        <button className="btn btn--ghost" onClick={onLoadExample} type="button">
          Cargar ejemplo
        </button>
        <button className="btn btn--ghost" onClick={onClear} type="button">
          Limpiar
        </button>
        <button
          className="btn btn--primary"
          onClick={onAnalyze}
          type="button"
          disabled={isAnalyzing}
        >
          {isAnalyzing ? "Analizando..." : "Analizar"}
        </button>
      </div>
    </header>
  );
}
