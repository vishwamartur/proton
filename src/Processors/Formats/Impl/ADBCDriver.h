#pragma once

#include <memory>
#include <string>
#include <arrow/table.h>
#include <arrow/flight/client.h>

namespace DB
{

class ADBCDriver
{
public:
    explicit ADBCDriver(const std::string & uri);
    std::shared_ptr<arrow::Table> ExecuteQuery(const std::string & query);

private:
    std::shared_ptr<arrow::flight::FlightClient> client_;
};

} // namespace DB
