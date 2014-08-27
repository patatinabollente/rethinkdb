// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_TABLES_TABLE_CONFIG_HPP_
#define CLUSTERING_ADMINISTRATION_TABLES_TABLE_CONFIG_HPP_

#include <string>
#include <vector>

#include "errors.hpp"
#include <boost/shared_ptr.hpp>

#include "clustering/administration/database_metadata.hpp"
#include "clustering/administration/namespace_metadata.hpp"
#include "containers/uuid.hpp"
#include "rdb_protocol/artificial_table/backend.hpp"
#include "rpc/semilattice/view.hpp"

class real_reql_cluster_interface_t;
class server_name_client_t;

/* This is publicly exposed so that it can be used to create the return value of
`table.reconfigure()`. */
counted_t<const ql::datum_t> convert_table_config_to_datum(
        const table_config_t &config);

class table_config_artificial_table_backend_t :
    public artificial_table_backend_t
{
public:
    table_config_artificial_table_backend_t(
            boost::shared_ptr< semilattice_readwrite_view_t<
                cow_ptr_t<namespaces_semilattice_metadata_t> > > _table_sl_view,
            boost::shared_ptr< semilattice_readwrite_view_t<
                databases_semilattice_metadata_t> > _database_sl_view,
            real_reql_cluster_interface_t *_reql_cluster_interface,
            server_name_client_t *_name_client) :
        table_sl_view(_table_sl_view),
        database_sl_view(_database_sl_view),
        reql_cluster_interface(_reql_cluster_interface),
        name_client(_name_client) { }

    std::string get_primary_key_name();
    bool read_all_primary_keys(
            signal_t *interruptor,
            std::vector<counted_t<const ql::datum_t> > *keys_out,
            std::string *error_out);
    bool read_row(
            counted_t<const ql::datum_t> primary_key,
            signal_t *interruptor,
            counted_t<const ql::datum_t> *row_out,
            std::string *error_out);
    bool write_row(
            counted_t<const ql::datum_t> primary_key,
            counted_t<const ql::datum_t> new_value,
            signal_t *interruptor,
            std::string *error_out);

private:
    boost::shared_ptr< semilattice_readwrite_view_t<
        cow_ptr_t<namespaces_semilattice_metadata_t> > > table_sl_view;
    boost::shared_ptr< semilattice_readwrite_view_t<
        databases_semilattice_metadata_t> > database_sl_view;
    real_reql_cluster_interface_t *reql_cluster_interface;
    server_name_client_t *name_client;
};

#endif /* CLUSTERING_ADMINISTRATION_TABLES_TABLE_CONFIG_HPP_ */
