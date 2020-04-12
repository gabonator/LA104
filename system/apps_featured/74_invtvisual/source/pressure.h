
#include "graph/palette.h"
#include "graph/draw.h"
#include "graph/graph.h"
#include "graph/series.h"
#include "graph/axis.h"

class CXAxisTime : public CXAxis
{
    const char* units{""};
    int multiply{1};
    
protected:
    virtual void FormatNumber(char* buf, int i)
    {
        sprintf(buf, "%d%s", i, units);
    }
    
    virtual int reconstruct(int i)
    {
        return i*multiply;
    }

public:
    void SelectUnits(int& value)
    {
        multiply = 1000;
        value /= 1000;
        
        // seconds;
        if (value < 3)
            return;
        
        units = "s";
        if (value < 2*60)
            return;
        
        // minutes
        units = "m";
        multiply *= 60;
        value /= 60;
        if (value < 2*60)
            return;

        // hours
        units = "h";
        multiply *= 60;
        value /= 60;
    }

    virtual void Update(int value)
    {
        int simplifiedValue = value;
        SelectUnits(simplifiedValue);
        Redraw(simplifiedValue, value);
    }
};

class CYAxisPressure : public CYAxis
{
protected:
    virtual void FormatNumber(char* buf, int i)
    {
		sprintf(buf, "%d.%02d", i/100, i%100);
    }
};

class CPressureGraph : public CWnd
{
	static constexpr int mSeriesCount{3};
	
    CGraph<mSeriesCount> mWndGraph;
    CXAxisTime mXAxis;
    CYAxisPressure mYAxis;

	CSimpleSeries mSeriesData[mSeriesCount];
    CArray<CSimpleSeries> mSeries;
	
public:
	void Create(CRect rcWindow, CWnd* pParent)
	{
        mSeries.Init(mSeriesData, COUNT(mSeriesData));
        mSeries.SetSize(COUNT(mSeriesData));
		
        CWnd::Create("graph container", CWnd::WsVisible, rcWindow, pParent);
        CRect rcGraph(rcWindow.left+4+40, rcWindow.top+20, rcWindow.right-4, rcWindow.bottom-16+4-8);

        mWndGraph.Create("graph", CWnd::WsVisible, rcGraph, this);
        for (int i=0; i<mSeries.GetSize(); i++)
            mWndGraph.AddSeries(&mSeries[i]);

        CRect rcXAxis(rcWindow.left+4+40, rcWindow.bottom-16+4-8, rcWindow.right-4, rcWindow.bottom+4-8);
        mXAxis.Create("xaxis", CWnd::WsVisible, rcXAxis, this);

        CRect rcYAxis(rcWindow.left+4, rcWindow.top+20, rcWindow.left+4+40, rcWindow.bottom-16+4-8);
        mYAxis.Create("yaxis", CWnd::WsVisible, rcYAxis, this);
	}

	virtual void OnPaint()
	{
        MyGui::Window(m_rcClient, MyGui::TitleColor);
		
		const char* title = "PID Pressure chart";
		BIOS::LCD::Print(m_rcClient.CenterX() - strlen(title)*4, m_rcClient.top+2, RGB565(404040), RGBTRANS, title);

        mWndGraph.SetRange(Range{0, 550});
        mYAxis.Update(mWndGraph.GetRange());
        mXAxis.Update(5000);
	}

	void Update(int a, int b, int c)
	{
		static int counter = 0;
		counter++;

		mSeries[0] << a;
		mSeries[1] << b;
		mSeries[2] << c;

		bool needRedraw = mWndGraph.Redraw();

		if (needRedraw || (counter > 5 && mSeries[0].GetLength() > 5))
		{
			mYAxis.Update(mWndGraph.GetRange());
			
			static int lastestimated = 0;

			counter = 0;
			int duration = mSeries[0].GetTotalDuration();
			int estimated = 0;
			if (mSeries[0].GetLength() > 5)
				estimated = duration * mSeries[0].GetMaxLength() / mSeries[0].GetLength();
			
			if (estimated < 5000)
				estimated = 5000;
			
			if (abs(estimated - lastestimated) < estimated / 50)
				return;
			
			lastestimated = estimated;
			mXAxis.Update(estimated);
		}
	}

};

