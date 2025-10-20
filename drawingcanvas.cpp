#include "drawingcanvas.h"

DrawingCanvas::DrawingCanvas(QWidget *parent)  {
    // Set a minimum size for the canvas
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    // Set a solid background color
    setStyleSheet("background-color: white; border: 1px solid gray;");
}

void DrawingCanvas::clearPoints(){
    m_points.clear();
    m_rects.clear();
    // Trigger a repaint to clear the canvas
    update();
}

void DrawingCanvas::paintLines(){
    /* Todo
     * Implement lines drawing per even pair
    */

    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::segmentDetection(){
    m_rects.clear();
    QPixmap pixmap = this->grab(); //
    QImage image = pixmap.toImage();

    auto isPixelRed = [](QRgb pixel) {
        int red = qRed(pixel);
        int green = qGreen(pixel);
        int blue = qBlue(pixel);
        return (red > 200 && green < 50 && blue < 50);
    };

    cout << "image width " << image.width() << endl;
    cout << "image height " << image.height() << endl;

    vector<CustomMatrix> windows; //(image.width()*image.height());

    // Get the pixel value as an ARGB integer (QRgb is a typedef for unsigned int)
    for(int i = 1; i < image.width()-1;i++){
        for(int j = 1; j < image.height()-1;j++){
            QRgb centerRgb = image.pixel(i, j);
            bool centerIsDetected = isPixelRed(centerRgb);
            if (centerIsDetected) {
                bool local_window[3][3];
                short neighbor_count = 0;

                for(int m=-1;m<=1;m++){
                    for(int n=-1;n<=1;n++){
                        QRgb rgbValue = image.pixel(i+m, j+n);
                        local_window[m+1][n+1] = isPixelRed(rgbValue);
                        neighbor_count += isPixelRed(rgbValue);
                    }
                }

                CustomMatrix mat(local_window);
                if (neighbor_count > 3) m_rects.append(QRect(i - 20, j - 20, 10, 10));
                if (neighbor_count > 3) cout << i << " " << j << " " << neighbor_count << endl;
                windows.push_back(mat);
            }
        }
    }

    update(); // <-- CHANGED (This line was added to trigger the repaint)
}

void DrawingCanvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up the pen and brush for drawing the points
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));

    // Draw a small circle at each stored point
    for (const QPoint& point : std::as_const(m_points)) {
        painter.drawEllipse(point, 1, 1);
    }

    if(isPaintLinesClicked){
        cout << "paint lines block is called 2" << endl;
        pen.setColor(Qt::red);
        pen.setWidth(1); // 4-pixel wide line
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        // Set the painter's pen to our custom pen.
        painter.setPen(pen);

        for(int i=0;i<m_points.size()-1;i+=2){
            //cout << m_points[i].x() << endl;
            painter.drawLine(m_points[i], m_points[i+1]);
        }

        //return painter pen to blue
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }

    pen.setColor(Qt::magenta);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    for (const QRect& rect : std::as_const(m_rects)) {
        painter.drawRect(rect);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    // Add the mouse click position to our vector of points
    m_points.append(event->pos());
    // Trigger a repaint
    update();
}
