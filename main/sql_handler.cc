#include <main/sql_handler.hh>
#include <util/yield.hpp>

AbstractAnything::~AbstractAnything() {}

AbstractQueryExecutor::~AbstractQueryExecutor() {}

std::pair<AbstractQueryExecutor::ResultType, AbstractAnything *>
AbstractQueryExecutor::
next(const ResType &res, const NextParams &nparams)
{
    genericPreamble(nparams);
    return this->nextImpl(res, nparams);
}

void AbstractQueryExecutor::
genericPreamble(const NextParams &nparams)
{
    // We handle before any queries because a failed query
    // may stale the database during recovery and then
    // we'd have to handle there as well.

    //首先根据stales来设置这个executor会不会让当前在shared中的cache变成stale,对于simple来说, 继承的是默认的情况, 所以
    //不会设置cache为stale, 也就是下次进入rewrite的时候, 不需要重新load.
    try {
        nparams.ps.getSchemaCache().updateStaleness(
            nparams.ps.getEConn(), this->stales());
    } catch (const SchemaFailure &e) {
        FAIL_GenericPacketException("failed updating staleness");
    }

    if (this->usesEmbedded()) {
        TEST_ErrPkt(
            lowLevelSetCurrentDatabase(nparams.ps.getEConn(), nparams.default_db),
            "failed to set the embedded database to " + nparams.default_db + ";"
            " your client may be in an unrecoverable bad loop"
            " so consider restarting just the _client_. this can happen"
            " if you tell your client to connect to a database that exists"
            " but was not created through cryptdb.");
    }

    return;
}

std::pair<AbstractQueryExecutor::ResultType, AbstractAnything *> SimpleExecutor::
nextImpl(const ResType &res, const NextParams &nparams)
{
    reenter(this->corot) {
        yield return CR_QUERY_RESULTS(nparams.original_query);
    }

    assert(false);
}

std::pair<AbstractQueryExecutor::ResultType, AbstractAnything *> NoOpExecutor::
nextImpl(const ResType &res, const NextParams &nparams)
{
    reenter(this->corot) {
        yield return CR_QUERY_RESULTS("DO 0;");
    }

    assert(false);
}


