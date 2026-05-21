# This code is all AI generated
# Link to the AI conversation https://claude.ai/share/44d5be0b-9efd-4d4e-b435-67b24d38af0e
"""
Escape Room — Temperature Bridge
Reads radio signals from receiver micro:bit over USB serial,
serves them to the local webpage via a tiny HTTP server.

Usage:
    python3 temp_bridge.py

Requirements:
    pip3 install pyserial

The micro:bit usually appears as /dev/ttyACM0 on Raspberry Pi.
Run `ls /dev/ttyACM*` to confirm before starting.
"""

import serial
import threading
import json
from http.server import HTTPServer, BaseHTTPRequestHandler

# ── Config ────────────────────────────────────────────────────────────────────
SERIAL_PORT  = "/dev/ttyACM0"   # change if needed (check: ls /dev/ttyACM*)
BAUD_RATE    = 115200
HTTP_PORT    = 5000
# ─────────────────────────────────────────────────────────────────────────────

# Shared state — written by serial thread, read by HTTP handler
pending_command = None
lock = threading.Lock()


class Handler(BaseHTTPRequestHandler):
    """Serves a single endpoint: GET /poll returns the latest command as JSON."""

    def do_GET(self):
        global pending_command

        if self.path == "/poll":
            with lock:
                cmd = pending_command
                pending_command = None          # consume it — send once per press

            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.send_header("Access-Control-Allow-Origin", "*")  # allow local file:// origin
            self.end_headers()
            self.wfile.write(json.dumps({"command": cmd}).encode())

        else:
            self.send_response(404)
            self.end_headers()

    def log_message(self, format, *args):
        # Keep terminal clean — only print actual commands, not every poll
        pass


def serial_reader():
    """Runs in a background thread. Reads lines from the micro:bit."""
    global pending_command

    print(f"[serial] Opening {SERIAL_PORT} at {BAUD_RATE} baud...")

    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print("[serial] Connected. Waiting for signals...")
    except serial.SerialException as e:
        print(f"[serial] ERROR: {e}")
        print("[serial] Check that the micro:bit is plugged in and the port is correct.")
        return

    while True:
        try:
            line = ser.readline().decode("utf-8").strip().lower()
            if line in ("hot", "cold"):
                with lock:
                    pending_command = line
                print(f"[serial] Received: {line}")
        except Exception as e:
            print(f"[serial] Read error: {e}")


def main():
    # Start serial reader in background
    t = threading.Thread(target=serial_reader, daemon=True)
    t.start()

    # Start HTTP server (blocking)
    server = HTTPServer(("localhost", HTTP_PORT), Handler)
    print(f"[http]   Serving on http://localhost:{HTTP_PORT}/poll")
    print("[http]   Open index.html in a browser to start the puzzle.")
    print("         Press Ctrl+C to stop.\n")

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n[http]   Stopped.")


if __name__ == "__main__":
    main()