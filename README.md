# Godot GOG

GOG Api for the Godot game engine. For Windows, Linux and Mac platforms.

## Documentation

Refer to the code and check out the [GOG documentation](https://docs.gog.com/galaxyapi/)

## Quick How-To

* Download this repository and unpack it.
* Download and unpack the GOG Galaxy SDK; this requires a GOG developer account.
* Download and unpack the Godot source; branch **4.5**
* Move the following to `godotgog/sdk/`:
```
  sdk/Include
  sdk/Libraries/linux32
  sdk/Libraries/linux64
  sdk/Libraries/osx
  sdk/Libraries/win32
  sdk/Libraries/win64
```

- Now move the **godotgog** directory into the **modules** directory of the unpacked Godot Engine source.
- Recompile for your platform:
  - Windows ( https://docs.godotengine.org/en/4.5/engine_details/development/compiling/compiling_for_windows.html )
  - Linux ( https://docs.godotengine.org/en/4.5/engine_details/development/compiling/compiling_for_linuxbsd.html )
  - OSX ( https://docs.godotengine.org/en/4.5/engine_details/development/compiling/compiling_for_macos.html )
- When recompiling the engine is finished, copy the shared library (Galaxy) from sdk/Libraries/ folders to the Godot binary location (by default in the godot source /bin/ file but you can move them to a new folder). It should look like this:
  - Linux 32/64-bit
  ```
  linux32/libGalaxy.so
  linux64/libGalaxy64.so
  ./godot.linux.tools.32 or ./godot.linux.tools.64
  ```
  - OSX
  ```
  osx/libGalaxy64.dylib
  godot.osx.tools.64
  ```
  - Windows 32-bit
  ```
  win32/Galaxy.dll
  win32/Galaxy.lib
  win32/Galaxy.pdb
    godot.windows.tools.32.exe
  ```
  - Windows 64-bit
  ```
  win64/Galaxy64.dll
  win64/Galaxy64.lib
  win64/Galaxy64.pdb
  godot.windows.tools.64.exe
  ```

- Your game must ship with the executable, GOG Galaxy API DLL/SO/DyLIB. Lack of the GOG Galaxy API DLL/SO/DyLib (for your respective OS) will cause it fail and crash.
  - **NOTE:** For OSX, the `libGalaxy64.dylib` must be in the `Content/MacOS/` folder in your application zip or the game will crash.
  - **NOTE:** For OSX you need to run the following command `install_name_tool -add_rpath @executable_path/. <PATH TO THE APP>.app/Contents/MacOS/<EXECUTABLE NAME>`

From here you should be able to call various functions of GOG Galaxy. You should be able to look up the functions in Godot itself under the search section. In addition, you should be able to read the GOG Galaxy API documentation to see what all is available and cross-reference with GodotGOG.

License
-------------
MIT license