/*
 *  This file is part of the Off-the-Record Next Generation Messaging
 *  library (libotr-ng).
 *
 *  Copyright (C) 2016-2018, the libotr-ng contributors.
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

#include <libotr/privkey.h>

typedef struct otrng_fixture_s {
  otrng_s *otr;
  otrng_s *v3;
  otrng_s *v34;
  otrng_client_state_s *state;
} otrng_fixture_s, otrng_fixture_p[1];

int dh_mpi_cmp(const dh_mpi_p m1, const dh_mpi_p m2) {
  return gcry_mpi_cmp(m1, m2);
}

void otrng_fixture_set_up(otrng_fixture_s *otrng_fixture, gconstpointer data) {

  otrng_fixture->state = otrng_client_state_new(NULL); // No callbacks
  otrng_fixture->state->userstate = otrl_userstate_create();
  otrng_fixture->state->protocol_name = otrng_strdup("protocol");
  otrng_fixture->state->account_name = otrng_strdup("account@protocol");

  uint8_t sym[ED448_PRIVATE_BYTES] = {1}; // non-random private key on purpose
  otrng_client_state_add_private_key_v4(otrng_fixture->state, sym);

  otrng_client_state_add_shared_prekey_v4(otrng_fixture->state, sym);

  otrng_policy_s policy = {.allows = OTRNG_ALLOW_V4};
  otrng_fixture->otr = otrng_new(otrng_fixture->state, policy);

  otrng_policy_s policyv3 = {.allows = OTRNG_ALLOW_V3};
  otrng_fixture->v3 = otrng_new(otrng_fixture->state, policyv3);
  otrng_fixture->v3->v3_conn =
      otrng_v3_conn_new(otrng_fixture->state, "they_are_bob");

  otrng_policy_s policyv34 = {.allows = OTRNG_ALLOW_V3 | OTRNG_ALLOW_V4};
  otrng_fixture->v34 = otrng_new(otrng_fixture->state, policyv34);
  otrng_fixture->v34->v3_conn =
      otrng_v3_conn_new(otrng_fixture->state, "they_are_alice");

  // TODO: This should be done automatically
  FILE *tmpFILEp;
  tmpFILEp = tmpfile();
  otrng_assert(!otrl_privkey_generate_FILEp(
      otrng_fixture->state->userstate, tmpFILEp,
      otrng_fixture->state->account_name, otrng_fixture->state->protocol_name));
  fclose(tmpFILEp);

  // Generate instance tag
  otrng_client_state_add_instance_tag(otrng_fixture->state, 0x100 + 1);
}

void otrng_fixture_teardown(otrng_fixture_s *otrng_fixture,
                            gconstpointer data) {
  otrl_userstate_free(otrng_fixture->state->userstate);

  otrng_client_state_free(otrng_fixture->state);
  otrng_fixture->state = NULL;

  otrng_free(otrng_fixture->otr);
  otrng_fixture->otr = NULL;

  otrng_free(otrng_fixture->v3);
  otrng_fixture->v3 = NULL;

  otrng_free(otrng_fixture->v34);
  otrng_fixture->v34 = NULL;
}

typedef struct identity_message_fixture_s {
  otrng_keypair_s *keypair;
  otrng_shared_prekey_pair_s *shared_prekey;
  user_profile_s *profile;
} identity_message_fixture_s, identity_message_fixture_p[1];

typedef struct prekey_message_fixture_s {
  otrng_keypair_s *keypair;
  otrng_shared_prekey_pair_s *shared_prekey;
  user_profile_s *profile;
} prekey_message_fixture_s, prekey_message_fixture_p[1];

typedef struct non_interactive_auth_message_fixture_s {
  otrng_keypair_s *keypair;
  otrng_shared_prekey_pair_s *shared_prekey;
  user_profile_s *profile;
} non_interactive_auth_message_fixture_s,
    non_interactive_auth_message_fixture_p[1];

// TODO: unify all of these
static void identity_message_fixture_setup(identity_message_fixture_s *fixture,
                                           gconstpointer user_data) {
  fixture->keypair = otrng_keypair_new();

  uint8_t sym[ED448_PRIVATE_BYTES] = {1}; // non-random private key on purpose
  otrng_keypair_generate(fixture->keypair, sym);
  otrng_assert(otrng_ec_point_valid(fixture->keypair->pub) == SUCCESS);

  fixture->profile = user_profile_new("4");

  fixture->shared_prekey = otrng_shared_prekey_pair_new();
  otrng_shared_prekey_pair_generate(fixture->shared_prekey, sym);
  otrng_assert(otrng_ec_point_valid(fixture->shared_prekey->pub) == SUCCESS);

  memcpy(fixture->profile->shared_prekey, fixture->shared_prekey->pub,
         sizeof(otrng_shared_prekey_pub_p));

  otrng_assert(fixture->profile != NULL);
  fixture->profile->expires = time(NULL) + 60 * 60;
  otrng_assert(user_profile_sign(fixture->profile, fixture->keypair) ==
               SUCCESS);
}

static void
identity_message_fixture_teardown(identity_message_fixture_s *fixture,
                                  gconstpointer user_data) {
  otrng_keypair_free(fixture->keypair);
  fixture->keypair = NULL;

  otrng_shared_prekey_pair_free(fixture->shared_prekey);
  fixture->shared_prekey = NULL;

  otrng_user_profile_free(fixture->profile);
  fixture->profile = NULL;
}

static void prekey_message_fixture_setup(prekey_message_fixture_s *fixture,
                                         gconstpointer user_data) {
  fixture->keypair = otrng_keypair_new();

  uint8_t sym[ED448_PRIVATE_BYTES] = {1}; // non-random private key on purpose
  otrng_keypair_generate(fixture->keypair, sym);
  otrng_assert(otrng_ec_point_valid(fixture->keypair->pub) == SUCCESS);

  fixture->profile = user_profile_new("4");

  fixture->shared_prekey = otrng_shared_prekey_pair_new();
  otrng_shared_prekey_pair_generate(fixture->shared_prekey, sym);
  otrng_assert(otrng_ec_point_valid(fixture->shared_prekey->pub) == SUCCESS);

  memcpy(fixture->profile->shared_prekey, fixture->shared_prekey->pub,
         sizeof(otrng_shared_prekey_pub_p));

  otrng_assert(fixture->profile != NULL);
  fixture->profile->expires = time(NULL) + 60 * 60;
  otrng_assert(user_profile_sign(fixture->profile, fixture->keypair) ==
               SUCCESS);
}

static void prekey_message_fixture_teardown(prekey_message_fixture_s *fixture,
                                            gconstpointer user_data) {
  otrng_keypair_free(fixture->keypair);
  fixture->keypair = NULL;

  otrng_shared_prekey_pair_free(fixture->shared_prekey);
  fixture->shared_prekey = NULL;

  otrng_user_profile_free(fixture->profile);
  fixture->profile = NULL;
}

void do_dake_fixture(otrng_s *alice, otrng_s *bob) {
  otrng_response_s *response_to_bob = otrng_response_new();
  otrng_response_s *response_to_alice = otrng_response_new();
  string_p query_message = NULL;

  otrng_assert(alice->state == OTRNG_STATE_START);
  otrng_assert(bob->state == OTRNG_STATE_START);

  // Alice sends a query message
  otrng_assert(otrng_build_query_message(&query_message, "", alice) == SUCCESS);
  otrng_assert(alice->state == OTRNG_STATE_START);
  otrng_assert_cmpmem("?OTRv4", query_message, 6);

  // Bob receives a query message
  otrng_assert(otrng_receive_message(response_to_alice, query_message, bob) ==
               SUCCESS);
  free(query_message);
  query_message = NULL;

  // Bob replies with an identity message
  otrng_assert(bob->state == OTRNG_STATE_WAITING_AUTH_R);
  otrng_assert(response_to_alice->to_display == NULL);
  otrng_assert(response_to_alice->to_send);
  otrng_assert_cmpmem("?OTR:AAQI", response_to_alice->to_send, 9);

  // Alice receives an identity message
  otrng_assert(otrng_receive_message(response_to_bob,
                                     response_to_alice->to_send,
                                     alice) == SUCCESS);
  free(response_to_alice->to_send);
  response_to_alice->to_send = NULL;

  // Alice has Bob's ephemeral keys
  otrng_assert_ec_public_key_eq(alice->keys->their_ecdh,
                                bob->keys->our_ecdh->pub);
  otrng_assert_dh_public_key_eq(alice->keys->their_dh, bob->keys->our_dh->pub);
  otrng_assert_not_zero(alice->keys->ssid, sizeof(alice->keys->ssid));
  otrng_assert_not_zero(alice->keys->shared_secret, sizeof(shared_secret_p));

  // Alice replies with an auth-r message
  otrng_assert(alice->state == OTRNG_STATE_WAITING_AUTH_I);
  otrng_assert(response_to_bob->to_display == NULL);
  otrng_assert(response_to_bob->to_send);
  otrng_assert_cmpmem("?OTR:AASR", response_to_bob->to_send, 9);

  // Bob receives an auth-r message
  otrng_assert(otrng_receive_message(response_to_alice,
                                     response_to_bob->to_send, bob) == SUCCESS);
  free(response_to_bob->to_send);
  response_to_bob->to_send = NULL;

  // Bob has Alice's ephemeral keys
  otrng_assert_ec_public_key_eq(bob->keys->their_ecdh,
                                alice->keys->our_ecdh->pub);
  otrng_assert_dh_public_key_eq(bob->keys->their_dh, alice->keys->our_dh->pub);
  otrng_assert_not_zero(bob->keys->ssid, sizeof(alice->keys->ssid));
  otrng_assert_zero(bob->keys->shared_secret, sizeof(shared_secret_p));
  otrng_assert_not_zero(bob->keys->current->root_key, sizeof(root_key_p));

  g_assert_cmpint(bob->keys->i, ==, 0);
  g_assert_cmpint(bob->keys->j, ==, 0);
  g_assert_cmpint(bob->keys->k, ==, 0);

  // Bob replies with an auth-i message
  otrng_assert(bob->state == OTRNG_STATE_ENCRYPTED_MESSAGES);
  otrng_assert(response_to_alice->to_display == NULL);
  otrng_assert(response_to_alice->to_send);
  otrng_assert_cmpmem("?OTR:AASI", response_to_alice->to_send, 9);

  // The double ratchet is initialized
  // otrng_assert(bob->keys->current);

  // for (unsigned int i = 0; i < 64; i++) {
  //  printf("%x", bob->keys->current->root_key[i]);
  //}

  // Alice receives an auth-i message
  otrng_assert(otrng_receive_message(response_to_bob,
                                     response_to_alice->to_send,
                                     alice) == SUCCESS);
  free(response_to_alice->to_send);
  response_to_alice->to_send = NULL;

  // Alice does not reply
  otrng_assert(alice->state == OTRNG_STATE_ENCRYPTED_MESSAGES);
  otrng_assert(response_to_bob->to_display == NULL);
  otrng_assert(!response_to_bob->to_send);

  g_assert_cmpint(alice->keys->i, ==, 0);
  g_assert_cmpint(alice->keys->j, ==, 0);
  g_assert_cmpint(alice->keys->k, ==, 0);

  // The double ratchet is initialized
  // otrng_assert(alice->keys->current);

  // Both have the same shared secret
  otrng_assert_root_key_eq(alice->keys->shared_secret,
                           bob->keys->shared_secret);
  // otrng_assert_chain_key_eq(alice->keys->current->chain_a->key,
  //                          bob->keys->current->chain_a->key);
  // otrng_assert_chain_key_eq(bob->keys->current->chain_b->key,
  //                          alice->keys->current->chain_b->key);

  // chain_key_p bob_sending_key, alice_receiving_key;
  // key_manager_get_sending_chain_key(bob_sending_key, bob->keys);
  // otrng_assert(key_manager_get_receiving_chain_key(alice_receiving_key, 0,
  //                                                alice->keys) == SUCCESS);
  // otrng_assert_chain_key_eq(bob_sending_key, alice_receiving_key);

  otrng_response_free(response_to_alice);
  response_to_alice = NULL;

  otrng_response_free(response_to_bob);
  response_to_bob = NULL;
}
