# Assembly instructions
CMake is used to create native makefiles or other build projects. The following examples use makefiles.
```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/logman # To install lib in /opt/logman. There is /usr/local/ by default.
make
```
# Install instructions
```bash
cd build/logman/
sudo make install # Install in /usr/local/ by default.
```
# Tests
```bash
cd build/tests/
ctest
```
# Examples
```bash
cd build/examples/
./example
```