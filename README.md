# qrc
qrc utilities
- For research and educational purposes only
- Open source under [LICENSE.txt]

## Build & Use

### Build

#### Requirements
To build from source, please make sure:
- [CMake] and a build tool are installed and in `PATH`
- C/C++ compilers are installed and configured properly
#### Configure Build With CMake
1. Switch to project directory.
2. Configure build with `cmake -B build -S .`
#### Build `qrc`
1. Switch to build directory with `pushd build`
2. Build the binary.
	##### Linux (Makefile)
- Build the binary with `make qrc`
	##### Windows (MSBuild)
- Build the binary with `msbuild /m /p:Configuration=Release qrc.vcxproj`
<br><br>
3. Move the binary.
	##### Linux (Makefile)
- Move the binary to wherever you like with `mv qrcdec qrc2lrc new_path_to_binaries`
	##### Windows (MSBuild)
- Move the binary to wherever you like with `move Release\qrcdec.exe,Release\qrc2lrc.exe new_path_to_binaries`
<br><br>
4. Switch back to project directory with `popd`

### Use

- To use `qrcdec`, simply execute it for an interactive interface or use `/path/to/qrcdec <filelist>` from CLI.
- To use `qrc2lrc`, simply execute it for an interactive interface or use `/path/to/qrc2lrc <filelist>` from CLI.

[LICENSE.txt]: https://www.apache.org/licenses/LICENSE-2.0.txt
[CMake]: https://cmake.org/download/
