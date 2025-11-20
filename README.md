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
- (you may or may not need more, lacking documentation)

### Roadmap
| Task                                 | Status      | Notes                                                                                                                                                                                         |
|--------------------------------------|-------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Port MCLib                           | ✅ Done      | Ported and tested. Minor crashes may still occur, but i was able to successfully send/control Movement packets                                                                                |
| Port Craftus                         | ✅ Done      | No crashes in C version. Minor 2D layer glitches when a 2D buffer overflows (will be fixed in cpp rewrite). No sound (SoundEngine got removed because it never worked in the real one either) |
| Rename Craftus source files to .cpp  | ✅ Done      |                                                                                                                                                                                               |
| Run MCLib code as separate thread    | ✅ Done      | Test implementation first, there is no need for a proper GUI here. We can use what we already got.                                                                                            |
| Added ImGui for debugging purposes   | ✅ Done      | Added a small Manager for utillizing ImGui everywhere                                                                                                                                         |
| Join Craftus and MCLib               | In Progress | Use the same Items, Blocks, Vectors, Math and more. This requires an almost full rewrite of craftus and feature-expansions of MC-lib (as i found out 27.10.2025 eg. Today)                    |
| Implement proper OOP design          | In Progress | Minecraft 1.12 Client sources may help here                                                                                                                                                   |
| Proper Itemstacks                    | Planned     | A Block uint8 ID is not enough                                                                                                                                                                |
| Inventory                            | Planned     | Survival and Creative                                                                                                                                                                         |
| Armor slots                          | Planned     | Survival and Creative, online only                                                                                                                                                            |
| Offhand slot                         | Planned     | Survival and Creative                                                                                                                                                                         |
| Chest                                | Planned     | only online for now (meaning only when a packet was triggered)                                                                                                                                |
| Furnace                              | Planned     | only online for now (meaning only when a packet was triggered)                                                                                                                                |
| Survival hearts                      | Planned     | I wrote this in my "3DSCraft" project, this could be ported                                                                                                                                   |
| XP Bar                               | Planned     | I wrote this in my "3DSCraft" project, this could be ported as well                                                                                                                           |
| Hotbar                               | Planned     | Survival and Creative                                                                                                                                                                         |
| Chat                                 | Planned     | Terra may help here                                                                                                                                                                           |
| ...                                  | ...         | Various bug fixing and client core feature implementations                                                                                                                                    |
| Autosaving in background             | Planned     | chunks need to to be saved in order to prevent memory flooding                                                                                                                                |
| ...                                  | ...         | Various bug fixing and client core feature implementations                                                                                                                                    |
| Multiplayer world                    | Planned     | A Propper attempt at 3DS Chunk and block rendering, entity rendering and more                                                                                                                 |
| Block interactions                   | Planned     | Blocks like Chests, Furncances and more should be interactive in both singleplayer and multiplayer                                                                                            |
| ...                                  | ...         | Various bug fixing and client core feature implementations                                                                                                                                    |
| Add mc-like GuiScreen manager system | Planned     | Minecraft 1.12 Client sources may help here                                                                                                                                                   |
| Add Java-like Thread system          | Planned     | My MiniThread implementation may help here                                                                                                                                                    |
| Add GUI system                       | Planned     | Minecraft 1.12 Client sources may help here                                                                                                                                                   |
| Add Main Menu GUI                    | Planned     | Minecraft 1.12 Client sources and my 3DSCraft attempt may help here                                                                                                                           |
| Add Multiplayer GUI                  | Planned     | Minecraft 1.12 Client sources and my 3DSCraft attempt may help here                                                                                                                           |
| ...                                  | ...         |                                                                                                                                                                                               |
| Implement working sound engine       | Planned     | this can be added if every other core feature has been added properly                                                                                                                         |

### Future Features
- [ ] Microsoft authentication (from [prism launcher](https://github.com/PrismLauncher/PrismLauncher))
- [ ] Newer MCLib version ([1.15.2 branch](https://github.com/plushmonkey/mclib/tree/1.15.2)) for newer server support
- [ ] SDMC Resource pack support
- [ ] Proper worldgen for client singleplayer
- [ ] On-device server (3DS "local multiplayer" to connect with PC and other 3DSes) — *may be limited by 3DS network capabilities*

### Credits
- Network engine bits from [mclib](https://github.com/plushmonkey/mclib)
- Learned how to utilize mclib from [Terracotta](https://github.com/plushmonkey/Terracotta)
- Client Engine bits from [craftus](https://github.com/Onixiya/craftus)
- Dear IMGUI [imgui](https://github.com/ocornut/imgui/tree/1362fc0c56d8742167379661a353d8342f7c4a86)
- 3DS Compatability layer for IMGUI [imgui-impl-ctr](https://github.com/npid7/imgui-impl-ctr)
- Audio & Other misc. bits from [QuickLime](https://github.com/syelan34/quicklime) (this may be used in the future)

### Special thanks goes out to: 
- Tobid7

### Licensing
- All external snippets must respect their original licenses
