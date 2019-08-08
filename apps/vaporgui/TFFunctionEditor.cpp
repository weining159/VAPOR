#include "TFFunctionEditor.h"
#include <QPaintEvent>
#include <glm/glm.hpp>

using std::vector;
using glm::vec2;
using glm::clamp;

vec2 qvec2(const QPoint &qp)
{
    return vec2(qp.x(), qp.y());
}

QPointF qvec2(const vec2 &v)
{
    return QPointF(v.x, v.y);
}

vec2 Project(vec2 a, vec2 b, vec2 p)
{
    vec2 n = glm::normalize(b-a);
    float t = glm::dot(n, p-a);
    
    return n * t + a;
}

float DistanceToLine(vec2 a, vec2 b, vec2 p)
{
    vec2 n = glm::normalize(b-a);
    float t = glm::dot(n, p-a);
    
    if (t < 0) return glm::distance(a, p);
    if (t > glm::distance(a, b)) return glm::distance(b, p);
    
    vec2 projection = n * t + a;
    return glm::distance(projection, p);
}






TFFunctionEditor::TFFunctionEditor()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    
    _controlPoints.Add(vec2(0.2,0.5));
    _controlPoints.Add(vec2(0.5,0.8));
}

void TFFunctionEditor::Update(VAPoR::DataMgr *dataMgr, VAPoR::ParamsMgr *paramsMgr, VAPoR::RenderParams *rParams)
{
}

QSize TFFunctionEditor::minimumSizeHint() const
{
    return QSize(100, 100);
}

#define CONTROL_POINT_RADIUS (4.0f)
#define PADDING (CONTROL_POINT_RADIUS + 1.0f)

void TFFunctionEditor::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    
//    p.fillRect(event->rect(), QBrush(QColor(64, 32, 64)));
    
    if (_controlPoints.Size()) {
        ControlPointList &cp = _controlPoints;
        
        for (auto it = cp.BeginLines(); it != cp.EndLines(); ++it) {
            p.drawLine(QNDCToPixel(it.a()), QNDCToPixel(it.b()));
            
            p.drawEllipse(qvec2(Project(NDCToPixel(it.a()), NDCToPixel(it.b()), m)), 2, 2);
        }
        
        
        
        QPen pen(Qt::darkGray, 0.8);
        pen.setWidth(0.5);
        p.setPen(pen);
        QBrush brush(QColor(0xFA, 0xFA, 0xFA));
        p.setBrush(brush);
        
        for (auto it = cp.BeginPoints(); it != cp.EndPoints(); ++it)
            p.drawEllipse(QNDCToPixel(*it), CONTROL_POINT_RADIUS, CONTROL_POINT_RADIUS);
        
    }
}

void TFFunctionEditor::mousePressEvent(QMouseEvent *event)
{
    vec2 mouse(event->posF().x(), event->posF().y());
    for (int i = 0; i < _controlPoints.Size(); i++) {
        const vec2 controlPixel = NDCToPixel(_controlPoints[i]);
        if (glm::distance(mouse, controlPixel) <= CONTROL_POINT_RADIUS) {
            _draggedID = i;
            _dragOffset = controlPixel - mouse;
            break;
        }
    }
}

void TFFunctionEditor::mouseReleaseEvent(QMouseEvent *event)
{
    _draggedID = -1;
}

void TFFunctionEditor::mouseMoveEvent(QMouseEvent *event)
{
    vec2 mouse = qvec2(event->pos());
    m = mouse;
    const int i = _draggedID;
    ControlPointList &cp = _controlPoints;
    const int N = cp.Size();
    
    if (_draggedID >= 0) {
        vec2 newPos = mouse + _dragOffset;
        vec2 newVal = PixelToNDC(newPos);
        newVal = glm::clamp(
                            newVal,
                            vec2(i>0?cp[i-1].x:0, 0),
                            vec2(i<N-1?cp[i+1].x:1, 1));
        
        _controlPoints[_draggedID] = newVal;
    }
    update();
}

void TFFunctionEditor::mouseDoubleClickEvent(QMouseEvent *event)
{
    printf("Double click\n");
    
    vec2 mouse = qvec2(event->pos());
    ControlPointList &cp = _controlPoints;
    
    for (auto it = cp.BeginLines(); it != cp.EndLines(); ++it) {
        const vec2 a = NDCToPixel(it.a());
        const vec2 b = NDCToPixel(it.b());
        
        if (DistanceToLine(a, b, mouse) <= CONTROL_POINT_RADIUS) {
            cp.Add(PixelToNDC(Project(a, b, mouse)), it);
            break;
        }
    }
    
    update();
}

glm::vec2 TFFunctionEditor::NDCToPixel(const glm::vec2 &v) const
{
    return vec2(PADDING + v.x * (width()-2*PADDING), PADDING + (1.0f - v.y) * (height()-2*PADDING));
}

QPointF TFFunctionEditor::QNDCToPixel(const glm::vec2 &v) const
{
    const vec2 p = NDCToPixel(v);
    return QPointF(p.x, p.y);
}

glm::vec2 TFFunctionEditor::PixelToNDC(const glm::vec2 &p) const
{
    float width = QWidget::width();
    float height = QWidget::height();
    VAssert(width != 0 && height != 0);
    
    return vec2((p.x - PADDING) / (width-2*PADDING), 1.0f - (p.y - PADDING) / (height-2*PADDING));
}

glm::vec2 TFFunctionEditor::PixelToNDC(const QPointF &p) const
{
    return PixelToNDC(vec2(p.x(), p.y()));
}

























/*
TFFunctionEditor::TFFunctionEditor()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    this->setMinimumHeight(100);
}

void TFFunctionEditor::Update(VAPoR::DataMgr *dataMgr, VAPoR::ParamsMgr *paramsMgr, VAPoR::RenderParams *rParams)
{
    this->setMaximumSize(width()-1, height()-1);
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    
    updateGL();
}

void TFFunctionEditor::paintGL()
{
    bool swapped = false;
    if (!swapped) {
//        swapBuffers();
        swapped = true;
    }
    printf("%s\n", glGetString(GL_VERSION));
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(1, 0);
    glVertex2f(1, 1);
    glVertex2f(0, 1);
    glEnd();
}

void TFFunctionEditor::resizeGL(int w, int h)
{
//    swapBuffers();
}
*/
