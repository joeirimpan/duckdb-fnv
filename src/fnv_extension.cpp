#define DUCKDB_EXTENSION_MAIN

#include "fnv_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// fnv constants
const uint32_t initial_hash = 0x811c9dc5; // FNV offset basis
const uint32_t FNV_prime = 0x01000193;    // FNV Prime

// fnvHash returns the FNV-1a hash of the given key
uint32_t fnvHash(const std::string &key) {
  uint32_t hash = initial_hash;
  for (auto c : key) {
    hash = hash ^ (c);
    hash = hash * FNV_prime;
  }
  return hash;
}

// fnv_partition returns the partition number for the given key
static int32_t fnv_partition(const std::string &key, int32_t partitions) {
  uint32_t hash = fnvHash(key);
  return int32_t(hash % partitions);
}

namespace duckdb {

inline void FnvScalarFun(DataChunk &args, ExpressionState &state,
                         Vector &result) {
  auto &name_vector = args.data[0];
  UnaryExecutor::Execute<string_t, int32_t>(
      name_vector, result, args.size(),
      [&](string_t name) {
        return fnvHash(name.GetString());
      });
}

inline void FnvPartitionScalarFun(DataChunk &args, ExpressionState &state,
                         Vector &result) {
  auto &name_vector = args.data[0];
  auto &parts_vector = args.data[1];
  BinaryExecutor::Execute<string_t, int32_t, int32_t>(
      name_vector, parts_vector, result, args.size(),
      [&](string_t name, int32_t parts) {
        return fnv_partition(name.GetString(), parts);
      });
}

static void LoadInternal(DatabaseInstance &instance) {
  // Register a scalar function
  auto fnv_scalar_function =
      ScalarFunction("fnv", {LogicalType::VARCHAR},
                     LogicalType::INTEGER, FnvScalarFun);
  auto fnv_partition_scalar_function =
      ScalarFunction("fnv_partition", {LogicalType::VARCHAR, LogicalType::INTEGER},
                     LogicalType::INTEGER, FnvPartitionScalarFun);
  ExtensionUtil::RegisterFunction(instance, fnv_scalar_function);
  ExtensionUtil::RegisterFunction(instance, fnv_partition_scalar_function);
}

void FnvExtension::Load(DuckDB &db) { LoadInternal(*db.instance); }
std::string FnvExtension::Name() { return "fnv"; }

std::string FnvExtension::Version() const {
#ifdef EXT_VERSION_FNV
  return EXT_VERSION_FNV;
#else
  return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void fnv_init(duckdb::DatabaseInstance &db) {
  duckdb::DuckDB db_wrapper(db);
  db_wrapper.LoadExtension<duckdb::FnvExtension>();
}

DUCKDB_EXTENSION_API const char *fnv_version() {
  return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
