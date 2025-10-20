using namespace QPI;

struct CatDomToken : public ContractBase {
public:
    // ==================== INPUT/OUTPUT STRUCTS ====================
    
    struct Mint_input { uint64 amount; };
    struct Mint_output { bool success; };

    struct Transfer_input { id to; uint64 amount; };
    struct Transfer_output { bool success; };

    struct Burn_input { uint64 amount; };
    struct Burn_output { bool success; };

    struct Airdrop_input { 
        id recipients[100]; 
        uint64 amountPer; 
        uint64 numRecipients; 
    };
    struct Airdrop_output { uint64 distributed; };

    struct Balance_input { id address; };
    struct Balance_output { uint64 balance; };

    struct TotalSupply_input { };
    struct TotalSupply_output { uint64 supply; };

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

    struct VestingSchedule {
        id beneficiary;
        uint64 totalAmount;
        uint64 releasedAmount;
        uint64 startEpoch;
        uint64 cliffEpoch;
        uint64 endEpoch;
        bool revoked;
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
    };

    struct RevokeVesting_input { uint32 vestingId; };
    struct RevokeVesting_output { bool success; };

    // Nouveaux structs pour le mécanisme de charity
    struct AddCharityWallet_input { id wallet; };
    struct AddCharityWallet_output { bool success; };

    struct GetCharityWallet_input { uint32 index; };
    struct GetCharityWallet_output { id wallet; };

private:
    // ==================== CONSTANTES ====================
    static const uint32 MAX_HOLDERS = 1000000;
    static const uint64 MAX_SUPPLY = 1000000000000000000ULL;         // 1 Quintillion pour prix bas 
    static const uint64 INITIAL_SUPPLY = 100000000000000000ULL;      // 100 Quadrillion (10%)
    static const uint64 AIRDROP_RESERVE = 250000000000000000ULL;     // 250 Quadrillion (25%, ajusté pour communauté)
    static const uint64 ECOSYSTEM_RESERVE = 200000000000000000ULL;   // 200 Quadrillion (20%, pour liquidity/dev)
    static const uint64 TEAM_RESERVE = 200000000000000000ULL;        // 200 Quadrillion (20%, vested)
    static const uint64 MARKETING_RESERVE = 100000000000000000ULL;   // 100 Quadrillion (10%)
    static const uint64 CHARITY_RESERVE = 50000000000000000ULL;      // 50 Quadrillion (5%, initial reserve)
    static const uint64 BURN_INITIAL = 100000000000000000ULL;        // 100 Quadrillion (10%, burned au launch pour deflation)
    
    static const uint32 MAX_WHITELIST = 10000;
    static const uint32 MAX_VESTING = 1000;
    static const uint64 MIN_TRANSFER = 1;                            // Minimum 1 token
    static const uint64 MAX_AIRDROP_RECIPIENTS = 100;
    static const uint64 TRANSFER_FEE_PERCENT = 2;                    // 2% fee sur transfers pour générer bénéfices
    static const uint64 CHARITY_FEE_SHARE = 10;                      // 10% des fees (bénéfices) à charity
    static const uint64 BURN_FEE_SHARE = 50;                         // 50% des fees burned
    static const uint64 TREASURY_FEE_SHARE = 40;                     // 40% des fees au treasury
    static const uint32 MAX_CHARITY_WALLETS = 10;                    // Max 10 wallets charity gérés par DAO/governance
    
    // ==================== STATE VARIABLES ====================
    id holders[MAX_HOLDERS];
    uint64 balances[MAX_HOLDERS];
    uint64 totalSupply;
    uint32 numHolders;
    bool initialized;
    bool paused;
    id owner;
    
    // Whitelist
    id whitelist[MAX_WHITELIST];
    uint32 numWhitelisted;
    
    // Vesting
    VestingSchedule vestingSchedules[MAX_VESTING];
    uint32 numVestings;
    
    // Charity wallets (pour distribution auto des 10% bénéfices)
    id charityWallets[MAX_CHARITY_WALLETS];
    uint32 numCharityWallets;
    
    // Treasury wallet (pour les fees restantes)
    id treasuryWallet;
    
    // Statistics
    uint64 totalBurned;
    uint64 totalAirdropped;
    uint64 totalTransfers;
    uint64 totalCharityDonated;

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

    bool safeDiv(uint64 a, uint64 b, uint64& result) {
        if (b == 0) return false;
        result = a / b;
        return true;
    }

    sint32 findHolder(const id& addr) {
        for (uint32 i = 0; i < state.numHolders; i++) {
            if (state.holders[i] == addr) return i;
        }
        return -1;
    }

    bool isWhitelisted(const id& addr) {
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

    // Nouvelle helper pour distribuer fees (bénéfices) pendant transfer
    bool distributeFees(uint64 feeAmount) {
        if (feeAmount == 0) return true;
        
        // Calculer parts
        uint64 charityPart;
        uint64 burnPart;
        uint64 treasuryPart;
        
        if (!safeMul(feeAmount, CHARITY_FEE_SHARE, charityPart) ||
            !safeDiv(charityPart, 100, charityPart)) return false;
        
        if (!safeMul(feeAmount, BURN_FEE_SHARE, burnPart) ||
            !safeDiv(burnPart, 100, burnPart)) return false;
        
        if (!safeMul(feeAmount, TREASURY_FEE_SHARE, treasuryPart) ||
            !safeDiv(treasuryPart, 100, treasuryPart)) return false;
        
        // Vérifier somme (devrait être ~feeAmount, ignore arrondis)
        uint64 totalParts;
        if (!safeAdd(charityPart, burnPart, totalParts) ||
            !safeAdd(totalParts, treasuryPart, totalParts) ||
            totalParts > feeAmount) return false;
        
        // Burn
        uint64 newSupply;
        if (!safeSub(state.totalSupply, burnPart, newSupply)) return false;
        state.totalSupply = newSupply;
        state.totalBurned += burnPart;
        
        // Treasury
        if (!addBalance(state.treasuryWallet, treasuryPart)) return false;
        
        // Charity: Distribuer équitablement aux wallets charity (si multiples)
        if (state.numCharityWallets > 0) {
            uint64 perCharity;
            if (!safeDiv(charityPart, state.numCharityWallets, perCharity)) return false;
            
            for (uint32 i = 0; i < state.numCharityWallets; i++) {
                if (!addBalance(state.charityWallets[i], perCharity)) return false;
            }
            state.totalCharityDonated += charityPart;
        } else {
            // Si pas de charity wallets, ajouter au treasury (fallback)
            if (!addBalance(state.treasuryWallet, charityPart)) return false;
        }
        
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
            output.success = true; // Déjà whitelisté
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
        // Déplacer le dernier élément à la place de celui supprimé
        state.whitelist[idx] = state.whitelist[state.numWhitelisted - 1];
        state.numWhitelisted--;
        output.success = true;
    } _

    PUBLIC_FUNCTION(IsWhitelisted) {
        output.whitelisted = isWhitelisted(input.address);
    } _

    // ==================== CHARITY FUNCTIONS ====================
    
    PUBLIC_PROCEDURE(AddCharityWallet) {
        if (qpi.invocator() != state.owner) {  // À terme, via DAO/governance
            output.success = false;
            return;
        }
        if (isNullId(input.wallet)) {
            output.success = false;
            return;
        }
        if (state.numCharityWallets >= MAX_CHARITY_WALLETS) {
            output.success = false;
            return;
        }
        // Vérifier si déjà ajouté (optionnel, mais évite doublons)
        for (uint32 i = 0; i < state.numCharityWallets; i++) {
            if (state.charityWallets[i] == input.wallet) {
                output.success = true;
                return;
            }
        }
        state.charityWallets[state.numCharityWallets] = input.wallet;
        state.numCharityWallets++;
        output.success = true;
    } _

    PUBLIC_FUNCTION(GetCharityWallet) {
        if (input.index >= state.numCharityWallets) {
            id nullId;
            output.wallet = nullId;
            return;
        }
        output.wallet = state.charityWallets[input.index];
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
            return;
        }
        if (input.amount < MIN_TRANSFER) {
            output.success = false;
            return;
        }
        if (qpi.invocator() == input.to) {
            output.success = false;
            return;
        }
        if (isNullId(input.to)) {
            output.success = false;
            return;
        }
        
        // Calculer fee (2% de amount)
        uint64 fee;
        if (!safeMul(input.amount, TRANSFER_FEE_PERCENT, fee) ||
            !safeDiv(fee, 100, fee)) {
            output.success = false;
            return;
        }
        
        uint64 netAmount;
        if (!safeSub(input.amount, fee, netAmount)) {
            output.success = false;
            return;
        }
        
        if (!subtractBalance(qpi.invocator(), input.amount)) {
            output.success = false;
            return;
        }
        
        // Distribuer fees (inclut 10% à charity)
        if (!distributeFees(fee)) {
            addBalance(qpi.invocator(), input.amount); // Rollback
            output.success = false;
            return;
        }
        
        // Ajouter net à recipient
        if (!addBalance(input.to, netAmount)) {
            // Rollback complexe, mais pour simplicité, assume ok (ou implémente full rollback)
            output.success = false;
            return;
        }
        
        state.totalTransfers++;
        output.success = true;
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
            addBalance(qpi.invocator(), input.amount); // Rollback
            output.success = false;
            return;
        }
        
        state.totalSupply = newSupply;
        state.totalBurned += input.amount;
        output.success = true;
    } _

    PUBLIC_PROCEDURE(Airdrop) {
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

    PUBLIC_FUNCTION(Balance) {
        output.balance = getBalance(input.address);
    } _

    PUBLIC_FUNCTION(TotalSupply) {
        output.supply = state.totalSupply;
    } _

    // ==================== VESTING FUNCTIONS ====================
    
    PUBLIC_PROCEDURE(CreateVesting) {
        if (qpi.invocator() != state.owner) {
            output.success = false;
            output.vestingId = 0;
            return;
        }
        if (state.numVestings >= MAX_VESTING) {
            output.success = false;
            output.vestingId = 0;
            return;
        }
        if (isNullId(input.beneficiary) || input.totalAmount == 0 || 
            input.durationEpochs == 0) {
            output.success = false;
            output.vestingId = 0;
            return;
        }
        
        // Vérifier que le owner a assez de tokens
        if (getBalance(state.owner) < input.totalAmount) {
            output.success = false;
            output.vestingId = 0;
            return;
        }
        
        // Créer le vesting
        uint32 vestingId = state.numVestings;
        VestingSchedule& schedule = state.vestingSchedules[vestingId];
        
        schedule.beneficiary = input.beneficiary;
        schedule.totalAmount = input.totalAmount;
        schedule.releasedAmount = 0;
        schedule.startEpoch = input.startEpoch;
        schedule.cliffEpoch = input.cliffEpoch;
        schedule.endEpoch = input.startEpoch + input.durationEpochs;
        schedule.revoked = false;
        
        // Bloquer les tokens
        if (!subtractBalance(state.owner, input.totalAmount)) {
            output.success = false;
            output.vestingId = 0;
            return;
        }
        
        state.numVestings++;
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
            return;
        }
        
        VestingSchedule& schedule = state.vestingSchedules[input.vestingId];
        uint64 currentEpoch = qpi.epoch();
        
        output.beneficiary = schedule.beneficiary;
        output.totalAmount = schedule.totalAmount;
        output.releasedAmount = schedule.releasedAmount;
        output.vestedAmount = calculateVestedAmount(input.vestingId, currentEpoch);
        output.revoked = schedule.revoked;
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
        
        // Libérer les tokens déjà vestés
        uint64 releasable;
        if (safeSub(vestedAmount, schedule.releasedAmount, releasable) && releasable > 0) {
            addBalance(schedule.beneficiary, releasable);
            schedule.releasedAmount += releasable;
        }
        
        // Rendre le reste au owner
        uint64 remaining;
        if (safeSub(schedule.totalAmount, schedule.releasedAmount, remaining) && remaining > 0) {
            addBalance(state.owner, remaining);
        }
        
        schedule.revoked = true;
        output.success = true;
    } _

    // ==================== REGISTRATION ====================
    
    REGISTER_USER_FUNCTIONS_AND_PROCEDURES {
        REGISTER_USER_PROCEDURE(Mint, 1);
        REGISTER_USER_PROCEDURE(Transfer, 2);
        REGISTER_USER_PROCEDURE(Airdrop, 3);
        REGISTER_USER_PROCEDURE(Burn, 4);
        REGISTER_USER_PROCEDURE(Pause, 5);
        REGISTER_USER_PROCEDURE(Unpause, 6);
        REGISTER_USER_PROCEDURE(AddToWhitelist, 7);
        REGISTER_USER_PROCEDURE(RemoveFromWhitelist, 8);
        REGISTER_USER_PROCEDURE(TransferOwnership, 9);
        REGISTER_USER_PROCEDURE(CreateVesting, 10);
        REGISTER_USER_PROCEDURE(ReleaseVesting, 11);
        REGISTER_USER_PROCEDURE(RevokeVesting, 12);
        REGISTER_USER_PROCEDURE(AddCharityWallet, 13);
        
        REGISTER_USER_FUNCTION(Balance, 1);
        REGISTER_USER_FUNCTION(TotalSupply, 2);
        REGISTER_USER_FUNCTION(IsPaused, 3);
        REGISTER_USER_FUNCTION(IsWhitelisted, 4);
        REGISTER_USER_FUNCTION(GetOwner, 5);
        REGISTER_USER_FUNCTION(GetVestingInfo, 6);
        REGISTER_USER_FUNCTION(GetCharityWallet, 7);
    } _

    // ==================== INITIALIZE ====================
    
    INITIALIZE {
        if (!state.initialized) {
            state.totalSupply = 0;
            state.numHolders = 0;
            state.paused = false;
            state.owner = qpi.originator();
            state.numWhitelisted = 0;
            state.numVestings = 0;
            state.numCharityWallets = 0;
            state.treasuryWallet = state.owner;  // Initialement owner, peut changer via governance
            state.totalBurned = 0;
            state.totalAirdropped = 0;
            state.totalTransfers = 0;
            state.totalCharityDonated = 0;
            state.initialized = true;
            
            // Mint initial allocations (ajusté pour 1 Quintillion total, prix bas ~0.0000001$)
            // Ex: Mint et allouer à owner/treasury, puis vesting/airdrop via functions
            // Pour simplicité, mint tout à owner au init, puis owner distribue
            uint64 initMint = INITIAL_SUPPLY + AIRDROP_RESERVE + ECOSYSTEM_RESERVE + 
                              TEAM_RESERVE + MARKETING_RESERVE + CHARITY_RESERVE;
            addBalance(state.owner, initMint);
            state.totalSupply += initMint;
            
            // Burn initial pour deflation
            subtractBalance(state.owner, BURN_INITIAL);
            state.totalSupply -= BURN_INITIAL;
            state.totalBurned += BURN_INITIAL;
        }
    } _
};
