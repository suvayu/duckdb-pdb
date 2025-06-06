#define DUCKDB_EXTENSION_MAIN

#include "pdb_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void PdbScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Pdb " + name.GetString() + " 🐥");
	});
}

inline void PdbOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Pdb " + name.GetString() + ", my linked OpenSSL version is " +
		                                           OPENSSL_VERSION_TEXT);
	});
}

static void LoadInternal(DatabaseInstance &instance) {
	// Register a scalar function
	auto pdb_scalar_function = ScalarFunction("pdb", {LogicalType::VARCHAR}, LogicalType::VARCHAR, PdbScalarFun);
	ExtensionUtil::RegisterFunction(instance, pdb_scalar_function);

	// Register another scalar function
	auto pdb_openssl_version_scalar_function =
	    ScalarFunction("pdb_openssl_version", {LogicalType::VARCHAR}, LogicalType::VARCHAR, PdbOpenSSLVersionScalarFun);
	ExtensionUtil::RegisterFunction(instance, pdb_openssl_version_scalar_function);
}

void PdbExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string PdbExtension::Name() {
	return "pdb";
}

std::string PdbExtension::Version() const {
#ifdef EXT_VERSION_PDB
	return EXT_VERSION_PDB;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void pdb_init(duckdb::DatabaseInstance &db) {
	duckdb::DuckDB db_wrapper(db);
	db_wrapper.LoadExtension<duckdb::PdbExtension>();
}

DUCKDB_EXTENSION_API const char *pdb_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
