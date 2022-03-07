/*
 * Copyright (c) 2022, Ben Abraham <ben.d.abraham@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibJS/Forward.h>
#include <LibJS/Runtime/Completion.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Realm.h>
#include <LibJS/Runtime/VM.h>
#include <LibWeb/Forward.h>
#include <LibWeb/Bindings/WorkerGlobalScope.h>
#include <LibWeb/Bindings/DedicatedWorkerGlobalScope.h>

namespace Web {
namespace Bindings {

class DedicatedWorkerGlobalScope final
    : public WorkerGlobalScope
    , public Weakable<DedicatedWorkerGlobalScope> {
public:

    DedicatedWorkerGlobalScope();
    virtual ~DedicatedWorkerGlobalScope() override;

    //virtual JS::ThrowCompletionOr<bool> internal_has_property(JS::PropertyKey const& name) const override;
    //virtual JS::ThrowCompletionOr<JS::Value> internal_get(JS::PropertyKey const&, JS::Value receiver) const override;
    //virtual JS::ThrowCompletionOr<bool> internal_set(JS::PropertyKey const&, JS::Value value, JS::Value receiver) override;
};

}
}
