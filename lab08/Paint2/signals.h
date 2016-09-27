#pragma once

template <typename Signature>
using Signal = boost::signals2::signal<Signature>;

using Connection = boost::signals2::connection;
using ScopedConnection = boost::signals2::scoped_connection;
