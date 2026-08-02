//Declaro URL de la API que usara el backend
const BASE_URL = import.meta.env.VITE_API_URL || "http://localhost:8080/api"

export async function analyzeCommands(rawText) {
  const response = await fetch(`${BASE_URL}/analyze`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ script: rawText }),
  });

  if (!response.ok) {
    throw new Error(`El backend respondio con estado ${response.status}`);
  }

  const data = await response.json();
  return data.results;
}

export async function checkBackendHealth() {
  try {
    const response = await fetch(`${BASE_URL}/health`, { method: "GET" });
    return response.ok;
  } catch {
    return false;
  }
}