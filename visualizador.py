"""
visualizador_servo.py
─────────────────────
Interfaz gráfica para visualizar en tiempo real:
  • Valor ADC (0–4095) del joystick
  • Pulso PWM calculado (500–2500 µs)
  • Forma de onda PWM de los últimos ciclos
  • Ángulo estimado del servo (0–180°)

Requiere:
    pip install pyserial matplotlib

Uso:
    python visualizador_servo.py          # detecta el puerto automáticamente
    python visualizador_servo.py COM3     # o especificá el puerto
    python visualizador_servo.py /dev/ttyUSB0

Protocolo esperado desde la LPC (9600 8N1):
    "ADC:2048,PWM:1500\r\n"

Nota: en esta versión del firmware la trama se envía cada ~2 segundos
(disparada por TIMER1), no en cada lectura del ADC. La interfaz va a
actualizarse a ese ritmo.
"""

import sys
import threading
import queue
import re
import time
import tkinter as tk
from tkinter import ttk, font as tkfont
from collections import deque

import serial
import serial.tools.list_ports
import matplotlib
matplotlib.use("TkAgg")
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.patches as mpatches
import numpy as np

# ─────────────────────────── Constantes ───────────────────────────
BAUD        = 9600
PERIOD_US   = 20_000
PULSE_MIN   = 500
PULSE_MAX   = 2_500
ADC_MAX     = 4_095
HISTORY     = 60          # muestras a mostrar en el gráfico de historia
PWM_CYCLES  = 3           # ciclos PWM a dibujar en la forma de onda

# Colores
C_BG        = "#1e1e2e"
C_PANEL     = "#2a2a3e"
C_ACCENT    = "#89b4fa"   # azul
C_GREEN     = "#a6e3a1"
C_YELLOW    = "#f9e2af"
C_RED       = "#f38ba8"
C_TEXT      = "#cdd6f4"
C_SUBTEXT   = "#6c7086"
C_PULSE     = "#89dceb"   # cian para forma de onda

# ─────────────────────────── Detección de puerto ──────────────────
def detectar_puerto():
    puertos = serial.tools.list_ports.comports()
    # Preferir puertos con palabras clave típicas de USB-UART
    for p in puertos:
        desc = (p.description or "").lower()
        if any(k in desc for k in ["usb", "uart", "serial", "ch340", "cp210", "ftdi", "prolific"]):
            return p.device
    # Si no, tomar el primero disponible
    if puertos:
        return puertos[0].device
    return None

# ─────────────────────────── Hilo lector serial ───────────────────
class SerialReader(threading.Thread):
    """Lee líneas del puerto serie y las pone en una queue."""

    
    PATTERN = re.compile(r"ADC:(\d+),\s*PWM:(\d+)")
    def __init__(self, port, data_queue):
        super().__init__(daemon=True)
        self.port       = port
        self.q          = data_queue
        self.running    = True
        self._ser       = None
        self.connected  = False
        self.error_msg  = ""

    def run(self):
        try:
            self._ser = serial.Serial(self.port, BAUD, timeout=1)
            self.connected = True
            while self.running:
                try:
                    line = self._ser.readline().decode("ascii", errors="ignore").strip()
                    m = self.PATTERN.match(line)
                    if m:
                        adc = int(m.group(1))
                        pwm = int(m.group(2))
                        self.q.put(("data", adc, pwm))
                except serial.SerialException:
                    self.connected = False
                    self.q.put(("error", "Conexión perdida"))
                    break
        except serial.SerialException as e:
            self.error_msg = str(e)
            self.q.put(("error", str(e)))

    def stop(self):
        self.running = False
        if self._ser and self._ser.is_open:
            self._ser.close()

# ─────────────────────────── Aplicación ───────────────────────────
class App(tk.Tk):
    def __init__(self, port):
        super().__init__()
        self.title("Visualizador Joystick → Servo  |  LPC1769")
        self.configure(bg=C_BG)
        self.resizable(True, True)
        self.geometry("960x680")

        self.port       = port
        self.q          = queue.Queue()
        self.reader     = None

        # Datos históricos
        self.adc_hist   = deque([0] * HISTORY, maxlen=HISTORY)
        self.pwm_hist   = deque([1500] * HISTORY, maxlen=HISTORY)
        self.last_adc   = 0
        self.last_pwm   = 1500

        self._build_ui()
        self._start_reader()
        self.after(50, self._poll_queue)

    # ── Construcción de la UI ──────────────────────────────────────
    def _build_ui(self):
        # Título
        hdr = tk.Frame(self, bg=C_BG)
        hdr.pack(fill="x", padx=16, pady=(14, 4))
        tk.Label(hdr, text="Visualizador Joystick → Servo",
                 bg=C_BG, fg=C_ACCENT,
                 font=("Segoe UI", 16, "bold")).pack(side="left")
        self.lbl_status = tk.Label(hdr, text="⬤ Conectando…",
                                   bg=C_BG, fg=C_YELLOW,
                                   font=("Segoe UI", 10))
        self.lbl_status.pack(side="right", padx=8)
        self.lbl_port = tk.Label(hdr, text=f"Puerto: {self.port or '—'}",
                                 bg=C_BG, fg=C_SUBTEXT,
                                 font=("Segoe UI", 10))
        self.lbl_port.pack(side="right", padx=16)

        ttk.Separator(self, orient="horizontal").pack(fill="x", padx=16)

        # ── Fila superior: métricas ──
        top = tk.Frame(self, bg=C_BG)
        top.pack(fill="x", padx=16, pady=10)

        self._metric_adc   = self._metric_card(top, "ADC (0–4095)", "0",     C_GREEN)
        self._metric_pwm   = self._metric_card(top, "Pulso PWM (µs)", "1500", C_ACCENT)
        self._metric_angle = self._metric_card(top, "Ángulo servo (°)", "90", C_YELLOW)
        self._metric_pct   = self._metric_card(top, "Posición (%)", "50",    C_TEXT)

        for w in top.winfo_children():
            w.pack(side="left", expand=True, fill="both", padx=6)

        # ── Barra de progreso ADC ──
        bar_frame = tk.Frame(self, bg=C_BG)
        bar_frame.pack(fill="x", padx=22, pady=(0, 6))
        tk.Label(bar_frame, text="Posición joystick",
                 bg=C_BG, fg=C_SUBTEXT, font=("Segoe UI", 9)).pack(anchor="w")
        style = ttk.Style()
        style.theme_use("clam")
        style.configure("ADC.Horizontal.TProgressbar",
                         troughcolor=C_PANEL, background=C_GREEN,
                         bordercolor=C_PANEL, lightcolor=C_GREEN, darkcolor=C_GREEN)
        self.progress = ttk.Progressbar(bar_frame, style="ADC.Horizontal.TProgressbar",
                                        orient="horizontal", length=400,
                                        mode="determinate", maximum=ADC_MAX)
        self.progress.pack(fill="x", ipady=6)

        # ── Gráficos matplotlib ──
        fig_frame = tk.Frame(self, bg=C_BG)
        fig_frame.pack(fill="both", expand=True, padx=16, pady=(4, 12))

        self.fig = Figure(figsize=(10, 4), facecolor=C_BG)
        self.fig.subplots_adjust(hspace=0.45, left=0.07, right=0.97, top=0.88, bottom=0.12)

        # Subplot 1: historia del pulso PWM
        self.ax_hist = self.fig.add_subplot(1, 2, 1)
        self._style_ax(self.ax_hist, "Historial pulso PWM (µs)", "Muestra", "µs")
        self.ax_hist.set_ylim(300, 2700)
        self.line_pwm, = self.ax_hist.plot(
            list(self.pwm_hist), color=C_ACCENT, linewidth=1.8, label="Pulso PWM")
        self.ax_hist.axhline(PULSE_MIN, color=C_SUBTEXT, linestyle="--", linewidth=0.8, alpha=0.6)
        self.ax_hist.axhline(PULSE_MAX, color=C_SUBTEXT, linestyle="--", linewidth=0.8, alpha=0.6)
        self.ax_hist.axhline(1500, color=C_YELLOW, linestyle=":", linewidth=0.8, alpha=0.5)
        self.ax_hist.legend(facecolor=C_PANEL, edgecolor=C_SUBTEXT,
                            labelcolor=C_TEXT, fontsize=8, loc="upper right")

        # Subplot 2: forma de onda PWM (últimos N ciclos reconstruidos)
        self.ax_wave = self.fig.add_subplot(1, 2, 2)
        self._style_ax(self.ax_wave, "Forma de onda PWM (último valor)", "Tiempo (µs)", "Nivel")
        self.ax_wave.set_ylim(-0.15, 1.25)
        self.ax_wave.set_yticks([0, 1])
        self.ax_wave.set_yticklabels(["LOW", "HIGH"])
        self.wave_line, = self.ax_wave.plot([], [], color=C_PULSE, linewidth=2)
        self.pulse_fill = None
        self._draw_pwm_wave(1500)

        self.canvas = FigureCanvasTkAgg(self.fig, master=fig_frame)
        self.canvas.get_tk_widget().pack(fill="both", expand=True)
        self.canvas.draw()

    def _metric_card(self, parent, label, value, color):
        """Devuelve dict con referencias a los labels de una tarjeta de métrica."""
        frame = tk.Frame(parent, bg=C_PANEL, bd=0, relief="flat")
        tk.Label(frame, text=label, bg=C_PANEL, fg=C_SUBTEXT,
                 font=("Segoe UI", 9)).pack(pady=(10, 2))
        val_lbl = tk.Label(frame, text=value, bg=C_PANEL, fg=color,
                           font=("Segoe UI", 22, "bold"))
        val_lbl.pack(pady=(0, 10))
        return val_lbl

    def _style_ax(self, ax, title, xlabel, ylabel):
        ax.set_facecolor(C_PANEL)
        ax.set_title(title, color=C_TEXT, fontsize=10, pad=6)
        ax.set_xlabel(xlabel, color=C_SUBTEXT, fontsize=8)
        ax.set_ylabel(ylabel, color=C_SUBTEXT, fontsize=8)
        ax.tick_params(colors=C_SUBTEXT, labelsize=7)
        for spine in ax.spines.values():
            spine.set_edgecolor(C_SUBTEXT)
            spine.set_alpha(0.3)
        ax.grid(True, color=C_SUBTEXT, alpha=0.15, linestyle="--")

    # ── Actualización de gráficos ─────────────────────────────────
    def _draw_pwm_wave(self, pulse_us):
        """Reconstruye la forma de onda PWM a partir del ancho de pulso."""
        t, y = [], []
        for c in range(PWM_CYCLES):
            base = c * PERIOD_US
            # Flanco de subida
            t += [base,        base,           base + pulse_us, base + pulse_us]
            y += [0,           1,              1,               0             ]
        t.append(PWM_CYCLES * PERIOD_US)
        y.append(0)

        self.wave_line.set_data(t, y)
        self.ax_wave.set_xlim(0, PWM_CYCLES * PERIOD_US)

        # Sombrear el área del pulso del primer ciclo
        if self.pulse_fill:
            self.pulse_fill.remove()
        self.pulse_fill = self.ax_wave.fill_between(
            [0, pulse_us], [1, 1], color=C_PULSE, alpha=0.18)

        # Anotación del ancho de pulso
        for txt in self.ax_wave.texts:
            txt.remove()
        self.ax_wave.text(pulse_us / 2, 1.12,
                          f"{pulse_us} µs",
                          color=C_PULSE, fontsize=8, ha="center")

    def _update_plots(self):
        # Historia PWM
        data = list(self.pwm_hist)
        self.line_pwm.set_ydata(data)
        self.line_pwm.set_xdata(range(len(data)))
        self.ax_hist.set_xlim(0, len(data) - 1)

        # Forma de onda
        self._draw_pwm_wave(self.last_pwm)

        self.canvas.draw_idle()

    def _update_metrics(self, adc, pwm):
        angle = round((pwm - PULSE_MIN) / (PULSE_MAX - PULSE_MIN) * 180)
        pct   = round(adc / ADC_MAX * 100)

        self._metric_adc.config(text=str(adc))
        self._metric_pwm.config(text=str(pwm))
        self._metric_angle.config(text=str(angle))
        self._metric_pct.config(text=f"{pct}%")
        self.progress["value"] = adc

        # Color dinámico del ángulo
        if angle < 60:
            self._metric_angle.config(fg=C_ACCENT)
        elif angle < 120:
            self._metric_angle.config(fg=C_GREEN)
        else:
            self._metric_angle.config(fg=C_RED)

    # ── Polling de la queue ───────────────────────────────────────
    def _poll_queue(self):
        try:
            while True:
                msg = self.q.get_nowait()
                if msg[0] == "data":
                    _, adc, pwm = msg
                    adc = max(0, min(ADC_MAX, adc))
                    pwm = max(PULSE_MIN, min(PULSE_MAX, pwm))
                    self.last_adc = adc
                    self.last_pwm = pwm
                    self.adc_hist.append(adc)
                    self.pwm_hist.append(pwm)
                    self._update_metrics(adc, pwm)
                    self._update_plots()
                    self.lbl_status.config(text="⬤ Conectado", fg=C_GREEN)
                elif msg[0] == "error":
                    self.lbl_status.config(text=f"⬤ Error: {msg[1]}", fg=C_RED)
        except queue.Empty:
            pass
        self.after(50, self._poll_queue)

    # ── Serial ────────────────────────────────────────────────────
    def _start_reader(self):
        if not self.port:
            self.lbl_status.config(text="⬤ Sin puerto", fg=C_RED)
            return
        self.reader = SerialReader(self.port, self.q)
        self.reader.start()

    def on_close(self):
        if self.reader:
            self.reader.stop()
        self.destroy()


# ─────────────────────────── Entry point ──────────────────────────
if __name__ == "__main__":
    if len(sys.argv) > 1:
        puerto = sys.argv[1]
    else:
        puerto = detectar_puerto()
        if puerto:
            print(f"Puerto detectado automáticamente: {puerto}")
        else:
            print("⚠  No se detectó ningún puerto serie.")
            print("   Ejecutá:  python visualizador_servo.py COM3")
            print("   (reemplazando COM3 por tu puerto)")
            puerto = None

    app = App(puerto)
    app.protocol("WM_DELETE_WINDOW", app.on_close)
    app.mainloop()
