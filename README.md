# Cairo University – Faculty of Computing and AI

## CS213: Object Oriented Programming
### Assignment 1 (2026 / 2027) – First Semester

**Instructor:** Dr. Mohammad El-Ramly

---

## 📌 Program Description

A "Baby Photoshop" console program written in C++. You load an image, apply one or more filters, and save the result. The menu loops until you choose to exit, and the current image always becomes the result of the last filter.

The program supports **16 required filters** and **2 bonus filters (17, 18)**. It reads and writes `.jpg`, `.jpeg`, `.png` and `.bmp` through the provided `Image_Class.h` library.

---

## 👥 Team Members

| Name | ID | Section |
|---|---|---|
| Rawan Osama | 20250233 |  S1, S2 |
| Sama Tarek | 20250295 | S29, S30 |
| Malak Abdelhamid | 20250663 | S1, S2 |
| Salma Ayman | 20250880 | S29, S30 |

---

## 📝 Work Division


- Rawan Osama ( 1, 5, 9, 13 )
- Sama Tarek ( 2, 6, 10, 14 )
- Malak Abdelhamid ( 3, 7, 11, 15 )
- Salma Ayman  ( 4, 8, 12, 16 )


The Menu, GitHub repo, GUI and Bonus filters were done by the whole team.


---

## ⚙️ How to Run

1. **Open the Project:**
    - Extract the project folder  inside your preferred C++ IDE (such as CLion, Visual Studio, or VS Code).
    - Ensure that `CMakeLists.txt`, `Image_Class.h`, and the `stb` header files reside in the root project directory alongside the source files.
   
2. **Build and Run:**
    - Let CMake configure and generate the build files automatically.
    - Build the project target and run the executable.
   - Prepare the Project:* Create a folder containing main.cpp and gui.py.
   - Compile: Run main.cpp to generate main.exe, then add the three library files to the folder.
   - Launch GUI: Open gui.py in any Python IDE and run it to launch the interface.


3. **Using the GUI:**
    - Once the application window launches, click **Open Image** to select an image from your computer.
    - Choose any filter option from the interactive graphical control panel to apply the effect instantly.
    - Click **Save Image** to export your modified image under your chosen file name and format.
> Note: after saving, the program calls `system(<file name>)` to open the saved image, which works on Windows.

---

## 📋 Menu

On start, the program asks for an image file name. It checks that the file exists and has a valid extension, and asks again if not. Then it shows:

| Option | Action |
|---|---|
| 1 | Load a new image (asks to save the current one first) |
| 2 | Save the image (same file or a new name, extension is validated) |
| 101 – 118 | Filters 1 – 18 |
| 0 | Exit (asks to save first) |

After every filter the same menu appears again.

---

## 🎨 Filters

### [1] Greyscale
Averages the R, G, B values of every pixel and writes that average back to all three channels.

### [2] Black & White
Computes a weighted average of each pixel, `(2R + 3G + B) / 6`. If it is 127 or more the pixel becomes white (255), otherwise black (0).

### [3] Invert
Replaces every channel value with `255 - value`, producing a negative of the image.

### [4] Frame
The user chooses a **simple** or **fancy** frame. Simple draws a blue border (5% of the image size) on all four sides. Fancy adds a thin inner line inside the border.

### [5] Flip
Horizontal or vertical, chosen by the user. Pixels are swapped between the two halves of the image, channel by channel.

### [6] Rotate
Rotates clockwise by 90°, 180° or 270°. For 90° and 270° a new image is created with width and height swapped.

### [7] Darken & Lighten
The user picks darken or lighten and enters a percentage. Each channel value is decreased or increased by that percentage of itself. Lightened values are capped at 255.

### [8] Resize
The user enters a new width and height. A new image is built using nearest-neighbour: each new pixel copies the closest pixel from the original, using the ratio between old and new dimensions.

### [9] Merge
Loads a second image and averages the two. If the images are the same size, the whole image is merged. If they differ, only the common area (smaller width × smaller height) is merged.

### [10] Edge Detection
Uses a Sobel operator on the grey value of each pixel. If the gradient magnitude is above 100 the pixel is black (an edge), otherwise white.

### [11] Crop
The user enters a starting point `(x, y)` and a width and height. The dimensions are checked against the original image. If valid, that region is copied into a new image.

### [12] Blur
The user enters a blur percentage, which sets the blur radius. Each pixel becomes the average of its neighbours inside a square box. The averages are computed with a prefix-sum table (per channel), so large radii stay fast.

### [13] Natural Sunlight
Gives a warm sunlight look: Red +40, Green +40, Blue −25, clamped to the range 0–255.

### [14] TV Effect
Darkens every fourth row of pixels to half brightness, which creates the horizontal scan lines of an old TV.

### [15] Purple Filter
Scales the channels to push the image toward purple: Red × 1.15, Green × 0.8, Blue × 1.25, clamped to 255.

### [16] Infrared
Inverts the channels, averages them, then sets Red to 255 and Green and Blue to that average, which gives the red infrared look.

### [17] Skew 
The user enters an angle. A wider canvas is created (width + `height × tan(angle)`) and each row is shifted sideways in proportion to its height. Empty areas are filled with white.

### [18] Oil Paint
Reduces each channel to 10 intensity levels. Smooth gradients such as sky, water and lights collapse into flat bands of colour, which is what gives the painted look.

