# Re::Craft 3DS
a Minecraft (1.10.x - 1.12.x) client implemented on the 3DS (and new 3DS)

### IMPORTANT
Due to personal reasons, I (SomeRandoLameo) will pause my part on development for the next 4-5 weeks.
You can still reach out to me, do PRs and commits, i will accept / answer them, i just wont be pushing code for that time.

### Other informations
This is a hobby project. I cannot tell how far this will go in the future but for now this will stay active for a longer amount of time.
Please dont be annoyed if this crashed on your device, as this is my first proper c++ lang and 3DS hardware project.
The current roms are built with focus on the N3DS, however o3DS Support is being tested as well.
The game should run in theory. If not: create an issue and steps to recreate it. We will greatly appreciate this!

### Additional libraries (you need to install them in order to build this project)
- 3ds-zlib
- 3ds-curl 
- 3ds-mbedtls
- 3ds-libopus
  - (or the entire 3ds-portlibs)
- submodules
  - [mclib-3ds](https://github.com/SomeRandoLameo/mclib-3ds)
  - [amethyst](https://github.com/tobid7/amethyst)
  - [imgui-impl-ctr](https://github.com/tobid7/imgui-impl-ctr)
  - [stb](https://github.com/nothings/stb/tree/f1c79c02822848a9bed4315b12c8c8f3761e1296)

### How do i build this rom? 

**Install dependencies:**
```bash
# Install all portlibs (easier & recommended option)
# You can also use normal pacman if you configured that correctly
sudo dkp-pacman -S 3ds-portlibs

# OR install individual libraries
sudo dkp-pacman -S 3ds-zlib 3ds-curl 3ds-mbedtls 3ds-libopus
```

**Clone and build:**
```bash
# Clone the repository with submodules
git clone --recursive https://github.com/SomeRandoLameo/Re-Craft-3DS
cd ReCraft

# If you already cloned without --recursive, initialize submodules
git submodule update --init --recursive

# Configure CMake
# The Toolchain has to be set to the toolchain of DevKitPro.
# The 3DS IP is used for the send target. If you dont want to send via your IDE, you can leave it empty
cmake -B build -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/3ds.cmake -D3DS_IP="YOUR_3DS_IP_HERE"

# Build the project
make Re-Craft-3DS_3dsx

# Install via 3dslink (3DS must be on network)
make send

# you can also use standalone 3DSLink
cd cmake_build_debug/
3dslink Re-Craft-3DS.3dsx

# OR use FTP to transfer the .3dsx file to your 3DS manually
```

### How do i connect to servers?

**Quick setup (included local test server):**
```bash
# Download the Minecraft 1.12 server
cmake --build . --target download-server

# Navigate to server folder and accept EULA
cd server
# Edit eula.txt and change "eula=false" to "eula=true"
# if the file is not present, start the server once. 
# Then you should be able to accept the EULA

# Start the server
cd ..
cmake --build . --target start-server
```
The server runs as an "offline/cracked" server. You can modify `server.properties` in the server folder if needed.
Online mode is not (yet) supported.

**Connecting to the Server:**
```bash
# Get your local IP address (The IP where you ran the java server)
# Windows:
ipconfig

# Linux/Mac:
ifconfig
# or
ip -4 addr show

# Look for an address like:
# 192.168.x.x, 172.x.x.x, 10.x.x.x, or 127.0.0.1 (localhost only)
# Note this address
```

Then think of a username. Eg: "Nintendo3DS".

When you launched the rom, press on Multiplayer. You will be asked about the IP Address of the server, then your Username.

*SIDENOTE*: In order to connect to the offline dev server, your 3DS *NEEDS TO BE ON THE SAME NETWORK AS THE SERVER*, otherwise this wont connect and will crash

After submitting both, the Game should freeze for about 30 seconds.
then: 
- The o3DS will disconnect because it took too long to decode the data 
- The emulator (azahar in this case) will have a graphical bug and should disconnect as well.
- The n3DS connects just fine

This is tested and expected behavour.

When the Client disconnects, you are still able to explore the downloaded chunks, but nothing will update and its very likely to crash. This is normal for now.

When playing on the server, new chunks will get loaded when nessessary, causing some lag spikes. Move around if they dont stop. This is normal for as well.

### Roadmap
There is a [project board](https://github.com/users/SomeRandoLameo/projects/2/views/2). if you want to contribute, but dont know where to start, check it out. 
It has priorities and is organized for easy navigation.
Be sure to read the [CONTRIBUTION GUIDELINES](CONTRIBUTING.md) before changing things.

### Communication
If you have any questions regarding this project or you want to show this project in a youtube video or anywhere else, please contact me on Discord!
My Tag is "SomeRandoLameo"!

### Credits
- Network engine bits from [mclib-3ds](https://github.com/SomeRandoLameo/mclib-3ds)
- Client Engine bits from [craftus](https://github.com/Onixiya/craftus)
- Research from Official Minecraft PE decompilations [MCPED](https://github.com/ReMinecraftPE/mcped)
- Research from Official Minecraft PE decompilations [MinecraftLCE](https://github.com/GRAnimated/MinecraftLCE/tree/main)
- Research from Official Minecraft Decompilations [MCP919](https://github.com/Marcelektro/MCP-919)
- Dear IMGUI for 3DS (used only for debugging) [imgui-ctr](https://github.com/npid7/imgui-impl-ctr/tree/e67a33579fa6424171ec1a583b6baffa077ee2d3)
- Learned how to utilize mclib from [Terracotta](https://github.com/plushmonkey/Terracotta)
- 3DS Compatability layer for IMGUI [imgui-impl-ctr](https://github.com/npid7/imgui-impl-ctr)
- Audio & Other misc. bits from [QuickLime](https://github.com/syelan34/quicklime) (this may be used in the future)

### Special thanks goes out to: 
- Tobid7 ([Amethyst](https://github.com/tobid7/amethyst) and main contributor)
- RSDuck & Onixia ([Craftus](https://github.com/Onixiya/craftus))
- Plushmonkey ([original mclib](https://github.com/plushmonkey/mclib) & [Terra](https://github.com/plushmonkey/Terracotta))
- The people behing [MCPED](https://github.com/ReMinecraftPE/mcped)
- The people behind the MCLE Decomp [MinecraftLCE](https://github.com/GRAnimated/MinecraftLCE/tree/main)
- The people behind MCP919 and the ones that published this build [MCP919](https://github.com/Marcelektro/MCP-919)
- The people behind [DEAR IMGUI](https://github.com/ocornut/imgui)
- The people behind [stb](https://github.com/nothings/stb)
- And many more! (if you think, your code is used here, feel free to commit a change to the readme. We will validate and add it if possible)

### Licensing
- All external snippets must respect their original licenses

### AI Code
AI Is a tool, not a fullstack programmer.
  
- You can use AI to explain how things work
- You can use AI to help sketching out features/Idea concepts

- PRs that are 100% AI Code will NOT be accepted.
- You cannot use AI to write ccode that you could have wrote yourself
- You cannot use AI to automate development processes
- You cannot use AI to vibecode here, unless someone allows you to
- You cannot use AI to communicate to other devs, speak for yourself

- We want human written - human readable and human usable code, not AI Slop
