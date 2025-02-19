/*
 * Copyright (c) 2020, Matthew Olsson <mattco@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibWeb/DOM/Document.h>
#include <LibWeb/DOM/Element.h>

namespace Web::SVG {

namespace FIXME {
class TemporarilyEnableQuirksMode {
public:
    TemporarilyEnableQuirksMode(DOM::Document const& document)
        : m_document(const_cast<DOM::Document&>(document))
        , m_previous_quirks_mode(document.mode())
    {
        m_document.set_quirks_mode(DOM::QuirksMode::Yes);
    }

    ~TemporarilyEnableQuirksMode()
    {
        m_document.set_quirks_mode(m_previous_quirks_mode);
    }

private:
    DOM::Document& m_document;
    DOM::QuirksMode m_previous_quirks_mode {};
};
}

class SVGElement : public DOM::Element {
    WEB_PLATFORM_OBJECT(SVGElement, DOM::Element);

public:
    virtual bool requires_svg_container() const override { return true; }

    virtual void attribute_changed(DeprecatedFlyString const& name, DeprecatedString const& value) override;

    virtual void children_changed() override;
    virtual void inserted() override;
    virtual void removed_from(Node*) override;

    HTML::DOMStringMap* dataset() { return m_dataset.ptr(); }
    HTML::DOMStringMap const* dataset() const { return m_dataset.ptr(); }

protected:
    SVGElement(DOM::Document&, DOM::QualifiedName);

    virtual JS::ThrowCompletionOr<void> initialize(JS::Realm&) override;
    virtual void visit_edges(Cell::Visitor&) override;

    void update_use_elements_that_reference_this();
    void remove_from_use_element_that_reference_this();

    JS::GCPtr<HTML::DOMStringMap> m_dataset;

private:
    virtual bool is_svg_element() const final { return true; }
};

}

namespace Web::DOM {

template<>
inline bool Node::fast_is<SVG::SVGElement>() const { return is_svg_element(); }

}
