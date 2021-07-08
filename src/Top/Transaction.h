// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Address.h"
#include "../Data.h"
#include <boost/multiprecision/cpp_int.hpp>

namespace TW::Top {
class Transaction {
  public:
    using uint128_t = boost::multiprecision::uint128_t;

    std::vector<uint8_t> from;
    std::vector<uint8_t> to;
    uint64_t amount;
    uint32_t tx_deposit;
    uint64_t last_tx_nonce;
    std::vector<uint8_t> m_last_tx_hash;
    std::vector<uint8_t> token;
    std::vector<uint8_t> note;

    /// Transaction signature.
    std::vector<uint8_t> signature;

    Transaction(const Data& from, const Data& to, uint64_t amount, uint32_t tx_deposit,
                uint64_t last_tx_nonce, const Data& last_tx_hash, const Data& token, const Data& note)
        : from(std::move(from))
        , to(std::move(to))
        , amount(std::move(amount))
        , tx_deposit(std::move(tx_deposit))
        , last_tx_nonce(std::move(last_tx_nonce))
        , m_last_tx_hash(std::move(last_tx_hash))
        , token(std::move(token)) 
        , note(std::move(note)) 
        {
            if (m_last_tx_hash.size() % 2 != 0)
                m_last_tx_hash.insert(m_last_tx_hash.begin()+2, '0');
        }

  public:
    /// Encodes the transaction.
    Data encode() const noexcept;
};

} // namespace TW::Top
