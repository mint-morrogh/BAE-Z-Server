class DogSelectMenu extends UIScriptedMenu
{
    private bool                 m_Initialized;
    private bool                 m_IsMenuOpen;

    //---Elements from .layout file
    private ScrollWidget 		 m_Scroller; //Btw the m_ thingy is just a bat habbit i picked up from DayZ devs (ಠ¿ಠ)
    private GridSpacerWidget     m_Grid;
    private TextWidget           m_Title;
    private Widget               m_pnl_bg;
    private ButtonWidget         m_BtnAddChild;
    private ButtonWidget         m_BtnRemoveChild;
	private ButtonWidget         m_BtnSpawnDog;
	private ButtonWidget         m_BtnSpawnRandomDog;
	private TextListboxWidget	m_DogList;
	private ItemPreviewWidget	m_dogpreview;
	protected EntityAI 			previewDog;
	protected ref map<string, string> member_sorting_map;
	//---

    void DogSelectMenu()
    {
       /*Do something when this class is created. This is the constructor, it is the very firt thing called out of all other methods.
    		More info on Enforce syntax and functionality here:  https://community.bistudio.com/wiki/DayZ:Enforce_Script_Syntax
    	*/
		member_sorting_map = new map<string, string> ();
    }

    /*
		This is the destructor, called when this class is deleted / destroyed
    */
    void ~DogSelectMenu() 
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
		
		if ( previewDog ) 
		{
			GetGame().ObjectDelete( previewDog );
		}

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
		if ( previewDog ) 
		{
			GetGame().ObjectDelete( previewDog );
		}
    }

    override Widget Init()
    {
        if (!m_Initialized) //Only draw and init widgets if not already done that, since this function is called each time you do ( ShowScriptedMenu() )
        {
            layoutRoot = GetGame().GetWorkspace().CreateWidgets( "Dayz_Dog/GUI/Layouts/DogSelectMenu.layout" ); //Create custom widget / .layout
            
            //Define elements from .layout ( you must cast each element to its according script class if you wish to use its functions see scripts\1_Core\proto\EnWidgets.c )
            m_Scroller = ScrollWidget.Cast( layoutRoot.FindAnyWidget( "Scroller" ) ); //Find the widget by its name that is defined in .layout
			m_Grid     = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "Grid" ) );
			m_Title	   = TextWidget.Cast( layoutRoot.FindAnyWidget( "Title" ) );
			//m_pnl_bg   = layoutRoot.FindAnyWidget( "pnl_bg" );
			m_DogList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "DogList" ) );
			//m_dogpreview   = ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget( "DogPreview" ));
			m_BtnAddChild    = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnAddChild" ) );
			m_BtnRemoveChild = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BtnRemoveChild" ) );
			m_BtnSpawnDog = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "SpawnButton" ) );
			m_BtnSpawnRandomDog = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "SpawnRandomButton" ) );
			//Widget preview_frame = layoutRoot.FindAnyWidget("DogPreview");
			//m_dogpreview = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, preview_frame) );
			//Print("dog preview " + m_dogpreview);

            //WidgetEventHandler.GetInstance().RegisterOnDoubleClick( m_Grid, this, "OnDoubleClicked" ); //Attach a double-click event on a specifc widget
            //WidgetEventHandler.GetInstance().RegisterOnDoubleClick( m_Scroller, this, "OnDoubleClicked" );

            m_Initialized = true;
        }
		UpdateDogs();
		m_DogList.SelectRow(0);
		UpdateDogPreview("DogPreview1");
        return layoutRoot;
    }
	
	void UpdateDogPreview(string dogType)
	{
		
		if ( !m_dogpreview )
			{
				Widget preview_frame = layoutRoot.FindAnyWidget("DogPreview");
				
				if ( preview_frame ) 
				{
					float width;
					float height;
					preview_frame.GetSize(width, height);

					m_dogpreview = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, preview_frame) );
				}
				
			}
		
			//m_dogpreview = ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget("DogPreview"));
			if ( previewDog )
				GetGame().ObjectDelete( previewDog );

			previewDog = EntityAI.Cast(GetGame().CreateObject( dogType, vector.Zero, true, false, true ));

			m_dogpreview.SetItem( previewDog );
			m_dogpreview.SetModelPosition( Vector(0,0,3.5) );

			float itemx, itemy;		
			m_dogpreview.GetPos(itemx, itemy);

			m_dogpreview.SetSize( 1.5, 1.5 );

			m_dogpreview.SetPos( -0.225, -0.225 );

	}
	
	void UpdateDogs()
	{
		if (m_Initialized)
		{
			UpdateDogList();
		}	
	}
	// THX Shana Mod Party to understand these UI things :-)
	void UpdateDogList() 
	{
		member_sorting_map.Clear ();
		string member_name;
		int insert_row = 0;

		auto members = new array<string> ();

        for (int i = 0; i < NUM_OF_DOGS; ++i) {
			if (i<9)
			{
				members.Insert ("#DD_dogtype0"+ (i+1).ToString());
			} else {
				members.Insert ("#DD_dogtype"+ (i+1).ToString());
			}
            
        }
		
		foreach (auto id : members) {
			member_name = "doggo";
			member_name.ToLower ();
			member_sorting_map.Insert (member_name + id, id);
		}

		auto sorted_keys = member_sorting_map.GetKeyArray ();
		sorted_keys.Sort ();
		foreach (auto sorted_key : sorted_keys) {
			string member_id = member_sorting_map.Get (sorted_key);
			member_name = member_sorting_map.Get(sorted_key) ;
			if (insert_row < m_DogList.GetNumItems ()) {
				m_DogList.SetItem (insert_row, member_name, new Param1<string> (member_id), 0);
			} else {
				m_DogList.AddItem (member_name, new Param1<string> (member_id), 0);
			}
			insert_row++;
		}

		while (insert_row < m_DogList.GetNumItems ()) {
			m_DogList.SelectRow (-1);
			m_DogList.RemoveRow (insert_row);
		}

		if (m_DogList.GetSelectedRow () >= insert_row) {
			m_DogList.SelectRow (insert_row - 1);
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
			case m_DogList:
				//kliknuti na seznam
				//UpdateDogPreview("Animal_CanisLupus_Grey");
				//UpdateDogPreview("Bone");
				//UpdateDogPreview("DogPreview1");
				local int row_index = m_DogList.GetSelectedRow();
				UpdateDogPreview("DogPreview"+(row_index+1).ToString());
				//Print("CLICKED");
			break;
			case m_BtnSpawnDog:
				//Print("CLICKED SPAWN");
				//potrebuji vektor pozice z akce a cislo psa
				//pak poslat rpc
				local int dognum = m_DogList.GetSelectedRow();
				GetRPCManager().SendRPC( "Dayz_Dog", "SpawnDog", new Param2< int, vector >( dognum+1, player.GetDogSpawnPos() ) );
				Close();
			break;
			case m_BtnSpawnRandomDog:
				int rndnum = Math.RandomIntInclusive(1, NUM_OF_DOGS);
				GetRPCManager().SendRPC( "Dayz_Dog", "SpawnDog", new Param2< int, vector >( rndnum, player.GetDogSpawnPos() ) );
				Close();
				///GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Random dog " + rndnum.ToString(), ""));
				//Print("CLICKED SPAWN RANDOM " + player.GetDogLastPos());
			break;
    	}
        return super.OnClick(w, x, y, button);
    }

    //Called by WidgetEventHandler that is registered at line 70 ( more of these examples can be found in vanilla scripts / menus creates by devs )
    void OnDoubleClicked(Widget w, int x, int y, int button) 
    {
        if (button == MouseState.LEFT)
        {
            //Do something
			        }
        else if (button == MouseState.RIGHT)
        {
            //Do something
        }
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
	
};