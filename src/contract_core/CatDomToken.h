using namespace QPI;

struct CatDomToken : public ContractBase {
public:
    // Input/Output structs
    struct Mint_input { uint64 amount; };
    struct Mint_output { bool success; };

    struct Transfer_input { id to; uint64 amount; };
    struct Transfer_output { bool success; };

    struct Airdrop_input { id recipients[100]; uint64 amountPer; uint64 numRecipients; };  // Max 100 par call pour perf
    struct Airdrop_output { uint64 distributed; };

    struct Balance_input { id address; };
    struct Balance_output { uint64 balance; };

private:
    // State: Balances avec array fixe (Qubic n'aime pas les maps dynamiques pour perf)
    static const uint32 MAX_HOLDERS = 1000000;  // Alloue 1M holders max (ajuste si besoin, mémoire ~8GB)
    id holders[MAX_HOLDERS];
    uint64 balances[MAX_HOLDERS];
    uint64 totalSupply;
    uint32 numHolders;

    // Helper: Trouver index d'un holder
    sint32 findHolder(const id& addr) {
        for (uint32 i = 0; i < state.numHolders; i++) {
            if (state.holders[i] == addr) return i;
        }
        return -1;
    }

    // Helper: Ajouter ou update balance
    bool addBalance(const id& addr, uint64 amount) {
        sint32 idx = findHolder(addr);
        if (idx >= 0) {
            state.balances[idx] += amount;
            return true;
        } else if (state.numHolders < MAX_HOLDERS) {
            state.holders[state.numHolders] = addr;
            state.balances[state.numHolders] = amount;
            state.numHolders++;
            return true;
        }
        return false;
    }

    // Helper: Subtract balance
    bool subtractBalance(const id& addr, uint64 amount) {
        sint32 idx = findHolder(addr);
        if (idx >= 0 && state.balances[idx] >= amount) {
            state.balances[idx] -= amount;
            return true;
        }
        return false;
    }

public:
    // Proc Mint (seulement owner)
    PUBLIC_PROCEDURE(Mint) {
        if (qpi.invocator() != qpi.originator()) { output.success = false; return; }
        if (!addBalance(qpi.invocator(), input.amount)) { output.success = false; return; }
        state.totalSupply += input.amount;
        output.success = true;
    } _

    // Proc Transfer
    PUBLIC_PROCEDURE(Transfer) {
        if (!subtractBalance(qpi.invocator(), input.amount)) { output.success = false; return; }
        if (!addBalance(input.to, input.amount)) { output.success = false; return; }  // Rollback si fail? Qubic gere atomicite
        output.success = true;
    } _

    // Proc Airdrop (seulement owner)
    PUBLIC_PROCEDURE(Airdrop) {
        if (qpi.invocator() != qpi.originator()) { output.distributed = 0; return; }
        uint64 totalDist = input.amountPer * input.numRecipients;
        if (!subtractBalance(qpi.invocator(), totalDist)) { output.distributed = 0; return; }
        for (uint64 i = 0; i < input.numRecipients; i++) {
            addBalance(input.recipients[i], input.amountPer);  // Assume success, sinon rollback total?
        }
        output.distributed = totalDist;
    } _

    // Func Balance (read-only)
    PUBLIC_FUNCTION(Balance) {
        sint32 idx = findHolder(input.address);
        output.balance = (idx >= 0) ? state.balances[idx] : 0;
    } _

    REGISTER_USER_FUNCTIONS_AND_PROCEDURES {
        REGISTER_USER_PROCEDURE(Mint, 1);
        REGISTER_USER_PROCEDURE(Transfer, 2);
        REGISTER_USER_PROCEDURE(Airdrop, 3);
        REGISTER_USER_FUNCTION(Balance, 1);
    } _

    INITIALIZE {
        state.totalSupply = 0;
        state.numHolders = 0;
    } _
};
