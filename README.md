# NICs Map
A physical map that shows the [network information center](https://en.wikipedia.org/wiki/InterNIC) / [regional internet registry](https://en.wikipedia.org/wiki/Regional_Internet_registry) responsible for the [IPv4 address](http://www.iana.org/assignments/ipv4-address-space) of incoming TCP connections on a specific port based on the first byte of the remote IP address.

## Map
Map [design files](map) by [T. Amberg](https://www.tamberg.org) licensed [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/deed.en), based on [this map](https://en.wikipedia.org/wiki/Regional_Internet_registry#/media/File:Regional_Internet_Registries_world_map.svg) by [Wikipedia.org](https://en.wikipedia.org/) licensed [CC BY-SA 3.0](https://creativecommons.org/licenses/by-sa/3.0/deed.en), edited on [Inkscape](https://inkscape.org), laser-cut at [FabLab Zürich](https://zurich.fablab.ch).

Address space information based on [IANA assignments](http://www.iana.org/assignments/ipv4-address-space) licensed under [their licensing terms](https://www.iana.org/help/licensing-terms).

To adapt the GPIO pins to your wiring on a Pi, see [this pinout](https://pinout.xyz).

## Code
Requires [pinctrl](https://github.com/raspberrypi/utils/tree/master/pinctrl).

### Build
Build [nics.c](nics.c) on Linux
```console
$ gcc -o nics nics.c
```
### Run
Run on Linux
```console
$ ./nics
```
### Test
Test on Linux
```console
$ curl -v 127.0.0.1:8080/#{0..999}
```
