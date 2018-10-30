# Wristcoach
> A Pebble app to aid Drive Coaches in FIRST Robotics Competition matches.

Wristcoach helps out by giving a sophisticated and configurable experience which can help FRC Drive Coaches (or even drivers) by giving easy access to a user-friendly countdown and supporting simple reminders.

## Setup
Given that Pebble's services have mostly shut down, you'll need to follow a somewhat unintuitive (but still simple) process to build and install this app on your watch.

(macOS is assumed for instructions. Guidance for other platforms or when not using Homebrew is available on [this Reddit thread](https://www.reddit.com/r/pebble/comments/9i9aqy/developing_for_pebble_without_cloudpebble_windows).)

First, you'll need to install the Pebble command line tools through Homebrew:
```sh
brew install pebble/pebble-sdk/pebble-sdk
```
Contrary to what one might expect from the name, that package does not actually install the Pebble SDK. However, if you try to run the `pebble` command-line tool, it will try and fail to install from Pebble's now-defunct website. So, you will need to forcibly install from a third-party GitHub repository:
```sh
pebble sdk install https://github.com/aveao/PebbleArchive/raw/master/SDKCores/sdk-core-4.3.tar.bz2
```

You will also likely need to install `freetype`. You can do this through
```sh
brew install freetype
```
If that does not work, you will need to manually install from [Freetype's website](https://www.freetype.org/download.html).

In order to push the code to your Pebble, you'll need to connect to it through the Pebble mobile app, then configure the [Pebble Developer Connection](https://developer.rebble.io/developer.pebble.com/guides/tools-and-resources/developer-connection/index.html).

If the standard developer connection method of installation doesn't work, the `.pbw` file can be installed to your Pebble [thus](https://www.youtube.com/watch?v=rTqPyec6EBo).

## License
[MIT](LICENSE)
## Author
Created by [Erik Boesen](https://github.com/ErikBoesen) for [FRC Team 1418](https://github.com/frc1418).
