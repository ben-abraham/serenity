/*
 * Copyright (c) 2022, Ben Abraham <ben.d.abraham@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Base64.h>
#include <AK/String.h>
#include <AK/Utf8View.h>
#include <AK/Vector.h>
#include <LibTextCodec/Decoder.h>
#include <LibWeb/Bindings/DedicatedWorkerGlobalScopeWrapper.h>
#include <LibWeb/Bindings/WorkerGlobalScopeWrapper.h>
#include <LibWeb/DOM/DOMException.h>
#include <LibWeb/Forward.h>
#include <LibWeb/HTML/DedicatedWorkerGlobalScope.h>

namespace Web::HTML {

DedicatedWorkerGlobalScope::DedicatedWorkerGlobalScope()
    : WorkerGlobalScope()
{
}

DedicatedWorkerGlobalScope::~DedicatedWorkerGlobalScope()
{
}

void DedicatedWorkerGlobalScope::initialize_global_object()
{
    JS::GlobalObject::initialize_global_object();
}

} // namespace Web::HTML
