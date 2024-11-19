#include <gtest/gtest.h>
#include "ADBCDriver.h"
#include <arrow/table.h>
#include <arrow/array.h>
#include <arrow/status.h>
#include <arrow/result.h>
#include <arrow/flight/sql/client.h>
#include <arrow/flight/sql/types.h>
#include <arrow/flight/client.h>
#include <arrow/flight/types.h>
#include <arrow/io/memory.h>
#include <arrow/util/logging.h>
#include <iostream>
#include <memory>
#include <string>

class ADBCDriverTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Set up any necessary resources or configurations for the tests
    }

    void TearDown() override
    {
        // Clean up any resources or configurations used in the tests
    }
};

TEST_F(ADBCDriverTest, TestConnection)
{
    std::string uri = "grpc://localhost:8815";
    DB::ADBCDriver driver(uri);
    ASSERT_NO_THROW(driver.ExecuteQuery("SELECT 1"));
}

TEST_F(ADBCDriverTest, TestQueryExecution)
{
    std::string uri = "grpc://localhost:8815";
    DB::ADBCDriver driver(uri);
    std::shared_ptr<arrow::Table> result;
    ASSERT_NO_THROW(result = driver.ExecuteQuery("SELECT 1"));
    ASSERT_NE(result, nullptr);
    ASSERT_EQ(result->num_rows(), 1);
    ASSERT_EQ(result->num_columns(), 1);
    auto column = result->column(0);
    auto array = column->chunk(0);
    auto int_array = std::dynamic_pointer_cast<arrow::Int64Array>(array);
    ASSERT_NE(int_array, nullptr);
    ASSERT_EQ(int_array->Value(0), 1);
}
