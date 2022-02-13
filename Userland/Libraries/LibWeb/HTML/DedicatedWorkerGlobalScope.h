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
#include <LibWeb/HTML/WorkerGlobalScope.h>

namespace Web::HTML {

class DedicatedWorkerGlobalScope final
 : public WorkerGlobalScope
 , public Weakable<DedicatedWorkerGlobalScope> {
public:
    using WrapperType = Bindings::DedicatedWorkerGlobalScopeWrapper;

    DedicatedWorkerGlobalScope();
    virtual ~DedicatedWorkerGlobalScope() override;

    //virtual JS::ThrowCompletionOr<bool> internal_has_property(JS::PropertyKey const& name) const override;
    //virtual JS::ThrowCompletionOr<JS::Value> internal_get(JS::PropertyKey const&, JS::Value receiver) const override;
    //virtual JS::ThrowCompletionOr<bool> internal_set(JS::PropertyKey const&, JS::Value value, JS::Value receiver) override;
    virtual void initialize_global_object() override;

};

} // namespace Web::HTML
