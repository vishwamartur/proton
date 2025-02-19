#pragma once
#include <Processors/QueryPlan/ITransformingStep.h>
#include <QueryPipeline/SizeLimits.h>
#include <Storages/SelectQueryInfo.h>
#include <Interpreters/Aggregator.h>

namespace DB
{

struct GroupingSetsParams
{
    GroupingSetsParams() = default;

    GroupingSetsParams(ColumnNumbers used_keys_, ColumnNumbers missing_keys_)
        : used_keys(std::move(used_keys_))
        , missing_keys(std::move(missing_keys_))
    {}

    ColumnNumbers used_keys;
    ColumnNumbers missing_keys;
};

using GroupingSetsParamsList = std::vector<GroupingSetsParams>;

Block appendGroupingSetColumn(Block header);

/// Aggregation. See AggregatingTransform.
class AggregatingStep : public ITransformingStep
{
public:
    AggregatingStep(
        const DataStream & input_stream_,
        Aggregator::Params params_,
        GroupingSetsParamsList grouping_sets_params_,
        bool final_,
        bool only_merge_,
        size_t max_block_size_,
        size_t aggregation_in_order_max_block_bytes_,
        size_t merge_threads_,
        size_t temporary_data_merge_threads_,
        bool storage_has_evenly_distributed_read_,
        bool shuffled_,
        InputOrderInfoPtr group_by_info_,
        SortDescription group_by_sort_description_,
        bool should_produce_results_in_order_of_bucket_number_);

    String getName() const override { return "Aggregating"; }

    void transformPipeline(QueryPipelineBuilder & pipeline, const BuildQueryPipelineSettings &) override;

    void describeActions(JSONBuilder::JSONMap & map) const override;

    void describeActions(FormatSettings &) const override;
    void describePipeline(FormatSettings & settings) const override;

    const Aggregator::Params & getParams() const { return params; }

private:
    Aggregator::Params params;
    GroupingSetsParamsList grouping_sets_params;
    bool final;
    bool only_merge;
    size_t max_block_size;
    size_t aggregation_in_order_max_block_bytes;
    size_t merge_threads;
    size_t temporary_data_merge_threads;

    bool storage_has_evenly_distributed_read;
    bool shuffled;

    InputOrderInfoPtr group_by_info;
    SortDescription group_by_sort_description;

    /// It determines if we should resize pipeline to 1 at the end.
    /// Needed in case of distributed memory efficient aggregation.
    const bool should_produce_results_in_order_of_bucket_number;

    Processors aggregating_in_order;
    Processors aggregating_sorted;
    Processors finalizing;

    Processors aggregating;
};

}
