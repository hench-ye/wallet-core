// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Top/Transaction.h"

#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Top;

TEST(TopTransaction, Encode) {
    Data from = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    Data to = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    auto transaction = Transaction(from, to, 4, 1, 2, 3, 5, 6, 7, {}, {});
    ASSERT_EQ(hex(transaction.encode()), "f867ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235040102030506078080");
}

TEST(TopTransaction, EncodeWithData) {
    Data from = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    Data to = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    auto transaction = Transaction(from, to, 4, 1, 2, 3, 5, 6, 7, data("12345678"), data("top unit test"));
    ASSERT_EQ(hex(transaction.encode()), "f87cae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235040102030506078831323334353637388d746f7020756e69742074657374");
}
