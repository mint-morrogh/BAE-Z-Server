class DogManageMenu extends UIScriptedMenu
{
    private bool                 m_Initialized;
    private bool                 m_IsMenuOpen;

    //---Elements from .layout file
    private ScrollWidget 		 m_Scroller; //Btw the m_ thingy is just a bat habbit i picked up from DayZ devs (ಠ¿ಠ)
	private TextWidget		 m_DogName;
	private EditBoxWidget		 m_DogNameEdit;
    //private ButtonWidget         m_BtnAddChild;
    private ButtonWidget         m_BtnNameDog;
	private ButtonWidget         m_BtnExit;
	private ItemPreviewWidget	m_dogpreview;
	protected EntityAI 			previewDog;

	private TextWidget		 m_StatDogHealth;
	private TextWidget		 m_StatZedsKilled;
	private TextWidget		 m_StatAnimalsKilled;
	private TextWidget		 m_StatCaughtChickens;
	private TextWidget		 m_StatSurvivorBitten;
	//---

    void DogManageMenu()
    {
       /*Do something when this class is created. This is the constructor, it is the very firt thing called out of all other methods.
    		More info on Enforce syntax and functionality here:  https://community.bistudio.com/wiki/DayZ:Enforce_Script_Syntax
    	*/
		
    }

    /*
		This is the destructor, called when this class is deleted / destroyed
    */
    void ~DogManageMenu() 
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
            layoutRoot = GetGame().GetWorkspace().CreateWidgets( "Dayz_Dog/GUI/Layouts/DogManageMenu.layout" ); //Create custom widget / .layout
            m_DogNameEdit = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "EditDogName" ) );
			m_DogName = TextWidget.Cast( layoutRoot.FindAnyWidget( "DogName" ) );
            m_BtnNameDog = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "Btn_NameDog" ) );
			m_BtnExit = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "Btn_Exit" ) );
			
			m_StatDogHealth = TextWidget.Cast( layoutRoot.FindAnyWidget( "DogHealth" ) );
			m_StatZedsKilled = TextWidget.Cast( layoutRoot.FindAnyWidget( "StatZedsKills" ) );
			m_StatAnimalsKilled = TextWidget.Cast( layoutRoot.FindAnyWidget( "StatAnimalsKills" ) );
			m_StatCaughtChickens = TextWidget.Cast( layoutRoot.FindAnyWidget( "StatChickenCaughts" ) );
			m_StatSurvivorBitten = TextWidget.Cast( layoutRoot.FindAnyWidget( "StatSurvivorBitten" ) );
			
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			DogPreview_Base dogpr = DogPreview_Base.Cast(player.GetDogSlot());
			if (dogpr.GetDogName())
			{
				m_DogName.SetText(dogpr.GetDogName());
				m_DogNameEdit.SetText(dogpr.GetDogName());
				m_DogName.Show(true);
				m_DogNameEdit.Show(false);
			}
			else
			{
				m_DogName.Show(false);
				m_DogNameEdit.Show(true);
			}
            
			m_Initialized = true;
        }
		UpdateDogPreview();
		ShowDogStats();
        return layoutRoot;
    }
	
	void ShowDogStats()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		DogPreview_Base dogpr = DogPreview_Base.Cast(player.GetDogSlot());
		m_StatDogHealth.SetText("Health: " + dogpr.GetDogHealth().ToString());
		m_StatZedsKilled.SetText("Killed Zeds: " + dogpr.GetZedKills().ToString());
		m_StatAnimalsKilled.SetText("Kllled Animals: " + dogpr.GetAnimalKills().ToString());
		m_StatCaughtChickens.SetText("Caught Chickens: " + dogpr.GetChickenKills().ToString());
		m_StatSurvivorBitten.SetText("Bitten Survivors: " + dogpr.GetSurvivorHits().ToString());
	}
	
	void UpdateDogPreview()
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
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			previewDog = player.GetMyDog();

			m_dogpreview.SetItem( previewDog );
			m_dogpreview.SetModelPosition( Vector(0,0,3.5) );

			float itemx, itemy;		
			m_dogpreview.GetPos(itemx, itemy);

			m_dogpreview.SetSize( 1.5, 1.5 );

			m_dogpreview.SetPos( -0.225, -0.225 );

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
			case m_BtnNameDog:
				//Print("CLICKED NAME");
				//poslat rpc
				DogPreview_Base dogpr = DogPreview_Base.Cast(player.GetDogSlot());
				dogpr.SetDogName(m_DogNameEdit.GetText());
				
			
				GetRPCManager().SendRPC( "Dayz_Dog", "SetNameDog", new Param1< string >( m_DogNameEdit.GetText() ) );
				//Close();
			break;
			
			case m_BtnExit:
				Close();
			break;
		}
        return super.OnClick(w, x, y, button);
    }

    //Called by WidgetEventHandler that is registered at line 70 ( more of these examples can be found in vanilla scripts / menus creates by devs )
    void OnDoubleClicked(Widget w, int x, int y, int button) 
    {
		if (button == MouseState.LEFT )
        {
            //Do something
			/*
			m_MapWidget.AddUserMark(GetMapClickPos(), "Test", ARGB(255,255,255,0), "dayz_dog\\GUI\\Imagesets\\dog_feet_small.paa");
			m_ClientWaypoints = GetDayZGame().GetDogWayPoints();
			string message = GetMapClickPos()[0].ToString() + " " + (GetGame().SurfaceY(GetMapClickPos()[0], GetMapClickPos()[2]) + 0.4).ToString() + " " + GetMapClickPos()[2].ToString();
			m_ClientWaypoints.WayPoints.Insert(new DoggoWaypoint("Testing", message));
			DayzDogWaypointsConfig.SaveConfig("waypoints.json", m_ClientWaypoints);
			*/
			
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