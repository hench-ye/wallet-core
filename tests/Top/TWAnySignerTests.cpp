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

using namespace TW;
using namespace TW::Top;

TEST(TWAnySignerTop, Sign) {
    auto privateKey = parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62");
    
    Proto::SigningInput input;
    input.set_from("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    input.set_to("T0000066ab344963eaa071f9636faac26b0d1a39900325");
    input.set_amount(1);
    input.set_tx_deposit(2);
    input.set_last_tx_nonce(3);
    input.set_last_tx_hash("12345678");
    input.set_token("hello world");
    input.set_note("top unit test");
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTop);

    ASSERT_EQ(hex(output.encoded()), "f8c7ae54303030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae543030303030363661623334343936336561613037316639363336666161633236623064316133393930303332350102038831323334353637388b68656c6c6f20776f726c648d746f7020756e69742074657374b841006415304fd83d3fc82c787c12690daaf903295e1ff4d4745c91462f9bf6ae06f731485495e7767df04f3607a8eb340da2097c5deb9ec439b1880593136c0e8405");
}
