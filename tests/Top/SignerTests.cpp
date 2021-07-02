// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Top/Signer.h"
#include "Top/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Top;

// TODO: Add tests

TEST(TopSigner, Sign) {
    auto from = Top::Address("T8000066ab344963eaa071f9636faac26b0d1a39900325");
    auto to = Top::Address("T8000066ab344963eaa071f9636faac26b0d1a39900325");
    auto transaction = Transaction(from, to, 1, 2, 3, {}, {}, {});

    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    Signer::sign(privateKey, transaction);
    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    EXPECT_EQ(hex(transaction.signature), "00d19c2d0bfdeb78585cd9e25e28c870c90ae76309d0877cdd43e874ce8238df545c6ff8d25acb0b09f83ac1d155f87c879bd374fd5ae9f90c51c2680de6d9df33");
    EXPECT_EQ(hex(transaction.encode()), "f8739466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a39900325010203808080b84100d19c2d0bfdeb78585cd9e25e28c870c90ae76309d0877cdd43e874ce8238df545c6ff8d25acb0b09f83ac1d155f87c879bd374fd5ae9f90c51c2680de6d9df33");
}

TEST(TopSigner, SignWithData) {
    auto from = Top::Address("T8000066ab344963eaa071f9636faac26b0d1a39900325");
    auto to = Top::Address("T8000066ab344963eaa071f9636faac26b0d1a39900325");
    auto transaction = Transaction(from, to, 0x010203040506, 0x030405, 0x0102030405060708, data(std::string("12345678")), data(std::string("hello world")), data(std::string("top unit test")));
    
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    Signer::sign(privateKey, transaction);

    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;

    EXPECT_EQ(hex(transaction.signature), "0051a134afd1fc323b4477d774a249742860c0d200f874ad6f3299c5270304e7f501423897a3d8e1613d339102af7f3011f901d85b0f848a27434a261563e259ee");
    EXPECT_EQ(hex(transaction.encode()), "f8a49466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a3990032586010203040506830304058801020304050607088831323334353637388b68656c6c6f20776f726c648d746f7020756e69742074657374b8410051a134afd1fc323b4477d774a249742860c0d200f874ad6f3299c5270304e7f501423897a3d8e1613d339102af7f3011f901d85b0f848a27434a261563e259ee");
}