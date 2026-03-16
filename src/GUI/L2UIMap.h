#pragma once
#include "L2UIBaseWidget.h"

enum L2UIMapTileState
{
    L2UIMTS_SELECTED = 0x0001,
    L2UIMTS_LOADED   = 0x0002,
    L2UIMTS_ACTIVE   = 0x0004
};

class L2UIMap : public L2UIBaseWidget
{
public:
    virtual void Init();
    virtual void update();
    virtual bool injectMouseDown(int x, int y, MyGUI::MouseButton btn);
    virtual bool injectMouseUp(int x, int y, MyGUI::MouseButton btn);
    virtual void setVisible(bool value);
    bool isVisible() { return ui_mapWnd && ui_mapWnd->getVisible(); }
    void onTilesLoaded();

    void onMapMouseDown(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn);
    void onMapMouseUp(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn);
    void onMapMouseClick(MyGUI::Widget* sender, MyGUI::MouseButton btn);
    // Window* → Widget* — MyGUI 3.4, и сигнатура string_view уже в .cpp
    void onMapWindowClose(MyGUI::Widget* sender, std::string_view evt);
    void onMapTileLoadClick(MyGUI::Widget* sender);
    void onMapTileHideClick(MyGUI::Widget* sender);
    void onMapTileLoadClick2(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn);
    void onMapTileHideClick2(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn);

    int16  getTileState(int x, int y);
    void   setTileState(int x, int y, int16 state);

public:
    static const uint16 MAP_TILES_X = 30;
    static const uint16 MAP_TILES_Y = 30;

protected:
    MyGUI::Widget*     ui_mapWnd;    // Window* → Widget* (нет скина Window в теме)
    MyGUI::ScrollView* ui_mapView;
    MyGUI::ImageBox*   ui_map;
    MyGUI::ImageBox*** ui_mapTiles;
    int16**            tileStates;
    MyGUI::Button*     ui_mapTileLoad;
    MyGUI::Button*     ui_mapTileHide;

    bool     isDraggingMap;
    bool     hasDraggedMap;
    _vector2 mapDragStartMousePos;
    _vector2 mapDragStartContentPos;

public:
    L2UIMap(void);
    ~L2UIMap(void);
};
