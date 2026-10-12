"""
Mini Photoshop - GUI
All filters run in YOUR C++ program (main.cpp -> main.exe).
This file only draws the window and calls main.exe.

Needs: Python 3 (Tkinter is included) + Pillow   ->   pip install pillow
Run:   python gui.py        (keep this file in the same folder as main.exe)
"""

import os
import shutil
import subprocess
import sys
import tempfile
import tkinter as tk
from tkinter import filedialog, messagebox, simpledialog, ttk

from PIL import Image, ImageTk

# ---------- colours (edit these to restyle the whole app) ----------
BG      = "#15121d"   # image area
PANEL   = "#1e1a2b"   # side panel
SURFACE = "#2a2340"   # buttons
HOVER   = "#3a2f5c"   # button hover
ACCENT  = "#8b5cf6"   # purple highlight
TEXT    = "#e7e3f1"
MUTED   = "#8f87a8"
FONT    = ("Segoe UI", 10)
FONT_H  = ("Segoe UI", 9, "bold")

MAX_UNDO = 20
SIDE_W = 270
HERE = os.path.dirname(os.path.abspath(__file__))
NO_WINDOW = getattr(subprocess, "CREATE_NO_WINDOW", 0)   # hide the black console on Windows


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Mini Photoshop")
        self.geometry("1100x720")
        self.minsize(850, 520)
        self.configure(bg=BG)

        self.engine = None            # path to the compiled main.exe
        self.work = tempfile.mkdtemp(prefix="minips_")
        self.original = None
        self.image = None
        self.history = []
        self.photo = None
        self.path = None

        self._build_ui()
        self.bind("<Control-o>", lambda e: self.open_image())
        self.bind("<Control-s>", lambda e: self.save_image())
        self.bind("<Control-z>", lambda e: self.undo())
        self.protocol("WM_DELETE_WINDOW", self._close)
        self.after(200, self._find_engine)

    def _close(self):
        shutil.rmtree(self.work, ignore_errors=True)
        self.destroy()

    # ================================================================ UI
    def _build_ui(self):
        style = ttk.Style(self)
        style.theme_use("clam")
        style.configure("P.Horizontal.TScale", background=ACCENT, troughcolor=SURFACE,
                        bordercolor=PANEL, lightcolor=ACCENT, darkcolor=ACCENT, gripcount=0)
        style.map("P.Horizontal.TScale", background=[("active", "#a07cf8")])

        # --- scrollable side panel ---
        wrap = tk.Frame(self, bg=PANEL, width=SIDE_W)
        wrap.pack(side="left", fill="y")
        wrap.pack_propagate(False)
        sc = tk.Canvas(wrap, bg=PANEL, highlightthickness=0)
        sc.pack(fill="both", expand=True)
        side = tk.Frame(sc, bg=PANEL)
        sc.create_window((0, 0), window=side, anchor="nw", width=SIDE_W)
        side.bind("<Configure>", lambda e: sc.configure(scrollregion=sc.bbox("all")))
        sc.bind("<Enter>", lambda e: sc.bind_all("<MouseWheel>",
                lambda ev: sc.yview_scroll(int(-ev.delta / 120), "units")))
        sc.bind("<Leave>", lambda e: sc.unbind_all("<MouseWheel>"))

        tk.Label(side, text="MINI PHOTOSHOP", bg=PANEL, fg=ACCENT,
                 font=("Segoe UI", 13, "bold")).pack(anchor="w", padx=16, pady=(16, 0))

        self._section(side, "FILE")
        self._buttons(side, [("Open", self.open_image), ("Save", self.save_image)])
        self._buttons(side, [("Undo", self.undo), ("Reset", self.reset)])

        self._section(side, "FILTERS")
        self._buttons(side, [("Greyscale", lambda: self.run(101)),
                             ("Black & White", lambda: self.run(102))])
        self._buttons(side, [("Invert", lambda: self.run(103)),
                             ("Infrared", lambda: self.run(116))])
        self._buttons(side, [("Sunlight", lambda: self.run(113)),
                             ("Merge...", self.merge)])

        self._section(side, "EFFECTS")
        self._buttons(side, [("Detect edges", lambda: self.run(110)),
                             ("TV effect", lambda: self.run(114))])
        self._buttons(side, [("Purple", lambda: self.run(115)),
                             ("Oil paint", lambda: self.run(118))])
        self._buttons(side, [("Crop...", self.crop),
                             ("Skew...", self.skew)])

        self._section(side, "FRAME")
        self._buttons(side, [("Simple", lambda: self.run(104, 1)),
                             ("Fancy", lambda: self.run(104, 2))])

        self._section(side, "FLIP / ROTATE")
        self._buttons(side, [("Flip horizontal", lambda: self.run(105, 1)),
                             ("Flip vertical", lambda: self.run(105, 2))])
        self._buttons(side, [("90\u00b0", lambda: self.run(106, 90)),
                             ("180\u00b0", lambda: self.run(106, 180)),
                             ("270\u00b0", lambda: self.run(106, 270))])

        self._section(side, "BRIGHTNESS  %")
        self.bright = self._slider(side, 1, 100, 20)
        self._buttons(side, [("Darken", lambda: self.run(107, 1, self.bright.get())),
                             ("Lighten", lambda: self.run(107, 2, self.bright.get()))])

        self._section(side, "BLUR  %")
        self.blur_amt = self._slider(side, 0, 100, 30)
        self._buttons(side, [("Apply blur", lambda: self.run(112, self.blur_amt.get()))])

        self._section(side, "RESIZE")
        row = self._row(side)
        self.w_var, self.h_var = tk.StringVar(), tk.StringVar()
        self._entry(row, self.w_var).pack(side="left", expand=True, fill="x", padx=(0, 3), ipady=4)
        tk.Label(row, text="x", bg=PANEL, fg=MUTED, font=FONT).pack(side="left")
        self._entry(row, self.h_var).pack(side="left", expand=True, fill="x", padx=(3, 0), ipady=4)
        self._buttons(side, [("Apply resize", self.resize)])
        tk.Frame(side, bg=PANEL, height=20).pack()

        # --- image area ---
        main = tk.Frame(self, bg=BG)
        main.pack(side="left", fill="both", expand=True)
        self.canvas = tk.Canvas(main, bg=BG, highlightthickness=0)
        self.canvas.pack(fill="both", expand=True)
        self.canvas.bind("<Configure>", lambda e: self._show())
        self.status = tk.Label(main, text="Open an image to start  (Ctrl+O)", bg=PANEL,
                               fg=MUTED, font=FONT, anchor="w", padx=12, pady=6)
        self.status.pack(fill="x", side="bottom")

    def _section(self, parent, text):
        tk.Label(parent, text=text, bg=PANEL, fg=MUTED, font=FONT_H).pack(
            anchor="w", padx=16, pady=(11, 3))

    def _row(self, parent):
        f = tk.Frame(parent, bg=PANEL)
        f.pack(fill="x", padx=16, pady=2)
        return f

    def _buttons(self, parent, items):
        row = self._row(parent)
        for i, (text, cmd) in enumerate(items):
            b = tk.Button(row, text=text, command=cmd, bg=SURFACE, fg=TEXT,
                          activebackground=ACCENT, activeforeground="white",
                          relief="flat", bd=0, highlightthickness=0,
                          font=FONT, cursor="hand2", pady=5)
            b.bind("<Enter>", lambda e, b=b: b.config(bg=HOVER))
            b.bind("<Leave>", lambda e, b=b: b.config(bg=SURFACE))
            b.grid(row=0, column=i, sticky="ew", padx=(0 if i == 0 else 3, 0))
            row.columnconfigure(i, weight=1, uniform="btn")

    def _entry(self, parent, var):
        return tk.Entry(parent, textvariable=var, bg=SURFACE, fg=TEXT, insertbackground=TEXT,
                        relief="flat", bd=0, font=FONT, justify="center", width=6,
                        highlightthickness=1, highlightbackground=SURFACE, highlightcolor=ACCENT)

    def _slider(self, parent, lo, hi, start):
        """Slider with the number shown on the right. Returns an IntVar (use .get())."""
        var = tk.IntVar(value=start)
        row = self._row(parent)
        ttk.Scale(row, from_=lo, to=hi, orient="horizontal", style="P.Horizontal.TScale",
                  command=lambda v: var.set(round(float(v))), value=start
                  ).pack(side="left", fill="x", expand=True)
        tk.Label(row, textvariable=var, width=4, bg=PANEL, fg=TEXT, font=FONT).pack(side="left")
        return var

    # ================================================================ display
    def _show(self):
        self.canvas.delete("all")
        if self.image is None:
            return
        cw, ch = max(self.canvas.winfo_width(), 60), max(self.canvas.winfo_height(), 60)
        view = self.image.copy()
        view.thumbnail((cw - 40, ch - 40))          # only shrinks, never enlarges
        self.photo = ImageTk.PhotoImage(view)
        self.canvas.create_image(cw // 2, ch // 2, image=self.photo)

    def _info(self, msg=""):
        if self.image is None:
            return
        w, h = self.image.size
        self.w_var.set(str(w))
        self.h_var.set(str(h))
        name = os.path.basename(self.path) if self.path else "image"
        self.status.config(text=f"{msg + '   |   ' if msg else ''}{name}   {w} x {h} px")

    def _need_image(self):
        if self.image is None:
            messagebox.showinfo("No image", "Open an image first.")
            return False
        return True

    def _set_image(self, new_image, msg):
        self.history.append(self.image)
        if len(self.history) > MAX_UNDO:
            self.history.pop(0)
        self.image = new_image
        self._show()
        self._info(msg)

    # ================================================================ C++ engine
    def _find_engine(self):
        """Look for main.exe next to this file; if missing, ask the user."""
        for name in ("main.exe", "main"):
            p = os.path.join(HERE, name)
            if os.path.isfile(p):
                self.engine = p
                return True
        messagebox.showinfo("Find main.exe",
                            "Put gui.py in the same folder as your compiled main.exe.\n\n"
                            "Or choose main.exe now (build main.cpp first).")
        p = filedialog.askopenfilename(title="Choose your compiled main.exe",
                                       filetypes=[("Program", "*.exe"), ("All files", "*.*")])
        if p:
            self.engine = p
            return True
        return False

    def run(self, code, *args):
        """Send the current image to main.exe, get the filtered image back."""
        if not self._need_image():
            return
        if not self.engine and not self._find_engine():
            return
        in_png = os.path.join(self.work, "in.png")
        out_png = os.path.join(self.work, "out.png")
        if os.path.exists(out_png):
            os.remove(out_png)
        self.image.save(in_png)

        self.status.config(text="Working...")
        self.update_idletasks()
        # main.exe is started normally; the filter + values travel as environment variables
        env = dict(os.environ, MINIPS_IN="in.png", MINIPS_OUT="out.png",
                   MINIPS_CODE=str(code), MINIPS_ARGS=" ".join(map(str, args)))
        try:
            # cwd=self.work so file names stay simple (no spaces / Arabic letters in paths)
            subprocess.run([self.engine], cwd=self.work, env=env, stdin=subprocess.DEVNULL,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                           timeout=60, creationflags=NO_WINDOW)
        except subprocess.TimeoutExpired:
            self._engine_error("main.exe did not finish.")
            return
        except OSError as e:
            self._engine_error(f"Could not start main.exe.\n\n{e}")
            return
        if not os.path.exists(out_png):
            self._engine_error("main.exe did not create the output image.")
            return

        result = Image.open(out_png).convert("RGB")
        result.load()
        self._set_image(result, FILTER_NAMES.get(code, "Done"))

    def _engine_error(self, text):
        self._info()
        messagebox.showerror(
            "main.exe problem",
            text + "\n\nMake sure you rebuilt main.cpp after adding the GUI block "
                   "(the old main.exe does not understand the GUI).")

    # ================================================================ file
    def open_image(self):
        path = filedialog.askopenfilename(
            title="Open image",
            filetypes=[("Images", "*.jpg *.jpeg *.png *.bmp *.tga"), ("All files", "*.*")])
        if not path:
            return
        try:
            img = Image.open(path).convert("RGB")
        except Exception as e:
            messagebox.showerror("Error", f"Could not open the image.\n\n{e}")
            return
        self.path = path
        self.original = img
        self.image = img.copy()
        self.history.clear()
        self._show()
        self._info("Opened")

    def save_image(self):
        if not self._need_image():
            return
        path = filedialog.asksaveasfilename(
            title="Save image", defaultextension=".png",
            filetypes=[("PNG", "*.png"), ("JPEG", "*.jpg"), ("BMP", "*.bmp"), ("TGA", "*.tga")])
        if not path:
            return
        try:
            self.image.save(path)
            self.status.config(text=f"Saved to {path}")
        except Exception as e:
            messagebox.showerror("Error", f"Could not save.\n\n{e}")

    def undo(self):
        if self.history:
            self.image = self.history.pop()
            self._show()
            self._info("Undo")

    def reset(self):
        if self.original is not None:
            self._set_image(self.original.copy(), "Reset to original")

    # ================================================================ filters with extra input
    def merge(self):
        if not self._need_image():
            return
        path = filedialog.askopenfilename(
            title="Choose the second image to merge",
            filetypes=[("Images", "*.jpg *.jpeg *.png *.bmp *.tga"), ("All files", "*.*")])
        if not path:
            return
        try:
            second = Image.open(path).convert("RGB")
        except Exception as e:
            messagebox.showerror("Error", f"Could not open the image.\n\n{e}")
            return
        second.save(os.path.join(self.work, "second.png"))
        # 1 = same size, 2 = different size (result is cropped to the smaller one)
        option = 1 if second.size == self.image.size else 2
        self.run(109, option, "second.png")

    def crop(self):
        if not self._need_image():
            return
        text = simpledialog.askstring("Crop", "Enter:  x  y  width  height\n(x, y = top-left corner)")
        if not text:
            return
        try:
            x, y, w, h = map(int, text.split())
            W, H = self.image.size
            if x < 0 or y < 0 or w < 1 or h < 1 or x + w > W or y + h > H:
                raise ValueError
        except ValueError:
            messagebox.showwarning("Crop", f"Enter 4 numbers inside the image ({self.image.size[0]} x {self.image.size[1]}).")
            return
        self.run(111, x, y, w, h)

    def skew(self):
        if not self._need_image():
            return
        angle = simpledialog.askinteger("Skew", "Skew angle (degrees):", minvalue=-80, maxvalue=80)
        if angle is None:
            return
        self.run(117, angle)

    def resize(self):
        if not self._need_image():
            return
        try:
            w, h = int(self.w_var.get()), int(self.h_var.get())
            if not (1 <= w <= 8000 and 1 <= h <= 8000):
                raise ValueError
        except ValueError:
            messagebox.showwarning("Resize", "Enter a valid width and height (1 - 8000).")
            return
        self.run(108, w, h)


FILTER_NAMES = {
    101: "Greyscale applied", 102: "Black & white applied", 103: "Colors inverted",
    104: "Frame added", 105: "Image flipped", 106: "Image rotated",
    107: "Brightness adjusted", 108: "Image resized", 109: "Images merged",
    110: "Edges detected", 111: "Image cropped", 114: "TV effect applied",
    115: "Purple filter applied", 117: "Image skewed", 118: "Oil paint applied",
    112: "Blur applied", 113: "Natural sunlight applied", 116: "Infrared applied",
}

if __name__ == "__main__":
    App().mainloop()
