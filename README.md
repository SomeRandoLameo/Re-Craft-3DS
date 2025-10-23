# Re-Craft-3DS
a Minecraft (1.10.x - 1.12.x) client implemented on the 3DS (and new 3DS)

### Other informations
This is a hobby project. I cannot tell how far this will go in the future but for now this will stay active for a longer amount of time.
I wish for other devs that may code with me, so this project can become bigger, like up to recent minecraft versions, proper authentications and more
Please dont be annoyed if this crashed on your device, as this is my first proper c++ lang and 3DS hardware project

### Additional libraries (you need to install them in order to build this project)
- 3ds-zlib
- 3ds-curl 
- 3ds-mbedtls
- 3ds-libopus
- (you may or may not need more, lacking documentation)

### Roadmap
| Task | Status | Notes |
|------|--------|-------|
| Port MCLib | ✅ Done | Ported and tested. Minor crashes may still occur, but i was able to successfully send/control Movement packets |
| Port Craftus | ✅ Done | No crashes in C version. Minor 2D layer glitches when a 2D buffer overflows (will be fixed in cpp rewrite). No sound (SoundEngine got removed because it never worked in the real one either) |
| Rename Craftus source files to .cpp | ✅ Done ||
| Implement proper OOP design | In Progress | Minecraft 1.12 Client sources may help here |
| Join Craftus and MCLib | In Progress | Use the same Items, Blocks, Vectors, Math and more|
| Run MCLib code as separate thread | Planned | Test implementation first, there is no need for a proper GUI here. We can use what we already got. |
| Add mc-like GuiScreen manager system | Planned | Minecraft 1.12 Client sources may help here |
| Add Java-like Thread system | Planned | My MiniThread implementation may help here |
| Add GUI system | Planned | Minecraft 1.12 Client sources may help here |
| Add Main Menu GUI | Planned | Minecraft 1.12 Client sources and my 3DSCraft attempt may help here |
| Add Multiplayer GUI | Planned | Minecraft 1.12 Client sources and my 3DSCraft attempt may help here |
| ... | ... ||
| Implement working sound engine | Planned ||

### Future Features
- [ ] Microsoft authentication (from [prism launcher](https://github.com/PrismLauncher/PrismLauncher))
- [ ] Newer MCLib version ([1.15.2 branch](https://github.com/plushmonkey/mclib/tree/1.15.2)) for newer server support
- [ ] SDMC Resource pack support
- [ ] Proper worldgen for client singleplayer
- [ ] On-device server (3DS "local multiplayer" to connect with PC and other 3DSes) — *may be limited by 3DS network capabilities*

### Credits
- Core bits from [mclib](https://github.com/plushmonkey/mclib)
- Engine bits from [craftus](https://github.com/Onixiya/craftus)

### Licensing
- All external snippets must respect their original licenses
