#include "ADBCDriver.h"
#include <adbc.h>
#include <arrow/flight/sql/client.h>
#include <arrow/flight/sql/types.h>
#include <arrow/flight/client.h>
#include <arrow/flight/types.h>
#include <arrow/io/memory.h>
#include <arrow/result.h>
#include <arrow/status.h>
#include <arrow/table.h>
#include <arrow/util/logging.h>
#include <iostream>
#include <memory>
#include <string>

namespace DB
{

ADBCDriver::ADBCDriver(const std::string & uri)
{
    arrow::flight::Location location;
    arrow::Status status = arrow::flight::Location::Parse(uri, &location);
    if (!status.ok())
    {
        throw std::runtime_error("Failed to parse URI: " + status.ToString());
    }

    arrow::flight::FlightClientOptions options;
    status = arrow::flight::FlightClient::Connect(location, options, &client_);
    if (!status.ok())
    {
        throw std::runtime_error("Failed to connect to Flight server: " + status.ToString());
    }
}

std::shared_ptr<arrow::Table> ADBCDriver::ExecuteQuery(const std::string & query)
{
    arrow::flight::sql::FlightSqlClient sql_client(client_);
    arrow::flight::sql::StatementQueryResult result;
    arrow::Status status = sql_client.Execute(query, &result);
    if (!status.ok())
    {
        throw std::runtime_error("Failed to execute query: " + status.ToString());
    }

    std::shared_ptr<arrow::Table> table;
    status = result.reader->ReadAll(&table);
    if (!status.ok())
    {
        throw std::runtime_error("Failed to read query result: " + status.ToString());
    }

    return table;
}

} // namespace DB
