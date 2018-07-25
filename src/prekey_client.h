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

#ifndef OTRNG_PREKEY_CLIENT_H
#define OTRNG_PREKEY_CLIENT_H

#include <stdint.h>

#include "auth.h"
#include "client_profile.h"
#include "shared.h"

typedef struct {
  uint32_t client_instance_tag;
  client_profile_p client_profile;
  ec_point_p I;
} otrng_prekey_dake1_message_s;

typedef struct {
  uint32_t client_instance_tag;
  uint8_t *server_identity;
  ec_point_p S;
  ring_sig_p sigma;
} otrng_prekey_dake2_message_s;

typedef struct {
  uint32_t client_instance_tag;
  ring_sig_p sigma;
  uint8_t *message;
  size_t message_len;
} otrng_prekey_dake3_message_s;

typedef struct {
} otrng_prekey_client_s;

API char *
otrng_prekey_client_request_storage_status(otrng_prekey_client_s *client);

#endif