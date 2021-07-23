// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "../Ethereum/RLP.h"
#include "cJSON.h"
#include "Serial.h"

using namespace TW;
using namespace TW::Top;

Data Transaction::encode() const noexcept {
    auto encoded = Data();
 /*   append(encoded, Ethereum::RLP::encode(from));
    append(encoded, Ethereum::RLP::encode(to));
    append(encoded, Ethereum::RLP::encode(tx_type));
    append(encoded, Ethereum::RLP::encode(amount));
    append(encoded, Ethereum::RLP::encode(extra));
    append(encoded, Ethereum::RLP::encode(tx_deposit));
    append(encoded, Ethereum::RLP::encode(source_action_type));
    append(encoded, Ethereum::RLP::encode(target_action_type));
    append(encoded, Ethereum::RLP::encode(last_tx_nonce));
    append(encoded, Ethereum::RLP::encode(m_last_tx_hash));
    append(encoded, Ethereum::RLP::encode(note));  */

    append(encoded, Ethereum::RLP::encode(serial_transaction()));
    if (!hash.empty()) {
        append(encoded, Ethereum::RLP::encode(hash));
    }

    if (!signature.empty()) {
        append(encoded, Ethereum::RLP::encode(signature));
    }
    return Ethereum::RLP::encodeList(encoded);
}

Data Transaction::serial_transaction() const noexcept {
    std::string sdata;
    serial_data(sdata, tx_type);
    serial_data(sdata, (uint16_t)0);
    serial_data(sdata, (uint32_t)0);
    serial_data(sdata, (uint16_t)0);
    serial_data(sdata, (uint16_t)0);
    serial_data(sdata, (uint32_t)tx_deposit);
    serial_data(sdata, (uint16_t)100);
    uint64_t t = time(NULL);
    std::cout <<"timestamp:" << t << std::endl;
    serial_data(sdata, (uint64_t)t);
    serial_data(sdata, (uint32_t)0);
    serial_data(sdata, (uint32_t)0);
    serial_data(sdata, (uint64_t)last_tx_nonce);
    std::string strtemp(m_last_tx_hash.begin(), m_last_tx_hash.end());
    serial_data(sdata, (uint64_t)hex_to_uint64(strtemp));
    serial_data(sdata, std::string(""));
    serial_data(sdata, std::string(""));
    serial_data(sdata, std::string(note.begin(), note.end()));

    serial_data(sdata, (uint32_t)0);
    serial_data(sdata, (uint16_t)source_action_type);
    serial_data(sdata, (uint16_t)0);
    serial_data(sdata, from);
    serial_data(sdata, std::string(""));
    if (tx_type == xtransaction_type_transfer)
    {
        std::string source_param(sizeof(uint32_t), 0);
        source_param += std::string((char *)&amount, sizeof(amount));
        serial_data_hex(sdata, hex(source_param));
    }    
    else
        serial_data(sdata, std::string(""));
    serial_data(sdata, std::string(""));
    serial_data(sdata, std::string(""));

    serial_data(sdata, (uint32_t)0);
    serial_data(sdata, (uint16_t)target_action_type);
    serial_data(sdata, (uint16_t)0);
    serial_data(sdata, to);
    serial_data(sdata, std::string(""));
    std::string target_param;
    if (tx_type == xtransaction_type_transfer ||
        tx_type == xtransaction_type_pledge_token_tgas ||
        tx_type == xtransaction_type_redeem_token_tgas)
    {
        target_param = std::string(sizeof(uint32_t), 0);
        target_param += std::string((char*)&amount, sizeof(amount));
        serial_data_hex(sdata, hex(target_param));
    } else if (tx_type == xtransaction_type_pledge_token_vote)
    {
        uint16_t lock_time = (uint16_t)extra;
        target_param += std::string((char*)&amount, sizeof(amount));
        target_param += std::string((char*)&lock_time, sizeof(lock_time));
        serial_data_hex(sdata, hex(target_param));
    } else if (tx_type == xtransaction_type_redeem_token_vote) {
        target_param += std::string((char*)&amount, sizeof(amount));
        serial_data_hex(sdata, hex(target_param));
    }    
    else
        serial_data(sdata, std::string(""));
    serial_data(sdata, std::string(""));
    serial_data(sdata, std::string(""));

    return ::data(sdata);

}