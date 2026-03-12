#pragma once

#include "RenderObject.h"

#include <wtf/Vector.h>
#include <wtf/StdMap.h>

namespace WebCore {

struct LayoutDelta {
    enum class Subject : uint8_t {
        X, // N.B. X and Y are *absolute*!
        Y,
        Width,
        Height
    };

    const RenderObject *m_object; // we use the element's address as an identifier, but never dereference it through this
    Subject m_subject;
    float m_newValue;
};

WTF::TextStream& operator<<(WTF::TextStream&, LayoutDelta::Subject);

class GraphicsContext;
class FloatRect;

class LayoutSpy {
public:
    LayoutSpy();

    void setX(const RenderObject*, float x);
    void setY(const RenderObject*, float y);
    void setWidth(const RenderObject*, float width);
    void setHeight(const RenderObject*, float height);

    void paintChangelog(GraphicsContext&, const FloatRect& damageRect);

private:
    void logChange(const RenderObject*, const LayoutDelta::Subject, float newValue);

    FloatRect& boxForElement(const RenderObject*);
    
    void processQueueChanges();
    void paintCurrentState(GraphicsContext&, const FloatRect& damageRect);

    Vector<LayoutDelta> m_changeQueue;

    Vector<const RenderObject*> m_elementOrdering;
    StdMap<const RenderObject*, FloatRect> m_elements;
};

}
