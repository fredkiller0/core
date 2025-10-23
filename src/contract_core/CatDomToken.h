struct Mint_input { uint64 amount; };
struct Mint_output { bool success; };

struct Transfer_input { id to; uint64 amount; };
struct Transfer_output { 
    bool success; 
    uint64 netAmount;
    uint64 taxAmount;
};

struct Burn_input { uint64 amount; };
struct Burn_output { bool success; };

struct AirdropWithVesting_input { 
    id recipients[100]; 
    uint64 amountPer; 
    uint64 numRecipients;
    uint8 catType;  // 0=General, 1=Noisette, 2=Cacahuète, 3=Chipie
    uint64 cliffEpochs;    // Période avant première libération
    uint64 vestingEpochs;   // Durée totale du vesting
};
struct AirdropWithVesting_output { 
    uint64 distributed;
    uint32 numVestingsCreated;
    bool success;
};

struct InstantAirdrop_input { 
    id recipients[100]; 
    uint64 amountPer; 
    uint64 numRecipients; 
};
struct InstantAirdrop_output { uint64 distributed; };

struct VerifyAndBonus_input {
    id userAddress;
    uint8 catType;
};
struct VerifyAndBonus_output {
    bool success;
    uint64 bonusAmount;
};

struct Balance_input { id address; };
struct Balance_output { uint64 balance; };

struct TotalSupply_input { };
struct TotalSupply_output { uint64 supply; };

struct GetTokenomics_input { };
struct GetTokenomics_output {
    uint64 maxSupply;
    uint64 currentSupply;
    uint64 creatorBalance;
    uint64 airdropReserveRemaining;
    uint64 marketingReserveRemaining;
    uint64 spaReserveRemaining;
    uint64 liquidityPoolRemaining;
    uint64 totalBurned;
    uint64 totalAirdropped;
};

struct Pause_input { };
struct Pause_output { bool success; };

struct Unpause_input { };
struct Unpause_output { bool success; };

struct IsPaused_input { };
struct IsPaused_output { bool paused; };

struct AddToWhitelist_input { id address; };
struct AddToWhitelist_output { bool success; };

struct RemoveFromWhitelist_input { id address; };
struct RemoveFromWhitelist_output { bool success; };

struct IsWhitelisted_input { id address; };
struct IsWhitelisted_output { bool whitelisted; };

struct TransferOwnership_input { id newOwner; };
struct TransferOwnership_output { bool success; };

struct GetOwner_input { };
struct GetOwner_output { id owner; };

struct SetTaxWallet_input { id newTaxWallet; };
struct SetTaxWallet_output { bool success; };

struct SetTaxEnabled_input { bool enabled; };
struct SetTaxEnabled_output { bool success; };

struct GetTaxConfig_input { };
struct GetTaxConfig_output {
    bool taxEnabled;
    uint16 totalTaxBPS;
    uint16 creatorTaxBPS;
    uint16 burnTaxBPS;
    uint16 spaTaxBPS;
    uint16 marketingTaxBPS;
    id taxWallet;
};

struct GetCatStats_input { uint8 catType; };
struct GetCatStats_output {
    uint64 reserve;
    uint64 distributed;
    uint64 remaining;
};

struct LockLiquidity_input { 
    uint64 amount;
    uint64 unlockEpoch;
};
struct LockLiquidity_output { 
    bool success; 
    uint32 lockId;
};

struct GetLiquidityLock_input { uint32 lockId; };
struct GetLiquidityLock_output {
    bool exists;
    uint64 amount;
    uint64 unlockEpoch;
    bool unlocked;
};

struct UnlockLiquidity_input { uint32 lockId; };
struct UnlockLiquidity_output { 
    bool success;
    uint64 unlockedAmount;
};

// ==================== VESTING STRUCTS ====================

struct VestingSchedule {
    id beneficiary;
    uint64 totalAmount;
    uint64 releasedAmount;
    uint64 startEpoch;
    uint64 cliffEpoch;
    uint64 endEpoch;
    bool revoked;
    bool isAirdropVesting;
};

struct CreateVesting_input {
    id beneficiary;
    uint64 totalAmount;
    uint64 startEpoch;
    uint64 cliffEpoch;
    uint64 durationEpochs;
};
struct CreateVesting_output { bool success; uint32 vestingId; };

struct ReleaseVesting_input { uint32 vestingId; };
struct ReleaseVesting_output { bool success; uint64 released; };

struct GetVestingInfo_input { uint32 vestingId; };
struct GetVestingInfo_output { 
    id beneficiary;
    uint64 totalAmount;
    uint64 releasedAmount;
    uint64 vestedAmount;
    bool revoked;
    bool isAirdropVesting;
};

struct RevokeVesting_input { uint32 vestingId; };
struct RevokeVesting_output { bool success; };

struct GetUserVestings_input { id user; };
struct GetUserVestings_output {
    uint32 numVestings;
    uint32 vestingIds[50];
};

// ==================== WARRIOR STRUCTS ====================

struct WarriorPoints {
    id warrior;
    uint64 points;
    uint64 totalRewarded;
};

struct AddWarriorPoints_input {
    id warrior;
    uint64 points;
    uint8 catType;
};
struct AddWarriorPoints_output { bool success; };

struct RewardWarrior_input {
    id warrior;
    uint64 rewardAmount;
    uint8 catType;
};
struct RewardWarrior_output { 
    bool success; 
    uint64 rewarded;
};

struct GetWarriorInfo_input { id warrior; };
struct GetWarriorInfo_output {
    uint64 points;
    uint64 totalRewarded;
    bool exists;
};

// ==================== NFT BADGE STRUCTS ====================

struct Badge {
    uint32 badgeId;
    uint8 badgeType;        // 1=Bronze, 2=Silver, 3=Gold, 4=Diamond, 5=Legendary
    uint8 category;         // 1=Recrutement, 2=Combat, 3=Loyauté, 4=Chat, 5=Spécial
    uint8 catAlignment;     // 0=Neutre, 1=Noisette, 2=Cacahuète, 3=Chipie
    id owner;
    uint64 mintEpoch;
    uint32 missionId;
    bool soulbound;
};

struct Mission {
    uint32 missionId;
    uint8 missionType;      // 1=Points, 2=Referrals, 3=Volume, 4=Time, 5=Special
    uint8 requiredCat;
    uint64 requirement;
    uint64 rewardAmount;
    uint8 badgeReward;
    uint8 badgeCategory;
    bool active;
    uint32 completions;
};

struct BadgeMetadata {
    uint8 badgeType;
    uint8 category;
    uint16 multiplier;
    uint64 requiredPoints;
};

struct InitBadgeType_input {
    uint8 badgeType;
    uint8 category;
    uint16 multiplier;
    uint64 requiredPoints;
};
struct InitBadgeType_output { bool success; };

struct CreateMission_input {
    uint8 missionType;
    uint8 requiredCat;
    uint64 requirement;
    uint64 rewardAmount;
    uint8 badgeReward;
    uint8 badgeCategory;
};
struct CreateMission_output { 
    bool success; 
    uint32 missionId;
};

struct MintBadge_input {
    id recipient;
    uint8 badgeType;
    uint8 category;
    uint8 catAlignment;
    uint32 missionId;
    bool soulbound;
};
struct MintBadge_output { 
    bool success; 
    uint32 badgeId;
};

struct CompleteMission_input {
    uint32 missionId;
    id warrior;
    uint64 proofValue;
};
struct CompleteMission_output {
    bool success;
    uint32 badgeId;
    uint64 rewardAmount;
};

struct TransferBadge_input {
    uint32 badgeId;
    id to;
};
struct TransferBadge_output { bool success; };

struct GetBadgeInfo_input { uint32 badgeId; };
struct GetBadgeInfo_output {
    bool exists;
    uint8 badgeType;
    uint8 category;
    uint8 catAlignment;
    id owner;
    uint64 mintEpoch;
    uint32 missionId;
    bool soulbound;
};

struct GetUserBadges_input { id user; };
struct GetUserBadges_output {
    uint32 numBadges;
    uint32 badgeIds[100];
};

struct GetMissionInfo_input { uint32 missionId; };
struct GetMissionInfo_output {
    bool exists;
    uint8 missionType;
    uint8 requiredCat;
    uint64 requirement;
    uint64 rewardAmount;
    uint8 badgeReward;
    bool active;
    uint32 completions;
};

struct CalculateUserMultiplier_input { id user; };
struct CalculateUserMultiplier_output { 
    uint16 totalMultiplier;
    uint32 numBadges;
};

struct CheckBadgeCombo_input { id user; };
struct CheckBadgeCombo_output {
    bool hasCombo;
    uint8 comboType;
    uint64 bonusReward;
};

struct GetBadgeStats_input { };
struct GetBadgeStats_output {
    uint32 totalBadges;
    uint32 totalMissions;
    uint64 totalMissionsCompleted;
    uint32 activeMissions;
    uint32 bronzeBadges;
    uint32 silverBadges;
    uint32 goldBadges;
    uint32 diamondBadges;
    uint32 legendaryBadges;
};

// ==================== LIQUIDITY LOCK STRUCT ====================

struct LiquidityLock {
    uint64 amount;
    uint64 unlockEpoch;
    bool unlocked;
};

private:
// ==================== CONSTANTES - NOUVELLE TOKENOMICS ====================
static const uint32 MAX_HOLDERS = 1000000;
static const uint64 MAX_SUPPLY = 1000000000000000ULL;           // 1 Quadrillion

// NOUVELLE RÉPARTITION OPTIMISÉE
static const uint64 CREATOR_SUPPLY = 150000000000000ULL;        // 15% - TOI (liquid dès le début)
static const uint64 AIRDROP_RESERVE = 80000000000000ULL;        // 8% - Airdrops avec vesting
static const uint64 MARKETING_RESERVE = 70000000000000ULL;      // 7% - Marketing & Growth
static const uint64 LIQUIDITY_POOL = 150000000000000ULL;        // 15% - Liquidity (locked 12-24 mois)
static const uint64 SPA_RESERVE = 100000000000000ULL;           // 10% - Donations SPA
static const uint64 TEAM_VESTING = 350000000000000ULL;          // 35% - Team (vesting 24 mois)
static const uint64 WARRIORS_POOL = 100000000000000ULL;         // 10% - Warriors rewards

// Réserves des chats (incluses dans team vesting)
static const uint64 NOISETTE_RESERVE = 20000000000000ULL;       // 2%
static const uint64 CACAHUETE_RESERVE = 20000000000000ULL;      // 2%
static const uint64 CHIPIE_RESERVE = 20000000000000ULL;         // 2%
static const uint64 CATS_TOTAL_RESERVE = 60000000000000ULL;     // 6% total

// TRANSFER TAX - TA RENTABILITÉ CONTINUE
static const uint16 TOTAL_TAX_BPS = 300;        // 3% total
static const uint16 CREATOR_TAX_BPS = 100;      // 1% pour toi
static const uint16 BURN_TAX_BPS = 100;         // 1% burn (déflation)
static const uint16 SPA_TAX_BPS = 50;           // 0.5% SPA
static const uint16 MARKETING_TAX_BPS = 50;     // 0.5% marketing

// VESTING par défaut pour airdrops
static const uint64 DEFAULT_AIRDROP_CLIFF = 10080;      // ~1 mois (7 epochs/tick)
static const uint64 DEFAULT_AIRDROP_VESTING = 50400;    // ~5 mois total

static const uint32 MAX_WHITELIST = 10000;
static const uint32 MAX_VESTING = 10000;
static const uint32 MAX_WARRIORS = 1000;
static const uint32 MAX_LIQUIDITY_LOCKS = 100;
static const uint64 MIN_TRANSFER = 1;
static const uint64 MAX_AIRDROP_RECIPIENTS = 100;

// NFT Badges constants
static const uint32 MAX_BADGES = 100000;
static const uint32 MAX_MISSIONS = 1000;
static const uint32 MAX_BADGE_TYPES = 50;

// ==================== STATE VARIABLES ====================

id holders[MAX_HOLDERS];
uint64 balances[MAX_HOLDERS];
uint64 totalSupply;
uint32 numHolders;
bool initialized;
bool paused;
id owner;
id taxWallet;
bool taxEnabled;

// Reserves tracking
uint64 airdropReserveUsed;
uint64 marketingReserveUsed;
uint64 spaReserveUsed;
uint64 liquidityPoolUsed;
uint64 teamVestingUsed;
uint64 warriorsPoolUsed;

// Whitelist
id whitelist[MAX_WHITELIST];
uint32 numWhitelisted;

// Vesting
VestingSchedule vestingSchedules[MAX_VESTING];
uint32 numVestings;

// Warriors
WarriorPoints warriors[MAX_WARRIORS];
uint32 numWarriors;

// Tracking par chat
uint64 noisetteDistributed;
uint64 cacahueteDistributed;
uint64 chipieDistributed;

// Tracking des airdrops vérifiés
id verifiedAirdropAddresses[MAX_HOLDERS];
uint32 numVerifiedAirdrops;
bool hasReceivedBonus[MAX_HOLDERS];

// NFT Badges
Badge badges[MAX_BADGES];
uint32 numBadges;
uint32 badgesPerAddress[MAX_HOLDERS];

// Missions
Mission missions[MAX_MISSIONS];
uint32 numMissions;

// Métadonnées des badges
BadgeMetadata badgeMetadata[MAX_BADGE_TYPES];
uint32 numBadgeTypes;

// Tracking des missions
uint32 userMissionProgress[MAX_HOLDERS][MAX_MISSIONS];
bool userMissionCompleted[MAX_HOLDERS][MAX_MISSIONS];

// Liquidity locks
LiquidityLock liquidityLocks[MAX_LIQUIDITY_LOCKS];
uint32 numLiquidityLocks;

// Statistics
uint64 totalBurned;
uint64 totalAirdropped;
uint64 totalTransfers;
uint64 totalTaxCollected;
uint64 totalBadgesMinted;
uint64 totalMissionsCompleted;

// ==================== HELPER FUNCTIONS ====================

bool safeAdd(uint64 a, uint64 b, uint64& result) {
    if (a > UINT64_MAX - b) return false;
    result = a + b;
    return true;
}

bool safeSub(uint64 a, uint64 b, uint64& result) {
    if (a < b) return false;
    result = a - b;
    return true;
}

bool safeMul(uint64 a, uint64 b, uint64& result) {
    if (a == 0 || b == 0) {
        result = 0;
        return true;
    }
    if (a > UINT64_MAX / b) return false;
    result = a * b;
    return true;
}

sint32 findHolder(const id& addr) {
    for (uint32 i = 0; i < state.numHolders; i++) {
        if (state.holders[i] == addr) return i;
    }
    return -1;
}

bool isWhitelisted(const id& addr) {
    if (addr == state.owner) return true;
    for (uint32 i = 0; i < state.numWhitelisted; i++) {
        if (state.whitelist[i] == addr) return true;
    }
    return false;
}

sint32 findWhitelisted(const id& addr) {
    for (uint32 i = 0; i < state.numWhitelisted; i++) {
        if (state.whitelist[i] == addr) return i;
    }
    return -1;
}

bool isNullId(const id& addr) {
    id nullId;
    return addr == nullId;
}

bool addBalance(const id& addr, uint64 amount) {
    if (amount == 0) return false;
    if (isNullId(addr)) return false;
    
    sint32 idx = findHolder(addr);
    if (idx >= 0) {
        uint64 newBalance;
        if (!safeAdd(state.balances[idx], amount, newBalance)) {
            return false;
        }
        state.balances[idx] = newBalance;
        return true;
    } else if (state.numHolders < MAX_HOLDERS) {
        state.holders[state.numHolders] = addr;
        state.balances[state.numHolders] = amount;
        state.numHolders++;
        return true;
    }
    return false;
}

bool subtractBalance(const id& addr, uint64 amount) {
    if (amount == 0) return false;
    
    sint32 idx = findHolder(addr);
    if (idx >= 0) {
        uint64 newBalance;
        if (!safeSub(state.balances[idx], amount, newBalance)) {
            return false;
        }
        state.balances[idx] = newBalance;
        return true;
    }
    return false;
}

uint64 getBalance(const id& addr) {
    sint32 idx = findHolder(addr);
    return (idx >= 0) ? state.balances[idx] : 0;
}

uint64 calculateVestedAmount(uint32 vestingId, uint64 currentEpoch) {
    if (vestingId >= state.numVestings) return 0;
    
    VestingSchedule& schedule = state.vestingSchedules[vestingId];
    
    if (schedule.revoked) return schedule.releasedAmount;
    if (currentEpoch < schedule.cliffEpoch) return 0;
    if (currentEpoch >= schedule.endEpoch) return schedule.totalAmount;
    
    uint64 duration = schedule.endEpoch - schedule.startEpoch;
    uint64 elapsed = currentEpoch - schedule.startEpoch;
    
    uint64 vested;
    if (!safeMul(schedule.totalAmount, elapsed, vested)) return 0;
    vested = vested / duration;
    
    return vested;
}

bool createVestingSchedule(const id& beneficiary, uint64 amount, 
                           uint64 cliffEpochs, uint64 vestingEpochs, 
                           bool isAirdrop, uint32& vestingId) {
    if (state.numVestings >= MAX_VESTING) return false;
    if (isNullId(beneficiary) || amount == 0) return false;
    
    vestingId = state.numVestings;
    VestingSchedule& schedule = state.vestingSchedules[vestingId];
    
    uint64 currentEpoch = qpi.epoch();
    schedule.beneficiary = beneficiary;
    schedule.totalAmount = amount;
    schedule.releasedAmount = 0;
    schedule.startEpoch = currentEpoch;
    schedule.cliffEpoch = currentEpoch + cliffEpochs;
    schedule.endEpoch = currentEpoch + vestingEpochs;
    schedule.revoked = false;
    schedule.isAirdropVesting = isAirdrop;
    
    state.numVestings++;
    return true;
}
public:
// ==================== ADMIN FUNCTIONS ====================
PUBLIC_PROCEDURE(Pause) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    state.paused = true;
    output.success = true;
} _

PUBLIC_PROCEDURE(Unpause) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    state.paused = false;
    output.success = true;
} _

PUBLIC_FUNCTION(IsPaused) {
    output.paused = state.paused;
} _

PUBLIC_PROCEDURE(TransferOwnership) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    if (isNullId(input.newOwner)) {
        output.success = false;
        return;
    }
    state.owner = input.newOwner;
    output.success = true;
} _

PUBLIC_FUNCTION(GetOwner) {
    output.owner = state.owner;
} _

PUBLIC_PROCEDURE(SetTaxWallet) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    if (isNullId(input.newTaxWallet)) {
        output.success = false;
        return;
    }
    state.taxWallet = input.newTaxWallet;
    output.success = true;
} _

PUBLIC_PROCEDURE(SetTaxEnabled) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    state.taxEnabled = input.enabled;
    output.success = true;
} _

PUBLIC_FUNCTION(GetTaxConfig) {
    output.taxEnabled = state.taxEnabled;
    output.totalTaxBPS = TOTAL_TAX_BPS;
    output.creatorTaxBPS = CREATOR_TAX_BPS;
    output.burnTaxBPS = BURN_TAX_BPS;
    output.spaTaxBPS = SPA_TAX_BPS;
    output.marketingTaxBPS = MARKETING_TAX_BPS;
    output.taxWallet = state.taxWallet;
} _

// ==================== WHITELIST FUNCTIONS ====================

PUBLIC_PROCEDURE(AddToWhitelist) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    if (isNullId(input.address)) {
        output.success = false;
        return;
    }
    if (isWhitelisted(input.address)) {
        output.success = true;
        return;
    }
    if (state.numWhitelisted >= MAX_WHITELIST) {
        output.success = false;
        return;
    }
    state.whitelist[state.numWhitelisted] = input.address;
    state.numWhitelisted++;
    output.success = true;
} _

PUBLIC_PROCEDURE(RemoveFromWhitelist) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    sint32 idx = findWhitelisted(input.address);
    if (idx < 0) {
        output.success = false;
        return;
    }
    state.whitelist[idx] = state.whitelist[state.numWhitelisted - 1];
    state.numWhitelisted--;
    output.success = true;
} _

PUBLIC_FUNCTION(IsWhitelisted) {
    output.whitelisted = isWhitelisted(input.address);
} _

// ==================== TOKEN FUNCTIONS ====================

PUBLIC_PROCEDURE(Mint) {
    if (state.paused) {
        output.success = false;
        return;
    }
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    if (input.amount == 0) {
        output.success = false;
        return;
    }
    
    uint64 newSupply;
    if (!safeAdd(state.totalSupply, input.amount, newSupply) || 
        newSupply > MAX_SUPPLY) {
        output.success = false;
        return;
    }
    
    if (!addBalance(qpi.invocator(), input.amount)) {
        output.success = false;
        return;
    }
    
    state.totalSupply = newSupply;
    output.success = true;
} _

PUBLIC_PROCEDURE(Transfer) {
    if (state.paused) {
        output.success = false;
        output.netAmount = 0;
        output.taxAmount = 0;
        return;
    }
    if (input.amount < MIN_TRANSFER) {
        output.success = false;
        output.netAmount = 0;
        output.taxAmount = 0;
        return;
    }
    if (qpi.invocator() == input.to) {
        output.success = false;
        output.netAmount = 0;
        output.taxAmount = 0;
        return;
    }
    if (isNullId(input.to)) {
        output.success = false;
        output.netAmount = 0;
        output.taxAmount = 0;
        return;
    }
    
    uint64 taxAmount = 0;
    uint64 netAmount = input.amount;
    
    // Applique la taxe sauf pour owner et whitelist
    if (state.taxEnabled && !isWhitelisted(qpi.invocator()) && !isWhitelisted(input.to)) {
        taxAmount = (input.amount * TOTAL_TAX_BPS) / 10000;
        netAmount = input.amount - taxAmount;
        
        // Distribution de la taxe
        uint64 creatorTax = (input.amount * CREATOR_TAX_BPS) / 10000;
        uint64 burnTax = (input.amount * BURN_TAX_BPS) / 10000;
        uint64 spaTax = (input.amount * SPA_TAX_BPS) / 10000;
        uint64 marketingTax = (input.amount * MARKETING_TAX_BPS) / 10000;
        
        // Transfert de la taxe créateur
        if (!isNullId(state.taxWallet) && creatorTax > 0) {
            addBalance(state.taxWallet, creatorTax);
        }
        
        // Burn
        if (burnTax > 0) {
            uint64 newSupply;
            if (safeSub(state.totalSupply, burnTax, newSupply)) {
                state.totalSupply = newSupply;
                state.totalBurned += burnTax;
            }
        }
        
        // Autres taxes (SPA, Marketing) - ajoutées aux réserves
        state.totalTaxCollected += taxAmount;
    }
    
    if (!subtractBalance(qpi.invocator(), input.amount)) {
        output.success = false;
        output.netAmount = 0;
        output.taxAmount = 0;
        return;
    }
    
    if (!addBalance(input.to, netAmount)) {
        addBalance(qpi.invocator(), input.amount);
        output.success = false;
        output.netAmount = 0;
        output.taxAmount = 0;
        return;
    }
    
    state.totalTransfers++;
    output.success = true;
    output.netAmount = netAmount;
    output.taxAmount = taxAmount;
} _

PUBLIC_PROCEDURE(Burn) {
    if (state.paused) {
        output.success = false;
        return;
    }
    if (input.amount == 0) {
        output.success = false;
        return;
    }
    
    if (!subtractBalance(qpi.invocator(), input.amount)) {
        output.success = false;
        return;
    }
    
    uint64 newSupply;
    if (!safeSub(state.totalSupply, input.amount, newSupply)) {
        addBalance(qpi.invocator(), input.amount);
        output.success = false;
        return;
    }
    
    state.totalSupply = newSupply;
    state.totalBurned += input.amount;
    output.success = true;
} _

// ==================== AIRDROP AVEC VESTING ====================

PUBLIC_PROCEDURE(AirdropWithVesting) {
    if (state.paused) {
        output.success = false;
        output.distributed = 0;
        output.numVestingsCreated = 0;
        return;
    }
    
    if (qpi.invocator() != state.owner && !isWhitelisted(qpi.invocator())) {
        output.success = false;
        output.distributed = 0;
        output.numVestingsCreated = 0;
        return;
    }
    
    if (input.numRecipients > MAX_AIRDROP_RECIPIENTS || input.amountPer == 0) {
        output.success = false;
        output.distributed = 0;
        output.numVestingsCreated = 0;
        return;
    }
    
    uint64 totalDist;
    if (!safeMul(input.amountPer, input.numRecipients, totalDist)) {
        output.success = false;
        output.distributed = 0;
        output.numVestingsCreated = 0;
        return;
    }
    
    // Vérifier les limites de réserve
    if (input.catType > 0) {
        uint64 newDistributed = 0;
        switch(input.catType) {
            case 1:
                newDistributed = state.noisetteDistributed + totalDist;
                if (newDistributed > NOISETTE_RESERVE) {
                    output.success = false;
                    output.distributed = 0;
                    output.numVestingsCreated = 0;
                    return;
                }
                break;
            case 2:
                newDistributed = state.cacahueteDistributed + totalDist;
                if (newDistributed > CACAHUETE_RESERVE) {
                    output.success = false;
                    output.distributed = 0;
                    output.numVestingsCreated = 0;
                    return;
                }
                break;
            case 3:
                newDistributed = state.chipieDistributed + totalDist;
                if (newDistributed > CHIPIE_RESERVE) {
                    output.success = false;
                    output.distributed = 0;
                    output.numVestingsCreated = 0;
                    return;
                }
                break;
        }
    } else {
        uint64 newUsed = state.airdropReserveUsed + totalDist;
        if (newUsed > AIRDROP_RESERVE) {
            output.success = false;
            output.distributed = 0;
            output.numVestingsCreated = 0;
            return;
        }
    }
    
    // Utiliser les valeurs par défaut si non spécifiées
    uint64 cliffEpochs = input.cliffEpochs > 0 ? input.cliffEpochs : DEFAULT_AIRDROP_CLIFF;
    uint64 vestingEpochs = input.vestingEpochs > 0 ? input.vestingEpochs : DEFAULT_AIRDROP_VESTING;
    
    uint32 vestingsCreated = 0;
    for (uint64 i = 0; i < input.numRecipients; i++) {
        if (isNullId(input.recipients[i])) continue;
        
        uint32 vestingId;
        if (createVestingSchedule(input.recipients[i], input.amountPer, 
                                 cliffEpochs, vestingEpochs, true, vestingId)) {
            vestingsCreated++;
        }
    }
    
    // Mettre à jour les compteurs
    uint64 actualDistributed = vestingsCreated * input.amountPer;
    
    if (input.catType > 0) {
        switch(input.catType) {
            case 1: state.noisetteDistributed += actualDistributed; break;
            case 2: state.cacahueteDistributed += actualDistributed; break;
            case 3: state.chipieDistributed += actualDistributed; break;
        }
    } else {
        state.airdropReserveUsed += actualDistributed;
    }
    
    state.totalAirdropped += actualDistributed;
    output.distributed = actualDistributed;
    output.numVestingsCreated = vestingsCreated;
    output.success = true;
} _

// ==================== AIRDROP INSTANTANÉ (pour whitelisted) ====================

PUBLIC_PROCEDURE(InstantAirdrop) {
    if (state.paused) {
        output.distributed = 0;
        return;
    }
    if (qpi.invocator() != state.owner && !isWhitelisted(qpi.invocator())) {
        output.distributed = 0;
        return;
    }
    if (input.amountPer == 0 || input.numRecipients == 0 || 
        input.numRecipients > MAX_AIRDROP_RECIPIENTS) {
        output.distributed = 0;
        return;
    }
    
    uint64 totalDist;
    if (!safeMul(input.amountPer, input.numRecipients, totalDist)) {
        output.distributed = 0;
        return;
    }
    
    if (!subtractBalance(qpi.invocator(), totalDist)) {
        output.distributed = 0;
        return;
    }
    
    uint64 actuallyDistributed = 0;
    for (uint64 i = 0; i < input.numRecipients; i++) {
        if (isNullId(input.recipients[i])) continue;
        
        if (addBalance(input.recipients[i], input.amountPer)) {
            actuallyDistributed += input.amountPer;
        }
    }
    
    uint64 toRefund = totalDist - actuallyDistributed;
    if (toRefund > 0) {
        addBalance(qpi.invocator(), toRefund);
    }
    
    state.totalAirdropped += actuallyDistributed;
    output.distributed = actuallyDistributed;
} _

PUBLIC_PROCEDURE(VerifyAndBonus) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        output.bonusAmount = 0;
        return;
    }
    
    sint32 idx = findHolder(input.userAddress);
    if (idx < 0) {
        output.success = false;
        output.bonusAmount = 0;
        return;
    }
    
    if (state.hasReceivedBonus[idx]) {
        output.success = false;
        output.bonusAmount = 0;
        return;
    }
    
    uint64 currentBalance = state.balances[idx];
    uint64 bonusAmount = (currentBalance * 5) / 100;
    
    uint64 newDistributed = 0;
    switch(input.catType) {
        case 1: 
            newDistributed = state.noisetteDistributed + bonusAmount;
            if (newDistributed > NOISETTE_RESERVE) {
                output.success = false;
                output.bonusAmount = 0;
                return;
            }
            state.noisetteDistributed = newDistributed;
            break;
        case 2:
            newDistributed = state.cacahueteDistributed + bonusAmount;
            if (newDistributed > CACAHUETE_RESERVE) {
                output.success = false;
                output.bonusAmount = 0;
                return;
            }
            state.cacahueteDistributed = newDistributed;
            break;
        case 3:
            newDistributed = state.chipieDistributed + bonusAmount;
            if (newDistributed > CHIPIE_RESERVE) {
                output.success = false;
                output.bonusAmount = 0;
                return;
            }
            state.chipieDistributed = newDistributed;
            break;
        default:
            output.success = false;
            output.bonusAmount = 0;
            return;
    }
    
    if (!addBalance(input.userAddress, bonusAmount)) {
        output.success = false;
        output.bonusAmount = 0;
        return;
    }
    
    state.hasReceivedBonus[idx] = true;
    state.verifiedAirdropAddresses[state.numVerifiedAirdrops] = input.userAddress;
    state.numVerifiedAirdrops++;
    
    if (!isWhitelisted(input.userAddress) && state.numWhitelisted < MAX_WHITELIST) {
        state.whitelist[state.numWhitelisted] = input.userAddress;
        state.numWhitelisted++;
    }
    
    output.success = true;
    output.bonusAmount = bonusAmount;
} _

PUBLIC_FUNCTION(Balance) {
    output.balance = getBalance(input.address);
} _

PUBLIC_FUNCTION(TotalSupply) {
    output.supply = state.totalSupply;
} _

PUBLIC_FUNCTION(GetTokenomics) {
    output.maxSupply = MAX_SUPPLY;
    output.currentSupply = state.totalSupply;
    output.creatorBalance = getBalance(state.owner);
    output.airdropReserveRemaining = AIRDROP_RESERVE - state.airdropReserveUsed;
    output.marketingReserveRemaining = MARKETING_RESERVE - state.marketingReserveUsed;
    output.spaReserveRemaining = SPA_RESERVE - state.spaReserveUsed;
    output.liquidityPoolRemaining = LIQUIDITY_POOL - state.liquidityPoolUsed;
    output.totalBurned = state.totalBurned;
    output.totalAirdropped = state.totalAirdropped;
} _

PUBLIC_FUNCTION(GetCatStats) {
    switch(input.catType) {
        case 1:
            output.reserve = NOISETTE_RESERVE;
            output.distributed = state.noisetteDistributed;
            output.remaining = NOISETTE_RESERVE - state.noisetteDistributed;
            break;
        case 2:
            output.reserve = CACAHUETE_RESERVE;
            output.distributed = state.cacahueteDistributed;
            output.remaining = CACAHUETE_RESERVE - state.cacahueteDistributed;
            break;
        case 3:
            output.reserve = CHIPIE_RESERVE;
            output.distributed = state.chipieDistributed;
            output.remaining = CHIPIE_RESERVE - state.chipieDistributed;
            break;
        default:
            output.reserve = 0;
            output.distributed = 0;
            output.remaining = 0;
    }
} _

// ==================== LIQUIDITY LOCK ====================

PUBLIC_PROCEDURE(LockLiquidity) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        output.lockId = 0;
        return;
    }
    
    if (state.numLiquidityLocks >= MAX_LIQUIDITY_LOCKS) {
        output.success = false;
        output.lockId = 0;
        return;
    }
    
    if (input.amount == 0 || input.unlockEpoch <= qpi.epoch()) {
        output.success = false;
        output.lockId = 0;
        return;
    }
    
    uint64 newUsed = state.liquidityPoolUsed + input.amount;
    if (newUsed > LIQUIDITY_POOL) {
        output.success = false;
        output.lockId = 0;
        return;
    }
    
    uint32 lockId = state.numLiquidityLocks;
    LiquidityLock& lock = state.liquidityLocks[lockId];
    
    lock.amount = input.amount;
    lock.unlockEpoch = input.unlockEpoch;
    lock.unlocked = false;
    
    state.liquidityPoolUsed = newUsed;
    state.numLiquidityLocks++;
    
    output.success = true;
    output.lockId = lockId;
} _

PUBLIC_FUNCTION(GetLiquidityLock) {
    if (input.lockId >= state.numLiquidityLocks) {
        output.exists = false;
        output.amount = 0;
        output.unlockEpoch = 0;
        output.unlocked = false;
        return;
    }
    
    LiquidityLock& lock = state.liquidityLocks[input.lockId];
    output.exists = true;
    output.amount = lock.amount;
    output.unlockEpoch = lock.unlockEpoch;
    output.unlocked = lock.unlocked;
} _

PUBLIC_PROCEDURE(UnlockLiquidity) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        output.unlockedAmount = 0;
        return;
    }
    
    if (input.lockId >= state.numLiquidityLocks) {
        output.success = false;
        output.unlockedAmount = 0;
        return;
    }
    
    LiquidityLock& lock = state.liquidityLocks[input.lockId];
    
    if (lock.unlocked) {
        output.success = false;
        output.unlockedAmount = 0;
        return;
    }
    
    if (qpi.epoch() < lock.unlockEpoch) {
        output.success = false;
        output.unlockedAmount = 0;
        return;
    }
    
    if (!addBalance(state.owner, lock.amount)) {
        output.success = false;
        output.unlockedAmount = 0;
        return;
    }
    
    lock.unlocked = true;
    output.success = true;
    output.unlockedAmount = lock.amount;
} _

// ==================== VESTING FUNCTIONS ====================

PUBLIC_PROCEDURE(CreateVesting) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        output.vestingId = 0;
        return;
    }
    
    uint32 vestingId;
    if (!createVestingSchedule(input.beneficiary, input.totalAmount, 
                               input.cliffEpoch - input.startEpoch, 
                               input.durationEpochs, false, vestingId)) {
        output.success = false;
        output.vestingId = 0;
        return;
    }
    
    if (getBalance(state.owner) < input.totalAmount) {
        state.numVestings--;
        output.success = false;
        output.vestingId = 0;
        return;
    }
    
    if (!subtractBalance(state.owner, input.totalAmount)) {
        state.numVestings--;
        output.success = false;
        output.vestingId = 0;
        return;
    }
    
    output.success = true;
    output.vestingId = vestingId;
} _

PUBLIC_PROCEDURE(ReleaseVesting) {
    if (input.vestingId >= state.numVestings) {
        output.success = false;
        output.released = 0;
        return;
    }
    
    VestingSchedule& schedule = state.vestingSchedules[input.vestingId];
    
    if (qpi.invocator() != schedule.beneficiary && 
        qpi.invocator() != state.owner) {
        output.success = false;
        output.released = 0;
        return;
    }
    
    if (schedule.revoked) {
        output.success = false;
        output.released = 0;
        return;
    }
    
    uint64 currentEpoch = qpi.epoch();
    uint64 vestedAmount = calculateVestedAmount(input.vestingId, currentEpoch);
    
    uint64 releasable;
    if (!safeSub(vestedAmount, schedule.releasedAmount, releasable)) {
        output.success = false;
        output.released = 0;
        return;
    }
    
    if (releasable == 0) {
        output.success = false;
        output.released = 0;
        return;
    }
    
    if (!addBalance(schedule.beneficiary, releasable)) {
        output.success = false;
        output.released = 0;
        return;
    }
    
    schedule.releasedAmount += releasable;
    output.success = true;
    output.released = releasable;
} _

PUBLIC_FUNCTION(GetVestingInfo) {
    if (input.vestingId >= state.numVestings) {
        id nullId;
        output.beneficiary = nullId;
        output.totalAmount = 0;
        output.releasedAmount = 0;
        output.vestedAmount = 0;
        output.revoked = false;
        output.isAirdropVesting = false;
        return;
    }
    
    VestingSchedule& schedule = state.vestingSchedules[input.vestingId];
    uint64 currentEpoch = qpi.epoch();
    
    output.beneficiary = schedule.beneficiary;
    output.totalAmount = schedule.totalAmount;
    output.releasedAmount = schedule.releasedAmount;
    output.vestedAmount = calculateVestedAmount(input.vestingId, currentEpoch);
    output.revoked = schedule.revoked;
    output.isAirdropVesting = schedule.isAirdropVesting;
} _

PUBLIC_FUNCTION(GetUserVestings) {
    uint32 count = 0;
    for (uint32 i = 0; i < state.numVestings && count < 50; i++) {
        if (state.vestingSchedules[i].beneficiary == input.user) {
            output.vestingIds[count] = i;
            count++;
        }
    }
    output.numVestings = count;
} _

PUBLIC_PROCEDURE(RevokeVesting) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    if (input.vestingId >= state.numVestings) {
        output.success = false;
        return;
    }
    
    VestingSchedule& schedule = state.vestingSchedules[input.vestingId];
    
    if (schedule.revoked) {
        output.success = false;
        return;
    }
    
    uint64 currentEpoch = qpi.epoch();
    uint64 vestedAmount = calculateVestedAmount(input.vestingId, currentEpoch);
    
    uint64 releasable;
    if (safeSub(vestedAmount, schedule.releasedAmount, releasable) && releasable > 0) {
        addBalance(schedule.beneficiary, releasable);
        schedule.releasedAmount += releasable;
    }
    
    uint64 remaining;
    if (safeSub(schedule.totalAmount, schedule.releasedAmount, remaining) && remaining > 0) {
        addBalance(state.owner, remaining);
    }
    
    schedule.revoked = true;
    output.success = true;
} _

// ==================== WARRIOR FUNCTIONS ====================

PUBLIC_PROCEDURE(AddWarriorPoints) {
    if (qpi.invocator() != state.owner && !isWhitelisted(qpi.invocator())) {
        output.success = false;
        return;
    }
    
    sint32 idx = -1;
    for (uint32 i = 0; i < state.numWarriors; i++) {
        if (state.warriors[i].warrior == input.warrior) {
            idx = i;
            break;
        }
    }
    
    if (idx < 0) {
        if (state.numWarriors >= MAX_WARRIORS) {
            output.success = false;
            return;
        }
        idx = state.numWarriors;
        state.warriors[idx].warrior = input.warrior;
        state.warriors[idx].points = 0;
        state.warriors[idx].totalRewarded = 0;
        state.numWarriors++;
    }
    
    state.warriors[idx].points += input.points;
    output.success = true;
} _

PUBLIC_PROCEDURE(RewardWarrior) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        output.rewarded = 0;
        return;
    }
    
    uint64 newUsed = state.warriorsPoolUsed + input.rewardAmount;
    if (newUsed > WARRIORS_POOL) {
        output.success = false;
        output.rewarded = 0;
        return;
    }
    
    if (!addBalance(input.warrior, input.rewardAmount)) {
        output.success = false;
        output.rewarded = 0;
        return;
    }
    
    state.warriorsPoolUsed = newUsed;
    
    for (uint32 i = 0; i < state.numWarriors; i++) {
        if (state.warriors[i].warrior == input.warrior) {
            state.warriors[i].totalRewarded += input.rewardAmount;
            break;
        }
    }
    
    output.success = true;
    output.rewarded = input.rewardAmount;
} _

PUBLIC_FUNCTION(GetWarriorInfo) {
    for (uint32 i = 0; i < state.numWarriors; i++) {
        if (state.warriors[i].warrior == input.warrior) {
            output.points = state.warriors[i].points;
            output.totalRewarded = state.warriors[i].totalRewarded;
            output.exists = true;
            return;
        }
    }
    output.points = 0;
    output.totalRewarded = 0;
    output.exists = false;
} _

// ==================== NFT BADGE FUNCTIONS (SIMPLIFIÉ) ====================

PUBLIC_PROCEDURE(InitBadgeType) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        return;
    }
    
    if (state.numBadgeTypes >= MAX_BADGE_TYPES) {
        output.success = false;
        return;
    }
    
    state.badgeMetadata[state.numBadgeTypes].badgeType = input.badgeType;
    state.badgeMetadata[state.numBadgeTypes].category = input.category;
    state.badgeMetadata[state.numBadgeTypes].multiplier = input.multiplier;
    state.badgeMetadata[state.numBadgeTypes].requiredPoints = input.requiredPoints;
    
    state.numBadgeTypes++;
    output.success = true;
} _

PUBLIC_PROCEDURE(CreateMission) {
    if (qpi.invocator() != state.owner) {
        output.success = false;
        output.missionId = 0;
        return;
    }
    
    if (state.numMissions >= MAX_MISSIONS) {
        output.success = false;
        output.missionId = 0;
        return;
    }
    
    uint32 missionId = state.numMissions;
    Mission& mission = state.missions[missionId];
    
    mission.missionId = missionId;
    mission.missionType = input.missionType;
    mission.requiredCat = input.requiredCat;
    mission.requirement = input.requirement;
    mission.rewardAmount = input.rewardAmount;
    mission.badgeReward = input.badgeReward;
    mission.badgeCategory = input.badgeCategory;
    mission.active = true;
    mission.completions = 0;
    
    state.numMissions++;
    output.success = true;
    output.missionId = missionId;
} _

PUBLIC_PROCEDURE(MintBadge) {
    if (qpi.invocator() != state.owner && !isWhitelisted(qpi.invocator())) {
        output.success = false;
        output.badgeId = 0;
        return;
    }
    
    if (state.numBadges >= MAX_BADGES) {
        output.success = false;
        output.badgeId = 0;
        return;
    }
    
    if (isNullId(input.recipient)) {
        output.success = false;
        output.badgeId = 0;
        return;
    }
    
    uint32 badgeId = state.numBadges;
    Badge& badge = state.badges[badgeId];
    
    badge.badgeId = badgeId;
    badge.badgeType = input.badgeType;
    badge.category = input.category;
    badge.catAlignment = input.catAlignment;
    badge.owner = input.recipient;
    badge.mintEpoch = qpi.epoch();
    badge.missionId = input.missionId;
    badge.soulbound = input.soulbound;
    
    sint32 holderIdx = findHolder(input.recipient);
    if (holderIdx >= 0) {
        state.badgesPerAddress[holderIdx]++;
    }
    
    state.numBadges++;
    state.totalBadgesMinted++;
    
    output.success = true;
    output.badgeId = badgeId;
} _

PUBLIC_PROCEDURE(CompleteMission) {
    if (qpi.invocator() != state.owner && !isWhitelisted(qpi.invocator())) {
        output.success = false;
        output.badgeId = 0;
        output.rewardAmount = 0;
        return;
    }
    
    if (input.missionId >= state.numMissions) {
        output.success = false;
        output.badgeId = 0;
        output.rewardAmount = 0;
        return;
    }
    
    Mission& mission = state.missions[input.missionId];
    
    if (!mission.active) {
        output.success = false;
        output.badgeId = 0;
        output.rewardAmount = 0;
        return;
    }
    
    sint32 holderIdx = findHolder(input.warrior);
    if (holderIdx < 0) {
        output.success = false;
        output.badgeId = 0;
        output.rewardAmount = 0;
        return;
    }
    
    if (state.userMissionCompleted[holderIdx][input.missionId]) {
        output.success = false;
        output.badgeId = 0;
        output.rewardAmount = 0;
        return;
    }
    
    if (input.proofValue < mission.requirement) {
        output.success = false;
        output.badgeId = 0;
        output.rewardAmount = 0;
        return;
    }
    
    state.userMissionCompleted[holderIdx][input.missionId] = true;
    mission.completions++;
    state.totalMissionsCompleted++;
    
    uint32 badgeId = 0;
    if (state.numBadges < MAX_BADGES) {
        badgeId = state.numBadges;
        Badge& badge = state.badges[badgeId];
        
        badge.badgeId = badgeId;
        badge.badgeType = mission.badgeReward;
        badge.category = mission.badgeCategory;
        badge.catAlignment = mission.requiredCat;
        badge.owner = input.warrior;
        badge.mintEpoch = qpi.epoch();
        badge.missionId = input.missionId;
        badge.soulbound = true;
        
        state.badgesPerAddress[holderIdx]++;
        state.numBadges++;
        state.totalBadgesMinted++;
    }
    
    uint64 rewardAmount = mission.rewardAmount;
    
    uint64 totalMultiplier = 0;
    for (uint32 i = 0; i < state.numBadges; i++) {
        if (state.badges[i].owner == input.warrior) {
            for (uint32 j = 0; j < state.numBadgeTypes; j++) {
                if (state.badgeMetadata[j].badgeType == state.badges[i].badgeType &&
                    state.badgeMetadata[j].category == state.badges[i].category) {
                    totalMultiplier += state.badgeMetadata[j].multiplier;
                    break;
                }
            }
        }
    }
    
    if (totalMultiplier > 0) {
        uint64 bonus = (rewardAmount * totalMultiplier) / 10000;
        rewardAmount += bonus;
    }
    
    if (rewardAmount > 0) {
        addBalance(input.warrior, rewardAmount);
    }
    
    output.success = true;
    output.badgeId = badgeId;
    output.rewardAmount = rewardAmount;
} _

PUBLIC_PROCEDURE(TransferBadge) {
    if (input.badgeId >= state.numBadges) {
        output.success = false;
        return;
    }
    
    Badge& badge = state.badges[input.badgeId];
    
    if (badge.owner != qpi.invocator()) {
        output.success = false;
        return;
    }
    
    if (badge.soulbound) {
        output.success = false;
        return;
    }
    
    if (isNullId(input.to)) {
        output.success = false;
        return;
    }
    
    sint32 fromIdx = findHolder(badge.owner);
    sint32 toIdx = findHolder(input.to);
    
    if (fromIdx >= 0) {
        state.badgesPerAddress[fromIdx]--;
    }
    if (toIdx >= 0) {
        state.badgesPerAddress[toIdx]++;
    }
    
    badge.owner = input.to;
    output.success = true;
} _

PUBLIC_FUNCTION(GetBadgeInfo) {
    if (input.badgeId >= state.numBadges) {
        output.exists = false;
        return;
    }
    
    Badge& badge = state.badges[input.badgeId];
    output.exists = true;
    output.badgeType = badge.badgeType;
    output.category = badge.category;
    output.catAlignment = badge.catAlignment;
    output.owner = badge.owner;
    output.mintEpoch = badge.mintEpoch;
    output.missionId = badge.missionId;
    output.soulbound = badge.soulbound;
} _

PUBLIC_FUNCTION(GetUserBadges) {
    uint32 count = 0;
    for (uint32 i = 0; i < state.numBadges && count < 100; i++) {
        if (state.badges[i].owner == input.user) {
            output.badgeIds[count] = state.badges[i].badgeId;
            count++;
        }
    }
    output.numBadges = count;
} _

PUBLIC_FUNCTION(GetMissionInfo) {
    if (input.missionId >= state.numMissions) {
        output.exists = false;
        return;
    }
    
    Mission& mission = state.missions[input.missionId];
    output.exists = true;
    output.missionType = mission.missionType;
    output.requiredCat = mission.requiredCat;
    output.requirement = mission.requirement;
    output.rewardAmount = mission.rewardAmount;
    output.badgeReward = mission.badgeReward;
    output.active = mission.active;
    output.completions = mission.completions;
} _

PUBLIC_FUNCTION(CalculateUserMultiplier) {
    uint16 totalMultiplier = 0;
    uint32 badgeCount = 0;
    
    for (uint32 i = 0; i < state.numBadges; i++) {
        if (state.badges[i].owner == input.user) {
            badgeCount++;
            for (uint32 j = 0; j < state.numBadgeTypes; j++) {
                if (state.badgeMetadata[j].badgeType == state.badges[i].badgeType &&
                    state.badgeMetadata[j].category == state.badges[i].category) {
                    totalMultiplier += state.badgeMetadata[j].multiplier;
                    break;
                }
            }
        }
    }
    
    output.totalMultiplier = totalMultiplier;
    output.numBadges = badgeCount;
} _

PUBLIC_FUNCTION(CheckBadgeCombo) {
    bool hasNoisette = false;
    bool hasCacahuete = false;
    bool hasChipie = false;
    
    for (uint32 i = 0; i < state.numBadges; i++) {
        if (state.badges[i].owner == input.user) {
            if (state.badges[i].catAlignment == 1) hasNoisette = true;
            if (state.badges[i].catAlignment == 2) hasCacahuete = true;
            if (state.badges[i].catAlignment == 3) hasChipie = true;
        }
    }
    
    if (hasNoisette && hasCacahuete && hasChipie) {
        output.hasCombo = true;
        output.comboType = 1;
        output.bonusReward = 1000000;
    } else {
        output.hasCombo = false;
        output.comboType = 0;
        output.bonusReward = 0;
    }
} _

PUBLIC_FUNCTION(GetBadgeStats) {
    output.totalBadges = state.numBadges;
    output.totalMissions = state.numMissions;
    output.totalMissionsCompleted = state.totalMissionsCompleted;
    
    uint32 active = 0;
    for (uint32 i = 0; i < state.numMissions; i++) {
        if (state.missions[i].active) active++;
    }
    output.activeMissions = active;
    
    uint32 bronze = 0, silver = 0, gold = 0, diamond = 0, legendary = 0;
    for (uint32 i = 0; i < state.numBadges; i++) {
        switch(state.badges[i].badgeType) {
            case 1: bronze++; break;
            case 2: silver++; break;
            case 3: gold++; break;
            case 4: diamond++; break;
            case 5: legendary++; break;
        }
    }
    
    output.bronzeBadges = bronze;
    output.silverBadges = silver;
    output.goldBadges = gold;
    output.diamondBadges = diamond;
    output.legendaryBadges = legendary;
} _

// ==================== REGISTRATION ====================

REGISTER_USER_FUNCTIONS_AND_PROCEDURES {
    REGISTER_USER_PROCEDURE(Mint, 1);
    REGISTER_USER_PROCEDURE(Transfer, 2);
    REGISTER_USER_PROCEDURE(InstantAirdrop, 3);
    REGISTER_USER_PROCEDURE(Burn, 4);
    REGISTER_USER_PROCEDURE(Pause, 5);
    REGISTER_USER_PROCEDURE(Unpause, 6);
    REGISTER_USER_PROCEDURE(AddToWhitelist, 7);
    REGISTER_USER_PROCEDURE(RemoveFromWhitelist, 8);
    REGISTER_USER_PROCEDURE(TransferOwnership, 9);
    REGISTER_USER_PROCEDURE(CreateVesting, 10);
    REGISTER_USER_PROCEDURE(ReleaseVesting, 11);
    REGISTER_USER_PROCEDURE(RevokeVesting, 12);
    REGISTER_USER_PROCEDURE(AirdropWithVesting, 13);
    REGISTER_USER_PROCEDURE(VerifyAndBonus, 14);
    REGISTER_USER_PROCEDURE(AddWarriorPoints, 15);
    REGISTER_USER_PROCEDURE(RewardWarrior, 16);
    REGISTER_USER_PROCEDURE(InitBadgeType, 17);
    REGISTER_USER_PROCEDURE(CreateMission, 18);
    REGISTER_USER_PROCEDURE(MintBadge, 19);
    REGISTER_USER_PROCEDURE(CompleteMission, 20);
    REGISTER_USER_PROCEDURE(TransferBadge, 21);
    REGISTER_USER_PROCEDURE(SetTaxWallet, 22);
    REGISTER_USER_PROCEDURE(SetTaxEnabled, 23);
    REGISTER_USER_PROCEDURE(LockLiquidity, 24);
    REGISTER_USER_PROCEDURE(UnlockLiquidity, 25);
    
    REGISTER_USER_FUNCTION(Balance, 1);
    REGISTER_USER_FUNCTION(TotalSupply, 2);
    REGISTER_USER_FUNCTION(IsPaused, 3);
    REGISTER_USER_FUNCTION(IsWhitelisted, 4);
    REGISTER_USER_FUNCTION(GetOwner, 5);
    REGISTER_USER_FUNCTION(GetVestingInfo, 6);
    REGISTER_USER_FUNCTION(GetCatStats, 7);
    REGISTER_USER_FUNCTION(GetWarriorInfo, 8);
    REGISTER_USER_FUNCTION(GetBadgeInfo, 9);
    REGISTER_USER_FUNCTION(GetUserBadges, 10);
    REGISTER_USER_FUNCTION(GetMissionInfo, 11);
    REGISTER_USER_FUNCTION(CalculateUserMultiplier, 12);
    REGISTER_USER_FUNCTION(CheckBadgeCombo, 13);
    REGISTER_USER_FUNCTION(GetBadgeStats, 14);
    REGISTER_USER_FUNCTION(GetTokenomics, 15);
    REGISTER_USER_FUNCTION(GetTaxConfig, 16);
    REGISTER_USER_FUNCTION(GetLiquidityLock, 17);
    REGISTER_USER_FUNCTION(GetUserVestings, 18);
} _

// ==================== INITIALIZE ====================

INITIALIZE {
    if (!state.initialized) {
        state.totalSupply = 0;
        state.numHolders = 0;
        state.paused = false;
        state.owner = qpi.originator();
        state.taxWallet = qpi.originator();
        state.taxEnabled = true;
        state.numWhitelisted = 0;
        state.numVestings = 0;
        state.totalBurned = 0;
        state.totalAirdropped = 0;
        state.totalTransfers = 0;
        state.totalTaxCollected = 0;
        
        // Reserves tracking
        state.airdropReserveUsed = 0;
        state.marketingReserveUsed = 0;
        state.spaReserveUsed = 0;
        state.liquidityPoolUsed = 0;
        state.teamVestingUsed = 0;
        state.warriorsPoolUsed = 0;
        
        // Cat reserves
        state.noisetteDistributed = 0;
        state.cacahueteDistributed = 0;
        state.chipieDistributed = 0;
        state.numVerifiedAirdrops = 0;
        
        // Warriors
        state.numWarriors = 0;
        
        // NFT Badges
        state.numBadges = 0;
        state.numMissions = 0;
        state.numBadgeTypes = 0;
        state.totalBadgesMinted = 0;
        state.totalMissionsCompleted = 0;
        
        // Liquidity locks
        state.numLiquidityLocks = 0;
        
        // Distribution initiale : Mint le creator supply
        addBalance(state.owner, CREATOR_SUPPLY);
        state.totalSupply = CREATOR_SUPPLY;
        
        state.initialized = true;
    }
} _
