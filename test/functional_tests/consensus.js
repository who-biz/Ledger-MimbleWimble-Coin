// Use strict
"use strict";


// Requires
const Common = require("./common.js");


// Classes

// Consensus class
class Consensus {

	// Public
	
		// Get wallet type
		static getWalletType() {
		
			// Get URL paramaters
			var urlParamaters = Common.getUrlParameters();
		
			// Check if override wallet type exists
			if(Consensus.OVERRIDE_WALLET_TYPE_URL_PARAMETER_NAME in urlParamaters === true) {
			
				// Check override wallet type
				switch(urlParamaters[Consensus.OVERRIDE_WALLET_TYPE_URL_PARAMETER_NAME]) {
				
					// MWC wallet
					case Consensus.WALLET_MWC_TEXT_VALUE:
					
						// Return MWC wallet type
						return Consensus.MWC_WALLET_TYPE;
					
					// GRIN wallet
					case Consensus.WALLET_GRIN_TEXT_VALUE:
					
						// Return GRIN wallet type
						return Consensus.GRIN_WALLET_TYPE;

					// EPIC wallet
					case Consensus.WALLET_EPIC_TEXT_VALUE:
					
						// Return EPIC wallet type
						return Consensus.EPIC_WALLET_TYPE;
				}
			}
			
			// Return default wallet type
			return Consensus.DEFAULT_WALLET_TYPE;
		}
		
		// Get network type
		static getNetworkType() {
		
			// Get URL paramaters
			var urlParamaters = Common.getUrlParameters();
		
			// Check if override network type exists
			if(Consensus.OVERRIDE_NETWORK_TYPE_URL_PARAMETER_NAME in urlParamaters === true) {
			
				// Check override network type
				switch(urlParamaters[Consensus.OVERRIDE_NETWORK_TYPE_URL_PARAMETER_NAME]) {
				
					// Mainnet network
					case Consensus.NETWORK_MAINNET_TEXT_VALUE:
					
						// Return mainnet network type
						return Consensus.MAINNET_NETWORK_TYPE;
					
					// Testnet network
					case Consensus.NETWORK_TESTNET_TEXT_VALUE:
					
						// Check wallet type
						switch(Consensus.getWalletType()) {
						
							// GRIN wallet
							case Consensus.GRIN_WALLET_TYPE:
							
								// Return testnet network type
								return Consensus.TESTNET_NETWORK_TYPE;
						}
						
						// Break
						break;
					
					// Floonet network
					case Consensus.NETWORK_FLOONET_TEXT_VALUE:
					
						// Check wallet type
						switch(Consensus.getWalletType()) {
						
							// MWC wallet
							case Consensus.MWC_WALLET_TYPE:
					
								// Return testnet network type
								return Consensus.TESTNET_NETWORK_TYPE;

						}
						
						// Break
						break;
				}
			}
			
			// Return default network type
			return Consensus.DEFAULT_NETWORK_TYPE;
		}
		
		// Wallet type to text
		static walletTypeToText(walletType) {
		
			// Check wallet type
			switch(walletType) {
			
				// MWC wallet type
				case Consensus.MWC_WALLET_TYPE:
				
					// Return MWC wallet text value
					return Consensus.WALLET_MWC_TEXT_VALUE;
				
				// GRIN wallet type
				case Consensus.GRIN_WALLET_TYPE:
				
					// Return GRIN wallet text value
					return Consensus.WALLET_GRIN_TEXT_VALUE;
			}
		}
		
		// Network type to text
		static networkTypeToText(networkType) {
		
			// Check network type
			switch(networkType) {
			
				// Mainnet network type
				case Consensus.MAINNET_NETWORK_TYPE:
				
					// Return mainnet network text value
					return Consensus.NETWORK_MAINNET_TEXT_VALUE;
				
				// Testnet network type
				case Consensus.TESTNET_NETWORK_TYPE:
				
					// Check wallet type
					switch(Consensus.getWalletType()) {
					
						// MWC wallet
						case Consensus.MWC_WALLET_TYPE:
						
							// Return floonet network text value
							return Consensus.NETWORK_FLOONET_TEXT_VALUE;
					
						// GRIN wallet
						case Consensus.GRIN_WALLET_TYPE:
						
							// Return testnet network text value
							return Consensus.NETWORK_TESTNET_TEXT_VALUE;
					}
			}
		}
		
		// Get reward
		static getReward(isMainnet, fees, height) {
		
			// Get block reward at height
			var reward = Consensus.getBlockReward(isMainnet, height);
		
			// Add fee to reward
			reward = reward.plus(fees);
			
			// Return reward
			return reward;
		}
		
		// Is valid header version
		static isValidHeaderVersion(isMainnet, height, version) {
		
			// Return if version is valid at height while taking hard forks into account
			return version.isEqualTo(Consensus.getHeaderVersion(isMainnet, height)) === true;
		}
		
		// Is no recent duplicate kernel enabled
		static isNoRecentDuplicateKernelsEnabled(isMainnet) {
		
			// TODO Support no recent duplicate kernels
			return false;
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// Mainnet network type
				case Consensus.MAINNET_NETWORK_TYPE:
		
					// Check if is mainnet
					if(isMainnet === true) {
					
						// Return false
						return false;
					}
					
					// Otherwise
					else {
					
						// Return true
						return true;
					}
				
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
				
					// Check if is mainnet
					if(isMainnet === true) {
					
						// Return false
						return false;
					}
					
					// Otherwise
					else {
					
						// Return true
						return true;
					}
			}
		}
		
		// Block time seconds
		static get BLOCK_TIME_SECONDS() {
		
			// Return block time seconds
			return 60;
		}
		
		// Block height minute
		static get BLOCK_HEIGHT_MINUTE() {
		
			// Return block height minute
			return Math.floor(Common.SECONDS_IN_A_MINUTE / Consensus.BLOCK_TIME_SECONDS);
		}

		// Block height hour
		static get BLOCK_HEIGHT_HOUR() {
		
			// Return block height hour
			return Common.MINUTES_IN_AN_HOUR * Consensus.BLOCK_HEIGHT_MINUTE;
		}
		
		// Block height day
		static get BLOCK_HEIGHT_DAY() {
		
			// Return block height day
			return Common.HOURS_IN_A_DAY * Consensus.BLOCK_HEIGHT_HOUR;
		}

		// Block height week
		static get BLOCK_HEIGHT_WEEK() {
		
			// Return block height week
			return Common.DAYS_IN_A_WEEK * Consensus.BLOCK_HEIGHT_DAY;
		}

		// Block height year
		static get BLOCK_HEIGHT_YEAR() {
		
			// Return block height year
			return Common.WEEKS_IN_A_YEAR * Consensus.BLOCK_HEIGHT_WEEK;
		}
		
		// Coinbase maturity
		static get COINBASE_MATURITY() {
		
			// Return coinbase maturity
			return Consensus.BLOCK_HEIGHT_DAY;
		}
		
		// First block height
		static get FIRST_BLOCK_HEIGHT() {
		
			// Return first block height
			return 0;
		}
		
		// Value number base
		static get VALUE_NUMBER_BASE() {
		
			// Return value number base
			return 1E9;
		}
		
		// Currency name
		static get CURRENCY_NAME() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
				
					// Return currency name
					return "MWC";
			
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
				
					// Return currency name
					return "GRIN";
			}
		}
		
		// Slatepack address human-readable part
		static get SLATEPACK_ADDRESS_HUMAN_READABLE_PART() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
		
					// Check network type
					switch(Consensus.getNetworkType()) {
					
						// Mainnet network type
						case Consensus.MAINNET_NETWORK_TYPE:
						
							// Return Slatepack address human-readable part
							return "grin";
						
						// Testnet network type
						case Consensus.TESTNET_NETWORK_TYPE:
						
							// Return Slatepack address human-readable part
							return "tgrin";
					}
			}
		}
		
		// MWC wallet type
		static get MWC_WALLET_TYPE() {
		
			// Return MWC wallet type
			return 0;
		}
		
		// GRIN wallet type
		static get GRIN_WALLET_TYPE() {
		
			// Return GRIN wallet type
			return Consensus.MWC_WALLET_TYPE + 1;
		}
		
		// Mainnet network type
		static get MAINNET_NETWORK_TYPE() {
		
			// Return mainnet network type
			return 0;
		}
		
		// Testnet type
		static get TESTNET_NETWORK_TYPE() {
		
			// Return testnet network type
			return Consensus.MAINNET_NETWORK_TYPE + 1;
		}
		
		// Legacy header version
		static get LEGACY_HEADER_VERSION() {
		
			// Return legacy header version
			return new BigNumber(1);
		}
		
		// Block input weight
		static get BLOCK_INPUT_WEIGHT() {
		
			// Return block input weight
			return 1;
		}
		
		// Block output weight
		static get BLOCK_OUTPUT_WEIGHT() {
		
			// Return block output weight
			return 21;
		}
		
		// Block kernel weight
		static get BLOCK_KERNEL_WEIGHT() {
		
			// Return block kernel weight
			return 3;
		}
		
		// Maximum block weight
		static get MAXIMUM_BLOCK_WEIGHT() {
		
			// Return maximum block weight
			return 40000;
		}
		
		// BIP44 coin type
		static get BIP44_COIN_TYPE() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
				
					// Check network type
					switch(Consensus.getNetworkType()) {
					
						// Mainnet network
						case Consensus.MAINNET_NETWORK_TYPE:
						
							// Return BIP44 coin type
							return 593;
						
						// Testnet network
						case Consensus.TESTNET_NETWORK_TYPE:
						
							// Return BIP44 coin type
							return 1;
					}
					
					// Break
					break;
				
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
				
					// Check network type
					switch(Consensus.getNetworkType()) {
					
						// Mainnet network
						case Consensus.MAINNET_NETWORK_TYPE:
						
							// Return BIP44 coin type
							return 592;
						
						// Testnet network
						case Consensus.TESTNET_NETWORK_TYPE:
						
							// Return BIP44 coin type
							return 1;
					}
					
					// Break
					break;
			}
		}
		
		// Explorer kernel excess URL
		static get EXPLORER_KERNEL_EXCESS_URL() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
				
					// Check network type
					switch(Consensus.getNetworkType()) {
					
						// Mainnet network
						case Consensus.MAINNET_NETWORK_TYPE:
						
							// Return explorer kernel excess URL
							return "https://explorer.mwc.mw/#k";
						
						// Testnet network
						case Consensus.TESTNET_NETWORK_TYPE:
						
							// Return explorer kernel excess URL
							return "https://explorer.floonet.mwc.mw/#k";
					}
					
					// Break
					break;
				
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
				
					// Check network type
					switch(Consensus.getNetworkType()) {
					
						// Mainnet network
						case Consensus.MAINNET_NETWORK_TYPE:
						
							// Return no explorer URL
							return Consensus.NO_EXPLORER_URL;
						
						// Testnet network
						case Consensus.TESTNET_NETWORK_TYPE:
						
							// Return no explorer URL
							return Consensus.NO_EXPLORER_URL;
					}
					
					// Break
					break;
			}
		}
		
		// Explorer output commitment URL
		static get EXPLORER_OUTPUT_COMMITMENT_URL() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
				
					// Check network type
					switch(Consensus.getNetworkType()) {
					
						// Mainnet network
						case Consensus.MAINNET_NETWORK_TYPE:
						
							// Return explorer output commitment URL
							return "https://explorer.mwc.mw/#o";
						
						// Testnet network
						case Consensus.TESTNET_NETWORK_TYPE:
						
							// Return explorer output commitment URL
							return "https://explorer.floonet.mwc.mw/#o";
					}
					
					// Break
					break;
				
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
				
					// Check network type
					switch(Consensus.getNetworkType()) {
					
						// Mainnet network
						case Consensus.MAINNET_NETWORK_TYPE:
						
							// Return no explorer URL
							return Consensus.NO_EXPLORER_URL;
						
						// Testnet network
						case Consensus.TESTNET_NETWORK_TYPE:
						
							// Return no explorer URL
							return Consensus.NO_EXPLORER_URL;
					}
					
					// Break
					break;
			}
		}
		
		// No explorer URL
		static get NO_EXPLORER_URL() {
		
			// Return no explorer URL
			return null;
		}
		
		// Hardware waller starting height
		static get HARDWARE_WALLET_STARTING_HEIGHT() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
				
					// Check network type
					switch(Consensus.getNetworkType()) {
					
						// Mainnet network
						case Consensus.MAINNET_NETWORK_TYPE:
						
							// Return hardware wallet starting height
							return 1239928;
						
						// Testnet network
						case Consensus.TESTNET_NETWORK_TYPE:
						
							// Return hardware wallet starting height
							return 1115028;
					}
					
					// Break
					break;
				
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
				
					// Check network type
					switch(Consensus.getNetworkType()) {
					
						// Mainnet network
						case Consensus.MAINNET_NETWORK_TYPE:
						
							// Return hardware wallet starting height
							return 1687446;
						
						// Testnet network
						case Consensus.TESTNET_NETWORK_TYPE:
						
							// Return hardware wallet starting height
							return 1195819;
					}
					
					// Break
					break;
			}
		}
	
	// Private
	
		// Get block reward
		static getBlockReward(isMainnet, height) {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
				
					// Check if height is the first block height
					if(height.isEqualTo(Consensus.FIRST_BLOCK_HEIGHT) === true)
					
						// Return genesis block reward
						return new BigNumber("10000000041800000");
					
					// Otherwise check if height is less than the second block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 2)) === true)
					
						// Return reward during the first epoch range
						return new BigNumber(2380952380);
					
					// Otherwise check if height is less than the third block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 3)) === true)
					
						// Return reward during the second epoch range
						return new BigNumber(600000000);
					
					// Otherwise check if height is less than the fourth block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 4)) === true)
					
						// Return reward during the third epoch range
						return new BigNumber(450000000);
					
					// Otherwise check if height is less than the fifth block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 5)) === true)
					
						// Return reward during the fourth epoch range
						return new BigNumber(300000000);
					
					// Otherwise check if height is less than the sixth block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 6)) === true)
					
						// Return reward during the fifth epoch range
						return new BigNumber(250000000);
					
					// Otherwise check if height is less than the seventh block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 7)) === true)
					
						// Return reward during the sixth epoch range
						return new BigNumber(200000000);
					
					// Otherwise check if height is less than the eighth block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 8)) === true)
					
						// Return reward during the seventh epoch range
						return new BigNumber(150000000);
					
					// Otherwise check if height is less than the ninth block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 9)) === true)
					
						// Return reward during the eighth epoch range
						return new BigNumber(100000000);
					
					// Otherwise check if height is less than the tenth block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 10)) === true)
					
						// Return reward during the ninth epoch range
						return new BigNumber(50000000);
					
					// Otherwise check if height is less than the eleventh block offset
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 11)) === true)
					
						// Return reward during the tenth epoch range
						return new BigNumber(25000000);
					
					// Otherwise check if height is less than the eleventh block offset plus the eleventh epoch duration
					else if(height.isLessThan(Consensus.getEpochBlockOffset(isMainnet, 11).plus(Consensus.getEpochDuration(isMainnet, 11))) === true)
					
						// Return reward during the eleventh epoch range
						return new BigNumber(10000000);
					
					// Otherwise check if height is equal to the eleventh block offset plus the eleventh epoch duration
					else if(height.isEqualTo(Consensus.getEpochBlockOffset(isMainnet, 11).plus(Consensus.getEpochDuration(isMainnet, 11))) === true)
					
						// Return reward during the twelfth epoch range
						return new BigNumber(2211980);
					
					// Otherwise
					else
					
						// Return zero
						return new BigNumber(0);
				
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
				
					// Return block reward
					return new BigNumber("60000000000");
			}
		}
		
		// Get epoch duration
		static getEpochDuration(isMainnet, epoch) {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC allet
				case Consensus.MWC_WALLET_TYPE:
		
					// Check epoch
					switch(epoch) {
					
						// One
						case 1:
						
							// Return duration of first epoch
							return 0;
					
						// Two
						case 2:
						
							// Return duration of second epoch
							return (isMainnet === true) ? 120 * Consensus.BLOCK_HEIGHT_DAY : Consensus.BLOCK_HEIGHT_DAY;
						
						// Three
						case 3:
						
							// Return duration of third epoch
							return (isMainnet === true) ? 60 * Consensus.BLOCK_HEIGHT_DAY : Consensus.BLOCK_HEIGHT_DAY;
						
						// Four
						case 4:
						
							// Return duration of fourth epoch
							return 120 * Consensus.BLOCK_HEIGHT_DAY;
						
						// Five
						case 5:
						
							// Return duration of fifth epoch
							return 180 * Consensus.BLOCK_HEIGHT_DAY;
						
						// Six
						case 6:
						
							// Return duration of sixth epoch
							return 180 * Consensus.BLOCK_HEIGHT_DAY;
						
						// Seven
						case 7:
						
							// Return duration of seventh epoch
							return Consensus.BLOCK_HEIGHT_YEAR;
						
						// Eight
						case 8:
						
							// Return duration of eighth epoch
							return Consensus.BLOCK_HEIGHT_YEAR;
						
						// Nine
						case 9:
						
							// Return duration of ninth epoch
							return 6 * Consensus.BLOCK_HEIGHT_YEAR;
						
						// Tenth
						case 10:
						
							// Return duration of tenth epoch
							return 10 * Consensus.BLOCK_HEIGHT_YEAR;
						
						// Eleven
						case 11:
						
							// Return duration of eleventh epoch
							return 876349148;
					}
			}
		}
		
		// Get epoch block offset
		static getEpochBlockOffset(isMainnet, epoch) {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
		
					// Set offset to the C31 hard fork height
					var offset = Consensus.getC31HardForkHeight(isMainnet);
					
					// Check if past second epoch range
					if(epoch >= 2)
					
						// Add initial block duration to offset
						offset = offset.plus((isMainnet === true) ? Consensus.BLOCK_HEIGHT_WEEK : Consensus.BLOCK_HEIGHT_DAY);
					
					// Go through all remaining epoch ranges
					for(var i = 3; i <= epoch; ++i)
					
						// Add epoch's block duration to offset
						offset = offset.plus(Consensus.getEpochDuration(isMainnet, i - 1));
					
					// Return offset
					return offset;
			}
		}
		
		// Get C31 hard fork height
		static getC31HardForkHeight(isMainnet) {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
		
					// Return C31 hard fork height
					return (isMainnet === true) ? new BigNumber(202500) : new BigNumber(270000);
			}
		}
		
		// Get header version
		static getHeaderVersion(isMainnet, height) {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
				
					// Check if height is less than the C31 hard fork height
					if(height.isLessThan(Consensus.getC31HardForkHeight(isMainnet)) === true)

						// Return legacy header version
						return Consensus.LEGACY_HEADER_VERSION;
					
					// Otherwise
					else
					
						// Return C31 header version
						return Consensus.C31_HEADER_VERSION;
			
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
				
					// Check if mainnet
					if(isMainnet === true) {
					
						// Get header version based on number of hard forks
						var headerVersion = height.dividedToIntegerBy(Consensus.HARD_FORK_INTERVAL).plus(1);
						
						// Check if header version is greater than the maximum header version
						if(headerVersion.isGreaterThan(Consensus.MAXIMUM_HEADER_VERSION) === true)
						
							// Set header version to the maximum header version
							headerVersion = new BigNumber(Consensus.MAXIMUM_HEADER_VERSION);
						
						// Return header version
						return headerVersion;
					}
					
					// Otherwise
					else {
					
						// Check is height is less than first hard fork height
						if(height.isLessThan(Consensus.FIRST_HARD_FORK_HEIGHT) === true)
						
							// Return legacy header version
							return Consensus.LEGACY_HEADER_VERSION;
						
						// Otherwise check if height is less than second hard fork height
						else if(height.isLessThan(Consensus.SECOND_HARD_FORK_HEIGHT) === true)
						
							// Return second header version
							return Consensus.LEGACY_HEADER_VERSION.plus(1);
						
						// Otherwise check if height is less than third hard fork height
						else if(height.isLessThan(Consensus.THIRD_HARD_FORK_HEIGHT) === true)
						
							// Return third header version
							return Consensus.LEGACY_HEADER_VERSION.plus(2);
						
						// Otherwise check if height is less than fourth hard forks height
						else if(height.isLessThan(Consensus.FOURTH_HARD_FORK_HEIGHT) === true)
						
							// Return fourth header version
							return Consensus.LEGACY_HEADER_VERSION.plus(3);
						
						// Otherwise
						else
						
							// Return fifth header version
							return Consensus.LEGACY_HEADER_VERSION.plus(4);
					}
			}
		}
		
		// Hard fork interval
		static get HARD_FORK_INTERVAL() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
		
					// Return hard fork interval
					return Math.floor(Consensus.BLOCK_HEIGHT_YEAR / 2);
			}
		}
		
		// First hard fork height
		static get FIRST_HARD_FORK_HEIGHT() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
			
					// Return first hard fork height
					return 185040;
			}
		}
		
		// Second hard fork height
		static get SECOND_HARD_FORK_HEIGHT() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
			
					// Return second hard fork height
					return 298080;
			}
		}
		
		// Third hard fork height
		static get THIRD_HARD_FORK_HEIGHT() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
			
					// Return third hard fork height
					return 552960;
			}
		}
		
		// Fourth hard fork height
		static get FOURTH_HARD_FORK_HEIGHT() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
			
					// Return fourth hard fork height
					return 642240;
			}
		}
		
		// C31 header version
		static get C31_HEADER_VERSION() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// MWC wallet
				case Consensus.MWC_WALLET_TYPE:
		
					// Return C31 header version
					return Consensus.LEGACY_HEADER_VERSION.plus(1);
			}
		}
		
		// Maximum header version
		static get MAXIMUM_HEADER_VERSION() {
		
			// Check wallet type
			switch(Consensus.getWalletType()) {
			
				// GRIN wallet
				case Consensus.GRIN_WALLET_TYPE:
			
					// Return maximum header version
					return 5;
			}
		}
		
		// Default Wallet type
		static get DEFAULT_WALLET_TYPE() {
		
			// Return default wallet type
			return Consensus.MWC_WALLET_TYPE;
		}
		
		// Default network type
		static get DEFAULT_NETWORK_TYPE() {
		
			// Return default network type
			return Consensus.MAINNET_NETWORK_TYPE;
		}
		
		// Wallet MWC text value
		static get WALLET_MWC_TEXT_VALUE() {
		
			// Return wallet MWC text value
			return (typeof Language !== "undefined") ? Language.getDefaultTranslation('MimbleWimble Coin') : "MimbleWimble Coin";
		}
		
		// Wallet GRIN value
		static get WALLET_GRIN_TEXT_VALUE() {
		
			// Return wallet GRIN text value
			return (typeof Language !== "undefined") ? Language.getDefaultTranslation('Grin') : "Grin";
		}

		// Wallet GRIN value
		static get WALLET_EPIC_TEXT_VALUE() {
		
			// Return wallet EPIC text value
			return (typeof Language !== "undefined") ? Language.getDefaultTranslation('EpicCash') : "EpicCash";
		}
		
		// Network mainnet value
		static get NETWORK_MAINNET_TEXT_VALUE() {
		
			// Return network mainnet text value
			return (typeof Language !== "undefined") ? Language.getDefaultTranslation('Mainnet') : "Mainnet";
		}
		
		// Network floonet value
		static get NETWORK_FLOONET_TEXT_VALUE() {
		
			// Return network floonet text value
			return (typeof Language !== "undefined") ? Language.getDefaultTranslation('Floonet') : "Floonet";
		}
		
		// Network testnet value
		static get NETWORK_TESTNET_TEXT_VALUE() {
		
			// Return network testnet text value
			return (typeof Language !== "undefined") ? Language.getDefaultTranslation('Testnet') : "Testnet";
		}
		
		// Override wallet type URL paramater name
		static get OVERRIDE_WALLET_TYPE_URL_PARAMETER_NAME() {
		
			// Return override wallet type URL paramater name
			return "Override Wallet Type";
		}
		
		// Override network type URL paramater name
		static get OVERRIDE_NETWORK_TYPE_URL_PARAMETER_NAME() {
		
			// Return override network type URL paramater name
			return "Override Network Type";
		}
}


// Main function

// Set global object's consensus
globalThis["Consensus"] = Consensus;

// Export consensus
module["exports"] = Consensus;
