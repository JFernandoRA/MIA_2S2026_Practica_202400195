import { useEffect, useState } from "react";
import Toolbar from "./components/Toolbar.jsx";
import CommandEditor from "./components/CommandEditor.jsx";
import OutputConsole from "./components/OutputConsole.jsx";
import StatusBar from "./components/StatusBar.jsx";
import { analyzeCommands, checkBackendHealth } from "./services/api.js";
import { mockAnalyze } from "./utils/mockAnalyzer.js";

const EXAMPLE_SCRIPT = `mkdisk -size=3000 -unit=M -path=/home/user/Disco1.mia
fdisk -size=300 -path=/home/user/Disco1.mia -name=Particion1
mount -path=/home/user/Disco1.mia -name=Particion1
mkfs -type=full -id=341A
mkusr -user=user1 -pass=1234 -grp=usuarios
comandoInexistente -x=1`;

export default function App() {
  const [script, setScript] = useState("");
  const [results, setResults] = useState([]);
  const [isAnalyzing, setIsAnalyzing] = useState(false);
  const [backendOnline, setBackendOnline] = useState(false);

  useEffect(() => {
    checkBackendHealth().then(setBackendOnline);
  }, []);

  async function handleAnalyze() {
    setIsAnalyzing(true);
    try {
      const online = await checkBackendHealth();
      setBackendOnline(online);

      const data = online ? await analyzeCommands(script) : mockAnalyze(script);
      setResults(data);
    } catch (err) {
      setBackendOnline(false);
      setResults(mockAnalyze(script));
    } finally {
      setIsAnalyzing(false);
    }
  }

  function handleClear() {
    setScript("");
    setResults([]);
  }

  function handleLoadExample() {
    setScript(EXAMPLE_SCRIPT);
    setResults([]);
  }

  return (
    <div className="app">
      <Toolbar
        onAnalyze={handleAnalyze}
        onClear={handleClear}
        onLoadExample={handleLoadExample}
        isAnalyzing={isAnalyzing}
        backendOnline={backendOnline}
      />
      <main className="workspace">
        <CommandEditor value={script} onChange={setScript} lineStatuses={results} />
        <OutputConsole results={results} />
      </main>
      <StatusBar results={results} />
    </div>
  );
}