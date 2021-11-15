// Header files
#include <alloca.h>
#include <string.h>
#include "blake2b.h"
#include "common.h"
#include "crypto.h"
#include "currency_information.h"
#include "finish_transaction.h"
#include "menus.h"
#include "mqs.h"
#include "transaction.h"
#include "tor.h"


// Constants

// Kernel features
enum KernelFeatures {

	// Plain features
	PLAIN_FEATURES,
	
	// Coinbase features
	COINBASE_FEATURES,
	
	// Height locked features
	HEIGHT_LOCKED_FEATURES,
	
	// No recent duplicate features
	NO_RECENT_DUPLICATE_FEATURES
};

// Address type
enum AddressType {

	// Tor address type
	TOR_ADDRESS_TYPE,
	
	// MQS address type
	MQS_ADDRESS_TYPE,
	
	// Ed25519 address type
	ED25519_ADDRESS_TYPE
};


// Supporting function implementation

// Process finish transaction request
void processFinishTransactionRequest(unsigned short *responseLength, __attribute__((unused)) unsigned char *responseFlags) {

	// Get request's first parameter
	const uint8_t firstParameter = G_io_apdu_buffer[APDU_OFF_P1];
	
	// Get request's second parameter
	const uint8_t secondParameter = G_io_apdu_buffer[APDU_OFF_P2];
	
	// Get request's data length
	const size_t dataLength = G_io_apdu_buffer[APDU_OFF_LC];
	
	// Get request's data
	uint8_t *data = &G_io_apdu_buffer[APDU_OFF_DATA];
	
	// Check if parameters or data are invalid
	if(secondParameter || dataLength < NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + sizeof(uint8_t)) {
	
		// Throw invalid parameters error
		THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Get address type from first parameter
	const enum AddressType addressType = firstParameter;
	
	// Check address type
	switch(addressType) {
	
		// MQS address type
		case MQS_ADDRESS_TYPE:
		
			// Check currency doesn't allow MQS addresses
			if(!currencyInformation.mqsAddressPaymentProofAllowed) {
			
				// Throw invalid parameters error
				THROW(INVALID_PARAMETERS_ERROR);
			}
			
			// Break
			break;
		
		// Tor address type
		case TOR_ADDRESS_TYPE:
		
			// Check currency doesn't allow Tor addresses
			if(!currencyInformation.torAddressPaymentProofAllowed) {
			
				// Throw invalid parameters error
				THROW(INVALID_PARAMETERS_ERROR);
			}
		
			// Break
			break;
		
		// Ed25519 address type
		case ED25519_ADDRESS_TYPE:
		
			// Check currency doesn't allow Ed25519 addresses
			if(!currencyInformation.ed25519AddressPaymentProofAllowed) {
			
				// Throw invalid parameters error
				THROW(INVALID_PARAMETERS_ERROR);
			}
		
			// Break
			break;
		
		// Default
		default:
		
			// Throw invalid parameters error
			THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Get secret nonce from data
	const uint8_t *secretNonce = data;
	
	// Check if secret nonce is invalid
	if(cx_math_is_zero(secretNonce, NONCE_SIZE)) {
	
		// Throw invalid parameters error
		THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Get public nonce from data
	const uint8_t *publicNonce = &data[NONCE_SIZE];
	
	// Check if public nonce is invalid
	if(!isValidSecp256k1PublicKey(publicNonce, COMPRESSED_PUBLIC_KEY_SIZE)) {
	
		// Throw invalid parameters error
		THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Get public key from data
	const uint8_t *publicKey = &data[NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE];
	
	// Check if public key is invalid
	if(!isValidSecp256k1PublicKey(publicKey, COMPRESSED_PUBLIC_KEY_SIZE)) {
	
		// Throw invalid parameters error
		THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Get kernel features from data
	const enum KernelFeatures kernelFeatures = data[NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE];
	
	// Check kernel features
	size_t kernelFeaturesLength;
	switch(kernelFeatures) {
	
		// Plain and coinbase features
		case PLAIN_FEATURES:
		case COINBASE_FEATURES:
		
			// Set kernel features length
			kernelFeaturesLength = sizeof(uint8_t);
		
			// Break
			break;
		
		// Height locked and no recent duplicate features
		case HEIGHT_LOCKED_FEATURES:
		case NO_RECENT_DUPLICATE_FEATURES:
		
			// Set kernel features length
			kernelFeaturesLength = sizeof(uint8_t) + sizeof(uint64_t);
			
			// Break
			break;
		
		// Default
		default:
		
			// Throw invalid parameters error
			THROW(INVALID_PARAMETERS_ERROR);
	};
	
	// Check if transaction hasn't been started
	if(!transaction.started) {
	
		// Throw invalid state error
		THROW(INVALID_STATE_ERROR);
	}
	
	// Check if transaction has remaining output or input
	if(transaction.remainingOutput || transaction.remainingInput) {
	
		// Throw invalid state error
		THROW(INVALID_STATE_ERROR);
	}
	
	// Check if transaction is sending
	if(transaction.send) {
	
		// Check if data is invalid
		if(dataLength < NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + kernelFeaturesLength) {
		
			// Throw invalid parameters error
			THROW(INVALID_PARAMETERS_ERROR);
		}
		
		// Check if a payment proof information is provided
		if(dataLength != NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + kernelFeaturesLength) {
		
			// Check if data is invalid
			if(dataLength <= NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + kernelFeaturesLength + COMMITMENT_SIZE) {
			
				// Throw invalid parameters error
				THROW(INVALID_PARAMETERS_ERROR);
			}
			
			// Get commitment from data
			const uint8_t *commitment = &data[NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + kernelFeaturesLength];
			
			// Check if commitment is invalid
			if(!isValidCommitment(commitment)) {
			
				// Throw invalid parameters error
				THROW(INVALID_PARAMETERS_ERROR);
			}
			
			// Get signature from data
			uint8_t *signature = &data[NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + kernelFeaturesLength + COMMITMENT_SIZE];
			
			// Get signature length
			const size_t signatureLength = dataLength - (NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + kernelFeaturesLength + COMMITMENT_SIZE);
			
			// Check address type
			size_t addressLength;
			uint8_t *address;
			switch(addressType) {
			
				// MQS address type
				case MQS_ADDRESS_TYPE:
				
					// Set address length
					addressLength = MQS_ADDRESS_SIZE;
					
					// Allocate memory for the address
					address = alloca(addressLength);
					
					// Get MQS address
					getMqsAddress(address, transaction.account);
					
					// Break
					break;
				
				// Tor address type
				case TOR_ADDRESS_TYPE:
				
					// Set address length
					addressLength = TOR_ADDRESS_SIZE;
					
					// Allocate memory for the address
					address = alloca(addressLength);
					
					// Get Tor address
					getTorAddress(address, transaction.account);
				
					// Break
					break;
				
				// Ed25519 address type
				case ED25519_ADDRESS_TYPE:
				
					// Set address length
					addressLength = ED25519_PUBLIC_KEY_SIZE;
					
					// Allocate memory for the address
					address = alloca(addressLength);
					
					// Get Ed25519 public key
					getEd25519PublicKey(address, transaction.account);
				
					// Break
					break;
			}
			
			// Get payment proof message
			uint8_t paymentProofMessage[getPaymentProofMessageLength(transaction.send, addressLength)];
			getPaymentProofMessage(paymentProofMessage, transaction.send, commitment, address, addressLength);
			
			// Check if verifying payment proof failed
			if(!verifyPaymentProofMessage(paymentProofMessage, sizeof(paymentProofMessage), transaction.receiverAddress, transaction.receiverAddressLength, signature, signatureLength)) {
			
				// Throw invalid parameters error
				THROW(INVALID_PARAMETERS_ERROR);
			}
			
			// Check receiver address length
			switch(transaction.receiverAddressLength) {
			
				// MQS address size or Tor address size
				case MQS_ADDRESS_SIZE:
				case TOR_ADDRESS_SIZE:
				
					// Copy receiver address into the receiver line buffer
					explicit_bzero(receiverLineBuffer, sizeof(receiverLineBuffer));
					memcpy(receiverLineBuffer, transaction.receiverAddress, transaction.receiverAddressLength);
				
					// Break
					break;
			
				// Ed25519 address size
				case ED25519_PUBLIC_KEY_SIZE:
				
					// Copy receiver address into the receiver line buffer
					explicit_bzero(receiverLineBuffer, sizeof(receiverLineBuffer));
					toHexString(receiverLineBuffer, transaction.receiverAddress, transaction.receiverAddressLength);
				
					// Break
					break;
			}
		}
		
		// Otherwise
		else {
			
			// Clear receiver line buffer
			explicit_bzero(receiverLineBuffer, sizeof(receiverLineBuffer));
		}
		
		// Check if transaction offset wasn't applied
		if(!transaction.offsetApplied) {
		
			// Throw invalid state error
			THROW(INVALID_STATE_ERROR);
		}
	
		// Copy transaction's input into the amount line buffer
		explicit_bzero(amountLineBuffer, sizeof(amountLineBuffer));
		toString(amountLineBuffer, transaction.send, currencyInformation.fractionalDigits);
		
		strcat(amountLineBuffer, " ");
		strcat(amountLineBuffer, currencyInformation.abbreviation);
		
		// Copy transaction's fee into the fee line buffer
		explicit_bzero(feeLineBuffer, sizeof(feeLineBuffer));
		toString(feeLineBuffer, transaction.fee, currencyInformation.fractionalDigits);
		
		strcat(feeLineBuffer, " ");
		strcat(feeLineBuffer, currencyInformation.abbreviation);

		// Show finalize transaction menu
		showMenu(FINALIZE_TRANSACTION_MENU);
		
		// Set response flags to send response later
		*responseFlags |= IO_ASYNCH_REPLY;
	}
	
	// Otherwise
	else {
	
		// Check if data is invalid
		if(dataLength != NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + kernelFeaturesLength) {
		
			// Throw invalid parameters error
			THROW(INVALID_PARAMETERS_ERROR);
		}
	
		// Process finish transaction user interaction
		processFinishTransactionUserInteraction(responseLength);
	}
}

// Process finish transaction user interaction
void processFinishTransactionUserInteraction(unsigned short *responseLength) {

	// Get request's data
	uint8_t *data = &G_io_apdu_buffer[APDU_OFF_DATA];
	
	// Get secret nonce from data
	uint8_t *secretNonce = data;
	
	// Get public nonce from data
	const uint8_t *publicNonce = &data[NONCE_SIZE];
	
	// Get public key from data
	const uint8_t *publicKey = &data[NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE];
	
	// Get kernel features from data
	const enum KernelFeatures kernelFeatures = data[NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE];
	
	// Initialize kernel data
	uint8_t *kernelData;
	
	// Initialize kernel data length
	size_t kernelDataLength;
	
	// Check kernel features
	switch(kernelFeatures) {
	
		// Plain features
		case PLAIN_FEATURES:
		
			// Set kernel data length
			kernelDataLength = sizeof(uint8_t) + sizeof(uint64_t);
			
			// Allocate memory for kernel data
			kernelData = alloca(kernelDataLength);
			
			// Set kernel features in the kernel data
			kernelData[0] = kernelFeatures;
			
			// Convert fee to big endian
			swapEndianness((uint8_t *)&transaction.fee, sizeof(transaction.fee));
			
			// Append fee to the kernel data
			memcpy(&kernelData[sizeof(kernelData[0])], &transaction.fee, sizeof(transaction.fee));
		
			// Break
			break;
		
		// Coinbase features
		case COINBASE_FEATURES:
		
			// Set kernel data length
			kernelDataLength = sizeof(uint8_t);
			
			// Allocate memory for kernel data
			kernelData = alloca(kernelDataLength);
			
			// Set kernel features in the kernel data
			kernelData[0] = kernelFeatures;
		
			// Break
			break;
		
		// Height locked features
		case HEIGHT_LOCKED_FEATURES:
		
			// Set kernel data length
			kernelDataLength = sizeof(uint8_t) + sizeof(uint64_t) + sizeof(uint64_t);
			
			// Allocate memory for kernel data
			kernelData = alloca(kernelDataLength);
			
			// Set kernel features in the kernel data
			kernelData[0] = kernelFeatures;
			
			// Convert fee to big endian
			swapEndianness((uint8_t *)&transaction.fee, sizeof(transaction.fee));
			
			// Append fee to the kernel data
			memcpy(&kernelData[sizeof(kernelData[0])], &transaction.fee, sizeof(transaction.fee));
			
			{
				// Get lock height from data
				uint64_t *lockHeight = (uint64_t *)&data[NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + sizeof(uint8_t)];
				
				// Convert lock height to big endian
				swapEndianness((uint8_t *)lockHeight, sizeof(*lockHeight));
				
				// Append lock height to the kernel data
				memcpy(&kernelData[sizeof(kernelData[0]) + sizeof(transaction.fee)], lockHeight, sizeof(*lockHeight));
			}
			
			// Break
			break;
		
		// No recent duplicate features
		case NO_RECENT_DUPLICATE_FEATURES:
		
			// Set kernel data length
			kernelDataLength = sizeof(uint8_t) + sizeof(uint64_t) + sizeof(uint64_t);
			
			// Allocate memory for kernel data
			kernelData = alloca(kernelDataLength);
			
			// Set kernel features in the kernel data
			kernelData[0] = kernelFeatures;
			
			// Convert fee to big endian
			swapEndianness((uint8_t *)&transaction.fee, sizeof(transaction.fee));
			
			// Append fee to the kernel data
			memcpy(&kernelData[sizeof(kernelData[0])], &transaction.fee, sizeof(transaction.fee));
			
			{
				
				// Get relative height from data
				uint64_t *relativeHeight = (uint64_t *)&data[NONCE_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + COMPRESSED_PUBLIC_KEY_SIZE + sizeof(uint8_t)];
				
				// Convert relative height to big endian
				swapEndianness((uint8_t *)relativeHeight, sizeof(*relativeHeight));
				
				// Append relative height to the kernel data
				memcpy(&kernelData[sizeof(kernelData[0]) + sizeof(transaction.fee)], relativeHeight, sizeof(*relativeHeight));
			}
			
			// Break
			break;
	};

	// Get message from the kernel data
	uint8_t message[SINGLE_SIGNER_MESSAGE_SIZE];
	getBlake2b(message, sizeof(message), kernelData, kernelDataLength, NULL, 0);
	
	// Initialize signature
	uint8_t signature[SINGLE_SIGNER_COMPACT_SIGNATURE_SIZE];

	// Create single-signer signature from the message, private key, secret nonce, public nonce if used, and public key
	createSingleSignerSignature(signature, message, (uint8_t *)transaction.blindingFactor, secretNonce, publicNonce, publicKey);
	
	// Check if response with the signature will overflow
	if(willResponseOverflow(*responseLength, sizeof(signature))) {
	
		// Throw length error
		THROW(ERR_APD_LEN);
	}
	
	// Append signature to response
	memcpy(&G_io_apdu_buffer[*responseLength], (uint8_t *)signature, sizeof(signature));
	
	*responseLength += sizeof(signature);
	
	// Reset the transaction
	resetTransaction();

	// Throw success
	THROW(SWO_SUCCESS);
}
