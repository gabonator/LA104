namespace Layout
{
    class Color
    {
    public:
        int mF, mB;
        Color(int f, int b = RGBTRANS) : mF(f), mB(b)
        {
        }
    };

    class Element
    {
    };

    class NewLine
    {
    };

    class Goto
    {
    public:
        int mX;
        Goto(int x) : mX(x)
        {
        }
    };
    
    class Spacer
    {
    public:
        int mW;
        Spacer(int w) : mW(w)
        {
        }
    };

    class Padding
    {
    public:
        CRect mPadding;
        Padding(int l, int t, int r, int b) : mPadding(l, t, r, b)
        {
        }
    };

    class Select
    {
    public:
        bool mSelected;
        Select(bool b) : mSelected(b)
        {
        }
    };

    class Render;
    class Sublayout
    {
    public:
        virtual void Visit(Render* r) const = 0;
    };

    class Render
    {
        friend Sublayout;
        friend class Button;
        
    protected:
        CRect mRect;
        int mX{0}, mF{RGB565(b0b0b0)}, mB{RGBTRANS};
        bool mSelected{false};
        
    public:
        Render(CRect rect)
        {
            mRect = rect;
            mX = mRect.left;
        }
        
        Render& operator << (const Padding& p)
        {
            mRect.Deflate(p.mPadding.left, p.mPadding.top, p.mPadding.right, p.mPadding.bottom);
            mX = mRect.left;
            return *this;
        }

        Render& operator << (Element& e)
        {
            return *this;
        }
        Render& operator << (char* str)
        {
            mX += BIOS::LCD::Print(mX, mRect.top, mF, mB, str);
            return *this;
        }
        Render& operator << (const char* str)
        {
            mX += BIOS::LCD::Print(mX, mRect.top, mF, mB, str);
            return *this;
        }
        Render& operator << (char c)
        {
            switch (c)
            {
                case '<': mX += BIOS::LCD::Draw(mX, mRect.top, mB, mF, CShapes_sel_left); break;
                case '>': mX += BIOS::LCD::Draw(mX, mRect.top, mB, mF, CShapes_sel_right); break;
                case 'x': mX += BIOS::LCD::Draw(mX, mRect.top, mF, mB, CShapes_check_box); break;
                case 'X': BIOS::LCD::Draw(mX, mRect.top, mF, mB, CShapes_check_box);
                    mX += BIOS::LCD::Draw(mX, mRect.top, RGB565(ff0000), RGBTRANS, CShapes_check_on); break;
                default:
                {
                    char str[2] = {c, 0};
                    mX += BIOS::LCD::Print(mX, mRect.top, mF, mB, str);
                }
            }
            return *this;
        }
        Render& operator << (int n)
        {
            mX += BIOS::LCD::Printf(mX, mRect.top, mF, mB, "%d", n);
            return *this;
        }
        Render& operator << (NewLine)
        {
            mX = mRect.left;
            mRect.top += 14;
            return *this;
        }
        Render& operator << (const Color& c)
        {
            mF = c.mF;
            mB = c.mB;
            return *this;
        }
        Render& operator << (const Goto& g)
        {
            mX = g.mX;
            return *this;
        }
        Render& operator << (const Spacer& s)
        {
            mX += s.mW;
            return *this;
        }
        Render& operator << (const Select& s)
        {
            mSelected = s.mSelected;
            if (mSelected)
                return *this << Color(RGB565(000000), RGB565(ffffff));
            else
                return *this << Color(RGB565(ffffff), RGBTRANS);
        }
        Render& operator <<(const Sublayout& s)
        {
            s.Visit(this);
            return *this;
        }
        bool IsSelected()
        {
            return mSelected;
        }
        const CRect& GetRectangle()
        {
            return mRect;
        }
    };

    class Units : public Sublayout
    {
        char* mValue;
        const char* mUnits;
    public:
        Units(char* value, const char* units) : mValue(value), mUnits(units)
        {
        }
        virtual void Visit(Render* r) const
        {
            if (r->IsSelected())
                *r << '<' << mValue << '>' << Select(false);
            else
                *r << ' ' << mValue << ' ' << Select(false);
            *r << mUnits << Color(RGB565(b0b0b0));
        }
    };
    
    class Button : public Sublayout
    {
        const char* mValue;
    
    public:
        Button(char* value) : mValue((char*)value)
        {
        }
        Button(const char* value) : mValue(value)
        {
        }
        virtual void Visit(Render* r) const
        {
            if (!r->IsSelected())
            {
                *r << Color(RGBTRANS, RGB565(808080)) << '<' << mValue << Select(false);
                *r << Color(RGBTRANS, RGB565(808080)) << '>';
            } else
            {
                *r << Color(RGBTRANS, RGB565(ffffff)) << '<' << mValue << Select(false);
                *r << Color(RGBTRANS, RGB565(ffffff)) << '>';
            }
            *r << Color(RGB565(b0b0b0), RGBTRANS);
        }
    };

    class Radio : public Sublayout
    {
        bool mB;
        const char* mName;
    public:
        Radio(bool b, char* name) : mB(b), mName(name)
        {
        }

        virtual void Visit(Render* r) const
        {
            if (!r->IsSelected())
            {
                *r << Color(RGB565(b0b0b0), RGB565(000000)) << ' ' << (mB ? 'X' : 'x') << Select(false);
                *r << Color(RGB565(b0b0b0), RGB565(000000)) << ' ' << Color(RGB565(b0b0b0), RGBTRANS) << mName;
            } else
            {
                *r << Color(RGB565(000000), RGB565(ffffff)) << '<' << (mB ? 'X' : 'x') << Select(false);
                *r << Color(RGB565(000000), RGB565(ffffff)) << '>' << Color(RGB565(ffffff), RGBTRANS) << mName << Color(RGB565(b0b0b0), RGBTRANS);
            }
        }
    };

    class Rectangle : public Sublayout
    {
        int mColor;
    public:
        Rectangle(int color) : mColor(color)
        {
        }
        
        virtual void Visit(Render* r) const
        {
            CRect rcElement(r->GetRectangle());
            BIOS::LCD::Rectangle(rcElement, RGB565(b0b0b0));
            rcElement.Deflate(1, 1, 1, 1);
            GUI::Background(rcElement, mColor, mColor);
            rcElement.Inflate(1, 1, 1, 1);
        }
    };

    class Window : public Sublayout
    {
        const char* mLabel;
        
    public:
        Window(const char* label) : mLabel(label)
        {
        }
        
        void DrawWindow(const CRect& rcRect, uint16_t clr) const
        {
          CRect rcClient = rcRect;
          BIOS::LCD::RoundRect(rcClient, RGB565(000000));
          rcClient.Deflate(2, 2, 2, 2);
          BIOS::LCD::RoundRect(rcClient, clr);
          rcClient.Deflate(2, 14, 2, 2);
          BIOS::LCD::RoundRect(rcClient, RGB565(404040));
        }

        virtual void Visit(Render* r) const
        {
            CRect rcWindow(r->GetRectangle());
            DrawWindow(rcWindow, RGB565(808080));
            BIOS::LCD::Print(rcWindow.left + 16, rcWindow.top+2, RGB565(ffffff), RGBTRANS, mLabel);
            *r << Padding(8, 20, 8, 4);
        }
    };

    class Bar : public Sublayout
    {
        const int mColor;
        
    public:
        Bar(int color) : mColor(color)
        {
        }

        virtual void Visit(Render* r) const
        {
            BIOS::LCD::Bar(r->GetRectangle(), mColor);
        }
    };

    class Background : public Sublayout
    {
    public:
        virtual void Visit(Render* r) const
        {
            GUI::Background(r->GetRectangle(), RGB565(404040), RGB565(101010));
        }
    };
}
