/*
 * WARNING: do not edit!
 * Generated by Makefile from providers/common/include/prov/der_ml_dsa.h.in
 *
 * Copyright 2025 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include "internal/der.h"
#include "crypto/ml_dsa.h"

/* Well known OIDs precompiled */

/*
 * id-ml-dsa-44 OBJECT IDENTIFIER ::= { sigAlgs 17 }
 */
#define DER_OID_V_id_ml_dsa_44 DER_P_OBJECT, 9, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x11
#define DER_OID_SZ_id_ml_dsa_44 11
extern const unsigned char ossl_der_oid_id_ml_dsa_44[DER_OID_SZ_id_ml_dsa_44];

/*
 * id-ml-dsa-65 OBJECT IDENTIFIER ::= { sigAlgs 18 }
 */
#define DER_OID_V_id_ml_dsa_65 DER_P_OBJECT, 9, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x12
#define DER_OID_SZ_id_ml_dsa_65 11
extern const unsigned char ossl_der_oid_id_ml_dsa_65[DER_OID_SZ_id_ml_dsa_65];

/*
 * id-ml-dsa-87 OBJECT IDENTIFIER ::= { sigAlgs 19 }
 */
#define DER_OID_V_id_ml_dsa_87 DER_P_OBJECT, 9, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x13
#define DER_OID_SZ_id_ml_dsa_87 11
extern const unsigned char ossl_der_oid_id_ml_dsa_87[DER_OID_SZ_id_ml_dsa_87];


int ossl_DER_w_algorithmIdentifier_ML_DSA(WPACKET *pkt, int tag, ML_DSA_KEY *key);
