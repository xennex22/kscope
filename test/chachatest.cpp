//ithare::kscope chachatest.cpp 
//  adapted from tests/chachatest.c from LibreSSL 2.6.4

#include "lest.hpp"
#ifdef ITHARE_KSCOPE_TEST_EXTENSION
#include ITHARE_KSCOPE_TEST_EXTENSION //MUST go BEFORE ../src/kscope.h
#endif
//NB: normally, in your own files you should use simple #include "your_extension.h", 
//  #include MACRO above is to allow using the same test .cpp with external extensions

#include "../src/kscope.h"
#include "../kaleidoscoped/ssl/crypto/chacha/chacha.h"
#include "../kaleidoscoped/nostd.h"

/*
 * Copyright (c) 2014 Joel Sing <jsing@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

struct chacha_tv {
	const char *desc;
	const uint8_t key[32];
	const uint8_t iv[8];
	const size_t len;
	const unsigned char out[512];
};

/*
 * Test vectors from:
 *   http://tools.ietf.org/html/draft-strombergson-chacha-test-vectors-01
 */
constexpr chacha_tv chacha_test_vectors[] = {
	{
		"TC1: All zero key and IV",
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		64,
		{
			0x76, 0xb8, 0xe0, 0xad, 0xa0, 0xf1, 0x3d, 0x90,
			0x40, 0x5d, 0x6a, 0xe5, 0x53, 0x86, 0xbd, 0x28,
			0xbd, 0xd2, 0x19, 0xb8, 0xa0, 0x8d, 0xed, 0x1a,
			0xa8, 0x36, 0xef, 0xcc, 0x8b, 0x77, 0x0d, 0xc7,
			0xda, 0x41, 0x59, 0x7c, 0x51, 0x57, 0x48, 0x8d,
			0x77, 0x24, 0xe0, 0x3f, 0xb8, 0xd8, 0x4a, 0x37,
			0x6a, 0x43, 0xb8, 0xf4, 0x15, 0x18, 0xa1, 0x1c,
			0xc3, 0x87, 0xb6, 0x69, 0xb2, 0xee, 0x65, 0x86,
		},
	},
	{
		"TC2: Single bit in key set, all zero IV",
		{
			0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		64,
		{
			0xc5, 0xd3, 0x0a, 0x7c, 0xe1, 0xec, 0x11, 0x93,
			0x78, 0xc8, 0x4f, 0x48, 0x7d, 0x77, 0x5a, 0x85,
			0x42, 0xf1, 0x3e, 0xce, 0x23, 0x8a, 0x94, 0x55,
			0xe8, 0x22, 0x9e, 0x88, 0x8d, 0xe8, 0x5b, 0xbd,
			0x29, 0xeb, 0x63, 0xd0, 0xa1, 0x7a, 0x5b, 0x99,
			0x9b, 0x52, 0xda, 0x22, 0xbe, 0x40, 0x23, 0xeb,
			0x07, 0x62, 0x0a, 0x54, 0xf6, 0xfa, 0x6a, 0xd8,
			0x73, 0x7b, 0x71, 0xeb, 0x04, 0x64, 0xda, 0xc0,
		},
	},
	{
		"TC3: Single bit in IV set, all zero key",
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		{
			0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		64,
		{
			0xef, 0x3f, 0xdf, 0xd6, 0xc6, 0x15, 0x78, 0xfb,
			0xf5, 0xcf, 0x35, 0xbd, 0x3d, 0xd3, 0x3b, 0x80,
			0x09, 0x63, 0x16, 0x34, 0xd2, 0x1e, 0x42, 0xac,
			0x33, 0x96, 0x0b, 0xd1, 0x38, 0xe5, 0x0d, 0x32,
			0x11, 0x1e, 0x4c, 0xaf, 0x23, 0x7e, 0xe5, 0x3c,
			0xa8, 0xad, 0x64, 0x26, 0x19, 0x4a, 0x88, 0x54,
			0x5d, 0xdc, 0x49, 0x7a, 0x0b, 0x46, 0x6e, 0x7d,
			0x6b, 0xbd, 0xb0, 0x04, 0x1b, 0x2f, 0x58, 0x6b
		},
	},
	{
		"TC4: All bits in key and IV are set",
		{
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		},
		{
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		},
		64,
		{
			0xd9, 0xbf, 0x3f, 0x6b, 0xce, 0x6e, 0xd0, 0xb5,
			0x42, 0x54, 0x55, 0x77, 0x67, 0xfb, 0x57, 0x44,
			0x3d, 0xd4, 0x77, 0x89, 0x11, 0xb6, 0x06, 0x05,
			0x5c, 0x39, 0xcc, 0x25, 0xe6, 0x74, 0xb8, 0x36,
			0x3f, 0xea, 0xbc, 0x57, 0xfd, 0xe5, 0x4f, 0x79,
			0x0c, 0x52, 0xc8, 0xae, 0x43, 0x24, 0x0b, 0x79,
			0xd4, 0x90, 0x42, 0xb7, 0x77, 0xbf, 0xd6, 0xcb,
			0x80, 0xe9, 0x31, 0x27, 0x0b, 0x7f, 0x50, 0xeb,
		},
	},
	{
		"TC5: Every even bit set in key and IV",
		{
			0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
			0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
			0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
			0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		},
		{
			0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
		},
		64,
		{
			0xbe, 0xa9, 0x41, 0x1a, 0xa4, 0x53, 0xc5, 0x43,
			0x4a, 0x5a, 0xe8, 0xc9, 0x28, 0x62, 0xf5, 0x64,
			0x39, 0x68, 0x55, 0xa9, 0xea, 0x6e, 0x22, 0xd6,
			0xd3, 0xb5, 0x0a, 0xe1, 0xb3, 0x66, 0x33, 0x11,
			0xa4, 0xa3, 0x60, 0x6c, 0x67, 0x1d, 0x60, 0x5c,
			0xe1, 0x6c, 0x3a, 0xec, 0xe8, 0xe6, 0x1e, 0xa1,
			0x45, 0xc5, 0x97, 0x75, 0x01, 0x7b, 0xee, 0x2f,
			0xa6, 0xf8, 0x8a, 0xfc, 0x75, 0x80, 0x69, 0xf7,
		},
	},
	{
		"TC6: Every odd bit set in key and IV",
		{
			0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
			0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
			0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
			0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
		},
		{
			0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
		},
		64,
		{
			0x9a, 0xa2, 0xa9, 0xf6, 0x56, 0xef, 0xde, 0x5a,
			0xa7, 0x59, 0x1c, 0x5f, 0xed, 0x4b, 0x35, 0xae,
			0xa2, 0x89, 0x5d, 0xec, 0x7c, 0xb4, 0x54, 0x3b,
			0x9e, 0x9f, 0x21, 0xf5, 0xe7, 0xbc, 0xbc, 0xf3,
			0xc4, 0x3c, 0x74, 0x8a, 0x97, 0x08, 0x88, 0xf8,
			0x24, 0x83, 0x93, 0xa0, 0x9d, 0x43, 0xe0, 0xb7,
			0xe1, 0x64, 0xbc, 0x4d, 0x0b, 0x0f, 0xb2, 0x40,
			0xa2, 0xd7, 0x21, 0x15, 0xc4, 0x80, 0x89, 0x06,
		},
	},
	{
		"TC7: Sequence patterns in key and IV",
		{
			0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
			0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
			0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
			0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
		},
		{
			0x0f, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x69, 0x78,
		},
		64,
		{
			0x9f, 0xad, 0xf4, 0x09, 0xc0, 0x08, 0x11, 0xd0,
			0x04, 0x31, 0xd6, 0x7e, 0xfb, 0xd8, 0x8f, 0xba,
			0x59, 0x21, 0x8d, 0x5d, 0x67, 0x08, 0xb1, 0xd6,
			0x85, 0x86, 0x3f, 0xab, 0xbb, 0x0e, 0x96, 0x1e,
			0xea, 0x48, 0x0f, 0xd6, 0xfb, 0x53, 0x2b, 0xfd,
			0x49, 0x4b, 0x21, 0x51, 0x01, 0x50, 0x57, 0x42,
			0x3a, 0xb6, 0x0a, 0x63, 0xfe, 0x4f, 0x55, 0xf7,
			0xa2, 0x12, 0xe2, 0x16, 0x7c, 0xca, 0xb9, 0x31,
		},
	},
	{
		"TC8: key: 'All your base are belong to us!, IV: 'IETF2013'",
		{
			0xc4, 0x6e, 0xc1, 0xb1, 0x8c, 0xe8, 0xa8, 0x78,
			0x72, 0x5a, 0x37, 0xe7, 0x80, 0xdf, 0xb7, 0x35,
			0x1f, 0x68, 0xed, 0x2e, 0x19, 0x4c, 0x79, 0xfb,
			0xc6, 0xae, 0xbe, 0xe1, 0xa6, 0x67, 0x97, 0x5d,
		},
		{
			0x1a, 0xda, 0x31, 0xd5, 0xcf, 0x68, 0x82, 0x21,
		},
		64,
		{
			0xf6, 0x3a, 0x89, 0xb7, 0x5c, 0x22, 0x71, 0xf9,
			0x36, 0x88, 0x16, 0x54, 0x2b, 0xa5, 0x2f, 0x06,
			0xed, 0x49, 0x24, 0x17, 0x92, 0x30, 0x2b, 0x00,
			0xb5, 0xe8, 0xf8, 0x0a, 0xe9, 0xa4, 0x73, 0xaf,
			0xc2, 0x5b, 0x21, 0x8f, 0x51, 0x9a, 0xf0, 0xfd,
			0xd4, 0x06, 0x36, 0x2e, 0x8d, 0x69, 0xde, 0x7f,
			0x54, 0xc6, 0x04, 0xa6, 0xe0, 0x0f, 0x35, 0x3f,
			0x11, 0x0f, 0x77, 0x1b, 0xdc, 0xa8, 0xab, 0x92,
		},
	},
};

#define N_VECTORS (sizeof(chacha_test_vectors) / sizeof(*chacha_test_vectors))

using namespace ithare::kscope::ssl;

/* Single-shot ChaCha20 using CRYPTO_chacha_20 interface. */
static void
crypto_chacha_20_test(const chacha_tv* tv, uint8_t* out0, uint8_t* in0)
{
	assert(tv->len <= 64);
	//auto in = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3X(64,in0,tv->len);
	//ITHARE_KSCOPE_INT3(uint8_t) out[64];
	//auto key = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3(tv->key);
	//auto iv = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3(tv->iv);
	ITHARE_KSCOPE_CALL3(CRYPTO_chacha_20)(out0, in0, tv->len, tv->key, tv->iv, 0);
	//ithare::kscope::kscope_copyarr(out0,out,tv->len);
}

static void
constexpr crypto_chacha_20_test_compile_time(const chacha_tv* tv, [[maybe_unused]] const uint8_t* expected_out, const uint8_t* in0)
{
	assert(tv->len <= 64);
	//auto in = ithare::kscope::kscope_int_arr_to_ct_kscope_int<64 /*maximum value*/>(in0,tv->len /*actual value*/);		
	//ITHARE_KSCOPE_INT_CONSTEXPR(uint8_t) out[64];
	uint8_t out[64] = {};
	//auto key = ithare::kscope::kscope_int_arr_to_ct_kscope_int(tv->key);
	//auto iv = ithare::kscope::kscope_int_arr_to_ct_kscope_int(tv->iv);
	ITHARE_KSCOPE_CALL_AS_CONSTEXPR(CRYPTO_chacha_20)(out, in0, tv->len, tv->key, tv->iv, 0);
	assert(ithare::kscope::kscope_cmparr(out,expected_out,tv->len) == 0);	
}

/* Single-shot ChaCha20 using the ChaCha interface. */
static void
chacha_ctx_full_test(const chacha_tv* tv, uint8_t *out0, uint8_t* in0)
{
	ITHARE_KSCOPE_KSCOPECLASS(ChaCha_ctx) ctx;

	//auto key = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3(tv->key);
	//auto iv = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3(tv->iv);
	ITHARE_KSCOPE_CALL3(ChaCha_set_key)(&ctx, tv->key, 256);
	ITHARE_KSCOPE_CALL3(ChaCha_set_iv)(&ctx, tv->iv, ITHARE_KSCOPE_INTNULLPTR);
	assert(tv->len <= 64);
	//auto in = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3X(64,in0,tv->len);
	//ITHARE_KSCOPE_INT3(uint8_t) out[64];
	ITHARE_KSCOPE_CALL3(ChaCha)(&ctx, out0, in0, tv->len);
	//ithare::kscope::kscope_copyarr(out0,out,tv->len);
}

/* ChaCha20 with partial writes using the Chacha interface. */
static void
chacha_ctx_partial_test(const chacha_tv* tv, uint8_t* out0, uint8_t* in0)
{
	ITHARE_KSCOPE_KSCOPECLASS(ChaCha_ctx) ctx;
	size_t len, size = 0;
	assert(tv->len <= 64);
	//auto in1 = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3X(64,in0,tv->len);
	//ITHARE_KSCOPE_PTR_OF_SAME_TYPE_AS(in1.arr) in = in1.arr;
	//ITHARE_KSCOPE_INT3(uint8_t) out1[64];
	//ITHARE_KSCOPE_PTR_OF_SAME_TYPE_AS(out1) out = out1;

	//auto key = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3(tv->key);
	//auto iv = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3(tv->iv);
	ITHARE_KSCOPE_CALL3(ChaCha_set_key)(&ctx, tv->key, 256);
	ITHARE_KSCOPE_CALL3(ChaCha_set_iv)(&ctx, tv->iv, ITHARE_KSCOPE_INTNULLPTR);
	assert(tv->len > 0);
	len = tv->len - 1;
	while (len > 1) {
		size = len / 2;
		ITHARE_KSCOPE_CALL3(ChaCha)(&ctx, out0, in0, size);
		in0 += size;
		out0 += size;
		len -= size;
	}
	ITHARE_KSCOPE_CALL3(ChaCha)(&ctx, out0, in0, len + 1);
	//ithare::kscope::kscope_copyarr(out0,out1,tv->len);
}

/* ChaCha20 with single byte writes using the Chacha interface. */
static void
chacha_ctx_single_test(const chacha_tv *tv, uint8_t* out0, uint8_t* in0)
{//doing manual array copying here to test that it works too 
	ITHARE_KSCOPE_KSCOPECLASS(ChaCha_ctx) ctx;
	assert(tv->len <= 64);
	auto in = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3X(64,in0,tv->len);

	auto key = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3(tv->key);
	auto iv = ITHARE_KSCOPE_INT_ARR_TO_KSCOPE_INT3(tv->iv);
	ITHARE_KSCOPE_CALL3(ChaCha_set_key)(&ctx, key.arr, ITHARE_KSCOPE_INTLIT3(256));
	ITHARE_KSCOPE_CALL3(ChaCha_set_iv)(&ctx, iv.arr, ITHARE_KSCOPE_INTNULLPTR);
	ITHARE_KSCOPE_INT3(uint8_t) out[64];
	for (size_t i = 0; i < tv->len; i++)
		ITHARE_KSCOPE_CALL3(ChaCha)(&ctx, &out[i], &in.arr[i], 1);
	ithare::kscope::kscope_copyarr(out0,out,tv->len);
}

#ifdef __GNUC__ //warnings in lest.hpp - can only disable :-(
#pragma GCC diagnostic push
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wmissing-braces"
#endif
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#ifdef _MSC_VER //warnings in lest.hpp - can only disable :-(
#pragma warning(push)
#pragma warning(disable:4100)
#endif

const lest::test module[] = {
    CASE( "compile-time: crypto_chacha_20_test_compile_time()", ) 
    {//pretty ugly; NOT recommended for practical usage (see KSCOPE_CT_* wrappers below for recommended usage of constexpr crypto)
			constexpr uint8_t in[64] = {};
			constexpr uint8_t out[64] = {};
			constexpr const chacha_tv* tv = &chacha_test_vectors[0];
			static_assert(tv->len <= sizeof(in));
			static_assert(tv->len <= sizeof(out));
			crypto_chacha_20_test_compile_time(tv,tv->out,in);
	},
    CASE( "compile-time: KSCOPE_CT_Chacha(); RECOMMENDED way to use compile-time crypto", ) 
    {
			constexpr const chacha_tv* tv = &chacha_test_vectors[0];
#ifdef ITHARE_KSCOPE_WORKAROUND_FOR_GCC_BUG_84463
			constexpr ChaCha_ctx<> ctx1 = KSCOPE_CT_Chacha_set_key_iv(chacha_test_vectors[0].key,256,chacha_test_vectors[0].iv,nullptr);
#else
			constexpr ChaCha_ctx<> ctx1 = KSCOPE_CT_Chacha_set_key_iv(tv->key,256,tv->iv,nullptr);
#endif
			constexpr uint8_t in[64] = {};
			static_assert(tv->len <= sizeof(in));
			constexpr auto encrypted1 = KSCOPE_CT_Chacha(ctx1,in);
			constexpr ithare::kscope::KscopeArrayWrapper<uint8_t,64> out = encrypted1.second;
#ifdef ITHARE_KSCOPE_WORKAROUND_FOR_GCC_BUG_84463
			static_assert(ithare::kscope::kscope_cmparr(out.arr,chacha_test_vectors[0].out,tv->len) == 0);
#else
			static_assert(ithare::kscope::kscope_cmparr(out.arr,tv->out,tv->len) == 0);
#endif
			constexpr ChaCha_ctx<> ctx2 = encrypted1.first;
	},
    CASE( "crypto_chacha_20_test()", ) 
    {
		for (size_t i = 0; i < N_VECTORS; i++) {
			uint8_t in[64];
			uint8_t out[64];
			const chacha_tv* tv = &chacha_test_vectors[i];
			assert(tv->len <= sizeof(in));
			assert(tv->len <= sizeof(out));
			memset(in,0,sizeof(in));
			crypto_chacha_20_test(tv,out,in);
			EXPECT(memcmp(out,tv->out,tv->len)==0);
		}
    },
    CASE( "chacha_ctx_full_test()", ) 
    {
		for (size_t i = 0; i < N_VECTORS; i++) {
			uint8_t in[64];
			uint8_t out[64];
			const chacha_tv* tv = &chacha_test_vectors[i];
			assert(tv->len <= sizeof(in));
			assert(tv->len <= sizeof(out));
			memset(in,0,sizeof(in));
			chacha_ctx_full_test(tv,out,in);
			EXPECT(memcmp(out,tv->out,tv->len)==0);
		}
    },
    CASE( "chacha_ctx_partial_test()", ) 
    {
		for (size_t i = 0; i < N_VECTORS; i++) {
			uint8_t in[64];
			uint8_t out[64];
			const chacha_tv* tv = &chacha_test_vectors[i];
			assert(tv->len <= sizeof(in));
			assert(tv->len <= sizeof(out));
			memset(in,0,sizeof(in));
			chacha_ctx_partial_test(tv,out,in);
			EXPECT(memcmp(out,tv->out,tv->len)==0);
		}
    },
    CASE( "chacha_ctx_single_test()", ) 
    {
		for (size_t i = 0; i < N_VECTORS; i++) {
			uint8_t in[64];
			uint8_t out[64];
			const chacha_tv* tv = &chacha_test_vectors[i];
			assert(tv->len <= sizeof(in));
			assert(tv->len <= sizeof(out));
			memset(in,0,sizeof(in));
			chacha_ctx_single_test(tv,out,in);
			EXPECT(memcmp(out,tv->out,tv->len)==0);
		}
    },
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

extern lest::tests& specification();

MODULE( specification(), module )
