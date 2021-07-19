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
            $0.from = "T8000066ab344963eaa071f9636faac26b0d1a39900325"
            $0.to = "T8000066ab344963eaa071f9636faac26b0d1a39900325"
            $0.txType = 4
            $0.amount = 0x010203040506
            $0.extra = 0x030405
            $0.txDeposit = 5
            $0.sourceActionType = 6
            $0.targetActionType = 7
            $0.lastTxNonce = 0x0102030405060708
            $0.lastTxHash = "12345678"
            $0.note = "top unit test"
            $0.privateKey = Data(hexString: "2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62")!
        }

        let output: TopSigningOutput = AnySigner.sign(input: input, coin: .top)

        XCTAssertEqual(output.signature.hexString, "00062a200d70a0928de7f8ae4e9a39dde11f26115eaf462a021bd948304cd473b263ba78e7d10e55c98589c9f9ecf9d2033b976d56befba827a84e2c21a80bb085")
        XCTAssertEqual(output.encoded.hexString, "f8d0ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae543830303030363661623334343936336561613037316639363336666161633236623064316133393930303332350486010203040506830304050506078801020304050607088831323334353637388d746f7020756e69742074657374b84100062a200d70a0928de7f8ae4e9a39dde11f26115eaf462a021bd948304cd473b263ba78e7d10e55c98589c9f9ecf9d2033b976d56befba827a84e2c21a80bb085")
    }
    func testSignJSON() {
        let json = "hello world!"
        let key = Data(hexString: "2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62")!
        let result = AnySigner.signJSON(json, key: key, coin: .top)

        XCTAssertEqual(result, "f8508c68656c6c6f20776f726c6421b84101c3dd0f3a8099c32903f2751d5bc5ab078a5e997f7dcd45e0f1047f2bd0b87cde6a7f63b122ba1bb0bb1f4baf92a6748859156870f62b26dc2bed8416f560d135")
    }
}
