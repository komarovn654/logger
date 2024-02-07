# Assembly instructions
CMake is used to create native makefiles or other build projects. The following examples use makefiles.
## MacOS
```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/logger # To install lib in /opt/logger. There is /usr/local/ by default.
make
```
# Install instructions
## MacOS
```bash
cd build/logger/
sudo make install # Install in /usr/local/ by default.
```
# Tests
## MacOS
```bash
cd build/tests/
ctest
```
# Examples
## MacOS
```bash
cd build/examples/
./example
```