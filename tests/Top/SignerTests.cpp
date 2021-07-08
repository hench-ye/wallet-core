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
    Data from = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    Data to = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    auto transaction = Transaction(from, to, 1, 2, 3, {}, {}, {});

    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    Signer::sign(privateKey, transaction);
    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    EXPECT_EQ(hex(transaction.signature), "00bee3f7735eabcba3a991f49080b50d21cb859d6e04ab55f41ccb205e008cd6f229d9a63dbec754f2a658785e112164af347ebf69edc105507b792928468a7ffc");
    EXPECT_EQ(hex(transaction.encode()), "f8a7ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235010203808080b84100bee3f7735eabcba3a991f49080b50d21cb859d6e04ab55f41ccb205e008cd6f229d9a63dbec754f2a658785e112164af347ebf69edc105507b792928468a7ffc");
}

TEST(TopSigner, SignWithData) {
    Data from = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    Data to = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    auto transaction = Transaction(from, to, 0x010203040506, 0x030405, 0x0102030405060708, data(std::string("12345678")), data(std::string("hello world")), data(std::string("top unit test")));

    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    Signer::sign(privateKey, transaction);

    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;

    EXPECT_EQ(hex(transaction.signature), "016c0a03419f26882c0bb2037821a75c87ef3111e229ad741e490549ea884321d833c45432d9a131160c3adeb10db1f940cd30eaa6aeb4c593e50bf73277a51130");
    EXPECT_EQ(hex(transaction.encode()), "f8d8ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae5438303030303636616233343439363365616130373166393633366661616332366230643161333939303033323586010203040506830304058801020304050607088831323334353637388b68656c6c6f20776f726c648d746f7020756e69742074657374b841016c0a03419f26882c0bb2037821a75c87ef3111e229ad741e490549ea884321d833c45432d9a131160c3adeb10db1f940cd30eaa6aeb4c593e50bf73277a51130");
}

TEST(TopSigner, topchain_test) {
    Data from = data(std::string("T80000968927100f3cb7b23e8d477298311648978d8613"));
    Data to = data(std::string("T00000LgGPqEpiK6XLCKRj9gVPN8Ej1aMbyAb3Hu"));
    auto transaction = Transaction(from, to, 80000000, 1000000, 2, data(std::string("0xdec40619edf2fbbe")), data(std::string("hello world")), data(std::string("top unit test")));

    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    auto privateKey = PrivateKey(parse_hex("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19"));
    Signer::sign(privateKey, transaction);

    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
 }