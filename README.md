# Horker
A simple CSGO Linux external hack.

### Current Features
* AimBot (experimental)
* Glow
* NoFlash
* Trigger

### Compiling
##### Clone the repository
```bash
git clone https://github.com/Teklad/csgo-horker.git
```

##### Build the project
```bash
cd csgo-horker
mkdir build && cd build
cmake ..
make
```

### Running
In order to run this, you MUST use sudo/su due to the memory functions.  Once you've compiled the project, you can run it from the build directory:
```bash
sudo ./src/horker
```

On first run, a config.ini file will be placed in your build directory.  If you want easier access to the file use chown to get ownership of the file.  The list of options available will grow with the project.

### Screenshots
![Screenshot 1](https://github.com/Teklad/resources/blob/master/csgo-horker/glow1.jpg?raw=true "One")
![Screenshot 2](https://github.com/Teklad/resources/blob/master/csgo-horker/glow2.jpg?raw=true "Two")
