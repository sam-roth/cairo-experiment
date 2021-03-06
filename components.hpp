#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_

#include <pango/pangocairo.h>
#include <pango/pango.h>
#include <string>
#include <memory>
#include <vector>
#include "rapidxml/rapidxml.hpp"
#include <functional>

class SDL_Window;
class SDL_Renderer;
class SDL_Surface;
class SDL_Texture;

struct Color {
    float r;
    float g;
    float b;
    float a;
    Color(float r, float g, float b, float a) : r(r),g(g),b(b),a(a) {}
};

struct Margin {
    double left;
    double top;
    double right;
    double bottom;
    explicit Margin(double all): left(all),top(all),right(all),bottom(all) {}
    Margin(double left, double top, double right, double bottom): left(left), top(top), right(right), bottom(bottom) {}
};

typedef Margin Padding;

namespace Colors {
    static const Color White = Color(1,1,1,1);
    static const Color Black = Color(0,0,0,1);
};

class Graphics {
public:
    virtual double getTextWidth(const std::string &text) = 0;
    virtual void save() = 0;
    virtual void restore() = 0;
    virtual void translate(double x, double y) = 0;
    virtual void setColor(const Color &color) = 0;
    virtual void setAntialias(bool value) = 0;
    virtual void fillRectangle(int x, int y, int width, int height) = 0;
    virtual void drawRectangle(int x, int y, int width, int height) = 0;
    virtual void setLineWidth(int width) = 0;
    virtual void drawText(int x, int y, int width, int height, const std::string &text, PangoAlignment alignment) = 0;
};

class CairoGraphics : public Graphics {
private:
    cairo_t *_cr;
public:
    CairoGraphics(cairo_t *cr);
    double getTextWidth(const std::string &text) override;
    void save() override;
    void restore() override;
    void translate(double x, double y) override;
    void setColor(const Color &color) override;
    void setAntialias(bool value) override;
    void fillRectangle(int x, int y, int width, int height) override;
    void drawRectangle(int x, int y, int width, int height) override;
    void setLineWidth(int width) override;
    void drawText(int x, int y, int width, int height, const std::string &text, PangoAlignment alignment) override;
};

class Widget {
protected:
    std::vector<std::shared_ptr<Widget>> children;
    Margin _margin { 0 };
    Padding _padding { 0 };

public:
    void addWidget(std::shared_ptr<Widget> child);
    void render(Graphics &graphics);

    virtual double height() = 0;
    virtual double width() = 0;

    const Margin& margin();
    void setMargin(const Margin &margin);

    const Padding& padding();
    void setPadding(const Padding &padding);

protected:
    virtual void paint(Graphics &graphics) = 0;
};

class Button : public Widget {
private:
    std::string _text;

public:
    Button(): _text("Click Me") {}
    explicit Button(const std::string &text);
    double width() override;
    double height() override;

protected:
    void paint(Graphics &graphics) override;

};

class Window {
private:
    std::vector<std::shared_ptr<Widget>> _widgets;
    SDL_Window *_window = nullptr;
    SDL_Renderer *_renderer = nullptr;
    SDL_Texture *_texture = nullptr;
    std::string title = "Window";
    int width, height;

public:
    Window(const std::string &title, int width, int height);
    ~Window();
    void open();
    void addWidget(std::shared_ptr<Widget> widget);

private:
    SDL_Texture* create_texture();
    void draw_to_sdl(SDL_Surface *sdlsurf);
};

class VerticalPanel : public Widget {
private:

public:
    double width() override;
    double height() override;

protected:
    void paint(Graphics &grahics) override;

};

class MainMenu : public Widget {
public:
    double width() override;
    double height() override;

protected:
    void paint(Graphics &graphics) override;
};

class Menu : public Widget {
private:
    double _width = 0;
    std::string _title;
public:
    Menu();
    double width() override;
    double height() override;
    std::string title();
    void setTitle(const std::string &title);

protected:
    void paint(Graphics &graphics) override;
};

class Program {
private:

public:
    Window* window = nullptr;
    void loadUserInterfaceFromXML(char *xml);
    void run();
    ~Program();
    void reloadWidgetsFromXML(const std::string &xml);

private:
    std::shared_ptr<Widget> createWidgetFromNode(rapidxml::xml_node<char>* node);
};

class FileWatcher {
public:

    void addWatch(const std::string &path, std::function<void()> callback);

};

#endif
