#include "config.h"

#include "Color.h"
#include "FloatRect.h"
#include "LayoutSpy.h"

#include "LogInitialization.h"
#include "Logging.h"

#if LOG_DISABLED && RELEASE_LOG_DISABLED
#error "uh oh!"
#endif

namespace WebCore {

WTF::TextStream& operator<<(WTF::TextStream& stream, LayoutDelta::Subject subject)
{
    return stream << [&]() {
        switch(subject) {
        case LayoutDelta::Subject::X: return "X";
        case LayoutDelta::Subject::Y: return "Y";
        case LayoutDelta::Subject::Width: return "Width";
        case LayoutDelta::Subject::Height: return "Height";
        default:
            ASSERT_NOT_REACHED();
            return "";
        }
    }();
}

LayoutSpy::LayoutSpy() = default;

void LayoutSpy::logChange(const RenderObject *object, const LayoutDelta::Subject subject, float newValue)
{
    ASSERT(object);
    LOG_WITH_STREAM(Layout, stream << object->renderName() << "::set" << subject << "(" << newValue << ")");
    m_changeQueue.append(LayoutDelta { object, subject, newValue });
}

FloatRect& LayoutSpy::boxForElement(const RenderObject *key)
{
    if (auto box = m_elements.find(key); box != m_elements.end())
        return box->second;

    m_elementOrdering.append(key);
    return m_elements[key];
}

void LayoutSpy::processQueueChanges()
{
    for (auto& delta : m_changeQueue) {
        auto& elementBox = boxForElement(delta.m_object);
        auto newValue = delta.m_newValue;

        switch(delta.m_subject) {
        case LayoutDelta::Subject::X: elementBox.setX(newValue); break;
        case LayoutDelta::Subject::Y: elementBox.setY(newValue); break;
        case LayoutDelta::Subject::Width: elementBox.setWidth(newValue); break;
        case LayoutDelta::Subject::Height: elementBox.setHeight(newValue); break;
        default:
            ASSERT_NOT_REACHED();
        }
    }
    m_changeQueue.clear();
}

void LayoutSpy::paintCurrentState(GraphicsContext& context, const FloatRect& damageRect)
{
    GraphicsContextStateSaver stateSaver(context);

    LOG_WITH_STREAM(Layout, stream << "Have " << m_elements.size() << " element boxes to paint");

    (void)damageRect; // FIXME

    context.setStrokeColor(SRGBA<uint8_t> { 0, 0, 0, 255 });
    
    for (auto& key : m_elementOrdering) {
        FloatRect& elementBox = m_elements[key];
        // This actually takes a FloatRect, perhaps we should just store it like that as well?
        context.strokeRect(elementBox, 2);
    }
}

void LayoutSpy::paintChangelog(GraphicsContext& context, const FloatRect& damageRect)
{
    LOG_WITH_STREAM(Layout, stream << "Should paint " << m_changeQueue.size() << " queue events for region " << damageRect);
    
    processQueueChanges();
    paintCurrentState(context, damageRect);
}

void LayoutSpy::setX(const RenderObject *object, float x) { logChange(object, LayoutDelta::Subject::X, x); }
void LayoutSpy::setY(const RenderObject *object, float y) { logChange(object, LayoutDelta::Subject::Y, y); }
void LayoutSpy::setWidth(const RenderObject *object, float width) { logChange(object, LayoutDelta::Subject::Width, width); }
void LayoutSpy::setHeight(const RenderObject *object, float height) { logChange(object, LayoutDelta::Subject::Height, height); }

}
