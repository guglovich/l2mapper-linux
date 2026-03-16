#include "L2UIMap.h"

#include "../main.h"
#include "L2UIBusyScreen.h"

L2UIMap::L2UIMap(void)
	: L2UIBaseWidget()
{
	isDraggingMap = false;
	hasDraggedMap = false;
}

L2UIMap::~L2UIMap(void)
{
}

void L2UIMap::Init()
{
	ui_mapWnd = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>("ImageBox", 60, 44, 500, 400, MyGUI::Align::Default, "Overlapped", "MapWnd");
	ui_mapWnd->setNeedMouseFocus(true);
	ui_mapWnd->eventMouseButtonPressed += MyGUI::newDelegate(this, &L2UIMap::onMapMouseDown);
	ui_mapView = ui_mapWnd->createWidget<MyGUI::ScrollView>("ScrollView", 0, 0, 500, 400, MyGUI::Align::Stretch, "MapView");
	//ui_mapView->setAlign(MyGUI::Align::HStretch | MyGUI::Align::VStretch);
	ui_mapView->setVisibleVScroll(false);
	ui_mapView->setVisibleHScroll(false);
	ui_mapView->setCanvasSize(2796, 2650);
	ui_map = ui_mapView->createWidget<MyGUI::ImageBox>("ImageBox", 0, 0, 2796, 2620, MyGUI::Align::Default, "Map");
	ui_map->setProperty("ImageTexture", "map_ct25.jpg");
	ui_mapView->setViewOffset(MyGUI::IntPoint(-1443, -1745));
	ui_mapWnd->eventMouseButtonReleased += MyGUI::newDelegate(this, &L2UIMap::onMapMouseUp);

	char buf[256];
	char *tbuf;
	ui_mapTiles = new MyGUI::ImageBox**[MAP_TILES_X]();
	tileStates = new int16*[MAP_TILES_X]();
	for(int x = 10; x < 30; x++)
	{
		ui_mapTiles[x] = new MyGUI::ImageBox *[MAP_TILES_Y];
		memset(ui_mapTiles[x], 0, MAP_TILES_Y * sizeof(MyGUI::ImageBox*));
		tileStates[x] = new int16[MAP_TILES_Y];
		memset(tileStates[x], 0, MAP_TILES_Y * sizeof(int16));
		for(int y = 10; y < MAP_TILES_Y; y++)
		{
			sprintf(buf, "%d_%d", x, y);
			if(UPackageMgr.GetUPackage(buf))
			{
				float tileWidth = 164;
				float tileHeight = 162;
				MyGUI::ImageBox *ui_mapTile_temp = ui_mapView->createWidget<MyGUI::ImageBox>("ImageBox", 820 + (x - 15) * tileWidth, 2440 + (y - 25) * tileHeight, tileWidth, tileHeight, MyGUI::Align::Default);
				ui_mapTile_temp->setImageTexture("mapTile.png");
				tbuf = new char[3];
				sprintf(tbuf, "%d", x);
				ui_mapTile_temp->setUserString("map_x", tbuf);
				tbuf = new char[3];
				sprintf(tbuf, "%d", y);
				ui_mapTile_temp->setUserString("map_y", tbuf);
				//ui_mapTile_temp->eventMouseButtonClick += MyGUI::newDelegate(this, &L2UIMap::onMapMouseClick);
				ui_mapTiles[x][y] = ui_mapTile_temp;
				setTileState(x, y, 0);
			}
		}
	}

	ui_mapTileLoad = ui_mapWnd->createWidget<MyGUI::Button>("Button", 220, 320, 120, 30, MyGUI::Align::Right | MyGUI::Align::Bottom, "MapTileLoad");
	ui_mapTileLoad->setCaption("Load");
	ui_mapTileLoad->setEnabled(false);
	ui_mapTileLoad->eventMouseButtonPressed += MyGUI::newDelegate(this, &L2UIMap::onMapTileLoadClick2);

	ui_mapTileHide = ui_mapWnd->createWidget<MyGUI::Button>("Button", 355, 320, 120, 30, MyGUI::Align::Right | MyGUI::Align::Bottom, "MapTileHide");
	ui_mapTileHide->setCaption("Hide");
	ui_mapTileHide->setEnabled(false);
	ui_mapTileHide->eventMouseButtonPressed += MyGUI::newDelegate(this, &L2UIMap::onMapTileHideClick2);

	if(ui_mapWnd) ui_mapWnd->setVisible(false);
}

void L2UIMap::update()
{
	if(isDraggingMap)
	{
		if(abs(g_input.getMousePos().x - mapDragStartMousePos.x) > 2
			|| abs(g_input.getMousePos().y - mapDragStartMousePos.y) > 2)
		{
			hasDraggedMap = true;
		}
		ui_mapView->setViewOffset(MyGUI::IntPoint(mapDragStartContentPos.x + (g_input.getMousePos().x - mapDragStartMousePos.x),
													mapDragStartContentPos.y + (g_input.getMousePos().y - mapDragStartMousePos.y)));
	}
}

bool L2UIMap::injectMouseDown(int x, int y, MyGUI::MouseButton btn)
{
	if (!ui_mapWnd || !ui_mapWnd->getVisible()) return false;
	// Ignore clicks on Load/Hide buttons
	if (ui_mapTileLoad && ui_mapTileHide) {
		MyGUI::IntCoord lc = ui_mapTileLoad->getAbsoluteCoord();
		MyGUI::IntCoord hc = ui_mapTileHide->getAbsoluteCoord();
		if ((x >= lc.left && x < lc.left+lc.width && y >= lc.top && y < lc.top+lc.height) ||
		    (x >= hc.left && x < hc.left+hc.width && y >= hc.top && y < hc.top+hc.height))
			return false;
	}
	MyGUI::IntCoord wc = ui_mapWnd->getAbsoluteCoord();
	MyGUI::IntCoord vc = ui_mapView->getCoord();
	MyGUI::IntPoint vo = ui_mapView->getViewOffset();
	int cx = x - wc.left - vc.left - vo.left;
	int cy = y - wc.top  - vc.top  - vo.top;
	fprintf(stderr, "inject cx=%d cy=%d\n", cx, cy); fflush(stderr);
	onMapMouseDown(nullptr, cx, cy, btn);
	return true;
}

bool L2UIMap::injectMouseUp(int x, int y, MyGUI::MouseButton btn)
{
	if (!ui_mapWnd || !ui_mapWnd->getVisible()) return false;
	MyGUI::IntCoord wc = ui_mapWnd->getAbsoluteCoord();
	MyGUI::IntCoord vc = ui_mapView->getCoord();
	MyGUI::IntPoint vo = ui_mapView->getViewOffset();
	int cx = x - wc.left - vc.left - vo.left;
	int cy = y - wc.top  - vc.top  - vo.top;
	onMapMouseUp(nullptr, cx, cy, btn);
	return true;
}

void L2UIMap::setVisible(bool value)
{
	if (ui_mapWnd) {
		MyGUI::LayerManager::getInstance().upLayerItem(ui_mapWnd);
		ui_mapWnd->setVisible(value);
	}
}

void L2UIMap::onTilesLoaded()
{
	// BusyScreen disabled
}

void L2UIMap::onMapMouseDown(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn)
{
	if (!ui_mapWnd || !ui_mapWnd->getVisible()) return;
	fprintf(stderr, "onMapMouseDown x=%d y=%d\n", x, y); fflush(stderr);
	if (ui_mapTiles) {
		int absX = x;
		int absY = y;
		for (int tx = 10; tx < MAP_TILES_X; tx++) {
			if (!ui_mapTiles[tx]) continue;
			for (int ty = 10; ty < MAP_TILES_Y; ty++) {
				MyGUI::ImageBox* tile = ui_mapTiles[tx][ty];
				if (!tile) continue;
				MyGUI::IntCoord coord = tile->getCoord();
				if (absX >= coord.left && absX < coord.left + coord.width &&
				    absY >= coord.top  && absY < coord.top  + coord.height) {
					fprintf(stderr, "TILE tx=%d ty=%d coord=%d,%d+%dx%d\n", tx, ty, coord.left, coord.top, coord.width, coord.height); fflush(stderr);
					onMapMouseClick(tile, btn);
					break;
				}
			}
		}
	}
	if(btn == MyGUI::MouseButton::Left)
	{
		if(!isDraggingMap)
		{
			isDraggingMap = true;
			hasDraggedMap = false;
			mapDragStartMousePos = g_input.getMousePos();
			mapDragStartContentPos.x = ui_mapView->getViewOffset().left;
			mapDragStartContentPos.y = ui_mapView->getViewOffset().top;
		}
	}
}

void L2UIMap::onMapMouseUp(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn)
{
	if (!ui_mapWnd || !ui_mapWnd->getVisible()) return;
	if(btn == MyGUI::MouseButton::Left)
	{
		if(isDraggingMap)
		{
			isDraggingMap = false;

			if(!hasDraggedMap)
			{
				onMapMouseClick(sender, btn);
			}
		}
	}
	else if(btn == MyGUI::MouseButton::Right)
	{
		onMapMouseClick(sender, btn);
	}
}

void L2UIMap::onMapMouseClick(MyGUI::Widget* sender, MyGUI::MouseButton btn)
{
	if(btn == MyGUI::MouseButton::Left)
	{
		if(sender && sender->isType<MyGUI::ImageBox>() && sender != ui_map)
		{
			int16 map_x = atoi(sender->castType<MyGUI::ImageBox>()->getUserStrings().at("map_x").c_str());
			int16 map_y = atoi(sender->castType<MyGUI::ImageBox>()->getUserStrings().at("map_y").c_str());


			int16 tileState = getTileState(map_x, map_y);

			if(tileState & L2UIMTS_SELECTED)
				setTileState(map_x, map_y, tileState & ~L2UIMTS_SELECTED);
			else
				setTileState(map_x, map_y, tileState | L2UIMTS_SELECTED);

			bool nonloadedTileSelected = false;
			bool activeTileSelected = false;

			for(uint16 x = 10; x < MAP_TILES_X; x++)
			{
				for(uint16 y = 10; y < MAP_TILES_Y; y++)
				{
					uint16 tileState = getTileState(x, y);
					if((tileState & L2UIMTS_SELECTED) && !(tileState & L2UIMTS_ACTIVE))
					{
						nonloadedTileSelected = true;
					}
					if((tileState & L2UIMTS_SELECTED) && (tileState & L2UIMTS_ACTIVE))
					{
						activeTileSelected = true;
					}
				}
			}

			if(nonloadedTileSelected)
				ui_mapTileLoad->setEnabled(true);
			else
				ui_mapTileLoad->setEnabled(false);

			if(activeTileSelected)
				ui_mapTileHide->setEnabled(true);
			else
				ui_mapTileHide->setEnabled(false);

			/*g_ui.getL2BusyScreen()->setMessage(L"Загрузка сектора...");
			g_ui.getL2BusyScreen()->setVisible(true);
			DWORD tid;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&loadLevel, sender, 0, &tid);
			setVisible(false);*/
		}
	}
	else if(btn == MyGUI::MouseButton::Right)
	{
		/*if(sender->isType<MyGUI::ImageBox>())
		{
			g_levelMgr.setLevelVisible(atoi(sender->castType<MyGUI::ImageBox>()->getUserStrings().at("map_x").c_str()),
										atoi(sender->castType<MyGUI::ImageBox>()->getUserStrings().at("map_y").c_str()),
										false);
		}*/
	}
}

void L2UIMap::onMapWindowClose(MyGUI::Widget* sender, std::string_view evt)
{
	setVisible(false);
}

void L2UIMap::onMapTileLoadClick2(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn)
{
	onMapTileLoadClick(sender);
}
void L2UIMap::onMapTileHideClick2(MyGUI::Widget* sender, int x, int y, MyGUI::MouseButton btn)
{
	onMapTileHideClick(sender);
}
void L2UIMap::onMapTileLoadClick(MyGUI::Widget* sender)
{
	jfArray<L2MapTileInfo*, uint16> *tiles = new jfArray<L2MapTileInfo*, uint16>();
	for(uint16 x = 10; x < MAP_TILES_X; x++)
	{
		for(uint16 y = 10; y < MAP_TILES_Y; y++)
		{
			uint16 tileState = getTileState(x, y);
			if((tileState & L2UIMTS_SELECTED) && !(getTileState(x, y) & L2UIMTS_ACTIVE))
			{
				L2MapTileInfo *tileInfo = new L2MapTileInfo();
				tileInfo->map_x = x;
				tileInfo->map_y = y;
				tiles->add(tileInfo);
				g_geo.Load(x, y);
				setTileState(x, y, tileState | L2UIMTS_LOADED | L2UIMTS_ACTIVE);
			}
			setTileState(x, y, getTileState(x, y) & ~L2UIMTS_SELECTED);
		}
	}

	ui_mapTileLoad->setEnabled(false);
	ui_mapTileHide->setEnabled(false);

	// BusyScreen disabled until fully implemented
	//setVisible(false);
	g_levelMgr.loadTiles(tiles);
}

void L2UIMap::onMapTileHideClick(MyGUI::Widget* sender)
{
	//jfArray<L2MapTileInfo*, uint16> *tiles = new jfArray<L2MapTileInfo*, uint16>();
	for(uint16 x = 10; x < MAP_TILES_X; x++)
	{
		for(uint16 y = 10; y < MAP_TILES_Y; y++)
		{
			uint16 tileState = getTileState(x, y);
			if((tileState & L2UIMTS_SELECTED) && (getTileState(x, y) & L2UIMTS_ACTIVE))
			{
				g_levelMgr.setLevelVisible(x, y, false);
				setTileState(x, y, tileState & ~L2UIMTS_ACTIVE);
			}
			setTileState(x, y, getTileState(x, y) & ~L2UIMTS_SELECTED);
		}
	}

	ui_mapTileLoad->setEnabled(false);
	ui_mapTileHide->setEnabled(false);
}

int16 L2UIMap::getTileState(int x, int y)
{
	return tileStates[x][y];
}

void L2UIMap::setTileState(int x, int y, int16 state)
{
	tileStates[x][y] = state;

	if(!ui_mapTiles[x][y])
		return;

	if(state & L2UIMTS_ACTIVE)
	{
		if(state & L2UIMTS_SELECTED)
		{
			ui_mapTiles[x][y]->setImageTexture("mapTile_selected.png");
		}
		else
		{
			ui_mapTiles[x][y]->setImageTexture("mapTile_active.png");
		}
	}
	else if(state & L2UIMTS_LOADED)
	{
		if(state & L2UIMTS_SELECTED)
		{
			ui_mapTiles[x][y]->setImageTexture("mapTile_selected.png");
		}
		else
		{
			ui_mapTiles[x][y]->setImageTexture("mapTile_loaded.png");
		}
	}
	else
	{
		if(state & L2UIMTS_SELECTED)
		{
			ui_mapTiles[x][y]->setImageTexture("mapTile_selected.png");
		}
		else
		{
			ui_mapTiles[x][y]->setImageTexture("mapTile.png");
		}
	}
}
