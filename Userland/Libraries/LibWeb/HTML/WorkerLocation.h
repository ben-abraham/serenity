/*
 * Copyright (c) 2022, Andrew Kaster <akaster@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/RefCounted.h>
#include <LibWeb/Bindings/WorkerGlobalScope.h>
#include <LibWeb/Bindings/Wrappable.h>
#include <LibWeb/Forward.h>

namespace Web {
namespace HTML {

// https://html.spec.whatwg.org/multipage/workers.html#worker-locations
class WorkerLocation
    : public RefCounted<WorkerLocation>
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::WorkerLocationWrapper;

    WorkerLocation(Bindings::WorkerGlobalScope&);

    static NonnullRefPtr<WorkerLocation> create(Bindings::WorkerGlobalScope& scope)
    {
        return adopt_ref(*new WorkerLocation(scope));
    }

    static NonnullRefPtr<WorkerLocation> create_with_global_object(Bindings::WorkerGlobalScope& scope)
    {
        return WorkerLocation::create(scope);
    }

    String href() const;
    String origin() const;
    String protocol() const;
    String host() const;
    String hostname() const;
    String port() const;
    String pathname() const;
    String search() const;
    String hash() const;

private:

    Bindings::WorkerGlobalScope& m_global_scope;
};

}

namespace Bindings {

WorkerLocationWrapper* wrap(JS::GlobalObject&, HTML::WorkerLocation&);

}
}
