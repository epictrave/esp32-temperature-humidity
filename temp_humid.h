// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef TEMP_HUMID_H
#define TEMP_HUMID_H

#include "device_twin_state.h"
#include "dht.h"
#include "parson.h"

char* temp_humid_make_message(float temperature, float humidity);
void temp_humid_parse_from_json(const char* json,
                                DEVICE_TWIN_STATE update_state);
#endif /* TEMP_HUMID_H */