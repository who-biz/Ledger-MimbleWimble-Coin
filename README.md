# Biz Compilation Notes:

Ensure that you have the SDK & BOLOS environments set up as subdirectories in root of this repository.  If you do not, you will experience some compilation errors due to header declarations not being found by source files in SDK.  Namely, `C_warning_icon` in my tests.

You do not need to download `clang` from github releases, as well.  Any old `clang` will work, but make sure its v13.0.1 or higher.

### Python Modules omitted in instructions below:

You will need to install `python` and `pip`, then use `pip` to install `Pillow` and `ledgerblue` modules.

```
pip install Pillow
pip install ledgerblue
```

Other than these steps, compilation is very straightforward.

### Compile for Epic Cash on Nano S Plus:

First, set up environment for `bolos-environment` and `nanosplus-secure-sdk` following instructions in `Building` section of MWC/GRIN README below.  After that, follow the sections that follow *directly below* for **Epic Cash**:

**Compile embedded app for Epic Cash:**
```
make CURRENCY=epic_cash
```

**Load embedded app onto Ledger:**

With your ledger unlocked, run the following:

```
make load CURRENCY=epic_cash
```

### Functional Tests for Epic Cash:

In order for functional tests to succeed on a physical device, you need to import the following mnemonic which is used for testing:


```
1. abandon, 2.) abandon, 3.) abandon, 4.) abandon, 5.) abandon, 6.) abandon, 7.) abandon, 8.) abandon, 9.) abandon, 10.) abandon, 11.) abandon 12.) about
```


Obviously, don't send any meaningful funds to the associated Root or HD addresses/keys.

Once this mnemonic has been entered to the device, load the app with `make load CURRENCY=epic_cash` (if you haven't already).

Install remaining dependencies:

```
npm i @ledgerhq/hw-transport-node-speculos @ledgerhq/hw-transport-node-hid
```

Finally, open the app, so that you see a screen that reads `Epic Cash is ready`, then run the following command:

```
make functional_tests CURRENCY=epic_cash
```

You will be asked to confirm various displays.

### Unit Tests for Epic Cash

**Install CMocka development package:**

```
sudo apt-get install libcmocka-dev
```

**Add `TARGET` environment variable:**

```bash
# For Nano S Plus:
export TARGET=nanos2
```
Other possible targets are as follows: Nano S (`nanos`), Nano X (`nanox`), Stax (`fatstacks`)

**Compile Unit tests for given target:**

```
cmake -Btest/unit_tests/build -Htest/unit_tests/
cd test/unit_tests/build
make
```
You can then run the tests singularly, or all together.

To run all 9 tests at once, call `make test` from `test/unit_tests/build` directory.


# MimbleWimble Coin (MWC) and Grin (GRIN) Ledger hardware wallet apps

### Description
MimbleWimble Coin (MWC) and Grin (GRIN) apps for Ledger Nano S, Ledger Nano X, Ledger Nano S Plus, and Ledger Stax hardware wallets.

These apps can be installed onto Ledger Nano S and Ledger Nano S Plus hardware wallets by going [here](https://htmlpreview.github.io/?https://github.com/NicolasFlamel1/Ledger-MimbleWimble-Coin/blob/master/tools/installer/index.html) with a web browser that supports [WebUSB](https://caniuse.com/webusb).

Ledger Live Desktop and Ledger Live Mobile with MimbleWimble Coin (MWC) and Grin (GRIN) support can be downloaded [here](https://github.com/NicolasFlamel1/ledger-live/releases).

[These video tutorials](https://youtube.com/playlist?list=PLb1nuT3sFYbD_sydCVCngbvATsm9RwWyF) go over how to start using Grin with Ledger Live Desktop/Mobile and how to send Grin to and receive Grin from popular wallets and exchanges with it.

### Building
Building these apps requires setting up a Ledger Nano S, Ledger Nano X, Ledger Nano S Plus, or Ledger Stax build environment. The following steps describe how to do this on Linux.

Install dependencies:
```
sudo apt install libc6-dev gcc-multilib g++-multilib
```
Download the Ledger Nano S SDK, Ledger Nano X SDK, Ledger Nano S Plus SDK, and/or Ledger Stax SDK:
```
git clone https://github.com/LedgerHQ/nanos-secure-sdk.git

git clone https://github.com/LedgerHQ/ledger-secure-sdk.git
mv ledger-secure-sdk nanox-secure-sdk
cd nanox-secure-sdk
git checkout nanox_2.1.0
git checkout API_LEVEL_1
git pull
echo nanox > .target
cd ..

git clone https://github.com/LedgerHQ/ledger-secure-sdk.git
mv ledger-secure-sdk nanosplus-secure-sdk
cd nanosplus-secure-sdk
git checkout nanos+_1.1.0
git checkout API_LEVEL_1
git pull
echo nanos2 > .target
cd ..

git clone https://github.com/LedgerHQ/ledger-secure-sdk.git
mv ledger-secure-sdk stax-secure-sdk
cd stax-secure-sdk
git checkout stax_1.0.0-rc9
git checkout API_LEVEL_5
git pull
echo stax > .target
cd ..
```
Download the ARM and Clang development tools:
```
wget -O gcc.tar.bz2 'https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2'
wget -O clang.tar.xz 'https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.1/clang+llvm-13.0.1-x86_64-linux-gnu-ubuntu-18.04.tar.xz'
mkdir bolos-environment
tar -xJf clang.tar.xz --directory bolos-environment
rm clang.tar.xz
mv bolos-environment/clang+llvm-13.0.1-x86_64-linux-gnu-ubuntu-18.04 bolos-environment/clang-arm-fropi
tar -xjf gcc.tar.bz2 --directory bolos-environment
rm gcc.tar.bz2
mv bolos-environment/gcc-arm-none-eabi-10.3-2021.10 bolos-environment/gcc-arm-none-eabi
```
Add environmental variables (Remove the starting # from the line in your ~/.bashrc file for the device that you want to build for):
```
echo "#export BOLOS_SDK=$PWD/nanos-secure-sdk" >> ~/.bashrc
echo "#export BOLOS_SDK=$PWD/nanox-secure-sdk" >> ~/.bashrc
echo "#export BOLOS_SDK=$PWD/nanosplus-secure-sdk" >> ~/.bashrc
echo "#export BOLOS_SDK=$PWD/stax-secure-sdk" >> ~/.bashrc
echo "export BOLOS_ENV=$PWD/bolos-environment" >> ~/.bashrc
```
The apps can then be built with the following commands:
```
make CURRENCY=mimblewimble_coin
make CURRENCY=grin
```
The apps can be installed onto Ledger Nano S, Ledger Nano S Plus, and Ledger Stax hardware wallets with the following commands:
```
make load CURRENCY=mimblewimble_coin
make load CURRENCY=grin
```

### Testing
Functional tests for the apps can be ran with the following commands:
```
npm i @ledgerhq/hw-transport-node-speculos @ledgerhq/hw-transport-node-hid
make functional_tests CURRENCY=mimblewimble_coin
make functional_tests CURRENCY=grin
```
Unit tests can be ran with the following commands:
```
cmake -Btest/unit_tests/build -Htest/unit_tests/
make -C test/unit_tests/build/
make -C test/unit_tests/build test
```
