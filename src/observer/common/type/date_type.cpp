/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/date_type.h"
#include "common/value.h"

int DateType::compare(const Value &left, const Value &right) const
{
  // if (right.attr_type() == AttrType::INTS) {
  //   return common::compare_int((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);
  // } else if (right.attr_type() == AttrType::FLOATS) {
  //   return common::compare_float((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);
  // }
  // return INT32_MAX;
  return common::compare_int((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);
}

RC DateType::add(const Value &left, const Value &right, Value &result) const
{
  result.set_int(left.get_int() + right.get_int());
  return RC::SUCCESS;
}

RC DateType::subtract(const Value &left, const Value &right, Value &result) const
{
  result.set_int(left.get_int() - right.get_int());
  return RC::SUCCESS;
}

RC DateType::multiply(const Value &left, const Value &right, Value &result) const
{
  result.set_int(left.get_int() * right.get_int());
  return RC::SUCCESS;
}

RC DateType::negative(const Value &val, Value &result) const
{
  result.set_int(-val.get_int());
  return RC::SUCCESS;
}

RC DateType::set_value_from_str(Value &val, const string &data) const
{
  RC                rc = RC::SUCCESS;
  stringstream deserialize_stream;
  deserialize_stream.clear();  // 清理stream的状态，防止多次解析出现异常
  deserialize_stream.str(data);
  int int_value;
  deserialize_stream >> int_value;
  if (!deserialize_stream || !deserialize_stream.eof()) {
    rc = RC::SCHEMA_FIELD_TYPE_MISMATCH;
  } else {
    val.set_int(int_value);
  }
  return rc;
}

RC DateType::to_string(const Value &val, string &result) const
{
  stringstream ss;
   char buf[16] = {0};
  snprintf(buf,sizeof(buf),"%04d-%02d-%02d",val.value_.int_value_/10000,    (val.value_.int_value_%10000)/100,val.value_.int_value_%100); // 注意这里月份和天数，不足两位时需要填充0
  ss << buf;
  result = ss.str();
  return RC::SUCCESS;
}
RC DateType::data_max(const Value &left, const Value &right, Value &result)const
{
  result.set_int(std::max(left.get_int(), right.get_int()));
  return RC::SUCCESS;
}

RC DateType::data_min(const Value &left, const Value &right, Value &result)const
{
  result.set_int(std::min(left.get_int(), right.get_int()));
  return RC::SUCCESS;
}