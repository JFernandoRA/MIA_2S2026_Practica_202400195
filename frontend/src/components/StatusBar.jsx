export default function StatusBar({ results }) {
  const total = results?.length ?? 0;
  const ok = results?.filter((r) => r.status === "ok").length ?? 0;
  const errors =
    results?.filter((r) => r.status === "lex_error" || r.status === "syntax_error").length ?? 0;
  const empty = results?.filter((r) => r.status === "empty").length ?? 0;

  return (
    <footer className="status-bar">
      <div className="status-bar__item">
        <span className="status-bar__dot status-bar__dot--muted" />
        {total} lineas
      </div>
      <div className="status-bar__item">
        <span className="status-bar__dot status-bar__dot--ok" />
        {ok} validas
      </div>
      <div className="status-bar__item">
        <span className="status-bar__dot status-bar__dot--error" />
        {errors} con error
      </div>
      <div className="status-bar__item">
        <span className="status-bar__dot status-bar__dot--muted" />
        {empty} vacias / comentarios
      </div>
      <div className="status-bar__spacer" />
      <div className="status-bar__item status-bar__item--muted">
        Manejo e Implementacion de Archivos - USAC - 2S2026
      </div>
    </footer>
  );
}