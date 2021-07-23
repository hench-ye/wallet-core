// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class TopTests: XCTestCase {

    func testAddress() {
        let privateKey = PrivateKey(data: Data(hexString: "b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: publicKey, coin: .top)
        XCTAssertEqual(address.description, "T80000968927100f3cb7b23e8d477298311648978d8613")
    }

    func testSigner() {
        let input = TopSigningInput.with {
            $0.from = "T80000968927100f3cb7b23e8d477298311648978d8613"
            $0.to = "T00000LNi53Ub726HcPXZfC4z6zLgTo5ks6GzTUp"
            $0.txType = 4
            $0.amount = 600000000000
            $0.extra = 0
            $0.txDeposit = 100000
            $0.sourceActionType = 0
            $0.targetActionType = 6
            $0.lastTxNonce = 1
            $0.lastTxHash = "0x9a26a60395e8efc3"
            $0.note = "transfer test"
            $0.privateKey = Data(hexString: "b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19")!
        }

        let output: TopSigningOutput = AnySigner.sign(input: input, coin: .top)

        //XCTAssertEqual(output.signature.hexString, "00062a200d70a0928de7f8ae4e9a39dde11f26115eaf462a021bd948304cd473b263ba78e7d10e55c98589c9f9ecf9d2033b976d56befba827a84e2c21a80bb085")
        //XCTAssertEqual(output.encoded.hexString, "f8d0ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae543830303030363661623334343936336561613037316639363336666161633236623064316133393930303332350486010203040506830304050506078801020304050607088831323334353637388d746f7020756e69742074657374b84100062a200d70a0928de7f8ae4e9a39dde11f26115eaf462a021bd948304cd473b263ba78e7d10e55c98589c9f9ecf9d2033b976d56befba827a84e2c21a80bb085")
    }
    func testSignJSON() {
        let json = "{\"version\":\"1.0\",\"target_account_addr\":\"T80000968927100f3cb7b23e8d477298311648978d8613\",\"token\":\"\",\"method\":\"sendTransaction\",\"sequence_id\":1626855255269,\"body\":\"{\\\"params\\\":{\\\"tx_type\\\":4,\\\"tx_len\\\":0,\\\"tx_structure_version\\\":0,\\\"to_ledger_id\\\":0,\\\"from_ledger_id\\\":0,\\\"tx_deposit\\\":100000,\\\"tx_expire_duration\\\":100,\\\"send_timestamp\\\":1626946435,\\\"tx_random_nonce\\\":0,\\\"premium_price\\\":0,\\\"last_tx_nonce\\\":1,\\\"last_tx_hash\\\":\\\"0x8c0b3a1fe140e8e4\\\",\\\"tx_hash\\\":\\\"0x12018406ae050dd998efabc97e24ab0b779de7fff8a86c691a675bdb85125596\\\",\\\"ext\\\":\\\"\\\",\\\"note\\\":\\\"transfer test\\\",\\\"challenge_proof\\\":\\\"\\\",\\\"authorization\\\":\\\"\\\",\\\"sender_action\\\":{\\\"action_hash\\\":0,\\\"action_type\\\":0,\\\"action_size\\\":0,\\\"tx_sender_account_addr\\\":\\\"T80000968927100f3cb7b23e8d477298311648978d8613\\\",\\\"action_name\\\":\\\"\\\",\\\"action_param\\\":\\\"0x000000000070c9b28b000000\\\",\\\"action_ext\\\":\\\"\\\",\\\"action_authorization\\\":\\\"\\\"},\\\"receiver_action\\\":{\\\"action_hash\\\":0,\\\"action_type\\\":6,\\\"action_size\\\":0,\\\"tx_receiver_account_addr\\\":\\\"T00000LNi53Ub726HcPXZfC4z6zLgTo5ks6GzTUp\\\",\\\"action_name\\\":\\\"\\\",\\\"action_param\\\":\\\"0x000000000070c9b28b000000\\\",\\\"action_ext\\\":\\\"\\\",\\\"action_authorization\\\":\\\"\\\"},\\\"public_key\\\":\\\"\\\",\\\"confirm_unit_info\\\":{\\\"exec_status\\\":0}}}\"}"
        let key = Data(hexString: "b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19")!
        let result = AnySigner.signJSON(json, key: key, coin: .top)

        XCTAssertEqual(result, "f90157b8f1040000000000000000000000a08601006400833bf9600000000000000000000000000100000000000000e4e840e11f3a0b8c00000000000000000d0000007472616e73666572207465737400000000000000002e00000054383030303039363839323731303066336362376232336538643437373239383331313634383937386438363133000000000c000000000000000070c9b28b00000000000000000000000000000006000000280000005430303030304c4e6935335562373236486350585a6643347a367a4c67546f356b7336477a545570000000000c000000000000000070c9b28b0000000000000000000000a03fa159bad1fcd7e7eb96ff1239e9b5f1fe21a51b4e92de62191965e8882e1fc4b84100a6245cba1f3df6b238624b32be8e91351523dfac3a11a0866919c4bfd862487678559366e528e20a00001c063c3f7d1c4b431eb9b6c0f4df9c758cd0b4aaa4f4")
    }
}
