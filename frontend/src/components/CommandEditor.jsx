import { useRef } from "react";

export default function CommandEditor({ value, onChange, lineStatuses }) {
  const textareaRef = useRef(null);
  const gutterRef = useRef(null);

  const lines = value.split("\n");

  function handleScroll(e) {
    if (gutterRef.current) {
      gutterRef.current.scrollTop = e.target.scrollTop;
    }
  }

  function statusForLine(idx) {
    const s = lineStatuses?.[idx];
    if (!s) return "";
    if (s.status === "ok") return "gutter-row--ok";
    if (s.status === "lex_error" || s.status === "syntax_error") return "gutter-row--error";
    return "";
  }

  return (
    <section className="panel editor-panel">
      <div className="panel__header">
        <span>Entrada de comandos</span>
        <span className="panel__meta">{lines.length} lineas</span>
      </div>
      <div className="editor-panel__body">
        <div className="editor-gutter" ref={gutterRef}>
          {lines.map((_, idx) => (
            <div key={idx} className={`gutter-row ${statusForLine(idx)}`}>
              {idx + 1}
            </div>
          ))}
        </div>
        <textarea
          ref={textareaRef}
          className="editor-textarea"
          value={value}
          onChange={(e) => onChange(e.target.value)}
          onScroll={handleScroll}
          spellCheck={false}
          placeholder={`mkdisk -size=3000 -unit=M -path=/home/user/Disco1.mia\nfdisk -size=300 -path=/home/user/Disco1.mia -name=Particion1\nmount -path=/home/user/Disco1.mia -name=Particion1`}
        />
      </div>
    </section>
  );
}