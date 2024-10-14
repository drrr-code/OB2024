//
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "common/log/log.h"
// #include "sql/stmt/filter_stmt.h"

UpdateStmt::UpdateStmt(Table *table,std::vector<FieldMeta> fields,std::vector<std::unique_ptr<Expression>> values ,FilterStmt *filter_stmt)
    : table_(table),fields_(std::move(fields)), filter_stmt_(filter_stmt),values_(std::move(values))
{}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  // 查看表是否存在
  const char *table_name=update.relation_name.c_str();
  Table *table=db->find_table(table_name);
  if(table==nullptr){
    LOG_WARN("table not exist. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  // stmt = nullptr;
  //update t1 set c1=1;
  //检查t1表里面有没有c1列
  //检查c1列的类型和右值是否匹配
  const TableMeta &table_meta=table->table_meta();
  //当前仅支持一个字段
  const FieldMeta* update_field=table_meta.field(update.attribute_name.c_str());
  bool valid=false;
  if(update_field!=nullptr){
    if(update.value.attr_type()==update_field->type()){
      if(update_field->type()==AttrType::CHARS&&update_field->len()<update.value.length()){
        LOG_WARN("update field chars with longer length");
      }else{
        valid=true;
      }
      //将不定长度的char转化为定长char，便于管理
      if(valid&&update_field->type()==AttrType::CHARS){
        char* char_value=(char*)malloc(update_field->len());
        memset(char_value, 0, update_field->len());
        memcpy(char_value, update.value.data(), update_field->len());
        const_cast<Value&>(update.value).set_data(char_value, update_field->len());
        free(char_value);
      }else if(const_cast<Value&>(update.value).type_can_cast(update_field->type())!=RC::SUCCESS){
        LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
            table->name(), update_field->name(), update_field->type(), (update.value).attr_type());
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }else{
        valid=true;
      }
    }else{
      // valid=true;
    }
  }
  if(!valid){
      LOG_WARN("update field type mismatch. table=%s", table_name);
      return RC::INVALID_ARGUMENT;
  }
  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(std::string(table_name), table));
  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(
      db, table, &table_map, update.conditions.data(), static_cast<int>(update.conditions.size()), filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
    return rc;
  }
  // stmt = new UpdateStmt(table, update.value, 1);
  return RC::SUCCESS;
}

StmtType UpdateStmt::type() const{
  return StmtType::UPDATE;
}