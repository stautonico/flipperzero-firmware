<h2>

[Flipper Zero Firmware](https://github.com/flipperdevices/flipperzero-firmware/blob/dev/ReadMe.md) <= READ THIS READ ME</h2>
<b>Latest Updates:</b>
- Added [WiFi Scanner (By gotnull)](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/pull/99) `HIDDEN "wifi_scanner"` due to hardware requirement
- Renamed many apps internally to have ELFs compiled with the right name
- Updated [`ELF Loader`/[FL-2627] Plugins API #1387 (By hedger/DrZlo13)](https://github.com/flipperdevices/flipperzero-firmware/pull/1387), no more ELFs in /ext/elf
- - Now its FAPs in /ext/apps. Untested and unreleased. Feel free to test and compile your own FAPs.

<table width="100%" border="0" cellspacing="0">
  <tr>
    <td>

<h3>This software is for experimental purposes only and is not meant for any illegal activity/purposes. We do not condone illegal activity and strongly encourage keeping transmissions to legal/valid uses allowed by law.</h3>
<br><b>FLASH STOCK FIRST BEFORE UPDATING TO CUSTOM FIRMWARE<b>
    
<br>[FLIPPER PROJECT WISH LIST](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/RoadMap.md) - [SAMPLE EDUCATIONAL PROJECTS 😄](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/RoadMap.md)
<br>[`FUN Links HERE` Collection for your Flipper SD](https://github.com/RogueMaster/awesome-flipperzero-withModules)
</td>
      <td>

<h2>DONATIONS ACCEPTED 😄🚀💸</h2>
BTC: <B>172CE1H1XZVt7tc4HDQ1q9Mej1K7VuHfEf</B><br>
ETH: <B>0x6c46eE48Bd433b0ADE711B43D188Cf8215cd51c2</B>
    <br>
    <br>
    
Contact me on [Discord](https://discord.gg/gF2bBUzAFe) if you want a renamed Flipper DFU. I can do custom names too!
<br><h2>[Join my Flipper/Amiibo Discord](https://discord.gg/gF2bBUzAFe)</h2>
</td>
      </tr>
      <tr><td>

<details>
  <summary><B>You should clone with</B></summary></br>

  ```shell
$ git clone --recursive https://github.com/RogueMaster/flipperzero-firmware-wPlugins.git
$ docker-compose up -d
$ docker-compose exec dev ./fbt resources icons
$ docker-compose exec dev ./fbt firmware_extapps FIRMWARE_APP_SET=ext_apps
$ docker-compose exec dev ./fbt
```
</details>

<details>
  <summary><B>Special Instructions</B></summary></br>
  
- [GAMES ONLY MODE PASSWORD: UP UP DOWN DOWN LEFT CENTER LEFT CENTER FOR MAIN MENU](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/GAMES_ONLY.md)
- Add a folder to SD card named `elf` ([the `.elf` files](https://github.com/RogueMaster/awesome-flipperzero-withModules/tree/rogue_main/elf))
- Download these files into the subghz/assets folder on your SD card. Edit the two `_map` files to contain your specific subghz (.SUB) files.
- - Note 1: If you don't have a subghz/assets folder, you should ensure you have made at least one clean flash with stock firmware and your SD card installed in order to ensure the database structure is built, otherwise it will not exist for alternative forks.
- - Note 2: /any is a special keyword signifying either /int (internal storage) or /ext (external storage).
- - Note 3: the changes you are making to the `_map` files is to point to the location of the specific assets of the touchtunes folder as well as the universal RF map apps which you will have to develop or compile seperately and are not a part of this repo. 
- - Note 4: /any is effectively root, so the folder structure should start "/any/subghz/assets" and not what is based on the repo below do not blindly copy the repo it will not work.
- - [assets/resources/subghz/assets/universal_rf_map.txt](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/assets/resources/subghz/assets/universal_rf_map.txt)
- - [assets/resources/subghz/assets/touchtunes_map.txt](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/assets/resources/subghz/assets/touchtunes_map.txt)
- - [assets/resources/subghz/assets/setting_user](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/assets/resources/subghz/assets/setting_user) **UPDATE IGNORE FLAG TO TRUE TO UNLEASH YOUR FLIPPER!!**
- Download this file into the nfc/assets folder on your SD card. 
- - [assets/resources/nfc/assets/mf_classic_dict.nfc](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/assets/resources/nfc/assets/mf_classic_dict.nfc)
- Download this file into the infrared/assets folder on your SD card. 
- - [assets/resources/infrared/assets/tv.ir](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/assets/resources/infrared/assets/tv.ir)
- Add a folder to SD card named `wav_player` (for 8bit 2ch unsigned wav files) 
- Add a folder to SD card named `music_player` (FMF and RTTTL/TXT files)
- Eventually, if all possible apps are added to [applications/meta/application.fam](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/applications/meta/application.fam), your DFU will not install due to size. However, currently, all apps @ 817KB DFU has been tested to install successfully.
</details>


## Open PRs Checkedout & Not Merged In Main:
- [`ELF Loader`/[FL-2627] Plugins API #1387 (By hedger/DrZlo13)](https://github.com/flipperdevices/flipperzero-firmware/pull/1387)
  
</td><td>
<details>
  <summary><B>All Changes/Features</b></summary><br/>
  
- Animations: Hold Center to change flipper idle animation. [Thanks to Zycenios](https://github.com/flipperdevices/flipperzero-firmware/commit/111786ef40e50a40d2e510595672b569d9b97bba) With changes by RogueMaster.
- Animations: Idle animations will show all animations regardless of level and butthurt [Thanks to qqMajiKpp]
- Animations: SubGHZ Scanning image with Pikachu [Thanks to Panzer00Z](https://github.com/Panzer00Z/flipperzero-firmware/blob/3a548ea9bb181c9348d8afb427890c411456134e/assets/icons/SubGhz/Scanning_123x52.png)
- Development free space thanks to removal of unused debug tools and [thanks to ESurge](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/pull/46/files) for removal of first start assets.
- Flashing the firmware using the blackmagic board with make blackmagic_load [From WeTox](https://github.com/wetox-team/flipperzero-firmware)
- Icon Decode/Encode [(Thanks to PixlEmly)](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/pull/55/files)
- NFC: Display UL PWD_AUTH payload / ntag-pwd-capture [(Thanks to GMMan)](https://github.com/GMMan/flipperzero-firmware/tree/dev-new)
- NFC: Mifare UL update to show name of emulated tag (By RogueMaster with some help from GMMan)
- Plugins: Fixed sound decay issues on music player [Thanks to qqMajiKpp]
- Plugins: Menu, Icons and Raycast exit [Thanks to Redlink](https://github.com/redlink2/flipperzero-firmware/tree/menuChanges)
- Plugins: Snake and Tetris show score. [Thanks to whoamins](https://github.com/flipperdevices/flipperzero-firmware/commit/7feda832ede1ba8468eff2ca055fef3ddbdc16ac) With changes by RogueMaster.
- RFID: Direct Emulation from [(atomsmasha)](https://github.com/flipperdevices/flipperzero-firmware/pull/1371)
- RFID: Removing T5577 passwords via the cli command rfid clear_pass_t5577 using a dictionary attack [From WeTox](https://github.com/wetox-team/flipperzero-firmware)
- Settings: Actual PIN Lock [(By RogueMaster)](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/applications/desktop/desktop.c)
- Settings: Auto-Lock Options Added: 10s+15s+90s [(By RogueMaster)](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/applications/desktop/desktop_settings/scenes/desktop_settings_scene_start.c)
- Settings: Battery Meter on Desktop [Thanks to McAzzaMan](https://github.com/McAzzaMan/flipperzero-firmware/tree/BatteryPercentageView)
- Settings: Desktop => [Games Only Mode (By RogueMaster)](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/GAMES_ONLY.md) [(Thanks to Astrrra for Inverted Mode to Mimic)](https://github.com/wetox-team/flipperzero-firmware/commit/ce91582b7417c5d7a9d8416c17a102d3a5868238)
- - [UP UP DOWN DOWN LEFT CENTER LEFT CENTER FOR MAIN MENU](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/GAMES_ONLY.md)<== FULL LIST OF GAMES ONLY CONTROLS
- Settings: Favorite Game by holding UP on Desktop [Thanks to gotnull](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/pull/57)
- Settings: Hold Down for Games Menu [(Thanks to ESurge)](https://github.com/ESurge/flipperzero-firmware-wPlugins)
- Settings: LCD Timeout Options Added: 10s+90s+2min+5min+10min [(By RogueMaster)](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/applications/notification/notification_settings_app.c)
- Settings: Renamed flippers have the correct name for Bluetooth pairing
- Settings: Scan names will have timestamp instead of random name assigned for [NFC](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/lib/toolbox/random_name.c) and [SubGHz](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/applications/subghz/scenes/subghz_scene_read_raw.c) (By RogueMaster)
- SubGHz: Auto Detect Raw in READ action from [perspecdev](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/pull/79)
- SubGHz: Protocols Came Atomo, FAAC_SLH, Keeloq, Keeloq Common, Nice Flor S, SecPlus v1+v2 and Star Line updates from [Eng1n33r](https://github.com/Eng1n33r/flipperzero-firmware)
- SubGHz: Unlock from SD flag from [(cloudbreakdaniel)](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/commit/97db0dc91ee3dff812b4dec0618e3f198de14405).  Update `subghz/assets/setting_user` with [this file](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/assets/resources/subghz/assets/setting_user) on SD.  **UPDATE IGNORE FLAG TO TRUE TO UNLEASH YOUR FLIPPER!!**

 </details>

<details>
  <summary><B>GAMES</b></summary><br/>
  
- - Games `HIDDEN` if they dont work well; can be added under [applications/meta/application.fam](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/applications/meta/application.fam#L54)
- - Check [the `.elf` files](https://github.com/RogueMaster/awesome-flipperzero-withModules/tree/rogue_main/elf) for external SD games
- `ELF` [2048 (By OlegSchwann)](https://github.com/OlegSchwann/flipperzero-firmware/tree/hackaton/game_2048/applications/game-2048)
- `ELF` [Arkanoid (By gotnull)](https://github.com/gotnull/flipperzero-firmware-wPlugins)
- [Chess (By Okalachev)](https://github.com/okalachev/flipperzero-firmware/tree/chess) Crashes 1st load if FW <~750KB or every load on larger FW `HIDDEN "chess_game"`
- [Chip8 Emulator (By mega8bit)](https://github.com/mega8bit/flipperzero-firmware) Updated by ESurge. Add SD folder `chip8`, [Get GAMES HERE](https://johnearnest.github.io/chip8Archive/) `HIDDEN "chip8"`
- `ELF` [Dice Roller Including SEX/WAR/8BALL/WEED DICE (By RogueMaster)](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/unleashed/applications/dice/dice.c)
- `ELF` [Flappy Bird (By DroomOne)](https://github.com/DroomOne/flipperzero-firmware/tree/dev/applications/flappy_bird)
- [Game of Life (Updated to work by tgxn)(By itsyourbedtime)](https://github.com/tgxn/flipperzero-firmware/blob/dev/applications/game_of_life/game_of_life.c)
- `ELF` [Mandelbrot Set (By Possibly-Matt)](https://github.com/Possibly-Matt/flipperzero-firmware-wPlugins)
- [RayCast (Bt Zlo)](https://github.com/flipperdevices/flipperzero-firmware/tree/zlo/raycast-game-engine)
- `ELF` Snake [OFW]
- [Tanks (By Alexgr13)](https://github.com/alexgr13/flipperzero-firmware/tree/fork/dev/applications/tanks-game)
- [Tetris (By jeffplang)](https://github.com/jeffplang/flipperzero-firmware/tree/tetris_game/applications/tetris_game)
- `ELF` [Tic Tac Toe (By gotnull)](https://github.com/gotnull/flipperzero-firmware-wPlugins)
- `ELF` [Video Poker (By PixlEmly)](https://github.com/PixlEmly/flipperzero-firmware-testing/blob/unleashed/applications/VideoPoker/poker.c)
</details>

<details>
  <summary><B>PLUGINS</b></summary><br/>

- - Check [the `.elf` files](https://github.com/RogueMaster/awesome-flipperzero-withModules/tree/rogue_main/elf) for external SD plugins
- `ELF` [Barcode Generator (By McAzzaMan)](https://github.com/McAzzaMan/flipperzero-firmware/tree/UPC-A_Barcode_Generator/applications/barcode_generator)
- `ELF` [Bluetooth Remote (By Cutch)[OFW]](https://github.com/flipperdevices/flipperzero-firmware/pull/1330)
- [Clock/Stopwatch (By CompaqDisc, Stopwatch & Sound Alert By RogueMaster)](https://gist.github.com/CompaqDisc/4e329c501bd03c1e801849b81f48ea61)
- `ELF` [Dolphin Backup (By nminaylov)](https://github.com/flipperdevices/flipperzero-firmware/pull/1384) Modified by RogueMaster
- `ELF` [Dolphin Restorer (By nminaylov)](https://github.com/flipperdevices/flipperzero-firmware/pull/1384) Cloned by RogueMaster
- [HID Analyzer (By Ownasaurus)](https://github.com/Ownasaurus/flipperzero-firmware/tree/hid-analyzer/applications/hid_analyzer)
- [Mouse Jacker (By mothball187)](https://github.com/mothball187/flipperzero-nrf24/tree/main/mousejacker) ([Pin Out](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/tree/unleashed/applications/mousejacker) from nocomp/Frog/UberGuidoZ) `Req: NRF24`
- `ELF` [Mouse Jiggler (By Jacob-Tate)](https://github.com/Jacob-Tate/flipperzero-firmware/blob/dev/applications/mouse_jiggler/mouse_jiggler.c) (Original By MuddleBox)
- `ELF` Music Player [OFW]
- [NRF Sniff (By mothball187)](https://github.com/mothball187/flipperzero-nrf24/tree/main/nrfsniff) ([Pin Out](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/tree/unleashed/applications/nrfsniff) from nocomp/Frog/UberGuidoZ) `Req: NRF24`
- [PicoPass Reader (By Bettse)](https://github.com/flipperdevices/flipperzero-firmware/pull/1366)
- [RF Remix (By ESurge)](https://github.com/ESurge/flipperzero-firmware-unirfremix) [(Original By jimilinuxguy)](https://github.com/jimilinuxguy/flipperzero-universal-rf-remote/tree/028d615c83f059bb2c905530ddb3d4efbd3cbcae/applications/jukebox)
- `ELF` [Sentry Safe (By H4ckd4ddy)](https://github.com/H4ckd4ddy/flipperzero-sentry-safe-plugin) ([Pin Out](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/tree/unleashed/applications/sentry_safe) from [UberGuidoZ](https://github.com/UberGuidoZ/))
- `ELF` [Spectrum Analyzer (By jolcese)](https://github.com/jolcese/flipperzero-firmware/tree/spectrum/applications/spectrum_analyzer) [Updates (for testing) Thanks to theY4Kman](https://github.com/theY4Kman/flipperzero-firmware)
- [Touch Tunes Remote (By jimilinuxguy)](https://github.com/jimilinuxguy/flipperzero-universal-rf-remote/tree/028d615c83f059bb2c905530ddb3d4efbd3cbcae/applications/jukebox)
- `ELF` [WAV Player (By Zlo)](https://github.com/flipperdevices/flipperzero-firmware/tree/zlo/wav-player) Updated by Atmanos & RogueMaster To Work
- [WiFi Scanner (By gotnull)](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/pull/99) `HIDDEN "wifi_scanner"` due to `Req: ESP8266`
</details>
</td></tr></table>

### Thank you, [MuddleBox](https://github.com/MuddledBox/flipperzero-firmware), [Eng1n33r](https://github.com/Eng1n33r/flipperzero-firmware), [WeTox-Team](https://github.com/wetox-team/flipperzero-firmware) & of course, most of all [Flipper Devices](https://github.com/flipperdevices/flipperzero-firmware)!
