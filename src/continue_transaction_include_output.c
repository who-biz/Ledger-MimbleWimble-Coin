// Header files
#include <string.h>
#include "common.h"
#include "continue_transaction_include_output.h"
#include "crypto.h"
#include "transaction.h"


// Supporting function implementation

// Process continue transaction include output request
void processContinueTransactionIncludeOutputRequest(__attribute__((unused)) unsigned short *responseLength, __attribute__((unused)) unsigned char *responseFlags) {

	// Get request's first parameter
	const uint8_t firstParameter = G_io_apdu_buffer[APDU_OFF_P1];
	
	// Get request's second parameter
	const uint8_t secondParameter = G_io_apdu_buffer[APDU_OFF_P2];
	
	// Get request's data length
	const size_t dataLength = G_io_apdu_buffer[APDU_OFF_LC];
	
	// Get request's data
	uint8_t *data = &G_io_apdu_buffer[APDU_OFF_DATA];

	// Check if parameters or data are invalid
	if(firstParameter || secondParameter || dataLength != IDENTIFIER_SIZE + sizeof(uint64_t) + sizeof(uint8_t)) {
	
		// Throw invalid parameters error
		THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Get identifer depth from data
	const uint8_t identifierDepth = data[0];
	
	// Check if identifier depth is invalid
	if(identifierDepth > IDENTIFIER_MAXIMUM_DEPTH) {
	
		// Throw invalid parameters error
		THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Get identifier path from data
	uint32_t identifierPath[identifierDepth];
	memcpy(identifierPath, &data[sizeof(identifierDepth)], sizeof(identifierPath));
	
	// Go through all parts in the identifier path
	for(size_t i = 0; i < IDENTIFIER_MAXIMUM_DEPTH; ++i) {
	
		// Convert part from big endian to little endian
		identifierPath[i] = os_swap_u32(identifierPath[i]);
	}
	
	// Get value from data
	uint64_t value;
	memcpy(&value, &data[IDENTIFIER_SIZE], sizeof(value));
	
	// Check if value is invalid
	if(!value) {
	
		// Throw invalid parameters error
		THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Get switch type from data
	const enum SwitchType switchType = data[IDENTIFIER_SIZE + sizeof(uint64_t)];
	
	// Check if switch type is invalid
	if(switchType > REGULAR_SWITCH_TYPE) {
	
		// Throw invalid parameters error
		THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Check if transaction hasn't been started
	if(!transaction.started) {
	
		// Throw invalid state error
		THROW(INVALID_STATE_ERROR);
	}
	
	// Check if transaction has no more remaining output
	if(!transaction.remainingOutput) {
	
		// Throw invalid state error
		THROW(INVALID_STATE_ERROR);
	}
	
	// Check if value is too big for the transaction's remaining output
	if(value > transaction.remainingOutput) {
	
		// Throw invalid parameters error
		THROW(INVALID_PARAMETERS_ERROR);
	}
	
	// Initialize blinding factor
	volatile uint8_t blindingFactor[BLINDING_FACTOR_SIZE];
	
	// Begin try
	BEGIN_TRY {
	
		// Try
		TRY {
	
			// Derive blinding factor
			deriveBlindingFactor(blindingFactor, transaction.account, value, identifierPath, identifierDepth, switchType);
			
			// Update transaction's blinding factor with the positive blinding factor
			updateBlindingFactorSum((uint8_t *)transaction.blindingFactor, (uint8_t *)blindingFactor, true);
		}
		
		// Finally
		FINALLY {
		
			// Clear the blinding factor
			explicit_bzero((uint8_t *)blindingFactor, sizeof(blindingFactor));
		}
	}
	
	// End try
	END_TRY;
	
	// Remove value from the transaction's remaining output
	transaction.remainingOutput -= value;
	
	// Throw success
	THROW(SWO_SUCCESS);
}
