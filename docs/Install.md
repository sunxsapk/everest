# Installation Guide for Everest Engine

Using Everest Engine for building projects in C++ or using Everest Editor to build games without writing
any substantial amount of code, requires one to install Everest Engine in their machine. There are different
ways for you to get __Everest Engine__ up and running on your machine, but the ones discussed here are by
installing it on your machine.

## Installers

Installers for different platforms are provided [here](https://github.com/sunxsapk/everest/releases).
___If not, then it is on its way. Don't worry.___

Select the one for your platform, download it and then run it to install __Everest Engine__ on your
machine.

### __Considerations__

There are few things to consider.

---

## Custom Installation

If you want to install the project by yourself on your machine, then you might have to do a lot more.
First of all, follow the instructions on [SetUp guide](/docs/Setup.md) to install all the dependencies
and start up the project.

Then, you can install the project using `scripts/<os>/install.sh` depending upon your operating system
(linux or macos). ___(It is not available as of now for windows)___

---

# Insights

___The next topics explained are all the things installer does for you. You can just skip this and go
ahead and run the installer, if you want to use the engine immediately, but it is good thing to know
what goes in you machine and how everything works the way it does.___


## What happens during installation process in linux

- All the header files provided by the project are installed in `/usr/local/include/everest` so that
you can include them in your c++ application as `#include <everest.h>`

- The core library of Everest engine is then installed in `/usr/local/lib` which is easily discovered
by compilers if you are linking everest core library in your C++ applications.

- The scene editor for Everest (Everest-Editor) is then installed in `/usr/local/bin` which makes it
easier for you to run this by using command `everest-editor` from anywhere.

- All of the assets used by Everest Engine and Editor are installed in `usr/local/share/everest` directory.

These steps happen, so that you can use the full suite of tools and libraries from anywhere on your machine
for writing any kind of program, that facilitates from core Everest engine and its Editor.


# Uninstalling

## Used .deb to install?

Run the same .deb file, it will show an option to uninstall the program. And there you go..

## Used .tag.gz to install?

Run following commands to remove everything related to Everest Engine, if you've extracted the `.tar.gz`
inside `/usr/local/*` or remove from the directory where you've extracted the `/tar.gz` file:

```bash
sudo rm -rf /usr/local/bin/everest-editor
sudo rm -rf /usr/local/lib/libeverest.*
sudo rm -rf /usr/local/include/everest/
sudo rm -rf /usr/local/share/everest/
```

## Verify uninstallation

Run the following commands and it should not find any trace of Everest Engine.
```bash
which everest-editor  # Should return nothing if removed
ldconfig -p | grep everest  # Should not list Everest-related libraries
```
