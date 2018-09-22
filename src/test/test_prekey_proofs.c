/*
 *  This file is part of the Off-the-Record Next Generation Messaging
 *  library (libotr-ng).
 *
 *  Copyright (C) 2016-2018, the libotr-ng contributors.
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../prekey_proofs.h"

void test_ecdh_proof_generation_and_validation(void) {
  otrng_keypair_p v1, v2, v3, v4;
  uint8_t sym1[ED448_PRIVATE_BYTES] = {1}, sym2[ED448_PRIVATE_BYTES] = {2},
          sym3[ED448_PRIVATE_BYTES] = {3}, sym4[ED448_PRIVATE_BYTES] = {4};
  ec_scalar_p privs[3];
  ec_point_p pubs[3];
  uint8_t m[64] = {0x01, 0x02, 0x03};
  uint8_t m2[64] = {0x03, 0x02, 0x01};
  ecdh_proof_p res;

  otrng_keypair_generate(v1, sym1);
  otrng_keypair_generate(v2, sym2);
  otrng_keypair_generate(v3, sym3);
  otrng_keypair_generate(v4, sym4);

  goldilocks_448_scalar_copy(privs[0], v1->priv);
  goldilocks_448_scalar_copy(privs[1], v2->priv);
  goldilocks_448_scalar_copy(privs[2], v3->priv);
  goldilocks_448_point_copy(pubs[0], v1->pub);
  goldilocks_448_point_copy(pubs[1], v2->pub);
  goldilocks_448_point_copy(pubs[2], v3->pub);

  otrng_assert_is_success(ecdh_proof_generate(
      res, (const ec_scalar_p *)privs, (const ec_point_p *)pubs, 3, m, 0x13));
  otrng_assert(ecdh_proof_verify(res, (const ec_point_p *)pubs, 3, m, 0x13));
  otrng_assert(!ecdh_proof_verify(res, (const ec_point_p *)pubs, 3, m, 0x14));
  otrng_assert(!ecdh_proof_verify(res, (const ec_point_p *)pubs, 3, m2, 0x13));

  goldilocks_448_point_copy(pubs[1], v4->pub);

  otrng_assert(!ecdh_proof_verify(res, (const ec_point_p *)pubs, 3, m, 0x13));
}

void test_dh_proof_generation_and_validation(void) {
  dh_keypair_p v1, v2, v3, v4;
  gcry_mpi_t privs[3];
  gcry_mpi_t pubs[3];
  uint8_t m[64] = {0x01, 0x02, 0x03};
  uint8_t m2[64] = {0x03, 0x02, 0x01};
  dh_proof_p res;

  otrng_dh_keypair_generate(v1);
  otrng_dh_keypair_generate(v2);
  otrng_dh_keypair_generate(v3);
  otrng_dh_keypair_generate(v4);

  privs[0] = otrng_dh_mpi_copy(v1->priv);
  privs[1] = otrng_dh_mpi_copy(v2->priv);
  privs[2] = otrng_dh_mpi_copy(v3->priv);
  pubs[0] = otrng_dh_mpi_copy(v1->pub);
  pubs[1] = otrng_dh_mpi_copy(v2->pub);
  pubs[2] = otrng_dh_mpi_copy(v3->pub);

  otrng_assert_is_success(dh_proof_generate(
      res, (const gcry_mpi_t *)privs, (const gcry_mpi_t *)pubs, 3, m, 0x13));
  otrng_assert(dh_proof_verify(res, (const gcry_mpi_t *)pubs, 3, m, 0x13));
  otrng_assert(!dh_proof_verify(res, (const gcry_mpi_t *)pubs, 3, m, 0x14));
  otrng_assert(!dh_proof_verify(res, (const gcry_mpi_t *)pubs, 3, m2, 0x13));

  pubs[1] = otrng_dh_mpi_copy(v4->pub);

  otrng_assert(!dh_proof_verify(res, (const gcry_mpi_t *)pubs, 3, m, 0x13));
}
