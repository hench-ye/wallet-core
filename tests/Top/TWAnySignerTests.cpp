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

    //ASSERT_EQ(hex(output.encoded()), "f9015db8f70400000000000000000000000200000064008367fa600000000000000000000000000300000000000000000000000000000000000000000000000d000000746f7020756e6974207465737400000000050000002e00000054303030303036366162333434393633656161303731663936333666616163323662306431613339393030333235000000000c000000000000000100000000000000000000000000000000000000060000002e00000054303030303036366162333434393633656161303731663936333666616163323662306431613339393030333235000000000c0000000000000001000000000000000000000000000000a068d8343df0a2dd8fc4c245e9fbaf3f2d42f7aa080d20969f0ee6fadc54d276b8b841017cceeb678b6e65b0b179357da24f4f6d4d8b528a683b692c7af464d8ebc29eee317cf3846c507afc03b0215ff5281232b82241925a91ee3dc31dd866aabd43d5");
}
TEST(TWAnySignerTop, SignJson) {
    auto input = STRING(
        "{\"version\":\"1.0\",\"target_account_addr\":\"T80000968927100f3cb7b23e8d477298311648978d8613\",\"token\":\"\",\"method\":\"sendTransaction\",\"sequence_id\":1626855255269,\"body\":\"{\\\"params\\\":{\\\"tx_type\\\":0,\\\"tx_len\\\":0,\\\"tx_structure_version\\\":0,\\\"to_ledger_id\\\":0,\\\"from_ledger_id\\\":0,\\\"tx_deposit\\\":100000,\\\"tx_expire_duration\\\":100,\\\"send_timestamp\\\":1628575639,\\\"tx_random_nonce\\\":0,\\\"premium_price\\\":0,\\\"last_tx_nonce\\\":1,\\\"last_tx_hash\\\":\\\"0x5635d72e157804cb\\\",\\\"tx_hash\\\":\\\"0x12018406ae050dd998efabc97e24ab0b779de7fff8a86c691a675bdb85125596\\\",\\\"ext\\\":\\\"\\\",\\\"note\\\":\\\"transfer test\\\",\\\"challenge_proof\\\":\\\"\\\",\\\"authorization\\\":\\\"\\\",\\\"sender_action\\\":{\\\"action_hash\\\":0,\\\"action_type\\\":1,\\\"action_size\\\":0,\\\"tx_sender_account_addr\\\":\\\"T80000968927100f3cb7b23e8d477298311648978d8613\\\",\\\"action_name\\\":\\\"\\\",\\\"action_param\\\":\\\"\\\",\\\"action_ext\\\":\\\"\\\",\\\"action_authorization\\\":\\\"\\\"},\\\"receiver_action\\\":{\\\"action_hash\\\":0,\\\"action_type\\\":2,\\\"action_size\\\":0,\\\"tx_receiver_account_addr\\\":\\\"T80000968927100f3cb7b23e8d477298311648978d8613\\\",\\\"action_name\\\":\\\"\\\",\\\"action_param\\\":\\\"0x2e00000054383030303039363836383665303965626239313736383133623266643365653162623032653464613764323537\\\",\\\"action_ext\\\":\\\"\\\",\\\"action_authorization\\\":\\\"\\\"},\\\"public_key\\\":\\\"\\\",\\\"confirm_unit_info\\\":{\\\"exec_status\\\":0}}}\"}");
    auto privateKey = DATA("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19");
    auto encoded = WRAPS(TWAnySignerSignJSON(input.get(), privateKey.get(), TWCoinTypeTop));

    std::string strtemp(TWStringUTF8Bytes(encoded.get()),TWStringSize(encoded.get()));
    std::cout <<"sign:" << strtemp << std::endl;
    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeTop));

    Data rawTx = parse_hex(strtemp);
    TW::Ethereum::RLP::DecodedItem decoded = Ethereum::RLP::decode(rawTx);

    std::vector<std::string> vecData;
    for (int i = 0; i < (int)decoded.decoded.size(); i++)
    {
        std::string str(decoded.decoded[i].begin(), decoded.decoded[i].end());
        vecData.push_back(str);
        std::cout << "data" << i << ": " << hex(str) << std::endl;
    }
}
TEST(TWAnySignerTop, sign2) {
    auto privateKey = parse_hex("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19");
    Proto::SigningInput input;
    input.set_from("T80000968927100f3cb7b23e8d477298311648978d8613");
    input.set_to("T00000LgGPqEpiK6XLCKRj9gVPN8Ej1aMbyAb3Hu");
    input.set_tx_type(4);
    input.set_amount(600000000000);
    input.set_extra(0);
    input.set_tx_deposit(100000);
    input.set_last_tx_nonce(1);
    input.set_source_action_type(0);
    input.set_target_action_type(6);
    input.set_last_tx_hash("0x9a26a60395e8efc3");
    input.set_note("transfer test");
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTop);
    std::string strtemp = output.encoded();
    std::cout << "encoded: " << hex(strtemp) << std::endl;

    //Data rawTx = parse_hex(strtemp);
    TW::Ethereum::RLP::DecodedItem decoded = Ethereum::RLP::decode(::data(strtemp));

    std::vector<std::string> vecData;
    for (int i = 0; i < (int)decoded.decoded.size(); i++)
    {
        std::string str(decoded.decoded[i].begin(), decoded.decoded[i].end());
        vecData.push_back(str);
        std::cout << "data" << i << ": " << hex(str) << std::endl;
    }
}