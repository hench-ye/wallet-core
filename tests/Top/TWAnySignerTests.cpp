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
TEST(TWAnySignerTop, rlp) {
    std::string strtemp(300, '9');
    std::string strtemp2(10, '2');
    std::string strtemp3(15, '3');
    auto encoded = Data();
    append(encoded, Ethereum::RLP::encode(strtemp));
    append(encoded, Ethereum::RLP::encode(strtemp2));
    append(encoded, Ethereum::RLP::encode(strtemp3));
    Data output = Ethereum::RLP::encodeList(encoded);
    TW::Ethereum::RLP::DecodedItem decoded = Ethereum::RLP::decode(output);
    
    std::vector<std::string> vecData;
    for (int i = 0; i < (int)decoded.decoded.size(); i++)
    {
        std::string str(decoded.decoded[i].begin(), decoded.decoded[i].end());
        vecData.push_back(str);
        std::cout << "data" << i << ": " << hex(str) << std::endl;
    }
}
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
    auto input = STRING("{\"version\":\"1.0\",\"target_account_addr\":\"T80000968927100f3cb7b23e8d477298311648978d8613\",\"token\":\"\",\"method\":\"sendTransaction\",\"sequence_id\":1626855255269,\"body\":\"{\\\"params\\\":{\\\"tx_type\\\":4,\\\"tx_len\\\":0,\\\"tx_structure_version\\\":0,\\\"to_ledger_id\\\":0,\\\"from_ledger_id\\\":0,\\\"tx_deposit\\\":100000,\\\"tx_expire_duration\\\":100,\\\"send_timestamp\\\":1626946435,\\\"tx_random_nonce\\\":0,\\\"premium_price\\\":0,\\\"last_tx_nonce\\\":1,\\\"last_tx_hash\\\":\\\"0x8c0b3a1fe140e8e4\\\",\\\"tx_hash\\\":\\\"0x12018406ae050dd998efabc97e24ab0b779de7fff8a86c691a675bdb85125596\\\",\\\"ext\\\":\\\"\\\",\\\"note\\\":\\\"transfer test\\\",\\\"challenge_proof\\\":\\\"\\\",\\\"authorization\\\":\\\"\\\",\\\"sender_action\\\":{\\\"action_hash\\\":0,\\\"action_type\\\":0,\\\"action_size\\\":0,\\\"tx_sender_account_addr\\\":\\\"T80000968927100f3cb7b23e8d477298311648978d8613\\\",\\\"action_name\\\":\\\"\\\",\\\"action_param\\\":\\\"0x000000000070c9b28b000000\\\",\\\"action_ext\\\":\\\"\\\",\\\"action_authorization\\\":\\\"\\\"},\\\"receiver_action\\\":{\\\"action_hash\\\":0,\\\"action_type\\\":6,\\\"action_size\\\":0,\\\"tx_receiver_account_addr\\\":\\\"T00000LNi53Ub726HcPXZfC4z6zLgTo5ks6GzTUp\\\",\\\"action_name\\\":\\\"\\\",\\\"action_param\\\":\\\"0x000000000070c9b28b000000\\\",\\\"action_ext\\\":\\\"\\\",\\\"action_authorization\\\":\\\"\\\"},\\\"public_key\\\":\\\"\\\",\\\"confirm_unit_info\\\":{\\\"exec_status\\\":0}}}\"}");
    auto privateKey = DATA("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19");
    auto encoded = WRAPS(TWAnySignerSignJSON(input.get(), privateKey.get(), TWCoinTypeTop));
    std::cout <<"sign:" << std::string(TWStringUTF8Bytes(encoded.get()),TWStringSize(encoded.get())) << std::endl;
    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeTop));
}
TEST(TWAnySignerTop, decode) {
    Data rawTx = parse_hex("f90157b8f1040000000000000000000000a0860100640057d7f7600000000000000000000000001100000000000000ae6bb7fbfd066df700000000000000000d0000007472616e7366657220746573740000000000000000280000005430303030304c5943574239704d4c68755170647375616367456e64453862393839386854773274000000000c000000000000000070c9b28b000000000000000000000000000000060000002e00000054383030303036323831396664663434393965633432356465663435326534366366646333663238333334386538000000000c000000000000000070c9b28b0000000000000000000000a0b61e73c3fe720951ef5935352b7207bb80fc293262c9203452f1c46d8dc130d5b84101daf329ec6321fd2877e3255142059aa29b22eb52b1911e101c85c134b15329a848dbb8ef3959999cbc331504ae7f08a71153c29467cae28ac1ce4ac257015552");
    TW::Ethereum::RLP::DecodedItem decoded = Ethereum::RLP::decode(rawTx);

    std::vector<std::string> vecData;
    for (int i = 0; i < (int)decoded.decoded.size(); i++)
    {
        std::string str(decoded.decoded[i].begin(), decoded.decoded[i].end());
        vecData.push_back(str);
        std::cout << "data" << i << ": " << hex(str) << std::endl;
    }
}
