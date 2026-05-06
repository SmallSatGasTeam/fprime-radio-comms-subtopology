# Hardware Setup & Test Guide: RadioDeployment

This guide outlines the steps to build the `RadioDeployment` for a Raspberry Pi, deploy it, and connect a Ground Data System (GDS) from your laptop over TCP.

## 1. Build for Raspberry Pi (on Laptop)
First, ensure you have the cross-compilation tools installed:
```bash
sudo apt install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
```

Navigate to the deployment directory and build the binary:
```bash
# Activate your F' virtual environment first
source fprime-venv/bin/activate

cd Deployments/RadioDeployment
fprime-util generate arm-hf-linux
fprime-util build
```

## 2. Deploy to Raspberry Pi
Transfer the built binary to the Raspberry Pi. The binary location depends on the platform name used during generation.
```bash
# Replace <PI_USER> and <PI_IP> with your Pi's credentials
# The path below assumes the build was successful for arm-hf-linux
scp build-artifacts/arm-hf-linux/Deployments/RadioDeployment/bin/RadioDeployment <PI_USER>@<PI_IP>:~/
```

## 3. Run FSW on Raspberry Pi
SSH into the Raspberry Pi and execute the binary. You must bind the address to `0.0.0.0` to allow external TCP connections.
```bash
ssh <PI_USER>@<PI_IP>
# On the Pi:
./RadioDeployment -a 0.0.0.0 -p 50000
```
*Note: Using `0.0.0.0` makes the FSW listen on all network interfaces.*

## 4. Connect GDS (on Laptop)
Run the GDS on your laptop without starting a local instance of the application. Point it to the Raspberry Pi's IP address.
```bash
cd Deployments/RadioDeployment
fprime-gds --no-app --ip-address <PI_IP> --port 50000
```

## Verification Steps
- **GDS Connection:** The GDS dashboard should show "Connected" (Green status).
- **Telemetry:** Verify that standard telemetry (e.g., `CdhCore` channels) is updating.
- **Commands:** Send a `No-op` command through the GDS and verify the `CommandResponse` and `Activity_Hi` event in the log.

