#pragma once
#include <bitset>
#include <unordered_map>

using ComponentType                 = uint8_t;
const ComponentType MAX_COMPONENTS  = 32;
using Signature                     = std::bitset<MAX_COMPONENTS>;


class SignatureManager{
    private:
        std::unordered_map<const char *,ComponentType> mComponentToBitMap;
    public:
        void addComponentToSignature(const ComponentType type, Signature &signature){
            signature.set(type);
        }
        void removeComponentToSignature(const ComponentType type, Signature &signature){
            signature.reset(type);
        }
};