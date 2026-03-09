class DogMapMenu extends UIScriptedMenu
{
    private bool                 m_Initialized;
    private bool                 m_IsMenuOpen;

    //---Elements from .layout file
    private ScrollWidget 		 m_Scroller; //Btw the m_ thingy is just a bat habbit i picked up from DayZ devs (ಠ¿ಠ)
    private GridSpacerWidget     m_Grid;
    private TextWidget           m_Title;
    private Widget               m_pnl_bg;
    //private ButtonWidget         m_BtnAddChild;
    private ButtonWidget         m_BtnGo;
	private TextListboxWidget	m_WayPointsList;
	MapWidget m_MapWidget;
	private Widget m_PanelEditDialog;
    ImageWidget m_MapFakeBg;
	protected ref DogWaypointEditDialog m_EditDialog;
	private ref DoggoWaypoints m_ClientWaypoints;
	protected ref map<string, string> member_sorting_map;
	private vector m_MapClickPos;
	//---

    void DogMapMenu()
    {
       /*Do something when this class is created. This is the constructor, it is the very firt thing called out of all other methods.
    		More info on Enforce syntax and functionality here:  https://community.bistudio.com/wiki/DayZ:Enforce_Script_Syntax
    	*/
		member_sorting_map = new map<string, string> ();
		m_ClientWaypoints = GetDayZGame().GetDogWayPoints();
		DayzDogWaypointsConfig.LoadConfig("waypoints.json", m_ClientWaypoints);
		
    }

    /*
		This is the destructor, called when this class is deleted / destroyed
    */
    void ~DogMapMenu() 
    {
        PPEffects.SetBlurMenu( 0 );
        GetGame().GetUIManager().Back();
        g_Game.GetUIManager().ShowCursor(true);
        g_Game.GetUIManager().ShowUICursor(false);
        GetGame().GetInput().ResetGameFocus(); //1.18
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		player.GetInputController().SetDisabled(false);
        //GetGame().GetMission().PlayerControlEnable(false);
        //GetGame().GetMission().GetHud().Show( true ); //Show
		
        //Destroy root widget and all its children
        if ( layoutRoot )
        	layoutRoot.Unlink();
    }

    //after show
    override void OnShow()
    {
        super.OnShow();
        PPEffects.SetBlurMenu( 0.5 ); //Add blurr effect
		//GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL); //1.18
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		player.GetInputController().SetDisabled(true);
		
        GetGame().GetUIManager().ShowUICursor( true );
        //GetGame().GetMission().GetHud().Show( false );
    }

    //after hide
    override void OnHide()
    {
        super.OnHide();
        PPEffects.SetBlurMenu( 0 ); //Remove blurr effect

        //Unlock controls, this also happens in missionGameplay.c however including it here will assure control is gained back incase that method is not invoked. 
        //That can occur when other mods / scripts force a menu on screen illegally 
        g_Game.GetUIManager().ShowCursor(true);
        g_Game.GetUIManager().ShowUICursor(false);
        GetGame().GetInput().ResetGameFocus();
        //GetGame().GetMission().PlayerControlEnable(false);
        //GetGame().GetMission().GetHud().Show( true ); //Show
    }

    override Widget Init()
    {
        if (!m_Initialized) //Only draw and init widgets if not already done that, since this function is called each time you do ( ShowScriptedMenu() )
        {
            layoutRoot = GetGame().GetWorkspace().CreateWidgets( "Dayz_Dog/GUI/Layouts/DogMapMenu.layout" ); //Create custom widget / .layout
            
            //Define elements from .layout ( you must cast each element to its according script class if you wish to use its functions see scripts\1_Core\proto\EnWidgets.c )
            m_Scroller = ScrollWidget.Cast( layoutRoot.FindAnyWidget( "Scroller" ) ); //Find the widget by its name that is defined in .layout
			m_Grid     = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "Grid" ) );
			m_Title	   = TextWidget.Cast( layoutRoot.FindAnyWidget( "Title" ) );
			//m_pnl_bg   = layoutRoot.FindAnyWidget( "pnl_bg" );
			m_MapWidget = MapWidget.Cast( layoutRoot.FindAnyWidget( "MapWidget" ) );
			
			m_PanelEditDialog = layoutRoot.FindAnyWidget( "panel_editdialog" );
            m_MapFakeBg = ImageWidget.Cast(m_PanelEditDialog.FindAnyWidget("map_fakebg"));
			
			m_WayPointsList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "WayPoints_List" ) );
			//m_dogpreview   = ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget( "DogPreview" ));
			m_BtnGo    = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "GO_Button" ) );
			//m_BtnRemoveChild = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnRemoveChild" ) );
			//m_BtnSpawnDog = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "SpawnButton" ) );
			//m_BtnSpawnRandomDog = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "SpawnRandomButton" ) );
			//Widget preview_frame = layoutRoot.FindAnyWidget("DogPreview");
			//m_dogpreview = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, preview_frame) );
			//Print("dog preview " + m_dogpreview);

            WidgetEventHandler.GetInstance().RegisterOnDoubleClick( m_MapWidget, this, "OnDoubleClicked" ); //Attach a double-click event on a specifc widget
            //WidgetEventHandler.GetInstance().RegisterOnDoubleClick( m_Scroller, this, "OnDoubleClicked" );

            m_Initialized = true;
        }
		
		UpdateWaypoints();
		m_WayPointsList.SelectRow(0);

        return layoutRoot;
    }
	
	void UpdateWaypoints()
	{
		if (m_Initialized)
		{
			UpdateWaypointsList();
		}	
	}
	
	void UpdateWaypointsList() 
	{
		member_sorting_map.Clear ();
		string member_name;
		int insert_row = 0;

		auto members = new array<string> ();
		m_MapWidget.ClearUserMarks();
        for (int i = 0; i < m_ClientWaypoints.WayPoints.Count() ; ++i) {

			members.Insert (  m_ClientWaypoints.WayPoints.Get(i).name );
			//Print ("XXXX " + m_ClientWaypoints.WayPoints.Get(i).pos.ToVector() + " X "+ m_ClientWaypoints.WayPoints.Get(i).name);
			
			m_MapWidget.AddUserMark(m_ClientWaypoints.WayPoints.Get(i).pos.ToVector(), m_ClientWaypoints.WayPoints.Get(i).name, ARGB(255,255,255,0), "dayz_dog\\GUI\\Imagesets\\dog_feet_small.paa");
            
        }
		
		foreach (auto id : members) {
			member_name = "waypoint";
			member_name.ToLower ();
			member_sorting_map.Insert (member_name + id, id);
		}
		
		auto sorted_keys = member_sorting_map.GetKeyArray ();
		sorted_keys.Sort ();
		//Print("--------");
		foreach (auto sorted_key : sorted_keys) {
			string member_id = member_sorting_map.Get (sorted_key);
			member_name = member_sorting_map.Get(sorted_key) ;
			//Print("NAME " + member_name + " sorted key " + sorted_key);
			if (insert_row < m_WayPointsList.GetNumItems ()) {
				m_WayPointsList.SetItem (insert_row, member_name, new Param1<string> (member_id), 0);
			} else {
				m_WayPointsList.AddItem (member_name, new Param1<string> (member_id), 0);
			}
			insert_row++;
		}

		while (insert_row < m_WayPointsList.GetNumItems ()) {
			m_WayPointsList.SelectRow (-1);
			m_WayPointsList.RemoveRow (insert_row);
		}

		if (m_WayPointsList.GetSelectedRow () >= insert_row) {
			m_WayPointsList.SelectRow (insert_row - 1);
		}
	}

	
    bool ShouldShowScrollBar()
    {
        float x, y;
        //m_pnl_bg.GetScreenSize( x, y );
        return m_Scroller.GetContentHeight() > y;
    }

    //Called on each frame, this will be paused when the menu is hidden or missiongameplay callqueue is frozen / paused
    override void Update(float timeslice)
    {
        super.Update(timeslice);
		
		if( GetGame() && GetGame().GetInput() && GetGame().GetInput().LocalPress("UAUIBack", false) )
		{
			Close();
		}

		/*
        if ( ShouldShowScrollBar() )
            m_Scroller.SetAlpha(1.0);
        else
            m_Scroller.SetAlpha(0.0);
		*/
    }

    //Click event triggers when you click on a widget, this method wont get called if your widget has the "IgnorePointer" property toggled in your .layout
    override bool OnClick(Widget w, int x, int y, int button)
    {
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		
		switch(w)
    	{
			case m_WayPointsList:
			//clicked to list
				local int row_index = m_WayPointsList.GetSelectedRow();
				string text;
				if (button == MouseState.RIGHT)
				{
					if (row_index > -1)
					{
						//spatne
						m_ClientWaypoints.WayPoints.Remove(GetElementNum(row_index)); //odebrat spravnej
						DayzDogWaypointsConfig.SaveConfig("waypoints.json", m_ClientWaypoints);
						UpdateWaypointsList();
					}
					
				}
			
			break;
			
			case m_BtnGo:
				local int row_index2 = m_WayPointsList.GetSelectedRow();
				if (button == MouseState.LEFT)
				{
					//select waypoint
					if (row_index2 > -1)
					{	
						vector target =	m_ClientWaypoints.WayPoints.Get(GetElementNum(row_index2)).pos.ToVector();
						Print ("target " + target + " row " + row_index2 + " element " + GetElementNum(row_index2) );
						GetRPCManager().SendRPC( "Dayz_Dog", "SetDogWaypoint", new Param1< vector >( target ) );
						Close();
					}	
				}
			break;

    	}
        return super.OnClick(w, x, y, button);
    }

    //Called by WidgetEventHandler that is registered at line 70 ( more of these examples can be found in vanilla scripts / menus creates by devs )
    void OnDoubleClicked(Widget w, int x, int y, int button) 
    {
		if (button == MouseState.LEFT && w == m_MapWidget)
        {
            //Do something
			/*
			m_MapWidget.AddUserMark(GetMapClickPos(), "Test", ARGB(255,255,255,0), "dayz_dog\\GUI\\Imagesets\\dog_feet_small.paa");
			m_ClientWaypoints = GetDayZGame().GetDogWayPoints();
			string message = GetMapClickPos()[0].ToString() + " " + (GetGame().SurfaceY(GetMapClickPos()[0], GetMapClickPos()[2]) + 0.4).ToString() + " " + GetMapClickPos()[2].ToString();
			m_ClientWaypoints.WayPoints.Insert(new DoggoWaypoint("Testing", message));
			DayzDogWaypointsConfig.SaveConfig("waypoints.json", m_ClientWaypoints);
			*/
			m_MapClickPos = GetMapClickPos();
			ShowDialog(1,true);
		}
    }
	
	// Thx DaOne(Mark)
	vector GetMapClickPos() 
	{
        vector world_pos,ScreenToMap,dir,from,to;

        dir = GetGame().GetPointerDirection();
        from = GetGame().GetCurrentCameraPosition();
        to = from + ( dir * 1000 );

        world_pos = GetGame().GetScreenPos( to );
        ScreenToMap = m_MapWidget.ScreenToMap( world_pos );
        return ScreenToMap;
    }

    bool IsMenuOpen() 
    {
        return m_IsMenuOpen;
    }

    void SetMenuOpen(bool isMenuOpen) 
    {
        m_IsMenuOpen = isMenuOpen;
        if (m_IsMenuOpen)
            PPEffects.SetBlurMenu( 0.5 );
        else
            PPEffects.SetBlurMenu( 0 );
    }
	
	void ShowDialog(int markerIndex, bool IsNew ) 
	{

        if (!m_EditDialog)
		{
        	m_EditDialog = new DogWaypointEditDialog(this, m_PanelEditDialog, m_MapClickPos );
		}
		//m_MapWidget.Show(false);
        //m_MapFakeBg.Show(true);
		m_EditDialog.SetMapClickPos(m_MapClickPos);
        m_PanelEditDialog.Show(true);
      
    }
	
	void HideDialog()
	{
        if (m_EditDialog) {
            m_PanelEditDialog.Show(false);
	
            //m_MapWidget.Show(true);
            //m_MapFakeBg.Show(false);
        }
    }
	MapWidget GetMapWidget()
	{
		return m_MapWidget;
	}
	int GetElementNum(int row)
	{
		int num = 0;
		string text;
		m_WayPointsList.GetItemText(row,0,text);
		for (int i = 0; i < m_ClientWaypoints.WayPoints.Count() ; ++i)
		{
			if (m_ClientWaypoints.WayPoints.Get(i).name == text )
			{
				num = i;
			}
		}
		
		return num;
	}
};