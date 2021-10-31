// Header guard
#ifndef CURRENCY_INFORMATION_H
#define CURRENCY_INFORMATION_H


// Header files
#include <ux.h>
#include <stdbool.h>


// Definitions

// Currency MQS version size
#define CURRENCY_MQS_VERSION_SIZE sizeof(uint16_t)

// Currency information name size
#define CURRENCY_INFORMATION_NAME_SIZE sizeof("MimbleWimble Coin Floonet")

// Currency information abbreviation size
#define CURRENCY_INFORMATION_ABBREVIATION_SIZE sizeof("GRIN")

// Currency information version size
#define CURRENCY_INFORMATION_VERSION_SIZE sizeof("0.0.1")

// Currency information icon colors size
#define CURRENCY_INFORMATION_ICON_COLORS_SIZE 2

// Currency information icon bitmap size
#define CURRENCY_INFORMATION_ICON_BITMAP_SIZE 32

// Currency information
struct CurrencyInformation {

	// BIP44 coin type
	uint32_t bip44CoinType;
	
	// Fractional digits
	uint8_t fractionalDigits;
	
	// MQS address payment proof allowed
	bool mqsAddressPaymentProofAllowed;
	
	// Tor address payment proof allowed
	bool torAddressPaymentProofAllowed;
	
	// Ed25519 address payment proof allowed
	bool ed25519AddressPaymentProofAllowed;
	
	// MQS version
	uint8_t mqsVersion[CURRENCY_MQS_VERSION_SIZE];
	
	// Name
	char name[CURRENCY_INFORMATION_NAME_SIZE];
	
	// Abbreviation
	char abbreviation[CURRENCY_INFORMATION_ABBREVIATION_SIZE];
	
	// Version
	char version[CURRENCY_INFORMATION_VERSION_SIZE];
	
	// Icon colors
	unsigned int iconColors[CURRENCY_INFORMATION_ICON_COLORS_SIZE];
	
	// Icon bitmap
	unsigned char iconBitmap[CURRENCY_INFORMATION_ICON_BITMAP_SIZE];
	
	// Icon details
	bagl_icon_details_t iconDetails;
};


// Global variables

// Currency information
extern struct CurrencyInformation currencyInformation;


// Function prototypes

// Get currency information
void getCurrencyInformation(struct CurrencyInformation *currencyInformation);


#endif
