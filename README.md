# Re-Craft-3DS
a Minecraft (1.10.x - 1.12.x) client implemented on the 3DS (and new 3DS)

### Other informations
This is a hobby project. I cannot tell how far this will go in the future but for now this will stay active for a longer amount of time.
Please dont be annoyed if this crashed on your device, as this is my first proper c++ lang and 3DS hardware project.
Also. I, personally DO NOT own an O3DS. The current roms are built with focus on the N3DS, since i cannot test O3DS compatability.
The game should run in theory. If not: create an issue and steps to recreate it. We will greatly appreciate this!

### Additional libraries (you need to install them in order to build this project)
- 3ds-zlib
- 3ds-curl 
- 3ds-mbedtls
- 3ds-libopus
- submodules
  - [mclib-3ds](https://github.com/SomeRandoLameo/mclib-3ds)
  - [imgui-ctr](https://github.com/npid7/imgui-impl-ctr/tree/e67a33579fa6424171ec1a583b6baffa077ee2d3)
  - [imgui-impl-ctr](https://github.com/npid7/imgui-impl-ctr)
  - [stb](https://github.com/nothings/stb/tree/f1c79c02822848a9bed4315b12c8c8f3761e1296)

### How to get this running?
- install the Dependencies
- make the Re_Craft_3dsx target
- install it via 3dslink, ftp or the custom send target built into the cmakelist

### How do i connect to servers?
- For now, the IP and Username are hardcoded inside the MCBridge. You need to change them accordingly.
- We created build targets to download and start a vanilla 1.12 minecraft server.
  - run download-server target
  - run start-server target
  - navigate to the server folder to accept the EULA for Minecraft servers
  - run start-server again
  - the server should run as an offline superflat server named ReCraft dev server.
- We already created a server configuration. You can change it, but you usually dont need to.
- Get your local PC IP address (the one that looks like 192.168.xxx.xxx, 172.xxx.xxx.xxx, 127.0.xxx or 10.0.xxx.xxx)
- Replace the IP in MCBridge with the one you notated
- Rebuild the project and reinstall it.

*SIDENOTE*: In order to connect to the offline dev server, your 3DS *NEEDS TO BE ON THE SAME NETWORK AS THE SERVER*, otherwise this wont connect.

### Roadmap
We recently created a [project board](https://github.com/users/SomeRandoLameo/projects/2/views/2). if you want to contribute, but dont know where to start, check it out. 
it has priorities and is organized way better than this table below. The table likely wont be updated and removed in the future

| Task                                 | Status      | Notes                                                                                                                                                                                         |
|--------------------------------------|-------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Port MCLib                           | ✅ Done      | Ported and tested. Minor crashes may still occur, but i was able to successfully send/control Movement packets                                                                                |
| Port Craftus                         | ✅ Done      | No crashes in C version. Minor 2D layer glitches when a 2D buffer overflows (will be fixed in cpp rewrite). No sound (SoundEngine got removed because it never worked in the real one either) |
| Rename Craftus source files to .cpp  | ✅ Done      |                                                                                                                                                                                               |
| Run MCLib code as separate thread    | ✅ Done      | Test implementation first, there is no need for a proper GUI here. We can use what we already got.                                                                                            |
| Added ImGui for debugging purposes   | ✅ Done      | Added a small Manager for utillizing ImGui everywhere                                                                                                                                         |
| Join Craftus and MCLib               | In Progress | Use the same Items, Blocks, Vectors, Math and more. This requires an almost full rewrite of craftus and feature-expansions of MC-lib (as i found out 27.10.2025 eg. Today)                    |
| Implement proper OOP design          | In Progress | Minecraft 1.12 Client sources may help here                                                                                                                                                   |
| New Renderer                         | In Progress | A completely new Renderer that should fix all rendering problems, written from scratch                                                                                                        |
| Async Keyboard                         | Planned | a custom keyboard with dpad navigation that doesnt (unlike the swkb keyboard) block the main thread.                                                                                                        |
| ...                                  | ...         | Various bug fixing and client core feature implementations                                                                                                                                    |
| Proper Itemstacks                    | Planned     | A Block uint8 ID is not enough                                                                                                                                                                |
| Inventory                            | Planned     | Survival and Creative                                                                                                                                                                         |
| Armor slots                          | Planned     | Survival and Creative, online only                                                                                                                                                            |
| Offhand slot                         | Planned     | Survival and Creative                                                                                                                                                                         |
| Chest                                | Planned     | only online for now (meaning only when a packet was triggered)                                                                                                                                |
| Furnace                              | Planned     | only online for now (meaning only when a packet was triggered)                                                                                                                                |
| Survival hearts                      | Planned     | was ported, but but not all types of poision arent yet implemented because they dont exist yet                                                                                                                                  |
| XP Bar                               | Planned     | was ported, but not yet fully integrated, because XP doesnt exist yet                                                                                                                           |
| Hotbar                               | Planned     | Survival and Creative                                                                                                                                                                         |
| Chat                                 | Planned     | Added only in Online mode, still on IMGUI, needs a propper UI as GUIOverlay                                                                                                                                                                           |
| ...                                  | ...         | Various bug fixing and client core feature implementations                                                                                                                                    |
| Autosaving in background             | Planned     | chunks need to to be saved in order to prevent memory flooding                                                                                                                                |
| ...                                  | ...         | Various bug fixing and client core feature implementations                                                                                                                                    |
| Multiplayer world                    | Planned     | A Propper attempt at 3DS Chunk and block rendering, entity rendering and more                                                                                                                 |
| Block interactions                   | Planned     | Blocks like Chests, Furncances and more should be interactive in both singleplayer and multiplayer                                                                                            |
| ...                                  | ...         | Various bug fixing and client core feature implementations                                                                                                                                    |
| Add mc-like GuiScreen manager system | Planned     | Minecraft 1.12 Client sources may help here                                                                                                                                                   |
| Add Java-like Thread system          | Planned     | My MiniThread implementation may help here                                                                                                                                                    |
| Add GUI system                       | Planned     | Minecraft 1.12 Client sources may help here                                                                                                                                                   |
| OreUI for GUI                        | Planned     | Fully fledged Minecraft OreUI WITH resource pack support!                                                                                                                                     |
| Add Main Menu GUI                    | Planned     | Minecraft 1.12 Client sources and my 3DSCraft attempt may help here                                                                                                                           |
| Add Multiplayer GUI                  | Planned     | Minecraft 1.12 Client sources and my 3DSCraft attempt may help here                                                                                                                           |
| ...                                  | ...         |                                                                                                                                                                                               |
| Implement working sound engine       | Planned     | this can be added if every other core feature has been added properly                                                                                                                         |
| CIA Build target       | Planned     | this doesnt exist yet, but woutl be cool. preferrably with a 3D icon, similar to my 3DSCraft                                                                                                                      |


### Future Features
- [ ] Microsoft authentication (from [prism launcher](https://github.com/PrismLauncher/PrismLauncher)) - this might help as well: [Minecraft Wiki article](https://minecraft.wiki/w/Microsoft_authentication)
- [ ] Newer MCLib version [1.15.2 branch](https://github.com/plushmonkey/mclib/tree/1.15.2) for newer server support
- [ ] SDMC Resource pack support
- [ ] Proper worldgen for client singleplayer
- [ ] On-device server (3DS "local multiplayer" to connect with PC and other 3DSes) — *may be limited by 3DS network capabilities*

### Credits
- Network engine bits from [mclib-3ds](https://github.com/SomeRandoLameo/mclib-3ds)
- Client Engine bits from [craftus](https://github.com/Onixiya/craftus)
- Dear IMGUI for 3DS (used only for debugging) [imgui-ctr](https://github.com/npid7/imgui-impl-ctr/tree/e67a33579fa6424171ec1a583b6baffa077ee2d3)
- Learned how to utilize mclib from [Terracotta](https://github.com/plushmonkey/Terracotta)
- 3DS Compatability layer for IMGUI [imgui-impl-ctr](https://github.com/npid7/imgui-impl-ctr)
- Audio & Other misc. bits from [QuickLime](https://github.com/syelan34/quicklime) (this may be used in the future)

### Special thanks goes out to: 
- Tobid7 ([Amethyst](https://github.com/tobid7/amethyst) and main contributor)
- RSDuck & Onixia ([Craftus](https://github.com/Onixiya/craftus))
- Plushmonkey ([original mclib](https://github.com/plushmonkey/mclib) & [Terra](https://github.com/plushmonkey/Terracotta))
- the people behind [DEAR IMGUI](https://github.com/ocornut/imgui)
- the people behind [stb](https://github.com/nothings/stb)
- And many more! (if you think, your code is used here, feel free to commit a change to the readme. We will validate and add it if possible)

### Licensing
- All external snippets must respect their original licenses
