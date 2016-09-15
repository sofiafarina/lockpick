#ifndef __lockpick__
#define __lockpick__

#include <passe_par_tout.h>
#include <unistd.h>

class vector
{
public:

  // Members

  double x;
  double y;

  // Constructors
  vector()
  {
    x = 0.;
    y = 0.;
  }

  vector(double x, double y)
  {
    this->x = x;
    this->y = y;
  }

  vector(const vector & v)
  {
    x = v.x;
    y = v.y;
  }
};

class color
{
public:

  // Members

  unsigned int red, green, blue;

  // Constructors

  color()
  {
  }

  color(unsigned int red, unsigned int green, unsigned int blue)
  {
    this->red = red;
    this->green = green;
    this->blue = blue;
  }

  color(const color & c)
  {
    red = c.red;
    green = c.green;
    blue = c.blue;
  }
};

class window
{
  // Members

  int _id;

  // Static members

  static bool __started;

  static char * __default_title;
  static int __default_width;
  static int __default_height;

  static int __window_count;

public:

  // Constructors

  window(const char * title = nullptr, int width = __default_width, int height = __default_height, int position_x = 0, int position_y = 0, color background = color(255, 255, 255), int frame_width_percentage = 95, int frame_height_percentage = 95)
  {
    start();
    ++__window_count;

    if(title)
    {
      int position[] = {position_x, position_y};
      m_place_window(position, const_cast <char *> (title));
    }

    frame_width_percentage = (frame_width_percentage << 24) + (background.red << 16) + (background.green << 8) + background.blue;
    frame_height_percentage = (frame_height_percentage << 24) + (width << 11) + height;
    _id = m_window(&frame_width_percentage, &frame_height_percentage) - 1;

    double xmin = 0., ymin = 0., xmax = 1., ymax = 1.;
    m_frame(&xmin, &ymin, &xmax, &ymax);
  }

  // Destructor

  ~window()
  {
    --__window_count;
    m_close(&_id);

    if(__window_count == 0)
    {
        m_endg();
        __started = false;
    }
  }

  // Methods

  void ellipse(vector center, vector axes, bool fill = false)
  {
    m_select(&_id);

    double vcenter[2] = {center.x, center.y};
    double vaxes[2] = {axes.x, axes.y};

    int mode = fill;
    m_ellipse(vcenter, vaxes, &mode);
  }

  void arc(vector center, vector axes, double initial_ang, double amp, double incl = 360, bool fill = false)
  {
    m_select (&_id);

    double vcenter[2] = {center.x, center.y};
    double vaxes[2] = {axes.x, axes.y};
    double ang[3] = {initial_ang, amp, incl};

    int mode = fill;
    m_arc(vcenter, vaxes, ang, &mode);

  }

  void circle(vector center, double radius, bool fill = false, int c = 0)
  {
      colors(c);
      ellipse(center, {radius, radius}, fill);
  }

  void line(vector from, vector to)
  {
    m_select(&_id);

    double vfrom[2] = {from.x, from.y};
    m_move(vfrom);

    double vto[2] = {to.x, to.y};
    m_line(vto);
  }
    
  void text(char * c, vector x)
  {
      m_select(&_id);
      double vx[2] = {x.x, x.y};
      
      m_text(c, vx);
      
  }
    
  void colors (int n)
  {
      m_select(&_id);
      m_color(&n);
  }

  // Static methods

  static void set_default_title(const char * title)
  {
    __default_title = const_cast <char *> (title);
  }

  static void set_default_size(int width, int height)
  {
    __default_width = width;
    __default_height = height;
  }

  static void wait_enter()
  {
    int event = EVENTO_ENTER;
    m_wait_for_events(&event);
  }

  static void wait_click()
  {
    int event = EVENTO_MOUSE;
    m_wait_for_events(&event);
  }

  void flush()
  {
    m_redraw(&_id);
  }

  void clear()
  {
    m_clear(&_id);
  }


private:

  // Static private methods

  static void start()
  {
    if(!__started)
    {
      __started = true;
      m_startg(__default_title, &__default_width, &__default_height);
    }
  }
};

bool window :: __started = false;
char * window :: __default_title = const_cast <char *> ("Lockpick");
int window :: __default_width = 750;
int window :: __default_height = 750;
int window :: __window_count = 0;

#endif