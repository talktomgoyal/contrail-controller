/*
 * Copyright (c) 2014 Juniper Networks, Inc. All rights reserved.
 */

#ifndef SRC_VNSW_AGENT_OVS_TOR_AGENT_OVSDB_CLIENT_OVSDB_OBJECT_H_
#define SRC_VNSW_AGENT_OVS_TOR_AGENT_OVSDB_CLIENT_OVSDB_OBJECT_H_

#include <boost/intrusive_ptr.hpp>

#include <db/db_entry.h>
#include <ksync/ksync_entry.h>
#include <ksync/ksync_index.h>
#include <ksync/ksync_object.h>

#include <ovsdb_client_idl.h>

namespace OVSDB {
class OvsdbDBEntry;

class OvsdbObject : public KSyncObject {
public:
    OvsdbObject(OvsdbClientIdl *idl);
    virtual ~OvsdbObject();

    bool IsActiveEntry(KSyncEntry *entry);
    KSyncEntry *FindActiveEntry(KSyncEntry *key);
    // Trigger delete of object table
    void DeleteTable(void);

    virtual void EmptyTable(void);

    OvsdbClientIdl *client_idl() { return client_idl_.get();}

protected:
    OvsdbClientIdlPtr client_idl_;
private:
    DISALLOW_COPY_AND_ASSIGN(OvsdbObject);
};

class OvsdbDBObject : public KSyncDBObject {
public:
    static const uint32_t StaleEntryCleanupTimer = 300000;          // 5 mins
    static const uint32_t StaleEntryYeildTimer = 100;               // 100 ms
    static const uint16_t StaleEntryDeletePerIteration = 32;

    OvsdbDBObject(OvsdbClientIdl *idl, bool init_stale_entry_cleanup);
    OvsdbDBObject(OvsdbClientIdl *idl, DBTable *tbl,
                  bool init_stale_entry_cleanup);
    virtual ~OvsdbDBObject();

    // API to register db table, if not already registered.
    virtual void OvsdbRegisterDBTable(DBTable *tbl);

    void NotifyAddOvsdb(OvsdbDBEntry *key, struct ovsdb_idl_row *row);
    void NotifyDeleteOvsdb(OvsdbDBEntry *key, struct ovsdb_idl_row *row);

    virtual void OvsdbNotify(OvsdbClientIdl::Op, struct ovsdb_idl_row *) = 0;
    virtual OvsdbDBEntry *AllocOvsEntry(struct ovsdb_idl_row *row) = 0;
    bool DBWalkNotify(DBTablePartBase *partition, DBEntryBase *entry);
    void DBWalkDone(DBTableBase *partition);

    // Trigger delete of object table
    void DeleteTable(void);

    virtual void EmptyTable(void);

    OvsdbClientIdl *client_idl() { return client_idl_.get();}
protected:
    OvsdbClientIdlPtr client_idl_;
private:
    friend class OvsdbDBEntry;
    DBTableWalker::WalkId walkid_;
    DISALLOW_COPY_AND_ASSIGN(OvsdbDBObject);
};
};

#endif //SRC_VNSW_AGENT_OVS_TOR_AGENT_OVSDB_CLIENT_OVSDB_OBJECT_H_

