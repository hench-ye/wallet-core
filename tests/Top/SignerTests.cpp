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
    auto transaction = Transaction(from, to, 4, 1, 2, 3, 5, 6, 7, {}, {});

    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    Signer::sign(privateKey, transaction);
    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    EXPECT_EQ(hex(transaction.signature), "00890215bdd4da610826cfccf95fef6b83686e12375481563666d11fc99e3381d81b9eebd445bfe00ae557c7f65a8ca844bd624a21f9783ae84959da3e1670b509");
    EXPECT_EQ(hex(transaction.encode()), "f8aaae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235040102030506078080b84100890215bdd4da610826cfccf95fef6b83686e12375481563666d11fc99e3381d81b9eebd445bfe00ae557c7f65a8ca844bd624a21f9783ae84959da3e1670b509");
}

TEST(TopSigner, SignWithData) {
    Data from = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    Data to = data(std::string("T8000066ab344963eaa071f9636faac26b0d1a39900325"));
    auto transaction = Transaction(from, to, 4, 0x010203040506, 0x030405, 5, 6, 7, 0x0102030405060708, data(std::string("12345678")), data(std::string("top unit test")));

    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    //f88dae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae543830303030363661623334343936336561613037316639363336666161633236623064316133393930303332350486010203040506830304050506078801020304050607088831323334353637388d746f7020756e69742074657374
    auto privateKey = PrivateKey(parse_hex("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62"));
    Signer::sign(privateKey, transaction);

    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;

    EXPECT_EQ(hex(transaction.signature), "00062a200d70a0928de7f8ae4e9a39dde11f26115eaf462a021bd948304cd473b263ba78e7d10e55c98589c9f9ecf9d2033b976d56befba827a84e2c21a80bb085");
    EXPECT_EQ(hex(transaction.encode()), "f8d0ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae543830303030363661623334343936336561613037316639363336666161633236623064316133393930303332350486010203040506830304050506078801020304050607088831323334353637388d746f7020756e69742074657374b84100062a200d70a0928de7f8ae4e9a39dde11f26115eaf462a021bd948304cd473b263ba78e7d10e55c98589c9f9ecf9d2033b976d56befba827a84e2c21a80bb085");
}

TEST(TopSigner, topchain_test) {
    Data from = data(std::string("T80000968927100f3cb7b23e8d477298311648978d8613"));
    Data to = data(std::string("T00000LgGPqEpiK6XLCKRj9gVPN8Ej1aMbyAb3Hu"));
    uint64_t nonce;
    std::cin >> nonce;
    char last_hash[64];
    std::cin >> last_hash;
    std::cout << nonce <<","<< last_hash << std::endl;
    // transfer
    {
    Transaction transaction(from, to, 4, 90000000, 0, 1000000, 0, 6, nonce, data(std::string(last_hash)), data(std::string("")));
    std::cout <<"transfer encode:" << hex(transaction.encode()) << std::endl;
    auto privateKey = PrivateKey(parse_hex("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19"));
    Signer::sign(privateKey, transaction);
    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    }
    // staking
    {
    Transaction transaction(from, from, 27, 10000, 30, 1000000, 1, 21, nonce, data(std::string(last_hash)), data(std::string("")));
    std::cout <<"staking encode:" << hex(transaction.encode()) << std::endl;
    auto privateKey = PrivateKey(parse_hex("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19"));
    Signer::sign(privateKey, transaction);
    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    }
    // unstake
    {
    Transaction transaction(from, from, 28, 10000, 0, 1000000, 1, 22, nonce, data(std::string(last_hash)), data(std::string("")));
    std::cout <<"unstake encode:" << hex(transaction.encode()) << std::endl;
    auto privateKey = PrivateKey(parse_hex("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19"));
    Signer::sign(privateKey, transaction);
    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    }    
    // gas
    {
    Transaction transaction(from, from, 22, 1000, 0, 1000000, 1, 23, nonce, data(std::string(last_hash)), data(std::string("")));
    std::cout <<"Gas encode:" << hex(transaction.encode()) << std::endl;
    auto privateKey = PrivateKey(parse_hex("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19"));
    Signer::sign(privateKey, transaction);
    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    }
    // withdraw
    {
    Transaction transaction(from, from, 23, 1000, 0, 1000000, 1, 24, nonce, data(std::string(last_hash)), data(std::string("")));
    std::cout <<"Withdraw encode:" << hex(transaction.encode()) << std::endl;
    auto privateKey = PrivateKey(parse_hex("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19"));
    Signer::sign(privateKey, transaction);
    std::cout <<"sign:" << hex(transaction.signature) << std::endl;
    std::cout <<"encode:" << hex(transaction.encode()) << std::endl;
    }    
 }