let temp = -27;
const display = document.getElementById('display');
const overlay = document.getElementById('modal-overlay');

// ── Temperature logic ────────────────────────────────────────────────────────

function changeTemp(amount) {
    temp += amount;
    display.textContent = temp;

    if (temp === 20) {
        triggerWin();
    }
}

function triggerWin() {
    overlay.classList.add('visible');
}

function closeModal() {
    overlay.classList.remove('visible');
}

// ── Micro:bit polling ────────────────────────────────────────────────────────
// Polls the local Python bridge every 300 ms.
// The server runs on the Pi at localhost:5000.
// Returns { "command": "hot" | "cold" | null }

const POLL_URL      = "http://localhost:5000/poll";
const POLL_INTERVAL = 300; // ms

async function pollBridge() {
    try {
        const res  = await fetch(POLL_URL, { cache: "no-store" });
        const data = await res.json();

        if (data.command === "hot") {
            changeTemp(10);
        } else if (data.command === "cold") {
            changeTemp(-1);
        }
    } catch (_) {
        // Server not running or Pi not connected — silent fail, buttons still work
    }
}

// Start polling loop
setInterval(pollBridge, POLL_INTERVAL);