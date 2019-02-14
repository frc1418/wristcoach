# Wristcoach
> A Pebble app to aid drive coaches in FIRST Robotics Competition matches.

![Screenshot](screenshot.png)

Wristcoach helps out by giving a sophisticated and configurable experience which can help FRC Drive Coaches (or other drivers or team members) by giving easy access to a user-friendly countdown and supporting simple reminders.

The Pebble company announced in 2016 that [it would shut down](https://techcrunch.com/2016/12/07/pebble-confirms-its-shutting-down-devs-and-software-going-to-fitbit/) as a result of accquisition by Fitbit. While the original Pebble web services are no longer active, Pebble watches can still be used fully through third-party open-source platforms and software, and can be bought secondhand quite cheaply, selling for as little as $20 [on websites like eBay](https://www.ebay.com/sch/i.html?_nkw=pebble+watch&_sop=15). This combined with their easy programability makes them an ideal assistant for FRC drive coaches.

## Motivations
As drive coach for [FRC Team 1418](https://github.com/frc1418), I've been challenged in keeping track of time. Depending on your station, it can be difficult to see the countdown on the other side of the field, and looking to the livestream screen is time-consuming when it's even possible. Furthermore, it's difficult to pay enough attention to any clock to remember exactly when drivers must prepare for endgame or other actions. Thus, a simple smartwatch app fits the ideal goal of taking menial mental tasks off my hands as drive coach. With robotic timing tasks moved to my wrist, I can better focus on the field and noticing important events there.

Wristcoach helps drive coaches with the following features:
* An easily-readable countdown, and mode color-coding to help with easy reading
* Automatic vibration at configurable times to remind you it's time for endgame
* Easy tweaking to add other reminders and information

## FAQ
### Is this illegal?
As this app neither sends nor recieves data outside the driver station nor obstructs driver vision, it is legal to use in FRC competitions.

## Setup
(macOS is assumed for developer setup instructions. Guidance for other platforms or when not using Homebrew is available on [this Reddit thread](https://www.reddit.com/r/pebble/comments/9i9aqy/developing_for_pebble_without_cloudpebble_windows).)

1. On your mobile device, install the Pebble app ([Android](https://play.google.com/store/apps/details?id=com.getpebble.android.basalt), [iOS](https://itunes.apple.com/us/app/pebble/id957997620)).
2. Visit [boot.rebble.io](https://boot.rebble.io) on your mobile device, and follow those instructions to point your Pebble app to the alternative Rebble server.
3. Connect to your Pebble through the Pebble mobile app, then configure the [Pebble Developer Connection](https://developer.rebble.io/developer.pebble.com/guides/tools-and-resources/developer-connection/index.html).
4. On your development machine, install the Pebble command line tools and dependencies through Homebrew:
```sh
brew install pebble/pebble-sdk/pebble-sdk freetype
```
5. Download the newest Pebble SDK from GitHub (the standard Pebble command line tool attempts to install from the now-defunct Pebble server, so a third-party copy must be used):
```sh
pebble sdk install https://github.com/aveao/PebbleArchive/raw/master/SDKCores/sdk-core-4.3.tar.bz2
```
6. Then, build your app and install through the Pebble Developer Connection, providing the IP of your mobile device as a parameter to installation:
```sh
pebble build
pebble install --phone your_phone_ip
```

(If the standard developer connection method of installation doesn't work, the `.pbw` file can be installed to your Pebble [thus](https://www.youtube.com/watch?v=rTqPyec6EBo). This method is highly inconvenient and is not recommended.)

## License
[MIT](LICENSE)
## Author
Created by [Erik Boesen](https://github.com/ErikBoesen) for [FRC Team 1418](https://github.com/frc1418).
