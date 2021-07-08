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
    auto transaction = Transaction(from, to, 1, 2, 3, {}, {}, {});
    ASSERT_EQ(hex(transaction.encode()), "f864ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235010203808080");
}

TEST(TopTransaction, EncodeWithData) {
    Data from = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    Data to = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    auto transaction = Transaction(from, to, 1, 2, 3, data("12345678"), data("hello world"), data("top unit test"));
    ASSERT_EQ(hex(transaction.encode()), "f884ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae543830303030363661623334343936336561613037316639363336666161633236623064316133393930303332350102038831323334353637388b68656c6c6f20776f726c648d746f7020756e69742074657374");
}
