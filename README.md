# SoulCalibur Vita

<p align="center"><img src="./extras/screenshots/game1.png"></p>

This is a wrapper/port of <b>SoulCalibur</b> for the *PS Vita*.

The port works by loading the official Android ARMv7 executables in memory, resolving its imports with native functions and patching it in order to properly run.
By doing so, it's basically as if we emulate a minimalist Android environment in which we run natively the executable as is.

## Changelog

### v1.0

- Initial release.

## Note

- The port has been tested with v.1.0.15 of the game.

## Setup Instructions (For End Users)

In order to properly install the game, you'll have to follow these steps precisely:

- Install [kubridge](https://github.com/TheOfficialFloW/kubridge/releases/) and [FdFix](https://github.com/TheOfficialFloW/FdFix/releases/) by copying `kubridge.skprx` and `fd_fix.skprx` to your taiHEN plugins folder (usually `ux0:tai`) and adding two entries to your `config.txt` under `*KERNEL`:
  
```
  *KERNEL
  ux0:tai/kubridge.skprx
  ux0:tai/fd_fix.skprx
```

**Note** Don't install fd_fix.skprx if you're using rePatch plugin

- **Optional**: Install [PSVshell](https://github.com/Electry/PSVshell/releases) to overclock your device to 500Mhz.
- Install `libshacccg.suprx`, if you don't have it already, by following [this guide](https://samilops2.gitbook.io/vita-troubleshooting-guide/shader-compiler/extract-libshacccg.suprx).
- Obtain your copy of *SoulCalibur* legally for Android in form of an `.apk` file and an obb. [You can get all the required files directly from your phone](https://stackoverflow.com/questions/11012976/how-do-i-get-the-apk-of-an-installed-app-without-root-access) or by using an apk extractor you can find in the play store.
- Open the apk with your zip explorer and extract the folder `lib` folder to `ux0:data/soulcalibur`.
- Install [WinImage](https://www.winimage.com/download.htm) and open the `obb` file with it.
- Select all the files and right-click them, then click on `Extract` and extract all the files in `ux0:data/soulcalibur`.
- Download [xdelta UI](https://www.romhacking.net/utilities/598/) and extract the zip content in `ux0:data/soulcalibur`.
- Download the `shader.xdelta` patch from Release page and apply it, using `xdelta UI` to `ux0:data/soulcalibur/shader.dat` and replace original file with the patched one.
- **Optional**: For trophies to be unlockable, install [NoTrpDRM](https://github.com/Rinnegatamante/NoTrpDrm).

## Build Instructions (For Developers)

In order to build the loader, you'll need a [vitasdk](https://github.com/vitasdk) build fully compiled with softfp usage.  
You can find a precompiled version here: https://github.com/vitasdk/buildscripts/actions/runs/1102643776.  
Additionally, you'll need these libraries to be compiled as well with `-mfloat-abi=softfp` added to their CFLAGS:

- [SoLoud](https://github.com/vitasdk/packages/blob/master/soloud/VITABUILD)

- [libmathneon](https://github.com/Rinnegatamante/math-neon)

  - ```bash
    make install
    ```

- [vitaShaRK](https://github.com/Rinnegatamante/vitaShaRK)

  - ```bash
    make install
    ```

- [kubridge](https://github.com/TheOfficialFloW/kubridge)

  - ```bash
    mkdir build && cd build
    cmake .. && make install
    ```

- [vitaGL](https://github.com/Rinnegatamante/vitaGL)

  - ````bash
    make SOFTFP_ABI=1 NO_DEBUG=1 HAVE_GLSL_SUPPORT=1 PHYCONT_ON_DEMAND=1 HAVE_UNFLIPPED_FBOS=1 install
    ````

After all these requirements are met, you can compile the loader with the following commands:

```bash
mkdir build && cd build
cmake .. && make
```

## Credits

- TheFloW for the original .so loader.
- gl33ntwine for the awesome Android subsystem reimplementation and [FalsoJNI](https://github.com/v-atamanenko/FalsoJNI).
- BrandonHeat8 for the Livearea assets.
