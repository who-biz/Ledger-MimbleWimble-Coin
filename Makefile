# Check if BOLOS SDK isn't defined
ifeq ($(BOLOS_SDK),)

# Display error
$(error Environment variable BOLOS_SDK is not set)
endif

# Include BOLOS SDK Makefile defines
include $(BOLOS_SDK)/Makefile.defines

# Application parameters
APP_LOAD_PARAMS += $(COMMON_LOAD_PARAMS)

# Application version
APPVERSION_M = 7
APPVERSION_N = 0
APPVERSION_P = 0
APPVERSION = "$(APPVERSION_M).$(APPVERSION_N).$(APPVERSION_P)"

# Add security review banner. To be removed once Ledger security review is done.
APP_LOAD_PARAMS += --tlvraw 9F:01
DEFINES += HAVE_PENDING_REVIEW_SCREEN

# Emulator flags
EMULATOR_FLAGS = --model `echo $(lastword $(subst _, ,$(TARGET_NAME))) | tr 2 P | tr A-Z a-z` --seed "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Check if target version is defined
ifneq ($(TARGET_VERSION),)

	# SDK emulator flag
	EMULATOR_FLAGS += --sdk $(subst $(eval) ,.,$(wordlist 1,2,$(subst ., ,$(TARGET_VERSION))))
endif

# Check if currency isn't defined
ifndef CURRENCY

	# Set currency to MimbleWimble Coin
	CURRENCY = epic_cash
endif

# Check if currency is MimbleWimble Coin
ifeq ($(CURRENCY),epic_cash)

	# Application name
	APPNAME = "Epic Cash"

	# 44'/744' path on secp256k1 curve
	APP_LOAD_PARAMS += --curve secp256k1 --path "44'/744'"

	# Check if target is the Nano X
	ifeq ($(TARGET_NAME),TARGET_NANOX)

		# APPLICATION_FLAG_LIBRARY and APPLICATION_FLAG_BOLOS_SETTINGS application flags for Bluetooth
		APP_LOAD_PARAMS += --appFlags 0xA00

	# Otherwise check if target is the Stax
	else ifeq ($(TARGET_NAME),TARGET_STAX)

		# APPLICATION_FLAG_LIBRARY and APPLICATION_FLAG_BOLOS_SETTINGS application flags for Bluetooth
		APP_LOAD_PARAMS += --appFlags 0xA00

	# Otherwise
	else

		# APPLICATION_FLAG_LIBRARY application flags
		APP_LOAD_PARAMS += --appFlags 0x800
	endif

	# Defines
	DEFINES += CURRENCY_ID=0
	DEFINES += CURRENCY_BIP44_COIN_TYPE=744
	DEFINES += CURRENCY_FRACTIONAL_DIGITS=9
	DEFINES += CURRENCY_ENABLE_MQS_ADDRESS
#	DEFINES += CURRENCY_ENABLE_TOR_ADDRESS
	DEFINES += CURRENCY_MQS_VERSION=\{1,0\}
	DEFINES += CURRENCY_NAME=\"Epic\\x20\\x0043\\x61sh\"
	DEFINES += CURRENCY_ABBREVIATION=\"EPIC\"
	
	# Check if target is the Stax
	ifeq ($(TARGET_NAME),TARGET_STAX)
	
		# Defines
		DEFINES += CURRENCY_ICON_DETAILS=C_icon_epic_cash_big
		DEFINES += CURRENCY_ICON_BITMAP=C_icon_epic_cash_big_bitmap
	
	# Otherwise
	else
	
		# Defines
		DEFINES += CURRENCY_ICON_DETAILS=C_icon_epic_cash
		DEFINES += CURRENCY_ICON_COLORS=C_icon_mimblewimble_coin_colors
		DEFINES += CURRENCY_ICON_BITMAP=C_icon_epic_cash_bitmap
	endif

	# Icon
	ICON = Epic Cash

# Otherwise check if currency is MimbleWimble Coin floonet
else ifeq ($(CURRENCY),mimblewimble_coin_floonet)

	# Application name
	APPNAME = "MimbleWimble Coin Floonet"

	# 44'/1' path on secp256k1 curve
	APP_LOAD_PARAMS += --curve secp256k1 --path "44'/1'"

	# Check if target is the Nano X
	ifeq ($(TARGET_NAME),TARGET_NANOX)

		# APPLICATION_FLAG_BOLOS_SETTINGS application flags for Bluetooth
		APP_LOAD_PARAMS += --appFlags 0x200

	# Otherwise check if target is the Stax
	else ifeq ($(TARGET_NAME),TARGET_STAX)

		# APPLICATION_FLAG_BOLOS_SETTINGS application flags for Bluetooth
		APP_LOAD_PARAMS += --appFlags 0x200

	# Otherwise
	else

		# No application flags
		APP_LOAD_PARAMS += --appFlags 0x000
	endif

	# Defines
	DEFINES += CURRENCY_ID=1
	DEFINES += CURRENCY_BIP44_COIN_TYPE=1
	DEFINES += CURRENCY_FRACTIONAL_DIGITS=9
	DEFINES += CURRENCY_ENABLE_MQS_ADDRESS
	DEFINES += CURRENCY_ENABLE_TOR_ADDRESS
	DEFINES += CURRENCY_MQS_VERSION=\{1,121\}
	DEFINES += CURRENCY_NAME=\"MimbleWimble\\x20\\x43oin\\x20\\x46loonet\"
	DEFINES += CURRENCY_ABBREVIATION=\"Floonet\\x20MWC\"
	
	# Check if target is the Stax
	ifeq ($(TARGET_NAME),TARGET_STAX)
	
		# Defines
		DEFINES += CURRENCY_ICON_DETAILS=C_icon_mimblewimble_coin_big
		DEFINES += CURRENCY_ICON_BITMAP=C_icon_mimblewimble_coin_big_bitmap
	
	# Otherwise
	else
	
		# Defines
		DEFINES += CURRENCY_ICON_DETAILS=C_icon_mimblewimble_coin
		DEFINES += CURRENCY_ICON_COLORS=C_icon_mimblewimble_coin_colors
		DEFINES += CURRENCY_ICON_BITMAP=C_icon_mimblewimble_coin_bitmap
	endif

	# Defines library
	DEFINES += USE_LIB_MIMBLEWIMBLE_COIN
	APP_LOAD_PARAMS += --dep "MimbleWimble Coin":$(APPVERSION)

	# Icon
	ICON = MimbleWimble Coin

	# Emulator flags
	EMULATOR_FLAGS += --library "MimbleWimble Coin":"mimblewimble coin.elf"

# Otherwise
else

# Display error
$(error Unsupported CURRENCY - use epic_cash, or mimblewimble_coin_floonet)
endif

# Define total number of supported currencies
DEFINES += TOTAL_NUMBER_OF_SUPPORTED_CURRENCIES=2

# Check if target is the Nano S
ifeq ($(TARGET_NAME),TARGET_NANOS)

	# Icon name
	ICONNAME="icons/$(ICON)/nanos_app.gif"

# Otherwise check if target is the Nano X
else ifeq ($(TARGET_NAME),TARGET_NANOX)

	# Icon name
	ICONNAME="icons/$(ICON)/nanox_app.gif"

# Otherwise check if target is the Nano S Plus
else ifeq ($(TARGET_NAME),TARGET_NANOS2)

	# Icon name
	ICONNAME="icons/$(ICON)/nanosplus_app.gif"

# Otherwise check if target is the Stax
else ifeq ($(TARGET_NAME),TARGET_STAX)

	# Icon name
	ICONNAME="icons/$(ICON)/stax_app.gif"
endif

# Make command
all: default

# Defines
DEFINES += APPNAME=\"$(APPNAME)\"
DEFINES += APPVERSION=\"$(APPVERSION)\"
DEFINES += MAJOR_VERSION=$(APPVERSION_M) MINOR_VERSION=$(APPVERSION_N) PATCH_VERSION=$(APPVERSION_P)
DEFINES += OS_IO_SEPROXYHAL
DEFINES += HAVE_SPRINTF
DEFINES += HAVE_IO_USB HAVE_L4_USBLIB IO_USB_MAX_ENDPOINTS=6 IO_HID_EP_LENGTH=64 HAVE_USB_APDU
DEFINES += USB_SEGMENT_SIZE=64
DEFINES += BLE_SEGMENT_SIZE=32
DEFINES += HAVE_WEBUSB WEBUSB_URL_SIZE_B=0 WEBUSB_URL=""
DEFINES += UNUSED\(x\)=\(void\)x

# Check if target is the Nano X
ifeq ($(TARGET_NAME),TARGET_NANOX)

	# Target specific defines
	DEFINES += HAVE_BLE BLE_COMMAND_TIMEOUT_MS=2000 HAVE_BLE_APDU

# Otherwise check if target is the Stax
else ifeq ($(TARGET_NAME),TARGET_STAX)

	# Target specific defines
	DEFINES += HAVE_BLE BLE_COMMAND_TIMEOUT_MS=2000 HAVE_BLE_APDU
endif

# Check if target is the Nano S
ifeq ($(TARGET_NAME),TARGET_NANOS)

	# Target specific defines
	DEFINES += IO_SEPROXYHAL_BUFFER_SIZE_B=128

# Otherwise
else

	# Target specific defines
	DEFINES += IO_SEPROXYHAL_BUFFER_SIZE_B=300
endif

# Check if target is the Stax
ifeq ($(TARGET_NAME),TARGET_STAX)

	# Target specific defines
	DEFINES += NBGL_QRCODE

# Otherwise
else

	# Target specific defines
	DEFINES += HAVE_BAGL HAVE_UX_FLOW
	
	# Check if target isn't the nano S
	ifneq ($(TARGET_NAME),TARGET_NANOS)
	
		# Target specific defines
		DEFINES += HAVE_GLO096
		DEFINES += BAGL_WIDTH=128 BAGL_HEIGHT=64
		DEFINES += HAVE_BAGL_ELLIPSIS
		DEFINES += HAVE_BAGL_FONT_OPEN_SANS_REGULAR_11PX
		DEFINES += HAVE_BAGL_FONT_OPEN_SANS_EXTRABOLD_11PX
		DEFINES += HAVE_BAGL_FONT_OPEN_SANS_LIGHT_16PX
	endif
endif

# Set debug
DEBUG = 0

# Check if debug is set
ifneq ($(DEBUG),0)

	# Define printf
	DEFINES += HAVE_PRINTF

	# Check if target is the Nano S
	ifeq ($(TARGET_NAME),TARGET_NANOS)

		# Define target specific printf
		DEFINES += PRINTF=screen_printf

	# Otherwise
	else

		# Define target specific printf
		DEFINES += PRINTF=mcu_usb_printf
	endif

# Otherwise
else

	# Define printf as nothing
	DEFINES += PRINTF\(...\)=
endif

# Define stack canary
DEFINES += HAVE_BOLOS_APP_STACK_CANARY

# Check if BOLOS environment is defined
ifneq ($(BOLOS_ENV),)

# Display message
$(info BOLOS_ENV=$(BOLOS_ENV))

	# Set compiler paths
	CLANGPATH := /usr/bin/
	GCCPATH := $(BOLOS_ENV)/gcc-arm-none-eabi/bin/

# Otherwise
else

# Display message
$(info BOLOS_ENV is not set: falling back to CLANGPATH and GCCPATH)
endif

# Check if Clang path isn't defined
ifeq ($(CLANGPATH),)

# Display message
$(info CLANGPATH is not set: clang will be used from PATH)
endif

# Check if GCC path isn't degined
ifeq ($(GCCPATH),)

# Display message
$(info GCCPATH is not set: arm-none-eabi-* will be used from PATH)
endif

# Compiler settings
CC := $(CLANGPATH)clang
CFLAGS += -Oz -Wall -Wextra
AS := $(GCCPATH)arm-none-eabi-gcc
LD := $(GCCPATH)arm-none-eabi-gcc
LDFLAGS +=
LDLIBS += -lgcc -lc

# Include BOLOS SDK Makefile glyphs
include $(BOLOS_SDK)/Makefile.glyphs

# Compiler settings
APP_SOURCE_PATH += src
SDK_SOURCE_PATH += lib_stusb lib_stusb_impl

# Check if target isn't the Stax
ifneq ($(TARGET_NAME),TARGET_STAX)

	# Target specific compiler settings
	SDK_SOURCE_PATH += lib_ux
endif

# Check if target is the Nano X
ifeq ($(TARGET_NAME),TARGET_NANOX)

	# Target specific compiler settings
	SDK_SOURCE_PATH += lib_blewbxx lib_blewbxx_impl

# Otherwise check if target is the Stax
else ifeq ($(TARGET_NAME),TARGET_STAX)

	# Target specific compiler settings
	SDK_SOURCE_PATH += lib_blewbxx lib_blewbxx_impl
endif

# Load command
load: all

	# Load application on device
	python3 -m ledgerblue.loadApp $(APP_LOAD_PARAMS)

# Delete command
delete:

	# Delete application from device
	python3 -m ledgerblue.deleteApp $(COMMON_DELETE_PARAMS)

# Run command
run: all

	# Run application in emulator
	SPECULOS_APPNAME=$(APPNAME):$(APPVERSION) $(BOLOS_EMU)/speculos.py bin/app.elf $(EMULATOR_FLAGS)

# Functional tests
functional_tests: all

	# Run functional tests
	node test/functional_tests/main.js $(CURRENCY)

# Include BOLOS SDK Makefile rules
include $(BOLOS_SDK)/Makefile.rules

# Source files
dep/%.d: %.c Makefile

# List variants
listvariants:
	@echo VARIANTS CURRENCY epic_cash mimblewimble_coin_floonet
