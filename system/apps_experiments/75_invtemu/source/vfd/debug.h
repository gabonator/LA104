void Serial_print(char* msg)
{
  CONSOLE::Color(RGB565(b0b0b0));
  CONSOLE::Print(msg);
  CONSOLE::Color(RGB565(ffffff));
}

void Serial_print(const char* msg)
{
  CONSOLE::Color(RGB565(b0b0b0));
  CONSOLE::Print(msg);
  CONSOLE::Color(RGB565(ffffff));
}

void Serial_print(int v)
{
  CONSOLE::Color(RGB565(b0b0b0));
  CONSOLE::Print("%d", v);
  CONSOLE::Color(RGB565(ffffff));
}
