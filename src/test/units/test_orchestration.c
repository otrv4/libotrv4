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

#include <glib.h>

#include "test_helpers.h"

#include "client_orchestration.h"
#include "messaging.h"

#include <libotr/privkey.h>

static otrng_client_s *temp_client;

static int load_privkey_v4__called = 0;
static otrng_client_id_s load_privkey_v4__called_with;
static otrng_keypair_s *load_privkey_v4__assign = NULL;
static void load_privkey_v4(const otrng_client_id_s cid) {
  load_privkey_v4__called++;
  load_privkey_v4__called_with = cid;

  temp_client->keypair = load_privkey_v4__assign;
}

static int create_privkey_v4__called = 0;
static otrng_client_id_s create_privkey_v4__called_with;
static otrng_keypair_s *create_privkey_v4__assign = NULL;
static void create_privkey_v4(const otrng_client_id_s cid) {
  create_privkey_v4__called++;
  create_privkey_v4__called_with = cid;

  temp_client->keypair = create_privkey_v4__assign;
}

static int store_privkey_v4__called = 0;
static otrng_client_s *store_privkey_v4__called_with;
static void store_privkey_v4(otrng_client_s *client) {
  store_privkey_v4__called++;
  store_privkey_v4__called_with = client;
}

static int load_client_profile__called = 0;
static otrng_client_id_s load_client_profile__called_with;
static otrng_client_profile_s *load_client_profile__assign = NULL;
static void load_client_profile(const otrng_client_id_s cid) {
  load_client_profile__called++;
  load_client_profile__called_with = cid;

  temp_client->client_profile = load_client_profile__assign;
}

static int store_client_profile__called = 0;
static otrng_client_s *store_client_profile__called_with;
static void store_client_profile(otrng_client_s *client,
                                 const otrng_client_id_s cid) {
  (void)cid;
  store_client_profile__called++;
  store_client_profile__called_with = client;
}

static int create_client_profile__called = 0;
static otrng_client_s *create_client_profile__called_with;
static otrng_client_profile_s *create_client_profile__assign = NULL;
static void create_client_profile(otrng_client_s *client,
                                  const otrng_client_id_s cid) {
  (void)cid;
  create_client_profile__called_with = client;
  create_client_profile__called++;

  temp_client->client_profile = create_client_profile__assign;
}

static int load_prekey_profile__called = 0;
static otrng_client_id_s load_prekey_profile__called_with;
static otrng_prekey_profile_s *load_prekey_profile__assign = NULL;
static void load_prekey_profile(const otrng_client_id_s cid) {
  load_prekey_profile__called++;
  load_prekey_profile__called_with = cid;

  temp_client->prekey_profile = load_prekey_profile__assign;
}

static int store_prekey_profile__called = 0;
static otrng_client_s *store_prekey_profile__called_with;
static void store_prekey_profile(otrng_client_s *client,
                                 const otrng_client_id_s cid) {
  (void)cid;
  store_prekey_profile__called++;
  store_prekey_profile__called_with = client;
}

static int create_prekey_profile__called = 0;
static otrng_client_s *create_prekey_profile__called_with;
static otrng_prekey_profile_s *create_prekey_profile__assign = NULL;
static void create_prekey_profile(otrng_client_s *client,
                                  const otrng_client_id_s cid) {
  (void)cid;
  create_prekey_profile__called_with = client;
  create_prekey_profile__called++;

  temp_client->prekey_profile = create_prekey_profile__assign;
}

static void load_prekey_messages(otrng_client_s *client) { (void)client; }
static void store_prekey_messages(otrng_client_s *client) { (void)client; }

static int load_forging_key__called = 0;
static otrng_client_s *load_forging_key__called_with;
static otrng_public_key *load_forging_key__assign = NULL;
static void load_forging_key(otrng_client_s *client) {
  load_forging_key__called++;
  load_forging_key__called_with = client;

  temp_client->forging_key = load_forging_key__assign;
}

static int store_forging_key__called = 0;
static otrng_client_s *store_forging_key__called_with;
static void store_forging_key(otrng_client_s *client) {
  store_forging_key__called++;
  store_forging_key__called_with = client;
}

static int create_forging_key__called = 0;
static otrng_client_id_s create_forging_key__called_with;
static otrng_public_key *create_forging_key__assign = NULL;
static void create_forging_key(const otrng_client_id_s cid) {
  create_forging_key__called++;
  create_forging_key__called_with = cid;

  temp_client->forging_key = create_forging_key__assign;
}

static int store_expired_client_profile__called = 0;
static otrng_client_s *store_expired_client_profile__called_with;
static void store_expired_client_profile(otrng_client_s *client) {
  store_expired_client_profile__called++;
  store_expired_client_profile__called_with = client;
}

static int load_expired_client_profile__called = 0;
static otrng_client_s *load_expired_client_profile__called_with;
static otrng_client_profile_s *load_expired_client_profile__assign = NULL;
static void load_expired_client_profile(otrng_client_s *client) {
  load_expired_client_profile__called++;
  load_expired_client_profile__called_with = client;

  client->exp_client_profile = load_expired_client_profile__assign;
}

static int store_expired_prekey_profile__called = 0;
static otrng_client_s *store_expired_prekey_profile__called_with;
static void store_expired_prekey_profile(otrng_client_s *client) {
  store_expired_prekey_profile__called++;
  store_expired_prekey_profile__called_with = client;
}

static int load_expired_prekey_profile__called = 0;
static otrng_client_s *load_expired_prekey_profile__called_with;
static otrng_prekey_profile_s *load_expired_prekey_profile__assign = NULL;
static void load_expired_prekey_profile(otrng_client_s *client) {
  load_expired_prekey_profile__called++;
  load_expired_prekey_profile__called_with = client;

  client->exp_prekey_profile = load_expired_prekey_profile__assign;
}

static otrng_result
get_account_and_protocol(char **account_name, char **protocol_name,
                         const otrng_client_id_s client_id) {
  *account_name = otrng_xstrdup(client_id.account);
  *protocol_name = otrng_xstrdup(client_id.protocol);
  return OTRNG_SUCCESS;
}

static void create_instag(const otrng_client_id_s client_opdata) {
  (void)client_opdata;
  otrng_client_add_instance_tag(temp_client, 1234);
}

typedef struct orchestration_fixture_s {
  otrng_client_callbacks_s *callbacks;
  otrng_keypair_s *long_term_key;
  otrng_keypair_s *forging_key;
  otrng_global_state_s *gs;
  otrng_client_id_s client_id;
  otrng_client_s *client;
  otrng_client_profile_s *client_profile;
  otrng_prekey_profile_s *prekey_profile;
} orchestration_fixture_s;

static void orchestration_fixture_setup(orchestration_fixture_s *f,
                                        gconstpointer user_data) {
  uint8_t sym1[ED448_PRIVATE_BYTES] = {1};
  uint8_t sym2[ED448_PRIVATE_BYTES] = {2};

  (void)user_data;

  f->callbacks = otrng_xmalloc_z(sizeof(otrng_client_callbacks_s));
  f->gs = otrng_xmalloc_z(sizeof(otrng_global_state_s));
  f->gs->callbacks = f->callbacks;
  f->gs->user_state_v3 = otrl_userstate_create();
  f->client_id.protocol = "test-otr";
  f->client_id.account = "sita@otr.im";

  f->client = otrng_client_new(f->client_id);
  f->client->global_state = f->gs;
  f->gs->clients = otrng_list_add(f->client, f->gs->clients);

  f->callbacks->load_privkey_v4 = load_privkey_v4;
  f->callbacks->store_privkey_v4 = store_privkey_v4;
  f->callbacks->create_privkey_v4 = create_privkey_v4;
  f->callbacks->load_client_profile = load_client_profile;
  f->callbacks->load_prekey_profile = load_prekey_profile;
  f->callbacks->store_client_profile = store_client_profile;
  f->callbacks->store_prekey_profile = store_prekey_profile;
  f->callbacks->load_prekey_messages = load_prekey_messages;
  f->callbacks->store_prekey_messages = store_prekey_messages;
  f->callbacks->load_forging_key = load_forging_key;
  f->callbacks->store_forging_key = store_forging_key;
  f->callbacks->create_forging_key = create_forging_key;
  f->callbacks->create_client_profile = create_client_profile;
  f->callbacks->store_expired_client_profile = store_expired_client_profile;
  f->callbacks->load_expired_client_profile = load_expired_client_profile;
  f->callbacks->store_expired_prekey_profile = store_expired_prekey_profile;
  f->callbacks->load_expired_prekey_profile = load_expired_prekey_profile;
  f->callbacks->create_prekey_profile = create_prekey_profile;
  f->callbacks->get_account_and_protocol = get_account_and_protocol;
  f->callbacks->create_instag = create_instag;

  f->long_term_key = otrng_keypair_new();
  otrng_keypair_generate(f->long_term_key, sym1);

  f->forging_key = otrng_keypair_new();
  otrng_keypair_generate(f->forging_key, sym2);

  f->client_profile = otrng_client_profile_build(1234, "4", f->long_term_key,
                                                 f->forging_key->pub, 20020);
  f->prekey_profile = otrng_prekey_profile_build(1234, f->long_term_key);

  temp_client = f->client;

  load_privkey_v4__called = 0;
  load_privkey_v4__assign = NULL;
  load_privkey_v4__called_with.protocol = NULL;
  load_privkey_v4__called_with.account = NULL;

  store_privkey_v4__called = 0;
  store_privkey_v4__called_with = NULL;

  create_privkey_v4__called = 0;
  create_privkey_v4__assign = NULL;
  create_privkey_v4__called_with.protocol = NULL;
  create_privkey_v4__called_with.account = NULL;

  load_forging_key__called = 0;
  load_forging_key__assign = NULL;
  load_forging_key__called_with = NULL;

  store_forging_key__called = 0;
  store_forging_key__called_with = NULL;

  create_forging_key__called = 0;
  create_forging_key__assign = NULL;
  create_forging_key__called_with.protocol = NULL;
  create_forging_key__called_with.account = NULL;

  load_client_profile__called = 0;
  load_client_profile__assign = NULL;
  load_client_profile__called_with.protocol = NULL;
  load_client_profile__called_with.account = NULL;

  create_client_profile__called = 0;
  create_client_profile__assign = NULL;
  create_client_profile__called_with = NULL;

  store_client_profile__called = 0;
  store_client_profile__called_with = NULL;

  load_expired_client_profile__called = 0;
  load_expired_client_profile__assign = NULL;
  load_expired_client_profile__called_with = NULL;

  store_expired_client_profile__called = 0;
  store_expired_client_profile__called_with = NULL;

  create_prekey_profile__called = 0;
  create_prekey_profile__assign = NULL;
  create_prekey_profile__called_with = NULL;

  store_prekey_profile__called = 0;
  store_prekey_profile__called_with = NULL;

  load_prekey_profile__called = 0;
  load_prekey_profile__assign = NULL;
  load_prekey_profile__called_with.protocol = NULL;
  load_prekey_profile__called_with.account = NULL;

  load_expired_prekey_profile__called = 0;
  load_expired_prekey_profile__assign = NULL;
  load_expired_prekey_profile__called_with = NULL;

  store_expired_prekey_profile__called = 0;
  store_expired_prekey_profile__called_with = NULL;
}

static void orchestration_fixture_teardown(orchestration_fixture_s *f,
                                           gconstpointer user_data) {
  (void)user_data;

  free(f->callbacks);
  otrng_client_free(f->client);
  otrng_list_free_nodes(f->gs->clients);
  otrl_userstate_free(f->gs->user_state_v3);
  free(f->gs);
  free(f->long_term_key);
  free(f->forging_key);
  otrng_client_profile_free(f->client_profile);
  otrng_prekey_profile_free(f->prekey_profile);
}

static otrng_client_profile_s *
create_client_profile_copy_from(const otrng_client_profile_s *src) {
  otrng_client_profile_s *result =
      otrng_xmalloc_z(sizeof(otrng_client_profile_s));
  otrng_client_profile_copy(result, src);
  return result;
}

static otrng_prekey_profile_s *
create_prekey_profile_copy_from(const otrng_prekey_profile_s *src) {
  otrng_prekey_profile_s *result =
      otrng_xmalloc_z(sizeof(otrng_prekey_profile_s));
  otrng_prekey_profile_copy(result, src);
  return result;
}

static void test__otrng_client_ensure_correct_state__creates_new_long_term_key(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;

  create_privkey_v4__assign = f->long_term_key;
  f->client->client_profile =
      create_client_profile_copy_from(f->client_profile);
  f->client->prekey_profile =
      create_prekey_profile_copy_from(f->prekey_profile);

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_privkey_v4__called, ==, 1);
  g_assert_cmpstr(load_privkey_v4__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_privkey_v4__called_with.account, ==, "sita@otr.im");

  g_assert_cmpint(create_privkey_v4__called, ==, 1);
  g_assert_cmpstr(create_privkey_v4__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(create_privkey_v4__called_with.account, ==, "sita@otr.im");

  g_assert_cmpint(store_privkey_v4__called, ==, 1);
  g_assert(store_privkey_v4__called_with == f->client);

  g_assert(f->client->client_profile == NULL);
  g_assert(f->client->prekey_profile == NULL);

  f->client->keypair = NULL;
}

static void
test__otrng_client_ensure_correct_state__fails_creating_long_term_key(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_client_profile_s *copy_cp =
      create_client_profile_copy_from(f->client_profile);
  otrng_prekey_profile_s *copy_pp =
      create_prekey_profile_copy_from(f->prekey_profile);

  (void)data;

  f->client->client_profile = copy_cp;
  f->client->prekey_profile = copy_pp;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_privkey_v4__called, ==, 1);
  g_assert_cmpstr(load_privkey_v4__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_privkey_v4__called_with.account, ==, "sita@otr.im");

  g_assert_cmpint(create_privkey_v4__called, ==, 1);
  g_assert_cmpstr(create_privkey_v4__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(create_privkey_v4__called_with.account, ==, "sita@otr.im");

  g_assert_cmpint(store_privkey_v4__called, ==, 0);

  g_assert(f->client->keypair == NULL);

  g_assert(f->client->client_profile == copy_cp);
  g_assert(f->client->prekey_profile == copy_pp);
}

static void test__otrng_client_ensure_correct_state__loads_long_term_key(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;

  load_privkey_v4__assign = f->long_term_key;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_privkey_v4__called, ==, 1);
  g_assert_cmpstr(load_privkey_v4__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_privkey_v4__called_with.account, ==, "sita@otr.im");

  g_assert_cmpint(create_privkey_v4__called, ==, 0);
  g_assert_cmpint(store_privkey_v4__called, ==, 0);

  f->client->keypair = NULL;
}

static void
test__otrng_client_ensure_correct_state__doesnt_load_long_term_key_if_already_exists(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_privkey_v4__called, ==, 0);
  g_assert_cmpint(create_privkey_v4__called, ==, 0);
  g_assert_cmpint(store_privkey_v4__called, ==, 0);

  f->client->keypair = NULL;
}

static void test__otrng_client_ensure_correct_state__forging_key__ensures(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_forging_key__called, ==, 0);
  g_assert_cmpint(create_forging_key__called, ==, 0);
  g_assert_cmpint(store_forging_key__called, ==, 0);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
}

static void test__otrng_client_ensure_correct_state__forging_key__loads(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  load_forging_key__assign = &f->forging_key->pub;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_forging_key__called, ==, 1);
  g_assert(load_forging_key__called_with == f->client);

  g_assert_cmpint(create_forging_key__called, ==, 0);
  g_assert_cmpint(store_forging_key__called, ==, 0);

  g_assert(f->client->forging_key == &f->forging_key->pub);

  f->client->forging_key = NULL;
  f->client->keypair = NULL;
}

static void test__otrng_client_ensure_correct_state__forging_key__creates(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  create_forging_key__assign = &f->forging_key->pub;
  f->client->client_profile =
      create_client_profile_copy_from(f->client_profile);

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_forging_key__called, ==, 1);
  g_assert(load_forging_key__called_with == f->client);

  g_assert_cmpint(create_forging_key__called, ==, 1);
  g_assert_cmpstr(create_forging_key__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(create_forging_key__called_with.account, ==, "sita@otr.im");

  g_assert_cmpint(store_forging_key__called, ==, 1);
  g_assert(store_forging_key__called_with == f->client);

  g_assert(f->client->forging_key == &f->forging_key->pub);

  g_assert(f->client->client_profile == NULL);

  f->client->forging_key = NULL;
  f->client->keypair = NULL;
}

static void test__otrng_client_ensure_correct_state__forging_key__fails(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->client_profile = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_forging_key__called, ==, 1);
  g_assert(load_forging_key__called_with == f->client);

  g_assert_cmpint(create_forging_key__called, ==, 1);
  g_assert_cmpstr(create_forging_key__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(create_forging_key__called_with.account, ==, "sita@otr.im");

  g_assert_cmpint(store_forging_key__called, ==, 0);

  g_assert(f->client->forging_key == NULL);

  g_assert(f->client->client_profile == f->client_profile);

  f->client->client_profile = NULL;
  f->client->forging_key = NULL;
  f->client->keypair = NULL;
}

static void test__otrng_client_ensure_correct_state__client_profile__ensures(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 0);
  g_assert_cmpint(create_client_profile__called, ==, 0);
  g_assert_cmpint(store_client_profile__called, ==, 0);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void test__otrng_client_ensure_correct_state__client_profile__loads(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  load_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpstr(load_client_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_client_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_client_profile__called, ==, 0);
  g_assert_cmpint(store_client_profile__called, ==, 0);
  g_assert(f->client->client_profile == f->client_profile);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void test__otrng_client_ensure_correct_state__client_profile__creates(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  create_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpint(create_client_profile__called, ==, 1);
  g_assert_cmpint(store_client_profile__called, ==, 1);

  g_assert(f->client->client_profile == f->client_profile);
  g_assert(f->client->client_profile->should_publish == otrng_true);
  g_assert(f->client->should_publish == otrng_true);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void test__otrng_client_ensure_correct_state__client_profile__fails(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpint(create_client_profile__called, ==, 1);
  g_assert_cmpint(store_client_profile__called, ==, 0);

  g_assert(f->client->client_profile == NULL);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__client_profile__loads_when_wrong_key(
    orchestration_fixture_s *f, gconstpointer data) {
  uint8_t sym3[ED448_PRIVATE_BYTES] = {3};
  otrng_client_profile_s *client_profile2;
  otrng_keypair_s *long_term2 = otrng_keypair_new();

  (void)data;

  f->client->keypair = f->long_term_key;
  otrng_keypair_generate(long_term2, sym3);
  client_profile2 = otrng_client_profile_build(1234, "4", long_term2,
                                               f->forging_key->pub, 20020);
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = client_profile2;
  load_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpstr(load_client_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_client_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_client_profile__called, ==, 0);
  g_assert_cmpint(store_client_profile__called, ==, 0);
  g_assert(f->client->client_profile == f->client_profile);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  otrng_keypair_free(long_term2);
}

static void
test__otrng_client_ensure_correct_state__client_profile__loads_when_wrong_forging_key(
    orchestration_fixture_s *f, gconstpointer data) {
  uint8_t sym3[ED448_PRIVATE_BYTES] = {3};
  otrng_client_profile_s *client_profile2;
  otrng_keypair_s *forging2 = otrng_keypair_new();

  (void)data;

  f->client->keypair = f->long_term_key;
  otrng_keypair_generate(forging2, sym3);
  client_profile2 = otrng_client_profile_build(1234, "4", f->long_term_key,
                                               forging2->pub, 20020);
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = client_profile2;
  load_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpstr(load_client_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_client_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_client_profile__called, ==, 0);
  g_assert_cmpint(store_client_profile__called, ==, 0);
  g_assert(f->client->client_profile == f->client_profile);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  otrng_keypair_free(forging2);
}

static void
test__otrng_client_ensure_correct_state__client_profile__loads_when_wrong_instance_tag(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_client_profile_s *client_profile2;
  (void)data;

  f->client->keypair = f->long_term_key;
  client_profile2 = otrng_client_profile_build(4234, "4", f->long_term_key,
                                               f->forging_key->pub, 20020);
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = client_profile2;
  load_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpstr(load_client_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_client_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_client_profile__called, ==, 0);
  g_assert_cmpint(store_client_profile__called, ==, 0);
  g_assert(f->client->client_profile == f->client_profile);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__client_profile__loads_and_moves_when_expired(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_client_profile_s *client_profile2;
  (void)data;

  client_profile2 = otrng_client_profile_build(1234, "4", f->long_term_key,
                                               f->forging_key->pub, 20020);
  client_profile2->expires = time(NULL) - 100;
  client_profile_sign(client_profile2, f->long_term_key);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = client_profile2;
  f->client->exp_client_profile = NULL;
  load_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpstr(load_client_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_client_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_client_profile__called, ==, 0);
  g_assert_cmpint(store_client_profile__called, ==, 0);
  g_assert_cmpint(store_expired_client_profile__called, ==, 1);
  g_assert(f->client->client_profile == f->client_profile);
  g_assert(f->client->exp_client_profile == client_profile2);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__client_profile__loads_and_moves_when_close_to_expiring(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_client_profile_s *client_profile2;
  (void)data;

  client_profile2 = otrng_client_profile_build(1234, "4", f->long_term_key,
                                               f->forging_key->pub, 20020);
  client_profile2->expires = time(NULL) + 100;
  client_profile_sign(client_profile2, f->long_term_key);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = client_profile2;
  f->client->exp_client_profile = NULL;
  load_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpstr(load_client_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_client_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_client_profile__called, ==, 0);
  g_assert_cmpint(store_client_profile__called, ==, 0);
  g_assert_cmpint(store_expired_client_profile__called, ==, 1);
  g_assert(f->client->client_profile == f->client_profile);
  g_assert(f->client->exp_client_profile == client_profile2);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__client_profile__creates_and_moves_when_expired(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_client_profile_s *client_profile2;
  (void)data;

  client_profile2 = otrng_client_profile_build(1234, "4", f->long_term_key,
                                               f->forging_key->pub, 20020);
  client_profile2->expires = time(NULL) - 100;
  client_profile_sign(client_profile2, f->long_term_key);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = NULL;
  f->client->exp_client_profile = NULL;
  load_client_profile__assign = client_profile2;
  create_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpstr(load_client_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_client_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_client_profile__called, ==, 1);
  g_assert_cmpint(store_client_profile__called, ==, 1);
  g_assert_cmpint(store_expired_client_profile__called, ==, 1);
  g_assert(f->client->client_profile == f->client_profile);
  g_assert(f->client->exp_client_profile == client_profile2);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__client_profile__creates_and_moves_when_close_to_expiring(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_client_profile_s *client_profile2;
  otrng_client_profile_s *client_profile3;
  (void)data;

  client_profile2 = otrng_client_profile_build(1234, "4", f->long_term_key,
                                               f->forging_key->pub, 20020);
  client_profile2->expires = time(NULL) + 100;
  client_profile_sign(client_profile2, f->long_term_key);

  client_profile3 = otrng_client_profile_build(1234, "4", f->long_term_key,
                                               f->forging_key->pub, 1);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = NULL;
  f->client->exp_client_profile = client_profile3;
  load_client_profile__assign = client_profile2;
  create_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_client_profile__called, ==, 1);
  g_assert_cmpstr(load_client_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_client_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_client_profile__called, ==, 1);
  g_assert_cmpint(store_client_profile__called, ==, 1);
  g_assert_cmpint(store_expired_client_profile__called, ==, 1);
  g_assert(f->client->client_profile == f->client_profile);
  g_assert(f->client->exp_client_profile == client_profile2);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void test__otrng_client_ensure_correct_state__prekey_profile__ensures(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;

  f->client->prekey_profile = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 0);
  g_assert_cmpint(create_prekey_profile__called, ==, 0);
  g_assert_cmpint(store_prekey_profile__called, ==, 0);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
}

static void test__otrng_client_ensure_correct_state__prekey_profile__loads(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;

  load_prekey_profile__assign = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 1);
  g_assert_cmpstr(load_prekey_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_prekey_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_prekey_profile__called, ==, 0);
  g_assert_cmpint(store_prekey_profile__called, ==, 0);
  g_assert(f->client->prekey_profile == f->prekey_profile);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
}

static void test__otrng_client_ensure_correct_state__prekey_profile__creates(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  create_prekey_profile__assign = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 1);
  g_assert_cmpint(create_prekey_profile__called, ==, 1);
  g_assert_cmpint(store_prekey_profile__called, ==, 1);

  g_assert(f->client->prekey_profile == f->prekey_profile);
  g_assert(f->client->prekey_profile->should_publish == otrng_true);
  g_assert(f->client->should_publish == otrng_true);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
}

static void test__otrng_client_ensure_correct_state__prekey_profile__fails(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;
  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 1);
  g_assert_cmpint(create_prekey_profile__called, ==, 1);
  g_assert_cmpint(store_prekey_profile__called, ==, 0);

  g_assert(f->client->prekey_profile == NULL);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__prekey_profile__loads_when_wrong_key(
    orchestration_fixture_s *f, gconstpointer data) {
  uint8_t sym3[ED448_PRIVATE_BYTES] = {3};
  otrng_prekey_profile_s *prekey_profile2;
  otrng_keypair_s *long_term2 = otrng_keypair_new();

  (void)data;

  f->client->keypair = f->long_term_key;
  otrng_keypair_generate(long_term2, sym3);
  prekey_profile2 = otrng_prekey_profile_build(1234, long_term2);

  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = prekey_profile2;

  load_prekey_profile__assign = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 1);
  g_assert_cmpstr(load_prekey_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_prekey_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_prekey_profile__called, ==, 0);
  g_assert_cmpint(store_prekey_profile__called, ==, 0);
  g_assert(f->client->prekey_profile == f->prekey_profile);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
  otrng_keypair_free(long_term2);
}

static void
test__otrng_client_ensure_correct_state__prekey_profile__loads_when_wrong_instance_tag(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_prekey_profile_s *prekey_profile2;
  (void)data;

  f->client->keypair = f->long_term_key;
  prekey_profile2 = otrng_prekey_profile_build(4234, f->long_term_key);
  f->client->forging_key = &f->forging_key->pub;
  f->client->prekey_profile = prekey_profile2;
  f->client->client_profile = f->client_profile;
  load_prekey_profile__assign = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 1);
  g_assert_cmpstr(load_prekey_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_prekey_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_prekey_profile__called, ==, 0);
  g_assert_cmpint(store_prekey_profile__called, ==, 0);
  g_assert(f->client->prekey_profile == f->prekey_profile);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__prekey_profile__loads_and_moves_when_expired(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_prekey_profile_s *prekey_profile2;
  (void)data;

  prekey_profile2 = otrng_prekey_profile_build(1234, f->long_term_key);
  prekey_profile2->expires = time(NULL) - 100;
  otrng_prekey_profile_sign(prekey_profile2, f->long_term_key);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = prekey_profile2;
  f->client->exp_prekey_profile = NULL;
  load_prekey_profile__assign = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 1);
  g_assert_cmpstr(load_prekey_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_prekey_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_prekey_profile__called, ==, 0);
  g_assert_cmpint(store_prekey_profile__called, ==, 0);
  g_assert_cmpint(store_expired_prekey_profile__called, ==, 1);
  g_assert(f->client->prekey_profile == f->prekey_profile);
  g_assert(f->client->exp_prekey_profile == prekey_profile2);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__prekey_profile__loads_and_moves_when_close_to_expiring(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_prekey_profile_s *prekey_profile2;
  (void)data;

  prekey_profile2 = otrng_prekey_profile_build(1234, f->long_term_key);
  prekey_profile2->expires = time(NULL) + 100;
  otrng_prekey_profile_sign(prekey_profile2, f->long_term_key);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = prekey_profile2;
  f->client->exp_prekey_profile = NULL;
  load_prekey_profile__assign = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 1);
  g_assert_cmpstr(load_prekey_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_prekey_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_prekey_profile__called, ==, 0);
  g_assert_cmpint(store_prekey_profile__called, ==, 0);
  g_assert_cmpint(store_expired_prekey_profile__called, ==, 1);
  g_assert(f->client->prekey_profile == f->prekey_profile);
  g_assert(f->client->exp_prekey_profile == prekey_profile2);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__prekey_profile__creates_and_moves_when_expired(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_prekey_profile_s *prekey_profile2;
  (void)data;

  prekey_profile2 = otrng_prekey_profile_build(1234, f->long_term_key);
  prekey_profile2->expires = time(NULL) - 100;
  otrng_prekey_profile_sign(prekey_profile2, f->long_term_key);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = NULL;
  f->client->exp_prekey_profile = NULL;
  load_prekey_profile__assign = prekey_profile2;
  create_prekey_profile__assign = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 1);
  g_assert_cmpstr(load_prekey_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_prekey_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_prekey_profile__called, ==, 1);
  g_assert_cmpint(store_prekey_profile__called, ==, 1);
  g_assert_cmpint(store_expired_prekey_profile__called, ==, 1);
  g_assert(f->client->prekey_profile == f->prekey_profile);
  g_assert(f->client->exp_prekey_profile == prekey_profile2);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__prekey_profile__creates_and_moves_when_close_to_expiring(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_prekey_profile_s *prekey_profile2;
  otrng_prekey_profile_s *prekey_profile3;
  (void)data;

  prekey_profile2 = otrng_prekey_profile_build(1234, f->long_term_key);
  prekey_profile2->expires = time(NULL) + 100;
  otrng_prekey_profile_sign(prekey_profile2, f->long_term_key);

  prekey_profile3 = otrng_prekey_profile_build(1234, f->long_term_key);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = NULL;
  f->client->exp_prekey_profile = prekey_profile3;
  load_prekey_profile__assign = prekey_profile2;
  create_prekey_profile__assign = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_prekey_profile__called, ==, 1);
  g_assert_cmpstr(load_prekey_profile__called_with.protocol, ==, "test-otr");
  g_assert_cmpstr(load_prekey_profile__called_with.account, ==, "sita@otr.im");
  g_assert_cmpint(create_prekey_profile__called, ==, 1);
  g_assert_cmpint(store_prekey_profile__called, ==, 1);
  g_assert_cmpint(store_expired_prekey_profile__called, ==, 1);
  g_assert(f->client->prekey_profile == f->prekey_profile);
  g_assert(f->client->exp_prekey_profile == prekey_profile2);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__expired_client_profile__ensures(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = f->prekey_profile;
  f->client->exp_client_profile = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_expired_client_profile__called, ==, 0);
  g_assert_cmpint(store_expired_client_profile__called, ==, 0);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
  f->client->exp_client_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__expired_client_profile__loads(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_client_profile_s *client_profile2;
  (void)data;

  client_profile2 = otrng_client_profile_build(1234, "4", f->forging_key,
                                               f->forging_key->pub, 20020);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = f->prekey_profile;
  f->client->exp_client_profile = client_profile2;
  load_expired_client_profile__assign = f->client_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_expired_client_profile__called, ==, 1);
  g_assert_cmpint(store_expired_client_profile__called, ==, 0);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
  f->client->exp_client_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__expired_client_profile__stores_empty(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_client_profile_s *client_profile2;
  otrng_client_profile_s *client_profile3;
  (void)data;

  client_profile2 = otrng_client_profile_build(1234, "4", f->forging_key,
                                               f->forging_key->pub, 20020);

  client_profile3 = otrng_client_profile_build(1234, "4", f->long_term_key,
                                               f->long_term_key->pub, 20020);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = f->prekey_profile;
  f->client->exp_client_profile = client_profile2;
  load_expired_client_profile__assign = client_profile3;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_expired_client_profile__called, ==, 1);
  g_assert_cmpint(store_expired_client_profile__called, ==, 1);

  g_assert(f->client->exp_client_profile == NULL);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
  f->client->exp_client_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__expired_prekey_profile__ensures(
    orchestration_fixture_s *f, gconstpointer data) {
  (void)data;

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = f->prekey_profile;
  f->client->exp_client_profile = f->client_profile;
  f->client->exp_prekey_profile = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_expired_prekey_profile__called, ==, 0);
  g_assert_cmpint(store_expired_prekey_profile__called, ==, 0);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
  f->client->exp_client_profile = NULL;
  f->client->exp_prekey_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__expired_prekey_profile__loads(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_prekey_profile_s *prekey_profile2;
  (void)data;

  prekey_profile2 = otrng_prekey_profile_build(1234, f->forging_key);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = f->prekey_profile;
  f->client->exp_client_profile = f->client_profile;
  f->client->exp_prekey_profile = prekey_profile2;
  load_expired_prekey_profile__assign = f->prekey_profile;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_expired_prekey_profile__called, ==, 1);
  g_assert_cmpint(store_expired_prekey_profile__called, ==, 0);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
  f->client->exp_client_profile = NULL;
  f->client->exp_prekey_profile = NULL;
}

static void
test__otrng_client_ensure_correct_state__expired_prekey_profile__stores_empty(
    orchestration_fixture_s *f, gconstpointer data) {
  otrng_prekey_profile_s *prekey_profile2;
  otrng_prekey_profile_s *prekey_profile3;
  (void)data;

  prekey_profile2 = otrng_prekey_profile_build(1234, f->forging_key);

  prekey_profile3 = otrng_prekey_profile_build(4234, f->long_term_key);

  f->client->keypair = f->long_term_key;
  f->client->forging_key = &f->forging_key->pub;
  f->client->client_profile = f->client_profile;
  f->client->prekey_profile = f->prekey_profile;
  f->client->exp_client_profile = f->client_profile;
  f->client->exp_prekey_profile = prekey_profile2;
  load_expired_prekey_profile__assign = prekey_profile3;

  otrng_client_ensure_correct_state(f->client);

  g_assert_cmpint(load_expired_prekey_profile__called, ==, 1);
  g_assert_cmpint(store_expired_prekey_profile__called, ==, 1);

  g_assert(f->client->exp_prekey_profile == NULL);

  f->client->keypair = NULL;
  f->client->forging_key = NULL;
  f->client->client_profile = NULL;
  f->client->prekey_profile = NULL;
  f->client->exp_client_profile = NULL;
  f->client->exp_prekey_profile = NULL;
}

#define WITH_O_FIXTURE(_p, _c)                                                 \
  WITH_FIXTURE(_p, _c, orchestration_fixture_s, orchestration_fixture)

void units_orchestration_add_tests(void) {
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/long_term_key/creates",
      test__otrng_client_ensure_correct_state__creates_new_long_term_key);
  WITH_O_FIXTURE("/orchestration/ensure_correct_state/long_term_key/loads",
                 test__otrng_client_ensure_correct_state__loads_long_term_key);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/long_term_key/ensures",
      test__otrng_client_ensure_correct_state__doesnt_load_long_term_key_if_already_exists);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/long_term_key/fails",
      test__otrng_client_ensure_correct_state__fails_creating_long_term_key);

  WITH_O_FIXTURE("/orchestration/ensure_correct_state/forging_key/ensures",
                 test__otrng_client_ensure_correct_state__forging_key__ensures);
  WITH_O_FIXTURE("/orchestration/ensure_correct_state/forging_key/loads",
                 test__otrng_client_ensure_correct_state__forging_key__loads);
  WITH_O_FIXTURE("/orchestration/ensure_correct_state/forging_key/creates",
                 test__otrng_client_ensure_correct_state__forging_key__creates);
  WITH_O_FIXTURE("/orchestration/ensure_correct_state/forging_key/fails",
                 test__otrng_client_ensure_correct_state__forging_key__fails);

  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/ensures",
      test__otrng_client_ensure_correct_state__client_profile__ensures);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/loads",
      test__otrng_client_ensure_correct_state__client_profile__loads);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/loads_when_wrong_key",
      test__otrng_client_ensure_correct_state__client_profile__loads_when_wrong_key);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/"
      "loads_when_wrong_forging_key",
      test__otrng_client_ensure_correct_state__client_profile__loads_when_wrong_forging_key);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/"
      "loads_when_wrong_instance_tag",
      test__otrng_client_ensure_correct_state__client_profile__loads_when_wrong_instance_tag);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/"
      "loads_and_moves_when_expired",
      test__otrng_client_ensure_correct_state__client_profile__loads_and_moves_when_expired);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/"
      "loads_and_moves_when_close_to_expiring",
      test__otrng_client_ensure_correct_state__client_profile__loads_and_moves_when_close_to_expiring);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/"
      "creates_and_moves_when_expired",
      test__otrng_client_ensure_correct_state__client_profile__creates_and_moves_when_expired);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/"
      "creates_and_moves_when_close_to_expiring",
      test__otrng_client_ensure_correct_state__client_profile__creates_and_moves_when_close_to_expiring);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/creates",
      test__otrng_client_ensure_correct_state__client_profile__creates);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/client_profile/fails",
      test__otrng_client_ensure_correct_state__client_profile__fails);

  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/ensures",
      test__otrng_client_ensure_correct_state__prekey_profile__ensures);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/loads",
      test__otrng_client_ensure_correct_state__prekey_profile__loads);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/loads_when_wrong_key",
      test__otrng_client_ensure_correct_state__prekey_profile__loads_when_wrong_key);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/"
      "loads_when_wrong_instance_tag",
      test__otrng_client_ensure_correct_state__prekey_profile__loads_when_wrong_instance_tag);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/"
      "loads_and_moves_when_expired",
      test__otrng_client_ensure_correct_state__prekey_profile__loads_and_moves_when_expired);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/"
      "loads_and_moves_when_close_to_expiring",
      test__otrng_client_ensure_correct_state__prekey_profile__loads_and_moves_when_close_to_expiring);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/"
      "creates_and_moves_when_expired",
      test__otrng_client_ensure_correct_state__prekey_profile__creates_and_moves_when_expired);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/"
      "creates_and_moves_when_close_to_expiring",
      test__otrng_client_ensure_correct_state__prekey_profile__creates_and_moves_when_close_to_expiring);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/creates",
      test__otrng_client_ensure_correct_state__prekey_profile__creates);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/prekey_profile/fails",
      test__otrng_client_ensure_correct_state__prekey_profile__fails);

  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/expired_client_profile/ensures",
      test__otrng_client_ensure_correct_state__expired_client_profile__ensures);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/expired_client_profile/loads",
      test__otrng_client_ensure_correct_state__expired_client_profile__loads);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/expired_client_profile/stores_empty",
      test__otrng_client_ensure_correct_state__expired_client_profile__stores_empty);

  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/expired_prekey_profile/ensures",
      test__otrng_client_ensure_correct_state__expired_prekey_profile__ensures);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/expired_prekey_profile/loads",
      test__otrng_client_ensure_correct_state__expired_prekey_profile__loads);
  WITH_O_FIXTURE(
      "/orchestration/ensure_correct_state/expired_prekey_profile/stores_empty",
      test__otrng_client_ensure_correct_state__expired_prekey_profile__stores_empty);
}