# Horker
A simple CSGO Linux external hack.

### Preamble
I created this as a stop-gap while we work on updating jwaim.  Currently the only supported feature is glow, but I plan on adding a few more nice things once I get the code base in better shape.

I'll keep a feature list here and update it as things change.  There isn't any config to speak of yet, but I do plan on adding one once more features are completed.

### Current Features
* Glow

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

[Imgur](https://i.imgur.com/uMHveto.jpg)
