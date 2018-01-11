
# Arduino fan controller

# Setup

- Copy `config.example.h` to `config.h`
- Edit `config.h` to configure the different settings:
    + Pinout
    + Sensor list: pin
    + Fan list: pin, speed curve, associated sensor

# Usage

- Short button press
    * Send temperatures & fan speeds to serial output
    * Beep first temperature (mix of morse and roman numbers)
        - `.`: 1, `..`: 2, `...`: 3, `.-`: 4, `-`: 5, `-.`: 6, `-..`: 7, `-...`: 8, `.--`: 9, `--`: 0
- Long button press: Switch fan control modes
    * Auto (beep `.-`, morse code for A): Use curves to control fan speed
    * Low (beep `.-..`, morse code for L): All fans sets to their lowest value on the curve (that is not 0)
    * High (beep `....`, morse code for H): All fans to their highest value on the curve (that is not 100)
    * Full (beep `..-.`, morse code for F): All fans to 100%




