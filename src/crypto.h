// Header guard
#ifndef CRYPTO_H
#define CRYPTO_H


// Header files
#include "device.h"


// Definitions

// Blinding factor size
#define BLINDING_FACTOR_SIZE 32

// Ed25519 public key size
#define ED25519_PUBLIC_KEY_SIZE 32

// Compressed public key size
#define COMPRESSED_PUBLIC_KEY_SIZE 33

// Public key prefix size
#define PUBLIC_KEY_PREFIX_SIZE 1

// Proof message size
#define PROOF_MESSAGE_SIZE 20

// Uncompressed public key size
#define UNCOMPRESSED_PUBLIC_KEY_SIZE 65

// Public key component size
#define PUBLIC_KEY_COMPONENT_SIZE 32


// Constants

// Nonce size
extern const size_t NONCE_SIZE;

// Commitment size
extern const size_t COMMITMENT_SIZE;

// Identifier maximum depth
extern const size_t IDENTIFIER_MAXIMUM_DEPTH;

// Identifier size
extern const size_t IDENTIFIER_SIZE;

// Single-signer compact signature size
extern const size_t SINGLE_SIGNER_COMPACT_SIGNATURE_SIZE;

// Single-signer message size
extern const size_t SINGLE_SIGNER_MESSAGE_SIZE;

// Maximum DER signature size
extern const size_t MAXIMUM_DER_SIGNATURE_SIZE;

// Keep private
extern const int KEEP_PRIVATE_KEY;

// Even compressed public key prefix
extern const uint8_t EVEN_COMPRESSED_PUBLIC_KEY_PREFIX;

// Odd compressed public key prefix
extern const uint8_t ODD_COMPRESSED_PUBLIC_KEY_PREFIX;

// Ed25519 compressed public key prefix
extern const uint8_t ED25519_COMPRESSED_PUBLIC_KEY_PREFIX;

// Ed25519 signature size
extern const size_t ED25519_SIGNATURE_SIZE;

// Maximum account
extern const uint32_t MAXIMUM_ACCOUNT;

// X25519 public key size
extern const size_t X25519_PUBLIC_KEY_SIZE;

// X25519 compressed public key prefix
extern const uint8_t X25519_COMPRESSED_PUBLIC_KEY_PREFIX;

// Switch type
enum SwitchType {

	// No switch type
	NO_SWITCH_TYPE,
	
	// Regular switch type
	REGULAR_SWITCH_TYPE
};


// Function prototypes

// Get private key and chain code
void getPrivateKeyAndChainCode(volatile cx_ecfp_private_key_t *privateKey, volatile uint8_t *chainCode, uint32_t account);

// Get public key from private key
void getPublicKeyFromPrivateKey(volatile uint8_t *publicKey, const cx_ecfp_private_key_t *privateKey);

// Derive child key
void deriveChildKey(volatile cx_ecfp_private_key_t *privateKey, volatile uint8_t *chainCode, uint32_t account, const uint32_t *path, size_t pathLength, bool useProvidedPrivateKeyAndChainCode);

// Derive blinding factor
void deriveBlindingFactor(volatile uint8_t *blindingFactor, uint32_t account, uint64_t value, const uint32_t *path, size_t pathLength, enum SwitchType switchType);

// Commit value
void commitValue(volatile uint8_t *commitment, uint64_t value, const uint8_t *blindingFactor, bool compress);

// Get rewind nonce
void getRewindNonce(volatile uint8_t *rewindNonce, uint32_t account, const uint8_t *commitment);

// Get private nonce
void getPrivateNonce(volatile uint8_t *privateNonce, uint32_t account, const uint8_t *commitment);

// Get address private key
void getAddressPrivateKey(volatile cx_ecfp_private_key_t *addressPrivateKey, uint32_t account, uint32_t index, cx_curve_t curve);

// Update blinding factor sum
void updateBlindingFactorSum(uint8_t *blindingFactorSum, uint8_t *blindingFactor, bool blindingFactorIsPositive);

// Create single-signer signature
void createSingleSignerSignature(uint8_t *signature, const uint8_t *message, const uint8_t *blindingFactor, uint8_t *secretNonce, const uint8_t *publicNonce, const uint8_t *publicKey);

// Get encrypted data length
size_t getEncryptedDataLength(size_t dataLength);

// Encrypt data
void encryptData(volatile uint8_t *result, const uint8_t *data, size_t dataLength, const uint8_t *key, size_t keyLength);

// Get X25519 private key from Ed25519 private key
void getX25519PrivateKeyFromEd25519PrivateKey(volatile cx_ecfp_private_key_t *x25519PrivateKey, const cx_ecfp_private_key_t *ed25519PrivateKey);

// Get X25519 public key from Ed25519 public key
void getX25519PublicKeyFromEd25519PublicKey(uint8_t *x25519PublicKey, const uint8_t *ed25519PublicKey);

// Get payment proof message length
size_t getPaymentProofMessageLength(uint64_t value, size_t senderAddressLength);

// Get payment proof message
void getPaymentProofMessage(uint8_t *message, uint64_t value, const uint8_t *kernelCommitment, const char *senderAddress, size_t senderAddressLength);

// Verify payment proof message
bool verifyPaymentProofMessage(const uint8_t *message, size_t messageLength, const char *receiverAddress, size_t receiverAddressLength, uint8_t *signature, size_t signatureLength);

// Is valid commitment
bool isValidCommitment(const uint8_t *commitment);

// Is valid Ed25519 public key
bool isValidEd25519PublicKey(const uint8_t *publicKey, size_t length);

// Is valid secp256k1 private key
bool isValidSecp256k1PrivateKey(const uint8_t *privateKey, size_t length);

// Is valid secp256k1 public key
bool isValidSecp256k1PublicKey(const uint8_t *publicKey, size_t length);

// Uncompress secp256k1 public key
void uncompressSecp256k1PublicKey(uint8_t *publicKey);

// Get Ed25519 public key
void getEd25519PublicKey(uint8_t *ed25519PublicKey, uint32_t account, uint32_t index);

// Calculate bulletproof components
void calculateBulletproofComponents(uint8_t *tauX, uint8_t *tOne, uint8_t *tTwo, uint64_t value, const uint8_t *blindingFactor, const uint8_t *commitment, const uint8_t *rewindNonce, const uint8_t *privateNonce, const uint8_t *proofMessage);


#endif
