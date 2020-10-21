class CMenuMain : public CTopMenu
{
public:
    virtual TItem GetItem(int i)
    {
        switch (i)
        {
            case 0: return TItem{"RFID tool", TItem::Static};
            case 1: return TItem{"Scan", TItem::Default};
            case 2: return TItem{"Read", TItem::Default};
            case 3: return TItem{"Write", TItem::Default};
            case 4: return TItem{"Sniff", TItem::Default};
            default: return TItem{nullptr, TItem::None};
        }
    }
};
