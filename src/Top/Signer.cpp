// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../Ethereum/RLP.h"
#include "../HexCoding.h"
#include "../PublicKey.h"
#include "cJSON.h"
#include "Serial.h"

using namespace TW;
using namespace TW::Top;

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Transaction(
        static_cast<Data>(data(input.from())), static_cast<Data>(data(input.to())),
        static_cast<uint16_t>(input.tx_type()), static_cast<uint64_t>(input.amount()),
        static_cast<uint32_t>(input.extra()), static_cast<uint32_t>(input.tx_deposit()),
        static_cast<uint16_t>(input.source_action_type()),
        static_cast<uint16_t>(input.target_action_type()),
        static_cast<uint64_t>(input.last_tx_nonce()), static_cast<Data>(data(input.last_tx_hash())),
        static_cast<Data>(data(input.note())));
    //Signer::sign(key, transaction);
    Data sdata = transaction.serial_transaction();
    Data hashData = Hash::sha256(sdata.data(), sdata.size());
    Data hashSignature = privateKey.sign(hashData, TWCurveSECP256k1);
    hashSignature.insert(hashSignature.begin(), hashSignature.back());
    hashSignature.pop_back();
    transaction.signature = hashSignature;    
    transaction.hash = hashData;    

    auto output = Proto::SigningOutput();
    sdata = transaction.encode();
    output.set_encoded(sdata.data(), sdata.size());
    //output.set_hash(hashData, hashData.size());
    output.set_signature(transaction.signature.data(), transaction.signature.size());
    return output;
}
void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) noexcept {
    Data encoded = transaction.serial_transaction();
    Data hashData = Hash::sha256(encoded.data(), encoded.size());
//    std::cout << "hash: " << hex(hashData)<<std::endl;
    Data hashSignature = privateKey.sign(hashData, TWCurveSECP256k1);
//    std::cout << "signature: " << hex(hashSignature)<<std::endl;
//    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended).bytes;

//    Data result(publicKeyData.begin(), publicKeyData.end());
//    result.insert(result.end(), hashSignature.begin(), hashSignature.end());
    hashSignature.insert(hashSignature.begin(), hashSignature.back());
    hashSignature.pop_back();
    transaction.signature = hashSignature;
    transaction.hash = hashData;     
}

std::string Signer::signJSON(const std::string& json, const Data& key) noexcept {
    cJSON* pRoot = cJSON_CreateObject();
    if (pRoot == NULL)
        return "";
    pRoot = cJSON_Parse(json.c_str());
    cJSON* pBody = cJSON_CreateObject();
    pBody = cJSON_Parse(cJSON_GetObjectItem(pRoot, "body")->valuestring);
    cJSON_Delete(pRoot);
    if (pBody == NULL)
        return "";
    cJSON* pParams = cJSON_GetObjectItem(pBody, "params");
    if (pParams == NULL) {
        cJSON_Delete(pBody);
        return "";
    }

    std::string sdata;
    serial_data(sdata, (uint16_t)cJSON_GetObjectItem(pParams, "tx_type")->valueint);
    serial_data(sdata, (uint16_t)cJSON_GetObjectItem(pParams, "tx_len")->valueint);
    serial_data(sdata, (uint32_t)cJSON_GetObjectItem(pParams, "tx_structure_version")->valueint);
    serial_data(sdata, (uint16_t)cJSON_GetObjectItem(pParams, "to_ledger_id")->valueint);
    serial_data(sdata, (uint16_t)cJSON_GetObjectItem(pParams, "from_ledger_id")->valueint);
    serial_data(sdata, (uint32_t)cJSON_GetObjectItem(pParams, "tx_deposit")->valueint);
    serial_data(sdata, (uint16_t)cJSON_GetObjectItem(pParams, "tx_expire_duration")->valueint);
    serial_data(sdata, (uint64_t)cJSON_GetObjectItem(pParams, "send_timestamp")->valueint);
    serial_data(sdata, (uint32_t)cJSON_GetObjectItem(pParams, "tx_random_nonce")->valueint);
    serial_data(sdata, (uint32_t)cJSON_GetObjectItem(pParams, "premium_price")->valueint);
    serial_data(sdata, (uint64_t)cJSON_GetObjectItem(pParams, "last_tx_nonce")->valueint);
    std::string strtemp(cJSON_GetObjectItem(pParams, "last_tx_hash")->valuestring);
    serial_data(sdata, (uint64_t)hex_to_uint64(strtemp));
    serial_data(sdata, cJSON_GetObjectItem(pParams, "challenge_proof")->valuestring);
    serial_data(sdata, cJSON_GetObjectItem(pParams, "ext")->valuestring);
    serial_data(sdata, cJSON_GetObjectItem(pParams, "note")->valuestring);

    cJSON* pSender = cJSON_GetObjectItem(pParams, "sender_action");
    serial_data(sdata, (uint32_t)cJSON_GetObjectItem(pSender, "action_hash")->valueint);
    serial_data(sdata, (uint16_t)cJSON_GetObjectItem(pSender, "action_type")->valueint);
    serial_data(sdata, (uint16_t)cJSON_GetObjectItem(pSender, "action_size")->valueint);
    serial_data(sdata, cJSON_GetObjectItem(pSender, "tx_sender_account_addr")->valuestring);
    serial_data(sdata, cJSON_GetObjectItem(pSender, "action_name")->valuestring);
    serial_data_hex(sdata, cJSON_GetObjectItem(pSender, "action_param")->valuestring);
    serial_data(sdata, cJSON_GetObjectItem(pSender, "action_ext")->valuestring);
    serial_data(sdata, cJSON_GetObjectItem(pSender, "action_authorization")->valuestring);

    cJSON* pReceiver = cJSON_GetObjectItem(pParams, "receiver_action");
    serial_data(sdata, (uint32_t)cJSON_GetObjectItem(pReceiver, "action_hash")->valueint);
    serial_data(sdata, (uint16_t)cJSON_GetObjectItem(pReceiver, "action_type")->valueint);
    serial_data(sdata, (uint16_t)cJSON_GetObjectItem(pReceiver, "action_size")->valueint);
    serial_data(sdata, cJSON_GetObjectItem(pReceiver, "tx_receiver_account_addr")->valuestring);
    serial_data(sdata, cJSON_GetObjectItem(pReceiver, "action_name")->valuestring);
    serial_data_hex(sdata, cJSON_GetObjectItem(pReceiver, "action_param")->valuestring);
    serial_data(sdata, cJSON_GetObjectItem(pReceiver, "action_ext")->valuestring);
    serial_data(sdata, cJSON_GetObjectItem(pReceiver, "action_authorization")->valuestring);

    //std::cout << "data: " << hex(sdata) << std::endl;
    //std::cout << "key: " << hex(key) << std::endl;
    std::string tx_hash = cJSON_GetObjectItem(pParams, "tx_hash")->valuestring;
    cJSON_Delete(pBody);


    PrivateKey privateKey(key);
    Data hashData = Hash::sha256((uint8_t*)sdata.data(), sdata.size());
    Data hashSignature = privateKey.sign(hashData, TWCurveSECP256k1);
    //std::cout << "hashData: " << hex(std::string(hashData.begin(), hashData.end())) << std::endl;

    if (tx_hash.size() > 2 && tx_hash.substr(2) != hex(hashData)) {
//        std::cout << "hash check error." << std::endl;
//        return "";
    }

    hashSignature.insert(hashSignature.begin(), hashSignature.back());
    hashSignature.pop_back();
    //std::cout << "hashSignature: " << hex(std::string(hashSignature.begin(), hashSignature.end())) << std::endl;

    auto encoded = Data();
    append(encoded, Ethereum::RLP::encode(sdata));
    append(encoded, Ethereum::RLP::encode(hashData));
    append(encoded, Ethereum::RLP::encode(hashSignature));

    Data output = Ethereum::RLP::encodeList(encoded);
    return hex(output.begin(), output.end());
}
