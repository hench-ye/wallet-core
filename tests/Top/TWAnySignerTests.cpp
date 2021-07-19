// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "uint256.h"
#include "proto/Top.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>
#include "Ethereum/RLP.h"

using namespace TW;
using namespace TW::Top;

TEST(TWAnySignerTop, Sign) {
    auto privateKey = parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62");
    
    Proto::SigningInput input;
    input.set_from("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    input.set_to("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    input.set_tx_type(4);
    input.set_amount(1);
    input.set_extra(5);
    input.set_tx_deposit(2);
    input.set_last_tx_nonce(3);
    input.set_source_action_type(5);
    input.set_target_action_type(6);
    input.set_last_tx_hash("12345678");
    input.set_note("top unit test");
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTop);

    ASSERT_EQ(hex(output.encoded()), "f8bfae54303030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae54303030303036366162333434393633656161303731663936333666616163323662306431613339393030333235040105020506038831323334353637388d746f7020756e69742074657374b84100ba19b5548428f1a3823830d623599b451d8f3ca913088bdfb28975ff3c1c698b38123c6222b90966ab3898268929503b3f4dc72a8ec05f2df53905482917cf58");
}
TEST(TWAnySignerTop, SignJson) {
    auto input = STRING("hello world!");
    auto privateKey = DATA("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62");
    auto encoded = WRAPS(TWAnySignerSignJSON(input.get(), privateKey.get(), TWCoinTypeTop));
    std::cout <<"sign:" << std::string(TWStringUTF8Bytes(encoded.get()),TWStringSize(encoded.get())) << std::endl;
    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeTop));
}
TEST(TWAnySignerTop, decode) {
    Data rawTx = parse_hex("f8508c68656c6c6f20776f726c6421b84101c3dd0f3a8099c32903f2751d5bc5ab078a5e997f7dcd45e0f1047f2bd0b87cde6a7f63b122ba1bb0bb1f4baf92a6748859156870f62b26dc2bed8416f560d135");
    TW::Ethereum::RLP::DecodedItem decoded = Ethereum::RLP::decode(rawTx);

    std::vector<std::string> vecData;
    for (int i = 0; i < (int)decoded.decoded.size(); i++)
    {
        std::string str(decoded.decoded[i].begin(), decoded.decoded[i].end());
        vecData.push_back(str);
        std::cout << "data" << i << ": " << hex(str) << std::endl;
    }    
}