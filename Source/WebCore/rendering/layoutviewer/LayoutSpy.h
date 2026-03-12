#pragma once

#include "RenderObject.h"

#include <wtf/Vector.h>
#include <wtf/StdMap.h>

namespace WebCore {

struct LayoutDelta {
    enum class Subject : uint8_t {
        X,
        Y,
        Width,
        Height
    };

    const RenderObject *m_object; // we use the element's address as an identifier, but never dereference it through this
    Subject m_subject;
    LayoutUnit m_newValue;
};

WTF::TextStream& operator<<(WTF::TextStream&, LayoutDelta::Subject);

class GraphicsContext;
class IntRect;

class LayoutSpy {
public:
    LayoutSpy();

    void setX(const RenderObject*, LayoutUnit x);
    void setY(const RenderObject*, LayoutUnit y);
    void setWidth(const RenderObject*, LayoutUnit width);
    void setHeight(const RenderObject*, LayoutUnit height);

    void paintChangelog(GraphicsContext&, const IntRect& damageRect);

private:
    void logChange(const RenderObject*, const LayoutDelta::Subject, LayoutUnit newValue);

    IntRect& boxForElement(const RenderObject*);
    
    void processQueueChanges();
    void paintCurrentState(GraphicsContext&, const IntRect& damageRect);

    Vector<LayoutDelta> m_changeQueue;

    Vector<const RenderObject*> m_elementOrdering;
    StdMap<const RenderObject*, IntRect> m_elements;
};

}
