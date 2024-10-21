/* $OpenBSD: m_wp.c,v 1.13 2023/07/07 19:37:54 beck Exp $ */

#include <stdio.h>

#include <openssl/opensslconf.h>

#ifndef OPENSSL_NO_WHIRLPOOL

#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/whrlpool.h>

#include "evp_local.h"

static int
init(EVP_MD_CTX *ctx)
{
	return WHIRLPOOL_Init(ctx->md_data);
}

static int
update(EVP_MD_CTX *ctx, const void *data, size_t count)
{
	return WHIRLPOOL_Update(ctx->md_data, data, count);
}

static int
final(EVP_MD_CTX *ctx, unsigned char *md)
{
	return WHIRLPOOL_Final(md, ctx->md_data);
}

static const EVP_MD whirlpool_md = {
	.type = NID_whirlpool,
	.pkey_type = 0,
	.md_size = WHIRLPOOL_DIGEST_LENGTH,
	.flags = 0,
	.init = init,
	.update = update,
	.final = final,
	.copy = NULL,
	.cleanup = NULL,
	.block_size = WHIRLPOOL_BBLOCK / 8,
	.ctx_size = sizeof(EVP_MD *) + sizeof(WHIRLPOOL_CTX),
};

const EVP_MD *
EVP_whirlpool(void)
{
	return (&whirlpool_md);
}
#endif
