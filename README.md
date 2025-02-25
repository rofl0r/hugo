# Hu-Go! PC Engine/TurboGrafx-16 Emulator

_**Updated for modern AMD64 & ARM64 Linux. ARM64 binaries provided.**_

![Screenshot from 2025-02-25 04-34-09](https://github.com/user-attachments/assets/e8e53920-1a78-406f-89dc-ec07261c7cab)

![Screenshot from 2025-02-25 04-37-04](https://github.com/user-attachments/assets/5255ca68-2b58-4937-96b1-0adcc933baad)

_Screenshot from public domain ROM "Blox Graphics Slideshow"_

## Building (Linux)

Prerequisites:
- gcc
- gnu make
- autotools (should be included with the above)
- pkg-config
- GTK 2.0 libraries (used libgtk2.0-dev on Armbian)
- SDL libraries (used libsdl1.2-dev on Armbian)
- zlib libraries (optional)

After entering the `hugo` directory,
1. Set build.sh to be executable: `chmod +x build.sh`
2. Run build.sh: `./build.sh`
3. As root, run `make`, or run `sudo make` as a normal user
4. As root, run `make install`, or run `sudo make install` as a normal user
5. Launch Hu-Go! by typing `hugo`
