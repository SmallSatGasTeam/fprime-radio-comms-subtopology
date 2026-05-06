# Configuration Injection — `TransceiverConfig.hpp`

This module is intentionally free of radio-specific values. Hardware-specific constants (preamble, sync word, configuration command strings, callsigns, etc.) are injected at build time through a local config file that you provide.

## Steps

1. **Copy the template**

   ```bash
   cp keys_template/TransceiverConfig.hpp.example keys_template/TransceiverConfig.hpp
   ```

2. **Fill in the values** using your radio's datasheet.

3. **Add to your CMake include path** — in your deployment's `CMakeLists.txt`, before including this module:

   ```cmake
   include_directories("${CMAKE_SOURCE_DIR}/keys_template")
   ```

4. **Never commit the populated file.** `keys_template/TransceiverConfig.hpp` is listed in `.gitignore`. If you accidentally stage it:

   ```bash
   git rm --cached keys_template/TransceiverConfig.hpp
   ```

## Field reference

| Field | Description |
|---|---|
| `radio.preambleByte` | Repeated byte forming the preamble sequence |
| `radio.preambleLen` | Number of preamble bytes |
| `radio.syncWord` | Byte marking the start of frame |
| `radio.lengthFieldLen` | Bytes encoding the payload length |
| `radio.crcLen` | CRC byte count (e.g., 2) |
| `radio.crcPoly` | CRC-16 polynomial (CRC-16-CCITT = `0x1021`) |
| `radio.crcInit` | CRC-16 initial value (CRC-16-CCITT = `0xFFFF`) |
| `i2c.address` | I2C 7-bit slave address |
| `i2c.WRITE_*` / `i2c.READ_*` | Configuration command strings from the radio's datasheet |
| `beacon.beaconPayload` | ASCII string broadcast as the periodic beacon |
