/* $OpenBSD: pmeth_lib.c,v 1.33 2023/07/07 19:37:54 beck Exp $ */
/* Written by Dr Stephen N Henson (steve@openssl.org) for the OpenSSL
 * project 2006.
 */
/* ====================================================================
 * Copyright (c) 2006 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/opensslconf.h>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509v3.h>

#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif

#include "asn1_local.h"
#include "evp_local.h"

DECLARE_STACK_OF(EVP_PKEY_METHOD)
STACK_OF(EVP_PKEY_METHOD) *pkey_app_methods = NULL;

extern const EVP_PKEY_METHOD cmac_pkey_meth;
extern const EVP_PKEY_METHOD dh_pkey_meth;
extern const EVP_PKEY_METHOD dsa_pkey_meth;
extern const EVP_PKEY_METHOD ec_pkey_meth;
extern const EVP_PKEY_METHOD ed25519_pkey_meth;
extern const EVP_PKEY_METHOD gostimit_pkey_meth;
extern const EVP_PKEY_METHOD gostr01_pkey_meth;
extern const EVP_PKEY_METHOD hkdf_pkey_meth;
extern const EVP_PKEY_METHOD hmac_pkey_meth;
extern const EVP_PKEY_METHOD rsa_pkey_meth;
extern const EVP_PKEY_METHOD rsa_pss_pkey_meth;
extern const EVP_PKEY_METHOD x25519_pkey_meth;

static const EVP_PKEY_METHOD *pkey_methods[] = {
	&cmac_pkey_meth,
	&dh_pkey_meth,
	&dsa_pkey_meth,
	&ec_pkey_meth,
	&ed25519_pkey_meth,
	&gostimit_pkey_meth,
	&gostr01_pkey_meth,
	&hkdf_pkey_meth,
	&hmac_pkey_meth,
	&rsa_pkey_meth,
	&rsa_pss_pkey_meth,
	&x25519_pkey_meth,
};

static const size_t pkey_methods_count =
    sizeof(pkey_methods) / sizeof(pkey_methods[0]);

int
evp_pkey_meth_get_count(void)
{
	int num = pkey_methods_count;

	if (pkey_app_methods != NULL)
		num += sk_EVP_PKEY_METHOD_num(pkey_app_methods);

	return num;
}

const EVP_PKEY_METHOD *
evp_pkey_meth_get0(int idx)
{
	int num = pkey_methods_count;

	if (idx < 0)
		return NULL;
	if (idx < num)
		return pkey_methods[idx];

	idx -= num;

	return sk_EVP_PKEY_METHOD_value(pkey_app_methods, idx);
}

const EVP_PKEY_METHOD *
EVP_PKEY_meth_find(int type)
{
	const EVP_PKEY_METHOD *pmeth;
	int i;

	for (i = evp_pkey_meth_get_count() - 1; i >= 0; i--) {
		pmeth = evp_pkey_meth_get0(i);
		if (pmeth->pkey_id == type)
			return pmeth;
	}

	return NULL;
}

static EVP_PKEY_CTX *
evp_pkey_ctx_new(EVP_PKEY *pkey, ENGINE *engine, int id)
{
	EVP_PKEY_CTX *pkey_ctx = NULL;
	const EVP_PKEY_METHOD *pmeth;

	if (id == -1) {
		if (pkey == NULL || pkey->ameth == NULL)
			return NULL;
		id = pkey->ameth->pkey_id;
	}
#ifndef OPENSSL_NO_ENGINE
	if (pkey != NULL && pkey->engine != NULL)
		engine = pkey->engine;
	/* Try to find an ENGINE which implements this method. */
	if (engine != NULL) {
		if (!ENGINE_init(engine)) {
			EVPerror(ERR_R_ENGINE_LIB);
			return NULL;
		}
	} else
		engine = ENGINE_get_pkey_meth_engine(id);

	/* Look up method handler in ENGINE or use internal tables. */
	if (engine != NULL)
		pmeth = ENGINE_get_pkey_meth(engine, id);
	else
#endif
		pmeth = EVP_PKEY_meth_find(id);

	if (pmeth == NULL) {
		EVPerror(EVP_R_UNSUPPORTED_ALGORITHM);
		goto err;
	}

	if ((pkey_ctx = calloc(1, sizeof(*pkey_ctx))) == NULL) {
		EVPerror(ERR_R_MALLOC_FAILURE);
		goto err;
	}
	pkey_ctx->engine = engine;
	engine = NULL;
	pkey_ctx->pmeth = pmeth;
	pkey_ctx->operation = EVP_PKEY_OP_UNDEFINED;
	if ((pkey_ctx->pkey = pkey) != NULL)
		EVP_PKEY_up_ref(pkey_ctx->pkey);

	if (pmeth->init != NULL) {
		if (pmeth->init(pkey_ctx) <= 0)
			goto err;
	}

	return pkey_ctx;

 err:
	EVP_PKEY_CTX_free(pkey_ctx);
#ifndef OPENSSL_NO_ENGINE
	ENGINE_finish(engine);
#endif

	return NULL;
}

EVP_PKEY_METHOD*
EVP_PKEY_meth_new(int id, int flags)
{
	EVP_PKEY_METHOD *pmeth;

	if ((pmeth = calloc(1, sizeof(EVP_PKEY_METHOD))) == NULL)
		return NULL;

	pmeth->pkey_id = id;
	pmeth->flags = flags | EVP_PKEY_FLAG_DYNAMIC;

	return pmeth;
}

void
EVP_PKEY_meth_get0_info(int *ppkey_id, int *pflags, const EVP_PKEY_METHOD *meth)
{
	if (ppkey_id)
		*ppkey_id = meth->pkey_id;
	if (pflags)
		*pflags = meth->flags;
}

void
EVP_PKEY_meth_copy(EVP_PKEY_METHOD *dst, const EVP_PKEY_METHOD *src)
{
	EVP_PKEY_METHOD preserve;

	preserve.pkey_id = dst->pkey_id;
	preserve.flags = dst->flags;

	*dst = *src;

	dst->pkey_id = preserve.pkey_id;
	dst->flags = preserve.flags;
}

void
EVP_PKEY_meth_free(EVP_PKEY_METHOD *pmeth)
{
	if (pmeth && (pmeth->flags & EVP_PKEY_FLAG_DYNAMIC))
		free(pmeth);
}

EVP_PKEY_CTX *
EVP_PKEY_CTX_new(EVP_PKEY *pkey, ENGINE *engine)
{
	return evp_pkey_ctx_new(pkey, engine, -1);
}

EVP_PKEY_CTX *
EVP_PKEY_CTX_new_id(int id, ENGINE *engine)
{
	return evp_pkey_ctx_new(NULL, engine, id);
}

EVP_PKEY_CTX *
EVP_PKEY_CTX_dup(EVP_PKEY_CTX *pctx)
{
	EVP_PKEY_CTX *rctx = NULL;

	if (pctx->pmeth == NULL || pctx->pmeth->copy == NULL)
		goto err;
#ifndef OPENSSL_NO_ENGINE
	/* Make sure it's safe to copy a pkey context using an ENGINE */
	if (pctx->engine != NULL && !ENGINE_init(pctx->engine)) {
		EVPerror(ERR_R_ENGINE_LIB);
		goto err;
	}
#endif
	if ((rctx = calloc(1, sizeof(*rctx))) == NULL) {
		EVPerror(ERR_R_MALLOC_FAILURE);
		goto err;
	}

	rctx->pmeth = pctx->pmeth;
#ifndef OPENSSL_NO_ENGINE
	rctx->engine = pctx->engine;
#endif

	if ((rctx->pkey = pctx->pkey) != NULL)
		EVP_PKEY_up_ref(rctx->pkey);
	if ((rctx->peerkey = pctx->peerkey) != NULL)
		EVP_PKEY_up_ref(rctx->peerkey);

	rctx->operation = pctx->operation;

	if (pctx->pmeth->copy(rctx, pctx) <= 0)
		goto err;

	return rctx;

 err:
	EVP_PKEY_CTX_free(rctx);
	return NULL;
}

int
EVP_PKEY_meth_add0(const EVP_PKEY_METHOD *pmeth)
{
	if (pkey_app_methods == NULL) {
		pkey_app_methods = sk_EVP_PKEY_METHOD_new(NULL);
		if (pkey_app_methods == NULL)
			return 0;
	}

	if (!sk_EVP_PKEY_METHOD_push(pkey_app_methods, pmeth))
		return 0;

	return 1;
}

void
EVP_PKEY_CTX_free(EVP_PKEY_CTX *ctx)
{
	if (ctx == NULL)
		return;
	if (ctx->pmeth && ctx->pmeth->cleanup)
		ctx->pmeth->cleanup(ctx);
	EVP_PKEY_free(ctx->pkey);
	EVP_PKEY_free(ctx->peerkey);
#ifndef OPENSSL_NO_ENGINE
	ENGINE_finish(ctx->engine);
#endif
	free(ctx);
}

int
EVP_PKEY_CTX_ctrl(EVP_PKEY_CTX *ctx, int keytype, int optype, int cmd,
    int p1, void *p2)
{
	int ret;

	if (!ctx || !ctx->pmeth || !ctx->pmeth->ctrl) {
		EVPerror(EVP_R_COMMAND_NOT_SUPPORTED);
		return -2;
	}
	if ((keytype != -1) && (ctx->pmeth->pkey_id != keytype))
		return -1;

	if (ctx->operation == EVP_PKEY_OP_UNDEFINED) {
		EVPerror(EVP_R_NO_OPERATION_SET);
		return -1;
	}

	if ((optype != -1) && !(ctx->operation & optype)) {
		EVPerror(EVP_R_INVALID_OPERATION);
		return -1;
	}

	ret = ctx->pmeth->ctrl(ctx, cmd, p1, p2);

	if (ret == -2)
		EVPerror(EVP_R_COMMAND_NOT_SUPPORTED);

	return ret;

}

int
EVP_PKEY_CTX_ctrl_str(EVP_PKEY_CTX *ctx, const char *name, const char *value)
{
	if (!ctx || !ctx->pmeth || !ctx->pmeth->ctrl_str) {
		EVPerror(EVP_R_COMMAND_NOT_SUPPORTED);
		return -2;
	}
	if (!strcmp(name, "digest")) {
		return EVP_PKEY_CTX_md(ctx, EVP_PKEY_OP_TYPE_SIG,
		    EVP_PKEY_CTRL_MD, value);
	}
	return ctx->pmeth->ctrl_str(ctx, name, value);
}

int
EVP_PKEY_CTX_str2ctrl(EVP_PKEY_CTX *ctx, int cmd, const char *str)
{
	size_t len;

	if ((len = strlen(str)) > INT_MAX)
		return -1;

	return ctx->pmeth->ctrl(ctx, cmd, len, (void *)str);
}

int
EVP_PKEY_CTX_hex2ctrl(EVP_PKEY_CTX *ctx, int cmd, const char *hexstr)
{
	unsigned char *hex = NULL;
	long length;
	int ret = 0;

	if ((hex = string_to_hex(hexstr, &length)) == NULL)
		goto err;
	if (length < 0 || length > INT_MAX) {
		ret = -1;
		goto err;
	}

	ret = ctx->pmeth->ctrl(ctx, cmd, length, hex);

 err:
	free(hex);
	return ret;
}

int
EVP_PKEY_CTX_md(EVP_PKEY_CTX *ctx, int optype, int cmd, const char *md_name)
{
	const EVP_MD *md;

	if ((md = EVP_get_digestbyname(md_name)) == NULL) {
		EVPerror(EVP_R_INVALID_DIGEST);
		return 0;
	}
	return EVP_PKEY_CTX_ctrl(ctx, -1, optype, cmd, 0, (void *)md);
}

int
EVP_PKEY_CTX_get_operation(EVP_PKEY_CTX *ctx)
{
	return ctx->operation;
}

void
EVP_PKEY_CTX_set0_keygen_info(EVP_PKEY_CTX *ctx, int *dat, int datlen)
{
	ctx->keygen_info = dat;
	ctx->keygen_info_count = datlen;
}

void
EVP_PKEY_CTX_set_data(EVP_PKEY_CTX *ctx, void *data)
{
	ctx->data = data;
}

void *
EVP_PKEY_CTX_get_data(EVP_PKEY_CTX *ctx)
{
	return ctx->data;
}

EVP_PKEY *
EVP_PKEY_CTX_get0_pkey(EVP_PKEY_CTX *ctx)
{
	return ctx->pkey;
}

EVP_PKEY *
EVP_PKEY_CTX_get0_peerkey(EVP_PKEY_CTX *ctx)
{
	return ctx->peerkey;
}

void
EVP_PKEY_CTX_set_app_data(EVP_PKEY_CTX *ctx, void *data)
{
	ctx->app_data = data;
}

void *
EVP_PKEY_CTX_get_app_data(EVP_PKEY_CTX *ctx)
{
	return ctx->app_data;
}

void
EVP_PKEY_meth_set_init(EVP_PKEY_METHOD *pmeth,
    int (*init)(EVP_PKEY_CTX *ctx))
{
	pmeth->init = init;
}

void
EVP_PKEY_meth_set_copy(EVP_PKEY_METHOD *pmeth,
    int (*copy)(EVP_PKEY_CTX *dst, EVP_PKEY_CTX *src))
{
	pmeth->copy = copy;
}

void
EVP_PKEY_meth_set_cleanup(EVP_PKEY_METHOD *pmeth,
    void (*cleanup)(EVP_PKEY_CTX *ctx))
{
	pmeth->cleanup = cleanup;
}

void
EVP_PKEY_meth_set_paramgen(EVP_PKEY_METHOD *pmeth,
    int (*paramgen_init)(EVP_PKEY_CTX *ctx),
    int (*paramgen)(EVP_PKEY_CTX *ctx, EVP_PKEY *pkey))
{
	pmeth->paramgen_init = paramgen_init;
	pmeth->paramgen = paramgen;
}

void
EVP_PKEY_meth_set_keygen(EVP_PKEY_METHOD *pmeth,
    int (*keygen_init)(EVP_PKEY_CTX *ctx),
    int (*keygen)(EVP_PKEY_CTX *ctx, EVP_PKEY *pkey))
{
	pmeth->keygen_init = keygen_init;
	pmeth->keygen = keygen;
}

void
EVP_PKEY_meth_set_sign(EVP_PKEY_METHOD *pmeth,
    int (*sign_init)(EVP_PKEY_CTX *ctx),
    int (*sign)(EVP_PKEY_CTX *ctx, unsigned char *sig, size_t *siglen,
    const unsigned char *tbs, size_t tbslen))
{
	pmeth->sign_init = sign_init;
	pmeth->sign = sign;
}

void
EVP_PKEY_meth_set_verify(EVP_PKEY_METHOD *pmeth,
    int (*verify_init)(EVP_PKEY_CTX *ctx),
    int (*verify)(EVP_PKEY_CTX *ctx, const unsigned char *sig, size_t siglen,
    const unsigned char *tbs, size_t tbslen))
{
	pmeth->verify_init = verify_init;
	pmeth->verify = verify;
}

void
EVP_PKEY_meth_set_verify_recover(EVP_PKEY_METHOD *pmeth,
    int (*verify_recover_init)(EVP_PKEY_CTX *ctx),
    int (*verify_recover)(EVP_PKEY_CTX *ctx,
    unsigned char *sig, size_t *siglen,
    const unsigned char *tbs, size_t tbslen))
{
	pmeth->verify_recover_init = verify_recover_init;
	pmeth->verify_recover = verify_recover;
}

void
EVP_PKEY_meth_set_signctx(EVP_PKEY_METHOD *pmeth,
    int (*signctx_init)(EVP_PKEY_CTX *ctx, EVP_MD_CTX *mctx),
    int (*signctx)(EVP_PKEY_CTX *ctx, unsigned char *sig, size_t *siglen,
    EVP_MD_CTX *mctx))
{
	pmeth->signctx_init = signctx_init;
	pmeth->signctx = signctx;
}

void
EVP_PKEY_meth_set_verifyctx(EVP_PKEY_METHOD *pmeth,
    int (*verifyctx_init)(EVP_PKEY_CTX *ctx, EVP_MD_CTX *mctx),
    int (*verifyctx)(EVP_PKEY_CTX *ctx, const unsigned char *sig, int siglen,
    EVP_MD_CTX *mctx))
{
	pmeth->verifyctx_init = verifyctx_init;
	pmeth->verifyctx = verifyctx;
}

void
EVP_PKEY_meth_set_encrypt(EVP_PKEY_METHOD *pmeth,
    int (*encrypt_init)(EVP_PKEY_CTX *ctx),
    int (*encryptfn)(EVP_PKEY_CTX *ctx, unsigned char *out, size_t *outlen,
    const unsigned char *in, size_t inlen))
{
	pmeth->encrypt_init = encrypt_init;
	pmeth->encrypt = encryptfn;
}

void
EVP_PKEY_meth_set_decrypt(EVP_PKEY_METHOD *pmeth,
    int (*decrypt_init)(EVP_PKEY_CTX *ctx),
    int (*decrypt)(EVP_PKEY_CTX *ctx, unsigned char *out, size_t *outlen,
    const unsigned char *in, size_t inlen))
{
	pmeth->decrypt_init = decrypt_init;
	pmeth->decrypt = decrypt;
}

void
EVP_PKEY_meth_set_derive(EVP_PKEY_METHOD *pmeth,
    int (*derive_init)(EVP_PKEY_CTX *ctx),
    int (*derive)(EVP_PKEY_CTX *ctx, unsigned char *key, size_t *keylen))
{
	pmeth->derive_init = derive_init;
	pmeth->derive = derive;
}

void
EVP_PKEY_meth_set_ctrl(EVP_PKEY_METHOD *pmeth,
    int (*ctrl)(EVP_PKEY_CTX *ctx, int type, int p1, void *p2),
    int (*ctrl_str)(EVP_PKEY_CTX *ctx, const char *type, const char *value))
{
	pmeth->ctrl = ctrl;
	pmeth->ctrl_str = ctrl_str;
}

void
EVP_PKEY_meth_set_check(EVP_PKEY_METHOD *pmeth, int (*check)(EVP_PKEY *pkey))
{
	pmeth->check = check;
}

void
EVP_PKEY_meth_set_public_check(EVP_PKEY_METHOD *pmeth,
    int (*public_check)(EVP_PKEY *pkey))
{
	pmeth->public_check = public_check;
}

void
EVP_PKEY_meth_set_param_check(EVP_PKEY_METHOD *pmeth,
    int (*param_check)(EVP_PKEY *pkey))
{
	pmeth->param_check = param_check;
}
