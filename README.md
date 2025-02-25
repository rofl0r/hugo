# Hu-Go! PC Engine/TurboGrafx-16 Emulator

_**Updated for modern AMD64 & ARM64 Linux.**_

![Screenshot from 2025-02-25 03-36-04](https://github.com/user-attachments/assets/b0d69b44-dab5-45d5-b77f-975288a067fe)

### Building (Linux)

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
