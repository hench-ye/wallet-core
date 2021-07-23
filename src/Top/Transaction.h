// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Address.h"
#include "../Data.h"

namespace TW::Top {
enum enum_xtransaction_type {
    xtransaction_type_create_user_account        = 0,    // create user account

    xtransaction_type_run_contract               = 3,    // run contract
    xtransaction_type_transfer                   = 4,    // transfer asset

    xtransaction_type_vote                       = 20,
    xtransaction_type_abolish_vote               = 21,

    xtransaction_type_pledge_token_tgas          = 22,   // pledge token for tgas
    xtransaction_type_redeem_token_tgas          = 23,   // redeem token
    xtransaction_type_pledge_token_vote          = 27,   // pledge token for disk
    xtransaction_type_redeem_token_vote          = 28,   // redeem token

    xtransaction_type_max
};
  
class Transaction {
  public:
    std::vector<uint8_t> from;
    std::vector<uint8_t> to;
    uint16_t tx_type;
    uint64_t amount;
    uint32_t extra;
    uint32_t tx_deposit;
    uint16_t source_action_type;
    uint16_t target_action_type;
    uint64_t last_tx_nonce;
    std::vector<uint8_t> m_last_tx_hash;
    std::vector<uint8_t> note;
    /// Transaction signature.
    std::vector<uint8_t> signature;
    std::vector<uint8_t> hash;

    Transaction(const Data& from, const Data& to, uint16_t tx_type, uint64_t amount, uint32_t extra, 
                uint32_t tx_deposit, uint16_t source_action_type, uint16_t target_action_type,
                uint64_t last_tx_nonce, const Data& last_tx_hash, const Data& note)
        : from(std::move(from))
        , to(std::move(to))
        , tx_type(std::move(tx_type))
        , amount(std::move(amount))
        , extra(std::move(extra))
        , tx_deposit(std::move(tx_deposit))
        , source_action_type(std::move(source_action_type))
        , target_action_type(std::move(target_action_type))
        , last_tx_nonce(std::move(last_tx_nonce))
        , m_last_tx_hash(std::move(last_tx_hash))
        , note(std::move(note)) 
        {
            if (m_last_tx_hash.size() % 2 != 0 && m_last_tx_hash.size() > 2)
                m_last_tx_hash.insert(m_last_tx_hash.begin()+2, '0');
        }

  public:
    /// Encodes the transaction.
    Data encode() const noexcept;
    Data serial_transaction() const noexcept;
};

} // namespace TW::Top
