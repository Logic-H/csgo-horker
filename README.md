# Horker
A simple CSGO Linux external hack.

### Current Features
* AimBot (experimental)
* Automated Contrast/Digital Vibrance control (Nvidia only for now)
* Disable Post Processing
* Glow
* NoFlash
* Recoil Control
* Trigger

##### Mouse button support
There is also mouse button support as of commit cdc56a4.  The only known bug at this time is it doesn't register until after the first shot, which is mostly because I was too lazy to do it via Alt1 for now.

To enable this, you will need to set "TriggerKey" to "Mouse1", "Mouse2", or "Mouse3" respectively, without the quotes.

##### TODO
* Code base is starting to get.... messy?  I need to focus on cleaning this up when I get time.
* Implement more features.
* Error margin for recoil control so its less.... suspicious

### Compiling

##### Build requirements
Current build requirements are as follows:
* X11
* libXNVCtrl (Optional for automated contrast control with Nvidia drivers)

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
