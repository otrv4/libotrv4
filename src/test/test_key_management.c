#include "../key_management.h"

void test_derive_ratchet_keys()
{
	shared_secret_t shared = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	ratchet_t ratchet;
	derive_ratchet_keys(&ratchet, shared);

	root_key_t expected_root_key;

	gcry_md_hd_t sha3_512;
	uint8_t magic[3] = { 0x01, 0x02, 0x03 };
	gcry_md_open(&sha3_512, GCRY_MD_SHA3_512, GCRY_MD_FLAG_SECURE);
	gcry_md_write(sha3_512, &magic[0], 1);
	gcry_md_write(sha3_512, shared, sizeof(shared_secret_t));
	memcpy(expected_root_key, gcry_md_read(sha3_512, 0),
	       sizeof(root_key_t));
	gcry_md_close(sha3_512);

	otrv4_assert_cmpmem(expected_root_key, ratchet.root_key,
			    sizeof(root_key_t));
}

void test_key_manager_destroy()
{
	key_manager_t *manager = malloc(sizeof(key_manager_t));
	key_manager_init(manager);

	shared_secret_t shared = { // TODO: is there a simpler way to write this?
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	key_manager_new_ratchet(manager, shared);
	key_manager_new_ratchet(manager, shared); //ran a second time to fill manager->previous

	dh_init();
	bool ok = dh_keypair_generate(manager->our_dh);
	otrv4_assert(ok);

	manager->their_dh = dh_mpi_copy(manager->our_dh->pub);

	otrv4_assert(manager->current);
	otrv4_assert(manager->previous);
	otrv4_assert(manager->our_dh->priv);
	otrv4_assert(manager->our_dh->pub);
	otrv4_assert(manager->their_dh);
	// TODO: destroy mix_key too?
	// TODO: test destroy ecdh keys?

	key_manager_destroy(manager);

	otrv4_assert(!manager->current);
	otrv4_assert(!manager->previous);
	otrv4_assert(!manager->our_dh->priv);
	otrv4_assert(!manager->our_dh->pub);
	otrv4_assert(!manager->their_dh);

	dh_free();
	free(manager);
}
