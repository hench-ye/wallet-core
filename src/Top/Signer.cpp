// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../HexCoding.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Top;

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Transaction(
        /* from: */ static_cast<Data>(data(input.from())),
        /* to: */ static_cast<Data>(data(input.to())),
        /* amount: */ static_cast<uint64_t>(input.amount()),
        /* tx_deposit: */ static_cast<uint32_t>(input.tx_deposit()),
        /* last_tx_nonce: */ static_cast<uint64_t>(input.last_tx_nonce()),
        /* last_tx_hash: */ static_cast<Data>(data(input.last_tx_hash())),
        /* token */ static_cast<Data>(data(input.token())),
        /* note: */ static_cast<Data>(data(input.note())));
    Signer::sign(key, transaction);

    auto output = Proto::SigningOutput();
    auto encoded = transaction.encode();
    output.set_encoded(encoded.data(), encoded.size());
    output.set_signature(transaction.signature.data(), transaction.signature.size());
    return output;
}
void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) noexcept {
    Data encoded = transaction.encode();
    Data hashData = Hash::sha256(encoded.data(), encoded.size());
    // std::cout << "hash: " << hex(hashData)<<std::endl;
    Data hashSignature = privateKey.sign(hashData, TWCurveSECP256k1);
    // std::cout << "signature: " << hex(hashSignature)<<std::endl;
//    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended).bytes;

//    Data result(publicKeyData.begin(), publicKeyData.end());
//    result.insert(result.end(), hashSignature.begin(), hashSignature.end());
    hashSignature.insert(hashSignature.begin(), hashSignature.back());
    hashSignature.pop_back();
    transaction.signature = hashSignature;
}