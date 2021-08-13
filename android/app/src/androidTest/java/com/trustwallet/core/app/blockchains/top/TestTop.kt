package com.trustwallet.core.app.blockchains.top

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHex
import com.trustwallet.core.app.utils.toHexBytesInByteString
import org.junit.Assert.assertEquals
import org.junit.Test
import wallet.core.java.AnySigner
import wallet.core.jni.AnyAddress
import wallet.core.jni.CoinType
import wallet.core.jni.CoinType.TOP
import wallet.core.jni.PrivateKey
import wallet.core.jni.proto.Top
import wallet.core.jni.proto.Top.SigningOutput

class TestTopAddress {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testAddressFromPublicKey() {
        val privateKey = PrivateKey("b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19".toHexByteArray())
        val publicKey = privateKey.getPublicKeySecp256k1(false)
        val address = AnyAddress(publicKey, CoinType.TOP)

        assertEquals(address.description(), "T80000968927100f3cb7b23e8d477298311648978d8613")
    }

    @Test
    fun testAddressFromString() {
        val addressString = "T80000968927100f3cb7b23e8d477298311648978d8613"
        val address = AnyAddress(addressString, CoinType.TOP)
        assertEquals(address.description(), addressString)
    }

    @Test
    fun aeternityTransactionSigning() {
        val signingInput = Top.SigningInput.newBuilder()
            .setFrom("T8000066ab344963eaa071f9636faac26b0d1a39900325")
            .setTo("T8000066ab344963eaa071f9636faac26b0d1a39900325")
            .setTxType(4)
            .setAmount(0x010203040506)
            .setExtra(0x030405)
            .setTxDeposit(5)
            .setSourceActionType(6)
            .setTargetActionType(7)
            .setLastTxNonce(0x0102030405060708)
            .setLastTxHash("12345678")
            .setNote("top unit test")
            .setPrivateKey("2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62".toHexBytesInByteString())
            .build()

        val output = AnySigner.sign(signingInput, TOP, SigningOutput.parser())

        assertEquals(
            "0x00062a200d70a0928de7f8ae4e9a39dde11f26115eaf462a021bd948304cd473b263ba78e7d10e55c98589c9f9ecf9d2033b976d56befba827a84e2c21a80bb085",
            output.encoded.toByteArray().toHex()
        )
        assertEquals(
            "0xf8d0ae54383030303036366162333434393633656161303731663936333666616163323662306431613339393030333235ae543830303030363661623334343936336561613037316639363336666161633236623064316133393930303332350486010203040506830304050506078801020304050607088831323334353637388d746f7020756e69742074657374b84100062a200d70a0928de7f8ae4e9a39dde11f26115eaf462a021bd948304cd473b263ba78e7d10e55c98589c9f9ecf9d2033b976d56befba827a84e2c21a80bb085",
            output.signature.toByteArray().toHex()
        )
    }
}
