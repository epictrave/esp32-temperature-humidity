// Copyright (c) Janghun LEE. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "temp_humid.h"
#include "string.h"
#include "time.h"

static char* partition_key[128];
char* temp_humid_make_message(float temperature, float humidity) {
  time_t current_time;
  time(&current_time);
  current_time = current_time - current_time % 60;
  struct tm* time_info;
  time_info = localtime(&current_time);
  char format_time[40];
  strftime(format_time, 40, "%FT%TZ", time_info);

  JSON_Value* root_value = json_value_init_object();
  JSON_Object* root_object = json_value_get_object(root_value);

  json_object_set_string(root_object, "partitionKey", partition_key);
  json_object_set_string(root_object, "rowKey", format_time);
  json_object_set_number(root_object, "temperature", temperature);
  json_object_set_number(root_object, "humidity", humidity);
  char* result = json_serialize_to_string(root_value);
  json_value_free(root_value);
  return result;
}

void temp_humid_parse_from_json(const char* json,
                                DEVICE_TWIN_STATE update_state) {
  JSON_Value* root_value = json_parse_string(json);
  JSON_Object* root_object = json_value_get_object(root_value);
  char property_name[40];
  if (update_state == UPDATE_PARTIAL) {
    sprintf(property_name, "partitionKey");
  } else if (update_state == UPDATE_COMPLETE) {
    sprintf(property_name, "desired.partitionKey");
  }
  if (json_object_dotget_value(root_object, property_name) != NULL) {
    const char* key = json_object_dotget_string(root_object, property_name);
    memset(partition_key, 0, sizeof(partition_key));
    memcpy(partition_key, key, strlen(key));
  }
  json_value_free(root_value);
}
